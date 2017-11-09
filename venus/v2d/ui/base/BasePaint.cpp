#include "stdafx.h"
#include "BasePaint.h"

VENUS_BEG

void PaintArrowL(IPaint * pPaint, const rectix & rect, uint_32 uiColor)
{
	int_x iW = minof(rect.w / 4, rect.h / 4);
	int_x iX = rect.x + rect.w / 2;
	int_x iY = rect.y + rect.h / 2;

	pointix points[7];

	points[0].set(iX - iW, iY);
	points[1].set(iX, iY - iW);
	points[2].set(iX + iW, iY - iW);
	points[3].set(iX, iY);
	points[4].set(iX + iW + 1, iY + iW + 1);
	points[5].set(iX, iY + iW + 1);
	points[6].set(iX - iW, iY);
	pPaint->FillPolygon(points, 7, uiColor);
}

void PaintArrowR(IPaint * pPaint, const rectix & rect, uint_32 uiColor)
{
	int_x iW = minof(rect.w / 4, rect.h / 4);
	int_x iX = rect.x + rect.w / 2;
	int_x iY = rect.y + rect.h / 2;

	pointix points[7];

	points[0].set(iX, iY);
	points[1].set(iX - iW, iY - iW);
	points[2].set(iX, iY - iW);
	points[3].set(iX + iW, iY);
	points[4].set(iX - 1, iY + iW + 1);
	points[5].set(iX - iW - 1, iY + iW + 1);
	points[6].set(iX, iY);
	pPaint->FillPolygon(points, 7, uiColor);
}

void PaintArrowT(IPaint * pPaint, const rectix & rect, uint_32 uiColor)
{
	int_x iW = minof(rect.w / 4, rect.h / 4);
	int_x iX = rect.x + rect.w / 2;
	int_x iY = rect.y + rect.h / 2;

	pointix points[7];

	points[0].set(iX, iY - 1);
	points[1].set(iX - iW, iY + iW);
	points[2].set(iX - iW, iY);
	points[3].set(iX, iY - iW - 1);
	points[4].set(iX + iW + 1, iY + 1);
	points[5].set(iX + iW + 1, iY + iW + 1);
	points[6].set(iX, iY - 1);
	pPaint->FillPolygon(points, 7, uiColor);
}

void PaintArrowB(IPaint * pPaint, const rectix & rect, uint_32 uiColor)
{
	int_x iW = minof(rect.w / 4, rect.h / 4);
	int_x iX = rect.x + rect.w / 2;
	int_x iY = rect.y + rect.h / 2;

	pointix points[7];

	points[0].set(iX, iY);
	points[1].set(iX + iW + 1, iY - iW - 1);
	points[2].set(iX + iW + 1, iY - 1);
	points[3].set(iX, iY + iW);
	points[4].set(iX - iW, iY - 1);
	points[5].set(iX - iW, iY - iW - 1);
	points[6].set(iX, iY);
	pPaint->FillPolygon(points, 7, uiColor);
}

void PaintExpand(IPaint * pPaint, int_x iX, int_x iY, int_x iSize, uint_32 uiBackColor, uint_32 uiForeColor, bool bExpand)
{
	pPaint->SetShapeRenderLevel(ShapeRenderLevelAnti);
	pointix points[4];
	if(bExpand)
	{
		int_x iHalf = iSize / 2 + 1;
		int_x iOffY = iSize % 2;

		points[0].set(iX, iY + iHalf + iOffY);
		points[1].set(iX + iHalf + iOffY, iY);
		points[2].set(iX + iHalf + iOffY, iY + iHalf + iOffY);
		points[3] = points[0];
		pPaint->FillPolygon(points, 4, uiBackColor);
		//pPaint->DrawPolygon(points, 4, uiForeColor);
	}
	else
	{
		int_x iHalf = iSize / 2;
		int_x iOffY = iSize % 2;

		points[0].set(iX, iY);
		points[1].set(iX + iHalf, iY + iHalf);
		points[2].set(iX, iY + iSize - iOffY);
		points[3] = points[0];
		pPaint->FillPolygon(points, 4, uiBackColor);
		pPaint->DrawPolygon(points, 4, uiForeColor);
	}
	pPaint->SetShapeRenderLevel(ShapeRenderLevelDefault);
}

void PaintExpand(IPaint * pPaint, const rectix & rect, uint_32 uiBackColor, uint_32 uiForeColor, bool bExpand)
{
	pPaint->SetShapeRenderLevel(ShapeRenderLevelAnti);
	pointix points[4];
	if(bExpand)
	{
		int_x iSize = minof(rect.w, rect.h) / 2;
		int_x iHalf = iSize / 2 + 1;
		int_x iX = rect.x + iSize - 2;
		int_x iY = rect.y + (rect.h - iSize) / 2 + 1;
		int_x iOffY = iSize % 2;

		points[0].set(iX, iY + iHalf + iOffY);
		points[1].set(iX + iHalf + iOffY, iY);
		points[2].set(iX + iHalf + iOffY, iY + iHalf + iOffY);
		points[3] = points[0];
		pPaint->FillPolygon(points, 4, uiBackColor);
		//pPaint->DrawPolygon(points, 4, uiForeColor);
	}
	else
	{
		int_x iSize = minof(rect.w, rect.h) / 2;
		int_x iHalf = iSize / 2;
		int_x iX = rect.x + iSize;
		int_x iY = rect.y + (rect.h - iSize) / 2;
		int_x iOffY = iSize % 2;

		points[0].set(iX, iY);
		points[1].set(iX + iHalf, iY + iHalf);
		points[2].set(iX, iY + iSize - iOffY);
		points[3] = points[0];
		pPaint->FillPolygon(points, 4, uiBackColor);
		pPaint->DrawPolygon(points, 4, uiForeColor);
	}
	pPaint->SetShapeRenderLevel(ShapeRenderLevelDefault);
}

void PaintCheck(IPaint * pPaint, const rectix & rect, uint_32 uiBackColor, uint_32 uiFrameColor,
				uint_32 uiForeColor, bool bChecked)
{
	pPaint->FillRect(rect, uiBackColor);
	pPaint->DrawRect(rect.x, rect.y, rect.w, rect.h, uiFrameColor);

	rectix rcTick = rect.expand(-2, -2);
	if(bChecked)
	{
		pPaint->SetShapeRenderLevel(ShapeRenderLevelAnti);
		pointix points[5];
		points[0].set(rcTick.x, rcTick.y + rcTick.h * 4 / 10);
		points[1].set(rcTick.x + rcTick.w * 4 / 10, rcTick.y + rcTick.h * 6 / 10);
		points[2].set(rcTick.x + rcTick.w, rcTick.y);
		points[3].set(rcTick.x + rcTick.w * 4 / 10, rcTick.y + rcTick.h);
		points[4] = points[0];

		pPaint->FillPolygon(points, 5, uiForeColor);
		pPaint->SetShapeRenderLevel(ShapeRenderLevelDefault);
	}
}

void PaintYes(IPaint * pPaint, const rectix & rect, uint_32 uiForeColor)
{
	pPaint->SetShapeRenderLevel(ShapeRenderLevelAnti);
	pointix points[5];
	points[0].set(rect.x + rect.w * 0 / 10, rect.y + rect.h * 4 / 10);
	points[1].set(rect.x + rect.w * 5 / 10, rect.y + rect.h * 6 / 10);
	points[2].set(rect.x + rect.w * 10 / 10, rect.y + rect.h * 0 / 10);
	points[3].set(rect.x + rect.w * 5 / 10, rect.y + rect.h * 10 / 10);
	points[4] = points[0];

	pPaint->FillPolygon(points, 5, uiForeColor);
	pPaint->SetShapeRenderLevel(ShapeRenderLevelDefault);
}

void PaintMenuString(IPaint * pPaint,
					 const char_16 * szText, int_x iLength, const font_t & font,
					 int_x iX, int_x iY,
					 uint_32 uiColor)
{
	if(iLength < 0)
		iLength = textlen(szText);
	if(!iLength)
		return;

	IDevice2D * pService = Device2D();
	int_x iAnd = textch(szText, iLength, L'&');
	if(iAnd < 0 || iLength - iAnd - 2 < 0)
		pPaint->DrawString(szText, iLength, font, pointix(iX, iY), uiColor);
	else
	{
		pPaint->DrawString(szText, iAnd, font, pointix(iX, iY), uiColor);
		iX += pService->GetTextSize(szText, iAnd, font).w;

		font_t font2 = font;
		font2.underline = true;
		pPaint->DrawString(szText + iAnd + 1, 1, font2, pointix(iX, iY), uiColor);

		iX += pService->GetTextSize(szText + iAnd + 1, 1, font).w;

		pPaint->DrawString(szText + iAnd + 2, iLength - iAnd - 2, font, pointix(iX, iY), uiColor);
	}
}

sizeix MeasureMenuString(const char_16 * szText, int_x iLength, const font_t & font)
{
	IDevice2D * pService = Device2D();
	int_x iAnd = textch(szText, iLength, L'&');
	if(iAnd < 0 || iLength - iAnd - 2 < 0)
		return pService->GetTextSize(szText, iLength, font);
	else
	{
		sizeix size = pService->GetTextSize(szText, iAnd, font);
		size.w += pService->GetTextSize(szText + iAnd + 1, 1, font).w;
		size.w += pService->GetTextSize(szText + iAnd + 2, iLength - iAnd - 2, font).w;
		return size;
	}
}

VENUS_END
