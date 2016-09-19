#pragma once

VENUS_BEG

CORE_API err_t text_encode(int_x src_codepage, const void * src, int_x src_length,
	int_x dst_codepage, void * dst, int_x dst_size, int_x & dst_length);

CORE_API void thread_set_name(int_x iThreadId, const char_8 * szName);
CORE_API void thread_set_name(int_x iThreadId, const char_16 * szName);

VENUS_END
