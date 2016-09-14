#include "stdafx.h"
#include "image_converter_yuv.h"

VENUS_BEG

void image_convert_yuv_to_rgb(image_data_t * src)
{
	color_yuv_t * yuv = (color_yuv_t *)(src->buffer);
	color_rgb_t rgb;

	for(img_ix row = 0, col = 0; row != src->height; ++row)
	{
		for(col = 0; col != src->width; ++col)
		{
			color_yuv_to_r8g8b8(yuv, &rgb);
			yuv->y = rgb.b;
			yuv->u = rgb.g;
			yuv->v = rgb.r;
			++yuv;
		}
	}
	src->dst_mode = cmode_r8g8b8;
}

void image_convert_yuv_to_argb(image_data_t * src)
{
	img_u8 * temp = src->buffer;
	color_yuv_t * yuv = (color_yuv_t *)(src->buffer);
	color_argb_t * argb = 0;

	src->length = (src->width * src->height) << 2;
	src->buffer = new img_u8[src->length];
	argb = (color_argb_t *)(src->buffer);

	for(img_ix row = 0, col = 0; row != src->height; ++row)
	{
		for(col = 0; col != src->width; ++col)
		{
			color_yuv_to_a8r8g8b8(yuv, argb);
			++yuv;
			++argb;
		}
	}
	delete[] temp;
	src->dst_mode = cmode_a8r8g8b8;
}

VENUS_END
