#pragma once

#include "CSamplerBase.h"

VENUS_BEG

class V2D_API CPaintBase : public ObjectT<IPaint>
{
public:
	CPaintBase();
	~CPaintBase() {}

public: // 这些接口需要由子类来实现。
	virtual void SetTransform(const matrix3f & matrix) = 0;
	virtual void ResetTransform() = 0;

	virtual void PushClip(int_x iX, int_x iY, int_x iWidth, int_x iHeight) = 0;
	virtual void SetPixel(float_32 fX, float_32 fY, uint_32 uiColor) = 0;
	virtual uint_32 GetPixel(float_32 fX, float_32 fY) = 0;
	virtual void DrawLine(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, IPen * pPen) = 0;
	virtual void DrawRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IPen * pPen) = 0;
	virtual void FillRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IBrush * pBrush) = 0;
	virtual void DrawCircle(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IPen * pPen) = 0;
	virtual void FillCircle(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IBrush * pBrush) = 0;
	virtual void FillTriangle(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, float_32 fX3, float_32 fY3, IBrush * pBrush) = 0;
	virtual void DrawRoundRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, 
		float_32 fConorW, float_32 fConorH, IPen * pPen) = 0;
	virtual void FillRoundRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, 
		float_32 fConorW, float_32 fConorH, IBrush * pBrush) = 0;
	virtual void DrawBezier3(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, float_32 fX3, float_32 fY3, IPen * pPen) = 0;
	virtual void FillPolygon(const pointf32 * pPoints, int_x iCount, IBrush * pBrush) = 0;
	virtual void FillPolygon(const pointix * pPoints, int_x iCount, IBrush * pBrush) = 0;

	virtual void DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, pointix point, IBrush * pBrush) = 0;
	virtual void DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, rectix rect, IBrush * pBrush) = 0;

	virtual void DrawShape(const IShape * pShape, IPen * pPen) = 0;
	virtual void FillShape(const IShape * pShape, IBrush * pBrush) = 0;

	virtual void DrawImage(IImage * pImage, float_32 fX, float_32 fY) = 0;
	virtual void DrawImage(IImage * pImage, 
		float_32 fDstX, float_32 fDstY, float_32 fDstWidth, float_32 fDstHeight,
		float_32 fSrcX, float_32 fSrcY, float_32 fSrcWidth, float_32 fSrcHeight) = 0;

public:
	void SetPixel(int_x iX, int_x iY, uint_32 uiColor);
	uint_32 GetPixel(int_x iX, int_x iY);

	void DrawLine(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, uint_32 uiColor, float_32 fLineWidth);
	void DrawLine(int_x iX1, int_x iY1, int_x iX2, int_x iY2, IPen * pPen);
	void DrawLine(int_x iX1, int_x iY1, int_x iX2, int_x iY2, uint_32 uiColor, float_32 fLineWidth);
	void DrawLine(const pointf32 & point1, const pointf32 & point2, uint_32 uiColor, float_32 fLineWidth);
	void DrawLine(const pointf32 & point1, const pointf32 & point2, IPen * pPen);
	void DrawLine(const pointix & point1, const pointix & point2, uint_32 uiColor, float_32 fLineWidth);
	void DrawLine(const pointix & point1, const pointix & point2, IPen * pPen);
	void DrawPolygon(const pointf32 * pPoints, int_x iCount, uint_32 uiColor, float_32 fLineWidth);
	void DrawPolygon(const pointix * pPoints, int_x iCount, uint_32 uiColor, float_32 fLineWidth);

	void DrawRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, uint_32 uiColor, float_32 fLineWidth);
	void DrawRect(const rectf32 & rect, IPen * pPen);
	void DrawRect(const rectf32 & rect, uint_32 uiColor, float_32 fLineWidth);
	void DrawRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight, IPen * pPen);
	void DrawRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight, uint_32 uiColor, float_32 fLineWidth);
	void DrawRect(const rectix & rect, IPen * pPen);
	void DrawRect(const rectix & rect, uint_32 uiColor, float_32 fLineWidth);

	void FillRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, uint_32 uiColor);
	void FillRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight, IBrush * pBrush);
	void FillRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight, uint_32 uiColor);
	void FillRect(const rectix & rect, IBrush * pBrush);
	void FillRect(const rectix & rect, uint_32 uiColor);

	void DrawCircle(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, uint_32 uiColor, float_32 fLineWidth);
	void DrawCircle(int_x iX, int_x iY, int_x iWidth, int_x iHeight, IPen * pPen);
	void DrawCircle(const rectf32 & rect, uint_32 uiColor, float_32 fLineWidth);
	void DrawCircle(const rectf32 & rect, IPen * pPen);
	void DrawCircle(const rectix & rect, uint_32 uiColor, float_32 fLineWidth);
	void DrawCircle(const rectix & rect, IPen * pPen);

	void FillCircle(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, uint_32 uiColor);
	void FillCircle(int_x iX, int_x iY, int_x iWidth, int_x iHeight, IBrush * pBrush);
	void FillCircle(const rectf32 & rect, uint_32 uiColor);
	void FillCircle(const rectf32 & rect, IBrush * pBrush);
	void FillCircle(const rectix & rect, uint_32 uiColor);
	void FillCircle(const rectix & rect, IBrush * pBrush);

	void FillCircle(float_32 fX, float_32 fY, float_32 fRadius, IBrush * pBrush);
	void FillCircle(float_32 fX, float_32 fY, float_32 fRadius, uint_32 uiColor);
	void FillCircle(int_x iX, int_x iY, int_x iRadius, IBrush * pBrush);
	void FillCircle(int_x iX, int_x iY, int_x iRadius, uint_32 uiColor);

	void FillTriangle(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, float_32 fX3, float_32 fY3, uint_32 uiColor);
	void FillTriangle(int_x iX1, int_x iY1, int_x iX2, int_x iY2, int_x iX3, int_x iY3, uint_32 uiColor);

	void DrawRoundRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, 
		float_32 fConorW, float_32 fConorH, uint_32 uiColor, float_32 fLineWidth = 1.0f);
	void FillRoundRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, 
		float_32 fConorW, float_32 fConorH, uint_32 uiColor);
	void DrawRoundRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight, 
		int_x iConorW, int_x iConorH, uint_32 uiColor, float_32 fLineWidth = 1.0f);
	void FillRoundRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight, 
		int_x iConorW, int_x iConorH, uint_32 uiColor);
	void DrawRoundRect(const rectix & rect, int_x iConorW, int_x iConorH, uint_32 uiColor, float_32 fLineWidth = 1.0f);
	void FillRoundRect(const rectf32 & rect, float_32 fConorW, float_32 fConorH, uint_32 uiColor);

	void DrawBezier3(const pointf32 & point1, const pointf32 & point2, const pointf32 & point3, IPen * pPen);
	void FillPolygon(const pointix * pPoints, int_x iCount, uint_32 uiColor);

	void DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, pointix point, uint_32 uiColor);
	void DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, rectix rect, uint_32 uiColor);

	void DrawString(const textw & text, const textformat_t & format, pointix point, IBrush * pBrush);
	void DrawString(const textw & text, const textformat_t & format, rectix rect, IBrush * pBrush);
	void DrawString(const textw & text, const textformat_t & format, pointix point, uint_32 uiColor);
	void DrawString(const textw & text, const textformat_t & format, rectix rect, uint_32 uiColor);

	void DrawShape(const IShape * pShape, uint_32 uiColor, float_32 fWidth);
	void FillShape(const IShape * pShape, uint_32 uiColor);

public:
	void DrawImage(IImage * pImage, float_32 fX, float_32 fY, AlignE align);
	void DrawImage(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc);

	void DrawImage(IImage * pImage, float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight);
	void DrawImage(IImage * pImage, rectf32 rect);

	void DrawImage(IImage * pImage, 
		float_32 fDstX, float_32 fDstY,
		float_32 fSrcX, float_32 fSrcY, float_32 fSrcWidth, float_32 fSrcHeight);
	void DrawImage(IImage * pImage, const pointf32 & ptDst, const rectf32 & rcSrc);

	void DrawImage(IImage * pImage, int_x iX, int_x iY);
	void DrawImage(IImage * pImage, int_x iX, int_x iY, AlignE align = AlignLeftTop);
	void DrawImage(IImage * pImage, int_x iX, int_x iY, int_x iWidth, int_x iHeight);
	void DrawImage(IImage * pImage, rectix rect);
	void DrawImage(IImage * pImage, 
		int_x iDstX, int_x iDstY,
		int_x iSrcX, int_x iSrcY, int_x iSrcWidth, int_x iSrcHeight);
	void DrawImage(IImage * pImage, 
		int_x iDstX, int_x iDstY, int_x iDstWidth, int_x iDstHeight,
		int_x iSrcX, int_x iSrcY, int_x iSrcWidth, int_x iSrcHeight);

	void DrawImageGrid(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc, const rectf32 & rcGrid);
	void DrawImageGrid3x1LREx(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc, 
		float_32 fGridL, float_32 fGridM, float_32 fDstL);

	void DrawImageGrid3x1(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc, float_32 fGridX, float_32 fGridW);
	void DrawImageGrid1x3(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc, float_32 fGridY, float_32 fGridH);

	void DrawImageGridH3V3(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc);
	void DrawImageGridH3H(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc);
	void DrawImageGridH3LR(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc);
	void DrawImageGridV3V(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc);

	void DrawImageTileRepeat(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc);
	void DrawImageTileMirror(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc);
	void DrawImageMirrorH(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc);
	void DrawImageMirrorV(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc);
	void DrawImageMirrorHV(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc);

	void DrawImageRotate(IImage * pImage, float_32 fX, float_32 fY, float_32 fRotation, float_32 fOrignX = 0, float_32 fOrignY = 0);

	void DrawImageClamp(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc);

	void DrawImageWithMode(IImage * pImage, const rectix & rcDst, const rectix & rcSrc, ImageModeE eMode);

protected:
	int_x m_iOrignX;
	int_x m_iOrignY;
};

VENUS_END
