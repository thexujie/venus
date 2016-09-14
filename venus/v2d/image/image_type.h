#pragma once

VENUS_BEG

typedef uint_8 img_u8;
typedef int_8 img_8;
typedef int_16 img_16;
typedef int_32 img_32;
typedef int_64 img_64;

typedef uint_16 img_u16;
typedef uint_32 img_u32;
typedef uint_64 img_u64;

#ifndef IMAGE_BIT_64
typedef img_32 img_ix;
typedef img_u32 img_ux;
#else
typedef img_64 img_ix;
typedef img_u64 img_ux;
#endif

typedef float_32 img_f32;

typedef char_x img_cx;

/**
 * 图像格式   
 */
typedef enum tag_image_format_e
{
	image_format_invalid = 0,
	image_format_bmp = make_fourcc('b', 'm', 'p', 0),
	image_format_tga = make_fourcc('t', 'g', 'a', 0),
	image_format_png = make_fourcc('p', 'n', 'g', 0),
	image_format_jpg = make_fourcc('j', 'p', 'g', 0),
	image_format_dds = make_fourcc('d', 'd', 's', 0),
	image_format_user = make_fourcc('u', 's', 'e', 'r'),
}image_format_e;

//////////////////////////////////////////////////////////////////////////
/// 颜色模式
typedef enum tag_cmode
{
	cmode_invalid = 0,
	cmode_gray1,
	cmode_gray2,
	cmode_gray4,
	cmode_gray8,

	cmode_r3g3b2,

	//---------------------------------16 bits
	cmode_r8g8,
	cmode_r5g6b5,
	cmode_b5g6r5,
	cmode_a1r5g5b5,
	cmode_a1b5g5r5,
	cmode_x1r5g5b5,
	cmode_x1b5g5r5,
	cmode_a4r4g4b4,
	cmode_x4r4g4b4,
	cmode_a8r3g3b2,

	//---------------------------------24 bits
	cmode_r8g8b8,
	cmode_b8g8r8,
	cmode_a8r5g6b5,
	cmode_a8b5g6r5,
	cmode_a8x1r5g5b5,
	cmode_a8x1b5g5r5,

	//---------------------------------32 bits
	cmode_a8r8g8b8,
	cmode_x8r8g8b8,
	cmode_a8b8g8r8,
	cmode_x8b8g8r8,
	cmode_r8g8b8a8,
	cmode_r8g8b8x8,
	cmode_b8g8r8a8,
	cmode_b8g8r8x8,

	cmode_g16r16,

	cmode_a2r10g10b10,
	cmode_a2b10g10r10,

	//---------------------------------
	cmode_a16b16g16r16,

	cmode_r16f,
	cmode_g16r16f,
	cmode_a16b16g16r16f,
	cmode_r32f,
	cmode_g32r32f,
	cmode_b32g32r32f,
	cmode_a32b32g32r32f,

	cmode_x32y32z32f,

	cmode_index4_rle2_x8r8g8b8,
	cmode_index8_rle2_x8r8g8b8,

	cmode_rle_gray8,
	cmode_rle_x1r5g5b5,
	cmode_rle_r8g8b8,
	cmode_rle_a8r8g8b8,

	cmode_index1_x8r8g8b8,
	cmode_index2_x8r8g8b8,

	cmode_index4_r8g8b8,
	cmode_index4_x8r8g8b8,
	cmode_index4_a8r8g8b8,

	cmode_index8_gray8,
	cmode_index8_x1r5g5b5,
	cmode_index8_r8g8b8,
	cmode_index8_x8r8g8b8,
	cmode_index8_a8r8g8b8,

	cmode_yuv,
	cmode_cmyk,
	cmode_ycck,

	cmode_bc1, // r5g6b5
	cmode_bc2, // a8r5g6b5
	cmode_bc3, // a8r5g6b5
	cmode_bc4,
	cmode_bc5,
	cmode_dx10,


	cmode_d24s8,
	//cmode_dx100 = cmode_dx1 + 99, // keep..
}cmode_e;

IMAGE_API const img_cx * cmode_text(cmode_e cmode);

/**
 * 只能处理 argb 格式。
 */
IMAGE_API int_x cmode_bits(cmode_e cmode);

typedef IMAGE_API struct tag_image_data
{
	img_ix width;
	img_ix height;
	img_ix bits;
	img_ix pitch;
	img_ix length;
	cmode_e src_mode;
	cmode_e dst_mode;
	img_ix flags;

	img_u8 * buffer;
}image_data_t;

IMAGE_API bool image_data_malloc(image_data_t * data);
IMAGE_API void image_data_free(image_data_t * data);

#pragma pack(push, 1)

typedef IMAGE_API struct tag_image_frame
{
	img_ix width;
	img_ix height;
	img_ix pitch;
	img_ix bits;
	img_ix length;
	cmode_e mode;
	const img_u8 * buffer;
}image_frame_t;

typedef IMAGE_API struct tag_color_yuv
{
	img_u8 y;
	img_u8 u;
	img_u8 v;
}color_yuv_t;

typedef IMAGE_API struct tag_color_ycck
{
	union
	{
		img_u32 color;
		struct
		{
			img_u8 y;
			img_u8 b;
			img_u8 r;
			img_u8 k;
		};
	};
}color_ycck_t;

typedef IMAGE_API struct tag_color_rgb
{
	img_u8 b;
	img_u8 g;
	img_u8 r;
}color_rgb_t, color_r8g8b8_t;

typedef IMAGE_API struct tag_color_bgr
{
	img_u8 r;
	img_u8 g;
	img_u8 b;
}color_bgr_t, color_b8g8r8_t;

typedef IMAGE_API struct tag_color_abgr
{
	union
	{
		img_u32 color;
		struct
		{
			img_u8 r;
			img_u8 g;
			img_u8 b;
			img_u8 a;
		};
	};
}color_abgr_t, color_a8b8g8r8_t;

typedef IMAGE_API struct color_argb_t
{
	union
	{
		img_u32 color;
		struct
		{
			union
			{
				color_rgb_t rgb;
				struct
				{
					img_u8 b;
					img_u8 g;
					img_u8 r;
				};
			};
			img_u8 a;
		};
	};
}color_a8r8g8b8_t, color_xrgb_t, color_x8r8g8b8_t;


typedef IMAGE_API struct tag_color_r5g6b5
{
	union
	{
		img_u16 color;
		struct
		{
			img_u16 b : 5;
			img_u16 g : 6;
			img_u16 r : 5;
		};
	};
}color_r5g6b5;

typedef IMAGE_API struct tag_color_bgra
{
	union
	{
		img_u32 color;
		struct
		{
			img_u8 a;
			union
			{
				color_bgr_t bgr;
				struct
				{
					img_u8 r;
					img_u8 g;
					img_u8 b;
				};
			};
		};
	};
}color_bgra_t, color_b8g8r8a8_t, color_xbgr_t, color_x8b8g8r8_t;

typedef IMAGE_API struct tag_color_rgba
{
	union
	{
		img_u32 color;
		struct
		{
			union
			{
				img_u8 a;
				img_u8 x;
			};
			union
			{
				color_rgb_t rgb;
				struct
				{
					img_u8 b;
					img_u8 g;
					img_u8 r;
				};
			};
		};
	};
}color_rgba_t, color_r8g8b8a8_t, color_rgbx_t, color_r8g8b8x8_t;

typedef IMAGE_API struct tag_color_cmyk
{
	img_u8 c;
	img_u8 m;
	img_u8 y;
	img_u8 k;
}color_cmyk_t;


typedef IMAGE_API struct tag_color_a16r16g16b16f
{
	img_u16 r;
	img_u16 g;
	img_u16 b;
	img_u16 a;
}color_a16b16g16r16f_t, color_a16b16g16r16_t;

typedef IMAGE_API struct tag_color_g16r16f
{
	img_u16 r;
	img_u16 g;
}color_g16r16f_t;

typedef IMAGE_API struct tag_color_g32r32f
{
	img_f32 r;
	img_f32 g;
}color_g32r32f_t;

typedef IMAGE_API struct tag_color_a32b32g32r32f
{
	img_f32 r;
	img_f32 g;
	img_f32 b;
	img_f32 a;
}color_a32b32g32r32f_t;
#pragma pack(pop)

IMAGE_API img_ix align_to_2(img_ix val);
IMAGE_API img_ix align_to_4(img_ix val);

typedef IMAGE_API struct tag_color_x32y32z32f
{
	img_f32 x;
	img_f32 y;
	img_f32 z;
}color_x32y32z32f_t;

//////////////////////////////////////////////////////////////////////////
/// 颜色掩码
typedef IMAGE_API struct tag_color_mask
{
	img_u32 r;
	img_u32 g;
	img_u32 b;
	img_u32 a;
}color_mask_abgr_t;

const color_mask_abgr_t MASK_R3G3B2 = {0x00E0, 0x001C, 0x0003, 0x0000};

const color_mask_abgr_t MASK_A8R3G3B2 = {0x00E0, 0x001C, 0x0003, 0xFF00};

const color_mask_abgr_t MASK_A4R4G4B4 = {0x0F00, 0x00F0, 0x000F, 0xF000};
const color_mask_abgr_t MASK_X4R4G4B4 = {0x0F00, 0x00F0, 0x000F, 0x0000};

const color_mask_abgr_t MASK_A0R5G6B5 = {0xF800, 0x07E0, 0x001F, 0x0000};

const color_mask_abgr_t MASK_X1R5G5B5 = {0x7C00, 0x03E0, 0x001F, 0x0000};
const color_mask_abgr_t MASK_A1R5G5B5 = {0x7C00, 0x03E0, 0x001F, 0x8000};

const color_mask_abgr_t MASK_R8G8B8 = {0x00FF0000, 0x0000FF00, 0x000000FF, 0x00000000};
const color_mask_abgr_t MASK_B8G8R8 = {0x000000FF, 0x0000FF00, 0x00FF0000, 0x00000000};

const color_mask_abgr_t MASK_X8R8G8B8 = MASK_R8G8B8;
const color_mask_abgr_t MASK_A8R8G8B8 = {0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000};

const color_mask_abgr_t MASK_X8B8G8R8 = MASK_B8G8R8;
const color_mask_abgr_t MASK_A8B8G8R8 = {0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000};

const color_mask_abgr_t MASK_R8G8B8X8 = {0xFF000000, 0x00FF0000, 0x0000FF00, 0x00000000};
const color_mask_abgr_t MASK_R8G8B8A8 = {0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF};

const color_mask_abgr_t MASK_G16R16 = {0x0000FFFF, 0xFFFF0000, 0x00000000, 0x00000000};

const color_mask_abgr_t MASK_A2R10G10B10 = {0x3FF00000, 0x000FFC00, 0x000003FF, 0xC0000000};
const color_mask_abgr_t MASK_A2B10G10R10 = {0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000};
// 根据颜色掩码计算颜色类型
cmode_e cmode_from_mask_abgr(const color_mask_abgr_t & mask, img_ix bits);


//////////////////////////////////////////////////////////////////////////
/// 以下用于支持颜色之间的相互转换
typedef void (* pixel_convert_fun_t)(const void * src_pixel, void * dst_pixel);

// - 复制
IMAGE_API void color_8_to_8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_16_to_16(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_24_to_24(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_32_to_32(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_32_to_32_ex0(const void * src_pixel, void * dst_pixel);

IMAGE_API void color_r3g3b2_to_x1r5g5b5(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_r3g3b2_to_a1r5g5b5(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_r3g3b2_to_x8r8g8b8(const void * src_pixel, void * dst_pixel);

IMAGE_API void color_a8r3g3b2_to_a8r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_a8r3g3b2_to_a8b8g8r8(const void * src_pixel, void * dst_pixel);

IMAGE_API void color_r8g8b8_to_b8g8r8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_r8g8b8_to_x8r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_r8g8b8_to_r8g8b8a8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_a8r8g8b8_to_r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_a8b8g8r8_to_a8r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_x8b8g8r8_to_r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_x8r8g8b8_to_r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_x8r8g8b8_to_x1r5g5b5(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_x8r8g8b8_to_r5g6b5(const void * src_pixel, void * dst_pixel);

IMAGE_API void color_r5g6b5_to_x1r5g5b5(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_r5g6b5_to_r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_r5g6b5_to_x8r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_r5g6b5_to_x8b8g8r8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_b5g6r5_to_r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_b5g6r5_to_x8r8g8b8(const void * src_pixel, void * dst_pixel);


IMAGE_API void color_a1r5g5b5_to_x1r5g5b5(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_a1r5g5b5_to_a8r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_a1r5g6b5_to_a8r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_a4r5g6b5_to_a8r8g8b8(const void * src_pixel, void * dst_pixel);

IMAGE_API void color_x4r4g4b4_to_x1r5g5b5(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_x4r4g4b4_to_x8r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_x4r4g4b4_to_x8b8g8r8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_a4r4g4b4_to_a8r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_a4r4g4b4_to_a8b8g8r8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_a4r4g4b4_to_x8b8g8r8(const void * src_pixel, void * dst_pixel);

IMAGE_API void color_a16b16g16r16_to_a8r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_a16b16g16r16f_to_a8r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_g16r16_to_r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_g16r16f_to_r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_r16f_to_r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_r32f_to_r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_g32r32f_to_r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_a32b32g32r32f_to_a8r8g8b8(const void * src_pixel, void * dst_pixel);

IMAGE_API void color_gray8_to_x8r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_gray8_to_r8g8b8(const void * src_pixel, void * dst_pixel);

IMAGE_API void color_x1r5g5b5_to_a1r5g5b5(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_x1r5g5b5_to_r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_x1r5g5b5_to_x8r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_x1r5g5b5_to_x8b8g8r8(const void * src_pixel, void * dst_pixel);

IMAGE_API void color_a8r8g8b8_to_a8r8g8b8(const void * src_pixel, void * dst_pixel);

IMAGE_API void color_yuv_to_r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_r8g8b8_to_yuv(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_yuv_to_a8r8g8b8(const void * src_pixel, void * dst_pixel);

IMAGE_API void color_ycck_to_r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_ycck_to_a8r8g8b8(const void * src_pixel, void * dst_pixel);

IMAGE_API void color_gray_to_r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_gray_to_a8r8g8b8(const void * src_pixel, void * dst_pixel);

IMAGE_API void color_cmyk_to_r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_cmyk_to_a8r8g8b8(const void * src_pixel, void * dst_pixel);

IMAGE_API void color_r8g8b8_to_x32y32z32f(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_x32y32z32f_tor_r8g8b8(const void * src_pixel, void * dst_pixel);

IMAGE_API void color_r8g8b8x8_to_r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_r8g8b8x8_to_x8b8g8r8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_r8g8b8a8_to_b8g8r8a8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_a8r8g8b8_to_a8b8g8r8(const void * src_pixel, void * dst_pixel);


IMAGE_API void color_r8g8b8_to_a8b8g8r8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_r8g8b8_to_x8b8g8r8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_b8g8r8_to_a8b8g8r8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_b8g8r8_to_x8r8g8b8(const void * src_pixel, void * dst_pixel);

IMAGE_API void color_x8r8g8b8_to_x8b8g8r8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_x8b8g8r8_to_x8r8g8b8(const void * src_pixel, void * dst_pixel);


IMAGE_API void color_a2r10g10b10_to_a8r8g8b8(const void * src_pixel, void * dst_pixel);

IMAGE_API void color_a2b10g10r10_to_a8r8g8b8(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_a2b10g10r10_to_a2b10g10r10(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_a2b10g10r10_to_a2r10g10b10(const void * src_pixel, void * dst_pixel);

VENUS_END
