#pragma once

#include "core_inc.h"

VENUS_BEG

enum dayweek_e
{
	//! 星期一。
	dayweek_monday = 0,
	//! 星期二。
	dayweek_tuesday,
	//! 星期三。
	dayweek_wednesday,
	//! 星期四。
	dayweek_thursday,
	//! 星期五。
	dayweek_friday,
	//! 星期六。
	dayweek_saturday,
	//! 星期日。
	dayweek_sunday
};

class CORE_API daytime_t
{
public:
	daytime_t();
	daytime_t(int_64 msec);
	daytime_t(int_x _year, int_x _month, int_x _day, int_x _hour, int_x _minute, int_x _second, int_x _millisecond);
	daytime_t(const daytime_t & another);
	daytime_t & operator = (const daytime_t & another);
	int_64 to_s() const;
	int_64 to_ms() const;

	//! 年[-, -]
	int_x year;
	//! 月 [1, 12]
	int_x month;
	//! 日 [1, 31]
	int_x day;
	//! 时 [0, 23]
	int_x hour;
	//! 分 [0, 59]
	int_x minute;
	//! 秒 [0, 59]
	int_x second;
	//! 毫秒 [0, 999]
	int_x millisecond;

	//! 星期几 [1, 7]
	dayweek_e dayweek;
	//! 一年中第几天 [0, 365]
	int_x dayyear;

	static daytime_t now_utc();
	static daytime_t now();
};

CORE_API bool is_leap_year(int_x year);
CORE_API dayweek_e day_of_week(int_x year, int_x month, int_x day);
CORE_API int_x day_of_year(int_x year, int_x month, int_x day);
CORE_API int_64 time_s();
CORE_API int_64 time_ms();
CORE_API int_x time_run_ms();

VENUS_END
