#include "stdafx.h"
#include "image_converter_ycck.h"

VENUS_BEG

void image_convert_ycck_to_rgb(image_data_t * src)
{
	img_u8 * temp = src->buffer;
	src->length = (src->width * src->height) * 3;
	src->buffer = image_malloc(src->length);
	color_rgb_t * rgb = (color_rgb_t *)(src->buffer);

	for(img_u8 row = 0, col = 0; row != src->height; ++row)
	{
		for(col = 0; col != src->width; ++col)
		{
			rgb->r = 0x80;
			rgb->g = 0x80;
			rgb->b = 0x80;
			++rgb;
		}
	}
	src->dst_mode = cmode_r8g8b8;
	delete[] temp;
}
void image_convert_ycck_to_argb(image_data_t * src)
{
	color_ycck_t * ycck = (color_ycck_t *)(src->buffer);

	for(img_u8 row = 0, col = 0; row != src->height; ++row)
	{
		for(col = 0; col != src->width; ++col)
		{
			//ycck_to_argb(ycck, &argb);
			ycck->y = 0x80;
			ycck->b = 0x80;
			ycck->r = 0x80;
			ycck->k = 0xFF;
			++ycck;
		}
	}
	src->dst_mode = cmode_a8r8g8b8;
}

VENUS_END
