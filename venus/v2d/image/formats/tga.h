#pragma once

VENUS_BEG

//////////////////////////////////////////////////////////////////////////
// Tga �ļ�(ѹ��)��ʽ

enum tga_color_type_e : img_u8
{
	tga_color_type_rgb = 0,
	tga_color_type_index,
};

enum tga_format_e : img_u8
{
	tga_format_invalid = 0, // �ļ���û��ͼ������
	tga_format_index, // δѹ���ģ����� ��ɫ
	tga_format_rgb, // δѹ���ģ�RGB ��ɫ
	tga_format_gray, // δѹ���ģ��Ҷ� ��ɫ

	tga_format_rle_index = 9, // �γ̱���ģ����� ��ɫ
	tga_format_rle_rgb, // �γ̱���ģ�RGB ��ɫ
	tga_format_rle_gray, // �γ̱���ģ��Ҷ� ��ɫ

	// ���»�δ�ṩ֧��
	tga_format_hf_rle_index = 32,  // ������ �γ̱���ģ����� ��ɫ
	tga_format_hf_rle_rgb,  // ������ �γ̱���ģ�RGB ��ɫ
};

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
/// Tga �ļ�ͷ
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

// ����ļ��ǰ��մ��µ��ϴ洢�ġ�
const img_u32 TGAF_FLIPY = 0x20;
// �ļ�β
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
