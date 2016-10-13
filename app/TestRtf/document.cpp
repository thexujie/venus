#include "stdafx.h"
#include "document.h"

static bool HasMissingGlyphs(const uint_16 * pGlyphs, int_x iLength, const SCRIPT_FONTPROPERTIES & sfp)
{
	uint_16 wDefault = sfp.wgDefault;
	uint_16 wInvalid = sfp.wgInvalid;
	uint_16 wBlank = sfp.wgBlank;
	uint_16 wGlyph = 0;
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		wGlyph = pGlyphs[cnt];
		if(wGlyph == wDefault ||
			(wGlyph == wInvalid && wInvalid != wBlank))
			return true;
	}
	return false;
}

Document::Document()
{
}

Document::~Document()
{
}

const doc_font_t DOC_FONT_EMPTY = { 0, 0, 0 };

doc_font_t DocTextObject::GetFontFallBack(const font_t & font, int_x iLanguage, const char_16 * text, int_x length)
{
	char_32 ch;
	int_x len = utf16_to_unicode(text, length, ch);
	if(len <= 0)
		return DOC_FONT_EMPTY;

	font_t fontfb = font;
	const unicodeplane_t & plane = unicodeplane_find(text, len);
	switch(plane.plane)
	{
	case unicodeplane_arabic: // lang 26
		fontfb.name = L"Times New Roman";
		return GetFont(fontfb);

	case unicodeplane_sinhala: // lang 31
		fontfb.name = L"Tahoma";
		return GetFont(fontfb);

	case unicodeplane_hangul_syllables: // lang 19
		fontfb.name = L"Malgun Gothic";
		return GetFont(fontfb);

	case unicodeplane_cjk_unified_ideographs_ext_b: // lang 12
		fontfb.name = L"SimSun-ExtB";
		return GetFont(fontfb);

	default:
		return DOC_FONT_EMPTY;
	}
}

void DocTextObject::Layout(rectix rect)
{
	// font fallback
	enum fallback_e
	{
		// 等待字体回退
		fallback_waiting = 0,
		// 已检测到有无法显示的字符.
		fallback_prepare,
		// 正在使用后备字体进行 shape
		fallback_falling,
		// 无法回退到任何一个可用的字体，使用默认字体的 missing char.
		fallback_missing,
		// 用 missing char 代替了
		fallback_abandon,
	};

	doc_font_t font = GetFont(0);
	HGDIOBJ hOldFont = ::SelectObject(m_hdc, font.hfont);
	winerr_t err;

	SCRIPT_FONTPROPERTIES fprop = { sizeof(SCRIPT_FONTPROPERTIES) };

	int_32 goffset = 0;
	for(int_x irtf = 0; irtf < runitems.size(); ++irtf)
	{
		runitem_t & runitem = runitems[irtf];

		tclusters.resize(runitem.trange.index + runitem.trange.length);

		int_x ifont_new = runitem.font;
		font = GetFont(runitem.font);
		::SelectObject(m_hdc, font.hfont);

		int_x gcount_max = runitem.trange.length * 3 / 2 + 16;
		int_32 gcount = 0;

		glyphs.resize(goffset + gcount_max);
		gattrs.resize(goffset + gcount_max);

		fallback_e fallback = fallback_waiting;
		while(true)
		{
			// 获取 glyph 信息，包括 glyph id、text cluster、glyph attr.
			err = ScriptShape(m_hdc,
				font.cache, m_text + runitem.trange.index,
				runitem.trange.length,
				gcount_max, &(runitem.sa), glyphs + goffset,
				tclusters + runitem.trange.index, gattrs + goffset, &gcount);

			if(err == E_PENDING)
				err = S_OK; // cache 出错，不管了.
			else if(err == E_OUTOFMEMORY);
			else if(err == USP_E_SCRIPT_NOT_IN_FONT)
			{
				if(fallback == fallback_abandon)
					err = S_OK; // 实在不行了，连 missing char 都没有...
				else
					fallback = fallback_prepare;
			}
			else if(err == S_OK)
			{
				if(fallback == fallback_waiting || fallback == fallback_falling)
				{
					ScriptGetFontProperties(m_hdc, font.cache, &fprop);
					if(HasMissingGlyphs(glyphs + goffset, gcount, fprop))
					{
						if(fallback == fallback_waiting)
							fallback = fallback_prepare;
						else
							fallback = fallback_missing;
						err = USP_E_SCRIPT_NOT_IN_FONT;
					}
					else
					{
					}
				}
			}
			else
				err = S_OK; // 无法处理的情况

			if(err.succeeded())
				break;

			// 需要查找后备字体
			if(fallback == fallback_prepare)
			{
				doc_font_t fontfb = GetFontFallBack(font.font, runitem.sa.eScript, m_text + runitem.trange.index, runitem.trange.length);
				if(fontfb.hfont)
				{
					// 查找到了后备字体，将尝试使用后备字体进行 shape.
					::SelectObject(m_hdc, fontfb.hfont);
					ifont_new = IndexFont(fontfb.font);
					font = fontfb;
					fallback = fallback_falling;
				}
				else
				{
					// 没有找到后备字体，用默认字体的 missing char.
					fallback = fallback_missing;
				}
			}
			else
			{
			}

			if(fallback == fallback_missing)
			{
				font = GetFont(runitem.font);
				::SelectObject(m_hdc, font.hfont);
				ifont_new = runitem.font;
				//item.a.eScript = SCRIPT_UNDEFINED;
				fallback = fallback_abandon;
			}
			else
			{
			}
		}

		runitem.font = ifont_new;

		for(int_x itext = 0; itext < runitem.trange.length; ++itext)
		{
			tclusters[runitem.trange.index + itext] += runitem.trange.index;
		}

		glyphs.resize(goffset + gcount);
		gattrs.resize(goffset + gcount);

		// 获取 advances 和 offsets.
		advances.resize(goffset + gcount);
		offsets.resize(goffset + gcount);

		ABC abc = {};
		err = ScriptPlace(m_hdc, font.cache, glyphs + goffset, gcount, gattrs + goffset, &runitem.sa, advances + goffset, offsets + goffset, &abc);

		goffset += gcount;
		//runitem.advance = abc.abcA + abc.abcB + abc.abcC;
	}

	SelectObject(m_hdc, hOldFont);

	for(int_x iglyph = 0, icluster = 0; iglyph < gattrs.size(); ++iglyph, ++icluster)
	{
		cluster_t & cluster = clusters[icluster];
		cluster.grange.index = iglyph;
		cluster.grange.length = 1;

		while(iglyph < gattrs.size() - 1)
		{
			SCRIPT_VISATTR & attr = gattrs[iglyph + 1];
			if(attr.fClusterStart)
				break;

			++iglyph;
			++cluster.grange.length;
		}
	}


	for(int_x irun = 0; irun < runitems.size(); ++irun)
	{
		runitem_t & runitem = runitems[irun];
		runitem.rrange = { rtfitems.size(), 0 };

		int_x icluster = runitem.crange.index;
		int_x icluster_end = runitem.crange.index + runitem.crange.length;
		while(icluster < icluster_end)
		{
			rtfitem_t & rtfitem = rtfitems.add();
			rtfitem.sa = runitem.sa;
			rtfitem.rtf.font = runitem.font;
			rtfitem.rtf.color = 0;
			rtfitem.trange = {};
			rtfitem.crange = {};

			++runitem.rrange.length;

			for(; icluster < icluster_end; ++icluster)
			{
				const cluster_t & cluster = clusters[icluster];
				// first cluster
				if(!rtfitem.crange.length)
				{
					rtfitem.rtf.color = cluster.rtf.color;
					rtfitem.trange = cluster.trange;
					rtfitem.crange = { icluster, 1 };
				}
				else
				{
					if(cluster.rtf.color != rtfitem.rtf.color)
						break;

					rtfitem.trange.length += cluster.trange.length;
					++rtfitem.crange.length;
				}
			}
		}
	}

	// advance width
	for(int_x irtf = 0; irtf < rtfitems.size(); ++irtf)
	{
		rtfitem_t & rtfitem = rtfitems[irtf];
		for(int_x icluster = 0; icluster < rtfitem.crange.length; ++icluster)
		{
			cluster_t & cluster = clusters[rtfitem.crange.index + icluster];
			for(int_x iglyph = 0; iglyph < cluster.grange.length; ++iglyph)
			{
				cluster.advance += advances[cluster.grange.index + iglyph];
				rtfitem.advance += advances[cluster.grange.index + iglyph];
			}
		}
	}

	for(int_x iscp = 0; iscp < scpitems.size(); ++iscp)
	{
		scpitem_t & item = scpitems[iscp];
		item._debug_text = m_text.sub_text(item.trange.index, item.trange.index + item.trange.length);
	}

	for(int_x irun = 0; irun < runitems.size(); ++irun)
	{
		runitem_t & item = runitems[irun];
		item._debug_text = m_text.sub_text(item.trange.index, item.trange.index + item.trange.length);
	}

	for(int_x irtf = 0; irtf < rtfitems.size(); ++irtf)
	{
		rtfitem_t & item = rtfitems[irtf];
		item._debug_text = m_text.sub_text(item.trange.index, item.trange.index + item.trange.length);
	}

	for(int_x iclt = 0; iclt < clusters.size(); ++iclt)
	{
		cluster_t & item = clusters[iclt];
		item._debug_text = m_text.sub_text(item.trange.index, item.trange.index + item.trange.length);
	}
}


void MakeLOGFONT(HDC hdc, const font_t & font, LOGFONT & logFont)
{
	if(font.name[0])
		textcpy(logFont.lfFaceName, LF_FACESIZE, font.name.buffer, -1);
	else
	{
		chbufferw<MAX_FONTNAME> defFontName = Win32::GetDefaultFontName();
		textcpy(logFont.lfFaceName, LF_FACESIZE, defFontName.buffer, -1);
	}

	int iDpiY = GetDeviceCaps(hdc, LOGPIXELSY);
	logFont.lfWidth = 0;
	logFont.lfHeight = (int_32)(-font.size * 72 / iDpiY);
	//logFont.lfHeight = (int_32)font.size;
	logFont.lfWeight = (int_32)font.weight;

	logFont.lfItalic = (uint_8)font.italic;
	logFont.lfUnderline = (uint_8)font.underline;
	logFont.lfStrikeOut = (uint_8)font.strikeout;
	//logFont.lfCharSet = (uint_8)(font.charset);
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	switch(font.renderlevel)
	{
	case FontRenderLevelSystem:
		logFont.lfQuality = DEFAULT_QUALITY;
		break;
	case FontRenderLevelGray:
		logFont.lfQuality = DRAFT_QUALITY;
		break;
	case FontRenderLevelAntiGray:
		logFont.lfQuality = ANTIALIASED_QUALITY;
		break;
	case FontRenderLevelClearTypeGrid:
		logFont.lfQuality = CLEARTYPE_QUALITY;
		break;
	default:
		logFont.lfQuality = DEFAULT_QUALITY;
		break;
	}
	logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
}


dictionary<uint_32, HPEN> pens;

void DocTextObject::Draw(HDC hdc, int x, int y)
{
	HGDIOBJ hOldFont = NULL;
	HGDIOBJ hOldPen = NULL;
	for(int_x cnt = 0; cnt < rtfitems.size(); ++cnt)
	{
		const rtfitem_t & rtfitem = rtfitems[cnt];
		if(!rtfitem.crange.length)
			continue;

		RECT rect = { x, y, 1000, 1000 };

		doc_font_t font = GetFont(rtfitem.rtf.font);
		if(!hOldFont)
			hOldFont = ::SelectObject(hdc, font.hfont);
		else
			::SelectObject(hdc, font.hfont);

		uint_32 color = colors[rtfitem.rtf.color];

		uint_32 uiColor = ((color >> 16) & 0xFF) | (color & 0xFF00) | ((color << 16) & 0xFF0000);
		uint_32 uiColorOld = ::SetTextColor(hdc, uiColor);
		int iModeOld = ::SetBkMode(hdc, TRANSPARENT);

		//HPEN & hPen = pens[color];
		//if(!hPen)
		//{
		//	hPen = CreatePen(PS_SOLID, 1, color);
		//}

		//if(!hOldPen)
		//	hOldPen = ::SelectObject(hdc, hPen);
		//else
		//	::SelectObject(hdc, hPen);


		const cluster_t & cbeg = clusters[rtfitem.crange.index];
		const cluster_t & cend = clusters[rtfitem.crange.index + rtfitem.crange.length - 1];

		int_x gindex = cbeg.grange.index;
		int_x gcount = cend.grange.index + cend.grange.length - cbeg.grange.index;
		HRESULT hResult = ScriptTextOut(hdc, font.cache, x, y, ETO_CLIPPED, &rect, &rtfitem.sa, nullptr, 0, glyphs + gindex, gcount,
			advances + gindex, nullptr, offsets + gindex);

		x += rtfitem.advance;
	}
}