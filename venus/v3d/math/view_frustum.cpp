#include "stdafx.h"
#include "view_frustum.h"

VENUS_BEG

view_frustum_t::view_frustum_t()
{

}

view_frustum_t::view_frustum_t(const view_frustum_t & another)
{
	buffcpy(*this, another);
}

view_frustum_t::view_frustum_t(const float4x4 & view_matrix)
{
	from_view_matrix(view_matrix);
}

view_frustum_t & view_frustum_t::from_view_matrix(const float4x4 & view_matrix)
{
	// left clipping plane
	planes[view_vrustum_plane_l].x = view_matrix._14 + view_matrix._11;
	planes[view_vrustum_plane_l].y = view_matrix._24 + view_matrix._21;
	planes[view_vrustum_plane_l].z = view_matrix._34 + view_matrix._31;
	planes[view_vrustum_plane_l].w = view_matrix._44 + view_matrix._41;

	// right clipping plane
	planes[view_vrustum_plane_r].x = view_matrix._14 - view_matrix._11;
	planes[view_vrustum_plane_r].y = view_matrix._24 - view_matrix._21;
	planes[view_vrustum_plane_r].z = view_matrix._34 - view_matrix._31;
	planes[view_vrustum_plane_r].w = view_matrix._44 - view_matrix._41;

	// top clipping plane
	planes[view_vrustum_plane_t].x = view_matrix._14 - view_matrix._12;
	planes[view_vrustum_plane_t].y = view_matrix._24 - view_matrix._22;
	planes[view_vrustum_plane_t].z = view_matrix._34 - view_matrix._32;
	planes[view_vrustum_plane_t].w = view_matrix._44 - view_matrix._42;

	// bottom clipping plane
	planes[view_vrustum_plane_b].x = view_matrix._14 + view_matrix._12;
	planes[view_vrustum_plane_b].y = view_matrix._24 + view_matrix._22;
	planes[view_vrustum_plane_b].z = view_matrix._34 + view_matrix._32;
	planes[view_vrustum_plane_b].w = view_matrix._44 + view_matrix._42;

	// far clipping plane
	planes[view_vrustum_plane_f].x = view_matrix._14 - view_matrix._13;
	planes[view_vrustum_plane_f].y = view_matrix._24 - view_matrix._23;
	planes[view_vrustum_plane_f].z = view_matrix._34 - view_matrix._33;
	planes[view_vrustum_plane_f].w = view_matrix._44 - view_matrix._43;

	// near clipping plane
	planes[view_vrustum_plane_n].x = view_matrix._13;
	planes[view_vrustum_plane_n].y = view_matrix._23;
	planes[view_vrustum_plane_n].z = view_matrix._33;
	planes[view_vrustum_plane_n].w = view_matrix._43;

	// normalize normals
	for(int_x cnt = 0; cnt < view_vrustum_plane_count; ++cnt)
		planes[cnt].normalize();

	return *this;
}

bool view_frustum_t::get_point(view_vrustum_plane_e ePlane1,
				  view_vrustum_plane_e ePlane2,
				  view_vrustum_plane_e ePlane3,
				  float3 & point) const
{
	return planes[ePlane1].intersect_with(planes[ePlane2], planes[ePlane3], point);
}

float3 view_frustum_t::flt() const
{
	float3 point;
	get_point(view_vrustum_plane_f, view_vrustum_plane_l, view_vrustum_plane_t, point);
	return point;
}

float3 view_frustum_t::flb() const
{
	float3 point;
	get_point(view_vrustum_plane_f, view_vrustum_plane_l, view_vrustum_plane_b, point);
	return point;
}
float3 view_frustum_t::frt() const
{
	float3 point;
	get_point(view_vrustum_plane_f, view_vrustum_plane_r, view_vrustum_plane_t, point);
	return point;
}

float3 view_frustum_t::frb() const
{
	float3 point;
	get_point(view_vrustum_plane_f, view_vrustum_plane_r, view_vrustum_plane_b, point);
	return point;
}

float3 view_frustum_t::nlt() const
{
	float3 point;
	get_point(view_vrustum_plane_n, view_vrustum_plane_l, view_vrustum_plane_t, point);
	return point;
}
float3 view_frustum_t::nlb() const
{
	float3 point;
	get_point(view_vrustum_plane_n, view_vrustum_plane_l, view_vrustum_plane_b, point);
	return point;
}

float3 view_frustum_t::nrt() const
{
	float3 point;
	get_point(view_vrustum_plane_n, view_vrustum_plane_r, view_vrustum_plane_t, point);
	return point;
}

float3 view_frustum_t::nrb() const
{
	float3 point;
	get_point(view_vrustum_plane_n, view_vrustum_plane_r, view_vrustum_plane_b, point);
	return point;
}

bbox_t view_frustum_t::get_bbox() const
{
	bbox_t bbox;
	bbox.add_point(flt());
	bbox.add_point(flb());
	bbox.add_point(frt());
	bbox.add_point(frb());
	return bbox;
}

VENUS_END
