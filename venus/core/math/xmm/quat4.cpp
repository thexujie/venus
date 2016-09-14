#include "stdafx.h"
#include "quat4.h"

VENUS_BEG

float_32 quat4::length() const
{
	return xmv_x(xm_vec_length(*this));
}

float_32 quat4::length_sq() const
{
	return xmv_x(xm_vec_length_sq(*this));
}

float_32 quat4::dot(const quat4 & vec) const
{
	return xmv_x(xm_vec_dot(*this, vec));
}

quat4 quat4::cross(const quat4 & vec1, const quat4 & vec2) const
{
	return xm_vec_cross(*this, vec1, vec2);
}

quat4 & quat4::set(float_32 _x, float_32 _y, float_32 _z, float_32 _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
	return *this;
}

quat4 & quat4::negate()
{
	*this = xm_vec_negate(*this);
	return *this;
}

quat4 & quat4::normalize()
{
	*this = xm_vec_normalize(*this);
	return *this;
}

quat4 & quat4::add(const quat4 & vec)
{
	*this = xm_vec_add(*this, vec);
	return *this;

}

quat4 & quat4::sub(const quat4 & vec)
{
	*this = xm_vec_sub(*this, vec);
	return *this;
}

quat4 & quat4::mul(const quat4 & vec)
{
	*this = xm_vec_mul(*this, vec);
	return *this;
}

quat4 & quat4::div(const quat4 & vec)
{
	*this = xm_vec_div(*this, vec);
	return *this;
}

quat4 & quat4::lerp(const quat4 & min, const quat4 & max, float_32 lerp)
{
	*this = xm_quat_lerp(min, max, lerp);
	return *this;
}

quat4 quat4::operator +() const
{
	return *this;
}

quat4 quat4::operator -() const
{
	return xm_vec_negate(*this);
}

quat4 quat4::operator + (const quat4 & vec) const
{
	return xm_vec_add(*this, vec);
}

quat4 & quat4::operator += (const quat4 & vec)
{
	*this = xm_vec_add(*this, vec);
	return *this;
}

quat4 quat4::operator - (const quat4 & vec) const
{
	return xm_vec_sub(*this, vec);
}

quat4 & quat4::operator -= (const quat4 & vec)
{
	*this = xm_vec_sub(*this, vec);
	return *this;
}

quat4 quat4::operator * (const quat4 & vec) const
{
	return xm_vec_mul(*this, vec);
}

quat4 & quat4::operator *= (const quat4 & vec)
{
	*this = xm_vec_mul(*this, vec);
	return *this;
}

quat4 quat4::operator / (const quat4 & vec) const
{
	return xm_vec_div(*this, vec);
}

quat4 & quat4::operator /= (const quat4 & vec)
{
	*this = xm_vec_div(*this, vec);
	return *this;
}

quat4 quat4::operator * (float_32 rate) const
{
	return xm_vec_mul(*this, xm_vec_replicate(rate));
}

quat4 & quat4::operator *= (float_32 rate)
{
	*this = xm_vec_mul(*this, xm_vec_replicate(rate));
	return *this;
}

quat4 quat4::operator / (float_32 rate) const
{
	return xm_vec_div(*this, xm_vec_replicate(rate));
}

quat4 & quat4::operator /= (float_32 rate)
{
	*this = xm_vec_div(*this, xm_vec_replicate(rate));
	return *this;
}

quat4 quat4::operator * (const float4x4 & matrix) const
{
	return xm_matr_transform_coord(matrix, *this);
}

quat4 & quat4::operator *= (const float4x4 & matrix)
{
	*this = xm_matr_transform_coord(matrix, *this);
	return *this;
}

VENUS_END
