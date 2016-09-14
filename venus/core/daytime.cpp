#include "stdafx.h"
#include "daytime.h"

#include <time.h>
#include <sys/timeb.h>

VENUS_BEG

int_64 time_s()
{
	return time(nullptr);
}
int_64 time_ms()
{
	timeb t;
	ftime(&t);
	return 1000i64 * t.time + t.millitm;
}
int_x time_run_ms()
{
	return clock();
}

bool is_leap_year(int_x year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

dayweek_e day_of_week(int_x year, int_x month, int_x day)
{
	if(month < 1 || 12 < month)
		return dayweek_monday;

	static int_x DAY[] = {6, 2, 1, 4, 6, 2, 4, 0, 3, 5, 1, 3};
	year -= month < 3;
	return (dayweek_e)((year + year / 4 - year / 100 + year / 400 + DAY[month - 1] + day) % 7);
}

int_x day_of_year(int_x year, int_x month, int_x day)
{
	if(month < 1 || 12 < month)
		return 0;

	static int_x DAY[] = {1, 0, 2, 0, 3, 0, 4, 5, 0, 6, 0, 7};
	int_x temp = (month - 1) * 30 + DAY[month - 1] + day;
	if(month > 2)
		temp -= is_leap_year(year) ? 1 : 2;
	return temp;
}

daytime_t::daytime_t():daytime_t(time_ms())
{
	
}
daytime_t::daytime_t(int_64 msec)
{
	__time64_t t = msec / 1000i64;
	tm ltime;
	_localtime64_s(&ltime, &t);
	year = ltime.tm_year + 1900;
	month = ltime.tm_mon + 1;
	day = ltime.tm_mday;
	hour = ltime.tm_hour;
	minute = ltime.tm_min;
	second = ltime.tm_sec;
	millisecond = (int_x)(msec % 1000);
	dayweek = (dayweek_e)((ltime.tm_wday + 6) % 7);
	dayyear = ltime.tm_yday;
}

daytime_t::daytime_t(int_x _year, int_x _month, int_x _day, int_x _hour, int_x _minute, int_x _second, int_x _millisecond):
	year(_year), month(_month), day(_day), hour(_hour), minute(_minute), second(_second), millisecond(_millisecond),
	dayweek(day_of_week(_year, _month, _day)), dayyear(day_of_year(_year, _month, _day))
{

}

daytime_t::daytime_t(const daytime_t & another)
{
	year = another.year;
	month = another.month;
	day = another.day;
	hour = another.hour;
	minute = another.minute;
	second = another.second;
	millisecond = another.millisecond;
	dayweek = another.dayweek;
	dayyear = another.dayyear;
}

daytime_t & daytime_t::operator = (const daytime_t & another)
{
	year = another.year;
	month = another.month;
	day = another.day;
	hour = another.hour;
	minute = another.minute;
	second = another.second;
	millisecond = another.millisecond;
	dayweek = another.dayweek;
	dayyear = another.dayyear;
	return *this;
}

daytime_t daytime_t::from_s(int_64 sec)
{
	return daytime_t(sec * 1000);
}

daytime_t daytime_t::from_ms(int_64 msec)
{
	return daytime_t(msec);
}

int_64 daytime_t::to_s() const
{
	tm ltime = {};
	ltime.tm_year = (int_32)(year - 1900);
	ltime.tm_mon = (int_32)(month - 1);
	ltime.tm_mday = (int_32)(day);
	ltime.tm_hour = (int_32)(hour);
	ltime.tm_min = (int_32)(minute);
	ltime.tm_sec = (int_32)(second);
	ltime.tm_wday = (int_32)(dayweek);
	ltime.tm_yday = (int_32)(dayyear);
	__time64_t t64 = _mkgmtime64(&ltime);
	return t64;
}

int_64 daytime_t::to_ms() const
{
	return to_s() * 1000 + millisecond;
}

VENUS_END
