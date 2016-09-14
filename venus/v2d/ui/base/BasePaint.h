#pragma once

VENUS_BEG

V2D_API void PaintArrowL(IPaint * pPaint, const rectix & rect, uint_32 uiColor);
V2D_API void PaintArrowR(IPaint * pPaint, const rectix & rect, uint_32 uiColor);
V2D_API void PaintArrowT(IPaint * pPaint, const rectix & rect, uint_32 uiColor);
V2D_API void PaintArrowB(IPaint * pPaint, const rectix & rect, uint_32 uiColor);

V2D_API void PaintExpand(IPaint * pPaint, int_x iX, int_x iY, int_x iSize, uint_32 uiBackColor, uint_32 uiForeColor, bool bExpand);
V2D_API void PaintExpand(IPaint * pPaint, const rectix & rect, uint_32 uiBackColor, uint_32 uiForeColor, bool bExpand);
V2D_API void PaintCheck(IPaint * pPaint, const rectix & rect, uint_32 uiBackColor, uint_32 uiFrameColor, uint_32 uiForeColor, bool bChecked);
V2D_API void PaintYes(IPaint * pPaint, const rectix & rect, uint_32 uiForeColor);

V2D_API void PaintMenuString(IPaint * pPaint, 
							const char_16 * szText, int_x iLength, const font_t & font,
							int_x iX, int_x iY, 
							uint_32 uiColor);

V2D_API sizeix MeasureMenuString(const char_16 * szText, int_x iLength, const font_t & font);

VENUS_END
