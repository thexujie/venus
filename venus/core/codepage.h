#pragma once

VENUS_BEG

/**
 * @brief ������֧�ֵĴ���ҳ�����ڽ���Ansi��ʽ���ַ���
 */
enum CodePage
{
	CodePageInvalid = -1,
	//! ��������(GB2312)��
	CodePageGB2312 = 20936,
	//! ��������(GBK)��
	CodePageGBK = 936,
	//! ��������(BIG-5)��
	CodePageBig5 = 950,
	//! Ӣ��(����)��
	CodePageEnUSA = 437,

	CodePageUnicode16 = 1200,
	CodePageUnicode16BigEndian = 1201,
	CodePageUTF8 = 65001,
};

CORE_API int_x SetCRTCodePage(int_x iCodePage);
CORE_API int_x GetCRTCodePage();
CORE_API int_x SetCRTCodePageDefault();

const char_8 CODE_PAGE_GBK[] = "Chinese_People's Republic of China.936";

CORE_API int_32 CP20936ToUnicode(int_32 region, int_32 index);
CORE_API int_32 CP936ToUnicode(int_32 region, int_32 index);

VENUS_END
