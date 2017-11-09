#include "stdafx.h"
#include "CBmpBuffer.h"

VENUS_BEG

CBmpBuffer::CBmpBuffer():
m_hdc(NULL), m_hBmp(NULL), m_pBuffer(nullptr), m_iStrike(0),
	m_iWidth(0), m_iPitch(0), m_iHeight(0)
{
}

CBmpBuffer::~CBmpBuffer()
{
	::DeleteDC(m_hdc);
	m_hdc = NULL;
}

const cid_t & CBmpBuffer::GetOid() const
{
	return OID_CBmpBuffer;
}

bool CBmpBuffer::NeedResize(int_x iWidth, int_x iHeight) const
{
	if(iWidth <= m_iWidth && iHeight <= m_iHeight)
		return false;
	else
		return true;
}

void CBmpBuffer::BitBltTo(HDC hdc, int_x iX, int_x iY, int_x iWidth, int_x iHeight)
{
	if(m_hdc)
		::BitBlt(hdc, (int_32)iX, (int_32)iY, (int_32)iWidth, (int_32)iHeight, m_hdc, (int_32)iX, (int_32)iY, SRCCOPY);
}

byte_t * CBmpBuffer::GetBuffer() const
{
	return m_pBuffer;
}

int_x CBmpBuffer::GetStrike() const
{
	return m_iStrike;
}

int_x CBmpBuffer::GetWidth() const
{
	return m_iWidth;
}

int_x CBmpBuffer::GetPitch() const
{
	return m_iPitch;
}

int_x CBmpBuffer::GetHeight() const
{
	return m_iHeight;
}

void CBmpBuffer::ReserveSize(int_x iWidth, int_x iHeight)
{
	if(iWidth > GetWidth() || iHeight > GetHeight())
		SetSize(iWidth, iHeight);
}

sizeix CBmpBuffer::GetSize() const
{
	return sizeix(m_iWidth, m_iHeight);
}

void CBmpBuffer::SetSize(int_x iWidth, int_x iHeight)
{
	if(!m_hdc)
	{
		HDC hdcScreen = ::GetDC(NULL);
		m_hdc = ::CreateCompatibleDC(hdcScreen);
		::ReleaseDC(NULL, hdcScreen);
	}

	HBITMAP hOldBmp = m_hBmp;
	int_x iOldWidth = m_iWidth;
	int_x iOldHeight = m_iHeight;
	int_x iOldPitch = m_iPitch;
	byte_t * pOldBuffer = m_pBuffer;

	int_x iStrike = 4;
	int_x iPitch = iWidth * iStrike;
	if(iPitch & 0x4)
		iPitch += 0x4 - (iPitch & 0x4);

	BITMAPINFO bmpInfo = {0};
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = (int_32)iWidth;
	bmpInfo.bmiHeader.biHeight = (int_32)-iHeight;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = (uint_16)(iStrike << 3);
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biSizeImage = (uint_32)(iHeight * iPitch);

	m_hBmp = ::CreateDIBSection(m_hdc, &bmpInfo, DIB_RGB_COLORS, (void **)&m_pBuffer, NULL, 0);
	if(!m_hBmp)
		throw exp_base();

	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iStrike = iStrike;
	m_iPitch = iPitch;

	::SelectObject(m_hdc, m_hBmp);

	if(hOldBmp)
	{
		int_x iWidth = minof(m_iWidth, iOldWidth);
		int_x iHeight = minof(m_iHeight, iOldHeight);
		int_x iPitch = iWidth * m_iStrike;
		byte_t * pBuffer = m_pBuffer;

		for(int_x cnt = 0; cnt < iHeight; ++cnt)
		{
			buffcpy8(pBuffer, m_iPitch, pOldBuffer, iPitch);
			pBuffer += m_iPitch;
			pOldBuffer += iOldPitch;
		}
		::DeleteObject(hOldBmp);
	}
}

void CBmpBuffer::ClearWithColor(uint_32 uiColor)
{
	byte_t * pPixel = nullptr;
	for(int_x row = 0, col = 0; row != m_iHeight; ++row)
	{
		pPixel = m_pBuffer + row * m_iPitch;
		buffset32(pPixel, m_iWidth, uiColor);
	}
}

void CBmpBuffer::ClearWithColor(const rectix & rcClear, uint_32 uiColor)
{
	if(m_pBuffer)
	{
		byte_t * pBuffer = m_pBuffer + (rcClear.y * m_iPitch + (rcClear.x << 2));
		byte_t * pPixel = nullptr;
		for(int_x row = 0, col = 0; row != rcClear.h; ++row)
		{
			pPixel = pBuffer + row * m_iPitch;
			buffset32(pPixel, rcClear.w, uiColor);
		}
	}
}

void CBmpBuffer::ClearWithByte(byte_t bValue)
{
	for(int_x cnt = 0; cnt < m_iHeight; ++cnt)
		buffset8(m_pBuffer, m_iPitch, bValue);
}

HDC CBmpBuffer::GetBufferDC() const
{
	return m_hdc;
}
HBITMAP CBmpBuffer::GetHBitmap() const
{
	return m_hBmp;
}

void CBmpBuffer::VerifyData() const
{
	if(!m_hBmp)
		throw exp_bad_state();
}

VENUS_END
