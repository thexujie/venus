#include "stdafx.h"
#include "GdiInc.h"

VENUS_BEG

void GdiGetLogfont(HDC hdc, const font_t & font, LOGFONT & logFont)
{
	if(font.name[0])
		textcpy(logFont.lfFaceName, LF_FACESIZE, font.name.buffer, -1);
	else
	{
		chbufferw<MAX_FONTNAME> defFontName = Win32::GetDefaultFontName();
		textcpy(logFont.lfFaceName, LF_FACESIZE, defFontName.buffer, -1);
	}

	int iDpiY = GetDeviceCaps(hdc, LOGPIXELSY);
	logFont.lfWidth = 0;
	logFont.lfHeight = (int_32)(-font.size * 72 / iDpiY);
	logFont.lfWeight = (int_32)font.weight;

	logFont.lfItalic = (uint_8)font.italic;
	logFont.lfUnderline = (uint_8)font.underline;
	logFont.lfStrikeOut = (uint_8)font.strikeout;
	//logFont.lfCharSet = (uint_8)(font.charset);
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	switch(font.renderlevel)
	{
	case FontRenderLevelSystem:
		logFont.lfQuality = DEFAULT_QUALITY;
		break;
	case FontRenderLevelGray:
		logFont.lfQuality = DRAFT_QUALITY;
		break;
	case FontRenderLevelAntiGray:
		logFont.lfQuality = ANTIALIASED_QUALITY;
		break;
	case FontRenderLevelClearTypeGrid:
		logFont.lfQuality = CLEARTYPE_QUALITY;
		break;
	default:
		logFont.lfQuality = DEFAULT_QUALITY;
		break;
	}
	logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
}

VENUS_END
