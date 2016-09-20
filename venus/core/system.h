#pragma once

#include "core_inc.h"

VENUS_BEG

class int_tmpl
{

};

class time_t
{
public:

};

class timing
{
public:
	// get utc time since from 1970.1.1 in milliseconds
	static int_64 now_utc();
	static int_64 now();
};

VENUS_END
