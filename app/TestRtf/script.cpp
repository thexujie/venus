#include "stdafx.h"
#include "script.h"
#include <algorithm>

#undef max
#undef min

namespace usp
{
    static bool HasMissingGlyphs(const uint_16 * pGlyphs, int_x iLength, const SCRIPT_FONTPROPERTIES & sfp)
    {
        uint_16 wDefault = sfp.wgDefault;
        uint_16 wInvalid = sfp.wgInvalid;
        uint_16 wBlank = sfp.wgBlank;
        uint_16 wGlyph = 0;
        for (int_x cnt = 0; cnt < iLength; ++cnt)
        {
            wGlyph = pGlyphs[cnt];
            if (wGlyph == wDefault ||
                (wGlyph == wInvalid && wInvalid != wBlank))
                return true;
        }
        return false;
    }

    font_t GetFontFallBack(const font_t & font, int_x iLanguage, const wchar_t * text, int_x length)
    {
        char_32 ch;
        // skip spaces
        while (length > 0 && *text == ' ')
        {
            ++text;
            --length;
        }

        int_x len = utf16_to_unicode(text, length, ch);
        if (len <= 0)
            return {};

        font_t fontfb = font;
        fontfb.name.buffer[0] = 0;
        const unicodeplane_t & plane = unicodeplane_find(text, len);
        switch (plane.plane)
        {
        case unicodeplane_hebrew:
            fontfb.name = L"Courier New";
            break;
        case unicodeplane_arabic: // lang 26
            fontfb.name = L"Times New Roman";
            break;

        case unicodeplane_sinhala: // lang 31
            fontfb.name = L"Tahoma";
            break;

        case unicodeplane_hangul_syllables: // lang 19
            fontfb.name = L"Malgun Gothic";
            break;

        case unicodeplane_cjk_unified_ideographs_ext_b: // lang 12
            fontfb.name = L"SimSun-ExtB";
            break;

        default:
            break;
        }
        return fontfb;
    }

    ScriptItem::ScriptItem()
    {
        font_t font;
        font.name = Win32::GetDefaultFontName();
        font.size = 30;
        AddFont(font);

        HDC hdcScreen = GetDC(NULL);
        _hdc = CreateCompatibleDC(hdcScreen);
        ReleaseDC(NULL, hdcScreen);
    }

    void ScriptItem::Itemize()
    {
        SCRIPT_DIGITSUBSTITUTE sds = { 0 };
        ScriptRecordDigitSubstitution(LOCALE_USER_DEFAULT, &sds);
        SCRIPT_CONTROL sc = { 0 };
        SCRIPT_STATE ss = { 0 };
        ScriptApplyDigitSubstitution(&sds, &sc, &ss);

        std::vector<SCRIPT_ITEM> scriptItems(_text.length() + 1);
        int32_t numScriptItems = 0;
        _hr = ScriptItemize(_text.c_str(), _text.length(), _text.length() + 1, &sc, &ss, scriptItems.data(), &numScriptItems);

        _items.reserve(numScriptItems);
        _items.resize(numScriptItems);
        int_x cluster_num = 0;
        for (int_x iscp = 0; iscp < numScriptItems; ++iscp)
        {
            SCRIPT_ITEM & siCurr = scriptItems[iscp];
            SCRIPT_ITEM & siNext = scriptItems[iscp + 1];

            scp_item & item = _items[iscp];
            item.sa = siCurr.a;
            item.trange = { siCurr.iCharPos, siNext.iCharPos - siCurr.iCharPos };
            item.crange = { cluster_num, 0 };
#ifdef _DEBUG
            item._text = _text.substr(siCurr.iCharPos, siNext.iCharPos - siCurr.iCharPos);
#endif
            std::vector<SCRIPT_LOGATTR> attrs(item.trange.length);
            _hr = ScriptBreak(_text.c_str() + item.trange.index, item.trange.length, &item.sa, attrs.data());

            for (int_x itext = 0; itext < attrs.size(); ++itext)
            {
                scp_cluster ch;
                ++cluster_num;

                ch.item_index = iscp;
                ch.trange = { item.trange.index + itext, 1 };
                ch.grange = {};

                const SCRIPT_LOGATTR & attr_first = attrs[itext];
                if (attr_first.fSoftBreak)
                    ch.softbreak = true;
                if (attr_first.fWhiteSpace)
                    ch.whitespace = true;

                // check the next char.
                while (itext < attrs.size() - 1)
                {
                    const SCRIPT_LOGATTR & tattr = attrs[itext + 1];

                    if (tattr.fCharStop || tattr.fInvalid)
                        break;

                    if (tattr.fSoftBreak)
                        ch.softbreak = true;
                    if (tattr.fWhiteSpace)
                        ch.whitespace = true;

                    ++ch.trange.length;
                    ++itext;
                }

                _clusters.emplace_back(ch);
                ++item.crange.length;
            }
        }

#ifdef _DEBUG
        for (int_x icluster = 0; icluster < _clusters.size(); ++icluster)
        {
            scp_cluster & cluster = _clusters[icluster];
            cluster._text = _text.substr(cluster.trange.index, cluster.trange.length);
        }
#endif
    }

    void ScriptItem::Slice()
    {
        for (int_x iitem = 0; iitem < _items.size(); ++iitem)
        {
            const scp_item & item = _items[iitem];
            int_x ich = item.crange.index;
            int_x ich_end = item.crange.index + item.crange.length;
            while (ich < ich_end)
            {
                scp_run run;
                run.item_index = iitem;
                run.sa = item.sa;
                run.trange = {};
                run.crange = {};
                run.rrange = {};

                for (; ich < ich_end; ++ich)
                {
                    scp_cluster & cluster = _clusters[ich];
                    cluster.item_index = iitem;
                    // first cluster
                    if (!run.crange.length)
                    {
                        run.font = cluster.format.font;
                        run.trange = cluster.trange;
                        run.crange = { ich, 1 };
                    }
                    else
                    {
                        if (cluster.format.font != run.font)
                            break;

                        run.trange.length += cluster.trange.length;
                        ++run.crange.length;
                    }
                }

                _runs.emplace_back(run);
            }
        }

#ifdef _DEBUG
        for (int_x irun = 0; irun < _runs.size(); ++irun)
        {
            scp_run & run = _runs[irun];
            run._text = _text.substr(run.trange.index, run.trange.length);
        }
#endif
    }

    void ScriptItem::Shape()
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

        view_font_t font = GetFont(0);
        HGDIOBJ hOldFont = ::SelectObject(_hdc, font.hfont);
        winerr_t err;

        SCRIPT_FONTPROPERTIES fprop = { sizeof(SCRIPT_FONTPROPERTIES) };

        int_32 goffset = 0;
        for (int_x irun = 0; irun < _runs.size(); ++irun)
        {
            scp_run & run = _runs[irun];

            int_x ifont_new = run.font;
            font = GetFont(run.font);
            ::SelectObject(_hdc, font.hfont);

            int_x gcount_max = run.trange.length * 3 / 2 + 16;
            int_32 gcount = 0;

            _glyph_indices.resize(goffset + gcount_max);
            _glyph_attrs.resize(goffset + gcount_max);
            _cluster_indices.resize(run.trange.index + run.trange.length);

            fallback_e fallback = fallback_waiting;
            while (true)
            {
                // 获取 glyph 信息，包括 glyph id、text cluster、glyph attr.
                err = ScriptShape(_hdc,
                    font.cache,
                    _text.c_str() + run.trange.index, run.trange.length,
                    gcount_max, &(run.sa),
                    _glyph_indices.data() + goffset, 
                    _cluster_indices.data() + run.trange.index,
                    _glyph_attrs.data() + goffset, 
                    &gcount);

                if (err == E_PENDING)
                {
                    // 仅当没有 SelectObject(m_hdc, font.hfont) 时出现，这是不可能的。
                    err = S_OK;
                }
                else if (err == E_OUTOFMEMORY)
                {
                    throw exp_out_of_memory();
                }
                else if (err == USP_E_SCRIPT_NOT_IN_FONT)
                {
                    if (fallback == fallback_abandon)
                        err = S_OK; // 实在不行了，连 missing char 都没有...
                    else
                        fallback = fallback_prepare;
                }
                else if (err == S_OK)
                {
                    if (fallback == fallback_waiting || fallback == fallback_falling)
                    {
                        ScriptGetFontProperties(_hdc, font.cache, &fprop);
                        if (HasMissingGlyphs(_glyph_indices.data() + goffset, gcount, fprop))
                        {
                            if (fallback == fallback_waiting)
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

                if (err.succeeded())
                    break;

                // 需要查找后备字体
                if (fallback == fallback_prepare)
                {
                    font_t fontfb = GetFontFallBack(font.font, run.sa.eScript, _text.c_str() + run.trange.index, run.trange.length);
                    if (!fontfb.name.is_empty())
                    {
                        ifont_new = AddFont(fontfb);
                        font = GetFont(ifont_new);
                        // 查找到了后备字体，将尝试使用后备字体进行 shape.
                        ::SelectObject(_hdc, font.hfont);
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

                if (fallback == fallback_missing)
                {
                    font = GetFont(run.font);
                    ::SelectObject(_hdc, font.hfont);
                    ifont_new = run.font;
                    run.sa.eScript = SCRIPT_UNDEFINED;
                    fallback = fallback_abandon;
                }
                else
                {
                }
            }

            run.font = ifont_new;

            _glyph_indices.resize(goffset + gcount);
            _glyph_attrs.resize(goffset + gcount);

            // 获取 advances 和 offsets.
            _glyph_advances.resize(goffset + gcount);
            _glyph_offsets.resize(goffset + gcount);

            ABC abc = {};
            _hr = ScriptPlace(_hdc, font.cache,
                _glyph_indices.data() + goffset, gcount, _glyph_attrs.data() + goffset, 
                &run.sa, 
                _glyph_advances.data() + goffset, _glyph_offsets.data() + goffset, 
                &abc);

            run.grange = { goffset, gcount };
            goffset += gcount;
            //runitem.advance = abc.abcA + abc.abcB + abc.abcC;
        }

        SelectObject(_hdc, hOldFont);


        // 计算出每个 cluster 的 glyph 范围
        for (int_x irun = 0; irun < _runs.size(); ++irun)
        {
            scp_run & run = _runs[irun];

            int_x iglyph_beg = run.grange.index;
            int_x iglyph_end = run.grange.index + run.grange.length;
            int_x iglyph_inc = 1;
            if (run.sa.fRTL)
            {
                iglyph_beg = run.grange.index + run.grange.length - 1;
                iglyph_end = run.grange.index - 1;
                iglyph_inc = -1;
            }

            for (int_x iglyph = iglyph_beg, icluster = 0; iglyph != iglyph_end;)
            {
                scp_cluster & cluster = _clusters[run.crange.index + icluster];
                if (cluster.grange.length != 0)
                {
                    SCRIPT_VISATTR & glyph_attr = _glyph_attrs[iglyph];
                    if (glyph_attr.fClusterStart)
                    {
                        ++icluster;
                        continue;
                    }
                }
                else
                {
                    // 可能是 fallback 下来的，跟 scp 中不一定相同，从 run 中获取
                    cluster.format.font = run.font;
                    cluster.grange.index = iglyph;
                }
                cluster.grange.length += iglyph_inc;
                iglyph += iglyph_inc;
            }
        }

        // 计算每个 cluster 的大小
        for (int_x icluster = 0; icluster < _clusters.size(); ++icluster)
        {
            scp_cluster & cluster = _clusters[icluster];
            if (cluster.grange.length < 0)
            {
                cluster.grange.index = cluster.grange.index + cluster.grange.length + 1;
                cluster.grange.length = -cluster.grange.length;
            }

            cluster.height = GetFont(cluster.format.font).font.size;
            for (int_x iglyph = 0; iglyph < cluster.grange.length; ++iglyph)
                cluster.width += _glyph_advances[cluster.grange.index + iglyph];
        }

        // 计算每个 run 的大小
        for (int_x irun = 0; irun < _runs.size(); ++irun)
        {
            scp_run & run = _runs[irun];
            for (int_x icluster = 0; icluster < run.crange.length; ++icluster)
            {
                scp_cluster & cluster = _clusters[icluster];
                cluster.run_index = irun;
                run.advance += cluster.width;
            }
        }
    }


    void ScriptItem::Layout(int_x start, int_x width, wrapmode_e wrapmode)
    {
        if (!_clusters.size())
            return;

        //rtfitems.clear();
        //rtflines.clear();

        //int_x iadvance = start_x;
        //if (iadvance && clusters.valid())
        //{
        //    const rtfcluster_t & cluster = clusters[0];
        //    if (iadvance + cluster.width > rect.w)
        //    {
        //        rtfline_t & rtfline_empty = rtflines.add();
        //        rtfline_empty.line = rtflines.size() - 1;
        //        rtfline_empty.rect = { start_x, 0, 0, m_source->GetDefFormat().font.size };
        //        iadvance = 0;
        //    }
        //}


        int32_t iadvance = 0;

        int_x icluster_curr = 0;
        int_x icluster_last = 0;
        int_x icluster_break = 0;

        for (int_x irun = 0; irun < _runs.size(); ++irun)
        {
            scp_run & runitem = _runs[irun];
            icluster_break = runitem.crange.index;

            for (int iclt = 0; iclt < runitem.crange.length; ++iclt)
            {
                icluster_curr = runitem.crange.index + iclt;
                const scp_cluster & cluster = _clusters[icluster_curr];
                if (cluster.softbreak || cluster.whitespace)
                    icluster_break = icluster_curr;

                if (icluster_curr - icluster_last < 1)
                    continue;

                if (iadvance + cluster.width < width)
                    continue;

                if (wrapmode == wrapmode_char || icluster_curr - icluster_last == 1)
                {
                    scp_line line;
                    line.crange = { icluster_last , icluster_curr};
                    _lines.push_back(line);
                    icluster_last = icluster_curr;
                    iadvance = 0;
                }
                else if (wrapmode == wrapmode_word)
                {
                }
                else{}

                iadvance += cluster.width;
            }
        }

        if (icluster_curr > icluster_last)
        {
            scp_line line;
            line.crange = { icluster_last, icluster_curr };
            _lines.push_back(line);
        }

        int_x line_y = 0;
        for (int_x iline = 0; iline < _lines.size(); ++iline)
        {
            scp_line & line = _lines[iline];

            for (int_x icluster = line.crange.index; icluster < line.crange.index + line.crange.length; ++icluster)
            {
                const scp_cluster & cluster = _clusters[icluster];
                line.advance += cluster.width;
                line.height = std::max(cluster.height, line.height);

                const scp_run & runitem = _runs[cluster.run_index];
                if (!line.rrange.length)
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
                    if (rtfitem_last.rtf != cluster.rtf || rtfitem_last.run != cluster.run)
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
        for (int_x irtf = 0; irtf < rtfitems.size(); ++irtf)
        {
            rtfitem_t & rtfitem = rtfitems[irtf];
            rtfitem._text = m_text.sub_text(rtfitem.trange.index, rtfitem.trange.length);
            for (int_x icluster = 0; icluster < rtfitem.crange.length; ++icluster)
            {
                rtfcluster_t & cluster = clusters[rtfitem.crange.index + icluster];
                rtfitem.advance += cluster.width;
            }
        }

        for (int_x iline = 0; iline < rtflines.size(); ++iline)
        {
            rtfline_t & rtfline = rtflines[iline];

            vector<uint_8> eles(rtfline.rrange.length);
            vector<int_32> orders(rtfline.rrange.length);
            vector<int_32> orders2(rtfline.rrange.length);

            for (int_x irtf = 0; irtf < rtfline.rrange.length; ++irtf)
            {
                eles[irtf] = runitems[rtfitems[rtfline.rrange.index + irtf].run].sa.s.uBidiLevel;
            }
            ScriptLayout(rtfline.rrange.length, eles, orders, orders2);

            int_x offset = rtfline.rect.x;
            for (int_x irtf = 0; irtf < rtfline.rrange.length; ++irtf)
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

        for (int_x iscp = 0; iscp < scpitems.size(); ++iscp)
        {
            scpitem_t & run = scpitems[iscp];
            run._debug_text = m_text.sub_text(run.trange.index, run.trange.length);
        }
    }
}
