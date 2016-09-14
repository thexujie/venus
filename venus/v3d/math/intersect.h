#pragma once

VENUS_BEG

/**
 * @breif 检测射线是否与三角形相交
 * @param ray_pos 射线的起点
 * @param ray_dir 射线的方向
 * @param pos0 三角形的第 0 个点
 * @param pos1 三角形的第 1 个点
 * @param pos2 三角形的第 2 个点
 * @param res_u 如果相交，则表示交点的 u 分量
 * @param res_v 如果相交，则表示交点的 v 分量
 * @param res_d 如果相交，则表示交点与射线起点的距离
 * @param tolerance 判定共线的误差范围，默认为 0.0001f
 * @return 如果有交点，返回 true，否则，返回 false
 */
V3D_API bool intersect_tri(const float3 & ray_pos, const float3 & ray_dir,
							   const float3 & pos0, const float3 & pos1, const float3 & pos2,
							   float_32 & res_u, float_32 & res_v, float_32 & res_d,
							   float_32 tolerance = 0.0001f);

VENUS_END
