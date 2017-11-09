#pragma once

#include "DxgiInc.h"

// TODO: ·ÖÀë dxgi

VENUS_BEG

const cid_t OID_CDxgiSurface = {L"CDxgiSurface"};

class WIN32_API CDxgiSurface : public ObjectT<I2DRTarget>
{
public:
	CDxgiSurface(IDXGISurface * pSurface);
	~CDxgiSurface();
	const cid_t & GetOid() const;

	int_x GetWidth() const;
	int_x GetHeight() const;
	sizeix GetSize() const;

public:
	void ResetSurface(IDXGISurface * pSurface);
	IDXGISurface * DxgiGetSurface() { return m_pSurface; }
private:
	IDXGISurface * m_pSurface;
	sizeix m_size;
};

VENUS_END
