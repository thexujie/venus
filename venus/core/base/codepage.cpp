#include "stdafx.h"
#include "codepage.h"
#include <mbctype.h>

VENUS_BEG

bool encoding_t::singlebyte() const
{
	switch(codepage)
	{
	case 1200:
		return false;
	default:
		return true;
	}
}

err_t encoding_t::encode(encoding_t src_encoding, const void * src, int_x src_length, void * dst, int_x dst_size, int_x * dst_length) const
{
	int_x length = 0;
	err_t err = text_encode(src_encoding.codepage, src, src_length, codepage, dst, dst_size, length);
	if(dst_length)
		*dst_length = length;
	return err;
}

int_x encoding_t::length(encoding_t src_encoding, const void * src, int_x src_length) const
{
	int_x dst_length = 0;
	err_t err = text_encode(src_encoding.codepage, src, src_length, codepage, nullptr, 0, dst_length);
	if(err)
		return 0;
	return dst_length;
}

namespace encodings
{
	encoding_t crt_encoding()
	{
		int_x codepage = _getmbcp();
		return encoding_t(codepage);
	}

	encoding_t from_charset(int_x charset)
	{
		switch(charset)
		{
		case 134:
			return gb2312;
		default:
			return crt_encoding();
		}
	}
}

VENUS_END
