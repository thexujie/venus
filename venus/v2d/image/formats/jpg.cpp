#include "stdafx.h"
#include "jpg.h"

#include <setjmp.h>
#include <stdio.h>
#include "../libs/libjpeg/jpeglib.h"

VENUS_BEG

struct irr_jpeg_error_mgr
{
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};


void  init_source(j_decompress_ptr cinfo)
{

}
boolean fill_input_buffer(j_decompress_ptr cinfo)
{
	return true;
}
void skip_input_data(j_decompress_ptr cinfo, long count)
{
	jpeg_source_mgr * src = cinfo->src;
	if(count > 0)
	{
		src->bytes_in_buffer -= count;
		src->next_input_byte += count;
	}
}

void term_source(j_decompress_ptr cinfo)
{

}
void error_exit(j_common_ptr cinfo)
{
	(*cinfo->err->output_message) (cinfo);

	irr_jpeg_error_mgr *myerr = (irr_jpeg_error_mgr*)cinfo->err;

	longjmp(myerr->setjmp_buffer, 1);
}


void output_message(j_common_ptr cinfo)
{
	char temp1[JMSG_LENGTH_MAX];
	(*cinfo->err->format_message)(cinfo, temp1);
}

// 做 10 个字节的测试
bool is_jpg_data(const img_u8 * buffer, img_ix length)
{
	if(length < 11)
		return false;

	for(img_ix cnt = 0; cnt < sizeof(JPEG_HEADER); ++cnt)
	{
		if(buffer[cnt] != JPEG_HEADER[cnt])
			return false;
	}

	//for(int cnt = 0; cnt != 2; ++cnt)
	//{
	//	if(buffer[cnt + length - 2] != JPEG_TAIL[cnt])
	//		return false;
	//}
	return true;
}

// 创建一幅JPEG图片的字节数组
img_ix jpg_create(const img_u8 * buffer, img_ix length, image_data_t * img, image_convert_rule_fun_t pfn_match, void * user_data)
{
	if(!pfn_match)
		pfn_match = jpg_rule_default;

	img_u8 ** rows = 0;
	// 解压信息
	struct jpeg_decompress_struct cinfo;
	// 错误报告
	struct irr_jpeg_error_mgr jerr;

	cinfo.err = jpeg_std_error(&jerr.pub);
	cinfo.err->error_exit = error_exit;
	cinfo.err->output_message = output_message;

	if(setjmp(jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress(&cinfo);
		image_free(rows);
	}

	jpeg_create_decompress(&cinfo);

	jpeg_source_mgr jsrc;

	jsrc.bytes_in_buffer = length;
	jsrc.next_input_byte = (const JOCTET *)buffer;
	cinfo.src = &jsrc;

	jsrc.init_source = init_source;
	jsrc.fill_input_buffer = fill_input_buffer;
	jsrc.skip_input_data = skip_input_data;
	jsrc.term_source = term_source;
	jsrc.resync_to_restart = jpeg_resync_to_restart;

	jpeg_read_header(&cinfo, TRUE);

	cinfo.out_color_space = JCS_RGB;
	cinfo.out_color_components = 3;
	cinfo.do_fancy_upsampling = FALSE;

	jpeg_start_decompress(&cinfo);

	img_ix width = (img_ix)cinfo.image_width;
	img_ix height = (img_ix)cinfo.image_height;

	image_convert_rule_t rule = {image_format_jpg, width, height, cmode_b8g8r8, user_data};
	if(!pfn_match(&rule))
	{
		//jpeg_finish_decompress(&cinfo);
		jpeg_destroy_decompress(&cinfo);
		return 1;
	}

	img_ix src_pitch = align_to_4(width * 3);
	img_ix src_length = src_pitch * height;
	img_u8 * src_buffer = image_malloc(src_length);

	rows = (img_u8 **)image_malloc(sizeof(img_u8 *)* height);
	for(img_ix cnt = 0; cnt < height; ++cnt)
		rows[cnt] = src_buffer + cnt * src_pitch;

	img_u32 rowsRead = 0;
	// 每次读取一行
	while(cinfo.output_scanline < cinfo.output_height)
		rowsRead += jpeg_read_scanlines(&cinfo, &rows[rowsRead], cinfo.output_height - rowsRead);
	
	image_free(rows);
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	if(rule.image_convert_fun == image_convert_jpeg_use_src)
	{
		img->width = rule.width;
		img->height = rule.height;
		img->bits = rule.dst_bits;
		img->pitch = rule.dst_pitch;
		img->length = rule.dst_length;
		img->buffer = rule.dst_buffer;
		img->src_mode = rule.src_mode;
		img->dst_mode = rule.dst_mode;
		img->flags = 0;
		img->buffer = src_buffer;
	}
	else
	{
		img->width = rule.width;
		img->height = rule.height;
		img->bits = rule.dst_bits;
		img->pitch = rule.dst_pitch;
		img->length = rule.dst_length;
		img->buffer = rule.dst_buffer;
		img->src_mode = rule.src_mode;
		img->dst_mode = rule.dst_mode;
		img->flags = 0;
		img->buffer = image_malloc(rule.dst_length);

		rule.image_convert_fun(rule.width, rule.height,
							   rule.pixel_convert_fun,
							   nullptr, rule.pal_stride,
							   src_buffer, rule.src_stride, rule.src_pitch,
							   img->buffer, rule.dst_stride, rule.dst_pitch, 0);
		image_free(src_buffer);
	}
	return 0;
}

bool jpg_rule_default(image_convert_rule_t * rule)
{
	return false;
}

VENUS_END
