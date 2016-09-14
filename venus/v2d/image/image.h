#pragma once

#ifdef _WINDLL
#define IMAGE_API __declspec (dllexport)
#else
#define IMAGE_API
#endif

#ifdef _DEBUG
#include <assert.h>
#define IMAGE_ASSERT(bBool) assert(bBool)
#define IMAGE_BREAK __debugbreak()
#else
#define IMAGE_ASSERT
#define IMAGE_BREAK
#endif

#if defined(_M_X64) || defined(_M_AMD64)
#define IMAGE_BIT_64 1 // 64 位操作系统
#elif defined(_M_IX86)
#define IMAGE_BIT_32 1 // 32 位操作系统
#endif

#include "image_type.h"
#include "image_api.h"
#include "image_converter.h"
#include "image_save.h"

#include "formats/bmp.h"
#include "formats/tga.h"
#include "formats/png.h"
#include "formats/jpg.h"
#include "formats/dds.h"

VENUS_BEG


IMAGE_API img_ix image_load(const char_x * file, image_data_t * img, image_format_e image_format = image_format_invalid,
	image_convert_rule_fun_t pfn_rule = nullptr, void * user_data = nullptr);

IMAGE_API image_format_e image_get_format(const img_u8 * buffer, img_ix length);

IMAGE_API img_ix image_create(const img_u8 * buffer, img_ix length, image_data_t * img, image_format_e image_format,
							  image_convert_rule_fun_t pfn_rule = nullptr, void * user_data = nullptr);

VENUS_END
