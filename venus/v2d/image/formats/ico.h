#pragma once

VENUS_BEG

#pragma pack(push, 1)
struct icon_plane_header_t
{
	img_u8        width;          // width, in pixels, of the image
	img_u8        height;         // height, in pixels, of the image
	img_u8        color_count;     // number of colors in image (0 if >=8bpp)
	img_u8        reserved;       // reserved ( must be 0)
	img_u16       plane_count;         // color planes
	img_u16       bi_tcount;       // bits per pixel
	img_u32       data_size;    // how many bytes in this resource?
	img_u32       data_offset;   // where in the file is this image?
};

struct icon_header_t
{
	img_u16           reserved;   // reserved (must be 0)
	img_u16           type;       // resource type (1 for icons)
	img_u16           plane_count;      // how many images?
};
#pragma pack(pop)

//typdef struct
//{
//	bmp_info_windows_t   icheader;      // dib header
//	rgbquad         iccolors[1];   // color table
//	byte            icxor[1];      // dib bits for xor mask
//	byte            icand[1];      // dib bits for and mask
//} iconimage, *lpiconimage;

IMAGE_API img_ix ico_create(img_u8 * buffer, img_ix length, image_data_t * img, img_ix iIndex);

VENUS_END
