#pragma once

#include "BaseInc.h"
#include "Rtf.h"

#include <usp10.h>
#pragma comment(lib, "usp10.lib")

//#define SCRIPT_DEBUG 1

class CScriptEngine;
class CScriptDocument;

/**
 * @brief Engine �����ڴ�ŵ����建�档   
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
 * @brief Engine ���ʾһ�ξ�����ͬ RichTextFormatT ���ı���
 * ������ı��� Paragraph ���棬����ֻ����������
 */
struct ScriptRichTextT
{
	// Unicode ����ʼ�����������ȡ�iLen = iEnd - iBeg��
	int_x iBeg;
	int_x iEnd;
	int_x iLen;
	// ��ʽ���������Ϊ��������ʾ���ظ�ǰһ����ʽ����iFormat = ~iFormat��
	int_x iFormat;
#ifdef SCRIPT_DEBUG_TEXT
	TextX text;
#endif // SCRIPT_DEBUG_TEXT
};

/**
 * @brief ��ʾ�����һ�����ɼ����ַ���   
 */
class UsCharT
{
public:
	UsCharT() { buffclr(*this); }
	UsCharT(int_x a, int_x b, int_x c, int_x d, int_x e, int_x f)
		: iBeg(a), iEnd(a), iLen(c), iGlyphBeg(d), iGlyphEnd(e), iGlyphLen(f), iFormat(0)
	{

	}

	// Unicode ����ʼ�����������ȡ�iLen = iEnd - iBeg��
	int_x iBeg;
	int_x iEnd;
	int_x iLen;

	// Glyph ����ʼ�����������ȡ�iLen = iEnd - iBeg��
	int_x iGlyphBeg;
	int_x iGlyphEnd;
	int_x iGlyphLen;

	// �ַ��Ŀ��
	int_x iWidth;
	// �ַ��ĸ�ʽ��ͬ ScriptRichTextT::iFormat
	int_x iFormat;
	// ��Ӧ�� SA��
	SCRIPT_ANALYSIS sa;
	// �Ƿ��ǽ��黻��λ�á�
	bool bSoftBreak;
};

/**
 * @brief ��ʾһ���ı���
 */
class UsLineT
{
public:
	UsLineT() { buffclr(*this); }

	// Unicode ����ʼ�����������ȡ�iLen = iEnd - iBeg��
	int_x iBeg;
	int_x iEnd;
	int_x iLen;

	// �ַ��ĸ�ʽ��ͬ ScriptRichTextT::iFormat
	int_x iFormat;
	// �иߣ�������ߵ��ַ��ĸ߶ȡ�
	int_x iHeight;
	// Ascent����������ַ���Ascent��
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

	// ��ʽ��
	vector<RichTextFormatT> m_formats;
	// ���ı���
	vector<ScriptRichTextT> m_texts;

	// �����ַ������� Generate ��õ���
	vector<UsCharT> m_chars;
	// �����С����� GenerateLine ��õ���
	vector<UsLineT> m_lines;

	// ����߶ȡ�
	int_x iHeight;
	int_x iPage;
	// ������ı���
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
 * @brief �ṩ���建��ȡ�����   
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