#include "stdafx.h"
#include "CPaintBase.h"

VENUS_BEG

CPaintBase::CPaintBase()
{

}

void CPaintBase::SetPixel(int_x iX, int_x iY, uint_32 uiColor)
{
	SetPixel((float_32)iX, (float_32)iY, uiColor);
}

uint_32 CPaintBase::GetPixel(int_x iX, int_x iY)
{
	return GetPixel((float_32)iX, (float_32)iY);
}
// ---------------------------------------------------------- DrawLine
void CPaintBase::DrawLine(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, uint_32 uiColor, float_32 fLineWidth)
{
	DrawLine(fX1, fY1, fX2, fY2, &CPen(uiColor, fLineWidth));
}

void CPaintBase::DrawLine(int_x iX1, int_x iY1, int_x iX2, int_x iY2, IPen * pPen)
{
	DrawLine((float_32)iX1, (float_32)iY1, (float_32)iX2, (float_32)iY2, pPen);
}

void CPaintBase::DrawLine(int_x iX1, int_x iY1, int_x iX2, int_x iY2, uint_32 uiColor, float_32 fLineWidth)
{
	DrawLine(iX1, iY1, iX2, iY2, &CPen(uiColor, fLineWidth));
}

void CPaintBase::DrawLine(const pointf32 & point1, const pointf32 & point2, uint_32 uiColor, float_32 fLineWidth)
{
	DrawLine(point1.x, point1.y, point2.x, point2.y, &CPen(uiColor, fLineWidth));
}

void CPaintBase::DrawLine(const pointf32 & point1, const pointf32 & point2, IPen * pPen)
{
	DrawLine(point1.x, point1.y, point2.x, point2.y, pPen);
}

void CPaintBase::DrawLine(const pointix & point1, const pointix & point2, uint_32 uiColor, float_32 fLineWidth)
{
	DrawLine((float_32)point1.x, (float_32)point1.y, (float_32)point2.x, (float_32)point2.y, &CPen(uiColor, fLineWidth));
}

void CPaintBase::DrawLine(const pointix & point1, const pointix & point2, IPen * pPen)
{
	DrawLine((float_32)point1.x, (float_32)point1.y, (float_32)point2.x, (float_32)point2.y, pPen);
}

void CPaintBase::DrawPolygon(const pointf32 * pPoints, int_x iCount, uint_32 uiColor, float_32 fLineWidth)
{
	while(iCount > 1)
	{
		DrawLine(*pPoints, *(pPoints + 1), &CPen(uiColor, fLineWidth));
		++pPoints;
		--iCount;
	}
}

void CPaintBase::DrawPolygon(const pointix * pPoints, int_x iCount, uint_32 uiColor, float_32 fLineWidth)
{
	while(iCount > 1)
	{
		DrawLine(*pPoints, *(pPoints + 1), &CPen(uiColor, fLineWidth));
		++pPoints;
		--iCount;
	}
}

// ---------------------------------------------------------- DrawRect
void CPaintBase::DrawRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, uint_32 uiColor, float_32 fLineWidth)
{
	DrawRect(fX, fY, fWidth, fHeight, &CPen(uiColor, fLineWidth));
}

void CPaintBase::DrawRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight, IPen * pPen)
{
	DrawRect((float_32)iX, (float_32)iY, (float_32)iWidth, (float_32)iHeight, pPen);
}

void CPaintBase::DrawRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight, uint_32 uiColor, float_32 fLineWidth)
{
	DrawRect((float_32)iX, (float_32)iY, (float_32)iWidth, (float_32)iHeight, &CPen(uiColor, fLineWidth));
}

void CPaintBase::DrawRect(const rectf32 & rect, IPen * pPen)
{
	DrawRect(rect.x, rect.y, rect.w, rect.h, pPen);
}

void CPaintBase::DrawRect(const rectf32 & rect, uint_32 uiColor, float_32 fLineWidth)
{
	DrawRect(rect.x, rect.y, rect.w, rect.h, &CPen(uiColor, fLineWidth));
}

void CPaintBase::DrawRect(const rectix & rect, IPen * pPen)
{
	DrawRect((float_32)rect.x, (float_32)rect.y, (float_32)rect.w, (float_32)rect.h, pPen);
}

void CPaintBase::DrawRect(const rectix & rect, uint_32 uiColor, float_32 fLineWidth)
{
	DrawRect((float_32)rect.x, (float_32)rect.y, (float_32)rect.w, (float_32)rect.h, &CPen(uiColor, fLineWidth));
}

void CPaintBase::FillRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, uint_32 uiColor)
{
	FillRect(fX, fY, fWidth, fHeight, &CSolidBrush(uiColor));
}

void CPaintBase::FillRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight, IBrush * pBrush)
{
	FillRect((float_32)iX, (float_32)iY, (float_32)iWidth, (float_32)iHeight, pBrush);
}

void CPaintBase::FillRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight, uint_32 uiColor)
{
	FillRect((float_32)iX, (float_32)iY, (float_32)iWidth, (float_32)iHeight, &CSolidBrush(uiColor));
}

void CPaintBase::FillRect(const rectix & rect, IBrush * pBrush)
{
	FillRect((float_32)rect.x, (float_32)rect.y, (float_32)rect.w, (float_32)rect.h, pBrush);
}
void CPaintBase::FillRect(const rectix & rect, uint_32 uiColor)
{
	FillRect((float_32)rect.x, (float_32)rect.y, (float_32)rect.w, (float_32)rect.h, &CSolidBrush(uiColor));
}

void CPaintBase::DrawCircle(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, uint_32 uiColor, float_32 fLineWidth)
{
	DrawCircle(fX, fY, fWidth, fHeight, &CPen(uiColor, fLineWidth));
}

void CPaintBase::DrawCircle(int_x iX, int_x iY, int_x iWidth, int_x iHeight, IPen * pPen)
{
	DrawCircle((float_32)iX, (float_32)iY, (float_32)iWidth, (float_32)iHeight, pPen);
}
void CPaintBase::DrawCircle(const rectf32 & rect, uint_32 uiColor, float_32 fLineWidth)
{
	DrawCircle(rect.x, rect.y, rect.w, rect.h, &CPen(uiColor, fLineWidth));
}

void CPaintBase::DrawCircle(const rectf32 & rect, IPen * pPen)
{
	DrawCircle(rect.x, rect.y, rect.w, rect.h, pPen);
}

void CPaintBase::DrawCircle(const rectix & rect, uint_32 uiColor, float_32 fLineWidth)
{
	DrawCircle((float_32)rect.x, (float_32)rect.y, (float_32)rect.w, (float_32)rect.h, &CPen(uiColor, fLineWidth));
}

void CPaintBase::DrawCircle(const rectix & rect, IPen * pPen)
{
	DrawCircle((float_32)rect.x, (float_32)rect.y, (float_32)rect.w, (float_32)rect.h, pPen);
}

void CPaintBase::FillCircle(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, uint_32 uiColor)
{
	FillCircle(fX, fY, fWidth, fHeight, &CSolidBrush(uiColor));
}

void CPaintBase::FillCircle(int_x iX, int_x iY, int_x iWidth, int_x iHeight, IBrush * pBrush)
{
	FillCircle((float_32)iX, (float_32)iY, (float_32)iWidth, (float_32)iHeight, pBrush);
}

void CPaintBase::FillCircle(const rectf32 & rect, uint_32 uiColor)
{
	FillCircle(rect.x, rect.y, rect.w, rect.h, &CSolidBrush(uiColor));
}

void CPaintBase::FillCircle(const rectf32 & rect, IBrush * pBrush)
{
	FillCircle(rect.x, rect.y, rect.w, rect.h, pBrush);
}

void CPaintBase::FillCircle(const rectix & rect, uint_32 uiColor)
{
	FillCircle((float_32)rect.x, (float_32)rect.y, (float_32)rect.w, (float_32)rect.h, &CSolidBrush(uiColor));
}

void CPaintBase::FillCircle(const rectix & rect, IBrush * pBrush)
{
	FillCircle((float_32)rect.x, (float_32)rect.y, (float_32)rect.w, (float_32)rect.h, pBrush);
}

void CPaintBase::FillCircle(float_32 fX, float_32 fY, float_32 fRadius, IBrush * pBrush)
{
	FillCircle(fX - fRadius, fY - fRadius, fRadius * 2.0f, fRadius * 2.0f, pBrush);
}

void CPaintBase::FillCircle(float_32 fX, float_32 fY, float_32 fRadius, uint_32 uiColor)
{
	FillCircle(fX - fRadius, fY - fRadius, fRadius * 2.0f, fRadius * 2.0f, uiColor);
}

void CPaintBase::FillCircle(int_x iX, int_x iY, int_x iRadius, IBrush * pBrush)
{
	FillCircle((float_32)(iX - iRadius), (float_32)(iY - iRadius), (float_32)iRadius * 2.0f, (float_32)iRadius * 2.0f, pBrush);
}

void CPaintBase::FillCircle(int_x iX, int_x iY, int_x iRadius, uint_32 uiColor)
{
	FillCircle((float_32)(iX - iRadius), (float_32)(iY - iRadius), (float_32)iRadius * 2.0f, (float_32)iRadius * 2.0f, uiColor);
}

void CPaintBase::FillTriangle(int_x iX1, int_x iY1, int_x iX2, int_x iY2, int_x iX3, int_x iY3, uint_32 uiColor)
{
	FillTriangle((float_32)iX1, (float_32)iY1, (float_32)iX2, (float_32)iY2, (float_32)iX3, (float_32)iY3, &CSolidBrush(uiColor));
}

void CPaintBase::FillTriangle(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, float_32 fX3, float_32 fY3, uint_32 uiColor)
{
	FillTriangle(fX1, fY1, fX2, fY2, fX3, fY3, &CSolidBrush(uiColor));
}

void CPaintBase::DrawRoundRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight,
	float_32 fConorW, float_32 fConorH, uint_32 uiColor, float_32 fLineWidth)
{

}

void CPaintBase::FillRoundRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight,
	float_32 fConorW, float_32 fConorH, uint_32 uiColor)
{

}

void CPaintBase::DrawRoundRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight,
	int_x iConorW, int_x iConorH, uint_32 uiColor, float_32 fLineWidth)
{
	DrawRoundRect((float_32)iX, (float_32)iY, (float_32)iWidth, (float_32)iHeight,
		(float_32)iConorW, (float_32)iConorH, &CPen(uiColor, fLineWidth));
}

void CPaintBase::FillRoundRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight,
	int_x iConorW, int_x iConorH, uint_32 uiColor)
{
	FillRoundRect((float_32)iX, (float_32)iY, (float_32)iWidth, (float_32)iHeight,
		(float_32)iConorW, (float_32)iConorH, &CSolidBrush(uiColor));
}

void CPaintBase::DrawRoundRect(const rectix & rect, int_x iConorW, int_x iConorH, uint_32 uiColor, float_32 fLineWidth)
{
	DrawRoundRect((float_32)rect.x, (float_32)rect.y, (float_32)rect.w, (float_32)rect.h,
		(float_32)iConorW, (float_32)iConorH, &CPen(uiColor, fLineWidth));
}

void CPaintBase::FillRoundRect(const rectf32 & rect, float_32 fConorW, float_32 fConorH, uint_32 uiColor)
{
	FillRoundRect(rect.x, rect.y, rect.w, rect.h, fConorW, fConorH, &CSolidBrush(uiColor));
}

void CPaintBase::DrawBezier3(const pointf32 & point1, const pointf32 & point2, const pointf32 & point3, IPen * pPen)
{
	DrawBezier3(point1.x, point1.y, point2.x, point2.y, point3.x, point3.y, pPen);
}

void CPaintBase::FillPolygon(const pointix * pPoints, int_x iCount, uint_32 uiColor)
{
	FillPolygon(pPoints, iCount, &CSolidBrush(uiColor));
}

void CPaintBase::DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, pointix point, uint_32 uiColor)
{
	DrawString(szText, iLength, format, point, &CSolidBrush(uiColor));
}

void CPaintBase::DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, rectix rect, uint_32 uiColor)
{
	DrawString(szText, iLength, format, rect, &CSolidBrush(uiColor));
}

void CPaintBase::DrawString(const textw & text, const textformat_t & format, pointix point, IBrush * pBrush)
{
	DrawString(text.buffer(), text.length(), format, point, pBrush);
}

void CPaintBase::DrawString(const textw & text, const textformat_t & format, rectix rect, IBrush * pBrush)
{
	DrawString(text.buffer(), text.length(), format, rect, pBrush);
}

void CPaintBase::DrawString(const textw & text, const textformat_t & format, pointix point, uint_32 uiColor)
{
	DrawString(text.buffer(), text.length(), format, point, &CSolidBrush(uiColor));
}

void CPaintBase::DrawString(const textw & text, const textformat_t & format, rectix rect, uint_32 uiColor)
{
	DrawString(text.buffer(), text.length(), format, rect, &CSolidBrush(uiColor));
}

void CPaintBase::DrawShape(const IShape * pShape, uint_32 uiColor, float_32 fWidth)
{
	DrawShape(pShape, &CPen(uiColor, fWidth));
}

void CPaintBase::FillShape(const IShape * pShape, uint_32 uiColor)
{
	FillShape(pShape, &CSolidBrush(uiColor));
}

void CPaintBase::DrawImage(IImage * pImage, float_32 fX, float_32 fY, AlignE align/* = AlignLeftTop*/)
{
	if(!pImage)
		return;

	sizeix siImage = pImage->GetSize();
	if(align & AlignRight)			fX -= float_32(siImage.w);
	else if(align & AlignCenterX)	fX -= float_32(siImage.w / 2);
	else {}

	if(align & AlignBottom)			fY -= float_32(siImage.h);
	else if(align & AlignCenterY)	fY -= float_32(siImage.h / 2);
	else {}

	DrawImage(pImage, fX, fY);
}

void CPaintBase::DrawImage(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc)
{
	if(!pImage)
		return;

	DrawImage(pImage, rcDst.x, rcDst.y, rcDst.w, rcDst.h,
		rcSrc.x, rcSrc.y, rcSrc.w, rcSrc.h);
}

void CPaintBase::DrawImage(IImage * pImage, float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight)
{
	if(!pImage)
		return;

	sizeix siImage = pImage->GetSize();
	DrawImage(pImage, fX, fY, fWidth, fHeight, (float_32)0, (float_32)0, (float_32)siImage.w, (float_32)siImage.h);
}

void CPaintBase::DrawImage(IImage * pImage, rectf32 rect)
{
	if(!pImage)
		return;

	sizeix siImage = pImage->GetSize();
	DrawImage(pImage,
		rect.x, rect.y, rect.w, rect.h, 
		(float_32)0, (float_32)0, (float_32)siImage.w, (float_32)siImage.h);
}

void CPaintBase::DrawImage(IImage * pImage,
	float_32 fDstX, float_32 fDstY,
	float_32 fSrcX, float_32 fSrcY, float_32 fSrcWidth, float_32 fSrcHeight)
{
	if(!pImage)
		return;

	DrawImage(pImage, fDstX, fDstY, fSrcWidth, fSrcHeight, fSrcX, fSrcY, fSrcWidth, fSrcHeight);
}

void CPaintBase::DrawImage(IImage * pImage, const pointf32 & ptDst, const rectf32 & rcSrc)
{
	if(!pImage)
		return;

	DrawImage(pImage, ptDst.x, ptDst.y, rcSrc.w, rcSrc.h, rcSrc.x, rcSrc.y, rcSrc.w, rcSrc.h);
}

void CPaintBase::DrawImage(IImage * pImage, int_x iX, int_x iY)
{
	if(!pImage)
		return;

	DrawImage(pImage, (float_32)iX, (float_32)iY);
}

void CPaintBase::DrawImage(IImage * pImage, int_x iX, int_x iY, AlignE align/* = AlignLeftTop*/)
{
	if(!pImage)
		return;

	DrawImage(pImage, (float_32)iX, (float_32)iY, align);
}

void CPaintBase::DrawImage(IImage * pImage, int_x iX, int_x iY, int_x iWidth, int_x iHeight)
{
	if(!pImage)
		return;

	DrawImage(pImage, (float_32)iX, (float_32)iY, (float_32)iWidth, (float_32)iHeight);
}
void CPaintBase::DrawImage(IImage * pImage, rectix rect)
{
	if(!pImage)
		return;

	DrawImage(pImage, (float_32)rect.x, (float_32)rect.y, (float_32)rect.w, (float_32)rect.h);
}

void CPaintBase::DrawImage(IImage * pImage,
	int_x iDstX, int_x iDstY,
	int_x iSrcX, int_x iSrcY, int_x iSrcWidth, int_x iSrcHeight)
{
	if(!pImage)
		return;

	DrawImage(pImage, (float_32)iDstX, (float_32)iDstY, (float_32)iSrcX,
		(float_32)iSrcY, (float_32)iSrcWidth, (float_32)iSrcHeight);
}

void CPaintBase::DrawImage(IImage * pImage,
	int_x iDstX, int_x iDstY, int_x iDstWidth, int_x iDstHeight,
	int_x iSrcX, int_x iSrcY, int_x iSrcWidth, int_x iSrcHeight)
{
	if(!pImage)
		return;

	DrawImage(pImage, (float_32)iDstX, (float_32)iDstY, (float_32)iDstWidth, (float_32)iDstHeight,
		(float_32)iSrcX, (float_32)iSrcY, (float_32)iSrcWidth, (float_32)iSrcHeight);
}

void CPaintBase::DrawImageGrid(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc, const rectf32 & rcGrid)
{
	if(!pImage)
		return;

	if(rcDst.is_valid() && rcSrc.is_valid())
	{
		if(rcDst.w < rcSrc.w)
			DrawImageGrid1x3(pImage, rcDst, rcSrc, rcGrid.y, rcGrid.h);
		else if(rcDst.h < rcSrc.h)
			DrawImageGrid3x1(pImage, rcDst, rcSrc, rcGrid.x, rcGrid.w);
		else
		{
			float_32 rightEdge = rcSrc.w - rcGrid.right();
			float_32 bottomEdge = rcSrc.h - rcGrid.bottom();
			float_32 width = rcDst.w - rcGrid.x - rightEdge;
			float_32 height = rcDst.h - rcGrid.y - bottomEdge;

			DrawImage(pImage,
				rcDst.x, rcDst.y, rcGrid.x, rcGrid.y,
				rcSrc.x, rcSrc.y, rcGrid.x, rcGrid.y); // 左上
			DrawImage(pImage,
				rcDst.x + rcGrid.x, rcDst.y, width, rcGrid.y,
				rcSrc.x + rcGrid.x, rcSrc.y, rcGrid.w, rcGrid.y); // 上中
			DrawImage(pImage,
				rcDst.right() - rightEdge, rcDst.y, rightEdge, rcGrid.y,
				rcSrc.x + rcGrid.right(), rcSrc.y, rightEdge, rcGrid.y); // 右上
			DrawImage(pImage,
				rcDst.right() - rightEdge, rcDst.y + rcGrid.y, rightEdge, height,
				rcSrc.x + rcGrid.right(), rcSrc.y + rcGrid.y, rightEdge, rcGrid.y); // 右中
			DrawImage(pImage,
				rcDst.right() - rightEdge, rcDst.bottom() - bottomEdge, rightEdge, bottomEdge,
				rcSrc.x + rcGrid.right(), rcSrc.y + rcGrid.bottom(), rightEdge, bottomEdge); // 右下
			DrawImage(pImage,
				rcDst.x + rcGrid.x, rcDst.bottom() - bottomEdge, width, bottomEdge,
				rcSrc.x + rcGrid.x, rcSrc.y + rcGrid.bottom(), rcGrid.w, bottomEdge); // 下中
			DrawImage(pImage,
				rcDst.x, rcDst.bottom() - bottomEdge, rcGrid.x, bottomEdge,
				rcSrc.x, rcSrc.y + rcGrid.bottom(), rcGrid.x, bottomEdge); // 左下
			DrawImage(pImage,
				rcDst.x, rcDst.y + rcGrid.y, rcGrid.x, height,
				rcSrc.x, rcSrc.y + rcGrid.y, rcGrid.x, rcGrid.h); // 左中
			DrawImage(pImage,
				rcDst.x + rcGrid.x, rcDst.y + rcGrid.y, width, height,
				rcSrc.x + rcGrid.x, rcSrc.y + rcGrid.y, rcGrid.w, rcGrid.h); // 左中
		}
	}
}

void CPaintBase::DrawImageGrid3x1(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc, float_32 fGridX, float_32 fGridW)
{
	if(!pImage)
		return;

	if(rcDst.is_valid() && rcSrc.is_valid())
	{
		if(rcDst.w < rcSrc.w)
			DrawImage(pImage, rcDst, rcSrc);
		else
		{
			float_32 fRightEdge = rcSrc.w - fGridX - fGridW;
			float_32 fWidth = rcDst.w - fGridX - fRightEdge;

			DrawImage(pImage,
				rcDst.x, rcDst.y, fGridX, rcDst.h,
				rcSrc.x, rcSrc.y, fGridX, rcSrc.h); // 左
			DrawImage(pImage,
				rcDst.x + fGridX, rcDst.y, fWidth, rcDst.h,
				rcSrc.x + fGridX, rcSrc.y, fGridW, rcSrc.h); // 中
			DrawImage(pImage,
				rcDst.right() - fRightEdge, rcDst.y, fRightEdge, rcDst.h,
				rcSrc.x + fGridX + fGridW, rcSrc.y, fRightEdge, rcSrc.h); // 下
		}
	}
}

void CPaintBase::DrawImageGrid3x1LREx(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc, float_32 fGridL, float_32 fGridM, float_32 fDstL)
{
	if(!pImage)
		return;

	if(rcDst.is_valid() && rcSrc.is_valid())
	{
		if(rcDst.w < rcSrc.w)
			DrawImage(pImage, rcDst, rcSrc);
		else
		{
			float_32 fGridR = rcSrc.w - fGridL - fGridM;
			float_32 fDstR = rcDst.w - fDstL - fGridM;

			DrawImage(pImage,
				rcDst.x, rcDst.y, fDstL, rcDst.h,
				rcSrc.x, rcSrc.y, fGridL, rcSrc.h); // 左
			DrawImage(pImage,
				rcDst.x + fDstL, rcDst.y, fGridM, rcDst.h,
				rcSrc.x + fGridL, rcSrc.y, fGridM, rcSrc.h); // 中
			DrawImage(pImage,
				rcDst.x + fDstL + fGridM, rcDst.y, fDstR, rcDst.h,
				rcSrc.x + fGridL + fGridM, rcSrc.y, fGridR, rcSrc.h); // 下
		}
	}
}

void CPaintBase::DrawImageGrid1x3(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc, float_32 fGridY, float_32 fGridH)
{
	if(!pImage)
		return;

	if(rcDst.is_valid() && rcSrc.is_valid())
	{
		if(rcDst.h < rcSrc.h)
			DrawImage(pImage, rcDst, rcSrc);
		else
		{
			float_32 fBottomEdge = rcSrc.h - fGridY - fGridH;
			float_32 fHeight = rcDst.h - fGridY - fBottomEdge;

			DrawImage(pImage,
				rcDst.x, rcDst.y, rcDst.w, fGridY,
				rcSrc.x, rcSrc.y, rcSrc.w, fGridY); // 上
			DrawImage(pImage,
				rcDst.x, rcDst.y + fGridY, rcDst.w, fHeight,
				rcSrc.x, rcSrc.y + fGridY, rcSrc.w, fGridH); // 中
			DrawImage(pImage,
				rcDst.x, rcDst.bottom() - fBottomEdge, rcDst.w, fBottomEdge,
				rcSrc.x, rcSrc.y + fGridY + fGridH, rcSrc.w, fBottomEdge); // 下
		}
	}
}

void CPaintBase::DrawImageGridH3V3(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc)
{
	ensure(pImage);
	rectf32 rcGrid = rcSrc;
	rcGrid.w /= 3;
	rcGrid.h /= 3;
	rcGrid.x = rcGrid.w;
	rcGrid.y = rcGrid.h;
	DrawImageGrid(pImage, rcDst, rcSrc, rcGrid);
}

void CPaintBase::DrawImageGridH3H(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc)
{
	ensure(pImage);
	float_32 fGrid = rcSrc.w / 3;
	DrawImageGrid3x1(pImage, rcDst, rcSrc, fGrid, fGrid);
}

void CPaintBase::DrawImageGridH3LR(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc)
{
	ensure(pImage);
	float_32 fGrid = rcSrc.w / 3;
	DrawImageGrid3x1LREx(pImage, rcDst, rcSrc, fGrid, fGrid, (rcDst.w - fGrid) * 0.5f);
}
void CPaintBase::DrawImageGridV3V(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc)
{
	ensure(pImage);
	DrawImageGrid1x3(pImage, rcDst, rcSrc, rcSrc.h / 3, rcSrc.h / 3);
}

void CPaintBase::DrawImageTileRepeat(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc)
{
	ensure(pImage);
	rectf32 dst(rcDst), src(rcSrc);
	float_32 fOffX = 0, fOffY = 0, fWidth = 0, fHeight = 0;
	while(true)
	{
		dst.w = rcSrc.w;
		dst.h = rcSrc.h;
		src = rcSrc;

		fOffX = dst.right() - rcDst.right();
		if(fOffX >= -F32_EPSILON)
		{
			dst.w -= fOffX;
			src.w -= fOffX;
		}
		fOffY = dst.bottom() - rcDst.bottom();
		if(fOffY >= -F32_EPSILON)
		{
			dst.h -= fOffY;
			src.h -= fOffY;
		}

		DrawImage(pImage, dst, src);

		dst.x += dst.w;

		if(fOffX >= -F32_EPSILON)
		{
			if(fOffY >= -F32_EPSILON)
				break;
			else
			{
				dst.x = rcDst.x;
				dst.y += dst.h;
			}
		}
	}
}
void CPaintBase::DrawImageTileMirror(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc)
{
	ensure(pImage);
	bool bMirrorH = false, bMirrorV = false;
	rectf32 dst(rcDst), src(rcSrc);
	float_32 fOffX = 0, fOffY = 0;
	while(true)
	{
		dst.w = rcSrc.w;
		dst.h = rcSrc.h;
		src = rcSrc;

		fOffX = dst.right() - rcDst.right();
		if(fOffX >= -F32_EPSILON)
		{
			dst.w -= fOffX;
			src.w -= fOffX;
			if(bMirrorH)
				src.x += fOffX;
		}
		fOffY = dst.bottom() - rcDst.bottom();
		if(fOffY >= -F32_EPSILON)
		{
			dst.h -= fOffY;
			src.h -= fOffY;
			if(bMirrorV)
				src.y += fOffY;
		}

		if(bMirrorH && bMirrorV)
			DrawImageMirrorHV(pImage, dst, src);
		else if(bMirrorH)
			DrawImageMirrorH(pImage, dst, src);
		else if(bMirrorV)
			DrawImageMirrorV(pImage, dst, src);
		else
			DrawImage(pImage, dst, src);

		dst.x += dst.w;
		if(fOffX >= -F32_EPSILON)
		{
			if(fOffY >= -F32_EPSILON)
				break;
			else
			{
				dst.x = rcDst.x;
				dst.y += dst.h;

				bMirrorH = false;
				bMirrorV = !bMirrorV;
			}
		}
		else
		{
			bMirrorH = !bMirrorH;
		}
	}
}

void CPaintBase::DrawImageMirrorH(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc)
{
	ensure(pImage);
	matrix3f matrix(-1, 0, 0, 1, rcDst.x * 2 + rcDst.w, 0);
	SetTransform(matrix);
	rectf32 dst = rcDst;
	dst.w += 1;
	DrawImage(pImage, dst, rcSrc);
	ResetTransform();
}

void CPaintBase::DrawImageMirrorV(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc)
{
	ensure(pImage);
	matrix3f matrix(1, 0, 0, -1, 0, rcDst.y * 2 + rcDst.h);
	SetTransform(matrix);
	rectf32 dst = rcDst;
	dst.h += 1;
	DrawImage(pImage, dst, rcSrc);
	ResetTransform();
}
void CPaintBase::DrawImageMirrorHV(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc)
{
	ensure(pImage);
	matrix3f matrix(-1, 0, 0, -1, rcDst.x * 2 + rcDst.w, rcDst.y * 2 + rcDst.h);
	SetTransform(matrix);
	rectf32 dst = rcDst;
	dst.w += 1;
	dst.h += 1;
	DrawImage(pImage, dst, rcSrc);
	ResetTransform();
}
void CPaintBase::DrawImageRotate(IImage * pImage, float_32 fX, float_32 fY, float_32 fRotation,
	float_32 fOrignX/* = 0*/, float_32 fOrignY/* = 0*/)
{
	ensure(pImage);
	matrix3f matrix;
	matrix.RotateAt(fRotation, fX + fOrignX, fY + fOrignY);
	SetTransform(matrix);
	DrawImage(pImage, fX, fY);
	ResetTransform();
}

void CPaintBase::DrawImageClamp(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc)
{
	float_32 fWidth = minof(rcDst.w, rcSrc.w);
	float_32 fHeight = minof(rcDst.h, rcSrc.h);
	DrawImage(pImage, rcDst.x, rcDst.y, fWidth, fHeight, rcSrc.x, rcSrc.y, fWidth, fHeight);
}

void CPaintBase::DrawImageWithMode(IImage * pImage, const rectix & rcDst, const rectix & rcSrc, ImageModeE eMode)
{
	switch(eMode)
	{
	case ImageModeScale:
		DrawImage(pImage, (rectf32)rcDst, (rectf32)rcSrc);
		break;
	case ImageModeGrid1x3:
		DrawImageGridV3V(pImage, (rectf32)rcDst, (rectf32)rcSrc);
		break;
	case ImageModeGrid3x1H:
		DrawImageGridH3H(pImage, (rectf32)rcDst, (rectf32)rcSrc);
		break;
	case ImageModeGrid3x1LR:
		DrawImageGridH3LR(pImage, (rectf32)rcDst, (rectf32)rcSrc);
		break;
	case ImageModeGrid3x3:
		DrawImageGridH3V3(pImage, (rectf32)rcDst, (rectf32)rcSrc);
		break;
	case ImageModeClamp:
		DrawImageClamp(pImage, (rectf32)rcDst, (rectf32)rcSrc);
		break;
	case ImageModeTileRepeat:
		DrawImageTileRepeat(pImage, (rectf32)rcDst, (rectf32)rcSrc);
		break;
	case ImageModeTileMirror:
		DrawImageTileMirror(pImage, (rectf32)rcDst, (rectf32)rcSrc);
		break;
	case ImageModeMirrorH:
		DrawImageMirrorH(pImage, (rectf32)rcDst, (rectf32)rcSrc);
		break;
	case ImageModeMirrorV:
		DrawImageMirrorV(pImage, (rectf32)rcDst, (rectf32)rcSrc);
		break;
	case ImageModeMirrorHV:
		DrawImageMirrorHV(pImage, (rectf32)rcDst, (rectf32)rcSrc);
		break;
	default: // ImageGridModeNone
		DrawImage(pImage, (rectf32)rcDst, (rectf32)rcSrc);
		break;
	}
}

VENUS_END
