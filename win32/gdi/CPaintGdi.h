#pragma once

#include "GdiInc.h"
#include "CDevice2DGdi.h"

#ifdef WIN32_DEVICE_GDI

VENUS_BEG

class WIN32_API GdiImageCacheT : public ObjectT<IImageCache>
{
public:
	GdiImageCacheT(HBITMAP _hBitmap) : hBitmap(_hBitmap) {}
	~GdiImageCacheT() { ReleaseCache(); }
	void ReleaseCache() { if(hBitmap) { ::DeleteObject(hBitmap); hBitmap = NULL; } }

	HBITMAP hBitmap;
};

class WIN32_API CPaintGdi : public CPaintBase
{
public:
	CPaintGdi(I2DRTarget * pRenderTarget, CDevice2DGdi * pService);
	~CPaintGdi();
	HDC GetHDC();

	void ResetTarget(I2DRTarget * pTarget);
	bool BeginPaint();
	void EndPaint();
public:
	void SetShapeRenderLevel(ShapeRenderLevelE eShapeRenderLevel);
	ShapeRenderLevelE GetShapeRenderLevel() const;

	void PushOrign(int_x iX, int_x iY);
	void PushOrign(const pointix & point);
	pointix GetOrign() const;
	void PopOrign();
	void PushClip(int_x iX, int_x iY, int_x iWidth, int_x iHeight);
	void PushClip(const rectix & rect);
	rectix GetClip() const;
	void PopClip();

	void SetTransform(const matrix3f & matrix);
	void GetTransform(matrix3f & matrix) const;
	void ResetTransform();

	void SetImageColorMatrix(const matrix5f & matrix);
	void GetImageColorMatrix(matrix5f & matrix) const;
	void ResetImageColorMatrix();

	void SetTextRendLevel(FontRenderlevelE renderLevel);

	FontRenderlevelE GetTextRendLevel() const;

	void Clear(uint_32 uiColor);
	void Clear(rectix rcClear, uint_32 uiColor);

	void SetPixel(float_32 fX, float_32 fY, uint_32 uiColor);
	uint_32 GetPixel(float_32 fX, float_32 fY);

	void DrawLine(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, IPen * pPen);

	void DrawRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IPen * pPen);
	void FillRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IBrush * pBrush);

	void DrawRoundRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, float_32 fConorW, float_32 fConorH, IPen * pPen);
	void FillRoundRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, float_32 fConorW, float_32 fConorH, IBrush * pBrush);

	void DrawCircle(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IPen * pPen);
	void FillCircle(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IBrush * pBrush);
	void FillTriangle(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, float_32 fX3, float_32 fY3, IBrush * pBrush);

	void DrawBezier3(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, float_32 fX3, float_32 fY3, IPen * pPen);
	void DrawBezier4(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2,
		float_32 fX3, float_32 fY3, float_32 fX4, float_32 fY4, IPen * pPen);

	void DrawArc(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, float_32 fStartAngle, float_32 fAngle, IPen * pPen);

	void DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, pointix point, IBrush * pBrush);
	void DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, rectix rect, IBrush * pBrush);
	void DrawTextLayout(const ITextDocument * pDocument,
		const pointix & ptDraw, const rectix & rcClip,
		uint_32 uiColor);
	void DrawTextLayout(const ITextLayout * pTextLayout,
		const pointix & ptDraw, const rectix & rcClip,
		uint_32 uiColor);

	void DrawImage(IImage * pImage, float_32 fX, float_32 fY);
	void DrawImage(IImage * pImage,
		float_32 fDstX, float_32 fDstY, float_32 fDstWidth, float_32 fDstHeight,
		float_32 fSrcX, float_32 fSrcY, float_32 fSrcWidth, float_32 fSrcHeight);

	void DrawShape(const IShape * pShape, IPen * pPen);
	void FillShape(const IShape * pShape, IBrush * pBrush);

	void FillPolygon(const pointf32 * pPoints, int_x iCount, IBrush * pBrush);
	void FillPolygon(const pointix * pPoints, int_x iCount, IBrush * pBrush);

	uint_32 GetSystemColor(SysColorE eColor) const;
private:
	HPEN GetPen(uint_32 uiColor, float_32 fWidth);
	HPEN GetPen(IPen * pPen);
	HBRUSH GetBrush(uint_32 uiColor);
	HBRUSH GetBrush(IBrush * pBrush);
	HFONT GetFont(const font_t & font);
	void AffineColor(uint_32 & color);
public:
	void AffineX(int_x & iX);
	void AffineY(int_x & iY);
	void AffineXY(int_x & iX, int_x & iY);
	void AffineXY(float_32 & fX, float_32 & fY);
	void Affine(pointix & point);

	int_x GetAffineX() const;
	int_x GetAffineY() const;

private:
	HBITMAP GetImage(IImage * pImage);
private:
	HDC m_hdc;
	HDC m_hdcDraw;
	I2DRTarget * m_pTarget;
	CDevice2DGdi * m_pService;

	HRGN m_hRgnClip;
	HGDIOBJ m_hOldPen;
	HGDIOBJ m_hOldBrush;
	HGDIOBJ m_hOldFont;
	int_32 m_iOldBkMode;

	uint_32 m_colors[SysColorCount];

	vector<pointix> m_origns;
	vector<rectix> m_clips;
};

VENUS_END

#endif //WIN32_DEVICE_GDI

