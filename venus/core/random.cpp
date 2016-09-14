#include "stdafx.h"
#include "random.h"

VENUS_BEG

void random::seed(uint_32 uiSeed)
{
	srand(uiSeed);
}

float_32 random::get_foat32(float_32 fMin, float_32 fMax)
{
	return ((rand() % 10000) * 0.0001f * (fMax - fMin)) + fMin; 
}

int_32 random::get_int32()
{
	return rand();
}

int_32 random::get_int32(int_32 _min, int_32 _max)
{
	return _min + get_int32() % (_max - _min + 1);
}

int_x random::get_intx()
{
	return rand();
}

int_x random::get_intx(int_32 _min, int_32 _max)
{
	return _min + get_int32() % (_max - _min + 1);
}

VENUS_END
