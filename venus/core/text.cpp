#include "stdafx.h"
#include "text.h"

#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>

VENUS_BEG

textw tounicodetext(const char_8 * text, int_x length)
{
	if(length < 0)
		length = textlen(text);

	textw result;
	if(!text || !length)
		return result;

	uint_x size = 0;
	mbstowcs_s((size_t *)&size, 0, 0, text, length);
	if(size)
	{
		result.resize(size - 1);
		buffset16(result.buffer(), size, 0);
		mbstowcs_s((size_t *)&size, result.buffer(), size, text, length);
	}
	return result;
}

textw tounicodetextex(const char_8 * text, int_x length, _locale_t locale)
{
	if(length < 0)
		length = textlen(text);

	textw result;
	if(!text || !length)
		return result;

	uint_x size = 0;
	_mbstowcs_s_l((size_t *)&size, 0, 0, text, length, locale);
	if(size)
	{
		result.resize(size - 1);
		buffset16(result.buffer(), size, 0);
		_mbstowcs_s_l((size_t *)&size, result.buffer(), size, text, length, locale);
	}
	return result;
}

texta toansitext(const char_16 * text, int_x length)
{
	if(length < 0)
		length = textlen(text);

	texta result;
	if(!text || !length)
		return result;

	uint_x size = 0;
	wcstombs_s((size_t *)&size, 0, 0, text, length);
	if(size)
	{
		result.resize(size);
		buffset8(result.buffer(), size + 1, 0);
		wcstombs_s((size_t *)&size, result.buffer(), size + 1, text, length);
	}
	return result;
}

VENUS_END
