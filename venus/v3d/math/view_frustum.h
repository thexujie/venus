#pragma once

#include "bbox.h"

VENUS_BEG

enum view_vrustum_plane_e
{
	view_vrustum_plane_f = 0,
	view_vrustum_plane_n,
	view_vrustum_plane_l,
	view_vrustum_plane_t,
	view_vrustum_plane_r,
	view_vrustum_plane_b,
	view_vrustum_plane_count,
};

class V2D_API view_frustum_t
{
public:
	view_frustum_t();
	view_frustum_t(const view_frustum_t & another);
	view_frustum_t(const float4x4 & view_matrix);

	view_frustum_t & from_view_matrix(const float4x4 & view_matrix);
	bool get_point(view_vrustum_plane_e ePlane1,
				  view_vrustum_plane_e ePlane2,
				  view_vrustum_plane_e ePlane3,
				  float3 & point) const;
	float3 flt() const; //! far left top
	float3 flb() const; //! far left bottom
	float3 frt() const; //! far right top
	float3 frb() const; //! far right bottom
	float3 nlt() const; //! near left top
	float3 nlb() const; //! near left bottom
	float3 nrt() const; //! near right top
	float3 nrb() const; //! near right bottom

	bbox_t get_bbox() const;

public:
	plane_t planes[view_vrustum_plane_count];
};

VENUS_END
