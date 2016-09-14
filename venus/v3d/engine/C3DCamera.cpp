#include "stdafx.h"
#include "C3DCamera.h"

VENUS_BEG

C3DCamera::C3DCamera():
m_fVisual(F32_MAX)
{

}

C3DCamera::~C3DCamera()
{

}

void C3DCamera::SetViewPort(rectix viewport)
{
	m_viewport = viewport;
}

rectix C3DCamera::GetViewPort() const
{
	return m_viewport;
}

void C3DCamera::SetMatrixP(const float4x4 & matProj)
{
	m_matProj = matProj;
}

float4x4 C3DCamera::GetMatrixP() const
{
	return m_matProj;
}

float4x4 C3DCamera::GetMatrixVP() const
{
	return GetMatrixV() * GetMatrixP();
}

float_32 C3DCamera::GetVisual() const
{
	return m_fVisual;
}

void C3DCamera::SetVisual(float_32 fVisualDistance)
{
	m_fVisual = fVisualDistance;
}
VENUS_END
