#include "stdafx.h"
#include "Uniscribe.h"
#include <stdio.h>

static uint_x RichTextFontHashFunc(const RichTextFontT & rtFont);
// --------------------------------------------------------------- RichParagraphT
CScriptParagraph::CScriptParagraph()
{
	RichTextFormatT rtFormat;
	rtFormat.rtFont.Face = _T("微软雅黑");
	rtFormat.rtFont.Size = 18;
	m_formats.add(rtFormat);
	iHeight = 0;
}

CScriptParagraph::CScriptParagraph(const RichTextFormatT & defRtFormat)
{
	m_formats.add(defRtFormat);
	iHeight = 0;
}

void CScriptParagraph::AddRichText(const char_8 * szText, int_x iLength)
{
	textw text = tounicodetext(szText, iLength);
	AddRichText(text.buffer(), text.length());
}

void CScriptParagraph::AddRichText(const char_16 * szText, int_x iLength)
{
	if(iLength < 0)
		iLength = textlen(szText);

	if(!iLength)
		return;

	if(m_texts.is_empty())
	{
		ScriptRichTextT & srt = m_texts.add();
		srt.iBeg = 0;
		srt.iEnd = iLength;
		srt.iLen = srt.iEnd - srt.iBeg;
		srt.iFormat = 0;

#ifdef SCRIPT_DEBUG_TEXT
		srt.text = szText;
#endif // SCRIPT_DEBUG_TEXT

		m_text = szText;
	}
	else
	{
		ScriptRichTextT & srtLast = m_texts.back();
		srtLast.iEnd += iLength;
		srtLast.iLen = srtLast.iEnd - srtLast.iBeg;
#ifdef SCRIPT_DEBUG_TEXT
		srtLast.text += szText;
#endif // SCRIPT_DEBUG_TEXT
		m_text += szText;
	}
}

void CScriptParagraph::AddRichText(const char_8 * szText, int_x iLength, const RichTextFormatT & rtFormat)
{
	textw text = tounicodetext(szText, iLength);
	AddRichText(text.buffer(), text.length(), rtFormat);
}

void CScriptParagraph::AddRichText(const char_16 * szText, int_x iLength, const RichTextFormatT & rtFormat)
{
	if(iLength < 0)
		iLength = textlen(szText);

	if(!iLength)
		return;

	if(m_texts.is_empty())
	{
		ScriptRichTextT & srt = m_texts.add();
		srt.iBeg = 0;
		srt.iEnd = iLength;
		srt.iLen = srt.iEnd - srt.iBeg;
		srt.iFormat = m_formats.size();
		m_formats.add(rtFormat);

#ifdef SCRIPT_DEBUG_TEXT
		srt.text = szText;
#endif // SCRIPT_DEBUG_TEXT

		m_text = szText;
	}
	else
	{
		ScriptRichTextT & srtLast = m_texts.back();
		if(m_formats[srtLast.iFormat] == rtFormat)
		{
			srtLast.iEnd += iLength;
			srtLast.iLen = srtLast.iEnd - srtLast.iBeg;

			m_text += szText;
		}
		else
		{
			ScriptRichTextT & srt = m_texts.add();
			srt.iBeg = m_text.length();
			srt.iEnd = srt.iBeg + iLength;
			srt.iLen = iLength;
			srt.iFormat = m_formats.size();
			m_formats.add(rtFormat);
#ifdef SCRIPT_DEBUG_TEXT
			srt.text += szText;
#endif // SCRIPT_DEBUG_TEXT

			m_text += szText;
		}
	}
}

void CScriptParagraph::Generate(CScriptEngine * pEngine)
{
	if(m_text.is_valid())
	{
		SCRIPT_CACHE * pCache = NULL;
		int_x iGlyph = 0;
		HDC hdc = pEngine->GetHDC();
		HRESULT hr = S_OK;
		const char_16 * pRichText = nullptr;
		int_x iRichLength = 0;
		int_x iFormat = 0;

		vector<SCRIPT_ITEM> arrItems;
		vector<WORD> clusts;
		vector<SCRIPT_VISATTR> visAttrs;
		vector<SCRIPT_LOGATTR> logAttrs;

		for(int_x irt = 0, len = m_texts.size(); irt != len; ++irt)
		{
			const ScriptRichTextT & srt = m_texts[irt];
			if(srt.iLen == 0)
				continue;
			iFormat = srt.iFormat;
			const RichTextFormatT & rtf = m_formats[iFormat];

			pRichText = m_text + srt.iBeg;
			iRichLength = srt.iLen;

			ScriptFontT & sFont = pEngine->GetScriptFont(rtf.rtFont);

			pCache = &sFont.sCache;

			HGDIOBJ hOldFont = SelectObject(hdc, sFont.hFont);

			//SCRIPT_DIGITSUBSTITUTE sds = {0};
			//ScriptRecordDigitSubstitution(LOCALE_USER_DEFAULT, &sds);
			//SCRIPT_CONTROL sc = {0};
			//SCRIPT_STATE ss = {0};
			//ScriptApplyDigitSubstitution(&sds, &sc, &ss);

			int_32 iItemCount = 0;
			arrItems.resize(iRichLength + 1);
			hr = ScriptItemize(pRichText, (int_32)iRichLength, (int_32)(iRichLength + 1), /*&sc, &ss, */NULL, NULL, arrItems, &iItemCount);
			Assert(hr == S_OK);

			arrItems.resize(iItemCount + 1);

			for(int_x ii = 0; ii < iItemCount; ++ii)
			{
				SCRIPT_ITEM & si = arrItems[ii];
				SCRIPT_ITEM & siNext = arrItems[ii + 1];

				const char_16 * pText = pRichText + si.iCharPos;
				int_x iLength = siNext.iCharPos - si.iCharPos;


				int_x iMaxGlyphs = iLength * 3 / 2 + 16;
				m_glyphs.resize(iGlyph + iMaxGlyphs);
				clusts.resize(iLength);
				visAttrs.resize(iMaxGlyphs);

				int_32 iGlyphCount = 0;
				hr = ScriptShape(hdc, pCache,
								 pText, (int_32)iLength,
								 (int_32)iMaxGlyphs, &si.a,
								 m_glyphs + iGlyph,
								 clusts,
								 visAttrs,
								 &iGlyphCount);

				switch(hr)
				{
				case E_OUTOFMEMORY:
					Assert(false);
					Win32::FormatWinError();
					break;
				case E_PENDING:
					Assert(false);
					Win32::FormatWinError();
					break;
				case USP_E_SCRIPT_NOT_IN_FONT:
					Win32::FormatWinError();
					Assert(false);
					break;
				case S_OK:
					break;
				default:
					Assert(false);
					break;
				}

				visAttrs.resize(iGlyphCount);

				m_glyphs.resize(iGlyph + iGlyphCount);
				m_advances.resize(iGlyph + iGlyphCount);
				m_offsets.resize(iGlyph + iGlyphCount);

				ABC abc = {};
				hr = ScriptPlace(hdc, pCache,
								 m_glyphs + iGlyph,
								 iGlyphCount,
								 visAttrs,
								 &si.a,
								 m_advances + iGlyph,
								 m_offsets + iGlyph,
								 &abc);

				Assert(hr == S_OK);

				logAttrs.resize(iLength);
				hr = ScriptBreak(pText, (int_32)iLength, &si.a, logAttrs);
				Assert(hr == S_OK);

				UsCharT uc(si.iCharPos, si.iCharPos, 0, iGlyph, iGlyph, 0);
				int_x iClust = 0, iTextIndex = 0;
				int_x iGlyphLen = 0;
				int_x iIndex = 0;
				int_x iCharBeg = srt.iBeg + si.iCharPos;
				while(iIndex != iGlyphCount)
				{
					if(visAttrs[iIndex].fClusterStart)
					{
						UsCharT & usch = m_chars.add();

						// SoftBreak
						if(logAttrs[iTextIndex].fSoftBreak || !iTextIndex)
							usch.bSoftBreak = true;
						else
							usch.bSoftBreak = false;

						// 定位字符
						usch.iBeg = iCharBeg + iTextIndex;
						iClust = clusts[iTextIndex++];
						while(iTextIndex != iLength)
						{
							if(clusts[iTextIndex] != iClust)
								break;
							else
								++iTextIndex;
						}
						usch.iEnd = iCharBeg + iTextIndex;
						usch.iLen = usch.iEnd - usch.iBeg;

						// 定位Glyph
						usch.iGlyphBeg = iGlyph + iIndex;
						usch.iGlyphLen = 1;

						++iIndex;
						while(iIndex != iGlyphCount)
						{
							if(!visAttrs[iIndex].fClusterStart)
							{
								++usch.iGlyphLen;
								++iIndex;
							}
							else
								break;
						}
						usch.iGlyphEnd = usch.iGlyphBeg + usch.iGlyphLen;

						// 计算宽度
						if(m_text[usch.iBeg] == '\t')
							m_advances[usch.iGlyphBeg] = (int_32)(4 * sFont.rtMetric.AveCharWidth);

						usch.iWidth = m_advances[usch.iGlyphBeg];
						usch.sa = si.a;

						usch.iFormat = iFormat;
						iFormat = ~iFormat;
					}
					else 
					{
						++iIndex;
					}
				}
				iGlyph += iGlyphCount;
			}
			SelectObject(hdc, hOldFont);
		}
	}
}

void CScriptParagraph::GenerateLine(CScriptEngine * pEngine, int_x iWidth)
{
	m_lines.clear();
	iHeight = 0;

	if(m_chars.is_empty())
	{
		const RichTextFormatT & rtFormat = m_formats[0];
		ScriptFontT & sFont = pEngine->GetScriptFont(rtFormat.rtFont);
		iHeight = sFont.rtMetric.Height;
		return;
	}

	int_x iX = 0;
	UsLineT * pLine = &m_lines.add();
	for(int_x cnt = 0, len = m_chars.size(); cnt != len; ++cnt)
	{
		const UsCharT & usch = m_chars[cnt];
		if(pLine->iLen && iX + usch.iWidth > iWidth)
		{
			for(int_x index = cnt; index > pLine->iBeg; --index)
			{
				if(m_chars[index].bSoftBreak)
				{
					cnt = index;
					break;
				}
			}
			pLine->iEnd = cnt;
			pLine->iLen = pLine->iEnd - pLine->iBeg;
			pLine = &m_lines.add();
			pLine->iBeg = cnt;
			pLine->iEnd = cnt + 1;
			pLine->iLen = 1;
			iX = usch.iWidth;
		}
		else
		{
			++pLine->iEnd;
			++pLine->iLen;
			iX += usch.iWidth;
		}
	}

	// 计算行高和段落高度。
	int_x iFormat = 0;
	for(int_x line = 0, lineEnd = m_lines.size(); line != lineEnd; ++line)
	{
		UsLineT & usli = m_lines[line];
		for(int_x index = usli.iBeg; index != usli.iEnd; ++index)
		{
			const UsCharT & usch = m_chars[index];
			iFormat = usch.iFormat;
			if(index == usli.iBeg && iFormat < 0)
				iFormat = ~iFormat;

			if(iFormat >= 0)
			{
				const RichTextFormatT & rtFormat = m_formats[iFormat];
				ScriptFontT & sFont = pEngine->GetScriptFont(rtFormat.rtFont);
				if(sFont.rtMetric.Height > usli.iHeight)
				{
					usli.iHeight = sFont.rtMetric.Height;
					usli.iAscent = sFont.rtMetric.Ascent;
				}
			}
		}
		iHeight += usli.iHeight;
	}
}

void CScriptParagraph::Draw(CScriptEngine * pEngine, HDC hdc, int_x iX, int_x iY, int_x iWidth, int_x iHeight)
{
	//if(m_lines.IsEmpty())
	//{
	//	SetBkMode(hdc, TRANSPARENT);
	//	Rectangle(hdc, iX, iY, iWidth, this->iHeight);
	//	return;
	//}
	HGDIOBJ hOldFont = NULL;
	SCRIPT_CACHE * pCache = NULL;
	int_x iTextX = iX;
	int_x iTextY = iY;
	int_x iDrawY = 0;
	HRESULT hr = S_OK;
	//RichTextFormatT rtFormatCurr;
	RichTextMetricT rtMetricCurr;
	int_x iLineH = 0;
	for(int_x line = 0, lineEnd = m_lines.size(); line != lineEnd; ++line)
	{
		const UsLineT & usli = m_lines[line];
		iLineH = usli.iHeight;
		if(iY + iLineH <= 0)
			continue;
		else if(iY >= iHeight)
			break;

		for(int_x index = usli.iBeg; index != usli.iEnd; ++index)
		{
			const UsCharT & usch = m_chars[index];
			if(usch.iFormat >= 0)
			{
				const RichTextFormatT & rtFormat = m_formats[usch.iFormat];
				ScriptFontT & sFont = pEngine->GetScriptFont(rtFormat.rtFont);
				pCache = &sFont.sCache;
				hOldFont = SelectObject(hdc, sFont.hFont);
				SetTextColor(hdc, rtFormat.uiColor);
				rtMetricCurr = sFont.rtMetric;
				//rtFormatCurr = rtFormat;
			}
			else {}

			if(pCache)
			{
				//if(usch.bSoftBreak)
				//	SetTextColor(hdc, 0xFF);
				//else
				//	SetTextColor(hdc, rtFormatCurr.uiColor);
				iDrawY = iTextY + usli.iAscent - rtMetricCurr.Ascent;
				hr = ScriptTextOut(hdc, pCache, (int_32)iTextX, (int_32)iDrawY, 0, NULL,
								   &usch.sa, NULL, 0,
								   m_glyphs + usch.iGlyphBeg, (int_32)usch.iGlyphLen,
								   m_advances + usch.iGlyphBeg,
								   NULL,
								   m_offsets + usch.iGlyphBeg);
			}
			else {}
			iTextX += usch.iWidth;
		}
		iTextX = iX;
		iTextY += usli.iHeight;
	}
}

// --------------------------------------------------------------- CScriptDocument
CScriptDocument::CScriptDocument()
{
}

CScriptDocument::~CScriptDocument()
{

}

void CScriptDocument::Destroy()
{
	m_arrParagraphs.clear();
}

void CScriptDocument::Reset()
{
	m_arrParagraphs.clear();
}

IRtfParagraph * CScriptDocument::AddParagraph(const RichTextFormatT & rtFormat)
{
	return &m_arrParagraphs.add(CScriptParagraph(rtFormat));
}

void CScriptDocument::LoadText(const char_16 * szText, int_x iLength, const RichTextFormatT & rtFormat)
{
	if(iLength < 0)
		iLength = textlen(szText);

	const char_16 * const pText = szText;
	const char_16 * pLast = szText;
	const char_16 * pCurr = szText;
	const char_16 * pEnd = pCurr + iLength;

	textw text;

	int_x iPara = 0;
	IRtfParagraph * pParagraph = AddParagraph(rtFormat);
	bool bNewPara = true;
	while(pCurr != pEnd)
	{
		switch(*pCurr)
		{
		case L'\r':
			pParagraph->AddRichText(pLast, pCurr - pLast, rtFormat);
			pParagraph = AddParagraph(rtFormat);

			if(pCurr + 1 < pEnd && pCurr[1] == L'\n')
				pCurr += 2;
			else
				pCurr += 1;
			pLast = pCurr;
			break;
		case L'\n':
			pParagraph->AddRichText(pLast, pCurr - pLast, rtFormat);
			pParagraph = AddParagraph(rtFormat);

			pCurr += 1;
			pLast = pCurr;
			break;
		default:
			++pCurr;
			break;
		}
	}
	if(pCurr > pLast)
	{
		pParagraph->AddRichText(pLast, pCurr - pLast, rtFormat);
	}

}

void CScriptDocument::Generate(CScriptEngine * pEngine)
{
	for(int_x cnt = 0, len = m_arrParagraphs.size(); cnt != len; ++cnt)
		m_arrParagraphs[cnt].Generate(pEngine);
}

void CScriptDocument::GenerateLine(CScriptEngine * pEngine, int_x iWidth, int_x iHeight)
{
	for(int_x cnt = 0, len = m_arrParagraphs.size(); cnt != len; ++cnt)
	{
		m_arrParagraphs[cnt].GenerateLine(pEngine, iWidth);
	}
}

void CScriptDocument::Draw(CScriptEngine * pEngine, HDC hdc, int_x iX, int_x iY, int_x iWidth, int_x iHeight)
{
	int_x iParaH = 0;
	for(int_x cnt = 0, len = m_arrParagraphs.size(); cnt != len; ++cnt, iY += iParaH)
	{
		CScriptParagraph & para = m_arrParagraphs[cnt];
		iParaH = para.iHeight;

		if(iY + iParaH <= 0)
			continue;
		else if(iY >= iHeight)
			break;
		else
			m_arrParagraphs[cnt].Draw(pEngine, hdc, iX, iY, iWidth, iHeight);
	}
}

int_x CScriptDocument::GetHeight() const
{
	int_x iHeight = 0;
	for(int_x cnt = 0, len = m_arrParagraphs.size(); cnt != len; ++cnt)
		iHeight += m_arrParagraphs[cnt].iHeight;
	return iHeight;
}

// --------------------------------------------------------------- CScriptEngine
CScriptEngine::CScriptEngine()
{
	HDC hdcScreen = GetDC(NULL);
	m_hdc = CreateCompatibleDC(hdcScreen);
	ReleaseDC(NULL, hdcScreen);
}

CScriptEngine::~CScriptEngine()
{
	Destroy();
}

void CScriptEngine::Destroy()
{
	for(int_x cnt = 0, len = m_fonts.size(); cnt != len; ++cnt)
	{
		ScriptFontT & sFont = m_fonts[cnt];
		DeleteObject(sFont.hFont);
		ScriptFreeCache(&sFont.sCache);
	}
	m_fonts.clear();
}

HDC CScriptEngine::GetHDC()
{
	return m_hdc;
}

ScriptFontT & CScriptEngine::GetScriptFont(const RichTextFontT & rtFont)
{
	uint_x uiId = RichTextFontHashFunc(rtFont);
	for(int_x cnt = 0, len = m_fonts.size(); cnt != len; ++cnt)
	{
		ScriptFontT & sFont = m_fonts[cnt];
		if(sFont.uiId == uiId)
			return sFont;
	}

	LOGFONT logFont = {};
	textcpy(logFont.lfFaceName, LF_FACESIZE, rtFont.Face.buffer, -1);
	logFont.lfCharSet = (uint_8)(rtFont.CharSet);

	logFont.lfHeight = -MulDiv((int_32)rtFont.Size, GetDeviceCaps(m_hdc, LOGPIXELSY), 72);
	logFont.lfWeight = (int_32)rtFont.Weight;

	logFont.lfItalic = rtFont.Italic;
	logFont.lfUnderline = rtFont.UnderLine;

	logFont.lfQuality = CLEARTYPE_NATURAL_QUALITY;

	HFONT hFont = CreateFontIndirect(&logFont);
	if(!hFont)
		throw CScriptException();

	LOGFONT logFont2 = {0};
	GetObject(hFont, sizeof(logFont2), &logFont2);

	ScriptFontT & sFont = m_fonts.add();
	sFont.uiId = uiId;
	sFont.hFont = hFont;
	sFont.sCache = NULL;
#ifdef SCRIPT_DEBUG
	sFont.rtFont = rtFont;
#endif

	TEXTMETRIC tm = {};

	HGDIOBJ hOldFont = SelectObject(m_hdc, hFont);
	GetTextMetrics(m_hdc, &tm);
	SelectObject(m_hdc, hOldFont);

	sFont.rtMetric.Height = tm.tmHeight;
	sFont.rtMetric.Ascent = tm.tmAscent;
	sFont.rtMetric.Descent = tm.tmDescent;

	sFont.rtMetric.InternalLeading = tm.tmInternalLeading;
	sFont.rtMetric.ExternalLeading = tm.tmExternalLeading;

	sFont.rtMetric.AveCharWidth = tm.tmAveCharWidth;
	sFont.rtMetric.MaxCharWidth = tm.tmMaxCharWidth;

	sFont.rtMetric.Weight = tm.tmWeight;
	sFont.rtMetric.Overhang = tm.tmOverhang;

	sFont.rtMetric.DigitizedAspectX = tm.tmDigitizedAspectX;
	sFont.rtMetric.DigitizedAspectY = tm.tmDigitizedAspectY;

	sFont.rtMetric.FirstChar = tm.tmFirstChar;
	sFont.rtMetric.LastChar = tm.tmLastChar;
	sFont.rtMetric.DefaultChar = tm.tmDefaultChar;
	sFont.rtMetric.BreakChar = tm.tmBreakChar;

	sFont.rtMetric.Italic = tm.tmItalic != 0;
	sFont.rtMetric.UnderLine = tm.tmUnderlined != 0;
	sFont.rtMetric.StruckOut = tm.tmStruckOut != 0;

	sFont.rtMetric.PitchAndFamily = tm.tmPitchAndFamily;
	sFont.rtMetric.CharSet = tm.tmCharSet;

	return sFont;
}

ScriptFontT & CScriptEngine::GetScriptFont(uint_x uiFontId)
{
	for(int_x cnt = 0, len = m_fonts.size(); cnt != len; ++cnt)
	{
		ScriptFontT & sFont = m_fonts[cnt];
		if(sFont.uiId == uiFontId)
			return sFont;
	}
	throw CScriptException();
}

static uint_x RichTextFontHashFunc(const RichTextFontT & rtFont)
{
	uint_x uiHash = hashtextx(rtFont.Face);
	uiHash = hashx(rtFont.CharSet, uiHash);
	uiHash = hashx(rtFont.Size, uiHash);
	uiHash = hashx(rtFont.Weight, uiHash);
	uiHash = hashx(rtFont.Flags, uiHash);
	return uiHash;
}
