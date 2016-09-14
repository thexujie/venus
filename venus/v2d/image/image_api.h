#pragma once

VENUS_BEG

inline img_u8 * image_malloc(img_ix size)
{
	return (img_u8 *)malloc(size);
}

inline void image_free(void * dst)
{
	if(dst)
		free(dst);
}

inline void image_clear(void * dst, img_ix length)
{
	memset(dst, 0, length);
}

inline void image_memcpy(void * dst, img_ix dst_size, const void * src, img_ix length)
{
	memcpy_s(dst, dst_size, src, length);
}

VENUS_END
