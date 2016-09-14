#include "stdafx.h"
#include "C2DDeviceGdi.h"
#include "CPaintGdi.h"

#ifdef WIN32_DEVICE_GDI

VENUS_BEG

C2DDeviceGdi::C2DDeviceGdi()
{
	HDC hdcScreen = ::GetDC(NULL);
	m_hdc = ::CreateCompatibleDC(hdcScreen);
	ReleaseDC(NULL, hdcScreen);

	m_pUspFactory = new CUspFactory(m_hdc);
	m_pUspFactory->LoadUsp10();
}

C2DDeviceGdi::~C2DDeviceGdi()
{
	SafeDelete(m_pUspFactory);
	for(int_x cnt = 0, len = m_sPens.size(); cnt < len; ++cnt)
		DeleteObject(m_sPens[cnt].hPen);
	for(int_x cnt = 0, len = m_sBrushs.size(); cnt < len; ++cnt)
		DeleteObject(m_sBrushs[cnt].hBrush);
	for(int_x cnt = 0, len = m_fonts.size(); cnt < len; ++cnt)
		DeleteObject(m_fonts[cnt].hFont);

	m_fonts.clear();
	m_sBrushs.clear();
	m_fonts.clear();
	::DeleteDC(m_hdc);
}

Device2DTypeE C2DDeviceGdi::GetType() const
{
	return Device2DTypeGdi;
}

HPEN C2DDeviceGdi::GetPen(uint_32 color, float_32 fWidth)
{
	if(!(color & 0xFF000000))
		color = Colors::Transparent;

	for(int_x cnt = 0, len = m_sPens.size(); cnt < len; ++cnt)
	{
		const GdiPenT & cache = m_sPens[cnt];
		if(cache.uiColor == color && f32_equal(cache.fWidth, fWidth))
			return cache.hPen;
	}

	HPEN hPen = NULL;
	if(color == Colors::Transparent)
		hPen = (HPEN)GetStockObject(NULL_PEN);
	else
		hPen = ::CreatePen(PS_SOLID, (int_32)fWidth, color & 0x00FFFFFF);

	GdiPenT cache = {color, fWidth, hPen};
	m_sPens.add(cache);
	return hPen;
}

HBRUSH C2DDeviceGdi::GetSolidBrush(uint_32 color)
{
	if(!(color & 0xFF000000))
		color = Colors::Transparent;

	for(int_x cnt = 0, len = m_sBrushs.size(); cnt < len; ++cnt)
	{
		const GdiSolidBrushT & cache = m_sBrushs[cnt];
		if(cache.uiColor == color)
			return cache.hBrush;
	}

	HBRUSH hBrush = NULL;
	if(color == Colors::Transparent)
		hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	else
		hBrush = CreateSolidBrush(color & 0x00FFFFFF);

	GdiSolidBrushT cache = {color, hBrush};
	m_sBrushs.add(cache);
	return hBrush;
}

HFONT C2DDeviceGdi::GetFont(const font_t & font)
{
	int_x iHash = font.Hash();
	for(int_x cnt = 0, len = m_fonts.size(); cnt < len; ++cnt)
	{
		GdiFontT & cache = m_fonts[cnt];
		if(cache.iHash == iHash)
			return cache.hFont;
	}

	LOGFONTW logFont = {};
	GdiGetLogfont(m_hdc, font, logFont);

	HFONT hFont = CreateFontIndirectW(&logFont);
	GdiFontT cache = {iHash, hFont};
	m_fonts.add(cache);
	return hFont;
}

fontmetrics_t C2DDeviceGdi::GetFontMetric(const font_t & font)
{
	HFONT hFont = GetFont(font);
	
	
	fontmetrics_t metrics;

	int iStyle = 0;
	metrics.size = font.size;
	metrics.weight = 400;
	metrics.italic = 0;
	metrics.underLine = 0;
	metrics.struckout = 0;

	return metrics;
}

ITextLayout * C2DDeviceGdi::CreateTextLayout()
{
	return m_pUspFactory->CreateLayout();
}

textsize_t C2DDeviceGdi::GetTextSize(const char_16 * szText, int_x iLength, const font_t & font)
{
	if(iLength < 0)
		iLength = textlen(szText);

	if(!iLength)
		return textsize_t(0, font.size);

	HFONT hFont = GetFont(font);
	HGDIOBJ hOld = ::SelectObject(m_hdc, hFont);
	
	SIZE size = {};
	GetTextExtentExPointW(m_hdc, szText, (int_32)iLength, I32_MAX, NULL, NULL, &size);
	textsize_t tSize(size.cx, size.cy);
	::SelectObject(m_hdc, hOld);

	return tSize;
}

image_convert_rule_fun_t C2DDeviceGdi::GetImageConvertRuleFun() const
{
	return nullptr;
}

IPaint * C2DDeviceGdi::CreatePaint(I2DRTarget * pRenderTarget)
{
	CPaintGdi * pPaint = new CPaintGdi(pRenderTarget, this);
	return pPaint;
}

VENUS_END

#endif //WIN32_DEVICE_GDI
