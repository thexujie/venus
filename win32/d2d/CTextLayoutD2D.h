#pragma once

#include "D2DInc.h"
#include "CDevice2DD2D.h"

VENUS_BEG

class WIN32_API CTextLayoutD2D : public ObjectT<ITextLayoutD2D>
{
public:
	CTextLayoutD2D(CDevice2DD2D * pEngine);
	~CTextLayoutD2D();

	err_t Initialize(IDocSource * pSrouce);
	IDocSource * GetSource() const;

	err_t Layout(trange_t range, int_x iWidth, paragraph_tag_e tag);
	err_t Branch(int_x iWidth);

	tl_cluster_t FindCluster(int_x iIndex) const;
	tl_cluster_t GetCluster(int_x iCluster) const;
	tl_line_t GetLine(int_x iLine) const;

	tl_range_t HitTest(pointix point) const;
	int_x HitTestRange(int_x iIndex, int_x iLength, tl_range_t * rects, int_x iCount) const;
	tl_metrics_t GetMetrics() const;

public:
	IDWriteTextLayout * GetDxTextLayout() const { return m_pLayout; }

private:
	CDevice2DD2D * m_pEngine;

	IDocSource * m_pSource;
	IDWriteTextLayout * m_pLayout;
	paragraph_tag_e m_tag;

	vector<tl_cluster_t> m_clusters;
	vector<tl_line_t> m_lines;
};

VENUS_END
