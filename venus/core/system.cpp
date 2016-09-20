#include "stdafx.h"
#include "daytime.h"

#define _CRT_NO_TIME_T
#include <time.h>
#include <sys/timeb.h>

VENUS_BEG

int_64 timing::now_utc()
{
	return _time64(nullptr);
}

int_64 timing::now()
{
	return _time64(nullptr) + _timezone;
}

VENUS_END
