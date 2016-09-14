#include "stdafx.h"
#include "image_converter.h"

#include "converter/image_converter_yuv.h"
#include "converter/image_converter_ycck.h"
#include "converter/image_converter_gray.h"
#include "converter/image_converter_cmyk.h"
#include "converter/image_converter_rgb.h"

VENUS_BEG

void image_convert_ex(img_ix width, img_ix height,
					  pixel_convert_fun_t conv_fun, 
					  const img_u8 *, img_ix,
					  const img_u8 * src, img_ix src_stride, img_ix src_pitch,
					  img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
					  img_ix flags)
{
	const img_u8 * src_line = src;
	const img_u8 * src_pixel = 0;
	img_u8 * dst_line = dst;
	img_u8 * dst_pixel = 0;

	img_ix pitch = dst_pitch;
	if(flags & IMAGE_CONVERT_FLIP_Y)
	{
		dst_line += (height - 1) * dst_pitch;
		pitch = -dst_pitch;
	}

	for(img_ix row = 0, col = 0; row != height; ++row)
	{
		src_pixel = src_line;
		dst_pixel = dst_line;
		for(col = 0; col != width; ++col)
		{
			conv_fun(src_pixel, dst_pixel);
			src_pixel += src_stride;
			dst_pixel += dst_stride;
		}
		src_line += src_pitch;
		dst_line += pitch;
	}
}
void image_convert_fill_ex(img_ix width, img_ix height,
						   pixel_convert_fun_t conv_fun, const img_u8 * palette,
						   const img_u8 * src, img_ix /*src_stride*/, img_ix /*src_pitch*/,
						   img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
						   img_ix flags)
{
	img_u8 * dst_line = dst;
	img_u8 * dst_pixel = 0;

	img_ix pitch = dst_pitch;
	if(flags & IMAGE_CONVERT_FLIP_Y)
	{
		dst_line += (height - 1) * dst_pitch;
		pitch = -pitch;
	}

	for(img_ix row = 0, col = 0; row != height; ++row)
	{
		dst_pixel = dst_line;
		for(col = 0; col != width; ++col)
		{
			conv_fun(src, dst_pixel);
			++dst_pixel;
		}
		dst_line += pitch;
	}
}

void image_convert_copy_ex(img_ix /*width*/, img_ix height,
						   pixel_convert_fun_t /*conv_fun*/, 
						   const img_u8 * /*pal*/, img_ix /*pal_stride*/,
						   const img_u8 * src, img_ix /*src_strike*/, img_ix src_pitch,
						   img_u8 * dst, img_ix /*dst_strike*/, img_ix dst_pitch,
						   img_ix flags)
{
	const img_u8 * src_line = src;
	const img_u8 * src_pixel = 0;
	img_u8 * dst_line = dst;
	img_u8 * dst_pixel = 0;

	img_ix pitch = src_pitch ? src_pitch : dst_pitch;
	if(flags & IMAGE_CONVERT_FLIP_Y)
	{
		src_line += (height - 1) * src_pitch;
		pitch = -pitch;
	}

	for(img_ix row = 0; row != height; ++row)
	{
		src_pixel = src_line;
		dst_pixel = dst_line;
		image_memcpy(dst_line, src_pitch, src_line, src_pitch);
		src_line += pitch;
		dst_line += src_pitch;
	}
}

void image_convert_index1_ex(img_ix width, img_ix height,
							 pixel_convert_fun_t conv_fun, 
							 const img_u8 * pal, img_ix pal_stride,
							 const img_u8 * src, img_ix src_stride, img_ix src_pitch,
							 img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
							 img_ix flags)
{
	const img_u8 * src_line = src;
	const img_u8 * src_pixel = 0;
	img_u8 * dst_line = dst;
	img_u8 * dst_pixel = 0;

	if(flags & IMAGE_CONVERT_FLIP_Y)
	{
		src_line += (height - 1) * src_pitch;
		src_pitch = -src_pitch;
	}

	for(img_ix row = 0, col = 0; row != height; ++row)
	{
		src_pixel = src_line;
		dst_pixel = dst_line;
		for(col = 0; col != width; ++col)
		{
#pragma pack(push, 1)
			union
			{
				struct
				{
					img_u8 v7 : 1;
					img_u8 v6 : 1;
					img_u8 v5 : 1;
					img_u8 v4 : 1;
					img_u8 v3 : 1;
					img_u8 v2 : 1;
					img_u8 v1 : 1;
					img_u8 v0 : 1;
				};
				img_u8 v;
			};

#pragma pack(pop)
			switch(col & 0x7)
			{
			case 0:
				v = *src_pixel;
				conv_fun(pal + v0 * pal_stride, dst_pixel);
				break;
			case 1:
				conv_fun(pal + v1 * pal_stride, dst_pixel);
				break;
			case 2:
				conv_fun(pal + v2 * pal_stride, dst_pixel);
				break;
			case 3:
				conv_fun(pal + v3 * pal_stride, dst_pixel);
				break;
			case 4:
				conv_fun(pal + v4 * pal_stride, dst_pixel);
				break;
			case 5:
				conv_fun(pal + v5 * pal_stride, dst_pixel);
				break;
			case 6:
				conv_fun(pal + v6 * pal_stride, dst_pixel);
				break;
			case 7:
				conv_fun(pal + v7 * pal_stride, dst_pixel);
				src_pixel += src_stride;
				break;
			default:
				break;
			}
			dst_pixel += dst_stride;
		}
		src_line += src_pitch;
		dst_line += dst_pitch;
	}
}

void image_convert_index2_ex(img_ix width, img_ix height,
							 pixel_convert_fun_t conv_fun, 
							 const img_u8 * pal, img_ix pal_stride,
							 const img_u8 * src, img_ix src_stride, img_ix src_pitch,
							 img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
							 img_ix flags)
{
	const img_u8 * src_line = src;
	const img_u8 * src_pixel = 0;
	img_u8 * dst_line = dst;
	img_u8 * dst_pixel = 0;

	if(flags & IMAGE_CONVERT_FLIP_Y)
	{
		src_line += (height - 1) * src_pitch;
		src_pitch = -src_pitch;
	}

	for(img_ix row = 0, col = 0; row != height; ++row)
	{
		src_pixel = src_line;
		dst_pixel = dst_line;
		for(col = 0; col != width; ++col)
		{
#pragma pack(push, 1)
			union
			{
				struct
				{
					img_u8 d : 2;
					img_u8 c : 2;
					img_u8 b : 2;
					img_u8 a : 2;
				};
				img_u8 v;
			};
#pragma pack(pop)
			switch(col & 0x3)
			{
			case 0:
				v = *src_pixel;
				conv_fun(pal + a * pal_stride, dst_pixel);
				break;
			case 1:
				conv_fun(pal + b * pal_stride, dst_pixel);
				break;
			case 2:
				conv_fun(pal + c * pal_stride, dst_pixel);
				break;
			case 3:
				conv_fun(pal + d * pal_stride, dst_pixel);
				src_pixel += src_stride;
				break;
			default:
				break;
			}
			dst_pixel += dst_stride;
		}
		src_line += src_pitch;
		dst_line += dst_pitch;
	}
}

void image_convert_index4_ex(img_ix width, img_ix height,
							 pixel_convert_fun_t conv_fun, 
							 const img_u8 * pal, img_ix pal_stride,
							 const img_u8 * src, img_ix src_stride, img_ix src_pitch,
							 img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
							 img_ix flags)
{
	const img_u8 * src_line = src;
	const img_u8 * src_pixel = 0;
	img_u8 * dst_line = dst;
	img_u8 * dst_pixel = 0;

	if(flags & IMAGE_CONVERT_FLIP_Y)
	{
		src_line += (height - 1) * src_pitch;
		src_pitch = -src_pitch;
	}

	for(img_ix row = 0, col = 0; row != height; ++row)
	{
		src_pixel = src_line;
		dst_pixel = dst_line;
		for(col = 0; col != width; ++col)
		{
#pragma pack(push, 1)
			union
			{
				struct
				{
					img_u8 b : 4;
					img_u8 a : 4;
				};
				img_u8 v;
			};
#pragma pack(pop)
			if(col & 0x1)
			{
				conv_fun(pal + b * pal_stride, dst_pixel);
				src_pixel += src_stride;
			}
			else
			{
				v = *src_pixel;
				conv_fun(pal + a * pal_stride, dst_pixel);
			}
			dst_pixel += dst_stride;
		}
		src_line += src_pitch;
		dst_line += dst_pitch;
	}
}

void image_convert_index8_ex(img_ix width, img_ix height,
							 pixel_convert_fun_t conv_fun, 
							 const img_u8 * pal, img_ix pal_stride,
							 const img_u8 * src, img_ix src_stride, img_ix src_pitch,
							 img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
							 img_ix flags)
{
	const img_u8 * src_line = src;
	const img_u8 * src_pixel = 0;
	img_u8 * dst_line = dst;
	img_u8 * dst_pixel = 0;

	if(flags & IMAGE_CONVERT_FLIP_Y)
	{
		src_line += (height - 1) * src_pitch;
		src_pitch = -src_pitch;
	}

	for(img_ix row = 0, col = 0; row != height; ++row)
	{
		src_pixel = src_line;
		dst_pixel = dst_line;
		for(col = 0; col != width; ++col)
		{
			conv_fun(pal + *src_pixel * pal_stride, dst_pixel);
			src_pixel += src_stride;
			dst_pixel += dst_stride;
		}
		src_line += src_pitch;
		dst_line += dst_pitch;
	}
}

VENUS_END
