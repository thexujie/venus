#pragma once

#include "core_inc.h"

VENUS_BEG

enum dayweek_e
{
	//! ����һ��
	dayweek_monday = 0,
	//! ���ڶ���
	dayweek_tuesday,
	//! ��������
	dayweek_wednesday,
	//! �����ġ�
	dayweek_thursday,
	//! �����塣
	dayweek_friday,
	//! ��������
	dayweek_saturday,
	//! �����ա�
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

	//! ��[-, -]
	int_x year;
	//! �� [1, 12]
	int_x month;
	//! �� [1, 31]
	int_x day;
	//! ʱ [0, 23]
	int_x hour;
	//! �� [0, 59]
	int_x minute;
	//! �� [0, 59]
	int_x second;
	//! ���� [0, 999]
	int_x millisecond;

	//! ���ڼ� [1, 7]
	dayweek_e dayweek;
	//! һ���еڼ��� [0, 365]
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
