#include "stdafx.h"

VENUS_BEG

bbox_t::bbox_t()
{
}

bbox_t::bbox_t(const bbox_t & another)
{
	buffcpy(*this, another);
}

bbox_t::bbox_t(const float3 & vCenter, const float3 & vExtent)
{
	float3 vExtentHalf = vExtent / 2.0f;
	min = vCenter - vExtentHalf;
	max = vCenter + vExtentHalf;
}

bool bbox_t::is_empty() const
{
	return f32_equal(min.x, max.x) ||
		f32_equal(min.y, max.y) ||
		f32_equal(min.z, max.z);
}

void bbox_t::reset(const float3 & point)
{
	min = point;
	max = point;
}

float3 bbox_t::extent() const
{
	return max - min;
}

float3 bbox_t::center() const
{
	return (max + min) * 0.5f;
}

bbox_t & bbox_t::from_center_extend(const float3 & center, const float3 & extent)
{
	float3 half_extent = extent / 2.0f;
	min = center - half_extent;
	max = center + half_extent;
	return *this;
}

bbox_t & bbox_t::from_min_max(const float3 & _min, const float3 & _max)
{
	min = _min;
	max = _max;
	return *this;
}

void bbox_t::add_point(const float3 & point)
{
	add_point(point.x, point.y, point.z);
}

void bbox_t::add_point(float_32 x, float_32 y, float_32 z)
{
	if(x > max.x) max.x = x;
	else if(x < min.x) min.x = x;
	else {}

	if(y > max.y) max.y = y;
	else if(y < min.y) min.y = y;
	else {}

	if(z > max.z) max.z = z;
	else if(z < min.z) min.z = z;
	else {}
}

bool bbox_t::is_full_in(const bbox_t & another) const
{
	return (min.x >= another.min.x && min.y >= another.min.y && min.z >= another.min.z &&
			max.x <= another.max.x && max.y <= another.max.y && max.z <= another.max.z);
}

bool bbox_t::intersect_with(const bbox_t & another) const
{
	return (min.x <= another.max.x && min.y <= another.max.y && min.z <= another.max.z &&
			max.x >= another.min.x && max.y >= another.min.y && max.z >= another.min.z);
}

bool bbox_t::intersect_with(const segment_t & segment) const
{
	float3 middle = segment.middle();
	float3 dir = segment.dir;
	float_32 half_length = segment.length() * 0.5f;

	float3 e = extent() * 0.5f;
	float3 t = center() - middle;

	if((float_abs(t.x) > e.x + half_length * float_abs(dir.x)) ||
	   (float_abs(t.y) > e.y + half_length * float_abs(dir.y)) ||
	   (float_abs(t.z) > e.z + half_length * float_abs(dir.z)))
	   return false;

	float_32 r = e.y * float_abs(dir.z) + e.z * float_abs(dir.y);
	if(float_abs(t.y * dir.z - t.z * dir.y) > r)
		return false;

	r = e.x * float_abs(dir.z) + e.z * float_abs(dir.x);
	if(float_abs(t.z * dir.x - t.x * dir.z) > r)
		return false;

	r = e.x * float_abs(dir.y) + e.y * float_abs(dir.x);
	if(float_abs(t.x * dir.y - t.y * dir.x) > r)
		return false;

	return true;
}

float3 bbox_t::flt() const
{
	return float3(min.x, max.y, max.z);
}

float3 bbox_t::flb() const
{
	return float3(min.x, min.y, max.z);
}

float3 bbox_t::frt() const
{
	return float3(max.x, max.y, max.z);
}

float3 bbox_t::frb() const
{
	return float3(max.x, min.y, max.z);
}

float3 bbox_t::nlt() const
{
	return float3(min.x, max.y, min.z);
}

float3 bbox_t::nlb() const
{
	return float3(min.x, min.y, min.z);
}

float3 bbox_t::nrt() const
{
	return float3(max.x, max.y, min.z);
}

float3 bbox_t::nrb() const
{
	return float3(min.x, max.y, min.z);
}

VENUS_END
