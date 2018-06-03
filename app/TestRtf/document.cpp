#include "stdafx.h"
#include "document.h"

typedef SCRIPT_LOGATTR TEXT_ATTR;

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

const doc_font_t DOC_FONT_EMPTY = { nullptr, nullptr, 0 };

DocTextObject::DocTextObject()
{
	font_t & font = fonts.add();
	font.name = Win32::GetDefaultFontName();
	font.size = 30;
	colors.add(Colors::Black);

	HDC hdcScreen = GetDC(NULL);
	m_hdc = CreateCompatibleDC(hdcScreen);
	ReleaseDC(NULL, hdcScreen);
}

doc_font_t DocTextObject::GetFontFallBack(const font_t & font, int_x iLanguage, const char_16 * text, int_x length)
{
	char_32 ch;
	// skip spaces
	while(length > 0 && *text == ' ')
	{
		++text;
		--length;
	}

	int_x len = utf16_to_unicode(text, length, ch);
	if(len <= 0)
		return DOC_FONT_EMPTY;

	font_t fontfb = font;
	const unicodeplane_t & plane = unicodeplane_find(text, len);
	switch(plane.plane)
	{
	case unicodeplane_hebrew:
		fontfb.name = L"Courier New";
		return GetFont(fontfb);
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

void DocTextObject::SetText(textw text)
{
	m_text = text;
}

void DocTextObject::Analyse()
{
	SCRIPT_DIGITSUBSTITUTE sds = { 0 };
	ScriptRecordDigitSubstitution(LOCALE_USER_DEFAULT, &sds);
	SCRIPT_CONTROL sc = { 0 };
	SCRIPT_STATE ss = { 0 };
	ScriptApplyDigitSubstitution(&sds, &sc, &ss);

	vector<SCRIPT_ITEM> items(m_text.length() + 1, m_text.length() + 1);
	int_32 nitems = 0;
	ScriptItemize(m_text, m_text.length(), m_text.length() + 1, &sc, &ss, items, &nitems);

	scpitems.reallocate(nitems, nitems);

	int_x cluster_num = 0;
	for(int_x iitem = 0; iitem < nitems; ++iitem)
	{
		scpitem_t & scpitem = scpitems[iitem];
		SCRIPT_ITEM & item = items[iitem];
		SCRIPT_ITEM & item_next = items[iitem + 1];
		scpitem.sa = item.a;
		scpitem.trange = { item.iCharPos, item_next.iCharPos - item.iCharPos };
		scpitem.crange = { cluster_num, 0 };
#ifdef _DEBUG
		scpitem._debug_text = m_text.sub_text(item.iCharPos, item_next.iCharPos - item.iCharPos);
#endif

		vector<TEXT_ATTR> tattrs(scpitem.trange.length, scpitem.trange.length);

		ScriptBreak(m_text + scpitem.trange.index, scpitem.trange.length, &scpitem.sa, tattrs);
		for(int_x itext = 0; itext < tattrs.size(); ++itext)
		{
			rtfcluster_t & cluster = clusters.add();
			++cluster_num;

			cluster.run = iitem;
			cluster.scp = 0;
			cluster.trange = { scpitem.trange.index + itext, 1 };
			cluster.rtf = { 0, 0 };

			const TEXT_ATTR & attr_first = tattrs[itext];
			if(attr_first.fSoftBreak)
				cluster.softbreak = true;
			if(attr_first.fWhiteSpace)
				cluster.whitespace = true;

			// check the next char.
			while(itext < tattrs.size() - 1)
			{
				const TEXT_ATTR & tattr = tattrs[itext + 1];

				if(tattr.fCharStop || tattr.fInvalid)
					break;

				if(tattr.fSoftBreak)
					cluster.softbreak = true;
				if(tattr.fWhiteSpace)
					cluster.whitespace = true;

				++cluster.trange.length;
				++itext;
			}
		}

		scpitem.crange.length = cluster_num - scpitem.crange.index;
	}

#ifdef _DEBUG
	for(int_x icluster = 0; icluster < clusters.size(); ++icluster)
	{
		rtfcluster_t & cluster = clusters[icluster];
		cluster._text = m_text.sub_text(cluster.trange.index, cluster.trange.length);
	}
#endif
}

void DocTextObject::Slice()
{
	for(int_x iscp = 0; iscp < scpitems.size(); ++iscp)
	{
		const scpitem_t & scpitem = scpitems[iscp];
		int_x icluster = scpitem.crange.index;
		int_x icluster_end = scpitem.crange.index + scpitem.crange.length;
		while(icluster < icluster_end)
		{
			runitem_t & runitem = runitems.add();
			runitem.sa = scpitem.sa;
			runitem.scp = iscp;
			runitem.trange = {};
			runitem.crange = {};
			runitem.rrange = {};

			for(; icluster < icluster_end; ++icluster)
			{
				rtfcluster_t & cluster = clusters[icluster];
				cluster.scp = iscp;
				// first cluster
				if(!runitem.crange.length)
				{
					runitem.font = cluster.rtf.font;
					runitem.trange = cluster.trange;
					runitem.crange = { icluster, 1 };
				}
				else
				{
					if(cluster.rtf.font != runitem.font)
						break;

					runitem.trange.length += cluster.trange.length;
					++runitem.crange.length;
				}
			}
		}
	}

#ifdef _DEBUG
	for(int_x run = 0; run < runitems.size(); ++run)
	{
		runitem_t & runitem = runitems[run];
		runitem._text = m_text.sub_text(runitem.trange.index, runitem.trange.length);
	}
#endif
}

void DocTextObject::Shape()
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
	for(int_x irun = 0; irun < runitems.size(); ++irun)
	{
		runitem_t & runitem = runitems[irun];
		//runitem.sa.fRTL = 1;

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
			{
				// 仅当没有 SelectObject(m_hdc, font.hfont) 时出现，这是不可能的。
				err = S_OK;
			}
			else if(err == E_OUTOFMEMORY)
			{
				throw exp_out_of_memory();
			}
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
				runitem.sa.eScript = SCRIPT_UNDEFINED;
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

		runitem.grange = { goffset, gcount };
		goffset += gcount;
		//runitem.advance = abc.abcA + abc.abcB + abc.abcC;
	}

	SelectObject(m_hdc, hOldFont);

	for(int_x irun = 0; irun < runitems.size(); ++irun)
	{
		runitem_t & runitem = runitems[irun];

		int_x iglyph_beg = runitem.grange.index;
		int_x iglyph_end = runitem.grange.index + runitem.grange.length;
		int_x iglyph_inc = 1;
		if(runitem.sa.fRTL)
		{
			iglyph_beg = runitem.grange.index + runitem.grange.length - 1;
			iglyph_end = runitem.grange.index - 1;
			iglyph_inc = -1;
		}

		for(int_x iglyph = iglyph_beg, icluster = 0; iglyph != iglyph_end;)
		{
			rtfcluster_t & cluster = clusters[runitem.crange.index + icluster];
            if(cluster.grange.length != 0)
            {
                SCRIPT_VISATTR & attr = gattrs[iglyph];
                if (attr.fClusterStart)
                {
                    ++icluster;
                    continue;
                }
            }
            else
            {
                // 可能是 fallback 下来的，跟 scp 中不一定相同，从 run 中获取
                cluster.rtf.font = runitem.font;
                cluster.grange.index = iglyph;
            }
			cluster.grange.length += iglyph_inc;
            iglyph += iglyph_inc;
		}
	}

	for(int_x iclt = 0; iclt < clusters.size(); ++iclt)
	{
		rtfcluster_t & cluster = clusters[iclt];
        if (cluster.grange.length < 0)
        {
            cluster.grange.index = cluster.grange.index + cluster.grange.length + 1;
            cluster.grange.length = -cluster.grange.length;
        }

        cluster.height = GetFont(cluster.rtf.font).font.size;
		for(int_x iglyph = 0; iglyph < cluster.grange.length; ++iglyph)
			cluster.width += advances[cluster.grange.index + iglyph];
	}

	for(int_x run = 0; run < runitems.size(); ++run)
	{
		runitem_t & runitem = runitems[run];
		for(int_x iclt = 0; iclt < runitem.crange.length; ++iclt)
		{
			rtfcluster_t & cluster = clusters[iclt];
			runitem.advance += cluster.width;
		}
	}
}

void DocTextObject::Layout(int_x start_x, rectix rect, wrapmode_e wrapmode)
{
	rtfitems.clear();
	rtflines.clear();

	int_x iadvance = start_x;
	if(iadvance && clusters.valid())
	{
		const rtfcluster_t & cluster = clusters[0];
		if(iadvance + cluster.width > rect.w)
		{
			rtfline_t & rtfline_empty = rtflines.add();
			rtfline_empty.line = rtflines.size() - 1;
			rtfline_empty.rect = { start_x, 0, 0, m_source->GetDefFormat().font.size };
			iadvance = 0;
		}
	}

	rtfline_t & rtfline_first = rtflines.add();
	rtfline_first.line = rtflines.size() - 1;
	rtfline_first.rect = { iadvance, 0, 0, 0};
	for(int_x irun = 0; irun < runitems.size(); ++irun)
	{
		runitem_t & runitem = runitems[irun];

		int_x icluster_break = -1;
		if(irun > 0)
			icluster_break = runitem.crange.index;

		for(int iclt = 0; iclt < runitem.crange.length; ++iclt)
		{
			int_x icluster = runitem.crange.index + iclt;
			const rtfcluster_t & cluster = clusters[icluster];
			if(cluster.softbreak || cluster.whitespace)
				icluster_break = icluster;

			rtfline_t & rtfline_last = rtflines.back();

			// there must be at least one cluster.
			if(!rtfline_last.crange.length)
			{
				//...
			}
			else if(iadvance + cluster.width > rect.w)
			{
				if(wrapmode == wrapmode_char || icluster_break < 0)
				{
					rtfline_t & rtfline_new = rtflines.add();
					rtfline_new.line = rtflines.size() - 1;
					rtfline_new.crange.index = icluster;
					rtfline_new.rect = {0, 0, 0, 0};
					icluster_break = -1;
					iadvance = 0;
				}
				else
				{
					rtfline_last.crange.length = icluster_break - rtfline_last.crange.index;
					iclt = icluster_break - runitem.crange.index;

					rtfline_t & rtfline_new = rtflines.add();
					rtfline_new.line = rtflines.size() - 1;
					rtfline_new.crange = { icluster_break, 0 };
					rtfline_new.rect = { 0, 0, 0, 0 };
					icluster_break = -1;
					iadvance = 0;
				}
			}

			rtfline_t & rtfline = rtflines.back();
            rtfline.crange.index = runitem.crange.index + iclt;
			rtfline.crange.length += 1;
			iadvance += cluster.width;
		}
	}

	for(int_x iline = 0; iline < rtflines.size(); ++iline)
	{
		rtfline_t & rtfline = rtflines[iline];
		// break lines fallback
		if(rtfline.crange.length < 0)
		{
			rtfline.crange.index = rtfline.crange.index + rtfline.crange.length + 1;
			rtfline.crange.length = -rtfline.crange.length;
		}
	}

	int_x line_y = 0;
	for(int_x iline = 0; iline < rtflines.size(); ++iline)
	{
		rtfline_t & line = rtflines[iline];
		line.rrange.index = rtfitems.size();
        line.rect.y = line_y;

		if(line.crange.length < 0)
		{
			line.crange.index = line.crange.index + line.crange.length;
			line.crange.length = -line.crange.length;
		}

		for(int_x icluster = line.crange.index; 
			icluster < line.crange.index + line.crange.length;
				++icluster)
		{
			const rtfcluster_t & cluster = clusters[icluster];
			line.advance += cluster.width;
			line.rect.w += cluster.width;
			line.rect.h = maxof(cluster.height, line.rect.h);
			if(!line.trange.length)
				line.trange = cluster.trange;
			else
				line.trange.length += cluster.trange.length;

			const runitem_t & runitem = runitems[cluster.run];
			if(!line.rrange.length)
			{
				rtfitem_t & rtfitem_new = rtfitems.add();
				++line.rrange.length;
				rtfitem_new.run = cluster.run;
				rtfitem_new.scp = cluster.scp;
				rtfitem_new.line = iline;
				rtfitem_new.rtf.font = runitem.font;
				rtfitem_new.rtf.color = cluster.rtf.color;

				rtfitem_new.crange.index = icluster;
				rtfitem_new.trange.index = cluster.trange.index;
			}
			else
			{
				rtfitem_t & rtfitem_last = rtfitems.back();
				if(rtfitem_last.rtf != cluster.rtf || rtfitem_last.run != cluster.run)
				{
					rtfitem_t & rtfitem_new = rtfitems.add();
					++line.rrange.length;
					rtfitem_new.run = cluster.run;
					rtfitem_new.scp = cluster.scp;
					rtfitem_new.line = iline;
					rtfitem_new.rtf.font = runitem.font;
					rtfitem_new.rtf.color = cluster.rtf.color;

					rtfitem_new.crange.index = icluster;
					rtfitem_new.trange.index = cluster.trange.index;
				}
			}

			rtfitem_t & rtfitem = rtfitems.back();
			rtfitem.trange.length += cluster.trange.length;
			rtfitem.crange.length += 1;
		}

        line_y += line.rect.h;
		line._text = m_text.sub_text(line.trange.index, line.trange.length);
	}

	// advance width
	for(int_x irtf = 0; irtf < rtfitems.size(); ++irtf)
	{
		rtfitem_t & rtfitem = rtfitems[irtf];
		rtfitem._text = m_text.sub_text(rtfitem.trange.index, rtfitem.trange.length);
		for(int_x icluster = 0; icluster < rtfitem.crange.length; ++icluster)
		{
			rtfcluster_t & cluster = clusters[rtfitem.crange.index + icluster];
			rtfitem.advance += cluster.width;
		}
	}

	for(int_x iline = 0; iline < rtflines.size(); ++iline)
	{
		rtfline_t & rtfline = rtflines[iline];

		vector<uint_8> eles(rtfline.rrange.length);
		vector<int_32> orders(rtfline.rrange.length);
		vector<int_32> orders2(rtfline.rrange.length);

		for(int_x irtf = 0; irtf < rtfline.rrange.length; ++irtf)
		{
			eles[irtf] = runitems[rtfitems[rtfline.rrange.index + irtf].run].sa.s.uBidiLevel;
		}
		ScriptLayout(rtfline.rrange.length, eles, orders, orders2);

		int_x offset = rtfline.rect.x;
		for(int_x irtf = 0; irtf < rtfline.rrange.length; ++irtf)
		{
			rtfitem_t & rtfitem = rtfitems[rtfline.rrange.index + orders[irtf]];
			rtfitem.offset = offset;
			offset += rtfitem.advance;
		}

        for (int_x icluster = 0; icluster < rtfline.crange.length; ++icluster)
        {
            rtfcluster_t & cluster = clusters[rtfline.crange.index + icluster];
            cluster.y = rtfline.rect.y;
        }

        for (int_x irtf = 0; irtf < rtfline.rrange.length; ++irtf)
        {
            rtfitem_t & rtfitem = rtfitems[rtfline.rrange.index + orders[irtf]];
            int_x offX = 0;
            for (int_x icluster = 0; icluster < rtfitem.crange.length; ++icluster)
            {
                rtfcluster_t & cluster = clusters[rtfitem.crange.index + icluster];
                cluster.x = rtfitem.offset + offX;
                offX += cluster.width;
            }
        }
	}

	for(int_x iscp = 0; iscp < scpitems.size(); ++iscp)
	{
		scpitem_t & run = scpitems[iscp];
		run._debug_text = m_text.sub_text(run.trange.index, run.trange.length);
	}
}

void DocTextObject::Draw(HDC hdc, int_x x, int_x y, rectix rect)
{
	::SetBkMode(hdc, TRANSPARENT);
	HGDIOBJ hOldFont = nullptr;
	int_x iline = -1;
	int_x ix = x;
	for(int_x cnt = 0; cnt < rtfitems.size(); ++cnt)
	{
		const rtfitem_t & rtfitem = rtfitems[cnt];
		if(!rtfitem.crange.length)
			continue;
		const runitem_t & rtfrun = runitems[rtfitem.run];

		if(rtfitem.line != iline)
		{
			iline = rtfitem.line;
		}

		RECT rc = { rect.x, rect.y, rect.right(), rect.bottom() };

		doc_font_t font = GetFont(rtfitem.rtf.font);
		if(!hOldFont)
			hOldFont = ::SelectObject(hdc, font.hfont);
		else
			::SelectObject(hdc, font.hfont);

		uint_32 color = colors[rtfitem.rtf.color];
		color = ((color >> 16) & 0xFF) | (color & 0xFF00) | ((color << 16) & 0xFF0000);
		::SetTextColor(hdc, color);

		const rtfcluster_t & cbeg = clusters[rtfitem.crange.index];
		const rtfcluster_t & cend = clusters[rtfitem.crange.index + rtfitem.crange.length - 1];

		int_x gindex = 0, gcount = 0;
		if(cbeg.grange.index < cend.grange.index)
		{
			gindex = cbeg.grange.index;
			gcount = cend.grange.index + cend.grange.length - cbeg.grange.index;
		}
		else
		{
			gindex = cend.grange.index;
			gcount = cbeg.grange.index + cbeg.grange.length - cend.grange.index;
		}

		HRESULT hResult = ScriptTextOut(hdc, font.cache, x + rtfitem.offset, y + rtfitem.line * font.font.size, ETO_CLIPPED, &rc, &rtfrun.sa, nullptr, 0, glyphs + gindex, gcount,
			advances + gindex, nullptr, offsets + gindex);

		ix += rtfitem.advance;
	}

	if(hOldFont)
		::SelectObject(hdc, hOldFont);
}

void DocTextObject::Destroy()
{
	tclusters.destroy();
	offsets.destroy();
	advances.destroy();
	gattrs.destroy();
	glyphs.destroy();
	colors.destroy();
	fonts.destroy();
	rtflines.destroy();
	rtfitems.destroy();
	runitems.destroy();
	scpitems.destroy();
	clusters.destroy();
	m_text.destroy();
	caches.destroy();
}


err_t DocTextObject::Layout(trange_t range, int_x iWidth, paragraph_tag_e tag)
{
	return err_ok;
}

err_t DocTextObject::Branch(int_x iWidth)
{
	return err_ok;
}

tl_metrics_t DocTextObject::GetMetrics() const
{
	return tl_metrics_t();
}

tl_line_t DocTextObject::GetLine(int_x iLine) const
{
	return tl_line_t();
}

tl_range_t DocTextObject::HitTest(pointix point) const
{
	return tl_range_t();
}

int_x DocTextObject::HitTestRange(int_x iIndex, int_x iLength, tl_range_t * rects, int_x iCount) const
{
	return 0;
}
