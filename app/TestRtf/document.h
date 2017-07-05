#pragma once

#include "BaseInc.h"

enum wrapmode_e
{
	wrapmode_none,
	wrapmode_char,
	wrapmode_word,
};

class DocObject
{
public:
	DocObject()
	{
	}

	virtual ~DocObject()
	{
	}
};

#include <usp10.h>
#pragma comment(lib, "usp10.lib")

struct rtf_t
{
	int_x font;
	int_x color;

	bool operator ==(const rtf_t & another) const { return font == another.font && color == another.color; }
	bool operator !=(const rtf_t & another) const { return !operator==(another); }
};

struct doc_font_t
{
	SCRIPT_CACHE * cache;
	HFONT hfont;
	int_x font_size;
	font_t font;
};

struct doc_font_cache_t
{
	HFONT hFont;
	SCRIPT_CACHE * cache;
	font_t font;
};

// rtf range
struct rrange_t
{
	int_x index;
	int_x length;
};

struct runitem_t
{
	SCRIPT_ANALYSIS sa;
	int_x scp;
	int_x font;
	trange_t trange;
	crange_t crange;
	grange_t grange;
	rrange_t rrange;
	int_x advance;
	textw _debug_text;
};

struct scpitem_t
{
	SCRIPT_ANALYSIS sa;
	trange_t trange;
	crange_t crange;
	textw _debug_text;
};

struct rtfitem_t
{
	int_x run;
	int_x scp;
	int_x line;
	rtf_t rtf;

	trange_t trange;
	crange_t crange;
	int_x advance;
	int_x offset;

	textw _debug_text;
};

struct rtfcluster_t
{
	int_x run;
	int_x scp;
	trange_t trange;
	grange_t grange;
	int_x width;
	int_x height;

	rtf_t rtf;

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

	textw _debug_text;
};

struct rtfline_t
{
	int_x line;
	rrange_t rrange;
	crange_t crange;
	trange_t trange;

	int_x advance;
	tlrect_t rect;
	textw _debug_text;
};

class DocTextObject : public DocObject, public ObjectT<ITextLayout>
{
public:
	DocTextObject();

	virtual ~DocTextObject()
	{
		DeleteDC(m_hdc);
	}

	int_x IndexFont(font_t font)
	{
		int_x ifont = -1;
		for(int_x cnt = 0; cnt < fonts.size(); ++cnt)
		{
			if(fonts[cnt] == font)
			{
				ifont = cnt;
				break;
			}
		}

		if(ifont < 0)
		{
			ifont = fonts.size();
			fonts.add(font);
		}
		return ifont;
	}

	void SetFont(crange_t range, font_t font)
	{
		int_x ifont = IndexFont(font);

		for(int_x cnt = 0; cnt < range.length; ++cnt)
		{
			clusters[range.index + cnt].rtf.font = ifont;
		}
	}

	void SetColor(crange_t range, uint_32 color)
	{
		int_x icolor = -1;
		for(int_x cnt = 0; cnt < colors.size(); ++cnt)
		{
			if(colors[cnt] == color)
			{
				icolor = cnt;
				break;
			}
		}

		if(icolor < 0)
		{
			icolor = colors.size();
			colors.add(color);
		}

		for(int_x cnt = 0; cnt < range.length; ++cnt)
		{
			clusters[range.index + cnt].rtf.color = icolor;
		}
	}

	doc_font_t GetFont(int_x ifont)
	{
		return GetFont(fonts[ifont]);
	}

	doc_font_t GetFont(font_t font)
	{
		for(int_x cnt = 0; cnt < caches.size(); ++cnt)
		{
			if(caches[cnt].font == font)
			{
				return caches[cnt];
			}
		}

		HDC hdc = m_hdc;
		LOGFONTW logFont = Win32::MappingFont(font);
		HFONT hFont = CreateFontIndirectW(&logFont);
		if(!hFont)
			throw exp_bad_state();

		doc_font_t docfont;
		docfont.font = font;
		docfont.hfont = hFont;
		docfont.cache = new SCRIPT_CACHE;
		*docfont.cache = nullptr;

		return caches.add(docfont);
	}

	doc_font_t GetFontFallBack(const font_t & font, int_x iLanguage, const char_16 * text = nullptr, int_x length = 0);

	void SetText(textw text);
	int_x GetClusterCount() const { return clusters.size(); }
	// generate scripts and clusters(by ScriptBreak).
	void Break();
	// generate runs for different font(name、size、bold、italic...)
	void Slice();
	// generate glyphs for each run, and calculate widths of all clusters and runs.
	void Shape();

	// layout all clusters
	void Layout(int_x start, rectix rect, wrapmode_e wrapmode);

	void Draw(HDC hdc, int_x, int_x y, rectix rect);

	void Destroy();

public:
	err_t Initialize(IDocSource * pSrouce) { return err_ok; }
	IDocSource * GetSource() const { return m_source; }

	err_t Layout(trange_t range, int_x iWidth, paragraph_tag_e tag = paragraph_tag_none);
	err_t Branch(int_x iWidth);

	tl_metrics_t GetMetrics() const;

	tl_cluster_t FindCluster(int_x iIndex) const;
	tl_cluster_t GetCluster(int_x iCluster) const;
	tl_line_t GetLine(int_x iLine) const;

	tl_range_t HitTest(pointix point) const;
	int_x HitTestRange(int_x iIndex, int_x iLength, tl_range_t * rects, int_x iCount) const;
protected:
	IDocSource * m_source;
	HDC m_hdc;

	vector<doc_font_t> caches;

	textw m_text;

	vector<rtfcluster_t> clusters;

	vector<scpitem_t> scpitems;
	vector<runitem_t> runitems;
	vector<rtfitem_t> rtfitems;
	vector<rtfline_t> rtflines;

	vector<font_t> fonts;
	vector<uint_32> colors;

	vector<uint_16> glyphs;
	// attribute, just for Shape()
	vector<SCRIPT_VISATTR> gattrs;
	// advance
	vector<int_32> advances;
	// offset
	vector<GOFFSET> offsets;

	vector<uint_16> tclusters;
};
