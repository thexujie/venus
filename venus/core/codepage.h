#pragma once

VENUS_BEG

struct CORE_API encoding_t
{
	int_x codepage;

	encoding_t() :codepage(0) {}
	constfun encoding_t(int_x _codepage): codepage(_codepage){}

	bool singlebyte() const;
	
	encoding_t & operator = (const encoding_t & another) { codepage = another.codepage; return *this; }
	bool operator == (const encoding_t & another) { return codepage == another.codepage; }
	bool operator != (const encoding_t & another) { return !operator==(another); }

	err_t encode(encoding_t src_encoding, const void * src, int_x src_length, void * dst, int_x dst_size, int_x * dst_length) const;
	int_x length(encoding_t src_encoding, const void * src, int_x src_length) const;
};


namespace encodings
{
	constdata encoding_t unknown = encoding_t(-1);
	constdata encoding_t ansi = encoding_t(0);
	constdata encoding_t utf8 = encoding_t(65001);
	constdata encoding_t utf16 = encoding_t(1200);
	constdata encoding_t utf16_be = encoding_t(1201);
	constdata encoding_t gb2312 = encoding_t(936);

#ifdef _UNICODE
	constdata encoding_t system_encoding = utf16;
#else
	constdata encoding_t system_encoding = ansi;
#endif

	CORE_API encoding_t crt_encoding();
}

VENUS_END
