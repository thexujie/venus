#pragma once

#include "D2DInc.h"

VENUS_BEG

struct WIN32_API D2DSolidBrushT
{
	uint_32 uiColor;
	ID2D1Brush * pBrush;
};

class WIN32_API D2DImageCacheT : public ObjectT<IImageCache>
{
public:
	D2DImageCacheT(ID2D1Bitmap * _pBitmap) : pBitmap(_pBitmap), bValid(true) {}
	~D2DImageCacheT() { SafeRelease(pBitmap); }
	void ReleaseCache() { SafeRelease(pBitmap); }

	ID2D1Bitmap * pBitmap;
	bool bValid;
};

class WIN32_API CPaintD2D : public CPaintBase
{
public:
	CPaintD2D(I2DRTarget * pRenderTarget, class C2DDeviceD2D * pService);
	~CPaintD2D();

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

	void DrawArc(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, float_32 fStartAngle, float_32 fAngle, IPen * pPen);

	void DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, pointix point, IBrush * pBrush);
	void DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, rectix rect, IBrush * pBrush);

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
public:
	void AffineX(int_x & iX);
	void AffineY(int_x & iY);
	void AffineXY(int_x & iX, int_x & iY);
	void AffineXY(float_32 & fX, float_32 & fY);
	void Affine(pointix & point);
	void AffineColor(uint_32 & color);
private:
	void Direct2Affine(D2D_POINT_2F & point);
	void Direct2Affine(D2D_RECT_F & rect);

private:
	ID2D1Brush * GetBrush(uint_32 color);
	ID2D1Brush * GetBrush(IPen * pPen);
	ID2D1Brush * GetBrush(IBrush * pBrush);
	IDWriteTextFormat * GetTextFormat(const textformat_t & format);
	ID2D1Bitmap * GetBitmap(IImage * pImage);
private:
	vector<pointix> m_origns;
	vector<rectix> m_clips;
	uint_32 m_colors[SysColorCount];

	ShapeRenderLevelE m_eShapeRenderLevel;

	I2DRTarget * m_pTarget;
	class C2DDeviceD2D * m_pService;
	ID2D1Factory * m_pFactory;
	ID2D1RenderTarget * m_pRenderTarget;
	IDWriteFactory * m_pDWrite;
	mutable HRESULT m_hResult;

	vector<D2DSolidBrushT> m_sBrushs;

	vector<D2DImageCacheT *> m_imgCaches;

public:
	static DXGI_FORMAT D2DPixelFormatFromColorMode(cmode_e eMode);
	static D2D1_ALPHA_MODE D2DAlphaModeFromColorMode(cmode_e eMode);
};

VENUS_END
