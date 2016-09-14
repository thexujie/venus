#include "stdafx.h"
#include "float3.h"

VENUS_BEG

float_32 float3::length() const
{
	return xmv_x(xm_vec3_length(*this));
}

float_32 float3::length_sq() const
{
	return xmv_x(xm_vec3_length_sq(*this));
}

float_32 float3::dot(const float3 & vec) const
{
	return xmv_x(xm_vec3_dot(*this, vec));
}

float3 float3::cross(const float3 & vec) const
{
	return xm_vec3_cross(*this, vec);
}

float3 float3::abs() const
{
	return xm_vec_abs(*this);
}

float3 & float3::negate()
{
	*this = xm_vec_negate(*this);
	return *this;
}

float3 & float3::normalize()
{
	*this = xm_vec3_normalize(*this);
	return *this;
}
float3 & float3::add(const float3 & vec)
{
	*this = xm_vec_add(*this, vec);
	return *this;
}

float3 & float3::sub(const float3 & vec)
{
	*this = xm_vec_sub(*this, vec);
	return *this;
}

float3 & float3::mul(const float3 & vec)
{
	*this = xm_vec_mul(*this, vec);
	return *this;
}

float3 & float3::div(const float3 & vec)
{
	*this = xm_vec_div(*this, vec);
	return *this;
}

float3 float3::operator +() const
{
	return *this;
}

float3 float3::operator -() const
{
	return xm_vec_negate(*this);
}

float3 float3::operator + (const float3 & vec) const
{
	return xm_vec_add(*this, vec);
}

float3 & float3::operator += (const float3 & vec)
{
	*this = xm_vec_add(*this, vec);
	return *this;
}

float3 float3::operator - (const float3 & vec) const
{
	return xm_vec_sub(*this, vec);
}

float3 & float3::operator -= (const float3 & vec)
{
	*this = xm_vec_sub(*this, vec);
	return *this;
}

float3 float3::operator * (float_32 rate) const
{
	return xm_vec_mul(*this, float3(rate, rate, rate));
}

float3 float3::operator * (const float3 & vec) const
{
	return xm_vec_mul(*this, vec);
}

float3 & float3::operator *= (float_32 rate)
{
	*this = xm_vec_mul(*this, float3(rate, rate, rate));
	return *this;
}

float3 & float3::operator *= (const float3 & vec)
{
	*this = xm_vec_mul(*this, vec);
	return *this;
}

float3 & float3::lerp(const float3 & min, const float3 & max, float_32 lerp)
{
	*this = xm_vec_lerp(min, max, lerp);
	return *this;
}

float3 & float3::transform(const float4x4 & matrix)
{
	*this = xm_matr_transform_coord(matrix, *this);
	return *this;
}

float3 float3::operator * (const float4x4 & matrix) const
{
	return xm_matr_transform_coord(matrix, *this);
}

float3 float3::operator *= (const float4x4 & matrix)
{
	*this = xm_matr_transform_coord(matrix, *this);
	return *this;
}

float3 float3::operator / (float_32 rate) const
{
	return xm_vec_div(*this, xm_vec_replicate(rate));
}

float3 float3::operator / (const float3 & vec) const
{
	return xm_vec_div(*this, vec);
}

float3 & float3::operator /= (float_32 rate)
{
	*this = xm_vec_div(*this, xm_vec_replicate(rate));
	return *this;
}
float3 & float3::operator /= (const float3 & vec)
{
	*this = xm_vec_div(*this, vec);
	return *this;
}

VENUS_END
