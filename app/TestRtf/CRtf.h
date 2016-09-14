#pragma once

#include "BaseInc.h"
#include "Rtf.h"
#include "RtfTag.h"

class CRtfException : public exp_base
{
public:
	CRtfException()
		: exp_base() {}
	CRtfException(uint_x uiNumber, const char_x * szName, const char_x * szWhat)
		: exp_base(uiNumber, szName, szWhat) {}
};

// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------

class RtfFontT
{
public:
	RtfFontT() : m_iIndex(-1), m_iCharSet(0) {}
	int_x m_iIndex;
	int_x m_iCharSet;
	TextX m_name;
	void Reset()
	{
		m_iIndex = -1;
		m_iCharSet = 0;
		m_name.clear();
	}
};

class CRtfParser
{
public:
	CRtfParser();
	~CRtfParser();

	int_x LoadRtf(IRtfDocument * pDoc, const char_8 * pText, const char_8 * pEnd);
	int_x LoadTruck(const char_8 *& pText, const char_8 * pEnd, bool bCanSkip);

	int_x LoadFonts(const char_8 *& pText, const char_8 * pEnd);
	int_x LoadFont(const char_8 *& pText, const char_8 * pEnd);
	int_x LoadColors(const char_8 *& pText, const char_8 * pEnd);
	void ConvertText();
	void FlushText();
protected:
	void SkipTrunk(const char_8 *& pText, const char_8 * pEnd);
	void InitialLocale();
	void UnInitialLocale();
public:

	int_x m_iVersion;
	int_x m_iCodePage;
	int_x m_iDefFont;
	int_x m_iDefLanguage;
	int_x m_iDefLanguageFe;
	int_x m_iViewKind; // 4 Õý³£ÊÓÍ¼


	RichTextFormatT m_rtFormat;
	texta m_text;
	textw m_textw;
	int_x m_iLanguage;

	IRtfDocument * pDoc;
	IRtfParagraph * m_pParagraph;

	vector<RtfFontT> m_fonts;
	vector<uint_32> m_colors;
	linear_map<int_x, _locale_t> m_mapLocale;
};
