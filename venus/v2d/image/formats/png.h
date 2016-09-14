#pragma once

VENUS_BEG

//	数据块符号	数据块名称			多数据块	可选否	位置限制
//	IHDR		文件头数据块			否		否		第一块
//	cHRM		基色和白色点数据块		否		是		在PLTE和IDAT之前
//	gAMA		图像γ数据块			否		是		在PLTE和IDAT之前
//	sBIT		样本有效位数据块		否		是		在PLTE和IDAT之前
//	PLTE		调色板数据块			否		是		在IDAT之前
//	bKGD		背景颜色数据块		否		是		在PLTE之后IDAT之前
//	hIST		图像直方图数据块		否		是		在PLTE之后IDAT之前
//	tRNS		图像透明数据块		否		是		在PLTE之后IDAT之前
//	oFFs		(专用公共数据块)		否		是		在IDAT之前
//	pHYs		物理像素尺寸数据块		否		是		在IDAT之前
//	sCAL		(专用公共数据块)		否		是		在IDAT之前
//	IDAT		图像数据块			是		否		与其他IDAT连续
//	tIME		图像最后修改时间数据块	否		是		无限制
//	tEXt		文本信息数据块		是		是		无限制
//	zTXt		压缩文本数据块		是		是		无限制
//	fRAc		(专用公共数据块)		是		是		无限制
//	gIFg		(专用公共数据块)		是		是		无限制
//	gIFt		(专用公共数据块)		是		是		无限制
//	gIFx		(专用公共数据块)		是		是		无限制
//	IEND		图像结束数据			否		否		最后一个数据块

#define PNG_MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
	((unsigned int)(unsigned char)(ch0) | ((unsigned int)(unsigned char)(ch1) << 8) | \
	((unsigned int)(unsigned char)(ch2) << 16) | ((unsigned int)(unsigned char)(ch3) << 24))

const img_u8 PNG_HEADER[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
const img_u8 PNG_TAIL[12] = {0x0, 0x0, 0x0, 0x0, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82};
#pragma pack(push, 1)
typedef enum tag_png_color_type
{
	png_color_type_invalid = -1,
	png_color_type_gray = 0,
	//png_color_type_palette,
	png_color_type_rgb = 2,
	png_color_type_index_rgb = 3,
	png_color_type_gray_alpha = 4,
	png_color_type_argb = 6,
}png_color_type_e;

typedef struct tag_png_header
{
	unsigned int width;
	unsigned int height;
	unsigned char bits;
	unsigned char color_type;
	unsigned char format;
	unsigned char filter;
	unsigned char interlace;
}png_header_t;

typedef enum tag_png_chunk_type
{
	png_chunk_type_ihdr = PNG_MAKEFOURCC('I', 'H', 'D', 'R'), // 文件头 --------
	png_chunk_type_chrm = PNG_MAKEFOURCC('c', 'H', 'R', 'M'), // 基色和白色点数据块
	png_chunk_type_gama = PNG_MAKEFOURCC('g', 'A', 'M', 'A'), // 图像 gama 数据块
	png_chunk_type_sbits = PNG_MAKEFOURCC('s', 'B', 'I', 'T'), // 样本有效位数据块
	png_chunk_type_plte = PNG_MAKEFOURCC('P', 'L', 'T', 'E'), // 调色板数据块 ---------
	png_chunk_type_bkgd = PNG_MAKEFOURCC('b', 'K', 'G', 'D'), // 背景颜色数据块
	png_chunk_type_hist = PNG_MAKEFOURCC('h', 'I', 'S', 'T'), // 图像直方图数据块
	png_chunk_type_trns = PNG_MAKEFOURCC('t', 'R', 'N', 'S'), // 图像透明数据块
	png_chunk_type_offs = PNG_MAKEFOURCC('o', 'F', 'F', 's'), // (专用公共数据块)
	png_chunk_type_phys = PNG_MAKEFOURCC('p', 'H', 'Y', 's'), // 物理像素尺寸数据块
	png_chunk_type_scal = PNG_MAKEFOURCC('s', 'C', 'A', 'L'), // (专用公共数据块)
	png_chunk_type_idata = PNG_MAKEFOURCC('I', 'D', 'A', 'T'), // 图像数据块
	png_chunk_type_time = PNG_MAKEFOURCC('t', 'I', 'M', 'E'), // 图像最后修改时间数据块
	png_chunk_type_text = PNG_MAKEFOURCC('t', 'E', 'X', 't'), // 文本信息数据块
	png_chunk_type_ztxt = PNG_MAKEFOURCC('z', 'T', 'X', 't'), // 压缩文本数据块
	png_chunk_type_frac = PNG_MAKEFOURCC('f', 'R', 'A', 'c'), // (专用公共数据块)
	png_chunk_type_gift = PNG_MAKEFOURCC('g', 'I', 'F', 't'), // (专用公共数据块)
	png_chunk_type_gifx = PNG_MAKEFOURCC('g', 'I', 'F', 'x'), // (专用公共数据块)
	png_chunk_type_iend = PNG_MAKEFOURCC('I', 'E', 'N', 'D'), // 图像结束数据
}png_chunk_type_e;

typedef struct tag_png_chunk
{
	union
	{
		unsigned int size;
		struct
		{
			unsigned char size_4;
			unsigned char size_3;
			unsigned char size_2;
			unsigned char size_1;
		};
	};

	union
	{
		png_chunk_type_e type;
		unsigned char type_str[4];
	};
}png_chunk_t;

typedef struct tag_png_chunk_sbit
{
	unsigned int x1;
	unsigned int s_bit;
	unsigned int x2;
}png_chunk_sbit_t;

typedef struct tag_png_chunk_phys
{
	unsigned int ppux;
	unsigned int ppuy;
	unsigned char unit; // 0(未知), 1(米)
}png_chunk_phys_t;

#pragma pack(pop)

IMAGE_API bool is_png_data(const img_u8 * pBuffer, img_ix length);
IMAGE_API img_ix png_create(const img_u8 * buffer, img_ix length, image_data_t * img,
						  image_convert_rule_fun_t pfn_match = nullptr, void * user_data = nullptr);
IMAGE_API bool png_rule_default(image_convert_rule_t * rule);

const image_convert_fun_t image_convert_png_use_src = (image_convert_fun_t)-1;

VENUS_END
