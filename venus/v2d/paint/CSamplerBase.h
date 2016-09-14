#pragma once

VENUS_BEG

class V2D_API CSolidSampler : public ISampler
{
public:
	CSolidSampler();
	CSolidSampler(uint_32 uiColor);
	~CSolidSampler();

	SamplerTyleE GetSampleType() const;
	uint_32 DoSample();
	uint_32 DoSample(int_x iX, int_x iY);

	uint_32 Color;
};

class V2D_API CSolidBrush : public IBrush
{
public:
	CSolidBrush(uint_32 uiColor);
	~CSolidBrush();
	SamplerTyleE GetSampleType() const;
	uint_32 DoSample();
	uint_32 DoSample(int_x iX, int_x iY);
public:
	void SetColor(uint_32 uiColor);
protected:
	uint_32 m_uiColor;
};

class V2D_API CPen : public IPen
{
public:
	CPen(uint_32 uiColor, float_32 fWidth);
	~CPen();
	SamplerTyleE GetSampleType() const;
	uint_32 DoSample();
	uint_32 DoSample(int_x iX, int_x iY);
public:
	void SetColor(uint_32 uiColor);
	uint_32 GetColor() const;
	float_32 GetWidth() const;
protected:
	uint_32 m_uiColor;
	float_32 m_fWidth;
};

class V2D_API CLinerGradientBrush : public ILinerGradientBrush
{
public:
	CLinerGradientBrush();
	CLinerGradientBrush(uint_32 uiColorA, uint_32 uiColorB, const pointix & pointA, const pointix & pointB);
	SamplerTyleE GetSampleType() const;
	uint_32 DoSample();
	uint_32 DoSample(int_x iX, int_x iY);

	BrushWrapModeE GetWrapMode() const;
	uint_32 GetColorA() const;
	uint_32 GetColorB() const;
	pointix GetPointA() const;
	pointix GetPointB() const;

	void SetColorA(uint_32 uiColor);
	void SetColorB(uint_32 uiColor);
	void SetPointA(const pointix & point);
	void SetPointB(const pointix & point);

protected:
	uint_32 m_uiColorA;
	uint_32 m_uiColorB;
	pointix m_pointA;
	pointix m_pointB;
};

VENUS_END
