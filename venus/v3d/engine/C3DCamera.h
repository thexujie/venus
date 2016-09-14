#pragma once

VENUS_BEG

class V3D_API C3DCamera : public ObjectT<I3DCamera>
{
public:
	C3DCamera();
	~C3DCamera();

	void SetViewPort(rectix viewport);
	rectix GetViewPort() const;
	void SetMatrixP(const float4x4 & matProj);
	float4x4 GetMatrixP() const;
	float4x4 GetMatrixVP() const;

	float_32 GetVisual() const;
	void SetVisual(float_32 fVisualDistance);
protected:
	rectix m_viewport;
	float_32 m_fVisual;
	float4x4 m_matProj;
};

VENUS_END
