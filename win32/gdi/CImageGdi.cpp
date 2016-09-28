#include "stdafx.h"
#include "CImageGdi.h"

#ifdef WIN32_DEVICE_GDI

VENUS_BEG

CImageGdi::CImageGdi():
m_hBitmap(NULL)
{
}

CImageGdi::~CImageGdi()
{
	verify(!m_iRef);
	m_size.set(0, 0);
	::DeleteObject(m_hBitmap);
}

const oid_t & CImageGdi::GetOid() const
{
	return CID_CImageGdi;
}

void CImageGdi::Load(const char_16 * szImage, int_x iLength)
{
#ifdef _DEBUG
	textcpy(m_szPath, 256, szImage, iLength);
#endif 

	if(iLength < 0)
		iLength = textlen(szImage);
	if(iLength * 2 == sizeof(win32_image_t))
	{
		const win32_image_t * pwi = (const win32_image_t *)szImage;
		if(pwi->tag == WIN32_IMAGE_TAG_ICON)
			FromIcon(pwi->hicon);
	}
	else
	{
	}

}

void CImageGdi::FromIcon(HICON hIcon)
{
	if(!hIcon)
		throw exp_illegal_argument();

	ICONINFO ic;
	::GetIconInfo(hIcon, &ic);
	image_data_t img;
	bitmap_create(ic.hbmColor, &img);
	FromImageData(img);
	image_free(img.buffer);
	::DeleteObject(ic.hbmColor);
	::DeleteObject(ic.hbmMask);
}

void CImageGdi::FromHBITMAP(HBITMAP hBitmap)
{
	m_hBitmap = hBitmap;
}

void CImageGdi::FromImageData(image_data_t & da)
{
}

VENUS_END

#endif //WIN32_DEVICE_GDI
