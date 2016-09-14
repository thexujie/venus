#pragma once

VENUS_BEG

const oid_t OID_CBmpBuffer = {L"CBmpBuffer"};

class WIN32_API CBmpBuffer : public ObjectT<I2DRTarget>
{
public:
	CBmpBuffer();
	~CBmpBuffer();

	const oid_t & GetOid() const;

public: // IBuffer
	byte_t * GetBuffer() const;
	int_x GetStrike() const;
	int_x GetWidth() const;
	int_x GetHeight() const;
	sizeix GetSize() const;
	int_x GetPitch() const;

	void ReserveSize(int_x iWidth, int_x iHeight);
	void SetSize(int_x iWidth, int_x iHeight);
	void ClearWithColor(uint_32 uiColor);
	void ClearWithColor(const rectix & rcClear, uint_32 uiColor);

	void ClearWithByte(byte_t bValue);
public:
	HDC GetBufferDC() const;
	HBITMAP GetHBitmap() const;
	bool NeedResize(int_x iWidth, int_x iHeight) const;
	void BitBltTo(HDC hdc, int_x iX, int_x iY, int_x iWidth, int_x iHeight);

private:
	void VerifyData() const;
public:
	HDC m_hdc;
	HBITMAP m_hBmp;
	byte_t * m_pBuffer;
	int_x m_iStrike;
	int_x m_iWidth;
	int_x m_iPitch;
	int_x m_iHeight;
};

VENUS_END
