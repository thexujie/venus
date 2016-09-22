#include "stdafx.h"
#include "CPaintGdip.h"

#ifdef WIN32_DEVICE_GDIP

VENUS_BEG

using namespace Gdiplus;
using namespace DllExports;

CPaintGdip::CPaintGdip(I2DRTarget * pTarget, C2DDeviceGdip * pService)
	: m_hdc(NULL), m_hBitmap(NULL), m_pTarget(nullptr), m_pService(pService), m_graphics(nullptr), m_bColorMatrix(false)
{
	Win32::GetSysColors(m_colors, SysColorCount);
	ResetTarget(pTarget);
}

CPaintGdip::~CPaintGdip()
{
	if(m_graphics)
	{
		Gdip::GdipDeleteGraphics(m_graphics);
		m_graphics = nullptr;
	}
	ResetTarget(nullptr);
}

void CPaintGdip::ResetTarget(I2DRTarget * pTarget)
{
	if(pTarget == m_pTarget)
		return;

	const oid_t & oid = m_pTarget ? m_pTarget->GetOid() : pTarget->GetOid();
	SafeRelease(m_pTarget);
	if(oid == OID_CBmpBuffer)
	{
		if(pTarget)
		{
			CBmpBuffer * pBitmap = (CBmpBuffer *)pTarget;
			m_hdc = pBitmap->GetBufferDC();
			m_hBitmap = pBitmap->GetHBitmap();
			m_status = Gdip::GdipCreateFromHDC(m_hdc, &m_graphics);
			InitializeGdipGraphics(m_graphics);
		}
		else
		{
			if(m_hdc)
			{
				Gdip::GdipDeleteGraphics(m_graphics);
				m_graphics = nullptr;
				m_hdc = NULL;
				m_hBitmap = NULL;
			}
		}
	}
	else if(oid == OID_CDxgiSurface)
	{
	}
	else {}

	m_pTarget = pTarget;
	SafeAddRef(m_pTarget);
}

bool CPaintGdip::BeginPaint()
{
	const oid_t & oid = m_pTarget->GetOid();
	if(oid == OID_CBmpBuffer)
	{
		CBmpBuffer * pBitmap = (CBmpBuffer *)m_pTarget;
		HDC hdc = pBitmap->GetBufferDC();
		HBITMAP hBitmap = pBitmap->GetHBitmap();
		if(hdc != m_hdc || hBitmap != m_hBitmap)
		{
			m_status = Gdip::GdipCreateFromHDC(hdc, &m_graphics);
			InitializeGdipGraphics(m_graphics);
			m_hdc = hdc;
			m_hBitmap = hBitmap;
		}
	}
	else if(oid == OID_CDxgiSurface)
	{
		CDxgiSurface * pDxgiSurface = (CDxgiSurface *)m_pTarget;
		IDXGISurface * pSurface = pDxgiSurface->DxgiGetSurface();
		IDXGISurface1 * pSurface1 = nullptr;
		if(pSurface)
			pSurface->QueryInterface(&pSurface1);
		if(pSurface1)
		{
			HDC hdc = NULL;
			pSurface1->GetDC(TRUE, &hdc);
			SafeRelease(pSurface1);
			if(!hdc)
				throw exp_bad_state();

			if(hdc != m_hdc)
			{
				m_status = Gdip::GdipCreateFromHDC(hdc, &m_graphics);
				InitializeGdipGraphics(m_graphics);
				m_hdc = hdc;
			}
		}
		else
		{
			log2(L"Can not get IDXGISurface1 interface!");
		}
	}
	else
		throw exp_bad_state();
	return true;
}

void CPaintGdip::EndPaint()
{
	const oid_t & oid = m_pTarget->GetOid();
	if(oid == OID_CBmpBuffer)
	{
		Gdip::GdipFlush(m_graphics, FlushIntentionFlush);
	}
	else if(oid == OID_CDxgiSurface)
	{
		CDxgiSurface * pDxgiSurface = (CDxgiSurface *)m_pTarget;
		if(m_hdc)
		{
			Gdip::GdipDeleteGraphics(m_graphics);
			m_graphics = nullptr;
			m_hdc = NULL;

			sizeix size = pDxgiSurface->GetSize();
			RECT rect = {0, 0, (int)size.w, (int)size.h};
			IDXGISurface * pSurface = pDxgiSurface->DxgiGetSurface();
			IDXGISurface1 * pSurface1 = nullptr;
			if(pSurface)
				pSurface->QueryInterface(&pSurface1);

			if(pSurface1)
			{
				HRESULT hResult = pSurface1->ReleaseDC(&rect);
				SafeRelease(pSurface1);
			}
		}
	}
	else
		throw exp_bad_state();
}

HDC CPaintGdip::GetHDC()
{
	return m_hdc;
}

void CPaintGdip::SetShapeRenderLevel(ShapeRenderLevelE eShapeRenderLevel)
{
	SetGraphicsShapeRenderLevel(m_graphics, eShapeRenderLevel);
}

ShapeRenderLevelE CPaintGdip::GetShapeRenderLevel() const
{
	return GetGraphicsShapeRenderLevel(m_graphics);
}

void CPaintGdip::PushOrign(int_x iX, int_x iY)
{
	PushOrign(pointix(iX, iY));
}

void CPaintGdip::PushOrign(const pointix & point)
{
	pointix temp = point;
	if(m_origns.is_valid())
		temp += m_origns.back();
	m_origns.add(temp);
}

pointix CPaintGdip::GetOrign() const
{
	if(m_origns.is_empty())
		return pointix();
	else
		return m_origns.back();
}

void CPaintGdip::PopOrign()
{
	m_origns.pop();
}

void CPaintGdip::PushClip(int_x iX, int_x iY, int_x iWidth, int_x iHeight)
{
	rectix rcClip(iX, iY, iWidth, iHeight);
	if(m_origns.is_valid())
	{
		const pointix & orign = m_origns.back();
		rcClip.x += orign.x;
		rcClip.y += orign.y;
	}

	if(m_clips.is_valid())
	{
		rectix rcParent = m_clips.back();
		int_x iLeft = rcClip.left;
		int_x iTop = rcClip.top;
		int_x iRight = rcClip.right();
		int_x iBottom = rcClip.bottom();
		if(iLeft < rcParent.x)
			iLeft = rcParent.x;
		if(iTop < rcParent.y)
			iTop = rcParent.y;
		if(iRight > rcParent.right())
			iRight = rcParent.right();
		if(iBottom > rcParent.bottom())
			iBottom = rcParent.bottom();
		rcClip.set(iLeft, iTop, iRight - iLeft, iBottom - iTop);
		if(!rcClip.is_valid())
			rcClip.set_empty();
	}
	m_clips.add(rcClip);
	m_status = Gdip::GdipSetClipRectI(m_graphics, (int_32)rcClip.x, (int_32)rcClip.y, (int_32)rcClip.w, (int_32)rcClip.h, CombineModeReplace);
}

void CPaintGdip::PushClip(const rectix & rect)
{
	PushClip(rect.x, rect.y, rect.w, rect.h);
}

rectix CPaintGdip::GetClip() const
{
	if(m_clips.is_empty())
		return rectix();
	else
		return m_clips.back();
}

void CPaintGdip::PopClip()
{
	m_clips.pop();

	//const CPointIntX & orign = m_origns.back();
	//rcClip.X += orign.X;
	//rcClip.Y += orign.Y;

	if(m_clips.is_empty())
		Gdip::GdipResetClip(m_graphics);
	else
	{
		rectix rcClip = m_clips.back();
		Gdip::GdipSetClipRectI(m_graphics, (int_32)rcClip.x, (int_32)rcClip.y, (int_32)rcClip.w, (int_32)rcClip.h, CombineModeReplace);
	}
}

void CPaintGdip::SetTransform(const matrix3f & matrix)
{
	//Matrix mtr(matrix(0, 0), matrix(0, 1), matrix(1, 0), matrix(1, 1), matrix(2, 0), matrix(2, 1));
	//m_graphics.SetTransform(&mtr);
}

void CPaintGdip::GetTransform(matrix3f & matrix) const
{
	//Matrix mtr;
	//m_graphics.GetTransform(&mtr);
	//float_32 values[6];
	//mtr.GetElements(values);
	//matrix.Set(values[0], values[1], values[2], values[3], values[4], values[5]);
}

void CPaintGdip::ResetTransform()
{
	//m_graphics.ResetTransform();
}

void CPaintGdip::SetImageColorMatrix(const matrix5f & matrix)
{
	m_matrColorMatrix = matrix;
	m_bColorMatrix = true;
}
void CPaintGdip::GetImageColorMatrix(matrix5f & matrix) const
{
	matrix = m_matrColorMatrix;
}
void CPaintGdip::ResetImageColorMatrix()
{
	m_bColorMatrix = false;
}


void CPaintGdip::Clear(uint_32 uiColor)
{
	AffineColor(uiColor);
	Gdip::GdipGraphicsClear(m_graphics, uiColor);
}

void CPaintGdip::Clear(rectix rcClear, uint_32 uiColor)
{
	PushClip(rcClear);
	Clear(uiColor);
	PopClip();
}

void CPaintGdip::SetPixel(float_32 fX, float_32 fY, uint_32 uiColor)
{
	AffineXY(fX, fY);
	AffineColor(uiColor);
	::SetPixel(m_hdc, (int_32)round(fX), (int_32)round(fY), uiColor);
}

uint_32 CPaintGdip::GetPixel(float_32 fX, float_32 fY)
{
	AffineXY(fX, fY);
	uint_32 uiColor = ::GetPixel(m_hdc, (int_32)round(fX), (int_32)round(fY));
	return uiColor;
}

void CPaintGdip::DrawLine(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, IPen * pPen)
{
	AffineXY(fX1, fY1);
	AffineXY(fX2, fY2);
	Gdip::GdipDrawLine(m_graphics, GetPen(pPen), fX1, fY1, fX2, fY2);
}

void CPaintGdip::DrawRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IPen * pPen)
{
	AffineXY(fX, fY);
	Gdip::GdipDrawRectangle(m_graphics, GetPen(pPen), fX, fY, fWidth, fHeight);
}

void CPaintGdip::FillRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IBrush * pBrush)
{
	AffineXY(fX, fY);
	Gdip::GdipFillRectangle(m_graphics, GetBrush(pBrush), fX, fY, fWidth, fHeight);
}

void CPaintGdip::DrawRoundRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, float_32 fConorW, float_32 fConorH, IPen * pPen)
{
	//AffineXY(fX, fY);
	//GpsRoundRectF rrect(RectF32(fX, fY, fWidth, fHeight), SizeF32(fConorW, fConorH));
	//m_graphics.DrawPath(GetPen(pPen), &rrect);
}

void CPaintGdip::FillRoundRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, float_32 fConorW, float_32 fConorH, IBrush * pBrush)
{
	//AffineXY(fX, fY);
	//GpsRoundRectF rrect(RectF32(fX, fY, fWidth, fHeight), SizeF32(fConorW, fConorH));
	//m_graphics.FillPath(GetBrush(pBrush), &rrect);
}


void CPaintGdip::DrawCircle(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IPen * pPen)
{
	AffineXY(fX, fY);
	Gdip::GdipDrawEllipse(m_graphics, GetPen(pPen), fX, fY, fWidth, fHeight);
}

void CPaintGdip::FillCircle(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IBrush * pBrush)
{
	AffineXY(fX, fY);
	Gdip::GdipFillEllipse(m_graphics, GetBrush(pBrush), fX, fY, fWidth, fHeight);
}

void CPaintGdip::FillTriangle(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, float_32 fX3, float_32 fY3, IBrush * pBrush)
{
	AffineXY(fX1, fY1);
	AffineXY(fX2, fY2);
	AffineXY(fX3, fY3);

	GpPointF points[4];
	points[0].X = fX1; points[0].Y = fY1;
	points[1].X = fX2; points[1].Y = fY2;
	points[2].X = fX3; points[2].Y = fY3;
	points[3].X = fX1; points[3].Y = fY1;

	Gdip::GdipFillPolygon(m_graphics, GetBrush(pBrush), points, 4, FillModeAlternate);
}

void CPaintGdip::DrawBezier3(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, float_32 fX3, float_32 fY3, IPen * pPen)
{
	float_32 fdX = (fX3 - fX1) * 0.5f;
	float_32 fdY = (fY3 - fY1) * 0.5f;
	Gdip::GdipDrawBezier(m_graphics, GetPen(pPen), fX1, fY1, fX2 - fdX, fY2 - fdY, fX2 + fdX, fY2 + fdY, fX3, fY3);
}

void CPaintGdip::DrawBezier4(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2,
	float_32 fX3, float_32 fY3, float_32 fX4, float_32 fY4, IPen * pPen)
{
	Gdip::GdipDrawBezier(m_graphics, GetPen(pPen), fX1, fY1, fX2, fY2, fX3, fY3, fX4, fY4);
}

void CPaintGdip::DrawArc(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, float_32 fStartAngle, float_32 fAngle, IPen * pPen)
{
	Gdip::GdipDrawArc(m_graphics, GetPen(pPen), fX, fY, fWidth, fHeight, fStartAngle, fAngle);
}

void CPaintGdip::DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, pointix point, IBrush * pBrush)
{
	if(iLength < 0)
		iLength = textlen(szText);
	Affine(point);
	gpfont_t font = GetFont(format);
	GpStringFormat * pFormat = GetFontFormat(format);
	Gdip::GdipDrawString(m_graphics, szText, (int_32)iLength, font.font,
		&GpRectF((float_32)point.x, (float_32)(point.y + font.external_leading), 0.0f, 0.0f),
		pFormat, GetBrush(pBrush));
}

void CPaintGdip::DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, rectix rect, IBrush * pBrush)
{
	if(iLength < 0)
		iLength = textlen(szText);

	Affine(rect.position);
	GpFontFamily * pFamily = m_pService->GetFontFamily(format.font.name);
	gpfont_t font = GetFont(format);
	GpStringFormat * pFormat = GetFontFormat(format);
	Gdip::GdipDrawString(m_graphics, szText, (int_32)iLength, font.font,
		&GpRectF((float_32)rect.x, (float_32)(rect.y + font.external_leading), (float_32)rect.w, (float_32)rect.h),
		pFormat, GetBrush(pBrush));
}

void CPaintGdip::DrawTextLayout(const ITextLayout * pTextLayout,
	const pointix & ptDraw, const rectix & rcClip,
	uint_32 uiColor)
{
	if(!pTextLayout || !m_hdc)
		return;

	pointix point = ptDraw;
	rectix rect = rcClip;
	AffineXY(point.x, point.y);
	AffineXY(rect.x, rect.y);
	AffineColor(uiColor);

	const ITextLayoutUsp * pTextLayoutUsp = dynamic_cast<const ITextLayoutUsp *>(pTextLayout);
	if(pTextLayoutUsp)
		pTextLayoutUsp->Draw(m_hdc, point.x, point.y, uiColor, rect);
}

void CPaintGdip::DrawImage(IImage * pImage, float_32 fX, float_32 fY)
{
	AffineXY(fX, fY);
	Gdip::GdipDrawImage(m_graphics, GetImage(pImage), fX, fY);
}

void CPaintGdip::DrawImage(IImage * pImage,
	float_32 fDstX, float_32 fDstY, float_32 fDstWidth, float_32 fDstHeight,
	float_32 fSrcX, float_32 fSrcY, float_32 fSrcWidth, float_32 fSrcHeight)
{
	AffineXY(fDstX, fDstY);

	//if(m_bColorMatrix)
	//{
	//	GpsImageAttributes ia;
	//	ia.SetColorMatrices((GpsColorMatrix *)m_matrColorMatrix.Values, nullptr);
	//	m_graphics.DrawImage(pGpsImage,
	//		GpsRectF(fDstX, fDstY, fDstWidth, fDstHeight),
	//		fSrcX, fSrcY, fSrcWidth, fSrcHeight, UnitPixel,
	//		&ia, NULL, NULL);
	//}
	//else
	{
		//GdipDrawImageRectRect(m_graphics, GetImage(pImage),
		//	fDstX, fDstY, fDstWidth, fDstHeight,
		//	fSrcX, fSrcY, fSrcWidth, fSrcHeight,
		//	UnitPixel,
		//	NULL, NULL, NULL);
		Gdip::GdipDrawImageRectRect(m_graphics, GetImage(pImage),
			fDstX, fDstY, fDstWidth, fDstHeight,
			fSrcX, fSrcY, fSrcWidth, fSrcHeight,
			UnitPixel,
			NULL, NULL, NULL);
	}
}

void CPaintGdip::SetTextRendLevel(FontRenderlevelE renderLevel)
{
	SetGraphicsTextRenderLevel(m_graphics, renderLevel);
}

FontRenderlevelE CPaintGdip::GetTextRendLevel() const
{
	return GetGraphicsTextRenderLevel(m_graphics);
}

void CPaintGdip::DrawShape(const IShape * pShape, IPen * pPen)
{
	//m_path.Reset();
	//float_32 fOrignX = 0.0f;
	//float_32 fOrignY = 0.0f;
	//AffineXY(fOrignX, fOrignY);
	//m_path.GenerateShape(pShape, fOrignX, fOrignY);
	//m_graphics.DrawPath(GetPen(pPen), &m_path);
}

void CPaintGdip::FillShape(const IShape * pShape, IBrush * pBrush)
{
	//m_path.Reset();
	//float_32 fOrignX = 0.0f;
	//float_32 fOrignY = 0.0f;
	//AffineXY(fOrignX, fOrignY);
	//m_path.GenerateShape(pShape, fOrignX, fOrignY);
	//m_graphics.FillPath(GetBrush(pBrush), &m_path);
}

void CPaintGdip::FillPolygon(const pointf32 * pPoints, int_x iCount, IBrush * pBrush)
{
	m_polygonF.clear();

	float_32 fX = 0.0f, fY = 0.0f;
	for(int_x cnt = 0; cnt < iCount; ++cnt)
	{
		fX = pPoints[cnt].x;
		fY = pPoints[cnt].y;
		AffineXY(fX, fY);
		m_polygonF.add(GpPointF(fX, fY));
	}
	m_status = Gdip::GdipFillPolygon(m_graphics, GetBrush(pBrush), m_polygonF, (int_32)iCount, FillModeAlternate);
}

void CPaintGdip::FillPolygon(const pointix * pPoints, int_x iCount, IBrush * pBrush)
{
	m_polygonI.clear();
	int_x iX = 0, iY = 0;
	for(int_x cnt = 0; cnt < iCount; ++cnt)
	{
		iX = pPoints[cnt].x;
		iY = pPoints[cnt].y;
		AffineXY(iX, iY);
		m_polygonI.add(GpPoint((int_32)iX, (int_32)iY));
	}
	m_status = Gdip::GdipFillPolygonI(m_graphics, GetBrush(pBrush), m_polygonI, (int_32)iCount, FillModeAlternate);

	//if(iCount > 1)
	//{
	//	--iCount;
	//	m_path.Reset();
	//	float_32 fX1 = 0.0f, fY1 = 0.0f;
	//	float_32 fX2 = 0.0f, fY2 = 0.0f;
	//	for(int_x cnt = 0; cnt < iCount; ++cnt)
	//	{
	//		fX1 = (float_32)pPoints[cnt].X;
	//		fY1 = (float_32)pPoints[cnt].Y;
	//		fX2 = (float_32)pPoints[cnt + 1].X;
	//		fY2 = (float_32)pPoints[cnt + 1].Y;
	//		AffineXY(fX1, fY1);
	//		AffineXY(fX2, fY2);
	//		m_path.AddLine(fX1, fY1, fX2, fY2);
	//	}
	//	m_graphics.FillPath(GetBrush(pBrush), &m_path);
	//}
}

uint_32 CPaintGdip::GetSystemColor(SysColorE eColor) const
{
	return Colors::Auto;
}

GpPen * CPaintGdip::GetPen(uint_32 uiColor, float_32 fWidth)
{
	AffineColor(uiColor);
	return m_pService->GetPen(uiColor, fWidth);
}

GpPen * CPaintGdip::GetPen(IPen * pPen)
{
	return GetPen(pPen->DoSample(), pPen->GetWidth());
}

GpBrush * CPaintGdip::GetBrush(uint_32 uiColor)
{
	AffineColor(uiColor);
	return m_pService->GetSolidBrush(uiColor);
}

GpBrush * CPaintGdip::GetBrush(IBrush * pBrush)
{
	return GetBrush(pBrush->DoSample());
}

gpfont_t CPaintGdip::GetFont(const textformat_t & format)
{
	return m_pService->GetFont(format);
}

Gdiplus::GpStringFormat * CPaintGdip::GetFontFormat(const textformat_t & format)
{
	return m_pService->GetFontFormat(format);
}

void CPaintGdip::AffineColor(uint_32 & color)
{
	if(color & 0xFF000000)
		return;

	uint_32 uiAlpha = color & 0xFF0000;
	color &= 0xFFFF;
	if(color < SysColorCount)
		color = m_colors[color];
	color |= uiAlpha;
}

void CPaintGdip::AffineX(int_x & iX)
{
	const pointix & orign = m_origns.back();
	iX += orign.x;
}

void CPaintGdip::AffineY(int_x & iY)
{
	const pointix & orign = m_origns.back();
	iY += orign.y;
}
void CPaintGdip::AffineXY(int_x & iX, int_x & iY)
{
	if(m_origns.is_valid())
	{
		const pointix & orign = m_origns.back();
		iX += orign.x;
		iY += orign.y;
	}
}

void CPaintGdip::AffineXY(float_32 & fX, float_32 & fY)
{
	if(m_origns.is_valid())
	{
		const pointix & orign = m_origns.back();
		fX += (float_32)orign.x;
		fY += (float_32)orign.y;
	}
}

void CPaintGdip::Affine(pointix & point)
{
	if(m_origns.is_valid())
	{
		const pointix & orign = m_origns.back();
		point += orign;
	}
}

int_x CPaintGdip::GetAffineX() const
{
	if(m_origns.is_valid())
		return m_origns.back().x;
	else
		return 0;
}

int_x CPaintGdip::GetAffineY() const
{
	if(m_origns.is_valid())
		return m_origns.back().y;
	else
		return 0;
}


GpImage * CPaintGdip::GetImage(IImage * pImage)
{
	return m_pService->GetImage(pImage);
}

VENUS_END

#endif //WIN32_DEVICE_GDIP
