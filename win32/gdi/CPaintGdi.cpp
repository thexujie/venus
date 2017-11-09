#include "stdafx.h"
#include "CPaintGdi.h"
#include "CImageGdi.h"

#ifdef WIN32_DEVICE_GDI

VENUS_BEG

#pragma warning(push)
#pragma warning(disable : 4244)

CPaintGdi::CPaintGdi(I2DRTarget * pTarget, CDevice2DGdi * pService)
	: m_hdc(NULL), m_hdcDraw(NULL), m_pTarget(nullptr), m_pService(pService),
	m_hRgnClip(NULL), m_hOldPen(NULL), m_hOldBrush(NULL), m_hOldFont(NULL), m_iOldBkMode(0)
{
	Win32::GetSysColors(m_colors, SysColorCount);
	ResetTarget(pTarget);
}

CPaintGdi::~CPaintGdi()
{
	DeleteDC(m_hdc);
	m_hdc = NULL;
	ResetTarget(nullptr);
}

void CPaintGdi::ResetTarget(I2DRTarget * pTarget)
{
	if(pTarget != m_pTarget)
	{
		SafeRelease(m_pTarget);
		m_pTarget = pTarget;
		SafeAddRef(m_pTarget);
	}
}

bool CPaintGdi::BeginPaint()
{
	const cid_t & cid = m_pTarget->GetOid();
	if(cid == OID_CBmpBuffer)
	{
		CBmpBuffer * pBitmap = (CBmpBuffer *)m_pTarget;
		HDC hdc = pBitmap->GetBufferDC();
		if(hdc != m_hdc)
		{
			m_hdc = hdc;

			m_hOldPen = ::SelectObject(m_hdc, GetPen(Colors::Auto, 1.0f));
			m_hOldBrush = ::SelectObject(m_hdc, GetBrush(Colors::Auto));
			m_hOldFont = ::SelectObject(m_hdc, GetFont(font_t()));
			m_iOldBkMode = ::SetBkMode(m_hdc, TRANSPARENT);
		}
	}
	else if(cid == OID_CDxgiSurface)
	{
		CDxgiSurface * pDxgiSurface = (CDxgiSurface *)m_pTarget;
		IDXGISurface * pSurface = pDxgiSurface->DxgiGetSurface();
		IDXGISurface1 * pSurface1 = nullptr;
		if(pSurface)
			pSurface->QueryInterface(&pSurface1);

		if(pSurface1)
		{
#ifdef _DEBUG
			DXGI_SURFACE_DESC sDesc = {};
			pSurface1->GetDesc(&sDesc);
			if(sDesc.Format != DXGI_FORMAT_B8G8R8A8_UNORM &&
				sDesc.Format != DXGI_FORMAT_B8G8R8A8_UNORM_SRGB)
				log1(L"PaintGdi suggest the DXGI_FORMAT_B8G8R8A8_UNORM or DXGI_FORMAT_B8G8R8A8_UNORM_SRGB format for IDXGISurface1!");
#endif
			HDC hdc = NULL;
			HRESULT hr = pSurface1->GetDC(FALSE, &hdc);
			if(hdc && hdc != m_hdc)
			{
				m_hdc = hdc;
				//TextOut(hdc, 10, 10, L"ABBD", 4);
				m_hOldPen = ::SelectObject(m_hdc, GetPen(Colors::Auto, 1.0f));
				m_hOldBrush = ::SelectObject(m_hdc, GetBrush(Colors::Auto));
				m_hOldFont = ::SelectObject(m_hdc, GetFont(font_t()));
				m_iOldBkMode = ::SetBkMode(m_hdc, TRANSPARENT);
			}
			SafeRelease(pSurface1);
		}
	}
	else
		throw exp_bad_state();
	return true;
}

void CPaintGdi::EndPaint()
{
	const cid_t & cid = m_pTarget->GetOid();
	if(cid == OID_CBmpBuffer)
	{
		if(m_hdc)
		{
			if(m_hRgnClip)
			{
				::SelectClipRgn(m_hdc, NULL);
				::DeleteObject(m_hRgnClip);
				m_hRgnClip = NULL;
			}

			::SetBkMode(m_hdc, m_iOldBkMode);
			::SelectObject(m_hdc, m_hOldPen);
			::SelectObject(m_hdc, m_hOldBrush);
			::SelectObject(m_hdc, m_hOldFont);

			m_hOldPen = NULL;
			m_hOldBrush = NULL;
			m_hOldFont = NULL;
			m_hdc = NULL;
		}
	}
	else if(cid == OID_CDxgiSurface)
	{
		CDxgiSurface * pDxgiSurface = (CDxgiSurface *)m_pTarget;
		if(m_hdc)
		{
			if(m_hRgnClip)
			{
				::SelectClipRgn(m_hdc, NULL);
				::DeleteObject(m_hRgnClip);
				m_hRgnClip = NULL;
			}

			::SetBkMode(m_hdc, m_iOldBkMode);
			::SelectObject(m_hdc, m_hOldPen);
			::SelectObject(m_hdc, m_hOldBrush);
			::SelectObject(m_hdc, m_hOldFont);

			m_hOldPen = NULL;
			m_hOldBrush = NULL;
			m_hOldFont = NULL;
			m_hdc = NULL;

			sizeix size = pDxgiSurface->GetSize();
			IDXGISurface * pSurface = pDxgiSurface->DxgiGetSurface();
			IDXGISurface1 * pSurface1 = nullptr;
			if(pSurface)
				pSurface->QueryInterface(&pSurface1);

			if(pSurface1)
			{
				RECT rect = {0, 0, (int)size.w, (int)size.h};
				HRESULT hResult = pSurface1->ReleaseDC(&rect);
				SafeRelease(pSurface1);
			}
		}
	}
	else
		throw exp_bad_state();
}

HDC CPaintGdi::GetHDC()
{
	return m_hdc;
}

void CPaintGdi::SetShapeRenderLevel(ShapeRenderLevelE eShapeRenderLevel)
{
}

ShapeRenderLevelE CPaintGdi::GetShapeRenderLevel() const
{
	return ShapeRenderLevelDefault;
}

void CPaintGdi::PushOrign(int_x iX, int_x iY)
{
	PushOrign(pointix(iX, iY));
}

void CPaintGdi::PushOrign(const pointix & point)
{
	pointix temp = point;
	if(m_origns.valid())
		temp += m_origns.back();
	m_origns.add(temp);
}

pointix CPaintGdi::GetOrign() const
{
	if(m_origns.empty())
		return pointix();
	else
		return m_origns.back();
}

void CPaintGdi::PopOrign()
{
	m_origns.pop();
}

void CPaintGdi::PushClip(int_x iX, int_x iY, int_x iWidth, int_x iHeight)
{
	rectix rcClip(iX, iY, iWidth, iHeight);
	if(m_origns.valid())
	{
		const pointix & orign = m_origns.back();
		rcClip.x += orign.x;
		rcClip.y += orign.y;
	}

	m_clips.add(rcClip);
	RECT rect = {(int_32)rcClip.x, (int_32)rcClip.y,
		(int_32)(rcClip.x + rcClip.w) + 0, (int_32)(rcClip.y + rcClip.h) + 0};

	if(m_hRgnClip)
	{
		::SelectClipRgn(m_hdc, NULL);
		::DeleteObject(m_hRgnClip);
		m_hRgnClip = NULL;
	}

	m_hRgnClip = ::CreateRectRgnIndirect(&rect);
	::SelectClipRgn(m_hdc, m_hRgnClip);
}

void CPaintGdi::PushClip(const rectix & rect)
{
	PushClip(rect.x, rect.y, rect.w, rect.h);
}

rectix CPaintGdi::GetClip() const
{
	if(m_clips.empty())
		return rectix();
	else
		return m_clips.back();
}

void CPaintGdi::PopClip()
{
	m_clips.pop();

	if(m_clips.empty())
		::SelectClipRgn(m_hdc, NULL);
	else
	{
		rectix rcClip = m_clips.back();
		RECT rect = {(int_32)rcClip.x, (int_32)rcClip.y,
			(int_32)(rcClip.x + rcClip.w) + 0, (int_32)(rcClip.y + rcClip.h) + 0};

		if(m_hRgnClip)
		{
			::SelectClipRgn(m_hdc, NULL);
			::DeleteObject(m_hRgnClip);
			m_hRgnClip = NULL;
		}

		m_hRgnClip = ::CreateRectRgnIndirect(&rect);
		::SelectClipRgn(m_hdc, m_hRgnClip);
	}
}

void CPaintGdi::SetTransform(const matrix3f & matrix)
{
	//Matrix mtr(matrix(0, 0), matrix(0, 1), matrix(1, 0), matrix(1, 1), matrix(2, 0), matrix(2, 1));
	//m_graphics.SetTransform(&mtr);
}

void CPaintGdi::GetTransform(matrix3f & matrix) const
{
	//Matrix mtr;
	//m_graphics.GetTransform(&mtr);
	//float_32 values[6];
	//mtr.GetElements(values);
	//matrix.Set(values[0], values[1], values[2], values[3], values[4], values[5]);
}

void CPaintGdi::ResetTransform()
{
	//m_graphics.ResetTransform();
}

void CPaintGdi::SetImageColorMatrix(const matrix5f & matrix)
{
}
void CPaintGdi::GetImageColorMatrix(matrix5f & matrix) const
{
}
void CPaintGdi::ResetImageColorMatrix()
{
}


void CPaintGdi::Clear(uint_32 uiColor)
{
	AffineColor(uiColor);
	int_x iWidth = GetDeviceCaps(m_hdc, VERTRES);
	int_x iHeight = GetDeviceCaps(m_hdc, HORZRES);
	FillRect(0, 0, iWidth, iHeight, &CSolidBrush(uiColor));
}

void CPaintGdi::Clear(rectix rcClear, uint_32 uiColor)
{
	PushClip(rcClear);
	Clear(uiColor);
	PopClip();
}

void CPaintGdi::SetPixel(float_32 fX, float_32 fY, uint_32 uiColor)
{
	AffineXY(fX, fY);
	AffineColor(uiColor);
	::SetPixel(m_hdc, (int_32)round(fX), (int_32)round(fY), uiColor);
}

uint_32 CPaintGdi::GetPixel(float_32 fX, float_32 fY)
{
	AffineXY(fX, fY);
	uint_32 uiColor = ::GetPixel(m_hdc, (int_32)round(fX), (int_32)round(fY));
	return uiColor;
}

void CPaintGdi::DrawLine(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, IPen * pPen)
{
	AffineXY(fX1, fY1);
	AffineXY(fX2, fY2);
	::SelectObject(m_hdc, GetPen(pPen));
	float_32 fK = (fY2 - fY1) / (fX2 - fX1);
	//if(fX2 > fX1)
	//{
	//	if(fK <= -2.414f)
	//		fY2 -= 1.0f;
	//	else if(fK <= -0.414f)
	//	{
	//		fX2 += 1.0f;
	//		fY2 -= 1.0f;
	//	}
	//	else if(fK <= -0.0f)
	//		fX2 += 1.0f;
	//	else if(fK <= 0.414f)
	//		fY2 += 1.0f;
	//	else if(fK <= 2.414f)
	//	{
	//		fX2 += 1.0f;
	//		fY2 += 1.0f;
	//	}
	//	else
	//		fX2 += 1.0f;
	//}

	MoveToEx(m_hdc, fX1, fY1, NULL);
	LineTo(m_hdc, fX2, fY2);
}

void CPaintGdi::DrawRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IPen * pPen)
{
	AffineXY(fX, fY);
	::SelectObject(m_hdc, GetPen(pPen));
	::SelectObject(m_hdc, GetBrush(Colors::Transparent));
	::Rectangle(m_hdc, fX, fY, fX + fWidth + 1, fY + fHeight + 1);
}

void CPaintGdi::FillRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IBrush * pBrush)
{
	AffineXY(fX, fY);
	RECT rect = {(int_32)fX, (int_32)fY, (int_32)(fX + fWidth), (int_32)(fY + fHeight)};
	::FillRect(m_hdc, &rect, GetBrush(pBrush));
}

void CPaintGdi::DrawRoundRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, float_32 fConorW, float_32 fConorH, IPen * pPen)
{
	//AffineXY(fX, fY);
	//GpsRoundRectF rrect(RectF32(fX, fY, fWidth, fHeight), SizeF32(fConorW, fConorH));
	//m_graphics.DrawPath(GetPen(pPen), &rrect);
}

void CPaintGdi::FillRoundRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, float_32 fConorW, float_32 fConorH, IBrush * pBrush)
{
	//AffineXY(fX, fY);
	//GpsRoundRectF rrect(RectF32(fX, fY, fWidth, fHeight), SizeF32(fConorW, fConorH));
	//m_graphics.FillPath(GetBrush(pBrush), &rrect);
}


void CPaintGdi::DrawCircle(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IPen * pPen)
{
	AffineXY(fX, fY);
	::SelectObject(m_hdc, GetPen(pPen));
	::Ellipse(m_hdc, fX, fY, fWidth, fHeight);
}

void CPaintGdi::FillCircle(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IBrush * pBrush)
{
	AffineXY(fX, fY);
	::SelectObject(m_hdc, GetStockObject(NULL_PEN));
	::SelectObject(m_hdc, GetBrush(pBrush));
	::Ellipse(m_hdc, fX, fY, fX + fWidth, fY + fHeight);
}

void CPaintGdi::FillTriangle(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, float_32 fX3, float_32 fY3, IBrush * pBrush)
{
	AffineXY(fX1, fY1);
	AffineXY(fX2, fY2);
	AffineXY(fX3, fY3);

	::BeginPath(m_hdc);
	MoveToEx(m_hdc, fX1, fY1, NULL);
	LineTo(m_hdc, fX2, fY2);
	LineTo(m_hdc, fX3, fY3);
	LineTo(m_hdc, fX1, fY1);
	::EndPath(m_hdc);
	::SelectObject(m_hdc, GetBrush(pBrush));
	::FillPath(m_hdc);
}

void CPaintGdi::DrawBezier3(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, float_32 fX3, float_32 fY3, IPen * pPen)
{
	AffineXY(fX1, fY1);
	AffineXY(fX2, fY2);
	AffineXY(fX3, fY3);

	float_32 fdX = (fX3 - fX1) * 0.5f;
	float_32 fdY = (fY3 - fY1) * 0.5f;

	POINT points[4] = {{(int_32)fX1, (int_32)fY1},
	{(int_32)(fX2 - fdX), (int_32)(fY2 - fdY)},
	{(int_32)(fX2 + fdX), (int_32)(fY2 + fdY)},
	{(int_32)fX3, (int_32)fY3}};
	::SelectObject(m_hdc, GetPen(pPen));
	::PolyBezier(m_hdc, points, 4);
}

void CPaintGdi::DrawBezier4(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2,
	float_32 fX3, float_32 fY3, float_32 fX4, float_32 fY4, IPen * pPen)
{
	POINT points[4] = {{(int_32)fX1, (int_32)fY1}, {(int_32)fX2, (int_32)fY2}, {(int_32)fX3, (int_32)fY3}, {(int_32)fX4, (int_32)fY4}};
	::SelectObject(m_hdc, GetPen(pPen));
	::PolyBezier(m_hdc, points, 4);
}

void CPaintGdi::DrawArc(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, float_32 fStartAngle, float_32 fAngle, IPen * pPen)
{
}

void CPaintGdi::DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, pointix point, IBrush * pBrush)
{
	if(iLength < 0)
		iLength = textlen(szText);
	Affine(point);

	uint_32 color = pBrush->DoSample();
	AffineColor(color);

	::SetTextColor(m_hdc, color & 0x00FFFFFF);
	::SelectObject(m_hdc, GetFont(format.font));

	TEXTMETRICW tm;
	::GetTextMetricsW(m_hdc, &tm);

	uint_32 uiAlign = 0;
	if(format.align & AlignRight)
		uiAlign |= TA_RIGHT;
	if(format.align & AlignCenterX)
		uiAlign |= TA_CENTER;
	else
		uiAlign |= TA_LEFT;

	if(format.align & AlignBottom)
	{
		uiAlign |= TA_BOTTOM;
	}
	else if(format.align & AlignCenterY)
	{
		uiAlign |= TA_TOP;
		point.y -= format.font.size / 2;
	}
	else
		uiAlign |= TA_TOP;

	uint_32 uiAlignOld = SetTextAlign(m_hdc, uiAlign);
	TextOutW(m_hdc, (int_32)point.x, (int_32)point.y + tm.tmExternalLeading, szText, (int_32)iLength);
	SetTextAlign(m_hdc, uiAlignOld);
}

void CPaintGdi::DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, rectix rect, IBrush * pBrush)
{
	if(iLength < 0)
		iLength = textlen(szText);

	Affine(rect.position);
	uint_32 color = pBrush->DoSample();
	AffineColor(color);

	::SetTextColor(m_hdc, color & 0x00FFFFFF);

	uint_32 uiAlign = 0;

	if(format.align & AlignRight)
		uiAlign |= TA_RIGHT;
	if(format.align & AlignCenterX)
		uiAlign |= TA_CENTER;
	else
		uiAlign |= TA_LEFT;

	if(format.align & AlignBottom)
	{
		uiAlign |= TA_BOTTOM;
		rect.y = rect.y + rect.h;
	}
	else if(format.align & AlignCenterY)
	{
		uiAlign |= TA_TOP;
		rect.y += rect .h / 2 - format.font.size / 2;
	}
	else
		uiAlign |= TA_TOP;

	winrect_t winrect = rect;
	::SelectObject(m_hdc, GetFont(format.font));
	TEXTMETRICW tm;
	::GetTextMetricsW(m_hdc, &tm);
	uint_32 uiAlignOld = SetTextAlign(m_hdc, uiAlign);
	ExtTextOutW(m_hdc, (int_32)rect.x, (int_32)rect.y + tm.tmExternalLeading, ETO_CLIPPED, &winrect, szText, iLength, NULL);
	SetTextAlign(m_hdc, uiAlignOld);
}

void CPaintGdi::DrawTextLayout(const ITextDocument * pDocument,
	const pointix & ptDraw, const rectix & rcClip,
	uint_32 uiColor)
{
	if(!pDocument || !m_hdc)
		return;

	pointix point = ptDraw;
	rectix rect = rcClip;
	AffineXY(point.x, point.y);
	AffineXY(rect.x, rect.y);
	AffineColor(uiColor);

	int_x iTop = point.y, iBottom = 0;
	for(int_x cnt = 0, len = pDocument->GetParagraphCount(); cnt < len; ++cnt)
	{
		const ITextLayout * pTextLayout = pDocument->GetParagraph(cnt);
		if(!pTextLayout)
			continue;

		tl_metrics_t tm = pTextLayout->GetMetrics();
		iBottom = iTop + tm.tl_height;

		if(iBottom < rect.y)
		{
			iTop = iBottom;
			continue;
		}

		if(iTop >= rect.bottom())
			break;

		CTextLayoutUsp * pUspTl = (CTextLayoutUsp *)pTextLayout;
		pUspTl->Draw(m_hdc, point.x, iTop, uiColor, rect);
		iTop = iBottom;
	}
}

void CPaintGdi::DrawTextLayout(const ITextLayout * pTextLayout,
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

	CTextLayoutUsp * pUspTl = (CTextLayoutUsp *)pTextLayout;
	pUspTl->Draw(m_hdc, point.x, point.y, uiColor, rect);
}

void CPaintGdi::DrawImage(IImage * pImage, float_32 fX, float_32 fY)
{
	AffineXY(fX, fY);

	sizeix size = pImage->GetSize();
	HBITMAP hBitmap = GetImage(pImage);
	if(!hBitmap)
		return;

	if(!m_hdcDraw)
		m_hdcDraw = CreateCompatibleDC(m_hdc);

	HGDIOBJ hBitmapOld = ::SelectObject(m_hdcDraw, hBitmap);
	if(pImage->GetCMode() == cmode_a8r8g8b8)
	{
		BLENDFUNCTION bfun = {};
		bfun.BlendOp = AC_SRC_OVER;
		bfun.SourceConstantAlpha = 0xff;
		bfun.AlphaFormat = AC_SRC_ALPHA;
		::GdiAlphaBlend(m_hdc, fX, fY, size.w, size.h, m_hdcDraw, 0, 0, size.w, size.h, bfun);
	}
	else
	{
		::BitBlt(m_hdc, fX, fY, size.w, size.h, m_hdcDraw, 0, 0, SRCCOPY);
	}
	::SelectObject(m_hdcDraw, hBitmapOld);
}

void CPaintGdi::DrawImage(IImage * pImage,
	float_32 fDstX, float_32 fDstY, float_32 fDstWidth, float_32 fDstHeight,
	float_32 fSrcX, float_32 fSrcY, float_32 fSrcWidth, float_32 fSrcHeight)
{

	AffineXY(fDstX, fDstY);

	HBITMAP hBitmap = GetImage(pImage);
	if(!hBitmap)
		return;

	if(!m_hdcDraw)
		m_hdcDraw = CreateCompatibleDC(m_hdc);

	HGDIOBJ hBitmapOld = ::SelectObject(m_hdcDraw, hBitmap);
	if(pImage->GetCMode() == cmode_a8r8g8b8)
	{
		BLENDFUNCTION bfun = {};
		bfun.BlendOp = AC_SRC_OVER;
		bfun.SourceConstantAlpha = 0xff;
		bfun.AlphaFormat = AC_SRC_ALPHA;
		::GdiAlphaBlend(m_hdc, fDstX, fDstY, fDstWidth, fDstHeight, m_hdcDraw, fSrcX, fSrcY, fSrcWidth, fSrcHeight, bfun);
	}
	else
	{
		::StretchBlt(m_hdc, fDstX, fDstY, fDstWidth, fDstHeight, m_hdcDraw, fSrcX, fSrcY, fSrcWidth, fSrcHeight, SRCCOPY);
	}
	::SelectObject(m_hdcDraw, hBitmapOld);
}

void CPaintGdi::SetTextRendLevel(FontRenderlevelE renderLevel)
{
}

FontRenderlevelE CPaintGdi::GetTextRendLevel() const
{
	return FontRenderLevelSystem;
}

void CPaintGdi::DrawShape(const IShape * pShape, IPen * pPen)
{
	//m_path.Reset();
	//float_32 fOrignX = 0.0f;
	//float_32 fOrignY = 0.0f;
	//AffineXY(fOrignX, fOrignY);
	//m_path.GenerateShape(pShape, fOrignX, fOrignY);
	//m_graphics.DrawPath(GetPen(pPen), &m_path);
}

void CPaintGdi::FillShape(const IShape * pShape, IBrush * pBrush)
{
	//m_path.Reset();
	//float_32 fOrignX = 0.0f;
	//float_32 fOrignY = 0.0f;
	//AffineXY(fOrignX, fOrignY);
	//m_path.GenerateShape(pShape, fOrignX, fOrignY);
	//m_graphics.FillPath(GetBrush(pBrush), &m_path);
}

void CPaintGdi::FillPolygon(const pointf32 * pPoints, int_x iCount, IBrush * pBrush)
{
	bool bFirst = true;
	float_32 fX = 0.0f, fY = 0.0f;
	::BeginPath(m_hdc);
	for(int_x cnt = 0; cnt < iCount; ++cnt)
	{
		fX = pPoints[cnt].x;
		fY = pPoints[cnt].y;
		AffineXY(fX, fY);
		if(bFirst)
		{
			MoveToEx(m_hdc, fX, fY, NULL);
			bFirst = false;
		}
		else
			LineTo(m_hdc, fX, fY);
	}
	::EndPath(m_hdc);
	::SelectObject(m_hdc, GetBrush(pBrush));
	::FillPath(m_hdc);
}

void CPaintGdi::FillPolygon(const pointix * pPoints, int_x iCount, IBrush * pBrush)
{
	bool bFirst = true;
	int_x iX = 0, iY = 0;
	::BeginPath(m_hdc);
	for(int_x cnt = 0; cnt < iCount; ++cnt)
	{
		iX = pPoints[cnt].x;
		iY = pPoints[cnt].y;
		AffineXY(iX, iY);
		if(bFirst)
		{
			MoveToEx(m_hdc, iX, iY, NULL);
			bFirst = false;
		}
		else
			LineTo(m_hdc, iX, iY);
	}
	::EndPath(m_hdc);
	::SelectObject(m_hdc, GetBrush(pBrush));
	::FillPath(m_hdc);
}

uint_32 CPaintGdi::GetSystemColor(SysColorE eColor) const
{
	return Colors::Auto;
}

HPEN CPaintGdi::GetPen(uint_32 uiColor, float_32 fWidth)
{
	AffineColor(uiColor);
	return m_pService->GetPen(uiColor, fWidth);
}

HPEN CPaintGdi::GetPen(IPen * pPen)
{
	return GetPen(pPen->DoSample(), pPen->GetWidth());
}

HBRUSH CPaintGdi::GetBrush(uint_32 uiColor)
{
	AffineColor(uiColor);
	return m_pService->GetSolidBrush(uiColor);
}

HBRUSH CPaintGdi::GetBrush(IBrush * pBrush)
{
	return GetBrush(pBrush->DoSample());
}

HFONT CPaintGdi::GetFont(const font_t & font)
{
	return m_pService->GetFont(font);
}

void CPaintGdi::AffineColor(uint_32 & color)
{
	if(color < SysColorCount)
		color = m_colors[color];
	color = (color & 0xFF000000) |
		((color & 0x0000FF) << 16) |
		((color & 0x00FF00) << 0) |
		((color & 0xFF0000) >> 16);
	//color &= 0x00FFFFFF;
}

void CPaintGdi::AffineX(int_x & iX)
{
	if(!m_origns.valid())
		return;

	const pointix & orign = m_origns.back();
	iX += orign.x;
}

void CPaintGdi::AffineY(int_x & iY)
{
	if(!m_origns.valid())
		return;

	const pointix & orign = m_origns.back();
	iY += orign.y;
}
void CPaintGdi::AffineXY(int_x & iX, int_x & iY)
{
	if(!m_origns.valid())
		return;

	const pointix & orign = m_origns.back();
	iX += orign.x;
	iY += orign.y;
}

void CPaintGdi::AffineXY(float_32 & fX, float_32 & fY)
{
	if(m_origns.valid())
	{
		const pointix & orign = m_origns.back();
		fX += (float_32)orign.x;
		fY += (float_32)orign.y;
	}
}

void CPaintGdi::Affine(pointix & point)
{
	if(m_origns.valid())
	{
		const pointix & orign = m_origns.back();
		point += orign;
	}
}

int_x CPaintGdi::GetAffineX() const
{
	if(m_origns.valid())
		return m_origns.back().x;
	else
		return 0;
}

int_x CPaintGdi::GetAffineY() const
{
	if(m_origns.valid())
		return m_origns.back().y;
	else
		return 0;
}


HBITMAP CPaintGdi::GetImage(IImage * pImage)
{
	if(!pImage)
		return nullptr;

	GdiImageCacheT * pGdiCache = dynamic_cast<GdiImageCacheT *>(pImage->GetCache());
	if(!pGdiCache || !pGdiCache->hBitmap)
	{
		ImageBufferT buffer = pImage->GetBuffer();
		if(!buffer.buffer)
			return nullptr;
		if(buffer.mode == cmode_a8r8g8b8)
		{
			BITMAPINFO bmpInfo = {0};
			bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmpInfo.bmiHeader.biWidth = (int_32)buffer.width;
			bmpInfo.bmiHeader.biHeight = (int_32)-buffer.height;
			bmpInfo.bmiHeader.biPlanes = 1;
			bmpInfo.bmiHeader.biBitCount = 32;
			bmpInfo.bmiHeader.biCompression = BI_RGB;
			bmpInfo.bmiHeader.biSizeImage = (uint_32)(buffer.height * buffer.pitch);

			void * pBuffer = nullptr;
			HBITMAP hBitmap = ::CreateDIBSection(m_hdc, &bmpInfo, DIB_RGB_COLORS, &pBuffer, NULL, 0);
			buffcpy8(pBuffer, bmpInfo.bmiHeader.biSizeImage, buffer.buffer, buffer.length);

			if(pGdiCache)
				pGdiCache->hBitmap = hBitmap;
			else
			{
				pGdiCache = new GdiImageCacheT(hBitmap);
				pImage->SetCache(pGdiCache);
				SafeRelease(pGdiCache);
			}
			return hBitmap;
		}

		return NULL;
	}
	else
	{
		return pGdiCache->hBitmap;
	}
}

#pragma warning(pop)

VENUS_END

#endif //WIN32_DEVICE_GDI
