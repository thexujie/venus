#pragma once

#include <d2d1.h>
#include <dwrite.h>

VENUS_BEG

class WIN32_API D2DColorT : public D2D_COLOR_F
{
public:
	D2DColorT(uint_32 uiColor)
	{
		a = (float_32)((uiColor >> 24) & 0xFF) / 0xFF;
		r = (float_32)((uiColor >> 16) & 0xFF) / 0xFF;
		g = (float_32)((uiColor >> 8) & 0xFF) / 0xFF;
		b = (float_32)((uiColor >> 0) & 0xFF) / 0xFF;
	}
};

class WIN32_API ITextLayoutD2D : public ITextLayout
{
public:
	virtual IDWriteTextLayout * GetDxTextLayout() const = 0;
};


V2D_API DWRITE_WORD_WRAPPING GetDwTextWrapping(TextWrapE eTextWrap);

VENUS_END
