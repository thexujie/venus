#pragma once

VENUS_BEG

/**
 * @brief 程序所支持的代码页，用于解析Ansi格式的字符。
 */
enum CodePage
{
	CodePageInvalid = -1,
	//! 简体中文(GB2312)。
	CodePageGB2312 = 20936,
	//! 简体中文(GBK)。
	CodePageGBK = 936,
	//! 繁体中文(BIG-5)。
	CodePageBig5 = 950,
	//! 英语(美国)。
	CodePageEnUSA = 437,

	CodePageUnicode16 = 1200,
	CodePageUnicode16BigEndian = 1201,
	CodePageUTF8 = 65001,
};

CORE_API int_x SetCRTCodePage(int_x iCodePage);
CORE_API int_x GetCRTCodePage();
CORE_API int_x SetCRTCodePageDefault();

const char_8 CODE_PAGE_GBK[] = "Chinese_People's Republic of China.936";

CORE_API int_32 CP936ToUnicode(uint_8 region, uint_8 index);
CORE_API bool CPUnicodeTo936(int_32 utf16, uint_8 & region, uint_8 & index);

struct CORE_API encoding_t
{
	typedef err_t(*encoding_convert_fun_t)(encoding_t src_encoding, const void * src, int_x src_length, void * dst, int_x dst_size, int_x * dst_length);

	int_32 codepage;
	encoding_convert_fun_t pfn_convert;

	bool is_singlebyte() const;
	encoding_t fromCP(int_x codepage);

	bool operator == (const encoding_t & another) { return codepage == another.codepage && pfn_convert == another.pfn_convert; }
	bool operator != (const encoding_t & another) { return !operator==(another); }

	static encoding_t ansi;
	static encoding_t utf8;
	static encoding_t utf16;
	static encoding_t gb2312;
};

VENUS_END
