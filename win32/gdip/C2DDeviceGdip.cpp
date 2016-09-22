#include "stdafx.h"
#include "C2DDeviceGdip.h"
#include "CPaintGdip.h"

#ifdef WIN32_DEVICE_GDIP

VENUS_BEG

using namespace Gdiplus;
using namespace DllExports;

C2DDeviceGdip::C2DDeviceGdip():
m_hResult(S_OK), m_pUspFactory(nullptr)
{
	GdiplusStartupInput input;
	GdiplusStartup(&m_ulToken, &input, nullptr);

	HDC hdcScreen = ::GetDC(NULL);
	m_hdc = ::CreateCompatibleDC(hdcScreen);
	ReleaseDC(NULL, hdcScreen);
	GdipCreateFromHDC(m_hdc, &m_pGraphics);
	InitializeGdipGraphics(m_pGraphics);
}

C2DDeviceGdip::~C2DDeviceGdip()
{
	SafeDelete(m_pUspFactory);
	for(int_x cnt = 0, len = m_sPens.size(); cnt < len; ++cnt)
		GdipDeletePen(m_sPens[cnt].pPen);
	for(int_x cnt = 0, len = m_sBrushs.size(); cnt < len; ++cnt)
		GdipDeleteBrush(m_sBrushs[cnt].pBrush);
	for(int_x cnt = 0, len = m_familys.size(); cnt < len; ++cnt)
		GdipDeleteFontFamily(m_familys[cnt].pFamily);
	for(int_x cnt = 0, len = m_fonts.size(); cnt < len; ++cnt)
		GdipDeleteFont(m_fonts[cnt].font.font);
	for(int_x cnt = 0, len = m_fontFormats.size(); cnt < len; ++cnt)
		GdipDeleteStringFormat(m_fontFormats[cnt].pFormat);

	for(int_x cnt = 0, len = m_images.size(); cnt < len; ++cnt)
	{
		m_images[cnt]->ReleaseCache();
		SafeRelease(m_images[cnt]);
	}

	m_fonts.clear();
	m_sBrushs.clear();
	m_familys.clear();
	m_fonts.clear();
	m_images.clear();
	GdipDeleteGraphics(m_pGraphics);
	m_pGraphics = nullptr;
	GdiplusShutdown(m_ulToken);
	::DeleteDC(m_hdc);
}

Device2DTypeE C2DDeviceGdip::GetType() const
{
	return Device2DTypeGdip;
}

GpPen * C2DDeviceGdip::GetPen(uint_32 color, float_32 fWidth)
{
	for(int_x cnt = 0, len = m_sPens.size(); cnt < len; ++cnt)
	{
		const GdipPenT & cache = m_sPens[cnt];
		if(cache.uiColor == color && f32_equal(cache.fWidth, fWidth))
			return cache.pPen;
	}

	GpPen * pPen = nullptr;
	GdipCreatePen1(color, fWidth, UnitPixel, &pPen);
	GdipPenT cache = {color, fWidth, pPen};
	m_sPens.add(cache);
	return pPen;
}

GpBrush * C2DDeviceGdip::GetSolidBrush(uint_32 color)
{
	for(int_x cnt = 0, len = m_sBrushs.size(); cnt < len; ++cnt)
	{
		const GdipSolidBrushT & gdipBrush = m_sBrushs[cnt];
		if(gdipBrush.uiColor == color)
			return gdipBrush.pBrush;
	}

	GpSolidFill* pBrush = nullptr;
	GdipCreateSolidFill(color, &pBrush);
	GdipSolidBrushT newBrush = {color, pBrush};
	m_sBrushs.add(newBrush);
	return pBrush;
}

GpFontFamily * C2DDeviceGdip::GetFontFamily(const char_16 * szFamily, int_x iLength)
{
	if(iLength < 0)
		iLength = textlen(szFamily);

	chbufferw<MAX_FONTNAME> chbName;
	if(!iLength || !szFamily[0])
		chbName = Win32::GetDefaultFontName();
	else
		chbName.set(szFamily, iLength);

	int_x iHash = hashtext16(chbName.buffer, iLength);
	for(int_x cnt = 0, len = m_familys.size(); cnt < len; ++cnt)
	{
		GdipFontFamilyT & cache = m_familys[cnt];
		if(cache.iHash == iHash)
			return cache.pFamily;
	}
	GpFontFamily * pFamily = nullptr;
	GdipCreateFontFamilyFromName(chbName, nullptr, &pFamily);

	GdipFontFamilyT cache = {iHash, pFamily};
	m_familys.add(cache);
	return pFamily;
}

gpfont_t C2DDeviceGdip::GetFont(const textformat_t & format)
{
	int_x iHash = format.font.Hash();
	for(int_x cnt = 0, len = m_fonts.size(); cnt < len; ++cnt)
	{
		GdipFontT & cache = m_fonts[cnt];
		if(cache.iHash == iHash)
			return cache.font;
	}

	int_32 iStyle = 0;
	if(format.font.weight > FONT_WEIGHT_NORMAL)
		iStyle |= FontStyleBold;
	if(format.font.italic)
		iStyle |= FontStyleItalic;
	if(format.font.underline)
		iStyle |= FontStyleUnderline;
	if(format.font.strikeout)
		iStyle |= FontStyleStrikeout;

	float fDpiY = 72.0f;
	GdipGetDpiY(m_pGraphics, &fDpiY);
	float_32 fFontSize = (float_32)format.font.size * 72.0f / fDpiY;
	GpFontFamily * pFamily = GetFontFamily(format.font.name);
	GpFont * pFont = nullptr;
	GdipCreateFont(pFamily, fFontSize, iStyle, UnitPixel, &pFont);
	
	uint_16 uiLineSpace = 0, uiEmHegiht = 0, uiAscent = 0, uiDecent = 0;
	GdipGetLineSpacing(pFamily, iStyle, &uiLineSpace);
	GdipGetEmHeight(pFamily, iStyle, &uiEmHegiht);
	GdipGetCellAscent(pFamily, iStyle, &uiAscent);
	GdipGetCellDescent(pFamily, iStyle, &uiDecent);
	int_x iExternal = format.font.size * (uiLineSpace - (uiAscent + uiDecent)) / uiEmHegiht;

	GdipFontT cache = {iHash, {pFont, iExternal}};
	m_fonts.add(cache);
	return cache.font;
}

Gdiplus::GpStringFormat * C2DDeviceGdip::GetFontFormat(const textformat_t & format)
{
	int_x iHash = hashx(format.align);
	iHash = hashx(format.wrap, iHash);
	iHash = hashx(format.trimming, iHash);
	iHash = hashx(format.direction, iHash);

	for(int_x cnt = 0, len = m_fontFormats.size(); cnt < len; ++cnt)
	{
		GdipFontFormatT & cache = m_fontFormats[cnt];
		if(cache.iHash == iHash)
			return cache.pFormat;
	}

	GpStringFormat * pFormat = nullptr;
	GpStringFormat * pTypoStringFormat = nullptr;
	GdipStringFormatGetGenericTypographic(&pTypoStringFormat);
	GdipCloneStringFormat(pTypoStringFormat, &pFormat);
	GdipDeleteStringFormat(pTypoStringFormat);

	switch(format.align & (AlignLeft | AlignCenterX | AlignRight))
	{
	case AlignCenterX:
		GdipSetStringFormatAlign(pFormat, StringAlignmentCenter);
		break;
	case AlignRight:
		GdipSetStringFormatAlign(pFormat, StringAlignmentFar);
		break;
	default:
		GdipSetStringFormatAlign(pFormat, StringAlignmentNear);
		break;
	}

	switch(format.align & (AlignTop | AlignCenterY | AlignBottom))
	{
	case AlignCenterY:
		GdipSetStringFormatLineAlign(pFormat, StringAlignmentCenter);
		break;
	case AlignBottom:
		GdipSetStringFormatLineAlign(pFormat, StringAlignmentFar);
		break;
	default:
		GdipSetStringFormatLineAlign(pFormat, StringAlignmentNear);
		break;
	}

	int_32 iFlags = /*StringFormatFlagsNoClip | */StringFormatFlagsNoFitBlackBox |
		/*StringFormatFlagsMeasureTrailingSpaces | StringFormatFlagsNoWrap |*/ StringFormatFlagsLineLimit;
	switch(format.wrap)
	{
	case TextWrapNone:
		iFlags |= StringFormatFlagsNoWrap;
		break;
	default:
		break;
	}

	switch(format.trimming)
	{
	case TextTrimmingChar:
		GdipSetStringFormatTrimming(pFormat, StringTrimmingCharacter);
		break;
	case TextTrimmingCharEllipsis:
		GdipSetStringFormatTrimming(pFormat, StringTrimmingEllipsisCharacter);
		break;
	case TextTrimmingWord:
		GdipSetStringFormatTrimming(pFormat, StringTrimmingWord);
		break;
	case TextTrimmingWordEllipsis:
		GdipSetStringFormatTrimming(pFormat, StringTrimmingEllipsisWord);
		break;
	case TextTrimmingPathEllipsis:
		GdipSetStringFormatTrimming(pFormat, StringTrimmingEllipsisPath);
		break;
	default:
		break;
	}

	//switch(format.direction)
	//{
	//default:
	//	break;
	//}

	GdipSetStringFormatFlags(pFormat, iFlags);

	GdipFontFormatT cache = {iHash, pFormat};
	m_fontFormats.add(cache);
	return pFormat;
}

Gdiplus::GpImage * C2DDeviceGdip::GetImage(IImage * pImage)
{
	if(!pImage)
		return nullptr;

	GdipImageCacheT * pGdipCache = (GdipImageCacheT *)pImage->GetCache();
	if(!pGdipCache || !pGdipCache->pImage)
	{
		ImageBufferT buffer = pImage->GetBuffer();
		if(!buffer.buffer)
			return nullptr;

		PixelFormat eFormat = PixelFormatFromColorMode(buffer.mode);
		if(eFormat == -1)
			return nullptr;

		GpBitmap * pBitmap = nullptr;
		Status status = GdipCreateBitmapFromScan0((int_32)buffer.width, (int_32)buffer.height,
			(int_32)buffer.pitch, eFormat, buffer.buffer, &pBitmap);
		if(pBitmap)
		{
			if(pGdipCache)
				pGdipCache->pImage = pBitmap;
			else
			{
				pGdipCache = new GdipImageCacheT(pBitmap);
				pImage->SetCache(pGdipCache);
				m_images.add(pGdipCache);
			}
		}
		return pBitmap;
	}
	else
	{
		return pGdipCache->pImage;
	}
}

fontmetrics_t C2DDeviceGdip::GetFontMetric(const font_t & font)
{
	gpfont_t gpfont = GetFont(font);

	fontmetrics_t metrics;

	int iStyle = 0;
	GdipGetFontStyle(gpfont.font, &iStyle);
	metrics.size = font.size;
	metrics.weight = (iStyle & FontStyleBold) ? FONT_WEIGHT_BOLD : FONT_WEIGHT_NORMAL;
	metrics.italic = (iStyle & FontStyleItalic) ? 1 : 0;
	metrics.underLine = (iStyle & FontStyleUnderline) ? 1 : 0;;
	metrics.struckout = (iStyle & FontStyleStrikeout) ? 1 : 0;;

	//metrics.LineSpace = Round(fLineSpace);
	//metrics.BaseLine = Round(fBaseLine);
	return metrics;
}

ITextLayout * C2DDeviceGdip::CreateTextLayout()
{
	if(!m_pUspFactory)
	{
		m_pUspFactory = new CUspFactory(m_hdc);
		m_pUspFactory->LoadUsp10();
	}

	return m_pUspFactory->CreateLayout();
}

textsize_t C2DDeviceGdip::GetTextSize(const char_16 * szText, int_x iLength, const font_t & font)
{
	if(iLength < 0)
		iLength = textlen(szText);

	if(!iLength)
		return textsize_t(0, font.size);

	textformat_t format(font);
	GpRectF rect;
	gpfont_t gpfont = GetFont(format);
	GpStringFormat * pFormat = GetFontFormat(format);

	m_status = GdipMeasureString(m_pGraphics, szText, (int_32)iLength, gpfont.font,
								 &GpRectF(), pFormat, &rect,
								 nullptr, nullptr);
	textsize_t tSize(ceil(rect.Width), ceil(rect.Height));
	return tSize;
}


bool d2d_rule_full(image_convert_rule_t * rule);
image_convert_rule_fun_t C2DDeviceGdip::GetImageConvertRuleFun() const
{
	return d2d_rule_full;
}

IPaint * C2DDeviceGdip::CreatePaint(I2DRTarget * pRenderTarget)
{
	CPaintGdip * pPaint = new CPaintGdip(pRenderTarget, this);
	return pPaint;
}

PixelFormat C2DDeviceGdip::PixelFormatFromColorMode(cmode_e eMode)
{
	switch(eMode)
	{
	case cmode_r8g8b8:	return PixelFormat24bppRGB;
	case cmode_a8r8g8b8:	return PixelFormat32bppARGB;
	case cmode_x8r8g8b8:	return PixelFormat32bppRGB;
	case cmode_x1r5g5b5:	return PixelFormat16bppRGB555;
	case cmode_r5g6b5:	return PixelFormat16bppRGB565;
	default:				return -1;
	}
}

VENUS_END

#endif //WIN32_DEVICE_GDIP
