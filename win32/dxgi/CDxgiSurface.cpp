#include "stdafx.h"
#include "CDxgiSurface.h"

VENUS_BEG

CDxgiSurface::CDxgiSurface(IDXGISurface * pSurface)
: m_pSurface(pSurface)
{
	SafeAddRef(m_pSurface);
	DXGI_SURFACE_DESC desc = {};
	m_pSurface->GetDesc(&desc);
	m_size.set(desc.Width, desc.Height);
}

CDxgiSurface::~CDxgiSurface()
{
	SafeRelease(m_pSurface);
}

const oid_t & CDxgiSurface::GetOid() const
{
	return OID_CDxgiSurface;
}

int_x CDxgiSurface::GetWidth() const
{
	DXGI_SURFACE_DESC desc = {};
	m_pSurface->GetDesc(&desc);
	return (int_x)desc.Height;
}

int_x CDxgiSurface::GetHeight() const
{
	DXGI_SURFACE_DESC desc = {};
	m_pSurface->GetDesc(&desc);
	return (int_x)desc.Width;
}

sizeix CDxgiSurface::GetSize() const
{
	DXGI_SURFACE_DESC desc = {};
	m_pSurface->GetDesc(&desc);
	return sizeix((int_x)desc.Width, (int_x)desc.Height);
}

void CDxgiSurface::ResetSurface(IDXGISurface * pSurface)
{
	SafeRelease(m_pSurface);
	m_pSurface = pSurface;
	SafeAddRef(m_pSurface);
}

VENUS_END
