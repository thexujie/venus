#include "stdafx.h"
#include "CSamplerBase.h"

VENUS_BEG

CSolidSampler::CSolidSampler()
	: Color(0xFFFFFFFF)
{

}
CSolidSampler::CSolidSampler(uint_32 uiColor)
	: Color(uiColor)
{

}
CSolidSampler::~CSolidSampler()
{

}

SamplerTyleE CSolidSampler::GetSampleType() const
{
	return SamplerTyleSolid;
}
uint_32 CSolidSampler::DoSample()
{
	return Color;
}
uint_32 CSolidSampler::DoSample(int_x iX, int_x iY)
{
	return Color;
}


CSolidBrush::CSolidBrush(uint_32 uiColor)
	: m_uiColor(uiColor)
{

}
CSolidBrush::~CSolidBrush()
{

}
SamplerTyleE CSolidBrush::GetSampleType() const
{
	return SamplerTyleSolid;
}
uint_32 CSolidBrush::DoSample()
{
	return m_uiColor;
}
uint_32 CSolidBrush::DoSample(int_x iX, int_x iY)
{
	return m_uiColor;
}
void CSolidBrush::SetColor(uint_32 uiColor)
{
	m_uiColor = uiColor;
}

CPen::CPen(uint_32 uiColor, float_32 fWidth)
	: m_uiColor(uiColor), m_fWidth(fWidth) {}
CPen::~CPen() {}

SamplerTyleE CPen::GetSampleType() const
{
	return SamplerTyleSolid;
}
uint_32 CPen::DoSample()
{
	return m_uiColor;
}
uint_32 CPen::DoSample(int_x iX, int_x iY)
{
	return m_uiColor;
}
void CPen::SetColor(uint_32 uiColor)
{
	m_uiColor = uiColor;
}

uint_32 CPen::GetColor() const
{
	return m_uiColor;
}

float_32 CPen::GetWidth() const
{
	return m_fWidth;
}

CLinerGradientBrush::CLinerGradientBrush(uint_32 uiColorA, uint_32 uiColorB, const pointix & pointA, const pointix & pointB)
	: m_uiColorA(uiColorA), m_uiColorB(uiColorB), m_pointA(pointA), m_pointB(pointB)
{

}

CLinerGradientBrush::CLinerGradientBrush()
	: m_uiColorA(0xFFFFFFFF), m_uiColorB(0xFFFFFFFF)
{

}

SamplerTyleE CLinerGradientBrush::GetSampleType() const
{
	return SamplerTyleLinerGradient;
}
uint_32 CLinerGradientBrush::DoSample()
{
	return m_uiColorA;
}
uint_32 CLinerGradientBrush::DoSample(int_x iX, int_x iY)
{
	return m_uiColorA;
}
BrushWrapModeE CLinerGradientBrush::GetWrapMode() const
{
	return BrushWrapModeTileFlipXY;
}
uint_32 CLinerGradientBrush::GetColorA() const
{
	return m_uiColorA;
}
uint_32 CLinerGradientBrush::GetColorB() const
{
	return m_uiColorB;
}
pointix CLinerGradientBrush::GetPointA() const
{
	return m_pointA;
}
pointix CLinerGradientBrush::GetPointB() const
{
	return m_pointB;
}
void CLinerGradientBrush::SetColorA(uint_32 uiColor) { m_uiColorA = uiColor;} 
void CLinerGradientBrush::SetColorB(uint_32 uiColor) { m_uiColorB = uiColor;} 
void CLinerGradientBrush::SetPointA(const pointix & point) { m_pointA = point;} 
void CLinerGradientBrush::SetPointB(const pointix & point) { m_pointB = point;} 

VENUS_END
