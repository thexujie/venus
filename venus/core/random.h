#pragma once

VENUS_BEG

class CORE_API random
{
public :
	static void seed(uint_32 uiSeed);
	static float_32 get_foat32(float_32 lowBound, float_32 highBound);
	static int_32 get_int32();
	static int_32 get_int32(int_32 _min, int_32 _max);
	static int_x get_intx();
	static int_x get_intx(int_32 _min, int_32 _max);
};

VENUS_END
