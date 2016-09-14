#include "stdafx.h"
#include "bitmap.h"

VENUS_BEG

err_t icon_create(HICON hIcon, image_data_t * img)
{
	ICONINFO ic;
	::GetIconInfo(hIcon, &ic);
	return bitmap_create(ic.hbmColor, img);
}

err_t bitmap_create(HBITMAP hBitmap, image_data_t * img)
{
	BITMAP bitmap;
	::GetObject(hBitmap, sizeof(BITMAP), &bitmap);
	cmode_e src_mode = cmode_invalid;
	int_x src_length = align_to_4(bitmap.bmWidthBytes) * bitmap.bmHeight;
	byte_t * src = image_malloc(src_length);

	::GetBitmapBits(hBitmap, (int_32)src_length, src);
	byte_t * pallete = nullptr;
	switch(bitmap.bmBitsPixel)
	{
	case 4:
		src_mode = cmode_index4_a8r8g8b8;
		pallete = src;
		src = pallete + 16 * 4;
		break;
	case 8:
		src_mode = cmode_index8_a8r8g8b8;
		pallete = src;
		src = pallete + 256 * 4;
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
		image_free(src);
		return 1;
	}

	int_x width = (int_x)bitmap.bmWidth;
	int_x height = (int_x)bitmap.bmHeight;
	image_convert_rule_t rule = {image_format_bmp, width, height, src_mode, nullptr};
	if(bmp_rule_default(&rule))
	{
		image_data_t bmp;
		image_clear(&bmp, sizeof(image_data_t));
		bmp.width = width;
		bmp.height = height;

		bmp.bits = rule.dst_bits;
		bmp.pitch = rule.dst_pitch;
		bmp.length = rule.dst_length;
		bmp.buffer = rule.dst_buffer;
		bmp.dst_mode = rule.dst_mode;
		bmp.src_mode = rule.src_mode;
		bmp.flags = 0;
		bmp.buffer = image_malloc(rule.dst_length);

		rule.image_convert_fun(bmp.width, bmp.height,
							   rule.pixel_convert_fun, 
							   pallete, rule.pal_stride,
							   src, rule.src_stride, rule.src_pitch,
							   bmp.buffer, rule.dst_stride, rule.dst_pitch, 0);
		*img = bmp;
		image_free(src);
		return 0;
	}
	else
	{
		image_free(src);
		buffclr(*img);
		return 2;
	}
}

VENUS_END
