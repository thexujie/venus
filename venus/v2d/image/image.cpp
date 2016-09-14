#include "stdafx.h"
#include "image.h"
#include <io.h>

VENUS_BEG

IMAGE_API img_ix image_load(const char_x * file, image_data_t * img, image_format_e image_format,
	image_convert_rule_fun_t pfn_rule, void * user_data)
{
	int_32 fileid = 0;
	int_32 mode = 0x8120; // _O_RDONLY | _O_BINARY | _S_IREAD | _SH_DENYWR;
	err_t err = _wsopen_s(&fileid, file, 0x8000, 0x20, 0x100);
	if(err)
		return err_no_file;

	_lseek(fileid, 0, SEEK_END);
	int_32 length = _tell(fileid);
	_lseek(fileid, 0, SEEK_SET);

	img_u8 * buffer = image_malloc(length);

	int_x read = (int_x)_read(fileid, buffer, length);
	_close(fileid);
	fileid = 0;

	if(read == length)
		err = image_create(buffer, length, img, image_format, pfn_rule, user_data);
	else
		err = err_bad_data;
	image_free(buffer);
	return err;
}

image_format_e image_get_format(const img_u8 * buffer, img_ix length)
{
	if(is_bmp_data(buffer, length))
		return image_format_bmp;
	else if(is_png_data(buffer, length))
		return image_format_png;
	else if(is_jpg_data(buffer, length))
		return image_format_jpg;
	else if(is_tga_data(buffer, length))
		return image_format_tga;
	else if(is_dds_data(buffer, length))
		return image_format_dds;
	else
		return image_format_invalid;
}

img_ix image_create(const img_u8 * buffer, img_ix length, image_data_t * img, image_format_e image_format, 
					image_convert_rule_fun_t pfn_rule, void * user_data)
{
	if(image_format == image_format_invalid)
		image_format = image_get_format(buffer, length);

	switch(image_format)
	{
	case image_format_bmp:
		return bmp_create(buffer, length, img, pfn_rule, user_data);
	case image_format_tga:
		return tga_create(buffer, length, img, pfn_rule, user_data);
	case image_format_png:
		return png_create(buffer, length, img, pfn_rule, user_data);
	case image_format_jpg:
		return jpg_create(buffer, length, img, pfn_rule, user_data);
	case image_format_dds:
		return dds_create(buffer, length, img, pfn_rule, user_data);
	default:
		return err_bad_format;
	}
}

VENUS_END
