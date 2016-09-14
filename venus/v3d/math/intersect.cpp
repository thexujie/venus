#include "stdafx.h"
#include "intersect.h"

VENUS_BEG

bool intersect_tri(const float3 & ray_pos, const float3 & ray_dir,
						  const float3 & pos0, const float3 & pos1, const float3 & pos2,
						  float_32 & res_u, float_32 & res_v, float_32 & res_d,
						  float_32 tolerance)
{
	float3 edge1 = pos1 - pos0;
	float3 edge2 = pos2 - pos0;

	float3 vertical = ray_dir.cross(edge2);
	float_32 det = edge1.dot(vertical);
	float3 vec0;
	if(det > 0.0f)
	{
		vec0 = ray_pos - pos0;
	}
	else
	{
		vec0 = pos0 - ray_pos;
		det = -det;
	}

	// 射线与面重合或垂直
	if(det < tolerance)
		return false;

	float_32 u = vec0.dot(vertical);
	if(u < 0.0f || u > det)
		return false;

	float3 qvec = vec0.cross(edge1);
	float_32 v = ray_dir.dot(qvec);
	if(v < 0.0f || u + v > det)
		return false;

	float_32 d = edge2.dot(qvec);
	float_32 rec = 1.0f / det;
	res_u = u / det;
	res_v = v / det;
	res_d = d / det;
	return true;
}

VENUS_END
