#pragma once

#include "GdiInc.h"

#ifdef WIN32_DEVICE_GDI

VENUS_BEG

const oid_t CID_CImageGdi = {L"CImageGdi"};

class WIN32_API CImageGdi : public ObjectT<IImage>
{
public:
	CImageGdi();
	~CImageGdi();

	const oid_t & GetOid() const;

	int_x GetWidth() const { return m_size.w; }
	int_x GetHeight() const { return m_size.h; }
	sizeix GetSize() const { return m_size; }

	void Load(const char_16 * szImage, int_x iLength = -1);
	void FromIcon(HICON hIcon);
	void FromHBITMAP(HBITMAP hBitmap);
	void FromImageData(image_data_t & da);

	HBITMAP GetImage() const { return m_hBitmap; }
private:
	HBITMAP m_hBitmap;
	sizeix m_size;
public:
#ifdef _DEBUG
	char_16 m_szPath[256];
#endif
};

VENUS_END

#endif //WIN32_DEVICE_GDI
