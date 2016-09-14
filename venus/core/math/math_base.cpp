#include "stdafx.h"
#include "math_base.h"
#include <math.h>

VENUS_BEG

int_x round(float_32 val)
{
	return (int_x)::roundf(val);
}

float_32 roundf(float_32 val)
{
	return ::roundf(val);
}

int_x ceil(float_32 val)
{
	return (int_x)::ceilf(val);
}

float_32 ceilf(float_32 val)
{
	return ::ceilf(val);
}

int_x floor(float_32 val)
{
	return (int_x)::floorf(val);
}

float_32 floorf(float_32 val)
{
	return ::floorf(val);
}

bool f32_equal(float_32 val0, float_32 val1)
{
	float_32 diff = val0 - val1;
	return (diff > -F32_EPSILON) && (diff < F32_EPSILON);
}

bool f32_less_or_equal(float_32 val0, float_32 val1)
{
	return val0 < val1 || f32_equal(val0, val1);
}

bool f32_greater_or_equal(float_32 val0, float_32 val1)
{
	return val0 > val1 || f32_equal(val0, val1);
}


float_32 abs(float_32 val)
{
	return ::fabsf(val);
}

float_32 cos(float_32 val)
{
	return ::cosf(val);
}

float_32 sin(float_32 val)
{
	return ::sinf(val);
}

float_32 tan(float_32 val)
{
	return ::tanf(val);
}

float_32 atan(float_32 val)
{
	return ::atanf(val);
}

float_32 cot(float_32 val)
{
	return 1.0f / ::tanf(val);
}

float_32 sqrt(float_32 val)
{
	return ::sqrtf(val);
}

float_32 pow(float_32 val, int_x exp)
{
	return ::powf(val, (float_32)exp);
}

float_32 pow(float_32 val, float_32 exp)
{
	return ::powf(val, exp);
}

int_32 randi32()
{
	return ::rand();
}

int_64 randi64()
{
	int_64 high = ::rand();
	int_64 low = ::rand();
	return (high << 32) | low;
}

int_x randix()
{
#ifdef BIT64
	return randi64();
#else
	return randi32();
#endif
}

float_32 lerp(float_32 min, float_32 max, float_32 determinant)
{
	return min + (max - min) * determinant;
}

VENUS_END
