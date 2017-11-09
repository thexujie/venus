#include "stdafx.h"
#include "CPaintD2D.h"
#include "CDevice2DD2D.h"

VENUS_BEG

CPaintD2D::CPaintD2D(I2DRTarget * pTarget, CDevice2DD2D * pService)
	: m_pTarget(nullptr), m_pFactory(nullptr), m_pDWriteFactory(nullptr), m_pService(pService), m_pRenderTarget(nullptr), m_hResult(S_OK)
{
	m_pFactory = m_pService->GetFactory();
	m_pDWriteFactory = m_pService->GetDWrite();
	Win32::GetSysColors(m_colors, SysColorCount);
	ResetTarget(pTarget);
}

CPaintD2D::~CPaintD2D()
{
	ResetTarget(nullptr);
	SafeNull(m_pDWriteFactory);
	SafeNull(m_pFactory);
}

void CPaintD2D::ResetTarget(I2DRTarget * pTarget)
{
	if(pTarget == m_pTarget)
		return;

	for(int_x cnt = 0, len = m_sBrushs.size(); cnt < len; ++cnt)
		SafeRelease(m_sBrushs[cnt].pBrush);
	for(int_x cnt = 0, len = m_imgCaches.size(); cnt < len; ++cnt)
	{
		m_imgCaches[cnt]->ReleaseCache();
		m_imgCaches[cnt]->bValid = false;
		SafeRelease(m_imgCaches[cnt]);
	}

	m_sBrushs.clear();
	m_imgCaches.clear();
	SafeRelease(m_pRenderTarget);
	SafeRelease(m_pTarget);

	if(m_pService && pTarget)
	{
		float_32 fDpiX = 0.0f, fDpiY = 0.0f;
		m_pFactory->GetDesktopDpi(&fDpiX, &fDpiY);

		const cid_t & cid = pTarget->GetOid();
		if(cid == OID_CBmpBuffer)
		{
			D2D1_RENDER_TARGET_PROPERTIES d2drtp =
				D2D1::RenderTargetProperties(
					D2D1_RENDER_TARGET_TYPE_DEFAULT,
					D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
					fDpiX, fDpiY);

			ID2D1DCRenderTarget * pRenderTarget = nullptr;
			m_hResult = m_pFactory->CreateDCRenderTarget(&d2drtp, &pRenderTarget);
			if(pRenderTarget)
				m_pRenderTarget = pRenderTarget;
		}
		else if(cid == OID_CDxgiSurface)
		{
			D2D1_RENDER_TARGET_PROPERTIES d2drtp =
				D2D1::RenderTargetProperties(
					D2D1_RENDER_TARGET_TYPE_HARDWARE,
					D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
					fDpiX, fDpiY);

			ID2D1RenderTarget * pRenderTarget = nullptr;
			CDxgiSurface * pSurface = (CDxgiSurface *)pTarget;
			m_hResult = m_pFactory->CreateDxgiSurfaceRenderTarget(pSurface->DxgiGetSurface(), &d2drtp, &pRenderTarget);

			if(pRenderTarget)
				m_pRenderTarget = pRenderTarget;
			else
			{
				log2(L"Can not create ID2D1RenderTarget from IDXGISurface.");
			}
		}
		else
			throw exp_illegal_argument();

		if(m_pRenderTarget)
			m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		m_pTarget = pTarget;
		SafeAddRef(m_pTarget);
	}
	else
	{
		m_pTarget = nullptr;
	}
}

bool CPaintD2D::BeginPaint()
{
	if(m_pRenderTarget)
	{
		cid_t cid = m_pTarget->GetOid();
		if(cid == OID_CBmpBuffer)
		{
			CBmpBuffer * pBitmap = (CBmpBuffer *)m_pTarget;
			ID2D1DCRenderTarget * pTarget = (ID2D1DCRenderTarget *)m_pRenderTarget;
			RECT rect = {0, 0, (int)pBitmap->GetWidth(), (int)pBitmap->GetHeight()};
			pTarget->BindDC(pBitmap->GetBufferDC(), &rect);
		}
		m_pRenderTarget->BeginDraw();
		return true;
	}
	else
		return false;
}

void CPaintD2D::EndPaint()
{
	if(m_pRenderTarget)
		m_hResult = m_pRenderTarget->EndDraw();
}

void CPaintD2D::SetShapeRenderLevel(ShapeRenderLevelE eShapeRenderLevel)
{
	if(!m_pRenderTarget)
		return;

	m_eShapeRenderLevel = eShapeRenderLevel;
	switch(eShapeRenderLevel)
	{
	case ShapeRenderLevelDefault:
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
		break;
	case ShapeRenderLevelAnti:
		m_pRenderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
		break;
	default:
		throw exp_illegal_argument();
	}
}

ShapeRenderLevelE CPaintD2D::GetShapeRenderLevel() const
{
	return m_eShapeRenderLevel;
}

void CPaintD2D::PushOrign(int_x iX, int_x iY)
{
	if(m_origns.empty())
		m_origns.push(pointix(iX, iY));
	else
	{
		const pointix & orignBase = m_origns.back();
		m_origns.push(pointix(orignBase.x + iX, orignBase.y + iY));
	}
}

void CPaintD2D::PushOrign(const pointix & point)
{
	PushOrign(point.x, point.y);
}

pointix CPaintD2D::GetOrign() const
{
	if(m_origns.empty())
		return pointix();
	else
		return m_origns.back();
}

void CPaintD2D::PopOrign()
{
	m_origns.pop();
}

void CPaintD2D::PushClip(int_x iX, int_x iY, int_x iWidth, int_x iHeight)
{
	rectix rcClip(iX, iY, iWidth, iHeight);
	pointix orign = GetOrign();
	rcClip.x += orign.x;
	rcClip.y += orign.y;

	if(m_clips.valid())
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

	m_clips.push(rcClip);
	if(m_pRenderTarget)
	{
		D2D_RECT_F d2dRect = {(float_32)rcClip.x, (float_32)rcClip.y, (float_32)rcClip.right(), (float_32)rcClip.bottom()};
		m_pRenderTarget->PushAxisAlignedClip(d2dRect, D2D1_ANTIALIAS_MODE_ALIASED);
	}
}

void CPaintD2D::PushClip(const rectix & rect)
{
	PushClip(rect.x, rect.y, rect.w, rect.h);
}

rectix CPaintD2D::GetClip() const
{
	return m_clips.back();
}

void CPaintD2D::PopClip()
{
	m_clips.pop();
	if(m_pRenderTarget)
		m_pRenderTarget->PopAxisAlignedClip();
}

void CPaintD2D::SetTransform(const matrix3f & matrix)
{
	//Matrix mtr(matrix(0, 0), matrix(0, 1), matrix(1, 0), matrix(1, 1), matrix(2, 0), matrix(2, 1));
	//m_graphics.SetTransform(&mtr);
}

void CPaintD2D::GetTransform(matrix3f & matrix) const
{
	//Matrix mtr;
	//m_graphics.GetTransform(&mtr);
	//float_32 values[6];
	//mtr.GetElements(values);
	//matrix.Set(values[0], values[1], values[2], values[3], values[4], values[5]);
}

void CPaintD2D::ResetTransform()
{
	//m_graphics.ResetTransform();
}

void CPaintD2D::SetImageColorMatrix(const matrix5f & matrix)
{
	//m_matrColorMatrix = matrix;
	//m_bColorMatrix = true;
}

void CPaintD2D::GetImageColorMatrix(matrix5f & matrix) const
{
	//matrix = m_matrColorMatrix;
}

void CPaintD2D::ResetImageColorMatrix()
{
	//m_bColorMatrix = false;
}


void CPaintD2D::Clear(uint_32 uiColor)
{
	if(!m_pRenderTarget)
		return;

	AffineColor(uiColor);
	D2DColorT d2dColor(uiColor);
	m_pRenderTarget->Clear(d2dColor);
}

void CPaintD2D::Clear(rectix rcClear, uint_32 uiColor)
{
	if(!m_pRenderTarget)
		return;

	PushClip(rcClear);
	Clear(uiColor);
	PopClip();
}

void CPaintD2D::SetPixel(float_32 fX, float_32 fY, uint_32 uiColor)
{
	if(!m_pRenderTarget)
		return;

	AffineXY(fX, fY);
	fX = (float_32)(int_x)fX;
	fY = (float_32)(int_x)fY;
	D2D_RECT_F d2dRect = {fX, fY, fX + 1.0f, fY + 1.0f};
	m_pRenderTarget->DrawRectangle(d2dRect, GetBrush(uiColor), 1.0f, nullptr);
}

uint_32 CPaintD2D::GetPixel(float_32 fX, float_32 fY)
{
	if(!m_pRenderTarget)
		return 0;

	//m_pRenderTarget->get
	//	AffineXY(fX, fY);
	//HDC hdc = m_graphics.GetHDC();
	//uint_32 uiColor = ::GetPixel(hdc, (int_32)Round(fX), (int_32)Round(fY));
	//m_graphics.ReleaseHDC(hdc);
	//return uiColor;
	throw exp_not_implement();
}

void CPaintD2D::DrawLine(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, IPen * pPen)
{
	if(!m_pRenderTarget)
		return;

	verify(pPen);
	AffineXY(fX1, fY1);
	AffineXY(fX2, fY2);
	D2D_POINT_2F point1 = {fX1, fY1};
	D2D_POINT_2F point2 = {fX2, fY2};

	Direct2Affine(point1);
	Direct2Affine(point2);

	m_pRenderTarget->DrawLine(point1, point2, GetBrush(pPen), pPen->GetWidth(), nullptr);
}

void CPaintD2D::DrawRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IPen * pPen)
{
	if(!m_pRenderTarget)
		return;

	verify(pPen);
	AffineXY(fX, fY);
	D2D_RECT_F d2dRect = {fX, fY, fX + fWidth, fY + fHeight};
	Direct2Affine(d2dRect);
	m_pRenderTarget->DrawRectangle(d2dRect, GetBrush(pPen), pPen->GetWidth(), nullptr);
}

void CPaintD2D::FillRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IBrush * pBrush)
{
	if(!m_pRenderTarget)
		return;

	AffineXY(fX, fY);
	D2D_RECT_F d2dRect = {fX, fY, fX + fWidth, fY + fHeight};
	Direct2Affine(d2dRect);
	m_pRenderTarget->FillRectangle(d2dRect, GetBrush(pBrush));
}

void CPaintD2D::DrawRoundRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, float_32 fConorW, float_32 fConorH, IPen * pPen)
{
	if(!m_pRenderTarget)
		return;

	AffineXY(fX, fY);
	D2D_RECT_F d2dRect = {fX, fY, fX + fWidth, fY + fHeight};
	D2D1_ROUNDED_RECT d2dRounded = {{fX, fY, fX + fWidth, fY + fHeight}, fConorW, fConorH};
	m_pRenderTarget->DrawRoundedRectangle(d2dRounded, GetBrush(pPen), pPen->GetWidth(), nullptr);
}

void CPaintD2D::FillRoundRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, float_32 fConorW, float_32 fConorH, IBrush * pBrush)
{
	if(!m_pRenderTarget)
		return;

	AffineXY(fX, fY);
	D2D_RECT_F d2dRect = {fX, fY, fX + fWidth, fY + fHeight};
	D2D1_ROUNDED_RECT d2dRounded = {{fX, fY, fX + fWidth, fY + fHeight}, fConorW, fConorH};
	m_pRenderTarget->FillRoundedRectangle(d2dRounded, GetBrush(pBrush));
}


void CPaintD2D::DrawCircle(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IPen * pPen)
{
	if(!m_pRenderTarget)
		return;

	AffineXY(fX, fY);
	float_32 fRadiusX = fWidth * 0.5f;
	float_32 fRadiusY = fHeight * 0.5f;
	D2D1_ELLIPSE d2dEllipse = {{fX + fRadiusX, fY + fRadiusY}, fRadiusX, fRadiusY};
	m_pRenderTarget->DrawEllipse(d2dEllipse, GetBrush(pPen), pPen->GetWidth(), nullptr);
}

void CPaintD2D::FillCircle(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IBrush * pBrush)
{
	if(!m_pRenderTarget)
		return;

	AffineXY(fX, fY);
	float_32 fRadiusX = fWidth * 0.5f;
	float_32 fRadiusY = fHeight * 0.5f;
	D2D1_ELLIPSE d2dEllipse = {{fX + fRadiusX, fY + fRadiusY}, fRadiusX, fRadiusY};
	m_pRenderTarget->FillEllipse(d2dEllipse, GetBrush(pBrush));
}

void CPaintD2D::FillTriangle(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, float_32 fX3, float_32 fY3, IBrush * pBrush)
{
	if(!m_pRenderTarget)
		return;

	ID2D1PathGeometry * pGeometry = nullptr;
	m_hResult = m_pFactory->CreatePathGeometry(&pGeometry);
	if(pGeometry)
	{
		ID2D1GeometrySink * pSink = nullptr;
		m_hResult = pGeometry->Open(&pSink);
		if(pSink)
		{
			AffineXY(fX1, fY1);
			AffineXY(fX2, fY2);
			AffineXY(fX3, fY3);
			D2D_POINT_2F d2dPoint1 = {fX1, fY1};
			D2D_POINT_2F d2dPoint2 = {fX2, fY2};
			D2D_POINT_2F d2dPoint3 = {fX3, fY3};
			Direct2Affine(d2dPoint1);
			Direct2Affine(d2dPoint2);
			Direct2Affine(d2dPoint3);
			pSink->BeginFigure(d2dPoint1, D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddLine(d2dPoint2);
			pSink->AddLine(d2dPoint3);
			pSink->AddLine(d2dPoint1);
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			pSink->Close();
			SafeRelease(pSink);
			m_pRenderTarget->FillGeometry(pGeometry, GetBrush(pBrush), nullptr);
		}
		SafeRelease(pGeometry);
	}
}

void CPaintD2D::DrawBezier3(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, float_32 fX3, float_32 fY3, IPen * pPen)
{
	if(!m_pRenderTarget)
		return;

	AffineXY(fX1, fY1);
	AffineXY(fX2, fY2);
	AffineXY(fX3, fY3);
	ID2D1PathGeometry * pGeometry = nullptr;
	m_hResult = m_pFactory->CreatePathGeometry(&pGeometry);
	if(pGeometry)
	{
		ID2D1GeometrySink * pSink = nullptr;
		m_hResult = pGeometry->Open(&pSink);
		if(pSink)
		{
			D2D1_BEZIER_SEGMENT d2dBezier = {{fX1, fY1}, {fX2, fY2}, {fX3, fY3}};
			D2D_POINT_2F d2dPoint = {0.0f, 0.0f};
			pSink->BeginFigure(d2dPoint, D2D1_FIGURE_BEGIN_FILLED);
			pSink->AddBezier(d2dBezier);
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			pSink->Close();
			SafeRelease(pSink);
			m_pRenderTarget->DrawGeometry(pGeometry, GetBrush(pPen), pPen->GetWidth(), nullptr);
		}
		SafeRelease(pGeometry);
	}
}

void CPaintD2D::DrawArc(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight,
	float_32 fStartAngle, float_32 fAngle, IPen * pPen)
{
	if(!m_pRenderTarget)
		return;

	//ID2D1PathGeometry * pGeometry = nullptr;
	//m_hResult = m_pD2DFactory->CreatePathGeometry(&pGeometry);
	//if(pGeometry)
	//{
	//	ID2D1GeometrySink * pSink = nullptr;
	//	m_hResult = pGeometry->Open(&pSink);
	//	if(pSink)
	//	{
	//		AffineXY(fX, fY);
	//		D2D1_ARC_SEGMENT d2dArc = {{fX, fY}, {fWidth, fHeight}, fAngle, D2D1_SWEEP_DIRECTION_CLOCKWISE, D2D1_ARC_SIZE_SMALL};
	//		pSink->BeginFigure((D2D1::Point2F(0.0f, 0.0f), D2D1_FIGURE_BEGIN_FILLED));
	//		pSink->AddArc(d2dArc);
	//		pSink->Close();
	//		SafeRelease(pSink);
	//		m_pRenderTarget->DrawGeometry(pGeometry, GetBrush(pPen->DoSample()), pPen->GetWidth(), nullptr);
	//	}
	//	SafeRelease(pGeometry);
	//}
}

void CPaintD2D::DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, pointix point, IBrush * pBrush)
{
	if(!m_pRenderTarget)
		return;

	if(iLength < 0)
		iLength = textlen(szText);
	if(!iLength)
		return;

	Affine(point);
	rectix rcClip = GetClip();
	D2D_RECT_F d2dRect = {(float_32)point.x, (float_32)point.y, (float_32)rcClip.right(), (float_32)rcClip.bottom()};
	D2D_POINT_2F d2dPoint = {d2dRect.left, d2dRect.top};
	IDWriteTextLayout * pLayout = nullptr;
	IDWriteTextFormat * pFormat = GetTextFormat(format);
	//m_pRenderTarget->DrawTextW(szText, iLength, pFormat, &d2dRect, GetBrush(pBrush), D2D1_DRAW_TEXT_OPTIONS_CLIP);
	m_pDWriteFactory->CreateTextLayout(szText, (uint_32)iLength, pFormat, F32_MAX, F32_MAX, &pLayout);
	if(pLayout)
	{
		m_pRenderTarget->DrawTextLayout(d2dPoint, pLayout, GetBrush(pBrush), D2D1_DRAW_TEXT_OPTIONS_CLIP);
		SafeRelease(pLayout);
	}
}

void CPaintD2D::DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, rectix rect, IBrush * pBrush)
{
	if(!m_pRenderTarget)
		return;

	if(iLength < 0)
		iLength = textlen(szText);
	if(!iLength)
		return;

	Affine(rect.position);
	D2D_RECT_F d2dRect = {(float_32)rect.x, (float_32)rect.y, (float_32)(rect.right()), (float_32)(rect.bottom())};
	Direct2Affine(d2dRect);

	IDWriteTextFormat * pFormat = GetTextFormat(format);

	// 如果在调用 DrawText 之前，SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE),
	// DrawText 会非常卡。
	m_pRenderTarget->DrawTextW(szText, (uint_32)iLength,
		pFormat,
		d2dRect,
		GetBrush(pBrush),
		D2D1_DRAW_TEXT_OPTIONS_CLIP, DWRITE_MEASURING_MODE_NATURAL);
}

void CPaintD2D::DrawTextLayout(const ITextLayout * pTextLayout,
	const pointix & ptDraw, const rectix & rcClip,
	uint_32 uiColor)
{
	if(!m_pRenderTarget)
		return;

	if(!pTextLayout)
		return;

	const ITextLayoutD2D * pDwTextLayout = dynamic_cast<const ITextLayoutD2D *>(pTextLayout);
	if(!pDwTextLayout)
		return;

	IDWriteTextLayout * pLayout = pDwTextLayout->GetDxTextLayout();
	if(pLayout)
	{
		pointix point = ptDraw;
		AffineXY(point.x, point.y);

		D2D_POINT_2F d2dPoint = {(float_32)(point.x), (float_32)(point.y)};
		Direct2Affine(d2dPoint);
		m_pRenderTarget->DrawTextLayout(d2dPoint, pLayout, GetBrush(uiColor), D2D1_DRAW_TEXT_OPTIONS_NONE);
	}
}

void CPaintD2D::DrawImage(IImage * pImage, float_32 fX, float_32 fY)
{
	if(!m_pRenderTarget)
		return;

	AffineXY(fX, fY);
	ID2D1Bitmap * pBitmap = GetBitmap(pImage);
	if(!pBitmap)
		return;

	sizeix siImage = pImage->GetSize();
	D2D_RECT_F d2dRect = {fX, fY, fX + (float_32)siImage.w, fY + (float_32)siImage.h};
	m_pRenderTarget->DrawBitmap(pBitmap,
		d2dRect,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		nullptr);
}

void CPaintD2D::DrawImage(IImage * pImage,
	float_32 fDstX, float_32 fDstY, float_32 fDstWidth, float_32 fDstHeight,
	float_32 fSrcX, float_32 fSrcY, float_32 fSrcWidth, float_32 fSrcHeight)
{
	if(!m_pRenderTarget)
		return;

	AffineXY(fDstX, fDstY);

	ID2D1Bitmap * pBitmap = GetBitmap(pImage);
	if(!pBitmap)
		return;

	D2D_RECT_F d2dRectDst = {fDstX, fDstY, fDstX + fDstWidth, fDstY + fDstHeight};
	D2D_RECT_F d2dRectSrc = {fSrcX, fSrcY, fSrcX + fSrcWidth, fSrcY + fSrcHeight};
	m_pRenderTarget->DrawBitmap(pBitmap,
		d2dRectDst,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		d2dRectSrc);
}

void CPaintD2D::SetTextRendLevel(FontRenderlevelE renderLevel)
{
	if(!m_pRenderTarget)
		return;

	switch(renderLevel)
	{
	case FontRenderLevelSystem:
		m_pRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_DEFAULT);
		break;
	case FontRenderLevelClearTypeGrid:
		m_pRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
		break;
	case FontRenderLevelGray:
		m_pRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
		break;
	case FontRenderLevelGrayGrid:
		m_pRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
		break;
	case FontRenderLevelAntiGray:
		m_pRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
		break;
	case FontRenderLevelAntiGrayGrid:
		m_pRenderTarget->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
		break;
	default:
		break;
	}
}

FontRenderlevelE CPaintD2D::GetTextRendLevel() const
{
	if(!m_pRenderTarget)
		return FontRenderLevelSystem;

	return FontRenderLevelSystem;
}

void CPaintD2D::DrawShape(const IShape * pShape, IPen * pPen)
{
	if(!m_pRenderTarget)
		return;

	//m_path.Reset();
	//float_32 fOrignX = 0.0f;
	//float_32 fOrignY = 0.0f;
	//AffineXY(fOrignX, fOrignY);
	//m_path.GenerateShape(pShape, fOrignX, fOrignY);
	//m_graphics.DrawPath(&GpsPenEx(pPen), &m_path);
}

void CPaintD2D::FillShape(const IShape * pShape, IBrush * pBrush)
{
	if(!m_pRenderTarget)
		return;

	//m_path.Reset();
	//float_32 fOrignX = 0.0f;
	//float_32 fOrignY = 0.0f;
	//AffineXY(fOrignX, fOrignY);
	//m_path.GenerateShape(pShape, fOrignX, fOrignY);
	//m_graphics.FillPath(&GpsSolidBrushEx(pBrush), &m_path);
}

void CPaintD2D::FillPolygon(const pointf32 * pPoints, int_x iCount, IBrush * pBrush)
{
	if(!m_pRenderTarget)
		return;

	ID2D1PathGeometry * pGeometry = nullptr;
	m_hResult = m_pFactory->CreatePathGeometry(&pGeometry);
	if(pGeometry)
	{
		ID2D1GeometrySink * pSink = nullptr;
		m_hResult = pGeometry->Open(&pSink);
		if(pSink)
		{
			D2D_POINT_2F d2dPoint = {0.0f, 0.0f};
			pSink->BeginFigure(d2dPoint, D2D1_FIGURE_BEGIN_FILLED);
			for(int_x cnt = 0; cnt < iCount; ++cnt)
			{
				const pointf32 * pPoint = pPoints + cnt;
				float_32 fX = pPoint->x;
				float_32 fY = pPoint->y;
				AffineXY(fX, fY);
				D2D_POINT_2F d2dPoint = {fX, fY};
				Direct2Affine(d2dPoint);
				pSink->AddLine(d2dPoint);
			}
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			pSink->Close();
			SafeRelease(pSink);
			m_pRenderTarget->FillGeometry(pGeometry, GetBrush(pBrush), nullptr);
		}
		SafeRelease(pGeometry);
	}
}

void CPaintD2D::FillPolygon(const pointix * pPoints, int_x iCount, IBrush * pBrush)
{
	if(!m_pRenderTarget)
		return;

	ID2D1PathGeometry * pGeometry = nullptr;
	m_hResult = m_pFactory->CreatePathGeometry(&pGeometry);
	if(pGeometry)
	{
		ID2D1GeometrySink * pSink = nullptr;
		m_hResult = pGeometry->Open(&pSink);
		if(pSink)
		{
			for(int_x cnt = 0; cnt < iCount; ++cnt)
			{
				const pointix * pPoint = pPoints + cnt;
				float_32 fX = (float_32)pPoint->x;
				float_32 fY = (float_32)pPoint->y;
				AffineXY(fX, fY);
				D2D_POINT_2F d2dPoint = {fX, fY};
				Direct2Affine(d2dPoint);
				if(cnt == 0)
					pSink->BeginFigure(d2dPoint, D2D1_FIGURE_BEGIN_FILLED);
				else
					pSink->AddLine(d2dPoint);
			}
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			pSink->Close();
			SafeRelease(pSink);
			m_pRenderTarget->FillGeometry(pGeometry, GetBrush(pBrush), nullptr);
		}
		SafeRelease(pGeometry);
	}
}

uint_32 CPaintD2D::GetSystemColor(SysColorE eColor) const
{
	if(0 <= eColor && eColor < SysColorCount)
		return m_colors[eColor];
	else
		return Colors::Auto;
}

void CPaintD2D::AffineX(int_x & iX)
{
	pointix orign = GetOrign();
	iX += orign.x;
}

void CPaintD2D::AffineY(int_x & iY)
{
	pointix orign = GetOrign();
	iY += orign.y;
}

void CPaintD2D::AffineXY(int_x & iX, int_x & iY)
{
	pointix orign = GetOrign();
	iX += orign.x;
	iY += orign.y;
}

void CPaintD2D::AffineColor(uint_32 & color)
{
	if(color < SysColorCount)
		color = m_colors[color];
}

void CPaintD2D::Direct2Affine(D2D_POINT_2F & point)
{
	point.x += 0.5f;
	point.y += 0.5f;
}
void CPaintD2D::Direct2Affine(D2D_RECT_F & rect)
{
	rect.left += 0.5f;
	rect.top += 0.5f;
	rect.right += 0.5f;
	rect.bottom += 0.5f;
}

void CPaintD2D::AffineXY(float_32 & fX, float_32 & fY)
{
	pointix orign = GetOrign();
	fX += (float_32)orign.x;
	fY += (float_32)orign.y;
}

void CPaintD2D::Affine(pointix & point)
{
	if(m_origns.valid())
	{
		const pointix & orign = m_origns.back();
		point += orign;
	}
}

ID2D1Brush * CPaintD2D::GetBrush(uint_32 color)
{
	AffineColor(color);

	if(!m_pRenderTarget)
		return nullptr;

	for(int_x cnt = 0, len = m_sBrushs.size(); cnt < len; ++cnt)
	{
		const D2DSolidBrushT & d2dBrush = m_sBrushs[cnt];
		if(d2dBrush.uiColor == color)
			return d2dBrush.pBrush;
	}

	ID2D1SolidColorBrush * pBrush = nullptr;
	D2DColorT d2dColor(color);
	m_hResult = m_pRenderTarget->CreateSolidColorBrush(d2dColor, &pBrush);
	if(pBrush)
	{
		D2DSolidBrushT newBrush = {color, pBrush};
		m_sBrushs.add(newBrush);
		return pBrush;
	}
	else
		return nullptr;
}

ID2D1Brush * CPaintD2D::GetBrush(IPen * pPen)
{
	return GetBrush(pPen->DoSample());
}

ID2D1Brush * CPaintD2D::GetBrush(IBrush * pBrush)
{
	return GetBrush(pBrush->DoSample());
}

IDWriteTextFormat * CPaintD2D::GetTextFormat(const textformat_t & format)
{
	if(m_pService)
	{
		IDWriteTextFormat * pFormat = m_pService->GetTextFormat(format);
		return pFormat;
	}
	else
		return nullptr;
}

ID2D1Bitmap * CPaintD2D::GetBitmap(IImage * pImage)
{
	if(!pImage)
		return nullptr;

	D2DImageCacheT * pD2DCache = (D2DImageCacheT *)pImage->GetCache();
	if(!pD2DCache || !pD2DCache->pBitmap)
	{
		ImageBufferT buffer = pImage->GetBuffer();

		D2D1_BITMAP_PROPERTIES bmpProps = {};
		bmpProps.pixelFormat.format = D2DPixelFormatFromColorMode(buffer.mode);
		bmpProps.pixelFormat.alphaMode = D2DAlphaModeFromColorMode(buffer.mode);

		ID2D1Bitmap * pBitmap = nullptr;
		D2D_SIZE_U d2dSize = {(uint_32)buffer.width, (uint_32)buffer.height};
		m_hResult = m_pRenderTarget->CreateBitmap(d2dSize, buffer.buffer, (uint_32)buffer.pitch, bmpProps, &pBitmap);

		if(!pD2DCache)
		{
			pD2DCache = new D2DImageCacheT(pBitmap);
			m_imgCaches.add(pD2DCache);
			pImage->SetCache(pD2DCache);
		}
		else
		{
			pD2DCache->pBitmap = pBitmap;
			if(!pD2DCache->bValid)
			{
				pD2DCache->bValid = true;
				m_imgCaches.add(pD2DCache);
				pD2DCache->AddRef();
			}
		}
		return pBitmap;
	}
	else
	{
		if(!pD2DCache->bValid)
		{
			pD2DCache->bValid = true;
			m_imgCaches.add(pD2DCache);
			pD2DCache->AddRef();
		}
		return pD2DCache->pBitmap;
	}
}

DXGI_FORMAT CPaintD2D::D2DPixelFormatFromColorMode(cmode_e eMode)
{
	switch(eMode)
	{
	case cmode_a8r8g8b8:	return DXGI_FORMAT_B8G8R8A8_UNORM;
	case cmode_x8r8g8b8:	return DXGI_FORMAT_B8G8R8X8_UNORM; // D2D1_ALPHA_MODE_IGNORE
	default:				return DXGI_FORMAT_UNKNOWN;
	}
}

D2D1_ALPHA_MODE CPaintD2D::D2DAlphaModeFromColorMode(cmode_e eMode)
{
	switch(eMode)
	{
	case cmode_a8r8g8b8:	return  D2D1_ALPHA_MODE_PREMULTIPLIED;
	case cmode_x8r8g8b8:	return D2D1_ALPHA_MODE_IGNORE;
	default:				return D2D1_ALPHA_MODE_UNKNOWN;
	}
}

VENUS_END
