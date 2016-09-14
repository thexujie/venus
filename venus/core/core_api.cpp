#include "stdafx.h"
#include "core_api.h"

#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>

VENUS_BEG

void breakpoint()
{
	//_asm int 3
	__debugbreak();
}

int_x textformat_calc(const char_8 * format, ...)
{
	va_list args = nullptr;
	va_start(args, format);
	int_x iLength = textformatargs_calc(format, args);
	va_end(args);
	return iLength;
}

int_x textformat_calc(const char_16 * format, ...)
{
	va_list args = nullptr;
	va_start(args, format);
	int_x iLength = textformatargs_calc(format, args);
	va_end(args);
	return iLength;
}

int_x textformat(char_8 * buffer, int_x size, const char_8 * format, ...)
{
	va_list args = nullptr;
	va_start(args, format);
	int_x iLength = textformat_args(buffer, size, format, args);
	va_end(args);
	return iLength;
}

int_x textformat(char_16 * buffer, int_x size, const char_16 * format, ...)
{
	va_list args = nullptr;
	va_start(args, format);
	int_x iLength = textformat_args(buffer, size, format, args);
	va_end(args);
	return iLength;
}

int_x textformatargs_calc(const char_8 * format, void * args)
{
	return _vscprintf(format, (va_list)args) + 1;
}

int_x textformatargs_calc(const char_16 * format, void * args)
{
	return _vscwprintf(format, (va_list)args) + 1;
}

int_x textformat_args(char_8 * buffer, int_x size, const char_8 * format, void * args)
{
	return vsprintf_s(buffer, size, format, (va_list)args);
}

int_x textformat_args(char_16 * buffer, int_x size, const char_16 * format, void * args)
{
	return vswprintf_s(buffer, size, format, (va_list)args);
}

bool isprintchar(char_32 ch)
{
	if(ch == 0x7F)
		return false;
	else if(ch >= 0xFF)
		return true;
	else if(ch == L'\t')
		return true;
	else if(ch >= L' ' && ch <= L'~')
		return true;
	else
		return false;
}

int_x ansitounicode(const char_8 * src, int_x length, char_16 * dst, int_x size)
{
	uint_x dst_length = 0;
	if(length < 0)
		length = textlen(src);
	mbstowcs_s((size_t *)&dst_length, dst, size, src, length);
	return (int_x)dst_length;
}

int_x unicodetoansi(const char_16 * src, int_x length, char_8 * dst, int_x size)
{
	uint_x dst_length = 0;
	if(length < 0)
		length = textlen(src);
	wcstombs_s((size_t *)&dst_length, dst, size, src, length);
	return (int_x)dst_length;
}

int_x hashtext16(const char_16 * const & text, int_x len)
{
	const char_16 * pTemp = text;
	int_x uiSeed = DEF_SEED_X;
	while(*pTemp && len--)
		uiSeed = DEF_BASE_X * uiSeed ^ (int_x)*pTemp++;
	return uiSeed;
}

int_x hashtext8(const char_8 * const & text, int_x len)
{
	const char_8 * pTemp = text;
	int_x uiSeed = DEF_SEED_X;
	while(*pTemp && len--)
		uiSeed = DEF_BASE_X * uiSeed ^ (int_x)*pTemp++;
	return uiSeed;
}

int_x hashtextx(const char_x * const & text, int_x len)
{
#ifdef _UNICODE
	return hashtext16(text, len);
#else
	return hashtext8(text, len);
#endif
}

int_x hashbytes(const byte_t * bytes, int_x len)
{
	const byte_t * pEnd = bytes + len;
	int_x uiSeed = DEF_SEED_X;
	while(bytes != pEnd)
		uiSeed = DEF_BASE_X * uiSeed ^ (int_x)*bytes++;
	return uiSeed;
}

int_x hashbool(const bool & val, int_x curr)
{
	int_x uiVal = val ? UIX_MAX : 0;
	return DEF_BASE_X * curr ^ uiVal;
}

VENUS_END
