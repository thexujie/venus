#pragma once

#include "D2DInc.h"

VENUS_BEG

struct WIN32_API D2DFontT
{
	int_x iHash;
#ifdef _DEBUG
	textformat_t format;
#endif
	IDWriteTextFormat * pFormat;
};

class WIN32_API CDevice2DD2D : public ObjectT<IDevice2D>
{
public:
	CDevice2DD2D();
	~CDevice2DD2D();

	Device2DTypeE GetType() const;

	fontmetrics_t GetFontMetric(const font_t & font);
	ITextLayout * CreateTextLayout();

	textsize_t GetTextSize(const char_16 * szText, int_x iLength, const font_t & font);

	image_convert_rule_fun_t GetImageConvertRuleFun() const;
	IPaint * CreatePaint(I2DRTarget * pRenderTarget);

	IDWriteTextFormat * GetTextFormat(const textformat_t & format);
	ID2D1Factory * GetFactory();
	IDWriteFactory * GetDWrite();
private:
	HMODULE m_hModuleD2D;
	HMODULE m_hModuleDWrite;

	ID2D1Factory * m_pFactory;
	IDWriteFactory * m_pDWrite;

	vector<D2DFontT> m_fonts;
};

VENUS_END
