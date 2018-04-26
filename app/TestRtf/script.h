#pragma once

#include <string>
#include <vector>

#include "BaseInc.h"
#include <usp10.h>
#pragma comment(lib, "usp10.lib")

namespace usp
{
    enum wrapmode_e
    {
        wrapmode_none,
        wrapmode_char,
        wrapmode_word,
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
        int32_t item_index;
        trange_t trange;
        grange_t grange;
        int32_t x = 0;
        int32_t y = 0;
        int32_t width = 0;
        int32_t height = 0;

        scp_format format;

        bool whitespace : 1;
        //! ���ַ����Ƿ��ǽ���Ļ���λ��.
        bool softbreak : 1;
        //! �Ƿ��Ƕ�����
        bool paragraphtag : 1;
        //! �Ƿ������ַ�
        bool softhyphen : 1;
        //! �Ƿ��Ǵ��ҵ�����Ķ�˳��
        bool right2left : 1;
        //! �Ƿ����б��
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
        int_x advance;
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
        // generate runs for different font(name��size��bold��italic...)
        void Slice();
        // generate glyphs for each run, and calculate widths of all clusters and runs.
        void Shape();

        // layout all clusters
        void Layout(int_x start, rectix rect, wrapmode_e wrapmode);
    private:
        std::wstring _text;
        HRESULT _hr = S_OK;

        // ����֣�Itemize���������ԶΣ�item �ڵ����ֶ���ͬһ���Եģ�һ�� item �ɱ���֣�Slice���ɶ�� run
        std::vector<scp_item> _items;

        // ����ֵ�ͼ�ζΣ�cluster ��ʾһ�����֡�
        std::vector<scp_cluster> _clusters;

        // ������ͬ�����һ�����֣�run �ڵ����ֶ���ͬһ���Եģ���� run һ�𹹳�һ�� item
        std::vector<scp_run> _runs;

        // -----------------------------��ʾ���
        // glyph indices
        std::vector<uint_16> _glyph_indices;
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