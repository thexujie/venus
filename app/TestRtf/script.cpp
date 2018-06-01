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
                scp_cluster cluster;
                ++cluster_num;

                cluster.index = _clusters.size();
                cluster.item_index = iscp;
                cluster.trange = { item.trange.index + itext, 1 };
                cluster.grange = {};

                const SCRIPT_LOGATTR & attr_first = attrs[itext];
                if (attr_first.fSoftBreak)
                    cluster.softbreak = true;
                if (attr_first.fWhiteSpace)
                    cluster.whitespace = true;

                // check the next char.
                while (itext < attrs.size() - 1)
                {
                    const SCRIPT_LOGATTR & tattr = attrs[itext + 1];

                    if (tattr.fCharStop || tattr.fInvalid)
                        break;

                    if (tattr.fSoftBreak)
                        cluster.softbreak = true;
                    if (tattr.fWhiteSpace)
                        cluster.whitespace = true;

                    ++cluster.trange.length;
                    ++itext;
                }

                _clusters.emplace_back(cluster);
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

            int_x ifont_new = -1;
            font = GetFont(run.font);
            ::SelectObject(_hdc, font.hfont);

            int_x gcount_max = run.trange.length * 3 / 2 + 16;
            int_32 gcount = 0;

            _glyphs.resize(goffset + gcount_max);
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
                    _glyphs.data() + goffset, 
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
                        if (HasMissingGlyphs(_glyphs.data() + goffset, gcount, fprop))
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

            if(ifont_new >= 0)
                run.font = ifont_new;

            _glyphs.resize(goffset + gcount);
            _glyph_attrs.resize(goffset + gcount);

            // 获取 advances 和 offsets.
            _glyph_advances.resize(goffset + gcount);
            _glyph_offsets.resize(goffset + gcount);

            ABC abc = {};
            _hr = ScriptPlace(_hdc, font.cache,
                _glyphs.data() + goffset, gcount, _glyph_attrs.data() + goffset, 
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
    }


    void ScriptItem::Layout(int_x start, int_x width, wrapmode_e wrapmode)
    {
        _views.clear();
        _lines.clear();
        if (!_clusters.size())
            return;

        int32_t iadvance = start;

        int_x icluster_curr = 0;
        int_x icluster_last = 0;
        int_x icluster_break = 0;

        int_x iview_last = 0;
        int32_t iline = 0;

        for (int32_t irun = 0; irun < _runs.size(); ++irun)
        {
            scp_run & runitem = _runs[irun];
            icluster_break = runitem.crange.index;
            scp_view view = {};

            view.index = _views.size();
            view.line = iline;
            view.run = irun;

            for (int icluster = 0; icluster < runitem.crange.length; ++icluster, ++icluster_curr)
            {
                scp_cluster & cluster = _clusters[icluster_curr];
                cluster.run_index = irun;
                if (cluster.softbreak || cluster.whitespace)
                    icluster_break = icluster_curr;

                if (icluster_curr - icluster_last < 1 || iadvance + cluster.width < width)
                {
                    //
                }
                else if (wrapmode == wrapmode_char || icluster_curr - icluster_last == 1)
                {
                    if (view.crange.length)
                    {
                        _views.push_back(view);
                        view.crange = {};
                        view.line = iline;
                        view.index = _views.size();
                    }

                    scp_line line;
                    line.line = iline++;
                    line.crange = { icluster_last , icluster_curr - icluster_last };
                    line.vrange = {iview_last, (int32_t)_views.size() - iview_last };
                    _lines.push_back(line);
                    icluster_last = icluster_curr;
                    iadvance = 0;
                    iview_last = _views.size();
                }
                else if (wrapmode == wrapmode_word)
                {
                }
                else{}

                iadvance += cluster.width;
                view.crange += crange_t{ cluster.index, 1};
            }

            if(view.crange.length)
            {
                _views.push_back(view);
                view.crange = {};
                view.line = iline;
                view.index = _views.size();
            }
        }

        if (icluster_curr > icluster_last)
        {
            scp_line line;
            line.line = iline;
            line.crange = { icluster_last, icluster_curr - icluster_last };
            line.vrange = { iview_last, (int32_t)_views.size() - iview_last };
            _lines.push_back(line);
        }

        for (int_x iline = 0; iline < _lines.size(); ++iline)
        {
            scp_line & line = _lines[iline];
            if(line.crange.length > 0)
            {
                const scp_cluster & cluster_beg = _clusters[line.crange.index];
                const scp_cluster & cluster_end = _clusters[line.crange.index + line.crange.length - 1];
                line.trange = { cluster_beg.trange.index, cluster_end.trange.index + cluster_end.trange.length - cluster_beg.trange.index };
                for (int_x icluster = line.crange.index; icluster < line.crange.index + line.crange.length; ++icluster)
                {
                    line.width += _clusters[icluster].width;
                    line.height = std::max(line.height, _clusters[icluster].height);
                }
#ifdef _DEBUG
                line._text = _text.substr(line.trange.index, line.trange.length);
#endif
            }
        }

        for (int_x iview = 0; iview < _views.size(); ++iview)
        {
            scp_view & view = _views[iview];
#ifdef _DEBUG
            const scp_cluster & cluster_beg = _clusters[view.crange.index];
            const scp_cluster & cluster_end = _clusters[view.crange.index + view.crange.length - 1];
            view._text = _text.substr(cluster_beg.trange.index, cluster_end.trange.index - cluster_beg.trange.index + cluster_end.trange.length);
#endif
            for(int_x icluster = 0; icluster < view.crange.length; ++icluster)
            {
                const scp_cluster & cluster = _clusters[view.crange.index + icluster];
                view.grange += cluster.grange;
                view.advance += cluster.width;
            }
        }

        for (int_x iline = 0; iline < _lines.size(); ++iline)
        {
            scp_line & line = _lines[iline];
            std::vector<uint_8> eles(line.vrange.length);
            std::vector<int_32> visualOrders(line.vrange.length);

            for (int_x iview = 0; iview < line.vrange.length; ++iview)
                eles[iview] = _runs[_views[line.vrange.index + iview].run].sa.s.uBidiLevel;

            ScriptLayout(line.vrange.length, eles.data(), visualOrders.data(), NULL);

            std::sort(_views.begin() + line.vrange.index, _views.begin() + line.vrange.index + line.vrange.length, 
                [&visualOrders, &line](const scp_view & lhs, const scp_view & rhs)
            {
                return visualOrders[lhs.index - line.vrange.index] < visualOrders[rhs.index - line.vrange.index];
            });
        }
    }

    void ScriptItem::Draw(HDC hdc, int_x x, int_x y, rectix rect)
    {
        ::SetBkMode(hdc, TRANSPARENT);
        ::SetTextColor(hdc, 0x0000ff);
        HGDIOBJ hOldFont = nullptr;
        int_x drawX = x;
        int_x drawY = rect.y;
        RECT rc = { rect.x, rect.y, rect.right(), rect.bottom() };

        for(int32_t iline = 0; iline < _lines.size(); ++iline)
        {
            const scp_line & line = _lines[iline];
            if (!line.crange.length)
                continue;

            int32_t run_x = 0;
            for(int32_t iview = 0; iview < line.vrange.length; ++iview)
            {
                const scp_view & view = _views[line.vrange.index + iview];

                const scp_run & run = _runs[view.run];
                const scp_item & item = _items[run.item_index];
                view_font_t font = GetFont(run.font);
                if (!hOldFont)
                    hOldFont = ::SelectObject(hdc, font.hfont);
                else
                    ::SelectObject(hdc, font.hfont);

                HRESULT hResult = ScriptTextOut(hdc, font.cache, drawX + run_x, drawY, ETO_CLIPPED, &rc,
                    &item.sa, nullptr, 0,
                    _glyphs.data() + view.grange.index, view.grange.length,
                    _glyph_advances.data() + view.grange.index,
                    nullptr,
                    _glyph_offsets.data() + view.grange.index);

                run_x += view.advance;
            }


            drawX = rect.x;
            drawY += line.height;
        }

        if (hOldFont)
            ::SelectObject(hdc, hOldFont);
    }
}
