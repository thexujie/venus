#include "stdafx.h"
#include "C3DCameraFP.h"

VENUS_BEG

C3DCameraFP::C3DCameraFP()
{
	m_vPosition = float3(0, 0, 0);
	m_vLook = float3(0, 0, 1);
	m_vUp = float3(0, 1, 0);
}

C3DCameraFP::~C3DCameraFP()
{

}

float4x4 C3DCameraFP::GetMatrixV() const
{
	float3 vLook = m_vLook;
	float3 vUp = m_vUp;
	float3 vRight = vUp.cross(vLook);

	vRight.normalize();
	vUp = vLook.cross(vRight);
	vUp.normalize();

	float_32 x = m_vPosition.dot(vRight);
	float_32 y = m_vPosition.dot(vUp);
	float_32 z = m_vPosition.dot(vLook);

	return float4x4(
		vRight.x, vUp.x, vLook.x, 0,
		vRight.y, vUp.y, vLook.y, 0,
		vRight.z, vUp.z, vLook.z, 0,
		-x, -y, -z, 1
		);
}

float3 C3DCameraFP::GetPosition() const
{
	return m_vPosition;
}

float3 C3DCameraFP::GetLook() const
{
	return m_vLook;
}

float3 C3DCameraFP::GetUp() const
{
	return m_vUp;
}

float3 C3DCameraFP::GetRight() const
{
	float3 vRight = m_vUp.cross(m_vLook);
	return vRight;
}

void C3DCameraFP::SetPosition(const float3 & vPosition)
{
	m_vPosition = vPosition;
}

void C3DCameraFP::SetLook(const float3 & vLook)
{
	m_vLook = vLook;
	m_vLook.normalize();
}

void C3DCameraFP::SetUp(const float3 & vUp)
{
	m_vUp = vUp;
	m_vUp.normalize();
}

void C3DCameraFP::ShiftX(float_32 fStrafe)
{
	float3 vRight = m_vUp.cross(m_vLook);
	m_vPosition += vRight * fStrafe;
}

void C3DCameraFP::ShiftY(float_32 fFLy)
{
	m_vPosition += m_vUp * fFLy;
}

void C3DCameraFP::ShiftZ(float_32 fWalk)
{
	m_vPosition += m_vLook * fWalk;
}

void C3DCameraFP::RollX(float_32 fPitch)
{
	float3 vRight = m_vUp.cross(m_vLook);
	float4x4 matrix;
	matrix.rotate_axis(vRight, fPitch);
	m_vLook *= matrix;
}

void C3DCameraFP::RollY(float_32 fYaw)
{
	float4x4 matrix;
	matrix.rotate_axis(m_vUp, fYaw);
	m_vLook *= matrix;
}

void C3DCameraFP::RollZ(float_32 fRoll)
{
	float4x4 matrix;
	matrix.rotate_axis(m_vLook, fRoll);
	m_vUp *= matrix;
}

VENUS_END
