#include "stdafx.h"
#include "image_converter_cmyk.h"

VENUS_BEG

void image_convert_cmyk_to_rgb(image_data_t * src)
{
	src->pitch = align_to_4(src->width * 3);
	img_u8 * temp = src->buffer;
	color_cmyk_t * cmyk = (color_cmyk_t *)(src->buffer);
	src->length = src->height * src->pitch;
	src->buffer = image_malloc(src->length);
	img_u8 * line = src->buffer;
	color_rgb_t * rgb = 0;

	for(img_ix row = 0, col = 0; row != src->height; ++row)
	{
		rgb = (color_rgb_t *)line;
		for(col = 0; col != src->width; ++col)
		{
			color_cmyk_to_r8g8b8(cmyk, rgb);
			++cmyk;
			++rgb;
		}
		line += src->pitch;
	}

	src->dst_mode = cmode_r8g8b8;
	delete[] temp;
}
void image_convert_cmyk_to_argb(image_data_t * src)
{
	color_cmyk_t * cmyk = (color_cmyk_t *)(src->buffer);
	color_argb_t argb;
	for(img_ix row = 0, col = 0; row != src->height; ++row)
	{
		for(col = 0; col != src->width; ++col)
		{
			color_cmyk_to_a8r8g8b8(cmyk, &argb);
			cmyk->k = argb.a;
			cmyk->y = argb.r;
			cmyk->m = argb.g;
			cmyk->c = argb.b;
			++cmyk;
		}
	}
	src->dst_mode = cmode_a8r8g8b8;
}


VENUS_END
