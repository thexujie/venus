#pragma once

#include <usp10.h>

// 跟 d2d 一样，usp10 动态加载.
//
VENUS_BEG

typedef HRESULT(WINAPI * ScriptFreeCacheFunT)(SCRIPT_CACHE * pSc);
typedef HRESULT(WINAPI * ScriptIsComplexFunT)(const char_16 * szText, int_32 iLength, uint_32 uiFlags);
typedef HRESULT(WINAPI * ScriptItemizeFunT)(const char_16 * szText, int_32 iLength, int_32 iMaxItems,
	const SCRIPT_CONTROL * psControl, const SCRIPT_STATE * psState,
	SCRIPT_ITEM * pItems, int_32 * piCount);
typedef HRESULT(WINAPI * ScriptShapeFunT)(HDC hdc, SCRIPT_CACHE * pSc,
	const char_16 * szText, int_32 iLength, int_32 iMaxGlyphs, SCRIPT_ANALYSIS * pSa,
	uint_16 * pwOutGlyphs, uint_16 * pwLogCluse, SCRIPT_VISATTR * pSv, int_32 * piGlyphs);
typedef HRESULT(WINAPI * ScriptPlaceFunT)(HDC hdc, SCRIPT_CACHE * psc, const uint_16 * pwGlyphs, int_32 icGlyphs,
	const SCRIPT_VISATTR * pSv, SCRIPT_ANALYSIS * pSa,
	int_32 * piAdvances, GOFFSET * pOffsets, ABC * pAbc);
typedef HRESULT(WINAPI * ScriptBreakFunT)(const char_16 * szText, int_32 iLength, const SCRIPT_ANALYSIS * pSa, SCRIPT_LOGATTR * pSl);
typedef HRESULT(WINAPI * ScriptGetFontPropertiesFunT)(HDC hdc, SCRIPT_CACHE * pSa, SCRIPT_FONTPROPERTIES * pSf);
typedef HRESULT(WINAPI * ScriptTextOutFunT)(const HDC hdc, SCRIPT_CACHE * pSc,
	int_32 x, int_32 y, UINT uOptions,
	const RECT * pRect,
	const SCRIPT_ANALYSIS * pSa,
	const char_16 * szReserved,
	int_32 iReserved,
	const uint_16 * pwGlyphs, int_32 icGlyphs,
	const int_32 * piAdvance,
	const int_32 * piJustify,
	const GOFFSET * pGoffset);

class WIN32_API UspBase : public CModule
{
public:
	UspBase(HDC hdc);
	~UspBase();

	HDC GetDC() const;
	HRESULT FreeCache(SCRIPT_CACHE * cache);
	HRESULT IsComplex(const char_16 * text, int_x length, uint_32 flags);
	HRESULT Itemize(const char_16 * text, int_x length,
		SCRIPT_ITEM  * items, int_x nitem_max, int_x * pnitem,
		const SCRIPT_CONTROL * control, const SCRIPT_STATE * state);

	HRESULT Shape(SCRIPT_CACHE * cache, 
		const char_16 * text, int_x length, SCRIPT_ANALYSIS * analysis, uint_16 * text_clusters,
		uint_16 * glyphs, SCRIPT_VISATTR * glyph_attrs, int_x nglyph_max, int_x * pnglyph);

	HRESULT Place(SCRIPT_CACHE * cache, SCRIPT_ANALYSIS * analysis,
		const uint_16 * glyphs, const SCRIPT_VISATTR * glyph_attrs, int_32 * advances, GOFFSET * offsets, int_x nglyph,
		ABC * abc);

	HRESULT Break(const char_16 * text, int_x length, const SCRIPT_ANALYSIS * analyses, SCRIPT_LOGATTR * text_attrs);
	HRESULT GetFontProperties(SCRIPT_CACHE * cache, SCRIPT_FONTPROPERTIES * pprop);
	HRESULT TextOutScp(const HDC hdc, SCRIPT_CACHE * cache,
		int_x x, int_x y, uint_32 options,
		const RECT * rect,
		const SCRIPT_ANALYSIS * analysis,
		const uint_16 * glyphs, int_x nglyph,
		const int_32 * advances,
		const int_32 * justfies,
		const GOFFSET * offsets);
	bool LoadUsp10();

protected:
	HDC m_hdc;

	proc_ptr<decltype(ScriptFreeCache)> m_pfnScriptFreeCache;
	proc_ptr<decltype(ScriptIsComplex)> m_pfnScriptIsComplex;
	proc_ptr<decltype(ScriptItemize)> m_pfnScriptItemize;
	proc_ptr<decltype(ScriptShape)> m_pfnScriptShape;
	proc_ptr<decltype(ScriptPlace)> m_pfnScriptPlace;
	proc_ptr<decltype(ScriptBreak)> m_pfnScriptBreak;
	proc_ptr<decltype(ScriptGetFontProperties)> m_pfnScriptGetFontProperties;
	proc_ptr<decltype(ScriptTextOut)> m_pfnScriptTextOut;
};

struct usp_font_t
{
	SCRIPT_CACHE * cache;
	HFONT hfont;
	int_x font_size;
};

class WIN32_API usp_run_t
{
public:
	trange_t trange;
	grange_t grange;
	crange_t crange;
	SCRIPT_ANALYSIS analysis;
	usp_font_t font;
};

class WIN32_API scp_cluster_t
{
public:
	int_x run;
	trange_t trange;
	grange_t grange;
	int_x width;

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

struct usp_cluster_t : public tl_cluster_t
{
	int_x run;
	grange_t grange;
};

bool UspHasMissingGlyphs(const uint_16 * pGlyphs, int_x iLength, const SCRIPT_FONTPROPERTIES & sfp);

class WIN32_API ITextLayoutUsp : public ITextLayout
{
public:
	virtual winerr_t Draw(HDC hdc, int_x iX, int_x iY, uint_32 color, const rectix & rect) const = 0;
};

VENUS_END
