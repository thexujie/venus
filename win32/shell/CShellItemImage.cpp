#include "stdafx.h"
#include "CShellItemImage.h"

VENUS_BEG

CShellItemImage::CShellItemImage(IShellItemImageFactory * pShliif) :m_pShliif(pShliif), m_iPrepare(-1)
{
	SafeAddRef(m_pShliif);
}

CShellItemImage::~CShellItemImage()
{
	for(int_x cnt = 0, count = m_caches.size(); cnt < count; ++cnt)
	{
		CacheT & cache = m_caches[cnt];
		SafeRelease(cache.pCache);
		image_free(cache.data.buffer);
	}

	SafeRelease(m_pShliif);
}

ImageBufferT CShellItemImage::GetBuffer() const
{
	ImageBufferT buffer;
	if(m_iPrepare >= 0)
	{
		const image_data_t & data = m_caches[m_iPrepare].data;
		buffer.buffer = data.buffer;
		buffer.length = data.length;
		buffer.width = data.width;
		buffer.height = data.height;
		buffer.pitch = data.pitch;
		buffer.bits = data.bits;
		buffer.mode = data.dst_mode;
	}
	return buffer;
}

sizeix CShellItemImage::GetSize() const
{
	if(m_iPrepare >= 0)
	{
		const image_data_t & data = m_caches[m_iPrepare].data;
		return sizeix(data.width, data.height);
	}
	else
		return sizeix();
}

cmode_e CShellItemImage::GetCMode() const
{
	if(m_iPrepare >= 0)
	{
		const image_data_t & data = m_caches[m_iPrepare].data;
		return data.dst_mode;
	}
	else
		return cmode_invalid;
}

void CShellItemImage::SetCache(IImageCache * pCache)
{
	if(m_iPrepare < 0)
		return;

	CacheT & cache = m_caches[m_iPrepare];
	SafeRelease(cache.pCache);
	cache.pCache = pCache;
	SafeAddRef(cache.pCache);
}

IImageCache * CShellItemImage::GetCache() const
{
	if(m_iPrepare < 0)
		return nullptr;

	const CacheT & cache = m_caches[m_iPrepare];
	return cache.pCache;
}

bool CShellItemImage::Prepare(sizeix size)
{
	return Prepare(size, false);
}

bool CShellItemImage::Prepare(sizeix size, bool bThumbnails)
{
	if(!m_pShliif)
		return false;

	for(int_x cnt = 0, count = m_caches.size(); cnt < count; ++cnt)
	{
		CacheT & cache = m_caches[cnt];
		if(cache.size == size && cache.bThumbnails == bThumbnails)
		{
			m_iPrepare = cnt;
			return true;
		}
	}
	
	CacheT newCache = {size, bThumbnails};
	int_32 iFlags = bThumbnails ? 0 : SIIGBF_ICONONLY;
	SIZE si = {(int_32)size.w, (int_32)size.h};
	HBITMAP hBitmap = NULL;
	HRESULT hResult = S_OK;
	__try
	{
		hResult = m_pShliif->GetImage(si, iFlags, &hBitmap);
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{
		log1(L"CShellItemImage::Prepare -> m_pShliif->GetImage failed!");
		hResult = E_FAIL;
	}
	if(hResult == S_OK)
	{
		bitmap_create(hBitmap, &(newCache.data));
		DeleteObject(hBitmap);
		m_iPrepare = m_caches.size();
		m_caches.add(newCache);
		return true;
	}
	else
		return false;
}

IImage * CShellItemImage::GetSpecificImage(sizeix size, bool bThumbnails)
{
	if(!m_pShliif)
		return nullptr;

	for(int_x cnt = 0, count = m_caches.size(); cnt < count; ++cnt)
	{
		CacheT & cache = m_caches[cnt];
		if(cache.size == size && cache.bThumbnails == bThumbnails)
		{
			const image_data_t & data = m_caches[cnt].data;
			ImageBufferT buffer;
			buffer.buffer = image_malloc(data.length);
			buffer.length = data.length;
			buffer.width = data.width;
			buffer.height = data.height;
			buffer.pitch = data.pitch;
			buffer.bits = data.bits;
			buffer.mode = data.dst_mode;
			buffcpy8(buffer.buffer, buffer.length, data.buffer, data.length);
			CImage * pImage = new CImage();
			pImage->SetBuffer(buffer);
			return pImage;
		}
	}

	int_32 iFlags = bThumbnails ? 0 : SIIGBF_ICONONLY;
	SIZE si = {(int_32)size.w, (int_32)size.h};
	HBITMAP hBitmap = NULL;
	image_data_t data = {};
	m_pShliif->GetImage(si, iFlags, &hBitmap);
	bitmap_create(hBitmap, &data);
	DeleteObject(hBitmap);

	ImageBufferT buffer;
	buffer.buffer = data.buffer;
	buffer.length = data.length;
	buffer.width = data.width;
	buffer.height = data.height;
	buffer.pitch = data.pitch;
	buffer.bits = data.bits;
	buffer.mode = data.dst_mode;
	CImage * pImage = new CImage();
	pImage->SetBuffer(buffer);
	return pImage;
}

VENUS_END
