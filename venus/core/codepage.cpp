#include "stdafx.h"
#include "codepage.h"

#include "cp_936.h"
#include "cp_20936.h"

#include <mbctype.h>
#include <locale.h>

VENUS_BEG

int_x SetCRTCodePage(int_x iCodePage)
{
	_setmbcp((int)iCodePage);
	return GetCRTCodePage();
}
int_x GetCRTCodePage()
{
	return (int_x)(_getmbcp());
}

int_x SetCRTCodePageDefault()
{
	return SetCRTCodePage(_MB_CP_ANSI);
}

int_32 CP936ToUnicode(int_32 region, int_32 index)
{
	if(0x81 <= region && region <= 0xFE)
	{
		int_32 row = 0, col = 0;
		index -= 0x40;
		row = (region - 0x81) * 12 + (index >> 4);
		col = index & 0xF;
		return CP_936[row][col];
	}
	else
		return 0x003F;
}

int_32 CP20936ToUnicode(int_32 region, int_32 index)
{
	if(0xA1 <= region && region <= 0xF7)
	{
		int_32 row = 0, col = 0;
		index -= 0xA0;
		row = (region - 0xA1) * 6 + (index >> 4);
		col = index & 0xF;
		return CP_20936[row][col];
	}
	else
		return 0x003F;
}

VENUS_END
