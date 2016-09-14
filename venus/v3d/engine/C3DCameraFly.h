#pragma once

#include "C3DCamera.h"

VENUS_BEG

class V3D_API C3DCameraFly : public C3DCamera
{
public:
	C3DCameraFly();
	~C3DCameraFly();

	float4x4 GetMatrixV() const;

	float3 GetPosition() const;
	float3 GetLook() const;
	float3 GetUp() const;
	float3 GetRight() const;

	void SetPosition(const float3 & vPosition);
	void SetLook(const float3 & vLook);
	void SetUp(const float3 & vUp);
	void SetRight(const float3 & vRight);

	void ShiftX(float_32 fStrafe);
	void ShiftY(float_32 fFLy);
	void ShiftZ(float_32 fWalk);

	void RollX(float_32 fPitch);
	void RollY(float_32 fYaw);
	void RollZ(float_32 fRoll);
private:
	float3 m_vPosition;
	float3 m_vLook;
	float3 m_vUp;
	float3 m_vRight;
};

VENUS_END
