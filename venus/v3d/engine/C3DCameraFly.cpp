#include "stdafx.h"
#include "C3DCameraFly.h"

VENUS_BEG

C3DCameraFly::C3DCameraFly()
{
	m_vPosition = float3(0, 0, 0);
	m_vLook = float3(0, 0, 1);
	m_vRight = float3(1, 0, 0);
	m_vUp = float3(0, 1, 0);
}

C3DCameraFly::~C3DCameraFly()
{

}

float4x4 C3DCameraFly::GetMatrixV() const
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

float3 C3DCameraFly::GetPosition() const
{
	return m_vPosition;
}

float3 C3DCameraFly::GetLook() const
{
	return m_vLook;
}

float3 C3DCameraFly::GetUp() const
{
	return m_vUp;
}

float3 C3DCameraFly::GetRight() const
{
	return m_vRight;
}

void C3DCameraFly::SetPosition(const float3 & vPosition)
{
	m_vPosition = vPosition;
}

void C3DCameraFly::SetLook(const float3 & vLook)
{
	m_vLook = vLook;
	m_vLook.normalize();
}

void C3DCameraFly::SetUp(const float3 & vUp)
{
	m_vUp = vUp;
	m_vUp.normalize();
}

void C3DCameraFly::SetRight(const float3 & vRight)
{
	m_vRight = vRight;
	m_vRight.normalize();
}

void C3DCameraFly::ShiftX(float_32 fStrafe)
{
	m_vPosition += m_vRight * fStrafe;
}

void C3DCameraFly::ShiftY(float_32 fFLy)
{
	m_vPosition += m_vUp * fFLy;
}

void C3DCameraFly::ShiftZ(float_32 fWalk)
{
	m_vPosition += m_vLook * fWalk;
}

void C3DCameraFly::RollX(float_32 fPitch)
{
	float4x4 matrix;
	matrix.rotate_axis(m_vRight, fPitch);
	m_vUp *= matrix;
	m_vLook *= matrix;
}

void C3DCameraFly::RollY(float_32 fYaw)
{
	float4x4 matrix;
	matrix.rotate_axis(m_vUp, fYaw);
	m_vRight *= matrix;
	m_vLook *= matrix;
}

void C3DCameraFly::RollZ(float_32 fRoll)
{
	float4x4 matrix;
	matrix.rotate_axis(m_vLook, fRoll);
	m_vUp *= matrix;
	m_vRight *= matrix;
}

VENUS_END
