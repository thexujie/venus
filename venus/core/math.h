#pragma once

#include "math/math_base.h"
#include "math/xmm/xmm.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/rect.h"
#include "math/edge.h"
#include "random.h"

#include "CTriangle.h"

#include "CMatrix3.h"
#include "CMatrix5.h"

VENUS_BEG

typedef vec2<int_32> int2;
typedef vec3<int_32> int3;
typedef vec4<int_32> int4;

typedef vec2<int_x> intx2;

typedef vec2<int_x> vec2ix;
typedef vec2<float_32> vec2f;

typedef vec2<int_32> pointi32;
typedef vec2<int_64> pointi64;
typedef vec2<int_x> pointix;

typedef vec3<int_x> vec3ix;
typedef vec3<float_32> vec3f;

typedef vec2<float_32> pointf32;
typedef vec2<float_64> pointf64;

typedef vec2<int_32> sizei32;
typedef vec2<int_64> sizei64;
typedef vec2<int_x> sizeix;

typedef vec2<float_32> sizef32;
typedef vec2<float_64> sizef64;

typedef rect_tmpl<int_32> recti32;
typedef rect_tmpl<int_64> recti64;
typedef rect_tmpl<int_x> rectix;

typedef rect_tmpl<float_32> rectf32;
typedef rect_tmpl<float_64> rectf64;

typedef edge_tmpl<int_32> edgei32;
typedef edge_tmpl<int_x> edgeix;

typedef Matrix3Tmpl<float_32> matrix3f;
typedef Matrix5Tmpl<float_32> matrix5f;

CORE_API vec2ix round(const vec2f & ptf);
CORE_API vec2f roundf(const vec2f & ptf);
CORE_API vec2ix ceil(const vec2f & szf);
CORE_API float2 texttofloat2(const char_16 * text, int_x length = -1, char_16 split = L',');
CORE_API float3 texttofloat3(const char_16 * text, int_x length = -1, char_16 split = L',');
CORE_API float4 texttofloat4(const char_16 * text, int_x length = -1, char_16 split = L',');

VENUS_END
