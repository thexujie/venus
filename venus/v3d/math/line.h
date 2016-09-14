#pragma once

VENUS_BEG

/**
* 直线方程：(x - x0)/a = (y - y0)/b = (z - z0)/c
* 向量表示：p = p0 + v*t，其中 p0 = {x0, y0, z0, 0}, v = {a, b, c, 1}
*/

class V3D_API line_t
{
public:
	line_t() {}
	line_t(const float3 & _pos, const float3 & _dir) :
		pos(_pos), dir(_dir)
	{
	}

public:
	float3 pos;
	float3 dir;
};

class V3D_API segment_t : public line_t
{
public:
	segment_t() {}
	segment_t(const float3 & _pos, const float3 & _dir) :
		line_t(_pos, _dir)
	{
	}

	float3 middle() const
	{
		return pos + dir * 0.5f;
	}

	float_32 length() const
	{
		return dir.length();
	}
};

VENUS_END
