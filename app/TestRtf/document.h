#pragma once

#include "BaseInc.h"


void MakeLOGFONT(HDC hdc, const font_t & font, LOGFONT & logFont);

class DocLine
{
};

class IDocFactory
{
public:
	virtual ~IDocFactory()
	{
	}
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

struct urange_t
{
	int_x icolor;
	int_x length;
};

class DocFont
{
};

struct textrtf_t
{
	trange_t trange;
	grange_t grange;
	crange_t crange;
	SCRIPT_ANALYSIS analysis;

	SCRIPT_CACHE * cache;
	HFONT hfont;

	font_t font;
	uint_32 color;
};

#include <usp10.h>
#pragma comment(lib, "usp10.lib")

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

struct rtf_t
{
	int_x font;
	int_x color;

	bool operator ==(const rtf_t & another) const { return font == another.font && color == another.color; }
	bool operator !=(const rtf_t & another) const { return !operator==(another); }
};

class DocTextObject : public DocObject
{
public:
	DocTextObject()
	{
		font_t & font = fonts.add();
		font.name = L"微软雅黑";
		font.size = 32;
		colors.add(Colors::Black);

		HDC hdcScreen = GetDC(NULL);
		m_hdc = CreateCompatibleDC(hdcScreen);
		ReleaseDC(NULL, hdcScreen);
	}

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
		LOGFONT logFont = {};
		MakeLOGFONT(hdc, font, logFont);
		HFONT hFont = CreateFontIndirect(&logFont);
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

	// generate scripts and clusters(by ScriptBreak).
	void Break();
	// generate runs for different font(name、size、bold、italic...)
	void Slice();
	// generate glyphs for each run, and calculate widths of all clusters and runs.
	void Shape();

	// layout all clusters
	void Layout(rectix rect);

	void Draw(HDC hdc, int x, int y);

protected:
	HDC m_hdc;

	struct scpitem_t
	{
		SCRIPT_ANALYSIS sa;
		trange_t trange;
		crange_t crange;
		textw _debug_text;
	};

	struct runitem_t
	{
		SCRIPT_ANALYSIS sa;
		int_x font;
		trange_t trange;
		crange_t crange;
		rrange_t rrange;
		textw _debug_text;
		int_x advance;
	};

	struct rtfitem_t
	{
		SCRIPT_ANALYSIS sa;
		rtf_t rtf;
		trange_t trange;
		crange_t crange;
		int_x advance;
		textw _debug_text;
	};

	struct docline_t
	{
		rrange_t rrange;
	};

	struct cluster_t
	{
		int_x item;
		trange_t trange;
		grange_t grange;
		int_x advance;

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
		rrange_t rrange;
		crange_t crange;

		int_x advance;
		textw _debug_text;
	};

	vector<doc_font_t> caches;

	textw m_text;
	vector<cluster_t> clusters;

	vector<scpitem_t> scpitems;
	vector<runitem_t> runitems;
	vector<rtfitem_t> rtfitems;
	vector<rtfline_t> rtflines;

	vector<font_t> fonts;
	vector<uint_32> colors;


	vector<uint_16> glyphs;
	// attribute
	vector<SCRIPT_VISATTR> gattrs;
	// advance
	vector<int_32> advances;
	// offset
	vector<GOFFSET> offsets;

	vector<uint_16> tclusters;

	// scp cluster
	vector<scp_cluster_t> m_scpclusters;
	// cluster
	vector<usp_cluster_t> m_clusters;
	// line
	vector<tl_line_t> m_lines;


	
};

class Paragraph
{
protected:

	vector<DocLine> lines;
	vector<DocObject *> objects;
};

class Document
{
public:
	Document();
	~Document();

protected:
	vector<Paragraph> paragraphs;
};
