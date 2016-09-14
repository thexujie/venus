#include "stdafx.h"
#include "CImage.h"

VENUS_BEG

CImage::CImage() :
	m_pCache(nullptr)
{
	buffclr(m_data);
}

CImage::~CImage()
{
	if(m_pCache)
	{
		m_pCache->ReleaseCache();
		SafeRelease(m_pCache);
	}
	image_free(m_data.buffer);
	buffclr(m_data);
}

void CImage::Load(const byte_t * pBuffer, int_x iLength, image_convert_rule_fun_t pfn_convert)
{
	if(!pfn_convert)
		pfn_convert = Get2DDevice()->GetImageConvertRuleFun();
	if(!pfn_convert)
		return;

	image_format_e image_format = image_format_invalid;

	if(is_bmp_data(pBuffer, iLength))
		image_format = image_format_bmp;
	else if(is_dds_data(pBuffer, iLength))
		image_format = image_format_dds;
	else if(is_tga_data(pBuffer, iLength))
		image_format = image_format_tga;
	else if(is_png_data(pBuffer, iLength))
		image_format = image_format_png;
	else if(is_jpg_data(pBuffer, iLength))
		image_format = image_format_jpg;
	else
		image_format = image_format_invalid;

	image_create(pBuffer, iLength, &m_data, image_format, pfn_convert, nullptr);
}

void CImage::Load(const char_16 * szFile, image_convert_rule_fun_t pfn_convert)
{
	if(!pfn_convert)
		pfn_convert = Get2DDevice()->GetImageConvertRuleFun();
	if(!pfn_convert)
		return;

	CFileStream file(szFile, StreamModeRead);
	byte_buffer_t buffer = file.ReadFile();
	if(buffer.buffer)
		Load(buffer.buffer, buffer.size, pfn_convert);
}

bool CImage::Prepare(sizeix size)
{
	return true;
}

void CImage::SetBuffer(const ImageBufferT & buffer)
{
	if(m_pCache)
		m_pCache->ReleaseCache();

	image_free(m_data.buffer);
	buffclr(m_data);
	m_data.buffer = buffer.buffer;
	m_data.length = buffer.length;
	m_data.width = buffer.width;
	m_data.height = buffer.height;
	m_data.pitch = buffer.pitch;
	m_data.dst_mode = buffer.mode;

	m_data.src_mode = cmode_invalid;
	m_data.flags = 0;
}

ImageBufferT CImage::GetBuffer() const
{
	ImageBufferT buffer;
	buffer.buffer = m_data.buffer;
	buffer.length = m_data.length;
	buffer.width = m_data.width;
	buffer.height = m_data.height;
	buffer.pitch = m_data.pitch;
	buffer.bits = m_data.bits;
	buffer.mode = m_data.dst_mode;
	return buffer;
}

sizeix CImage::GetSize() const
{
	return sizeix(m_data.width, m_data.height);
}

cmode_e CImage::GetCMode() const
{
	return m_data.dst_mode;
}

void CImage::SetCache(IImageCache * pCache)
{
	SafeRelease(m_pCache);
	m_pCache = pCache;
	SafeAddRef(m_pCache);
}

IImageCache * CImage::GetCache() const
{
	return m_pCache;
}

VENUS_END