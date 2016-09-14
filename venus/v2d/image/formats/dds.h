#pragma once

VENUS_BEG

#define DDS_MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
	((img_u32)(img_u8)(ch0) | ((img_u32)(img_u8)(ch1) << 8) | \
	((img_u32)(img_u8)(ch2) << 16) | ((img_u32)(img_u8)(ch3) << 24))

const img_u8 DDS_HEADER[4] = {0x44, 0x44, 0x53, 0x20};
const img_u32 DDS_MAGIC = DDS_MAKEFOURCC('D', 'D', 'S', ' ');

enum dxt_format_e
{
	dxt_format_rgb = -1,
	dxt_format_invalid = 0,
	// bc1
	dxt_format_dxt1 = DDS_MAKEFOURCC('D', 'X', 'T', '1'),
	// bc2
	dxt_format_dxt2 = DDS_MAKEFOURCC('D', 'X', 'T', '2'),
	dxt_format_dxt3 = DDS_MAKEFOURCC('D', 'X', 'T', '3'),
	// bc3
	dxt_format_dxt4 = DDS_MAKEFOURCC('D', 'X', 'T', '4'),
	dxt_format_dxt5 = DDS_MAKEFOURCC('D', 'X', 'T', '5'),
	// bc4
	dxt_format_bc4s = DDS_MAKEFOURCC('B', 'C', '4', 'S'),
	dxt_format_bc4u = DDS_MAKEFOURCC('B', 'C', '4', 'U'),
	dxt_format_ati1 = DDS_MAKEFOURCC('A', 'T', 'I', '1'),
	// bc5
	dxt_format_bc5s = DDS_MAKEFOURCC('B', 'C', '5', 'S'),
	dxt_format_bc5u = DDS_MAKEFOURCC('B', 'C', '5', 'U'),
	dxt_format_ati2 = DDS_MAKEFOURCC('A', 'T', 'I', '2'),
	// dx10
	dxt_format_dx10 = DDS_MAKEFOURCC('D', 'X', '1', '0'),



	dxt_format_a16b16g16r16 = 36,

	dxt_format_r16f = 111,
	dxt_format_g16r16f = 112,
	dxt_format_a16b16g16r16f = 113,
	dxt_format_r32f = 114,
	dxt_format_g32r32f = 115,
	dxt_format_a32b32g32r32f = 116,
};

enum dxgi_format_e
{
	dxgi_format_unknown = 0,
	dxgi_format_r32g32b32a32_typeless = 1,
	dxgi_format_r32g32b32a32_float = 2,
	dxgi_format_r32g32b32a32_uint = 3,
	dxgi_format_r32g32b32a32_sint = 4,
	dxgi_format_r32g32b32_typeless = 5,
	dxgi_format_r32g32b32_float = 6,
	dxgi_format_r32g32b32_uint = 7,
	dxgi_format_r32g32b32_sint = 8,
	dxgi_format_r16g16b16a16_typeless = 9,
	dxgi_format_r16g16b16a16_float = 10,
	dxgi_format_r16g16b16a16_unorm = 11,
	dxgi_format_r16g16b16a16_uint = 12,
	dxgi_format_r16g16b16a16_snorm = 13,
	dxgi_format_r16g16b16a16_sint = 14,
	dxgi_format_r32g32_typeless = 15,
	dxgi_format_r32g32_float = 16,
	dxgi_format_r32g32_uint = 17,
	dxgi_format_r32g32_sint = 18,
	dxgi_format_r32g8x24_typeless = 19,
	dxgi_format_d32_float_s8x24_uint = 20,
	dxgi_format_r32_float_x8x24_typeless = 21,
	dxgi_format_x32_typeless_g8x24_uint = 22,
	dxgi_format_r10g10b10a2_typeless = 23,
	dxgi_format_r10g10b10a2_unorm = 24,
	dxgi_format_r10g10b10a2_uint = 25,
	dxgi_format_r11g11b10_float = 26,
	dxgi_format_r8g8b8a8_typeless = 27,
	dxgi_format_r8g8b8a8_unorm = 28,
	dxgi_format_r8g8b8a8_unorm_srgb = 29,
	dxgi_format_r8g8b8a8_uint = 30,
	dxgi_format_r8g8b8a8_snorm = 31,
	dxgi_format_r8g8b8a8_sint = 32,
	dxgi_format_r16g16_typeless = 33,
	dxgi_format_r16g16_float = 34,
	dxgi_format_r16g16_unorm = 35,
	dxgi_format_r16g16_uint = 36,
	dxgi_format_r16g16_snorm = 37,
	dxgi_format_r16g16_sint = 38,
	dxgi_format_r32_typeless = 39,
	dxgi_format_d32_float = 40,
	dxgi_format_r32_float = 41,
	dxgi_format_r32_uint = 42,
	dxgi_format_r32_sint = 43,
	dxgi_format_r24g8_typeless = 44,
	dxgi_format_d24_unorm_s8_uint = 45,
	dxgi_format_r24_unorm_x8_typeless = 46,
	dxgi_format_x24_typeless_g8_uint = 47,
	dxgi_format_r8g8_typeless = 48,
	dxgi_format_r8g8_unorm = 49,
	dxgi_format_r8g8_uint = 50,
	dxgi_format_r8g8_snorm = 51,
	dxgi_format_r8g8_sint = 52,
	dxgi_format_r16_typeless = 53,
	dxgi_format_r16_float = 54,
	dxgi_format_d16_unorm = 55,
	dxgi_format_r16_unorm = 56,
	dxgi_format_r16_uint = 57,
	dxgi_format_r16_snorm = 58,
	dxgi_format_r16_sint = 59,
	dxgi_format_r8_typeless = 60,
	dxgi_format_r8_unorm = 61,
	dxgi_format_r8_uint = 62,
	dxgi_format_r8_snorm = 63,
	dxgi_format_r8_sint = 64,
	dxgi_format_a8_unorm = 65,
	dxgi_format_r1_unorm = 66,
	dxgi_format_r9g9b9e5_sharedexp = 67,
	dxgi_format_r8g8_b8g8_unorm = 68,
	dxgi_format_g8r8_g8b8_unorm = 69,
	dxgi_format_bc1_typeless = 70,
	dxgi_format_bc1_unorm = 71,
	dxgi_format_bc1_unorm_srgb = 72,
	dxgi_format_bc2_typeless = 73,
	dxgi_format_bc2_unorm = 74,
	dxgi_format_bc2_unorm_srgb = 75,
	dxgi_format_bc3_typeless = 76,
	dxgi_format_bc3_unorm = 77,
	dxgi_format_bc3_unorm_srgb = 78,
	dxgi_format_bc4_typeless = 79,
	dxgi_format_bc4_unorm = 80,
	dxgi_format_bc4_snorm = 81,
	dxgi_format_bc5_typeless = 82,
	dxgi_format_bc5_unorm = 83,
	dxgi_format_bc5_snorm = 84,
	dxgi_format_b5g6r5_unorm = 85,
	dxgi_format_b5g5r5a1_unorm = 86,
	dxgi_format_b8g8r8a8_unorm = 87,
	dxgi_format_b8g8r8x8_unorm = 88,
	dxgi_format_r10g10b10_xr_bias_a2_unorm = 89,
	dxgi_format_b8g8r8a8_typeless = 90,
	dxgi_format_b8g8r8a8_unorm_srgb = 91,
	dxgi_format_b8g8r8x8_typeless = 92,
	dxgi_format_b8g8r8x8_unorm_srgb = 93,
	dxgi_format_bc6h_typeless = 94,
	dxgi_format_bc6h_uf16 = 95,
	dxgi_format_bc6h_sf16 = 96,
	dxgi_format_bc7_typeless = 97,
	dxgi_format_bc7_unorm = 98,
	dxgi_format_bc7_unorm_srgb = 99,
	dxgi_format_ayuv = 100,
	dxgi_format_y410 = 101,
	dxgi_format_y416 = 102,
	dxgi_format_nv12 = 103,
	dxgi_format_p010 = 104,
	dxgi_format_p016 = 105,
	dxgi_format_420_opaque = 106,
	dxgi_format_yuy2 = 107,
	dxgi_format_y210 = 108,
	dxgi_format_y216 = 109,
	dxgi_format_nv11 = 110,
	dxgi_format_ai44 = 111,
	dxgi_format_ia44 = 112,
	dxgi_format_p8 = 113,
	dxgi_format_a8p8 = 114,
	dxgi_format_b4g4r4a4_unorm = 115,
	dxgi_format_force_uint = 0XFFFFFFFF
};

enum dds_dimension_e
{
	dds_dimension_unknown = 0,
	dds_dimension_buffer = 1,
	dds_dimension_texture1d = 2,
	dds_dimension_texture2d = 3,
	dds_dimension_texture3d = 4
};

#pragma pack(push, 1)
struct dds_color_key_t
{
	img_u32 low;
	img_u32 high;
};

struct dds_pixel_format_t
{
	img_u32 size;
	img_u32 flags;
	dxt_format_e dxt_format : 32;
	img_u32 bit_count;
	color_mask_abgr_t color_mask;
};

struct dds_caps_t
{
	img_u32 surface_flags;
	img_u32 cubemap_flags;
	img_u32 cap3;
	img_u32 cap4;
};

struct dds_header_t
{
	img_u32 magic;
	img_u32 size;
	img_u32 flags;
	img_u32 height;
	img_u32 width;
	union
	{
		img_32 pitch;
		img_u32 linear_size;
	};
	union
	{
		img_u32 depth;
		img_u32 back_buffer_count;
	};
	img_u32 mipmap_count;
	union
	{
		img_u32 reserved1[11];
		struct
		{
			img_u32 alpha_bit_depth;
			img_u32 reserverd;
			img_u32 lpsurface;
			union
			{
				dds_color_key_t dst_overlay;
				img_u32 empty_face_color;
			};
			dds_color_key_t dst_blt;
			dds_color_key_t src_overlay;
			dds_color_key_t srcblt;;
		};
	};
	dds_pixel_format_t pixel_format;
	dds_caps_t caps;
	img_u32 texture_stage;
};

struct dds_header_10_t
{
	dxgi_format_e dxgi_format;
	dds_dimension_e dimension;
	img_u32 misc_flag;
	img_u32 array_size;
	img_u32 reserved;
};

struct dds_texel_bc1_t
{
	img_u16 color0;
	img_u16 color1;
	img_u32 indices;
};

struct dds_texel_bc2_t
{
	img_u16 alphas[4];
	img_u16 color0;
	img_u16 color1;
	img_u32 indices;
};

struct dds_texel_bc3_t
{
	struct
	{
		img_u8 alpha0;
		img_u8 alpha1;

		img_u16 index0;
		img_u16 index1;
		img_u16 index2;
	};
	img_u16 color0;
	img_u16 color1;
	img_u32 indices;
};

struct dxt1_info_t
{
	color_rgb_t colors[4];
};

// 用于 dxt 解压
struct dxt_info_t
{
	union
	{
		color_rgb_t rgbs[4];
		color_argb_t argbs[4];
	};
	img_u8 alphas[8];
	img_u8 alpha_indices[4][4];
};

struct dxt_pixel_a16r16g16b16f_t
{
	img_u16 r;
	img_u16 g;
	img_u16 b;
	img_u16 a;
};

struct dxt_pixel_a32r32g32b32f_t
{
	img_f32 r;
	img_f32 g;
	img_f32 b;
	img_f32 a;
};

struct dxt_pixel_g16r16f_t
{
	img_u16 r;
	img_u16 g;
};

struct dxt_pixel_g32r32f_t
{
	img_f32 r;
	img_f32 g;
};
#pragma pack(pop)

struct dds_image_data_t : public image_data_t
{
};

//////////////////////////////////////////////////////////////////////////
/// dxt 压缩
/// 

IMAGE_API void dds_convert_bc1(img_ix width, img_ix height,
								pixel_convert_fun_t conv_fun,
								const img_u8 * /*pal*/, img_ix /*pal_stride*/,
								const img_u8 * src, img_ix /*src_stride*/, img_ix /*src_pitch*/,
								img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
								img_ix flags);

IMAGE_API void dds_convert_bc2(img_ix width, img_ix height,
								 pixel_convert_fun_t conv_fun,
								 const img_u8 * /*pal*/, img_ix /*pal_stride*/,
								 const img_u8 * src, img_ix /*src_strike*/, img_ix /*src_pitch*/,
								 img_u8 * dst, img_ix dst_strike, img_ix dst_pitch,
								 img_ix flags);

IMAGE_API void dds_convert_bc3(img_ix width, img_ix height,
								 pixel_convert_fun_t conv_fun,
								 const img_u8 * /*pal*/, img_ix /*pal_stride*/,
								 const img_u8 * src, img_ix src_strike, img_ix src_pitch,
								 img_u8 * dst, img_ix dst_strike, img_ix dst_pitch,
								 img_ix flags);

IMAGE_API void dds_convert_copy_dxt1(img_ix width, img_ix height,
									 pixel_convert_fun_t conv_fun,
									 const img_u8 * /*pal*/, img_ix /*pal_stride*/,
									 const img_u8 * src, img_ix src_stride, img_ix src_pitch,
									 img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
									 img_ix flags);

IMAGE_API void dds_convert_copy_dxt23(img_ix width, img_ix height,
									  pixel_convert_fun_t conv_fun,
									  const img_u8 * /*pal*/, img_ix /*pal_stride*/,
									  const img_u8 * src, img_ix src_strike, img_ix src_pitch,
									  img_u8 * dst, img_ix dst_strike, img_ix dst_pitch,
									  img_ix flags);

IMAGE_API void dds_convert_copy_dxt45(img_ix width, img_ix height,
									  pixel_convert_fun_t conv_fun,
									  const img_u8 * /*pal*/, img_ix /*pal_stride*/,
									  const img_u8 * src, img_ix src_strike, img_ix src_pitch,
									  img_u8 * dst, img_ix dst_strike, img_ix dst_pitch,
									  img_ix flags);


IMAGE_API void color_dds_a4r5g6b5_to_r5g5b5(const void * src_pixel, void * dst_pixel);
IMAGE_API void color_dds_a4r5g6b5_to_r8g8b8(const void * src_pixel, void * dst_pixel);

IMAGE_API void color_dds_a4r5g6b5_to_a8r8g8b8(const void * src_pixel, void * dst_pixel); //dxt2 dxt3
IMAGE_API void color_dds_a8r5g6b5_to_a8r8g8b8(const void * src_pixel, void * dst_pixel); //dxt4 dxt5

//////////////////////////////////////////////////////////////////////////
/// dds_header_t.flags 位掩码

/// caps 有效（默认有此结构）
const img_u32 DDSF_CAPS = 0x00000001;

/// height 有效
const img_u32 DDSF_HEIGHT = 0x00000002;

/// width 有效
const img_u32 DDSF_WIDTH = 0x00000004;

/// pitch 有效
const img_u32 DDSF_PITCH = 0x00000008;

/// back_buffer_count 有效
const img_u32 DDSF_BACKBUFFERCOUNT = 0x00000020;

/// z_buffer_bit_depth 有效（弃用）
const img_u32 DDSF_ZBUFFERBITDEPTH = 0x00000040;

/// alpha_bit_depth 有效
const img_u32 DDSF_ALPHA_BIT_DEPTH = 0x00000080;

/// lpsurface 有效
const img_u32 DDSF_LPSURFACE = 0x00000800;

/// pixel_format 有效
const img_u32 DDSF_PIXEL_FORMAT = 0x00001000;

/// dst_overlay 有效
const img_u32 DDSF_DST_OVERLAY = 0x00002000;

/// dst_blt 有效
const img_u32 DDSF_DST_BLT = 0x00004000;

/// src_overlay 有效
const img_u32 DDSF_SRC_OVERLAY = 0x00008000;

/// src_blt 有效
const img_u32 DDSF_SRC_BLT = 0x00010000;

/// mipmap_count 有效
const img_u32 DDSF_MIPMAP_COUNT = 0x00020000;

/// refresh_rate 有效
const img_u32 DDSF_REFRESH_RATE = 0x00040000;

/// linear_size 有效
const img_u32 DDSF_LINEAR_SIZE = 0x00080000;

/// texture_stage 有效
const img_u32 DDSF_TEXTURE_STAGE = 0x00100000;

/// fvf 有效
const img_u32 DDSF_FVF = 0x00200000;

/// src_vb_handle 有效
const img_u32 DDSF_SRC_VB_HANDLE = 0x00400000;

/// depth 有效
const img_u32 DDSF_VOLUME = 0x00800000;



//////////////////////////////////////////////////////////////////////////
/// dds_pixel_format_t.flags 位掩码

/// 有 Alpha 通道
const img_u32 DDSPF_ALPHA = 0x00000001;

/// 只有 Alpha 通道
const img_u32 DDSPF_ALPHA_ONLY = 0x00000002;

/// 使用“四字符编码”
const img_u32 DDSPF_FOURCC = 0x00000004;

/// 使用 四位颜色索引
const img_u32 DDSPF_INDEX_4 = 0x00000008;

/// The surface is indexed into a palette which stores indices into the destination surface's 8-bit palette.
const img_u32 DDSPF_INDEX_TO_8 = 0x00000010;

/// 使用 八位颜色索引
const img_u32 DDSPF_INDEX_8 = 0x00000020;

/// 使用 RGB 颜色
const img_u32 DDSPF_RGB = 0x00000040;

/// 内容是经过压缩的
const img_u32 DDSPF_COMPRESSED = 0x00000080;

/*
* The surface will accept RGB data and translate it during the write to YUV data.
* The format of the data to be written will be contained in the pixel format structure.
* The DDPF_RGB flag will be set.
*/
const img_u32 DDSPF_RGB_TO_YUV = 0x000000100;

/// 使用 YUV 颜色
const img_u32 DDSPF_YUV = 0x000000200;

/// 只拥有 Z-Buffer 信息
const img_u32 DDSPF_Z_BUFFER = 0x000000400;



/// 使用 一位颜色索引
const img_u32 DDSPF_INDEX_1 = 0x000000800;

/// 使用 二位颜色索引
const img_u32 DDSPF_INDEX_2 = 0x000001000;

/// 包含 深度信息
const img_u32 DDSPF_Z_INFO = 0x000002000;

/// 包含 与 深度信息相关的模板信息 The surface contains stencil information along with Z
const img_u32 DDSPF_Z_STENCIL = 0x000004000;

/// 颜色信息已经与 Alpha 做预乘
const img_u32 DDSPF_ALPHA_PREMULT = 0x000008000;

/// 包含 亮度信息
/// Luminance data in the pixel format is valid.
/// Use this flag for luminance-only or luminance+alpha surfaces,
/// the bit depth is then ddpf.dwLuminanceBitCount.
const img_u32 DDSPF_LUMINANCE = 0x000020000;


/// Luminance data in the pixel format is valid. Use this flag when hanging luminance off bumpmap surfaces,
/// the bit mask for the luminance portion of the pixel is then ddpf.dwBumpLuminanceBitMask
const img_u32 DDSPF_BUMPLUMINANCE = 0x00040000;


/// Bump map dUdV data in the pixel format is valid.
const img_u32 DDSPF_BUMPDUDV = 0x00080000;


//////////////////////////////////////////////////////////////////////////
/// dds_caps_t.caps1 位掩码
/// 
const img_u32 DDS_CAPS_TEXTURE = 0x00080000;

const img_u32 DDS_CAPS2_CUBE = 0x200;




//////////////////////////////////////////////////////////////////////////
/// 不用了的
void dds_dxt1(dds_header_t * header, img_u8 * buffer, dds_image_data_t * dds);
void dds_dxt2(dds_header_t * header, img_u8 * buffer, dds_image_data_t * dds);
void dds_dxt3(dds_header_t * header, img_u8 * buffer, dds_image_data_t * dds);
void dds_dxt4(dds_header_t * header, img_u8 * buffer, dds_image_data_t * dds);
void dds_dxt5(dds_header_t * header, img_u8 * buffer, dds_image_data_t * dds);

IMAGE_API bool is_dds_data(const img_u8 * buffer, img_ix length);
IMAGE_API img_ix dds_create(const img_u8 * buffer, img_ix length, image_data_t * img,
							image_convert_rule_fun_t pfn_match = nullptr, void * user_data = nullptr);
IMAGE_API cmode_e dds_get_cmode(const dds_pixel_format_t & pixel_format);
IMAGE_API cmode_e dds_get_cmode(dxgi_format_e format);
IMAGE_API void dds_get_pitch(cmode_e cmode, img_ix width, img_ix height, img_ix * row, img_ix * col, img_ix * pitch);
IMAGE_API bool dds_rule_default(image_convert_rule_t * rule);

VENUS_END
