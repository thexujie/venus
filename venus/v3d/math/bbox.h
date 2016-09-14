#pragma once

VENUS_BEG

class V3D_API bbox_t
{
public:
	bbox_t();
	bbox_t(const bbox_t & another);
	bbox_t(const float3 & center, const float3 & vExtent);
	bool is_empty() const;
	void reset(const float3 & point);
	float3 extent() const;
	float3 center() const;

	bbox_t & from_center_extend(const float3 & center, const float3 & vExtent);
	bbox_t & from_min_max(const float3 & _min, const float3 & _max);

	void add_point(const float3 & point);
	void add_point(float_32 x, float_32 y, float_32 z);

	//! �Ƿ���ȫ�� another ��Χ
	bool is_full_in(const bbox_t & another) const;
	//�� �ж�������Χ���Ƿ��ཻ
	bool intersect_with(const bbox_t & another) const;
	//! �ж��Ƿ��ָ��ֱ���ཻ
	bool intersect_with(const segment_t & segment) const;

	float3 flt() const; //! far left top
	float3 flb() const; //! far left bottom
	float3 frt() const; //! far right top
	float3 frb() const; //! far right bottom
	float3 nlt() const; //! near left top
	float3 nlb() const; //! near left bottom
	float3 nrt() const; //! near right top
	float3 nrb() const; //! near right bottom

public:
	float3 min, max;
};

VENUS_END
