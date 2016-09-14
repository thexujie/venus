#pragma once

VENUS_BEG

/**
 * @breif ��������Ƿ����������ཻ
 * @param ray_pos ���ߵ����
 * @param ray_dir ���ߵķ���
 * @param pos0 �����εĵ� 0 ����
 * @param pos1 �����εĵ� 1 ����
 * @param pos2 �����εĵ� 2 ����
 * @param res_u ����ཻ�����ʾ����� u ����
 * @param res_v ����ཻ�����ʾ����� v ����
 * @param res_d ����ཻ�����ʾ�������������ľ���
 * @param tolerance �ж����ߵ���Χ��Ĭ��Ϊ 0.0001f
 * @return ����н��㣬���� true�����򣬷��� false
 */
V3D_API bool intersect_tri(const float3 & ray_pos, const float3 & ray_dir,
							   const float3 & pos0, const float3 & pos1, const float3 & pos2,
							   float_32 & res_u, float_32 & res_v, float_32 & res_d,
							   float_32 tolerance = 0.0001f);

VENUS_END
