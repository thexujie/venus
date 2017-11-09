#include "stdafx.h"
#include "datetime.h"

#define _CRT_NO_TIME_T
#include <time.h>
#include <sys/timeb.h>

VENUS_BEG

int_64 timing::timezone()
{
	long timezone = 0;
	_get_timezone(&timezone);
	return timezone;
}

int_64 timing::now()
{
	return _time64(nullptr);
}

bool timing::is_leapyear(int_x year)
{
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

dayweek_e timing::dayweek(int_x year, int_x month, int_x day)
{
	if(month < 1 || 12 < month)
		return dayweek_monday;

	static int_x DAY[] = { 6, 2, 1, 4, 6, 2, 4, 0, 3, 5, 1, 3 };
	year -= month < 3;
	return static_cast<dayweek_e>((year + year / 4 - year / 100 + year / 400 + DAY[month - 1] + day) % 7);
}

int_x timing::dayyear(int_x year, int_x month, int_x day)
{
	if(month < 1 || 12 < month)
		return 0;

	static int_x DAY[] = { 1, 0, 2, 0, 3, 0, 4, 5, 0, 6, 0, 7 };
	int_x temp = (month - 1) * 30 + DAY[month - 1] + day;
	if(month > 2)
		temp -= is_leapyear(year) ? 1 : 2;
	return temp;
}

date_t::date_t() : date_t(timing::now())
{

}

date_t::date_t(int_64 msec)
{
	timezone = timing::timezone();

	__time64_t sec = msec / 1000 - timezone;
	tm ltime;
	_gmtime64_s(&ltime, &sec);
	year = ltime.tm_year + 1900;
	month = ltime.tm_mon + 1;
	day = ltime.tm_mday;
	hour = ltime.tm_hour;
	minute = ltime.tm_min;
	second = ltime.tm_sec;
	millisecond = static_cast<int_x>(msec % 1000);

	dayweek = static_cast<dayweek_e>((ltime.tm_wday + 6) % 7);
	dayyear = ltime.tm_yday;
}

date_t::date_t(int_x _year, int_x _month, int_x _day, int_x _hour, int_x _minute, int_x _second, int_x _millisecond) :
	timezone(timing::timezone()),
	year(_year), month(_month), day(_day), hour(_hour), minute(_minute), second(_second), millisecond(_millisecond),
	dayweek(timing::dayweek(_year, _month, _day)), dayyear(timing::dayyear(_year, _month, _day))
{
}

date_t::date_t(const date_t & another)
{
	timezone = another.timezone;
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

date_t & date_t::operator = (const date_t & another)
{
	timezone = another.timezone;
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

int_64 date_t::time() const
{
	tm ltime = {};
	ltime.tm_year = static_cast<int_32>(year - 1900);
	ltime.tm_mon = static_cast<int_32>(month - 1);
	ltime.tm_mday = static_cast<int_32>(day);
	ltime.tm_hour = static_cast<int_32>(hour);
	ltime.tm_min = static_cast<int_32>(minute);
	ltime.tm_sec = static_cast<int_32>(second);
	ltime.tm_wday = static_cast<int_32>(dayweek);
	ltime.tm_yday = static_cast<int_32>(dayyear);
	__time64_t sec = _mkgmtime64(&ltime);
	return (sec + timezone) * 1000 + millisecond;
}

date_t date_t::now()
{
	__timeb64 tb;
	_ftime64_s(&tb);

	return date_t(tb.time * 1000 + tb.millitm);
}

VENUS_END
