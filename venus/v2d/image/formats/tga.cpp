#include "stdafx.h"
#include "tga.h"

VENUS_BEG

bool is_tga_data(const img_u8 * buffer, img_ix length)
{
	if(length < sizeof(tag_tga_header))
		return false;

	const tga_header_t * header = (const tga_header_t *)buffer;
	switch(header->color_type)
	{
	case tga_color_type_rgb:
	case tga_color_type_index:
		break;
	default:
		return false;
	}
	switch(header->commpress)
	{
	case tga_format_index:
	case tga_format_rgb:
	case tga_format_gray:
	case tga_format_rle_index:
	case tga_format_rle_rgb:
	case tga_format_rle_gray:
	case tga_format_hf_rle_index:
	case tga_format_hf_rle_rgb:
		break;
	default:
		return false;
	}
	return true;
}


img_ix tga_create(const img_u8 * buffer, img_ix length, image_data_t * img,
				 image_convert_rule_fun_t pfn_rule, void * user_data)
{
	image_clear(img, sizeof(image_data_t));
	if(!pfn_rule)
		pfn_rule = tga_rule_default;

	const tga_header_t * header = (const tga_header_t *)buffer;
	buffer += sizeof(tga_header_t)+header->info_length/* 跳过图像信息字段 */;
	tga_format_e tga_format = (tga_format_e)header->commpress;

	img_ix width = (img_ix)header->width;
	img_ix height = (img_ix)header->height;
	img_ix flags = 0;
	// 从上到下存储
	if(!(header->flags & TGAF_FLIPY))
		flags |= IMAGE_CONVERT_FLIP_Y;

	const img_u8 * conv_buffer = buffer;
	const img_u8 * conv_palette = 0;
	cmode_e src_mode = cmode_invalid;
	switch(tga_format)
	{
	case tga_format_rgb:
		switch(header->bit_count)
		{
		case 8:
			src_mode = cmode_gray8;
			break;
		case 16:
			src_mode = cmode_x1r5g5b5;
			break;
		case 24:
			src_mode = cmode_r8g8b8;
			break;
		case 32:
			src_mode = cmode_a8r8g8b8;
			break;
		default:
			break;
		}
		break;
	case tga_format_gray:
		src_mode = cmode_gray8;
		break;
	case tga_format_index:
		conv_palette = buffer;
		switch(header->color_bit)
		{
		case 8:
			src_mode = cmode_index8_gray8;
			conv_buffer = buffer + header->color_count * 1;
			break;
		case 16:
			src_mode = cmode_index8_x1r5g5b5;
			conv_buffer = buffer + header->color_count * 2;
			break;
		case 24:
			src_mode = cmode_index8_r8g8b8;
			conv_buffer = buffer + header->color_count * 3;
			break;
		case 32:
			src_mode = cmode_index8_a8r8g8b8;
			conv_buffer = buffer + header->color_count * 4;
			break;
		default:
			break;
		}
		break;

	case tga_format_rle_gray:
		src_mode = cmode_rle_gray8;
		break;
	case tga_format_rle_rgb:
		switch(header->bit_count)
		{
		case 8:
			src_mode = cmode_rle_gray8;
			break;
		case 16:
			src_mode = cmode_rle_x1r5g5b5;
			break;
		case 24:
			src_mode = cmode_rle_r8g8b8;
			break;
		case 32:
			src_mode = cmode_rle_a8r8g8b8;
			break;
		default:
			break;
		}
		break;
	case tga_format_rle_index:
		//tga_rle_index(header, buffer, img, src_strike, dst_strike, conv_func);
		break;
	default:
		break;
	}

	image_convert_rule_t rule = {image_format_tga, width, height, src_mode, user_data};
	if(!pfn_rule(&rule))
		return 1;

	img->width = rule.width;
	img->height = rule.height;
	img->bits = rule.dst_bits;
	img->pitch = rule.dst_pitch;
	img->length = rule.dst_length;
	img->buffer = rule.dst_buffer;
	img->src_mode = rule.src_mode;
	img->dst_mode = rule.dst_mode;
	img->flags = flags;

	img->buffer = image_malloc(rule.dst_length);

	rule.image_convert_fun(rule.width, rule.height,
						   rule.pixel_convert_fun,
						   conv_palette, rule.pal_stride,
						   conv_buffer, rule.src_stride, rule.src_pitch,
						   img->buffer, rule.dst_stride, rule.dst_pitch, flags);
	return 0;
}

bool tga_rule_default(image_convert_rule_t * rule)
{
	if(!rule)
		return false;

	switch(rule->src_mode)
	{
	case cmode_rle_a8r8g8b8:
		rule->dst_mode = cmode_a8r8g8b8;
		rule->pixel_convert_fun = color_32_to_32;
		rule->image_convert_fun = image_convert_tga_rle8;

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
		rule->pixel_convert_fun = color_32_to_32;
		rule->image_convert_fun = image_convert_copy_ex;

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
		log2(L"[TGA]Unsupport color format : %s.", cmode_text(rule->src_mode));
		break;
	}
	return rule->image_convert_fun != nullptr;
}

void image_convert_tga_rle8(img_ix width, img_ix height,
							pixel_convert_fun_t conv_fun,
							const img_u8 * pal, img_ix pal_stride,
							const img_u8 * src, img_ix src_stride, img_ix src_pitch,
							img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
							img_ix flags)
{
	img_u8 * dst_line = dst;
	const img_8 * src_pixel = (const img_8 *)src;
	img_u8 * dst_pixel = 0;

	img_ix pitch = dst_pitch;
	if(flags & IMAGE_CONVERT_FLIP_Y)
	{
		dst_line += (height - 1) * pitch;
		pitch = -pitch;
	}

	img_ix state = 0;
	for(img_ix row = 0, col = 0; row != height; ++row)
	{
		dst_pixel = dst_line;
		for(col = 0; col != width; ++col)
		{
			if(state == 0)
			{
				state = *src_pixel++;
				if(state >= 0)
					++state;
				else
					state = -(0x81 + state);
			}
			else {}

			// 连续的、不重复的
			if(state > 0)
			{
				conv_fun(src_pixel, dst_pixel);
				dst_pixel += dst_stride;
				src_pixel += src_stride;
				--state;
			}
			// 连续、重复的
			else
			{
				conv_fun(src_pixel, dst_pixel);
				dst_pixel += dst_stride;
				++state;
				if(state == 0)
					src_pixel += src_stride;
			}
		}
		dst_line += pitch;
	}
}

err_t tga_save(const image_data_t * data, image_save_write_fun_t pfn_write, void * userdata)
{
	return tga_save_ex(data, pfn_write, userdata, tga_format_rgb);
}

err_t tga_save_ex(const image_data_t * data, image_save_write_fun_t pfn_write, void * userdata, tga_format_e format)
{
	if(!data || !pfn_write)
		return err_invalidarg;

	if(format == tga_format_rgb)
	{
		bool support = false;
		switch(data->bits)
		{
		case 8:
		case 16:
		case 24:
		case 32:
			support = true;
			break;
		default:
			break;
		}
		if(!support)
			return err_bad_format;

		tga_header_t header = {};
		header.color_type = tga_color_type_rgb;
		header.commpress = tga_format_rgb;
		header.width = (uint_16)data->width;
		header.height = (uint_16)data->height;
		header.bit_count = (uint_8)data->bits;
		header.flags = TGAF_FLIPY;

		pfn_write(&header, sizeof(tga_header_t), userdata);
		pfn_write(data->buffer, data->length, userdata);
		pfn_write(TGA_TAIL, sizeof(TGA_TAIL), userdata);// 包括 null
		return err_ok;
	}
	else
		return err_bad_format;
}

VENUS_END
