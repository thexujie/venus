#pragma once

#include "GdiInc.h"

#ifdef WIN32_DEVICE_GDI

VENUS_BEG

struct WIN32_API GdiPenT
{
	uint_32 uiColor;
	float_32 fWidth;
	HPEN hPen;
};

struct WIN32_API GdiSolidBrushT
{
	uint_32 uiColor;
	HBRUSH hBrush;
};

struct WIN32_API GdiFontT
{
	int_x iHash;
	HFONT hFont;
};

class WIN32_API CDevice2DGdi : public ObjectT<IDevice2D>
{
public:
	CDevice2DGdi();
	~CDevice2DGdi();

	Device2DTypeE GetType() const;

	ITextLayout * CreateTextLayout();

	textsize_t GetTextSize(const char_16 * szText, int_x iLength, const font_t & font);
	image_convert_rule_fun_t GetImageConvertRuleFun() const;
	IPaint * CreatePaint(I2DRTarget * pRenderTarget);

	HPEN GetPen(uint_32 color, float_32 fWidth = 1.0f);
	HBRUSH GetSolidBrush(uint_32 color);
	HFONT GetFont(const font_t & font);
private:
	HDC m_hdc;
	CUspFactory * m_pUspFactory;

	vector<GdiPenT> m_sPens;
	vector<GdiSolidBrushT> m_sBrushs;
	vector<GdiFontT> m_fonts;
};

VENUS_END

#endif //WIN32_DEVICE_GDI
