#include "stdafx.h"
#include "image_converter_rgb.h"

VENUS_BEG

void image_converter_rgb_to_argb(image_data_t * img)
{
	img_u8 * line = img->buffer;
	color_rgb_t * rgb = 0;
	img_u8 * buffer = image_malloc(img->width * img->height << 2);
	color_argb_t * argb = (color_argb_t *)buffer;
	for(img_ix row = 0, col = 0; row != img->height; ++row)
	{
		rgb = (color_rgb_t *)line;
		for(col = 0; col != img->width; ++col)
		{
			argb->rgb = *rgb;
			argb->a = 0xFF;
			++argb;
			++rgb;
		}
		line += img->pitch;
	}

	delete[] img->buffer;

	img->buffer = buffer;
	img->dst_mode = cmode_a8r8g8b8;
	img->pitch = img->width << 2;
	img->length = img->height * img->pitch;
}


void image_converter_bgr_to_rgb(image_data_t * img)
{
	img_u8 * line = img->buffer;
	color_rgb_t * src = 0;
	img_u8 temp = 0;
	for(img_ix row = 0, col = 0; row != img->height; ++row)
	{
		src = (color_rgb_t *)line;
		for(col = 0; col != img->width; ++col, ++src)
		{
			temp = src->r;
			src->r = src->b;
			src->b = temp;
		}
		line += img->pitch;
	}

	img->dst_mode = cmode_r8g8b8;
}

VENUS_END
