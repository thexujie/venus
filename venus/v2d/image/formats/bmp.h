#pragma once

VENUS_BEG

//////////////////////////////////////////////////////////////////////////
// Bitmap 文件(压缩)格式
typedef enum tag_bmp_encode
{
	bmp_encode_none = 0, // 未压缩
	bmp_encode_rle8, // rle8压缩
	bmp_encode_rle4, // rle4压缩
	bmp_encode_mask, // 由掩码(color_mask_abgr_t)决定

	// os2 压缩方式
	bmp_encode_os2_index4, // OS/2 的 4 位编码
	bmp_encode_os2_index8, // OS/2 的 8 位编码
	bmp_encode_os2_24, // OS/2 的 24 位编码
}bmp_encode_e;

const img_u8 BMP_HEADER[2] = {0x42, 0x4D};
const img_u16 BMP_MAGIC = 0x4D42; // 'BM',header.type

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
// 简单的 Bitmap 文件头，用于识别 Bitmap 文件类型。
struct bmp_simple_header_t
{
	img_u16 type;
	img_u32 file_size;
	img_u32 reserved;
	img_u32 data_offset;
	img_u32 info_size;
};

//////////////////////////////////////////////////////////////////////////
// Windows Bitmap 文件头
struct bmp_info_windows_t
{
	img_u32 size;
	img_32 width;
	img_32 height;
	img_u16 plane_count;
	img_u16 bit_count;
	bmp_encode_e encode;
	img_u32 image_size;
	img_32 ppm_x; // pels per meter
	img_32 ppm_y;
	img_u32 color_used;
	img_u32 color_important;
};

struct bmp_header_windows_t
{
	img_u16 type;
	img_u32 file_size;
	img_u32 reserved;
	img_u32  data_offset;
};

//////////////////////////////////////////////////////////////////////////
// OS/2 Bitmap 文件头
struct bmp_info_os2_t
{
	img_u32 info_size;
	img_u16 width;
	img_u16 height;
	img_u16 planes;
	img_u16 bits;
};

struct bmp_header_os2_t
{
	img_u16 type;
	img_u32 file_size;
	img_u32 reserved;
	img_u32  data_offset;
	union
	{
		bmp_info_os2_t info;
		struct
		{
			img_u32 info_size;
			img_u16 width;
			img_u16 height;
			img_u16 planes;
			img_u16 bits;
		};
	};
};

#pragma pack(pop)

IMAGE_API bool is_bmp_data(const img_u8 * buffer, img_ix length);
IMAGE_API img_ix bmp_create(const img_u8 * buffer, img_ix length, image_data_t * img,
						  image_convert_rule_fun_t pfn_match = nullptr, void * user_data = nullptr);

/**
* @brief 默认的 bmp 格式转换规则，默认将尽量在不损失精度的前提下转换为 GDI 兼容格式
* 可能的输出格式有 r5g6b5 r8g8b8 a8r8g8b8 三种
*/
IMAGE_API bool bmp_rule_default(image_convert_rule_t * rule);

/**
* Bmp 文件的 rle index4 压缩方式，使用索引色。
* rle 压缩 src_stride 和 src_pitch 没有意义。
*/
IMAGE_API void image_convert_bmp_index4_rle(img_ix width, img_ix height,
											pixel_convert_fun_t conv_fun,
											const img_u8 * pal, img_ix pal_stride,
											const img_u8 * src, img_ix /*src_stride*/, img_ix /*src_pitch*/,
											img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
											img_ix flags);
/**
* Bmp 文件的 rle index8 压缩方式，使用索引色。
* rle 压缩 src_stride 和 src_pitch 没有意义。
*/
IMAGE_API void image_convert_bmp_index8_rle(img_ix width, img_ix height,
											pixel_convert_fun_t conv_fun,
											const img_u8 * pal, img_ix pal_stride,
											const img_u8 * src, img_ix /*src_stride*/, img_ix /*src_pitch*/,
											img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
											img_ix flags);

VENUS_END
