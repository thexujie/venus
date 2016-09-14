#include "stdafx.h"
#include "image_type.h"

VENUS_BEG

int_x cmode_bits(cmode_e cmode)
{
	switch(cmode)
	{
	case cmode_gray1: return 1;
	case cmode_gray2: return 2;
	case cmode_gray4: return 4;
	case cmode_gray8: return 8;

	case cmode_r3g3b2: 
		return 8;

	case cmode_r8g8b8:
	case cmode_b8g8r8:
		return 24;

	case cmode_r5g6b5:
	case cmode_a1r5g5b5:
	case cmode_a1b5g5r5:
	case cmode_x1r5g5b5:
	case cmode_x1b5g5r5:
	case cmode_a4r4g4b4:
	case cmode_x4r4g4b4:
	case cmode_a8r3g3b2:
		return 16;

	case cmode_a8r8g8b8:
	case cmode_x8r8g8b8:
	case cmode_a8b8g8r8:
	case cmode_x8b8g8r8:
	case cmode_r8g8b8a8:
	case cmode_r8g8b8x8:
	case cmode_b8g8r8a8:
	case cmode_b8g8r8x8:
	case cmode_g16r16:
	case cmode_a2r10g10b10:
	case cmode_a2b10g10r10:
		return 32;

	case cmode_a16b16g16r16:
		return 64;
	
	case cmode_r16f: return 16;
	case cmode_g16r16f: return 32;
	case cmode_a16b16g16r16f: return 64;
	case cmode_r32f: return 32;
	case cmode_g32r32f: return 64;
	case cmode_b32g32r32f: return 96;
	case cmode_a32b32g32r32f: return 128;

	case cmode_x32y32z32f: return 96;

	default:
		return 0;
	}
}

#define COLOR_MODE_TEXT(mode) case mode: return _T(#mode)
const img_cx * cmode_text(cmode_e cmode)
{
	switch(cmode)
	{
		COLOR_MODE_TEXT(cmode_invalid);
		COLOR_MODE_TEXT(cmode_gray2);
		COLOR_MODE_TEXT(cmode_gray4);
		COLOR_MODE_TEXT(cmode_gray8);

		COLOR_MODE_TEXT(cmode_r8g8b8);
		COLOR_MODE_TEXT(cmode_a8r8g8b8);
		COLOR_MODE_TEXT(cmode_x8r8g8b8);

		COLOR_MODE_TEXT(cmode_b8g8r8);
		COLOR_MODE_TEXT(cmode_a8b8g8r8);
		COLOR_MODE_TEXT(cmode_x8b8g8r8);

		COLOR_MODE_TEXT(cmode_yuv);
		COLOR_MODE_TEXT(cmode_cmyk);
		COLOR_MODE_TEXT(cmode_ycck);

		COLOR_MODE_TEXT(cmode_r5g6b5);
		COLOR_MODE_TEXT(cmode_a8r5g6b5);
		COLOR_MODE_TEXT(cmode_a1r5g5b5);
		COLOR_MODE_TEXT(cmode_a1b5g5r5);
		COLOR_MODE_TEXT(cmode_x1r5g5b5);
		COLOR_MODE_TEXT(cmode_a4r4g4b4);
		COLOR_MODE_TEXT(cmode_x4r4g4b4);
		COLOR_MODE_TEXT(cmode_g16r16);
		COLOR_MODE_TEXT(cmode_a16b16g16r16);
		COLOR_MODE_TEXT(cmode_r3g3b2);

		COLOR_MODE_TEXT(cmode_r8g8b8a8);
		COLOR_MODE_TEXT(cmode_r8g8b8x8);

		COLOR_MODE_TEXT(cmode_r16f);
		COLOR_MODE_TEXT(cmode_g16r16f);
		COLOR_MODE_TEXT(cmode_a16b16g16r16f);
		COLOR_MODE_TEXT(cmode_r32f);
		COLOR_MODE_TEXT(cmode_g32r32f);
		COLOR_MODE_TEXT(cmode_a32b32g32r32f);

		COLOR_MODE_TEXT(cmode_x32y32z32f);

		COLOR_MODE_TEXT(cmode_index4_a8r8g8b8);
		COLOR_MODE_TEXT(cmode_index8_a8r8g8b8);
		COLOR_MODE_TEXT(cmode_index1_x8r8g8b8);
		COLOR_MODE_TEXT(cmode_index2_x8r8g8b8);
		COLOR_MODE_TEXT(cmode_index4_x8r8g8b8);
		COLOR_MODE_TEXT(cmode_index8_x8r8g8b8);
		COLOR_MODE_TEXT(cmode_index4_rle2_x8r8g8b8);
		COLOR_MODE_TEXT(cmode_index8_rle2_x8r8g8b8);

		COLOR_MODE_TEXT(cmode_rle_gray8);
		COLOR_MODE_TEXT(cmode_rle_x1r5g5b5);
		COLOR_MODE_TEXT(cmode_rle_r8g8b8);
		COLOR_MODE_TEXT(cmode_rle_a8r8g8b8);

		COLOR_MODE_TEXT(cmode_index4_r8g8b8);

		COLOR_MODE_TEXT(cmode_index8_gray8);
		COLOR_MODE_TEXT(cmode_index8_x1r5g5b5);
		COLOR_MODE_TEXT(cmode_index8_r8g8b8);

		COLOR_MODE_TEXT(cmode_d24s8);
	default:
		return _T("color_mode unkonwn");
	}
}

img_ix align_to_2(img_ix val)
{
	return val + 1 & ~1;
}

img_ix align_to_4(img_ix val)
{
	return val + 3 & ~3;
}

img_u8 color_float16_to_8(img_u16 f16)
{
#pragma pack(push, 4)
	union
	{
		img_32 ival;
		img_f32 fval;
	};
#pragma pack(pop)

	img_32 signific = (f16 >> 15) & 0x1;
	img_32 exponent = (f16 >> 10) & 0x1F;
	img_32 mantissa = (f16 & 0x3FF);

	if(mantissa == 0)
		return 0xFF;
	else if(exponent == 0)
	{
		while(!(mantissa & 0x00000400))
		{
			mantissa <<= 1;
			exponent -= 1;
		}

		exponent += 1;
		mantissa &= ~0x00000400;
	}
	else if(exponent == 31)
	{
		ival = (signific << 31) | 0x7f800000 | (mantissa << 13);
	}
	else
	{
		exponent += 0x70;
		ival = (signific << 31) | (exponent << 23) | (mantissa << 13);
	}
	//if(fval < 0.0f)
	//	return 0;
	//else if(fval > 1.0f)
	//	return 0xFF;
	//else
	return (img_u8)(fval * 0xFF);
}

static bool mask_equal(const color_mask_abgr_t & ma, const color_mask_abgr_t & mb)
{
	return 
		ma.r == mb.r &&
		ma.g == mb.g &&
		ma.b == mb.b &&
		ma.a == mb.a;
}

cmode_e cmode_from_mask_abgr(const color_mask_abgr_t & mask, img_ix bits)
{
	switch(bits)
	{
	case 8:
		if(mask_equal(mask, MASK_R3G3B2))
			return cmode_r3g3b2;
		break;
	case 16:
		if(mask_equal(mask, MASK_A8R3G3B2))
			return cmode_a8r3g3b2;
		if(mask_equal(mask, MASK_A4R4G4B4))
			return cmode_a4r4g4b4;
		if(mask_equal(mask, MASK_X4R4G4B4))
			return cmode_x4r4g4b4;
		if(mask_equal(mask, MASK_A0R5G6B5))
			return cmode_r5g6b5;
		if(mask_equal(mask, MASK_X1R5G5B5))
			return cmode_x1r5g5b5;
		if(mask_equal(mask, MASK_A1R5G5B5))
			return cmode_a1r5g5b5;
		break;
	case 24:
		if(mask_equal(mask, MASK_R8G8B8))
			return cmode_r8g8b8;
		if(mask_equal(mask, MASK_B8G8R8))
			return cmode_b8g8r8;
		break;
	case 32:
		if(mask_equal(mask, MASK_X8R8G8B8))
			return cmode_x8r8g8b8;
		if(mask_equal(mask, MASK_A8R8G8B8))
			return cmode_a8r8g8b8;
		if(mask_equal(mask, MASK_R8G8B8X8))
			return cmode_r8g8b8x8;
		if(mask_equal(mask, MASK_R8G8B8A8))
			return cmode_r8g8b8a8;
		if(mask_equal(mask, MASK_X8B8G8R8))
			return cmode_x8b8g8r8;
		if(mask_equal(mask, MASK_A8B8G8R8))
			return cmode_a8b8g8r8;
	
		// dds file format cmode_a2b10g10r10 is error, it should be cmode_a2r10g10b10.
		if(mask_equal(mask, MASK_A2R10G10B10))
			return cmode_a2b10g10r10;
		if(mask_equal(mask, MASK_A2B10G10R10))
			return cmode_a2r10g10b10;


		if(mask_equal(mask, MASK_G16R16))
			return cmode_g16r16;
		break;
	}
	return cmode_invalid;
}

bool image_data_malloc(image_data_t * data)
{
	data->buffer = (img_u8 *)malloc(data->length);
	return data->buffer != nullptr;
}

void image_data_free(image_data_t * data)
{
	free((void *)data->buffer);
}


//////////////////////////////////////////////////////////////////////////
/// color_a_to_b

void color_8_to_8(const void * src_pixel, void * dst_pixel)
{
	img_u8 * src = (img_u8 *)src_pixel;
	img_u8 * dst = (img_u8 *)dst_pixel;
	*dst = *src;
}

void color_16_to_16(const void * src_pixel, void * dst_pixel)
{
	img_u16 * src = (img_u16 *)src_pixel;
	img_u16 * dst = (img_u16 *)dst_pixel;
	*dst = *src;
}

void color_24_to_24(const void * src_pixel, void * dst_pixel)
{
	color_rgb_t * src = (color_rgb_t *)src_pixel;
	color_rgb_t * dst = (color_rgb_t *)dst_pixel;
	*dst = *src;
}

void color_32_to_32(const void * src_pixel, void * dst_pixel)
{
	img_u32 * src = (img_u32 *)src_pixel;
	img_u32 * dst = (img_u32 *)dst_pixel;
	*dst = *src;
}

void color_32_to_32_ex0(const void * src_pixel, void * dst_pixel)
{
	img_u32 * src = (img_u32 *)src_pixel;
	img_u32 * dst = (img_u32 *)dst_pixel;
	if(*src & 0xFF000000)
		*dst = *src;
	else if(*src)
		*dst = *src | 0xFF000000;
	else
		*dst = 0;
}

void color_r5g6b5_to_x1r5g5b5(const void * src_pixel, void * dst_pixel)
{
	img_u16 * src = (img_u16 *)src_pixel;
	img_u16 * dst = (img_u16 *)dst_pixel;
	*dst =
		((*src & 0xF800) >> 1) |
		(((*src & 0x07E0) >> 1) & 0x03E0) |
		((*src & 0x001F));
}
void color_r5g6b5_to_r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u16 * src = (img_u16 *)src_pixel;
	color_rgb_t * dst = (color_rgb_t *)dst_pixel;

	dst->r = img_u8((*src & 0xF800) >> 8);
	dst->g = img_u8((*src & 0x07E0) >> 3);
	dst->b = img_u8((*src & 0x001F) << 3);
}

void color_r5g6b5_to_x8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u16 * src = (img_u16 *)src_pixel;
	color_argb_t * dst = (color_argb_t *)dst_pixel;

	dst->r = img_u8((*src & 0xF800) >> 8);
	dst->g = img_u8((*src & 0x07E0) >> 3);
	dst->b = img_u8((*src & 0x001F) << 3);
	dst->a = 0xFF;
}

void color_r5g6b5_to_x8b8g8r8(const void * src_pixel, void * dst_pixel)
{
	img_u16 src = *(const img_u16 *)src_pixel;
	img_u32 * dst = (img_u32 *)dst_pixel;

	*dst =
		0xFF000000 |
		((((src >> 0) & 0x1F) * 0xFF / 0x1F) << 16) |
		((((src >> 5) & 0x3F) * 0xFF / 0x3F) << 8) |
		((((src >> 11) & 0x1F) * 0xFF / 0x1F) << 0);
}

void color_b5g6r5_to_r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u16 src = *(const img_u16 *)src_pixel;
	img_u8 * dst = (img_u8 *)dst_pixel;

	dst[0] = ((src >> 11) & 0x1F) * 0xFF / 0x1F;
	dst[1] = ((src >> 05) & 0x3F) * 0xFF / 0x3F;
	dst[2] = ((src >> 00) & 0x1F) * 0xFF / 0x1F;
}

void color_b5g6r5_to_x8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u16 src = *(const img_u16 *)src_pixel;
	img_u8 * dst = (img_u8 *)dst_pixel;

	dst[0] = ((src >> 11) & 0x1F) * 0xFF / 0x1F;
	dst[1] = ((src >> 05) & 0x3F) * 0xFF / 0x3F;
	dst[2] = ((src >> 00) & 0x1F) * 0xFF / 0x1F;
	dst[3] = 0xFF;
}

void color_a1r5g5b5_to_x1r5g5b5(const void * src_pixel, void * dst_pixel)
{
	img_u16 * src = (img_u16 *)src_pixel;
	img_u16 * dst = (img_u16 *)dst_pixel;
	*dst = *src | 0x8000;
}

void color_a1r5g5b5_to_a8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u16 src = *(img_u16 *)src_pixel;
	color_argb_t * dst = (color_argb_t *)dst_pixel;
	dst->a = img_u8((src & (0x01 << 15)) >> 15) * 0xFF / 0x01;
	dst->r = img_u8((src & (0x1F << 10)) >> 10) * 0xFF / 0x1F;
	dst->g = img_u8((src & (0x1F << 5)) >> 5) * 0xFF / 0x1F;
	dst->b = img_u8((src & (0x1F << 0)) >> 0) * 0xFF / 0x1F;
}

void color_a1r5g6b5_to_a8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u32 src = *(img_u32 *)src_pixel;
	color_argb_t * dst = (color_argb_t *)dst_pixel;
	dst->a = ((src & (0x01 << 16)) >> 16) * 0xFF / 0x01;
	dst->r = ((src & (0x1F << 11)) >> 11) * 0xFF / 0x1F;
	dst->g = ((src & (0x3F << 5)) >> 5) * 0xFF / 0x3F;
	dst->b = ((src & (0x1F << 0)) >> 0) * 0xFF / 0x1F;
}

void color_a4r5g6b5_to_a8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u32 src = *(img_u32 *)src_pixel;
	color_argb_t * dst = (color_argb_t *)dst_pixel;
	dst->a = ((src & (0x0F << 16)) >> 16) * 0xFF / 0x0F;
	dst->r = ((src & (0x1F << 11)) >> 11) * 0xFF / 0x1F;
	dst->g = ((src & (0x3F << 5)) >> 5) * 0xFF / 0x3F;
	dst->b = ((src & (0x1F << 0)) >> 0) * 0xFF / 0x1F;
}

void color_x4r4g4b4_to_x1r5g5b5(const void * src_pixel, void * dst_pixel)
{
	img_u16 * src = (img_u16 *)src_pixel;
	img_u16 * dst = (img_u16 *)dst_pixel;
	*dst =
		((*src & 0x0F00) << 3) |
		((*src & 0x00F0) << 2) |
		((*src & 0x00F) << 1);
}

void color_x4r4g4b4_to_x8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u16 src = *(const img_u16 *)src_pixel;
	img_u32 * dst = (img_u32 *)dst_pixel;
	*dst =
		(0xFF000000) |
		((((src & (0xF << 8)) >> 8) * 0xFF / 0xF) << 16) |
		((((src & (0xF << 4)) >> 4) * 0xFF / 0xF) << 8) |
		((((src & (0xF << 0)) >> 0) * 0xFF / 0xF) << 0);
}

void color_x4r4g4b4_to_x8b8g8r8(const void * src_pixel, void * dst_pixel)
{
	img_u16 src = *(const img_u16 *)src_pixel;
	img_u32 * dst = (img_u32 *)dst_pixel;
	*dst =
		(0xFF000000) |
		((((src & (0xF << 0)) >> 0) * 0xFF / 0xF) << 16) |
		((((src & (0xF << 8)) >> 8) * 0xFF / 0xF) << 8) |
		((((src & (0xF << 12)) >> 12) * 0xFF / 0xF) << 0);
}

void color_a4r4g4b4_to_a8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u16 & src = *(img_u16 *)src_pixel;
	img_u32 & dst = *(img_u32 *)dst_pixel;
	img_u32 a = ((src & 0xF000) >> 12) * 0xFF / 0xF;
	img_u32 r = ((src & 0x0F00) >> 8) * 0xFF / 0xF;
	img_u32 g = ((src & 0x00F0) >> 4) * 0xFF / 0xF;
	img_u32 b = ((src & 0x000F) >> 0) * 0xFF / 0xF;
	dst = (a << 24) | (r << 16) | (g << 8) | b;
}

void color_a4r4g4b4_to_a8b8g8r8(const void * src_pixel, void * dst_pixel)
{
	img_u16 src = *(const img_u16 *)src_pixel;
	img_u32 * dst = (img_u32 *)dst_pixel;
	*dst =
		(src & 0xFF000000) |
		((((src & 0x0F00) >> 8) * 0xFF / 0xF) << 0) |
		((((src & 0x00F0) >> 4) * 0xFF / 0xF) << 8) |
		((((src & 0x000F) >> 0) * 0xFF / 0xF) << 16);
}

void color_a4r4g4b4_to_x8b8g8r8(const void * src_pixel, void * dst_pixel)
{
	img_u16 src = *(const img_u16 *)src_pixel;
	img_u32 * dst = (img_u32 *)dst_pixel;
	*dst =
		(0xFF000000) |
		((((src >> 0) & 0x0003) * 0xFF / 0x0003) << 16) |
		((((src >> 2) & 0x0007) * 0xFF / 0x0007) << 8) |
		((((src >> 5) & 0x0007) * 0xFF / 0x0007) << 0);
}

void color_a16b16g16r16_to_a8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_a16b16g16r16_t * src = (color_a16b16g16r16_t *)src_pixel;
	color_argb_t * dst = (color_argb_t *)dst_pixel;
	dst->a = src->a >> 8;
	dst->r = src->r >> 8;
	dst->g = src->g >> 8;
	dst->b = src->b >> 8;
}
void color_a16b16g16r16f_to_a8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_a16b16g16r16f_t * src = (color_a16b16g16r16f_t *)src_pixel;
	color_argb_t * dst = (color_argb_t *)dst_pixel;
	dst->a = color_float16_to_8(src->a);
	dst->r = color_float16_to_8(src->r);
	dst->g = color_float16_to_8(src->g);
	dst->b = color_float16_to_8(src->b);
}
void color_g16r16_to_r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u32 * src = (img_u32 *)src_pixel;
	color_rgb_t * dst = (color_rgb_t *)dst_pixel;
	dst->r = (*src & 0x0000FFFF) >> 8;
	dst->g = (*src & 0xFFFF0000) >> 24;
	dst->b = 0xFF;
}
void color_g16r16f_to_r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_g16r16f_t * src = (color_g16r16f_t *)src_pixel;
	color_rgb_t * dst = (color_rgb_t *)dst_pixel;
	dst->r = color_float16_to_8(src->r);
	dst->g = color_float16_to_8(src->g);
	dst->b = 0xFF;
}
void color_r16f_to_r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u16 * src = (img_u16 *)src_pixel;
	color_rgb_t * dst = (color_rgb_t *)dst_pixel;
	dst->r = color_float16_to_8(*src);
	dst->g = 0xFF;
	dst->b = 0xFF;
}
void color_r32f_to_r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_f32 * src = (img_f32 *)src_pixel;
	color_rgb_t * dst = (color_rgb_t *)dst_pixel;
	dst->r = img_u8(*src * 255.0f);
	dst->g = 0xFF;
	dst->b = 0xFF;
}
void color_g32r32f_to_r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_g32r32f_t * src = (color_g32r32f_t *)src_pixel;
	color_rgb_t * dst = (color_rgb_t *)dst_pixel;
	dst->r = img_u8(src->r * 255.0f);
	dst->g = img_u8(src->g * 255.0f);
	dst->b = 0xFF;
}
void color_a32b32g32r32f_to_a8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_a32b32g32r32f_t * src = (color_a32b32g32r32f_t *)src_pixel;
	color_argb_t * dst = (color_argb_t *)dst_pixel;
	dst->a = img_u8(src->a * 255.0f);
	dst->r = img_u8(src->r * 255.0f);
	dst->g = img_u8(src->g * 255.0f);
	dst->b = img_u8(src->b * 255.0f);
}

void color_r3g3b2_to_x1r5g5b5(const void * src_pixel, void * dst_pixel)
{
	img_u8 src = *(const img_u8 *)src_pixel;
	img_u16 * dst = (img_u16 *)dst_pixel;
	*dst =
		(0x8000) |
		((((src >> 0) & 0x3) * 0x1F / 0x3) << 10) |
		((((src >> 2) & 0x7) * 0x1F / 0x7) << 5) |
		((((src >> 5) & 0x7) * 0x1F / 0x7) << 0);
}

void color_r3g3b2_to_a1r5g5b5(const void * src_pixel, void * dst_pixel)
{
	img_u8 src = *(const img_u8 *)src_pixel;
	img_u16 * dst = (img_u16 *)dst_pixel;
	*dst =
		(0x8000) |
		((((src >> 0) & 0x3) * 0x1F / 0x3) << 10) |
		((((src >> 2) & 0x7) * 0x1F / 0x7) << 5) |
		((((src >> 5) & 0x7) * 0x1F / 0x7) << 0);
}

void color_r3g3b2_to_x8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u8 src = *(const img_u8 *)src_pixel;
	img_u32 * dst = (img_u32 *)dst_pixel;
	*dst =
		(0xFF000000) |
		((((src & (0x7 << 5)) >> 5) * 0xFF / 0x7) << 16)|
		((((src & (0x7 << 2)) >> 2) * 0xFF / 0x7) << 8)|
		((((src & (0x3 << 0)) >> 0) * 0xFF / 0x3) << 0);
}

void color_a8r3g3b2_to_a8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u16 src = *(const img_u16 *)src_pixel;
	img_u32 * dst = (img_u32 *)dst_pixel;
	*dst =
		((((src >> 8) & 0xFF) * 0xFF / 0xFF) << 24) |
		((((src >> 5) & 0x07) * 0xFF / 0x07) << 16) |
		((((src >> 2) & 0x07) * 0xFF / 0x07) << 8) |
		((((src >> 0) & 0x03) * 0xFF / 0x03) << 0);
}

void color_a8r3g3b2_to_a8b8g8r8(const void * src_pixel, void * dst_pixel)
{
	img_u16 src = *(const img_u16 *)src_pixel;
	img_u32 * dst = (img_u32 *)dst_pixel;
	*dst =
		((src & 0xFF00) << 16) |
		((((src >> 0) & 0x3) * 0xFF / 0x3) << 16) |
		((((src >> 2) & 0x7) * 0xFF / 0x7) << 8) |
		((((src >> 5) & 0x7) * 0xFF / 0x7) << 0);
}

void color_r8g8b8_to_b8g8r8(const void * src_pixel, void * dst_pixel)
{
	color_rgb_t * src = (color_rgb_t *)src_pixel;
	color_rgb_t * dst = (color_rgb_t *)dst_pixel;
	dst->r = src->b;
	dst->g = src->g;
	dst->b = src->r;
}

void color_r8g8b8_to_x8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_rgb_t * src = (color_rgb_t *)src_pixel;
	color_argb_t * dst = (color_argb_t *)dst_pixel;
	dst->a = 0xFF;
	dst->rgb = *src;
}
void color_r8g8b8_to_r8g8b8a8(const void * src_pixel, void * dst_pixel)
{
	color_rgb_t * src = (color_rgb_t *)src_pixel;
	color_abgr_t * dst = (color_abgr_t *)dst_pixel;
	dst->a = 0xFF;
	dst->r = src->r;
	dst->g = src->g;
	dst->b = src->b;
}

void color_a8r8g8b8_to_r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_argb_t * src = (color_argb_t *)src_pixel;
	color_rgb_t * dst = (color_rgb_t *)dst_pixel;
	*dst = src->rgb;
}
void color_a8b8g8r8_to_a8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_argb_t * src = (color_argb_t *)src_pixel;
	color_argb_t * dst = (color_argb_t *)dst_pixel;
	dst->a = src->a;
	dst->r = src->b;
	dst->g = src->g;
	dst->b = src->r;
}

void color_x8b8g8r8_to_r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_argb_t * src = (color_argb_t *)src_pixel;
	color_rgb_t * dst = (color_rgb_t *)dst_pixel;
	dst->r = src->b;
	dst->g = src->g;
	dst->b = src->r;
}

void color_x8r8g8b8_to_r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_xrgb_t * src = (color_xrgb_t *)src_pixel;
	color_rgb_t * dst = (color_rgb_t *)dst_pixel;
	*dst = src->rgb;
}
void color_x8r8g8b8_to_x1r5g5b5(const void * src_pixel, void * dst_pixel)
{
	color_xrgb_t * src = (color_xrgb_t *)src_pixel;
	img_u16 * dst = (img_u16 *)dst_pixel;
	img_u8 r = src->r >> 3;
	img_u8 g = src->g >> 3;
	img_u8 b = src->b >> 3;
	*dst = 0x8000 | (r << 10) | (g << 5) | b;
}

void color_x8r8g8b8_to_r5g6b5(const void * src_pixel, void * dst_pixel)
{
	color_xrgb_t * src = (color_xrgb_t *)src_pixel;
	img_u16 * dst = (img_u16 *)dst_pixel;
	img_u8 r = src->r >> 3;
	img_u8 g = src->g >> 2;
	img_u8 b = src->b >> 3;
	*dst = (r << 11) | (g << 5) | b;
}

void color_gray8_to_x8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u8 src = *(const img_u8 *)src_pixel;
	img_u32 * dst = (img_u32 *)dst_pixel;
	*dst = 0xFF000000 | (src << 16) | (src << 8) | src;
}

void color_gray8_to_r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_rgb_t * dst_rgb = (color_rgb_t *)dst_pixel;
	img_u8 val = *((img_u8 *)src_pixel);
	dst_rgb->r = val;
	dst_rgb->g = val;
	dst_rgb->b = val;
}

void color_x1r5g5b5_to_a1r5g5b5(const void * src_pixel, void * dst_pixel)
{
	img_u16 src = *((const img_u16 *)src_pixel);
	img_u16 * dst = (img_u16 *)dst_pixel;
	*dst = src | 0x08000;
}

void color_x1r5g5b5_to_r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u16 src_rgb = *((img_u16 *)src_pixel);
	color_rgb_t * dst_rgb = (color_rgb_t *)dst_pixel;
	dst_rgb->r = img_u8((src_rgb & 0x7C00) >> 7);
	dst_rgb->g = img_u8((src_rgb & 0x3E0) >> 2);
	dst_rgb->b = img_u8((src_rgb & 0x1F) << 3);
}

void color_x1r5g5b5_to_x8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u16 src = *((img_u16 *)src_pixel);
	color_argb_t * dst = (color_argb_t *)dst_pixel;
	dst->a = 0xFF;
	dst->r = img_u8((src & 0x7C00) >> 7);
	dst->g = img_u8((src & 0x3E0) >> 2);
	dst->b = img_u8((src & 0x1F) << 3);
}

void color_x1r5g5b5_to_x8b8g8r8(const void * src_pixel, void * dst_pixel)
{
	img_u16 src = *((img_u16 *)src_pixel);
	color_abgr_t * dst = (color_abgr_t *)dst_pixel;
	dst->a = 0xFF;
	dst->r = img_u8((src & 0x7C00) >> 7);
	dst->g = img_u8((src & 0x3E0) >> 2);
	dst->b = img_u8((src & 0x1F) << 3);
}

void color_a8r8g8b8_to_a8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_argb_t * src_argb = (color_argb_t *)src_pixel;
	color_argb_t * dst_argb = (color_argb_t *)dst_pixel;
	*dst_argb = *src_argb;
}

void color_yuv_to_r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_yuv_t * yuv = (color_yuv_t *)src_pixel;
	color_rgb_t * rgb = (color_rgb_t *)dst_pixel;
	img_32 temp = 0;

	temp = (img_32)(yuv->y + 1.402f * (yuv->v - 128) + 0.03125f);
	if(temp < 0)
		rgb->r = 0;
	else if(temp > 0xFF)
		rgb->r = 0xFF;
	else
		rgb->r = (img_u8)temp;

	temp = (img_32)(yuv->y - 0.34413f * (yuv->u - 128) - 0.71414f * (yuv->v - 128));
	if(temp < 0)
		rgb->g = 0;
	else if(temp > 0xFF)
		rgb->g = 0xFF;
	else
		rgb->g = (img_u8)temp;

	temp = (img_32)(yuv->y + 1.772f * (yuv->u - 128));
	if(temp < 0)
		rgb->b = 0;
	else if(temp > 0xFF)
		rgb->b = 0xFF;
	else
		rgb->b = (img_u8)temp;
}

void color_r8g8b8_to_yuv(const void * src_pixel, void * dst_pixel)
{
	color_rgb_t * src = (color_rgb_t *)src_pixel;
	color_yuv_t * dst = (color_yuv_t *)dst_pixel;
	dst->y = img_u8(0.299f * src->r + 0.587f * src->g + 0.114f * src->b);
	dst->u = img_u8(0.169 * src->r - 0.3313f * src->g + 0.5f * src->b);
	dst->v = img_u8(0.5f * src->r - 0.4187f * src->g - 0.0813f * src->b);
}

void color_yuv_to_a8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_yuv_t * yuv = (color_yuv_t *)src_pixel;
	color_argb_t * argb = (color_argb_t *)dst_pixel;
	argb->a = 0xFF;
	color_yuv_to_r8g8b8(yuv, (color_rgb_t *)argb);
}

void color_ycck_to_r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_ycck_t * ycck = (color_ycck_t *)src_pixel;
	color_rgb_t * rgb = (color_rgb_t *)dst_pixel;
	img_32 temp = 0;

	temp = (img_32)(ycck->y + 1.402f * (ycck->r - 128) + 0.03125f);
	if(temp < 0)
		rgb->r = 0;
	else if(temp > 0xFF)
		rgb->r = 0xFF;
	else
		rgb->r = (img_u8)temp;

	temp = (img_32)(ycck->y - 0.34413f * (ycck->b - 128) - 0.71414f * (ycck->r - 128));
	if(temp < 0)
		rgb->g = 0;
	else if(temp > 0xFF)
		rgb->g = 0xFF;
	else
		rgb->g = (img_u8)temp;

	temp = (img_32)(ycck->y + 1.772f * (ycck->b - 128));
	if(temp < 0)
		rgb->b = 0;
	else if(temp > 0xFF)
		rgb->b = 0xFF;
	else
		rgb->b = (img_u8)temp;
}

void color_ycck_to_a8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_ycck_t * ycck = (color_ycck_t *)src_pixel;
	color_argb_t * argb = (color_argb_t *)dst_pixel;
	argb->a = 0xFF;
	color_ycck_to_r8g8b8(ycck, (color_rgb_t *)argb);
}

void color_gray_to_r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u8 * gray = (img_u8 *)src_pixel;
	color_rgb_t * rgb = (color_rgb_t *)dst_pixel;
	rgb->r = *gray;
	rgb->g = *gray;
	rgb->b = *gray;
}

void color_gray_to_a8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u8 * gray = (img_u8 *)src_pixel;
	color_argb_t * argb = (color_argb_t *)dst_pixel;
	argb->a = 0xFF;
	argb->r = *gray;
	argb->g = *gray;
	argb->b = *gray;
}

void color_cmyk_to_r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_cmyk_t * cmyk = (color_cmyk_t *)src_pixel;
	color_rgb_t * rgb = (color_rgb_t *)dst_pixel;
	rgb->r = (cmyk->k * cmyk->c) >> 8;
	rgb->g = (cmyk->k * cmyk->m) >> 8;
	rgb->b = (cmyk->k * cmyk->y) >> 8;
}

void color_cmyk_to_a8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_cmyk_t * cmyk = (color_cmyk_t *)src_pixel;
	color_argb_t * argb = (color_argb_t *)dst_pixel;
	argb->a = 0xFF;
	color_cmyk_to_r8g8b8(cmyk, (color_rgb_t *)argb);
}

void color_r8g8b8_to_x32y32z32f(const void * src_pixel, void * dst_pixel)
{
	color_rgb_t * src = (color_rgb_t *)src_pixel;
	color_x32y32z32f_t * dst = (color_x32y32z32f_t *)dst_pixel;

	img_f32 r = src->r / 255.0f;
	img_f32 g = src->g / 255.0f;
	img_f32 b = src->b / 255.0f;

	dst->x = 0.412453f * r + 0.357580f * g + 0.180423f * b;
	dst->y = 0.212671f * r + 0.715160f * g + 0.072169f * b;
	dst->z = 0.019334f * r + 0.119193f * g + 0.950227f * b;
}

void color_x32y32z32f_tor_r8g8b8(const void * src_pixel, void * dst_pixel)
{
	color_x32y32z32f_t * src = (color_x32y32z32f_t *)src_pixel;
	color_rgb_t * dst = (color_rgb_t *)dst_pixel;

	img_f32 r = 3.240479f * src->x - 1.537150f * src->y - 0.498535f * src->z;
	img_f32 g = -0.969256f * src->x + 1.875992f * src->y + 0.041556f * src->z;
	img_f32 b = 0.055648f * src->x - 0.204043f * src->y + 1.057311f * src->z;

	if(r <= 0.0f)		dst->r = 0;
	else if(r >= 1.0f)	dst->r = 0xFF;
	else				dst->r = img_u8(r * 0xFF);

	if(g <= 0.0f)		dst->g = 0;
	else if(g >= 1.0f)	dst->g = 0xFF;
	else				dst->g = img_u8(g * 0xFF);

	if(b <= 0.0f)		dst->b = 0;
	else if(r >= 1.0f)	dst->b = 0xFF;
	else				dst->b = img_u8(b * 0xFF);
}

void color_r8g8b8x8_to_r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u32 src = *(const img_u32 *)src_pixel;
	color_rgb_t * dst = (color_rgb_t *)dst_pixel;
	dst->r = (src >> 24) & 0xFF;
	dst->g = (src >> 16) & 0xFF;
	dst->b = (src >> 8) & 0xFF;
}

void color_r8g8b8x8_to_x8b8g8r8(const void * src_pixel, void * dst_pixel)
{
	img_u32 src = *(const img_u32 *)src_pixel;
	img_u32 * dst = (img_u32 *)dst_pixel;
	*dst = (0xFF000000) | ((src << 8) & 0xFF0000) | ((src >> 8) & 0xFF00) | ((src >> 24) & 0xFF);
}

void color_r8g8b8a8_to_b8g8r8a8(const void * src_pixel, void * dst_pixel)
{
	const color_r8g8b8a8_t * src = (color_r8g8b8a8_t *)src_pixel;
	color_b8g8r8a8_t * dst = (color_b8g8r8a8_t *)dst_pixel;
	dst->a = src->a;
	dst->r = src->r;
	dst->g = src->g;
	dst->b = src->b;
}
void color_a8r8g8b8_to_a8b8g8r8(const void * src_pixel, void * dst_pixel)
{
	const color_a8r8g8b8_t * src = (color_a8r8g8b8_t *)src_pixel;
	color_abgr_t * dst = (color_abgr_t *)dst_pixel;
	dst->a = src->a;
	dst->r = src->r;
	dst->g = src->g;
	dst->b = src->b;
}

void color_x8r8g8b8_to_x8b8g8r8(const void * src_pixel, void * dst_pixel)
{
	const color_a8r8g8b8_t * src = (const color_a8r8g8b8_t *)src_pixel;
	color_a8b8g8r8_t * dst = (color_a8b8g8r8_t *)dst_pixel;
	dst->a = 0xFF;
	dst->r = src->r;
	dst->g = src->g;
	dst->b = src->b;
}

void color_x8b8g8r8_to_x8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	const color_a8b8g8r8_t * src = (const color_a8b8g8r8_t *)src_pixel;
	color_a8r8g8b8_t * dst = (color_a8r8g8b8_t *)dst_pixel;
	dst->a = 0xFF;
	dst->r = src->r;
	dst->g = src->g;
	dst->b = src->b;
}

void color_r8g8b8_to_a8b8g8r8(const void * src_pixel, void * dst_pixel)
{
	const color_rgb_t * src = (color_rgb_t *)src_pixel;
	color_abgr_t * dst = (color_abgr_t *)dst_pixel;
	dst->a = 0xFF;
	dst->r = src->r;
	dst->g = src->g;
	dst->b = src->b;
}

void color_r8g8b8_to_x8b8g8r8(const void * src_pixel, void * dst_pixel)
{
	const color_rgb_t * src = (color_rgb_t *)src_pixel;
	color_abgr_t * dst = (color_abgr_t *)dst_pixel;
	dst->a = 0xFF;
	dst->r = src->r;
	dst->g = src->g;
	dst->b = src->b;
}

void color_b8g8r8_to_a8b8g8r8(const void * src_pixel, void * dst_pixel)
{
	const color_bgr_t * src = (color_bgr_t *)src_pixel;
	*(img_u32 *)dst_pixel =
		0xFF000000 |
		(src->b << 16) |
		(src->g << 8) |
		(src->r << 0);
}

void color_b8g8r8_to_x8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	const color_bgr_t * src = (color_bgr_t *)src_pixel;
	*(img_u32 *)dst_pixel =
		0xFF000000 |
		(src->r << 16) |
		(src->g << 8) |
		(src->b << 0);
}

void color_a2r10g10b10_to_a8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u32 src = *(const img_u32 *)src_pixel;
	*(img_u32 *)dst_pixel =
		((((src & 0xC0000000) >> 0x1E) * 0xFF / 0x003) << 0x18) |
		((((src & 0x3FF00000) >> 0x14) * 0xFF / 0x3FF) << 0x10) |
		((((src & 0x000FFC00) >> 0x0A) * 0xFF / 0x3FF) << 0x08) |
		((((src & 0x000003FF) >> 0x00) * 0xFF / 0x3FF) << 0x00);
}

void color_a2b10g10r10_to_a8r8g8b8(const void * src_pixel, void * dst_pixel)
{
	img_u32 src = *(const img_u32 *)src_pixel;
	*(img_u32 *)dst_pixel =
		((((src & 0xC0000000) >> 0x1E) * 0xFF / 0x003) << 0x18) |
		((((src & 0x000003FF) >> 0x00) * 0xFF / 0x3FF) << 0x10) |
		((((src & 0x000FFC00) >> 0x0A) * 0xFF / 0x3FF) << 0x08) |
		((((src & 0x3FF00000) >> 0x14) * 0xFF / 0x3FF) << 0x00);
}

void color_a2b10g10r10_to_a2b10g10r10(const void * src_pixel, void * dst_pixel)
{
	img_u32 src = *(const img_u32 *)src_pixel;
	img_u32 * dst = (img_u32 *)dst_pixel;
	*dst = (src & 0xC0000000) | ((src & 0x3FF00000) >> 20) | (src & 0xFFC00) | ((src & 0x3FF) << 20);
}

void color_a2b10g10r10_to_a2r10g10b10(const void * src_pixel, void * dst_pixel)
{
	img_u32 src = *(const img_u32 *)src_pixel;
	img_u32 * dst = (img_u32 *)dst_pixel;
	*dst = (src & 0xC0000000) | ((src & 0x3FF00000) >> 20) | (src & 0xFFC00) | ((src & 0x3FF) << 20);
}

VENUS_END
