#include "stdafx.h"

VENUS_BEG

bool d2d_rule_argb(image_convert_rule_t * rule)
{
	if(!rule)
		return false;

	switch(rule->src_mode)
	{
	case cmode_gray8:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = color_gray8_to_x8r8g8b8;
		rule->image_convert_fun = image_convert_ex;

		rule->src_bits = 8;
		rule->src_stride = 1;
		rule->src_pitch = align_to_4(rule->width * 1);

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_r3g3b2:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = color_r3g3b2_to_x8r8g8b8;
		rule->image_convert_fun = image_convert_ex;

		rule->src_bits = 8;
		rule->src_stride = 1;
		rule->src_pitch = align_to_4(rule->width * 1);

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = align_to_4(rule->width * 4);

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_x1r5g5b5:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = color_x1r5g5b5_to_x8r8g8b8;
		rule->image_convert_fun = image_convert_ex;

		rule->src_bits = 16;
		rule->src_stride = 2;
		rule->src_pitch = align_to_4(rule->width * 2);

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_a1r5g5b5:
		rule->dst_mode = cmode_a8r8g8b8;
		rule->pixel_convert_fun = color_a1r5g5b5_to_a8r8g8b8;
		rule->image_convert_fun = image_convert_ex;

		rule->src_bits = 16;
		rule->src_stride = 2;
		rule->src_pitch = align_to_4(rule->width * 2);

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = align_to_4(rule->width * 4);

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_r5g6b5:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = color_r5g6b5_to_x8r8g8b8;
		rule->image_convert_fun = image_convert_ex;

		rule->src_bits = 16;
		rule->src_stride = 2;
		rule->src_pitch = align_to_4(rule->width * 2);

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_a4r4g4b4:
		rule->dst_mode = cmode_a8r8g8b8;
		rule->pixel_convert_fun = color_a4r4g4b4_to_a8r8g8b8;
		rule->image_convert_fun = image_convert_ex;

		rule->src_bits = 16;
		rule->src_stride = 2;
		rule->src_pitch = align_to_4(rule->width * 2);

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_x4r4g4b4:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = color_x4r4g4b4_to_x8r8g8b8;
		rule->image_convert_fun = image_convert_ex;

		rule->src_bits = 16;
		rule->src_stride = 2;
		rule->src_pitch = align_to_4(rule->width * 2);

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_a8r3g3b2:
		rule->dst_mode = cmode_a8r8g8b8;
		rule->pixel_convert_fun = color_a8r3g3b2_to_a8r8g8b8;
		rule->image_convert_fun = image_convert_ex;

		rule->src_bits = 16;
		rule->src_stride = 2;
		rule->src_pitch = align_to_4(rule->width * 2);

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_a2r10g10b10:
		rule->dst_mode = cmode_a8r8g8b8;
		rule->pixel_convert_fun = color_a2r10g10b10_to_a8r8g8b8;
		rule->image_convert_fun = image_convert_ex;

		rule->src_bits = 32;
		rule->src_stride = 4;
		rule->src_pitch = rule->width * 4;

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_a2b10g10r10:
		rule->dst_mode = cmode_a8r8g8b8;
		rule->pixel_convert_fun = color_a2b10g10r10_to_a8r8g8b8;
		rule->image_convert_fun = image_convert_ex;

		rule->src_bits = 32;
		rule->src_stride = 4;
		rule->src_pitch = rule->width * 4;

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_r8g8b8:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = color_r8g8b8_to_x8r8g8b8;
		rule->image_convert_fun = image_convert_ex;

		rule->src_bits = 24;
		rule->src_stride = 3;
		rule->src_pitch = align_to_4(rule->width * 3);

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_b8g8r8:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = color_b8g8r8_to_x8r8g8b8;
		rule->image_convert_fun = image_convert_ex;

		rule->src_bits = 24;
		rule->src_stride = 3;
		rule->src_pitch = align_to_4(rule->width * 3);

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_x8r8g8b8:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = nullptr;
		rule->image_convert_fun = image_convert_copy_ex;

		rule->src_bits = 32;
		rule->src_stride = 4;
		rule->src_pitch = rule->width * 4;

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_x8b8g8r8:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = color_x8b8g8r8_to_x8r8g8b8;
		rule->image_convert_fun = image_convert_ex;

		rule->src_bits = 32;
		rule->src_stride = 4;
		rule->src_pitch = rule->width * 4;

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_a8r8g8b8:
		rule->dst_mode = cmode_a8r8g8b8;
		rule->pixel_convert_fun = nullptr;
		rule->image_convert_fun = image_convert_copy_ex;

		rule->src_bits = 32;
		rule->src_stride = 4;
		rule->src_pitch = rule->width * 4;

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_a8b8g8r8:
		rule->dst_mode = cmode_a8r8g8b8;
		rule->pixel_convert_fun = color_a8b8g8r8_to_a8r8g8b8;
		rule->image_convert_fun = image_convert_ex;

		rule->src_bits = 32;
		rule->src_stride = 4;
		rule->src_pitch = rule->width * 4;

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_a16b16g16r16:
		rule->dst_mode = cmode_a8r8g8b8;
		rule->pixel_convert_fun = color_a16b16g16r16_to_a8r8g8b8;
		rule->image_convert_fun = image_convert_ex;

		rule->src_bits = 64;
		rule->src_stride = 8;
		rule->src_pitch = rule->width * 8;

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_a16b16g16r16f:
		rule->dst_mode = cmode_a8r8g8b8;
		rule->pixel_convert_fun = color_a16b16g16r16f_to_a8r8g8b8;
		rule->image_convert_fun = image_convert_ex;

		rule->src_bits = 64;
		rule->src_stride = 8;
		rule->src_pitch = rule->width * 8;

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_a32b32g32r32f:
		rule->dst_mode = cmode_a8r8g8b8;
		rule->pixel_convert_fun = color_a32b32g32r32f_to_a8r8g8b8;
		rule->image_convert_fun = image_convert_ex;

		rule->src_bits = 128;
		rule->src_stride = 16;
		rule->src_pitch = rule->width * 16;

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	default:
		rule->image_convert_fun = nullptr;
		break;
	}
	return rule->image_convert_fun != nullptr;
}

bool d2d_rule_bmp(image_convert_rule_t * rule)
{
	if(!rule)
		return false;

	switch(rule->src_mode)
	{
	case cmode_x4r4g4b4:
	case cmode_a4r4g4b4:
	case cmode_x1r5g5b5:
	case cmode_r5g6b5:
	case cmode_r8g8b8:
	case cmode_x8r8g8b8:
	case cmode_a8r8g8b8:
	case cmode_r8g8b8x8:
		return d2d_rule_argb(rule);

	case cmode_index1_x8r8g8b8:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = color_32_to_32;
		rule->image_convert_fun = image_convert_index1_ex;

		rule->pal_bits = 32;
		rule->pal_stride = 4;

		rule->src_bits = 1;
		rule->src_stride = 1;
		rule->src_pitch = align_to_4((rule->width + 7) / 8);

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_index4_r8g8b8:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = color_r8g8b8_to_x8r8g8b8;
		rule->image_convert_fun = image_convert_index4_ex;

		rule->pal_bits = 24;
		rule->pal_stride = 3;

		rule->src_bits = 4;
		rule->src_stride = 1;
		rule->src_pitch = align_to_4((rule->width * 4 + 7) / 8);

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_index4_x8r8g8b8:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = color_32_to_32;
		rule->image_convert_fun = image_convert_index4_ex;

		rule->pal_bits = 32;
		rule->pal_stride = 4;

		rule->src_bits = 4;
		rule->src_stride = 1;
		rule->src_pitch = align_to_4((rule->width * 4 + 7) / 8);

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_index8_r8g8b8:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = color_r8g8b8_to_x8r8g8b8;
		rule->image_convert_fun = image_convert_index8_ex;

		rule->pal_bits = 24;
		rule->pal_stride = 3;

		rule->src_bits = 8;
		rule->src_stride = 1;
		rule->src_pitch = align_to_4(rule->width);

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_index8_x8r8g8b8:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = color_32_to_32;
		rule->image_convert_fun = image_convert_index8_ex;

		rule->pal_bits = 32;
		rule->pal_stride = 4;

		rule->src_bits = 8;
		rule->src_stride = 1;
		rule->src_pitch = align_to_4(rule->width);

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_index4_rle2_x8r8g8b8:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = color_32_to_32;
		rule->image_convert_fun = image_convert_bmp_index4_rle;

		rule->pal_bits = 32;
		rule->pal_stride = 4;

		rule->src_bits = 0;
		rule->src_stride = 0;
		rule->src_pitch = 0;

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_index8_rle2_x8r8g8b8:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = color_32_to_32;
		rule->image_convert_fun = image_convert_bmp_index8_rle;

		rule->pal_bits = 32;
		rule->pal_stride = 4;

		rule->src_bits = 0;
		rule->src_stride = 0;
		rule->src_pitch = 0;

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	default:
		rule->image_convert_fun = nullptr;
		log2(L"[BMP]Unsupport color format : %s.", cmode_text(rule->src_mode));
		break;
	}
	return rule->image_convert_fun != nullptr;
}

bool d2d_rule_dds(image_convert_rule_t * rule)
{
	if(!rule)
		return false;

	// dds 不支持调色板
	rule->pal_bits = 0;
	rule->pal_stride = 0;

	switch(rule->src_mode)
	{
	case cmode_r3g3b2:
	case cmode_x1r5g5b5:
	case cmode_a1r5g5b5:
	case cmode_r5g6b5:
	case cmode_a4r4g4b4:
	case cmode_x4r4g4b4:
	case cmode_a8r3g3b2:
	case cmode_a2r10g10b10:
	case cmode_a2b10g10r10:
	case cmode_r8g8b8:
	case cmode_x8r8g8b8:
	case cmode_x8b8g8r8:
	case cmode_a8r8g8b8:
	case cmode_a8b8g8r8:
	case cmode_a16b16g16r16:
	case cmode_a16b16g16r16f:
	case cmode_a32b32g32r32f:
		return d2d_rule_argb(rule);

	case cmode_bc1:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = color_r5g6b5_to_x8r8g8b8;
		rule->image_convert_fun = dds_convert_bc1;

		rule->src_bits = 0;
		rule->src_stride = 0;
		rule->src_pitch = 0;

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_bc2:
		rule->dst_mode = cmode_a8r8g8b8;
		rule->pixel_convert_fun = color_dds_a4r5g6b5_to_a8r8g8b8;
		rule->image_convert_fun = dds_convert_bc2;

		rule->src_bits = 0;
		rule->src_stride = 0;
		rule->src_pitch = 0;

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_bc3:
		rule->dst_mode = cmode_a8r8g8b8;
		rule->pixel_convert_fun = color_dds_a8r5g6b5_to_a8r8g8b8;
		rule->image_convert_fun = dds_convert_bc3;

		rule->src_bits = 0;
		rule->src_stride = 0;
		rule->src_pitch = 0;

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
		break;
	default:
		rule->image_convert_fun = nullptr;
		log2(L"[DDS]Unsupport color format : %s.", cmode_text(rule->src_mode));
		break;
	}
	return rule->image_convert_fun != nullptr;
}

bool d2d_rule_tga(image_convert_rule_t * rule)
{
	switch(rule->src_mode)
	{
	case cmode_gray8:
	case cmode_x1r5g5b5:
	case cmode_r8g8b8:
	case cmode_a8r8g8b8:
		return d2d_rule_argb(rule);

	case cmode_index8_r8g8b8:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = color_r8g8b8_to_x8r8g8b8;
		rule->image_convert_fun = image_convert_index8_ex;

		rule->pal_bits = 24;
		rule->pal_stride = 3;

		rule->src_bits = 8;
		rule->src_stride = 1;
		rule->src_pitch = align_to_4(rule->width);

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_rle_r8g8b8:
		rule->dst_mode = cmode_x8r8g8b8;
		rule->pixel_convert_fun = color_r8g8b8_to_x8r8g8b8;
		rule->image_convert_fun = image_convert_tga_rle8;

		rule->src_bits = 24;
		rule->src_stride = 3;
		rule->src_pitch = 0; // unused

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	case cmode_rle_a8r8g8b8:
		rule->dst_mode = cmode_a8r8g8b8;
		rule->pixel_convert_fun = color_32_to_32;
		rule->image_convert_fun = image_convert_tga_rle8;

		rule->src_bits = 32;
		rule->src_stride = 4;
		rule->src_pitch = 0; // unused

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;

	default:
		rule->image_convert_fun = nullptr;
		log2(L"[TGA]Unsupport color format : %s.", cmode_text(rule->src_mode));
		break;
	}
	return rule->image_convert_fun != nullptr;
}

bool d2d_rule_png(image_convert_rule_t * rule)
{
	switch(rule->src_mode)
	{
	case cmode_gray1:
	case cmode_gray2:
	case cmode_gray4:
	case cmode_gray8:
	case cmode_r8g8b8:
	case cmode_b8g8r8:
		return d2d_rule_argb(rule);

	case cmode_a8b8g8r8:
		rule->dst_mode = cmode_a8r8g8b8;
		rule->pixel_convert_fun = color_a8b8g8r8_to_a8r8g8b8;
		rule->image_convert_fun = image_convert_ex;

		rule->src_bits = 32;
		rule->src_stride = 4;
		rule->src_pitch = rule->width * 4;

		rule->dst_bits = 32;
		rule->dst_stride = 4;
		rule->dst_pitch = rule->width * 4;

		rule->dst_length = rule->dst_pitch * rule->height;
		break;
	default:
		rule->image_convert_fun = nullptr;
		log2(L"[PNG]Unsupport color format : %s.", cmode_text(rule->src_mode));
		break;
	}
	return rule->image_convert_fun != nullptr;
}

bool d2d_rule_jpg(image_convert_rule_t * rule)
{
	switch(rule->src_mode)
	{
	case cmode_b8g8r8:
		return d2d_rule_argb(rule);

	default:
		rule->image_convert_fun = nullptr;
		log2(L"[JPG]Unsupport color format : %s.", cmode_text(rule->src_mode));
		break;
	}
	return rule->image_convert_fun != nullptr;
}

bool d2d_rule_full(image_convert_rule_t * rule)
{
	switch(rule->format)
	{
	case image_format_bmp:
		return d2d_rule_bmp(rule);
	case image_format_tga:
		return d2d_rule_tga(rule);
	case image_format_png:
		return d2d_rule_png(rule);
	case image_format_jpg:
		return d2d_rule_jpg(rule);
	case image_format_dds:
		return d2d_rule_dds(rule);
	default:
		return false;
	}
}

VENUS_END
