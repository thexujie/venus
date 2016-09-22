#pragma once

#include "GdipInc.h"

#ifdef WIN32_DEVICE_GDIP

VENUS_BEG

const char_16 GDIP_MODULE_NAME[] = L"gdiplus.dll";

struct GdipPenT
{
	uint_32 uiColor;
	float_32 fWidth;
	Gdiplus::GpPen * pPen;
};

struct GdipSolidBrushT
{
	uint_32 uiColor;
	Gdiplus::GpBrush * pBrush;
};

struct GdipFontFamilyT
{
	int_x iHash;
	Gdiplus::GpFontFamily * pFamily;
};

struct gpfont_t
{
	Gdiplus::GpFont * font;
	int_x external_leading;
};

struct GdipFontT
{
	int_x iHash;
	gpfont_t font;
};

struct GdipFontFormatT
{
	int_x iHash;

	Gdiplus::GpStringFormat * pFormat;
};

class GdipImageCacheT : public ObjectT<IImageCache>
{
public:
	GdipImageCacheT(Gdiplus::GpImage * _pImage) : pImage(_pImage) {}
	~GdipImageCacheT() { ReleaseCache(); }
	void ReleaseCache();

	Gdiplus::GpImage * pImage;
};

class WIN32_API C2DDeviceGdip : public ObjectT<I2DDevice>
{
public:
	C2DDeviceGdip();
	~C2DDeviceGdip();

	Device2DTypeE GetType() const;

	fontmetrics_t GetFontMetric(const font_t & font);
	ITextLayout * CreateTextLayout();

	textsize_t GetTextSize(const char_16 * szText, int_x iLength, const font_t & font);

	image_convert_rule_fun_t GetImageConvertRuleFun() const;
	IPaint * CreatePaint(I2DRTarget * pRenderTarget);

public:
	Gdiplus::GpPen * GetPen(uint_32 color, float_32 fWidth = 1.0f);
	Gdiplus::GpBrush * GetSolidBrush(uint_32 color);
	Gdiplus::GpFontFamily * GetFontFamily(const char_16 * szFamily, int_x iLength = -1);
	gpfont_t GetFont(const textformat_t & format);
	Gdiplus::GpStringFormat * GetFontFormat(const textformat_t & format);
	Gdiplus::GpImage * GetImage(IImage * pImage);

protected:
	CModule m_module;

private:
	ULONG_PTR m_ulToken;

	HDC m_hdc;
	Gdiplus::GpStatus m_status;
	Gdiplus::GpGraphics * m_pGraphics;
	CUspFactory * m_pUspFactory;

	vector<GdipPenT> m_sPens;
	vector<GdipSolidBrushT> m_sBrushs;
	vector<GdipFontFamilyT> m_familys;
	vector<GdipFontT> m_fonts;
	vector<GdipFontFormatT> m_fontFormats;
	vector<GdipImageCacheT *> m_images;

public:
	static Gdiplus::PixelFormat PixelFormatFromColorMode(cmode_e eMode);
};

VENUS_END

#endif //WIN32_DEVICE_GDIP
