#pragma once

VENUS_BEG

/**
* 平面方程：ax + by + cz + d = 0;
*/
class V3D_API plane_t : public float4
{
public:
	plane_t() : float4(0, 1.0f, 0, 0) {}
	plane_t(const float3 & _nor, const float_32 & _dis) :float4(_nor, _dis) {}
	plane_t(const float3 & pos0,
			const float3 & pos1,
			const float3 & pos2)
	{
		xyz = (pos1 - pos0).cross(pos2 - pos1);
		xyz.normalize();
		w = -(xyz.dot(pos0));
	}

	plane_t(const float3 & _pos,
			const float3 & _nor)
	{
		xyz = _nor;
		w = -(_nor.dot(_pos));
	}

	//! 求与指定直线的交点
	bool intersect_with(const line_t & line, float3 & pos_ins) const
	{
		float_32 t2 = xyz.dot(line.dir);
		if(float_is_zero(t2))
			return false;

		float_32 t = -(xyz.dot(line.pos) + w) / t2;
		pos_ins = line.pos + (line.dir * t);
		return true;
	}

	//! 求与指定平面的交线
	bool intersect_with(const plane_t & plane, line_t & line_ins) const
	{
		float_32 fn00 = xyz.length();
		float_32 fn01 = xyz.dot(plane.xyz);
		float_32 fn11 = plane.xyz.length();
		float_32 det = fn00 * fn11 - fn01 * fn01;

		if(float_is_zero(det))
			return false;

		float_32 invdet = float_reciprocal(det);
		float_32 fc0 = (fn11 * -w + fn01 * plane.w) * invdet;
		float_32 fc1 = (fn00 * -plane.w + fn01 * w) * invdet;

		line_ins.dir = xyz.cross(plane.xyz);
		line_ins.pos = xyz * fc0 + plane.xyz * fc1;
		return true;
	}

	//! 求与指定两个平面的的交点
	bool intersect_with(const plane_t & plane1,
						const plane_t & plane2,
						float3 & pos_ins) const
	{
		line_t line;
		if(intersect_with(plane1, line) && plane2.intersect_with(line, pos_ins))
			return true;
		else
			return false;
	}
};

VENUS_END
