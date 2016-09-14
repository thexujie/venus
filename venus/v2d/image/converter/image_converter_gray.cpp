#include "stdafx.h"
#include "image_converter_gray.h"

VENUS_BEG

void image_convert_gray_to_rgb(image_data_t * src)
{
	img_u8 * temp = src->buffer;
	img_u8 * gray = src->buffer;
	color_rgb_t * rgb = 0;

	src->length = (src->width * src->height) * 3;
	src->buffer = image_malloc(src->length);
	rgb = (color_rgb_t *)(src->buffer);

	for(img_ix row = 0, col = 0; row != src->height; ++row)
	{
		for(col = 0; col != src->width; ++col)
		{
			color_gray_to_r8g8b8(gray, rgb);
			++gray;
			++rgb;
		}
	}
	delete[] temp;
	src->dst_mode = cmode_r8g8b8;
}
void image_convert_gray_to_argb(image_data_t * src)
{
	img_u8 * temp = src->buffer;
	img_u8 * gray = src->buffer;
	color_argb_t * argb = 0;

	src->length = (src->width * src->height) << 2;
	src->buffer = image_malloc(src->length);
	argb = (color_argb_t *)(src->buffer);

	for(img_ix row = 0, col = 0; row != src->height; ++row)
	{
		for(col = 0; col != src->width; ++col)
		{
			color_gray_to_a8r8g8b8(gray, argb);
			++gray;
			++argb;
		}
	}
	delete[] temp;
	src->dst_mode = cmode_a8r8g8b8;
}

VENUS_END
