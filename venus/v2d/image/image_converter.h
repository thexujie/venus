#pragma once

VENUS_BEG

// ת��ʱ�Ƿ���Ҫ��ת����
const img_ix IMAGE_CONVERT_FLIP_Y = 0x1;

/// ��������֧����ɫ֮����໥ת��
typedef void(*image_convert_fun_t)(img_ix width, img_ix height,
								   pixel_convert_fun_t conv_fun, const img_u8 * palette, img_ix pal_stride,
								   const img_u8 * src, img_ix src_strike, img_ix src_pitch,
								   img_u8 * dst, img_ix dst_strike, img_ix dst_pitch, img_ix flags);

struct image_convert_rule_t
{
	image_format_e format;
	//----------------------------------- ���벿��
	img_ix width;
	img_ix height;
	cmode_e src_mode;
	void * user_data;
	img_ix user_format;

	//-----------------------------------�������
	cmode_e dst_mode;

	pixel_convert_fun_t pixel_convert_fun;
	image_convert_fun_t image_convert_fun;

	//! ��ɫ�����ɫλ��������е�ɫ��Ļ���
	img_ix pal_bits;
	//! ��ɫ�����ɫ��ȣ�����е�ɫ��Ļ���
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
 * �� src ��ָ������ɫ��� dst��
 * src ֻ�ṩ 1 �����ص���ɫ��Ϣ��
 */
IMAGE_API void image_convert_fill_ex(img_ix width, img_ix height,
									 pixel_convert_fun_t conv_fun, const img_u8 * palette,
									 const img_u8 * src, img_ix /*src_stride*/, img_ix /*src_pitch*/,
									 img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
									 img_ix flags);

/**
 * ����ͼ��dst �� src ������ͬ����ɫ��ʽ��
 * Ŀ��ͼ��� width �� height ����С�� Դͼ��
 */
IMAGE_API void image_convert_copy_ex(img_ix /*width*/, img_ix height,
									 pixel_convert_fun_t /*conv_fun*/,
									 const img_u8 * /*pal*/, img_ix /*pal_stride*/,
									 const img_u8 * src, img_ix /*src_stride*/, img_ix src_pitch,
									 img_u8 * dst, img_ix /*dst_stride*/, img_ix dst_pitch,
									 img_ix flags);

/**
 * image_convert_ex ֻ�ܽ�һ�� rgb ��ʽת������һ�� rgb ��ʽ��
 * ���ܽ��е�ɫ�塢ѹ���Ȳ�����
 */
IMAGE_API void image_convert_ex(img_ix width, img_ix height,
								pixel_convert_fun_t conv_fun,
								const img_u8 * /*pal*/, img_ix /*pal_stride*/,
								const img_u8 * src, img_ix src_stride, img_ix src_pitch,
								img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
								img_ix flags);

/**
* ÿ��ȡһ���ֽڣ����� 8 �����ء�ͬʱ��src ǰ�� src_stride��dst ǰ�� dst_stride * 8��
* ÿ��ȡһ�У�src_line ǰ�� src_pitch��dst_line ǰ�� dst_pitch��
* @param width ͼ��Ŀ��
* @param height ͼ��ĸ߶�
* @param conv_fun ���صĸ�ʽת��
* @param pal ��ɫ�壬����Ϊ nullptr
* @param src ��������
* @param src_stride ÿ��ȡһ���ֽڣ����� 8 �����أ���src ǰ�����ֽڴ�С��
* @param src_pitch ÿ��ȡһ�У����� width �����أ���src ǰ�����ֽڴ�С��
* @param dst ���Ŀ��
* @param dst_stride ÿ����һ�����أ�dst ǰ�����ֽڴ�С
* @param dst_pitch ÿ������һ�У����� width �����أ���src ǰ�����ֽڴ�С��
* @param flags ��־λ���μ�...
*/
IMAGE_API void image_convert_index1_ex(img_ix width, img_ix height,
									   pixel_convert_fun_t conv_fun,
									   const img_u8 * pal, img_ix pal_stride,
									   const img_u8 * src, img_ix src_stride, img_ix src_pitch,
									   img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
									   img_ix flags);

/**
* ÿ��ȡһ���ֽڣ����� 4 �����ء�ͬʱ��src ǰ�� src_stride��dst ǰ�� dst_stride * 4��
* ÿ��ȡһ�У�src_line ǰ�� src_pitch��dst_line ǰ�� dst_pitch��
* @param width ͼ��Ŀ��
* @param height ͼ��ĸ߶�
* @param conv_fun ���صĸ�ʽת��
* @param pal ��ɫ�壬����Ϊ nullptr
* @param src ��������
* @param src_stride ÿ��ȡһ���ֽڣ����� 4 �����أ���src ǰ�����ֽڴ�С��
* @param src_pitch ÿ��ȡһ�У����� width �����أ���src ǰ�����ֽڴ�С��
* @param dst ���Ŀ��
* @param dst_stride ÿ����һ�����أ�dst ǰ�����ֽڴ�С
* @param dst_pitch ÿ������һ�У����� width �����أ���src ǰ�����ֽڴ�С��
* @param flags ��־λ���μ�...
*/
IMAGE_API void image_convert_index2_ex(img_ix width, img_ix height,
									   pixel_convert_fun_t conv_fun,
									   const img_u8 * pal, img_ix pal_stride,
									   const img_u8 * src, img_ix src_stride, img_ix src_pitch,
									   img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
									   img_ix flags);

/**
 * ÿ��ȡһ���ֽڣ����� 2 �����ء�ͬʱ��src ǰ�� src_stride��dst ǰ�� dst_stride * 2��
 * ÿ��ȡһ�У�src_line ǰ�� src_pitch��dst_line ǰ�� dst_pitch��
 * @param width ͼ��Ŀ��
 * @param height ͼ��ĸ߶�
 * @param conv_fun ���صĸ�ʽת��
 * @param pal ��ɫ�壬����Ϊ nullptr
 * @param src ��������
 * @param src_stride ÿ��ȡһ���ֽڣ������������أ���src ǰ�����ֽڴ�С��
 * @param src_pitch ÿ��ȡһ�У����� width �����أ���src ǰ�����ֽڴ�С��
 * @param dst ���Ŀ��
 * @param dst_stride ÿ����һ�����أ�dst ǰ�����ֽڴ�С
 * @param dst_pitch ÿ������һ�У����� width �����أ���src ǰ�����ֽڴ�С��
 * @param flags ��־λ���μ�...
 */
IMAGE_API void image_convert_index4_ex(img_ix width, img_ix height,
									   pixel_convert_fun_t conv_fun,
									   const img_u8 * pal, img_ix pal_stride,
									   const img_u8 * src, img_ix src_stride, img_ix src_pitch,
									   img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
									   img_ix flags);
/**
* ÿ��ȡһ���ֽڣ����� 1 �����ء�ͬʱ��src ǰ�� src_stride��dst ǰ�� dst_stride * 1��
* ÿ��ȡһ�У�src_line ǰ�� src_pitch��dst_line ǰ�� dst_pitch��
* @param width ͼ��Ŀ��
* @param height ͼ��ĸ߶�
* @param conv_fun ���صĸ�ʽת��
* @param pal ��ɫ�壬����Ϊ nullptr
* @param src ��������
* @param src_stride ÿ��ȡһ���ֽڣ����� 1 �����أ���src ǰ�����ֽڴ�С��
* @param src_pitch ÿ��ȡһ�У����� width �����أ���src ǰ�����ֽڴ�С��
* @param dst ���Ŀ��
* @param dst_stride ÿ����һ�����أ�dst ǰ�����ֽڴ�С
* @param dst_pitch ÿ������һ�У����� width �����أ���src ǰ�����ֽڴ�С��
* @param flags ��־λ���μ�...
*/
IMAGE_API void image_convert_index8_ex(img_ix width, img_ix height,
									   pixel_convert_fun_t conv_fun,
									   const img_u8 * pal, img_ix pal_stride,
									   const img_u8 * src, img_ix src_stride, img_ix src_pitch,
									   img_u8 * dst, img_ix dst_stride, img_ix dst_pitch,
									   img_ix flags);

VENUS_END
