#pragma once

#include "BaseInc.h"
#include "Rtf.h"

#include <usp10.h>
#pragma comment(lib, "usp10.lib")

//#define SCRIPT_DEBUG 1

class CScriptEngine;
class CScriptDocument;

/**
 * @brief Engine 里用于存放的字体缓存。   
 */
struct ScriptFontT
{
public:
	ScriptFontT() { buffclr(*this); }

	uint_x uiId;
	HFONT hFont;
	RichTextMetricT rtMetric;
	SCRIPT_CACHE sCache;
#ifdef SCRIPT_DEBUG
	RichTextFontT rtFont;
#endif
};

/**
 * @brief Engine 里表示一段具有相同 RichTextFormatT 的文本。
 * 具体的文本由 Paragraph 保存，这里只保留索引。
 */
struct ScriptRichTextT
{
	// Unicode 的起始、结束、长度。iLen = iEnd - iBeg。
	int_x iBeg;
	int_x iEnd;
	int_x iLen;
	// 格式索引，如果为负数，表示是重复前一个格式，且iFormat = ~iFormat。
	int_x iFormat;
#ifdef SCRIPT_DEBUG_TEXT
	TextX text;
#endif // SCRIPT_DEBUG_TEXT
};

/**
 * @brief 表示具体的一个、可见的字符。   
 */
class UsCharT
{
public:
	UsCharT() { buffclr(*this); }
	UsCharT(int_x a, int_x b, int_x c, int_x d, int_x e, int_x f)
		: iBeg(a), iEnd(a), iLen(c), iGlyphBeg(d), iGlyphEnd(e), iGlyphLen(f), iFormat(0)
	{

	}

	// Unicode 的起始、结束、长度。iLen = iEnd - iBeg。
	int_x iBeg;
	int_x iEnd;
	int_x iLen;

	// Glyph 的起始、结束、长度。iLen = iEnd - iBeg。
	int_x iGlyphBeg;
	int_x iGlyphEnd;
	int_x iGlyphLen;

	// 字符的宽度
	int_x iWidth;
	// 字符的格式，同 ScriptRichTextT::iFormat
	int_x iFormat;
	// 对应的 SA。
	SCRIPT_ANALYSIS sa;
	// 是否是建议换行位置。
	bool bSoftBreak;
};

/**
 * @brief 表示一行文本。
 */
class UsLineT
{
public:
	UsLineT() { buffclr(*this); }

	// Unicode 的起始、结束、长度。iLen = iEnd - iBeg。
	int_x iBeg;
	int_x iEnd;
	int_x iLen;

	// 字符的格式，同 ScriptRichTextT::iFormat
	int_x iFormat;
	// 行高，该行最高的字符的高度。
	int_x iHeight;
	// Ascent，该行最高字符的Ascent。
	int_x iAscent;
};

class CScriptParagraph : public IRtfParagraph
{
public:
	CScriptParagraph();
	CScriptParagraph(const RichTextFormatT & defRtFormat);

	void AddRichText(const char_8 * szText, int_x iLength = -1);
	void AddRichText(const char_16 * szText, int_x iLength = -1);

	void AddRichText(const char_8 * szText, int_x iLength, const RichTextFormatT & rtFormat);
	void AddRichText(const char_16 * szText, int_x iLength, const RichTextFormatT & rtFormat);

	void Generate(CScriptEngine * pEngine);
	void GenerateLine(CScriptEngine * pEngine, int_x iWidth);

	void Draw(CScriptEngine * pEngine, HDC hdc, int_x iX, int_x iY, int_x iWidth, int_x iHeight);

	// Glyph
	vector<WORD> m_glyphs;
	// Advance
	vector<int_32> m_advances;
	// Offset
	vector<GOFFSET> m_offsets;

	// 格式。
	vector<RichTextFormatT> m_formats;
	// 富文本。
	vector<ScriptRichTextT> m_texts;

	// 整个字符。调用 Generate 后得到。
	vector<UsCharT> m_chars;
	// 整个行。调用 GenerateLine 后得到。
	vector<UsLineT> m_lines;

	// 段落高度。
	int_x iHeight;
	int_x iPage;
	// 段落的文本。
	textw m_text;
};

class CScriptDocument : public IRtfDocument
{
public:
	CScriptDocument();
	~CScriptDocument();
	void Destroy();

	void Reset();
	IRtfParagraph * AddParagraph(const RichTextFormatT & rtFormat);

	void LoadText(const char_16 * szText, int_x iLength, const RichTextFormatT & rtFormat);
	void Generate(CScriptEngine * pEngine);
	void GenerateLine(CScriptEngine * pEngine, int_x iWidth, int_x iHeight = -1);
	void Draw(CScriptEngine * pEngine, HDC hdc, int_x iX, int_x iY, int_x iWidth, int_x iHeight);

	int_x GetHeight() const;

	vector<CScriptParagraph> m_arrParagraphs;
};

/**
 * @brief 提供字体缓存等。。。   
 */
class CScriptEngine
{
public:
	CScriptEngine();
	~CScriptEngine();

	void Destroy();

	HDC GetHDC();
	ScriptFontT & GetScriptFont(const RichTextFontT & rtFont);
	ScriptFontT & GetScriptFont(uint_x uiFontId);

	HDC m_hdc;
	vector<ScriptFontT> m_fonts;
};

class CScriptException
{

};