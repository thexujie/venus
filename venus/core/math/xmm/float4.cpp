#include "stdafx.h"
#include "float4.h"

VENUS_BEG

float_32 float4::length() const
{
	return xmv_x(xm_vec_length(*this));
}

float_32 float4::length_sq() const
{
	return xmv_x(xm_vec_length_sq(*this));
}

float_32 float4::dot(const float4 & vec) const
{
	return xmv_x(xm_vec_dot(*this, vec));
}

float4 float4::cross(const float4 & vec1, const float4 & vec2) const
{
	return xm_vec_cross(*this, vec1, vec2);
}

float4 & float4::negate()
{
	*this = xm_vec_negate(*this);
	return *this;
}

float4 & float4::normalize()
{
	*this = xm_vec_normalize(*this);
	return *this;
}

float4 & float4::add(const float4 & vec)
{
	*this = xm_vec_add(*this, vec);
	return *this;

}

float4 & float4::sub(const float4 & vec)
{
	*this = xm_vec_sub(*this, vec);
	return *this;
}

float4 & float4::mul(const float4 & vec)
{
	*this = xm_vec_mul(*this, vec);
	return *this;
}

float4 & float4::div(const float4 & vec)
{
	*this = xm_vec_div(*this, vec);
	return *this;
}

float4 & float4::lerp(const float4 & min, const float4 & max, float_32 lerp)
{
	*this = xm_vec_lerp(min, max, lerp);
	return *this;
}

float4 & float4::transform(const float4x4 & matrix)
{
	*this = xm_matr_transform_coord(matrix, *this);
	return *this;
}

float4 float4::operator +() const
{
	return *this;
}

float4 float4::operator -() const
{
	return xm_vec_negate(*this);
}

float4 float4::operator + (const float4 & vec) const
{
	return xm_vec_add(*this, vec);
}

float4 & float4::operator += (const float4 & vec)
{
	*this = xm_vec_add(*this, vec);
	return *this;
}

float4 float4::operator - (const float4 & vec) const
{
	return xm_vec_sub(*this, vec);
}

float4 & float4::operator -= (const float4 & vec)
{
	*this = xm_vec_sub(*this, vec);
	return *this;
}

float4 float4::operator * (const float4 & vec) const
{
	return xm_vec_mul(*this, vec);
}

float4 & float4::operator *= (const float4 & vec)
{
	*this = xm_vec_mul(*this, vec);
	return *this;
}

float4 float4::operator / (const float4 & vec) const
{
	return xm_vec_div(*this, vec);
}

float4 & float4::operator /= (const float4 & vec)
{
	*this = xm_vec_div(*this, vec);
	return *this;
}

float4 float4::operator * (float_32 rate) const
{
	return xm_vec_mul(*this, xm_vec_replicate(rate));
}

float4 & float4::operator *= (float_32 rate)
{
	*this = xm_vec_mul(*this, xm_vec_replicate(rate));
	return *this;
}

float4 float4::operator / (float_32 rate) const
{
	return xm_vec_div(*this, xm_vec_replicate(rate));
}

float4 & float4::operator /= (float_32 rate)
{
	*this = xm_vec_div(*this, xm_vec_replicate(rate));
	return *this;
}

float4 float4::operator * (const float4x4 & matrix) const
{
	return xm_matr_transform_coord(matrix, *this);
}

float4 & float4::operator *= (const float4x4 & matrix)
{
	*this = xm_matr_transform_coord(matrix, *this);
	return *this;
}

VENUS_END
