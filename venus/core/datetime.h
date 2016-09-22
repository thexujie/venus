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

class CORE_API timing
{
public:
	timing() = delete;
	// get utc time since from 1970.1.1 in milliseconds
	static int_64 timezone();
	static int_64 now();

	static bool is_leapyear(int_x year);
	static dayweek_e dayweek(int_x year, int_x month, int_x day);
	static 	int_x dayyear(int_x year, int_x month, int_x day);
};

// data_t use time of millisecond(s).
class CORE_API date_t
{
public:
	date_t();
	date_t(int_64 msec);
	date_t(int_x _year, int_x _month, int_x _day, int_x _hour, int_x _minute, int_x _second, int_x _millisecond);
	date_t(const date_t & another);
	date_t & operator = (const date_t & another);
	bool operator == (const date_t & another) const { return time() == another.time(); }
	bool operator != (const date_t & another) const { return !operator==(another); }
	int_64 time() const;

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
	//! ʱ��
	int_x timezone;

	//! ���ڼ� [1, 7]
	dayweek_e dayweek;
	//! һ���еڼ��� [0, 365]
	int_x dayyear;

	static date_t now();
};

VENUS_END
