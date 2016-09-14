#pragma once

VENUS_BEG

class ray_t
{
public:
	ray_t() {}
	ray_t(const float3 & _pos, const float3 & _dir) : pos(_pos), dir(_dir) {}
	ray_t(const ray_t & another) = default;
	ray_t & operator = (const ray_t & another) = default;

	float3 pos;
	float3 dir;
};

VENUS_END
