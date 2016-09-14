#pragma once

VENUS_BEG

// 转换时是否需要翻转行序
const img_ix IMAGE_CONVERT_FLIP_Y = 0x1;

/// 以下用于支持颜色之间的相互转换
typedef void(*image_convert_fun_t)(img_ix width, img_ix height,
								   pixel_convert_fun_t conv_fun, const img_u8 * palette, img_ix pal_stride,
								   const img_u8 * src, img_ix src_strike, img_ix src_pitch,
								   img_u8 * dst, img_ix dst_strike, img_ix dst_pitch, img_ix flags);

struct image_convert_rule_t
{
	image_format_e format;
	//----------------------------------- 输入部分
	img_ix width;
	img_ix height;
	cmode_e src_mode;
	void * user_data;
	img_ix user_format;

	//-----------------------------------输出部分
	cmode_e dst_mode;

	pixel_convert_fun_t pixel_convert_fun;
	image_convert_fun_t image_convert_fun;

	//! 调色板的颜色位数（如果有调色板的话）
	img_ix pal_bits;
	//! 调色板的颜色跨度（如果有调色板的话）
	img_ix pal_stride;

	img_ix src_bits;
	img_ix src_stride;
	img_ix src_pitch;

	img_ix dst_bits;
	img_ix dst_stride;
	img_ix dst_pitch;

	img_ix dst_length;
	img_u8 * dst_buffer;
};

typedef bool(*image_convert_rule_fun_t)(image_convert_rule_t * rule);

/**
 * 用 src 所指定的颜色填充 dst。
 * src 只提供 1 个像素的颜色信息。
 */
IMAGE_API void image_convert_fill_ex(img_ix width, img_ix height,
									 pixel_convert_fun_t conv_fun, const img_u8 * palette,
									 const img_u8 * src, img_ix /*src_stride*/, img_ix /*src_pitch*/,
									 img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
									 img_ix flags);

/**
 * 复制图像，dst 和 src 具有相同的颜色格式。
 * 目标图像的 width 和 height 不能小于 源图像。
 */
IMAGE_API void image_convert_copy_ex(img_ix /*width*/, img_ix height,
									 pixel_convert_fun_t /*conv_fun*/,
									 const img_u8 * /*pal*/, img_ix /*pal_stride*/,
									 const img_u8 * src, img_ix /*src_stride*/, img_ix src_pitch,
									 img_u8 * dst, img_ix /*dst_stride*/, img_ix dst_pitch,
									 img_ix flags);

/**
 * image_convert_ex 只能将一种 rgb 格式转换成另一种 rgb 格式，
 * 不能进行调色板、压缩等操作。
 */
IMAGE_API void image_convert_ex(img_ix width, img_ix height,
								pixel_convert_fun_t conv_fun,
								const img_u8 * /*pal*/, img_ix /*pal_stride*/,
								const img_u8 * src, img_ix src_stride, img_ix src_pitch,
								img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
								img_ix flags);

/**
* 每读取一个字节，生成 8 个像素。同时，src 前进 src_stride，dst 前进 dst_stride * 8。
* 每读取一行，src_line 前进 src_pitch，dst_line 前进 dst_pitch。
* @param width 图像的宽度
* @param height 图像的高度
* @param conv_fun 像素的格式转换
* @param pal 调色板，可以为 nullptr
* @param src 索引数组
* @param src_stride 每读取一个字节（生成 8 个像素），src 前进的字节大小。
* @param src_pitch 每读取一行（生成 width 个像素），src 前进的字节大小。
* @param dst 输出目标
* @param dst_stride 每生成一个像素，dst 前进的字节大小
* @param dst_pitch 每读生成一行（生成 width 个像素），src 前进的字节大小。
* @param flags 标志位，参见...
*/
IMAGE_API void image_convert_index1_ex(img_ix width, img_ix height,
									   pixel_convert_fun_t conv_fun,
									   const img_u8 * pal, img_ix pal_stride,
									   const img_u8 * src, img_ix src_stride, img_ix src_pitch,
									   img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
									   img_ix flags);

/**
* 每读取一个字节，生成 4 个像素。同时，src 前进 src_stride，dst 前进 dst_stride * 4。
* 每读取一行，src_line 前进 src_pitch，dst_line 前进 dst_pitch。
* @param width 图像的宽度
* @param height 图像的高度
* @param conv_fun 像素的格式转换
* @param pal 调色板，可以为 nullptr
* @param src 索引数组
* @param src_stride 每读取一个字节（生成 4 个像素），src 前进的字节大小。
* @param src_pitch 每读取一行（生成 width 个像素），src 前进的字节大小。
* @param dst 输出目标
* @param dst_stride 每生成一个像素，dst 前进的字节大小
* @param dst_pitch 每读生成一行（生成 width 个像素），src 前进的字节大小。
* @param flags 标志位，参见...
*/
IMAGE_API void image_convert_index2_ex(img_ix width, img_ix height,
									   pixel_convert_fun_t conv_fun,
									   const img_u8 * pal, img_ix pal_stride,
									   const img_u8 * src, img_ix src_stride, img_ix src_pitch,
									   img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
									   img_ix flags);

/**
 * 每读取一个字节，生成 2 个像素。同时，src 前进 src_stride，dst 前进 dst_stride * 2。
 * 每读取一行，src_line 前进 src_pitch，dst_line 前进 dst_pitch。
 * @param width 图像的宽度
 * @param height 图像的高度
 * @param conv_fun 像素的格式转换
 * @param pal 调色板，可以为 nullptr
 * @param src 索引数组
 * @param src_stride 每读取一个字节（生成两个像素），src 前进的字节大小。
 * @param src_pitch 每读取一行（生成 width 个像素），src 前进的字节大小。
 * @param dst 输出目标
 * @param dst_stride 每生成一个像素，dst 前进的字节大小
 * @param dst_pitch 每读生成一行（生成 width 个像素），src 前进的字节大小。
 * @param flags 标志位，参见...
 */
IMAGE_API void image_convert_index4_ex(img_ix width, img_ix height,
									   pixel_convert_fun_t conv_fun,
									   const img_u8 * pal, img_ix pal_stride,
									   const img_u8 * src, img_ix src_stride, img_ix src_pitch,
									   img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
									   img_ix flags);
/**
* 每读取一个字节，生成 1 个像素。同时，src 前进 src_stride，dst 前进 dst_stride * 1。
* 每读取一行，src_line 前进 src_pitch，dst_line 前进 dst_pitch。
* @param width 图像的宽度
* @param height 图像的高度
* @param conv_fun 像素的格式转换
* @param pal 调色板，可以为 nullptr
* @param src 索引数组
* @param src_stride 每读取一个字节（生成 1 个像素），src 前进的字节大小。
* @param src_pitch 每读取一行（生成 width 个像素），src 前进的字节大小。
* @param dst 输出目标
* @param dst_stride 每生成一个像素，dst 前进的字节大小
* @param dst_pitch 每读生成一行（生成 width 个像素），src 前进的字节大小。
* @param flags 标志位，参见...
*/
IMAGE_API void image_convert_index8_ex(img_ix width, img_ix height,
									   pixel_convert_fun_t conv_fun,
									   const img_u8 * pal, img_ix pal_stride,
									   const img_u8 * src, img_ix src_stride, img_ix src_pitch,
									   img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
									   img_ix flags);

VENUS_END
