#pragma once

VENUS_BEG

//////////////////////////////////////////////////////////////////////////
// Tga 文件(压缩)格式

enum tga_color_type_e : img_u8
{
	tga_color_type_rgb = 0,
	tga_color_type_index,
};

enum tga_format_e : img_u8
{
	tga_format_invalid = 0, // 文件中没有图像数据
	tga_format_index, // 未压缩的，索引 颜色
	tga_format_rgb, // 未压缩的，RGB 颜色
	tga_format_gray, // 未压缩的，灰度 颜色

	tga_format_rle_index = 9, // 游程编码的，索引 颜色
	tga_format_rle_rgb, // 游程编码的，RGB 颜色
	tga_format_rle_gray, // 游程编码的，灰度 颜色

	// 以下还未提供支持
	tga_format_hf_rle_index = 32,  // 哈夫曼 游程编码的，索引 颜色
	tga_format_hf_rle_rgb,  // 哈夫曼 游程编码的，RGB 颜色
};

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
/// Tga 文件头
typedef IMAGE_API struct tag_tga_header_coord
{
	img_u8 low;
	img_u8 high;
}tga_header_short_t;

typedef IMAGE_API struct tag_tga_header
{
	img_u8 info_length;
	tga_color_type_e color_type;
	tga_format_e commpress;
	tga_header_short_t color_entry;
	img_u16 color_count;
	img_u8 color_bit;
	union
	{
		tga_header_short_t x_orgin;
		img_u16 x;
	};
	union
	{
		tga_header_short_t y_orgin;
		img_u16 y;
	};
	img_u16 width;
	img_u16 height;
	img_u8 bit_count;
	img_u8 flags;
}tga_header_t;

// 标记文件是按照从下到上存储的。
const img_u32 TGAF_FLIPY = 0x20;
// 文件尾
const img_u8 TGA_TAIL[] = "TRUEVISION-XFILE";

typedef IMAGE_API struct tag_tga_tailer
{
	img_u32 extension_offset;
	img_u32 developer_offset;
	img_u8 signature[18];
}tga_tailer_t;

#pragma pack(pop)

typedef IMAGE_API struct tag_tga_image_data : public image_data_t
{
	tga_format_e format;
}tga_image_data_t;


IMAGE_API bool is_tga_data(const img_u8 * buffer, img_ix length);
IMAGE_API img_ix tga_create(const img_u8 * buffer, img_ix length, image_data_t * img,
						  image_convert_rule_fun_t pfn_match = nullptr, void * user_data = nullptr);

IMAGE_API bool tga_rule_default(image_convert_rule_t * rule);

IMAGE_API void image_convert_tga_rle8(img_ix width, img_ix height,
									  pixel_convert_fun_t conv_fun,
									  const img_u8 * pal, img_ix pal_stride,
									  const img_u8 * src, img_ix src_stride, img_ix src_pitch,
									  img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
									  img_ix flags);

IMAGE_API err_t tga_save(const image_data_t * data, image_save_write_fun_t pfn_write, void * userdata);
IMAGE_API err_t tga_save_ex(const image_data_t * data, image_save_write_fun_t pfn_write, void * userdata, tga_format_e format);

VENUS_END
