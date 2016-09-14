#include "stdafx.h"
#include "CRtf.h"

static int_x RtfSkipWhiteSpace(const char_8 *& pText, const char_8 * pEnd)
{
	int_x iCount = 0;
	bool bReading = true;
	while(bReading && pText < pEnd)
	{
		switch(*pText)
		{
		case '\r':
		case '\n':
		case '\t':
		case ' ':
			++iCount;
			++pText;
			break;
		default:
			bReading = false;
			break;
		}
	}
	return iCount;
}

static int_x RtfSkipLineTag(const char_8 *& pText, const char_8 * pEnd)
{
	int_x iCount = 0;
	bool bReading = true;
	while(bReading && pText < pEnd)
	{
		switch(*pText)
		{
		case '\r':
		case '\n':
			++iCount;
			++pText;
			break;
		default:
			bReading = false;
			break;
		}
	}
	return iCount;
}

/**寻找字符，定位到目标字符的下一个位置，如果没找到，就返回pEnd。*/
static int_x RtfReadTag(const char_8 *& pText, const char_8 * pEnd,
						RtfTagE & eRtfTag, int_x & iNumber, bool & bHasNumber)
{
	if(pText >= pEnd)
		return 1;
	RtfSkipLineTag(pText, pEnd);
	if(*pText == '{')
	{
		eRtfTag = RtfTag_trunk_beg;
		iNumber = 0;
		bHasNumber = false;
		return 0;
	}
	else if(*pText == '}')
	{
		eRtfTag = RtfTag_trunk_end;
		iNumber = 0;
		bHasNumber = false;
		return 0;
	}
	else if(*pText == ';')
	{
		++pText;
		eRtfTag = RtfTag_semicolon;
		iNumber = 0;
		bHasNumber = false;
		return 0;
	}
	else {}

	// 是一个ansi字符。
	if(*pText != '\\')
	{
		eRtfTag = RtfTag_char;
		iNumber = *pText;
		bHasNumber = true;
		++pText;
		return 0;
	}

	++pText;
	// 转义的字符 { }
	if(*pText == '{' || *pText == '}' || *pText == '\\')
	{
		eRtfTag = RtfTag_char;
		iNumber = *pText;
		bHasNumber = true;
		++pText;
		return 0;
	}
	// 是一个ansi格式的unicode字符。
	else if(*pText == '\'')
	{
		++pText;
		if(pEnd - pText < 2)
			return 3;

		char_8 ch = *pText;
		int_x iChar = 0;
		if('a' <= ch && ch <= 'z')
			iChar |= (ch - 'a' + 10) << 4;
		else if('A' <= ch && ch <= 'A')
			iChar |= (ch - 'a' + 10) << 4;
		else if('0' <= ch && ch <= '9')
			iChar |= (ch - '0') << 4;
		else
			return 4;
		++pText;

		ch = *pText;
		if('a' <= ch && ch <= 'z')
			iChar |= (ch - 'a' + 10);
		else if('A' <= ch && ch <= 'Z')
			iChar |= (ch - 'A' + 10);
		else if('0' <= ch && ch <= '9')
			iChar |= (ch - '0');
		else
			return 4;
		++pText;

		eRtfTag = RtfTag_char;
		iNumber = iChar;
		bHasNumber = true;
		return 0;
	}

	bool bReading = true;
	const char_8 * pRead = pText;
	const char_8 * pNumber = nullptr;
	char_8 ch = 0;

	while(bReading && pRead < pEnd)
	{
		ch = *pRead;
		if(('a' <= ch && ch <= 'z') ||
		   ('A' <= ch && ch <= 'Z'))
		   ++pRead;
		else if(ch == '-' || ('0' <= ch && ch <= '9'))
		{
			if(!pNumber)
				pNumber = pRead;
			++pRead;
		}
		else if(ch == '?')
		{
			++pRead;
			break;
		}
		else
			break;
	}

	if(pNumber < pEnd && pRead < pEnd)
	{
		if(pNumber > pText)
		{
			eRtfTag = GetRtfTag(pText, pNumber);
			bHasNumber = true;
			iNumber = texttoix(pNumber, pRead - pNumber, 10);
		}
		else
		{
			eRtfTag = GetRtfTag(pText, pRead);
			bHasNumber = false;
			iNumber = 0;
		}
		if(pRead < pEnd && *pRead == ' ')
			++pRead;
		pText = pRead;
		return 0;
	}
	else
		return 3;
}

CRtfParser::CRtfParser()
{
	InitialLocale();
}

CRtfParser::~CRtfParser()
{
	UnInitialLocale();
}

int_x CRtfParser::LoadRtf(IRtfDocument * pDoc2, const char_8 * pText, const char_8 * pEnd)
{
	pDoc = pDoc2;
	m_pParagraph = pDoc->AddParagraph(m_rtFormat);
	LoadTruck(pText, pEnd, false);
	m_pParagraph = nullptr;
	return 0;
}

int_x CRtfParser::LoadTruck(const char_8 *& pText, const char_8 * pEnd, bool bCanSkip)
{
	if(!pText || !pEnd || pText >= pEnd)
		return 1;
	// 块都是以 { 开始，以 } 结束的。
	if(*pText != '{')
		return 2;

	++pText;

	bool bReading = true;
	RtfTagE eRtfTag = RtfTag_invalid;
	int_x iNumber = 0;
	bool bHasNumber = false;
	int_x iState = 0;
	while(bReading && pText < pEnd)
	{
		iState = RtfReadTag(pText, pEnd, eRtfTag, iNumber, bHasNumber);
		if(iState)
			return 4;

		switch(eRtfTag)
		{
		case RtfTag_rtf:
			m_iVersion = iNumber;
			break;
		case RtfTag_ansi:
			break;
		case RtfTag_ansicpg:
			m_iCodePage = iNumber;
			break;
		case RtfTag_deff:
			m_iDefFont = iNumber;
			break;
		case RtfTag_nouicompat:
			break;
		case RtfTag_deflang:
			m_iDefLanguage = iNumber;
			break;
		case RtfTag_deflangfe:
			m_iDefLanguageFe = iNumber;
			break;
		case RtfTag_trunk_beg:
			RtfSkipWhiteSpace(pText, pEnd);
			iState = LoadTruck(pText, pEnd, true);
			if(iState)
				return 5;
			break;
		case RtfTag_viewkind:
			m_iViewKind = iNumber;
			break;
		case RtfTag_uc:
			break;
		case RtfTag_cf:
			FlushText();
			if(iNumber > 0)
				m_rtFormat.uiColor = m_colors[iNumber - 1];
			else
				m_rtFormat.uiColor = 0;
			break;
		case RtfTag_f:
			FlushText();
			m_rtFormat.rtFont.Face = m_fonts[iNumber].m_name;
			m_rtFormat.rtFont.CharSet = m_fonts[iNumber].m_iCharSet;
			break;
		case RtfTag_fs:
			FlushText();
			m_rtFormat.rtFont.Size = iNumber / 2;
			break;
		case RtfTag_ul:
			FlushText();
			m_rtFormat.rtFont.UnderLine = true;
			break;
		case RtfTag_ulnone:
			FlushText();
			m_rtFormat.rtFont.UnderLine = false;
			break;
		case RtfTag_b:
			FlushText();
			m_rtFormat.rtFont.Weight = bHasNumber ? 400 : 800;
			break;
		case RtfTag_char:
			m_text.append((char_8)iNumber);
			break;
		case RtfTag_tab:
			m_text.append('\t');
			break;
		case RtfTag_semicolon:
			m_text.append(';');
			break;
		case RtfTag_u:
			ConvertText();
			m_textw.append((char_16)iNumber);
			break;

		case RtfTag_pard:
			break;
		case RtfTag_par:
			FlushText();
			m_pParagraph = pDoc->AddParagraph(m_rtFormat);
			LoadTruck(pText, pEnd, false);
			break;
		case RtfTag_trunk_end:
			++pText;
			bReading = false;
			break;


		case RtfTag_fonttbl:
			RtfSkipWhiteSpace(pText, pEnd);
			LoadFonts(pText, pEnd);
			break;
		case RtfTag_colortbl:
			RtfSkipWhiteSpace(pText, pEnd);
			LoadColors(pText, pEnd);
			break;
		case RtfTag_ref:
			SkipTrunk(pText, pEnd);
			break;
		case RtfTag_lang:
			m_iLanguage = iNumber;
			break;
		default:
			if(bCanSkip)
				SkipTrunk(pText, pEnd);
			break;
		}
	}
	return 0;
}

int_x CRtfParser::LoadFonts(const char_8 *& pText, const char_8 * pEnd)
{
	if(pText >= pEnd)
		return 1;

	int_x iState = 0;
	bool bReading = true;
	RtfTagE eRtfTag = RtfTag_invalid;
	int_x iNumber = 0;
	bool bHasNumber = false;
	while(bReading && pText < pEnd)
	{
		iState = RtfReadTag(pText, pEnd, eRtfTag, iNumber, bHasNumber);
		if(iState)
			return 2;

		switch(eRtfTag)
		{
		case RtfTag_trunk_beg:
			RtfSkipWhiteSpace(pText, pEnd);
			LoadFont(pText, pEnd);
			break;
		case RtfTag_trunk_end:
			//++pText; // 因为这是具体的子对象。
			bReading = false;
			break;
		default:
			return 3;
		}
	}
	return 0;
}

int_x CRtfParser::LoadFont(const char_8 *& pText, const char_8 * pEnd)
{
	if(pText >= pEnd)
		return 1;

	if(*pText != '{')
		return 2;

	RtfFontT font;
	texta name;
	int_x iState = 0;
	++pText;
	bool bReading = true;
	RtfTagE eRtfTag = RtfTag_invalid;
	int_x iNumber = 0;
	bool bHasNumber = false;
	while(bReading && pText < pEnd)
	{
		iState = RtfReadTag(pText, pEnd, eRtfTag, iNumber, bHasNumber);
		if(iState)
			return 3;
		switch(eRtfTag)
		{
		case RtfTag_f:
			font.m_iIndex = iNumber;
			break;
		case RtfTag_fcharset:
			font.m_iCharSet = iNumber;
			break;
		case RtfTag_fnil:
			break;
		case RtfTag_char:
			name.append((char_8)iNumber);
			break;
		case RtfTag_semicolon:
			break;
		case RtfTag_trunk_end:
			++pText;
			bReading = false;
			break;
		default:
			break;
		}
	}
#ifdef _UNICODE
	font.m_name = tounicodetextex(name, name.length(), m_mapLocale[font.m_iCharSet]);
#else
	font.m_name = name;
#endif
	m_fonts.add(font);
	return 0;
}

int_x CRtfParser::LoadColors(const char_8 *& pText, const char_8 * pEnd)
{
	if(pText >= pEnd)
		return 1;

	uint_32 uiColor = 0;
	int_x iState = 0;
	bool bReading = true;
	RtfTagE eRtfTag = RtfTag_invalid;
	int_x iNumber = 0;
	bool bHasNumber = false;
	bool bColor = false;
	while(bReading && pText < pEnd)
	{
		iState = RtfReadTag(pText, pEnd, eRtfTag, iNumber, bHasNumber);
		if(iState)
			return 2;

		switch(eRtfTag)
		{
		case RtfTag_red:
			uiColor |= iNumber & 0xFF;
			bColor = true;
			break;
		case RtfTag_green:
			uiColor |= (iNumber << 8) & 0xFF00;
			bColor = true;
			break;
		case RtfTag_blue:
			uiColor |= (iNumber << 16) & 0xFF0000;
			bColor = true;
			break;
		case RtfTag_trunk_end:
			//++pText; // 因为是一个具体的子对象。
			bReading = false;
			break;
		case RtfTag_semicolon:
			if(bColor)
			{
				m_colors.add(uiColor);
				uiColor = 0;
				bColor = false;
			}
			break;
		default:
			return 3;
		}
	}
	return 0;
}

void CRtfParser::ConvertText()
{
	if(m_text.is_valid())
	{
		textw tw = tounicodetextex(m_text, m_text.length(), m_mapLocale[m_rtFormat.rtFont.CharSet]);
		m_textw += tw;
		m_text.clear();
	}
}

void CRtfParser::FlushText()
{
	ConvertText();
	if(m_pParagraph && m_textw.is_valid())
	{
		m_pParagraph->AddRichText(m_textw, m_textw.length(), m_rtFormat);
		m_textw.clear();
	}
}

void CRtfParser::SkipTrunk(const char_8 *& pText, const char_8 * pEnd)
{
	int_32 iDepth = 1;
	while(iDepth && pText < pEnd)
	{
		switch(*pText)
		{
		case '{':
			++iDepth;
			++pText;
			break;
		case '}':
			--iDepth;
			if(iDepth)
				++pText;
			break;
		default:
			++pText;
			break;
		}
	}
}

#include <locale.h>
#if MSVS >= MSVS_2012 
#ifdef _UNICODE
#define _tcreate_locale _wcreate_locale
#else
#define _tcreate_locale _create_locale
#endif // _UNICODE
#define LOCAL_T(x) _T(x)
#else
#define LOCAL_T(x) x
#define _tcreate_locale _create_locale
#endif

void CRtfParser::InitialLocale()
{
	m_mapLocale[0] = _tcreate_locale(LC_ALL, LOCAL_T("English_United States.1252"));
	m_mapLocale[1] = _tcreate_locale(LC_ALL, LOCAL_T(""));
	m_mapLocale[134] = _tcreate_locale(LC_ALL, LOCAL_T("Chinese-Simplified_China.936"));
	m_mapLocale[136] = _tcreate_locale(LC_ALL, LOCAL_T("Chinese-Traditional_China.950"));
#if MSVS >= MSVS_2012 
	m_mapLocale[222] = _tcreate_locale(LC_ALL, LOCAL_T("Thai_Thailand.874"));
#else
	m_mapLocale[222] = _tcreate_locale(LC_ALL, LOCAL_T("English_United States.874"));
#endif
}

void CRtfParser::UnInitialLocale()
{
	for(int_x cnt = 0, len = m_mapLocale.size(); cnt != len; ++cnt)
		_free_locale(m_mapLocale.at(cnt).value);
}
