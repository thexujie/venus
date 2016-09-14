#include "stdafx.h"
#include "Math.h"

VENUS_BEG

vec2ix round(const vec2f & ptf)
{
	return vec2ix(round(ptf.x), round(ptf.y));
}

vec2f roundf(const vec2f & ptf)
{
	return vec2f(roundf(ptf.x), roundf(ptf.y));
}

vec2ix ceil(const vec2f & szf)
{
	return vec2ix(ceil(szf.x), ceil(szf.y));
}

float2 texttofloat2(const char_16 * text, int_x length, char_16 split)
{
	float2 value;
	texttofarr(text, length, value.af, 2, split);
	return value;
}

float3 texttofloat3(const char_16 * text, int_x length, char_16 split)
{
	float3 value;
	texttofarr(text, length, value.af, 3, split);
	return value;
}

float4 texttofloat4(const char_16 * text, int_x length, char_16 split)
{
	float4 value;
	texttofarr(text, length, value.af, 4, split);
	return value;
}

VENUS_END
