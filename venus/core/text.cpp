#include "stdafx.h"
#include "text.h"

VENUS_BEG

namespace encodings
{
	textw encocde(const char_8 * text, int_x length, encoding_t src_encoding, encoding_t dst_encoding)
	{
		if(length < 0)
			length = textlen(text);

		int_x dst_length = dst_encoding.length(src_encoding, text, length);
		textw result(dst_length, dst_length);
		err_t err = dst_encoding.encode(src_encoding, text, length, result.buffer(), result.length(), nullptr);
		return result;
	}

	texta encocde(const char_16 * text, int_x length, encoding_t src_encoding, encoding_t dst_encoding)
	{
		if(length < 0)
			length = textlen(text);

		int_x dst_length = dst_encoding.length(src_encoding, text, length);
		texta result(dst_length, dst_length);
		err_t err = dst_encoding.encode(src_encoding, text, length, result.buffer(), result.length(), nullptr);
		return result;
	}

	textw encocde(texta text, encoding_t src_encoding, encoding_t dst_encoding)
	{
		int_x length = dst_encoding.length(src_encoding, text.buffer(), text.length());
		textw result(length, length);
		err_t err = dst_encoding.encode(src_encoding, text.buffer(), text.length(), result.buffer(), result.length(), nullptr);
		return result;
	}

	texta encocde(textw text, encoding_t src_encoding, encoding_t dst_encoding)
	{
		int_x length = dst_encoding.length(src_encoding, text.buffer(), text.length());
		texta result(length, length);
		err_t err = dst_encoding.encode(src_encoding, text.buffer(), text.length(), result.buffer(), result.length(), nullptr);
		return result;
	}
}
VENUS_END
