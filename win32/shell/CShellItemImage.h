#pragma once

#include <shobjidl.h>

VENUS_BEG

class WIN32_API IWin32ShellItemImage : public IImage
{
public:
	virtual bool Prepare(sizeix size, bool bThumbnails) = 0;
	//! 创建新的 IImage，由接收者来 Release。
	virtual IImage * GetSpecificImage(sizeix size, bool bThumbnails) = 0;
};

class CShellItemImage : public ObjectT<IWin32ShellItemImage>
{
public:
	CShellItemImage(IShellItemImageFactory * pShliif);
	~CShellItemImage();

	ImageBufferT GetBuffer() const;
	sizeix GetSize() const;
	cmode_e GetCMode() const;

	void SetCache(IImageCache * pCache);
	IImageCache * GetCache() const;

public:
	bool Prepare(sizeix size);
	bool Prepare(sizeix size, bool bThumbnails);
	IImage * GetSpecificImage(sizeix size, bool bThumbnails);

protected:
	IShellItemImageFactory * m_pShliif;
	int_x m_iPrepare;
	struct CacheT
	{
		sizeix size;
		bool bThumbnails;
		image_data_t data;
		IImageCache * pCache;
	};
	vector<CacheT> m_caches;
};

VENUS_END
