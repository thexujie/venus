#pragma once

VENUS_BEG

struct jpeg_image_info
{
	jpeg_image_info()
	{
		dwWidth = 0;
		dwHeight = 0;
		dwColorNum = 0;
		dwByteNum = 0;
	}
	unsigned int dwWidth;
	unsigned int dwHeight;
	unsigned int dwColorNum;
	unsigned int dwByteNum;
};

const img_u8 JPEG_HEADER[2] = {0xFF, 0xD8};
const img_u8 JPEG_TAIL[2] = {0xFF, 0xD9};

IMAGE_API bool is_jpg_data(const img_u8 * buffer, img_ix length);
IMAGE_API img_ix jpg_create(const img_u8 * buffer, img_ix length, image_data_t * img,
						  image_convert_rule_fun_t pfn_match = nullptr, void * user_data = nullptr);

IMAGE_API bool jpg_rule_default(image_convert_rule_t * rule);

const image_convert_fun_t image_convert_jpeg_use_src = (image_convert_fun_t)-1;

VENUS_END
