#pragma once

#include <string>
#include <vector>

#include "BaseInc.h"
#include <usp10.h>
#pragma comment(lib, "usp10.lib")

namespace usp
{
    inline crange_t operator + ( const crange_t & lhs, const crange_t & rhs)
    {
        if (lhs.index < 0 || !lhs.length)
            return rhs;

        if (rhs.index < 0 || !rhs.length)
            return lhs;

        if (lhs.index + lhs.length == rhs.index)
            return { lhs.index, rhs.index + rhs.length };

        if (rhs.index + rhs.length == lhs.index)
            return { rhs.index, lhs.index + lhs.length };

        return {-1, 0};
    }

    inline crange_t & operator += (crange_t & lhs, const crange_t & rhs)
    {
        if (lhs.index < 0 || !lhs.length)
            lhs = rhs;
        else if (rhs.index < 0 || !rhs.length)
            ;
        else if (lhs.index + lhs.length == rhs.index)
            lhs = { lhs.index, lhs.length + rhs.length };
        else if (rhs.index + rhs.length == lhs.index)
            lhs ={ rhs.index, lhs.length + rhs.length };
        else
            lhs = { -1, 0 };
        return lhs;
    }

    inline grange_t operator + (const grange_t & lhs, const grange_t & rhs)
    {
        if (lhs.index < 0 || !lhs.length)
            return rhs;

        if (rhs.index < 0 || !rhs.length)
            return lhs;

        if (lhs.index + lhs.length == rhs.index)
            return { lhs.index, rhs.index + rhs.length };

        if (rhs.index + rhs.length == lhs.index)
            return { rhs.index, lhs.index + lhs.length };

        return { -1, 0 };
    }

    inline grange_t & operator += (grange_t & lhs, const grange_t & rhs)
    {
        if (lhs.index < 0 || !lhs.length)
            lhs = rhs;
        else if (rhs.index < 0 || !rhs.length)
            ;
        else if (lhs.index + lhs.length == rhs.index)
            lhs = { lhs.index, lhs.length + rhs.length };
        else if (rhs.index + rhs.length == lhs.index)
            lhs = { rhs.index, lhs.length + rhs.length };
        else
            lhs = { -1, 0 };
        return lhs;
    }

    enum wrapmode_e
    {
        wrapmode_none,
        wrapmode_char,
        wrapmode_word,
    };

    // view range
    struct vrange_t
    {
        int_x index = -1;
        int_x length = 0;
    };

    struct view_font_t
    {
        SCRIPT_CACHE * cache;
        HFONT hfont;
        font_t font;
    };

    struct scp_format
    {
        int32_t font = 0;
        int32_t color = 0;
    };

    // run range
    struct rrange_t
    {
        int_x index;
        int_x length;
    };

    struct scp_item
    {
#ifdef _DEBUG
        std::wstring _text;
#endif
        SCRIPT_ANALYSIS sa;
        trange_t trange;
        crange_t crange;
    };

    struct scp_cluster
    {
#ifdef _DEBUG
        std::wstring _text;
#endif
        int32_t index = 0;
        int32_t item_index = 0;
        int32_t run_index = 0;
        trange_t trange;
        grange_t grange;

        int32_t x = 0;
        int32_t y = 0;
        int32_t width = 0;
        int32_t height = 0;

        scp_format format;

        bool whitespace : 1;
        //! 该字符后是否是建议的换行位置.
        bool softbreak : 1;
        //! 是否是段落标记
        bool paragraphtag : 1;
        //! 是否是连字符
        bool softhyphen : 1;
        //! 是否是从右到左的阅读顺序
        bool right2left : 1;
        //! 是否是行标记
        bool linetag : 1;
    };

    struct scp_run
    {
#ifdef _DEBUG
        std::wstring _text;
#endif
        int32_t item_index = -1;

        SCRIPT_ANALYSIS sa;
        int32_t font;
        trange_t trange;
        crange_t crange;
        grange_t grange;
        rrange_t rrange;
    };

    struct scp_view
    {
#ifdef _DEBUG
        std::wstring _text;
#endif
        int32_t index = -1;
        int32_t line = -1;
        int32_t run = -1;
        crange_t crange;
        grange_t grange;
        int32_t advance = 0;
    };

    struct scp_line
    {
#ifdef _DEBUG
        std::wstring _text;
#endif
        int32_t line = 0;
        vrange_t vrange;
        crange_t crange;
        trange_t trange;

        int32_t width = 0;
        int32_t height = 0;
    };


    class ScriptItem
    {
    public:
        ScriptItem();
        ScriptItem(std::wstring text) :_text(text) {}
        ~ScriptItem() = default;

        void SetText(std::wstring text) { _text = text; }
        // generate scripts and clusters(by ScriptBreak).
        void Itemize();
        // generate runs for different font(name、size、bold、italic...)
        void Slice();
        // generate glyphs for each run, and calculate widths of all clusters and runs.
        void Shape();

        // layout all clusters
        void Layout(int_x start, int_x width, wrapmode_e wrapmode);

        void Draw(HDC hdc, int_x x, int_x y, rectix rect);
    private:
        std::wstring _text;
        HRESULT _hr = S_OK;

        // 被拆分（Itemize）出的语言段，item 内的文字都是同一语言的，一个 item 可被拆分（Slice）成多个 run
        std::vector<scp_item> _items;

        // 被拆分的图形段，cluster 表示一个『字』
        std::vector<scp_cluster> _clusters;

        // 具有相同字体的一段文字，run 内的文字都是同一语言的，多个 run 一起构成一个 item
        std::vector<scp_run> _runs;

        std::vector<scp_view> _views;

        std::vector<scp_line> _lines;

        // -----------------------------显示相关
        // glyph indices
        std::vector<uint_16> _glyphs;
        std::vector<SCRIPT_VISATTR> _glyph_attrs;
        std::vector<int_32> _glyph_advances;
        std::vector<GOFFSET> _glyph_offsets;
        std::vector<uint_16> _cluster_indices;

        //-------------------------------------------------
        HDC _hdc;
        vector<view_font_t> _fonts;

        int32_t AddFont(const font_t & font)
        {
            int32_t ifont = -1;
            for (int32_t cnt = 0; cnt < _fonts.size(); ++cnt)
            {
                if (_fonts[cnt].font == font)
                {
                    ifont = cnt;
                    break;
                }
            }

            if (ifont < 0)
            {
                ifont = _fonts.size();

                LOGFONTW logFont = Win32::MappingFont(font);
                HFONT hFont = CreateFontIndirectW(&logFont);
                if (!hFont)
                    throw exp_bad_state();

                view_font_t vfont;
                vfont.font = font;
                vfont.hfont = hFont;
                vfont.cache = new SCRIPT_CACHE;

                *vfont.cache = nullptr;
                _fonts.add(vfont);
            }
            return ifont;
        }
        view_font_t GetFont(int_x ifont)
        {
            return _fonts[ifont];
        }
    };

}