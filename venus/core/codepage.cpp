#include "stdafx.h"
#include "codepage.h"

#include "cp_936.h"

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
	return static_cast<int_x>(_getmbcp());
}

int_x SetCRTCodePageDefault()
{
	return SetCRTCodePage(_MB_CP_ANSI);
}

int_32 CP936ToUnicode(uint_8 region, uint_8 index)
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

bool CPUnicodeTo936(int_32 utf16, uint_8 & region, uint_8 & index)
{
	//char_16 wch = static_cast<char_16>(utf16);
	//char_8 mbchs[4] = {};
	//int_32 count = WideCharToMultiByte(936, 0, &wch, 1, mbchs, 4, 0, NULL);
	//if(count == 2)
	//{
	//	region = mbchs[0];
	//	index = mbchs[1];
	//	return true;
	//}
	//else
	//	return false;
	return 0;
}

bool encoding_t::is_singlebyte() const
{
	switch(codepage)
	{
	case 1200:
		return false;
	default:
		return true;
	}
}

encoding_t encoding_t::fromCP(int_x codepage)
{
	switch(codepage)
	{
	case 936:
		return gb2312;
	case 1200:
		return utf16;
	case 65001:
		return utf8;
	default:
		return ansi;
	}
}

template<int_x codepage>
static err_t encoding_to(encoding_t src_encoding, const void * src, int_x src_length, void * dst, int_x dst_size, int_x * dst_length)
{
	int_x length = 0;
	err_t err = text_encode(src_encoding.codepage, src, src_length, codepage, dst, dst_size, length);
	if(dst_length)
		*dst_length = length;
	return err;
}

encoding_t encoding_t::ansi = { 0, encoding_to<0> };
encoding_t encoding_t::utf8 = { 65001, encoding_to<65001>};
encoding_t encoding_t::utf16 = { 1200, encoding_to<1200> };
encoding_t encoding_t::gb2312 = {936, encoding_to<936>};

VENUS_END
