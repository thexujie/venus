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

	virtual void SetText(textw text)
	{
		m_text = text;

		vector<SCRIPT_ITEM> items(text.length() + 1, text.length() + 1);
		int_32 nrun = 0;
		ScriptItemize(text, text.length(), text.length() + 1, nullptr, nullptr, items, &nrun);

		scpitems.reallocate(nrun, nrun);

		int_x cluster_num = 0;
		for(int_x iitem = 0; iitem < nrun; ++iitem)
		{
			scpitem_t & sitem = scpitems[iitem];
			SCRIPT_ITEM & item = items[iitem];
			SCRIPT_ITEM & item_next = items[iitem + 1];
			sitem.sa = item.a;
			sitem.trange = { item.iCharPos, item_next.iCharPos - item.iCharPos };
			sitem.crange = { cluster_num, 0 };

			vector<SCRIPT_LOGATTR> tattrs(sitem.trange.length, sitem.trange.length);

			ScriptBreak(text + sitem.trange.index, sitem.trange.length, &sitem.sa, tattrs);
			for(int_x itext = 0; itext < tattrs.size(); ++itext)
			{
				cluster_t & cluster = clusters.add();
				++cluster_num;

				cluster.item = iitem;
				cluster.trange = { sitem.trange.index + itext, 1 };
				cluster.rtf = { 0, 0 };

				const SCRIPT_LOGATTR & attr_first = tattrs[itext];
				if(attr_first.fSoftBreak)
					cluster.softbreak = true;
				if(attr_first.fWhiteSpace)
					cluster.whitespace = true;

				// check the next char.
				while(itext < tattrs.size() - 1)
				{
					const SCRIPT_LOGATTR & attr = tattrs[itext + 1];

					if(attr.fCharStop || attr.fInvalid)
						break;

					if(attr.fSoftBreak)
						cluster.softbreak = true;
					if(attr.fWhiteSpace)
						cluster.whitespace = true;

					++cluster.trange.length;
					++itext;
				}
			}

			sitem.crange.length = cluster_num - sitem.crange.index;
		}
	}

	virtual int_x IndexFont(font_t font)
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

	virtual void SetFont(crange_t range, font_t font)
	{
		int_x ifont = IndexFont(font);

		for(int_x cnt = 0; cnt < range.length; ++cnt)
		{
			clusters[range.index + cnt].rtf.font = ifont;
		}
	}

	virtual void SetColor(crange_t range, uint_32 color)
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

	void Slicing()
	{
		for(int_x iitem = 0; iitem < scpitems.size(); ++iitem)
		{
			const scpitem_t & scpitem = scpitems[iitem];
			int_x icluster = scpitem.crange.index;
			int_x icluster_end = scpitem.crange.index + scpitem.crange.length;
			while(icluster < icluster_end)
			{
				runitem_t & runitem = runitems.add();
				runitem.sa = scpitem.sa;
				runitem.trange = {};
				runitem.crange = {};
				runitem.rrange = {};

				for( ;icluster < icluster_end; ++icluster)
				{
					const cluster_t & cluster = clusters[icluster];
					// first cluster
					if(!runitem.crange.length)
					{
						runitem.font = cluster.rtf.font;
						runitem.trange = cluster.trange;
						runitem.crange = {icluster, 1};
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
	}

	void Layout(rectix rect);

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

	vector<doc_font_t> caches;

	textw m_text;
	vector<cluster_t> clusters;

	vector<scpitem_t> scpitems;
	vector<runitem_t> runitems;
	vector<rtfitem_t> rtfitems;

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
