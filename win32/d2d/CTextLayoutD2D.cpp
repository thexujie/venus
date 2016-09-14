#include "stdafx.h"
#include "CTextLayoutD2D.h"

VENUS_BEG

CTextLayoutD2D::CTextLayoutD2D(C2DDeviceD2D * pEngine) :
	m_pEngine(pEngine), m_pSource(nullptr), m_pLayout(nullptr)
{
	if(!pEngine)
		throw exp_bad_state();
}

CTextLayoutD2D::~CTextLayoutD2D()
{
	SafeRelease(m_pLayout);
}

err_t CTextLayoutD2D::Initialize(IDocSource * pSrouce)
{
	m_pSource = pSrouce;
	return err_ok;
}

IDocSource * CTextLayoutD2D::GetSource() const
{
	return m_pSource;
}

err_t CTextLayoutD2D::Layout(trange_t range, int_x iWidth, paragraph_tag_e tag)
{
	//log2(L"index = %i, length = %i", range.index, range.length);
	SafeRelease(m_pLayout);
	m_clusters.clear();
	m_lines.clear();

	if(!m_pEngine || !m_pSource)
		return err_not_initialized;

	doc_source_t source = m_pSource->GetDocSource();
	textformat_t format = m_pSource->GetDefFormat();

	if(range.index < 0 || range.index + range.length > source.length)
		return err_invalidarg;

	IDWriteFactory * pFactory = m_pEngine->GetDWrite();
	if(!pFactory)
		return err_bad_state;

	IDWriteTextFormat * pFormat = m_pEngine->GetTextFormat(format);

	if(range.length > 0)
	{
		winerr_t err = pFactory->CreateTextLayout(source.text + range.index, (uint_32)range.length, pFormat, (float_32)iWidth, F32_MAX, &m_pLayout);
		if(err.failed())
			return err_failed;
	}

	m_tag = tag;
	Branch(iWidth);
	return 0;
}

err_t CTextLayoutD2D::Branch(int_x iWidth)
{
	if(!m_pSource)
		return err_not_initialized;

	textformat_t format = m_pSource->GetDefFormat();
	if(!m_pLayout)
	{
		m_clusters.resize(1);
		m_lines.resize(1);

		tl_cluster_t & cluster = m_clusters[0];
		tl_line_t & line = m_lines[0];

		cluster.cluster = 0;
		cluster.line = 0;
		cluster.para = 0;
		cluster.trange = {0, GetParagraphTagLength(m_tag)};
		cluster.tlrect = {0, 0, 0, format.font.size};
		cluster.whitespace = true;
		cluster.softbreak = false;
		cluster.paragraphtag = true;
		cluster.softhyphen = false;
		cluster.right2left = false;

		line.line = 0;
		line.trange = {0, cluster.trange.length};
		line.crange = {0, 1};
		line.tlrect = {0, 0, 0, format.font.size};

		return err_ok;
	}
	else
	{
		m_pLayout->SetWordWrapping(GetDwTextWrapping(format.wrap));
		m_pLayout->SetMaxWidth((float_32)iWidth);

		// 获取每一个 cluster 的信息
		uint_32 cluster_count = 0;
		m_pLayout->GetClusterMetrics(nullptr, 0, &cluster_count);
		vector<DWRITE_CLUSTER_METRICS> dwclusters((int_x)cluster_count, (int_x)cluster_count);
		m_pLayout->GetClusterMetrics(dwclusters.buffer(), cluster_count, &cluster_count);

		// 获取 lines 的信息
		vector<DWRITE_LINE_METRICS> dwlms;
		uint_32 line_count = 0;

		m_pLayout->GetLineMetrics(nullptr, 0, &line_count);
		dwlms.reallocate(line_count, line_count);
		m_pLayout->GetLineMetrics(dwlms, line_count, &line_count);
		m_lines.resize(line_count);

		// 这里只有 text position，没有 cluster 信息。
		float_32 fliney = 0.0f;
		for(int_x cnt = 0, index = 0; cnt < dwlms.size(); ++cnt)
		{
			const DWRITE_LINE_METRICS dwlm = dwlms[cnt];
			tl_line_t & line = m_lines[cnt];
			line.line = cnt;
			line.para = 0;
			line.trange = {index, (int_x)dwlm.length};
			line.crange = {0, 0};
			line.tlrect = {0, round(fliney), 0, ceil(dwlm.height)};

			index += dwlm.length;
			fliney += dwlm.height;
		}
		// 扫描 cluster，每一行末尾都会增加一个 cluster。
		m_clusters.resize(cluster_count + line_count);
		int_x icluster = 0, idwcluster = 0, itext = 0;
		float_32 x_base = 0.0f;
		for(int_x iline = 0; iline < (int_x)line_count; ++iline)
		{
			tl_line_t & line = m_lines[iline];
			line.crange.index = icluster;
			x_base = 0.0f;
			while(idwcluster < (int_x)cluster_count)
			{
				tl_cluster_t & cluster = m_clusters[icluster];
				DWRITE_CLUSTER_METRICS & dwcluster = dwclusters[idwcluster];
				if(itext + dwcluster.length <= line.trange.index + line.trange.length)
				{
					cluster.cluster = icluster;
					cluster.trange.index = itext;
					cluster.trange.length = dwcluster.length;
					cluster.line = iline;

					cluster.tlrect.x = round(x_base);
					cluster.tlrect.y = line.tlrect.y;
					cluster.tlrect.w = round(dwcluster.width);
					cluster.tlrect.h = line.tlrect.h;

					cluster.whitespace = dwcluster.isWhitespace != 0;
					cluster.softbreak = dwcluster.canWrapLineAfter != 0;
					cluster.paragraphtag = dwcluster.isNewline != 0;
					cluster.softhyphen = dwcluster.isSoftHyphen != 0;
					cluster.right2left = dwcluster.isRightToLeft != 0;
					cluster.linetag = false;

					itext += dwcluster.length;
					x_base += dwcluster.width;
					++line.crange.length;
					++icluster;
					++idwcluster;
				}
				else
					break;
			}
			line.tlrect.w = ceil(x_base);
			// 行尾的 cluster
			tl_cluster_t & cluster_line_tail = m_clusters[icluster];
			cluster_line_tail.cluster = icluster;
			cluster_line_tail.trange.index = itext;
			if(iline == line_count - 1)
				cluster_line_tail.trange.length = GetParagraphTagLength(m_tag);
			else
				cluster_line_tail.trange.length = 0;
			cluster_line_tail.line = iline;
			cluster_line_tail.tlrect.x = round(x_base);
			cluster_line_tail.tlrect.y = line.tlrect.y;
			cluster_line_tail.tlrect.w = 0;
			cluster_line_tail.tlrect.h = line.tlrect.h;
			cluster_line_tail.paragraphtag = (iline == line_count - 1);
			cluster_line_tail.linetag = true;
			++line.crange.length;
			line.trange.length += cluster_line_tail.trange.length;
			++icluster;
		}

		return err_ok;
	}
}

tl_cluster_t CTextLayoutD2D::FindCluster(int_x iIndex) const
{
	for(int_x cnt = 0, len = m_clusters.size(); cnt < len; ++cnt)
	{
		const tl_cluster_t & cluster = m_clusters[cnt];
		if(cluster.trange.index <= iIndex && (iIndex < cluster.trange.index + cluster.trange.length || (cluster.trange.length == 0 && cluster.paragraphtag)))
			return cluster;
	}

	throw exp_out_of_bound();
}

tl_cluster_t CTextLayoutD2D::GetCluster(int_x iCluster) const
{
	return m_clusters[iCluster];
}

tl_line_t CTextLayoutD2D::GetLine(int_x iLine) const
{
	return m_lines[iLine];
}

tl_range_t CTextLayoutD2D::HitTest(pointix point) const
{
	int_x iLine = -1;
	if(point.y <= 0)
		iLine = 0;
	else
	{
		for(int_x iline = 0; iline < m_lines.size(); ++iline)
		{
			const tl_line_t & line = m_lines[iline];
			if(point.y < line.tlrect.y + line.tlrect.h)
			{
				iLine = iline;
				break;
			}
		}
		if(iLine < 0)
			iLine = m_lines.size() - 1;
	}

	const tl_line_t & line = m_lines[iLine];
	int_x iCluster = -1;
	if(point.x < 0)
		iCluster = line.crange.index;
	else
	{
		for(int_x icluster = line.crange.index; icluster < line.crange.index + line.crange.length; ++icluster)
		{
			const tl_cluster_t & cluster = m_clusters[icluster];
			if(point.x < cluster.tlrect.x + cluster.tlrect.w / 2)
			{
				iCluster = icluster;
				break;
			}
		}
		if(iCluster < 0)
		{
			iCluster = line.crange.index + line.crange.length - 1;
		}
	}

	const tl_cluster_t & cluster = m_clusters[iCluster];
	tl_range_t range;
	range.crange = {cluster.cluster, 1};
	range.trange = cluster.trange;
	range.tlrect = cluster.tlrect;
	return range;
}

int_x CTextLayoutD2D::HitTestRange(int_x iCluster, int_x iLength, tl_range_t * ranges, int_x iCount) const
{
	if(!iLength)
		return 0;

	const tl_cluster_t & cluster_beg = m_clusters[iCluster];
	const tl_cluster_t & cluster_end = m_clusters[iCluster + iLength - 1];
	// 只有一行，好办。
	if(cluster_beg.line == cluster_end.line)
	{
		if(ranges && iCount)
		{
			tl_range_t & range = ranges[0];
			range.trange.index = cluster_beg.trange.index;
			range.trange.length = cluster_end.trange.index + cluster_end.trange.length - cluster_beg.trange.index;
			range.tlrect.x = cluster_beg.tlrect.x;
			range.tlrect.y = cluster_beg.tlrect.y;
			range.tlrect.w = cluster_end.tlrect.x + cluster_end.tlrect.w - cluster_beg.tlrect.x;
			range.tlrect.h = cluster_beg.tlrect.h;
		}
		return 1;
	}
	else
	{
		int_x count = cluster_end.line - cluster_beg.line + 1;
		if(!ranges || !iCount)
			return count;

		if(iCount < count)
			return -1;

		tl_range_t & range_beg = ranges[0];
		tl_range_t & range_end = ranges[count - 1];
		const tl_line_t & line_beg = m_lines[cluster_beg.line];
		const tl_line_t & line_end = m_lines[cluster_end.line];

		range_beg.trange = {cluster_beg.trange.index, line_beg.trange.length - cluster_beg.trange.index};
		range_beg.tlrect = {cluster_beg.tlrect.x, line_beg.tlrect.y, line_beg.tlrect.w - cluster_beg.tlrect.x, line_beg.tlrect.h};

		range_end.trange = {line_end.trange.index, cluster_end.trange.index + cluster_end.trange.length - line_end.trange.index};
		range_end.tlrect = {line_end.tlrect.x, line_end.tlrect.y, cluster_end.tlrect.x + cluster_end.tlrect.w, line_end.tlrect.h};

		for(int_x iline = cluster_beg.line + 1; iline < cluster_end.line; ++iline)
		{
			const tl_line_t & line = m_lines[iline];
			int_x irange = iline - (cluster_beg.line + 1) + 1;
			tl_range_t & range = ranges[irange];
			range.trange = line.trange;
			range.crange = line.crange;
			range.tlrect = line.tlrect;
		}
		return count;
	}
}

tl_metrics_t CTextLayoutD2D::GetMetrics() const
{
	tl_metrics_t metrics = {};
	if(m_pLayout)
	{
		DWRITE_TEXT_METRICS dwmetrics = {};
		m_pLayout->GetMetrics(&dwmetrics);
		metrics.tl_width = ceil(dwmetrics.width);
		metrics.tl_height = ceil(dwmetrics.height);
	}
	else
	{
		metrics.tl_height = m_lines[0].tlrect.h;
	}
	//for(int_x cnt = 0; cnt < m_lines.size(); ++cnt)
	//{
	//	const tl_line_t & line = m_lines[cnt];
	//	if(line.width > metrics.tl_width)
	//		metrics.tl_width = line.width;
	//	metrics.tl_height += line.height;
	//}
	metrics.cluster_length = m_clusters.size();
	metrics.line_length = m_lines.size();
	return metrics;
}

VENUS_END
