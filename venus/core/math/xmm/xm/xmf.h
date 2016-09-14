#pragma once

VENUS_BEG

typedef float_32 float1;

union xmt
{
	xmt() {}
	xmt(float_32 _x, float_32 _y, float_32 _z, float_32 _w) : fx(_x), fy(_y), fz(_z), fw(_w) {}
	xmt(int_32 _x, int_32 _y, int_32 _z, int_32 _w) : ix(_x), iy(_y), iz(_z), iw(_w) {}
	xmt(uint_32 _x, uint_32 _y, uint_32 _z, uint_32 _w) : uix(_x), uiy(_y), uiz(_z), uiw(_w) {}

	float_32 af[4];
	struct
	{
		float_32 fx;
		float_32 fy;
		float_32 fz;
		float_32 fw;
	};
	int_32 ai[4];
	struct
	{
		int_32 ix;
		int_32 iy;
		int_32 iz;
		int_32 iw;
	};
	uint_32 aui[4];
	struct
	{
		uint_32 uix;
		uint_32 uiy;
		uint_32 uiz;
		uint_32 uiw;
	};

	float_32 & operator[](int_x iIndex) { return af[iIndex]; }
	const float_32 & operator[](int_x iIndex) const { return af[iIndex]; }

#ifdef VXM_SSE
#ifdef BIT_64
	__m128 m128;
	xmt(__m128 _m128) : m128(_m128) {}
	operator const __m128 &() const { return m128; }
	operator __m128 &() { return m128; }
	xmt & operator = (__m128 _m128) { m128 = _m128; return *this; }

	__m128i m128i;
	xmt(__m128i _m128i) : m128i(_m128i) {}
	operator const __m128i &() const { return m128i; }
	operator __m128i &() { return m128i; }
	xmt & operator = (__m128i _m128i) { m128i = _m128i; return *this; }
#else // BIT_64
	xmt(__m128 _m128) : xmt(_m128.m128_f32[0], _m128.m128_f32[1], _m128.m128_f32[2], _m128.m128_f32[3]) {}
	operator __m128 () const { return _mm_load_ps(af); }
	xmt & operator = (__m128 _m128) { buffcpy(af, 4, _m128.m128_f32, 4); return *this; }

	xmt(__m128i _m128i) : xmt(_m128i.m128i_i32[0], _m128i.m128i_i32[1], _m128i.m128i_i32[2], _m128i.m128i_i32[3]) {}
	operator __m128i () const { return _mm_castps_si128(*(const __m128 *)af); };
	xmt & operator = (__m128i _m128i) { _mm_store_si128((__m128i *)af, _m128i); return *this; }
#endif // BIT_64
#endif // VXM_SSE
};

typedef xmt xmf;
typedef xmt xmi;
typedef xmt xmui;

inline float_32 xmv_x(const xmf & v) { return v.fx; }
inline float_32 xmv_y(const xmf & v) { return v.fy; }
inline float_32 xmv_z(const xmf & v) { return v.fz; }
inline float_32 xmv_w(const xmf & v) { return v.fw; }
inline int_32 xmi_x(const xmf & v) { return v.ix; }
inline int_32 xmi_y(const xmf & v) { return v.iy; }
inline int_32 xmi_z(const xmf & v) { return v.iz; }
inline int_32 xmi_w(const xmf & v) { return v.iw; }

struct xmm
{
	xmm() {}
	xmm(xmf _row0, xmf _row1, xmf _row2, xmf _row3) : row0(_row0), row1(_row1), row2(_row2), row3(_row3) {}

	xmf row0;
	xmf row1;
	xmf row2;
	xmf row3;

	xmf & operator[](int_x iIndex) { return ((xmf *)this)[iIndex]; }
	const xmf & operator[](int_x iIndex) const { return ((xmf *)this)[iIndex]; }
};

const float_32 float_epsilon = 1.192092896e-07f;

inline bool float_is_zero(float_32 val) { return abs(val) < float_epsilon; }
inline bool float_equal(const float_32 & val1, const float_32 & val2) { return abs(val1 - val2) < float_epsilon; }
inline float_32 float_reciprocal(float_32 val) { return 1.0f / val; }
inline float_32 float_sqrt_reciprocal(float_32 val) { return 1.0f / sqrt(val); }

inline bool float_is_inf(float_32 val) { return ((*(uint_32 *)&val) & 0x7FFFFFFF) == 0x7F800000; }
inline float_32 float_abs(float_32 val) { return abs(val); }
inline float_32 float_round(float_32 val) { return roundf(val); }
inline float_32 float_sqrt(float_32 val) { return sqrt(val); }
inline float_32 float_sin(float_32 val) { return sin(val); }
inline float_32 float_cos(float_32 val) { return cos(val); }
inline float_32 float_tan(float_32 val) { return tan(val); }
inline float_32 float_cot(float_32 val) { return 1.0f / tan(val); }
inline float_32 float_atan(float_32 val) { return atan(val); }

float_32 float_lerp(float_32 min, float_32 max, float_32 determine);

const uint_32 xm_permute_0x = 0;
const uint_32 xm_permute_0y = 1;
const uint_32 xm_permute_0z = 2;
const uint_32 xm_permute_0w = 3;
const uint_32 xm_permute_1x = 4;
const uint_32 xm_permute_1y = 5;
const uint_32 xm_permute_1z = 6;
const uint_32 xm_permute_1w = 7;

const uint_32 xm_swizzle_x = 0;
const uint_32 xm_swizzle_y = 1;
const uint_32 xm_swizzle_z = 2;
const uint_32 xm_swizzle_w = 3;

const float_32 xm_pi = 3.141592654f;
const float_32 xm_2pi = xm_pi * 2;
const float_32 xm_1_of_1pi = 0.318309886f;
const float_32 xm_1_of_2pi = 0.159154943f;
const float_32 xm_1pi_of_2 = 1.570796327f;
const float_32 xm_1pi_of_4 = 0.785398163f;

const uint_32 xm_select_0 = 0x00000000;
const uint_32 xm_select_1 = 0xFFFFFFFF;

const xmf xmf_zero = {0.0f, 0.0f, 0.0f, 0.0f};
const xmf xmf_one = {1.0f, 1.0f, 1.0f, 1.0f};

const xmf xmf_1of2 = {0.5f, 0.5f, 0.5f, 0.5f};

const xmui xmf_negative_zero = {0x80000000, 0x80000000, 0x80000000, 0x80000000};
const xmf xmf_negative_one = {-1.0f, -1.0f, -1.0f, -1.0f};

const xmui xmf_abs_mask = {0x7FFFFFFFu, 0x7FFFFFFFu, 0x7FFFFFFFu, 0x7FFFFFFFu};
const xmui xmf_infinity = {0x7F800000u, 0x7F800000u, 0x7F800000u, 0x7F800000u};
const xmui xmf_mask_3 = {0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0x00000000u};
const xmui xmf_mask_x = {0xFFFFFFFFu, 0x00000000u, 0x00000000u, 0x00000000u};
const xmui xmf_mask_y = {0x00000000u, 0xFFFFFFFFu, 0x00000000u, 0x00000000u};
const xmui xmf_mask_z = {0x00000000u, 0x00000000u, 0xFFFFFFFFu, 0x00000000u};
const xmui xmf_mask_w = {0x00000000u, 0x00000000u, 0x00000000u, 0xFFFFFFFFu};
const xmui xmf_qnan = {0x7FC00000u, 0x7FC00000u, 0x7FC00000u, 0x7FC00000u};

const xmf xmf_pi = {xm_pi, xm_pi, xm_pi, xm_pi};
const xmf xmf_2pi = {xm_2pi, xm_2pi, xm_2pi, xm_2pi};
const xmf xmf_1pi_of_2 = {xm_1pi_of_2, xm_1pi_of_2, xm_1pi_of_2, xm_1pi_of_2};
const xmf xmf_1_of_2pi = {xm_1_of_2pi, xm_1_of_2pi, xm_1_of_2pi, xm_1_of_2pi};

const xmf xmf_row0 = {1.0f, 0.0f, 0.0f, 0.0f};
const xmf xmf_row1 = {0.0f, 1.0f, 0.0f, 0.0f};
const xmf xmf_row2 = {0.0f, 0.0f, 1.0f, 0.0f};
const xmf xmf_row3 = {0.0f, 0.0f, 0.0f, 1.0f};
const xmf xmf_row3_neg = {0.0f, 0.0f, 0.0f, -1.0f};

const xmf xmf_0000 = {0.0f, 0.0f, 0.0f, 0.0f};
const xmf xmf_1000 = {1.0f, 0.0f, 0.0f, 0.0f};
const xmf xmf_1110 = {1.0f, 1.0f, 1.0f, 0.0f};
const xmf xmf_1111 = {1.0f, 1.0f, 1.0f, 1.0f};

const xmui xmf_select_1110 = {xm_select_1, xm_select_1, xm_select_1, xm_select_0};
const xmui xmf_select_0101 = {xm_select_0, xm_select_1, xm_select_0, xm_select_1};

const xmf xmf_epsilon = {1.192092896e-07f, 1.192092896e-07f, 1.192092896e-07f, 1.192092896e-07f};
// atan ÏµÊý
const xmf xmf_atan_coefficients1 = {-0.0752896400f, +0.0429096138f, -0.0161657367f, +0.0028662257f};

const xmf xmf_sin_coefficients0 = {-0.16666667f, +0.0083333310f, -0.00019840874f, +2.7525562e-06f};
const xmf xmf_sin_coefficients1 = {-2.3889859e-08f, -0.16665852f, +0.0083139502f, -0.00018524670f};

const xmf xmf_cos_coefficients0 = {-0.5f, +0.041666638f, -0.0013888378f, +2.4760495e-05f};
const xmf xmf_cos_coefficients1 = {-2.6051615e-07f, -0.49992746f, +0.041493919f, -0.0012712436f};

#include "xmf_base.inl"
#include "xmf_vec.inl"
#include "xmf_quat.inl"
#include "xmf_matr.inl"

VENUS_END
