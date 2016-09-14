#pragma once

VENUS_BEG

class V2D_API IImageCache : public IObject
{
public:
	virtual void ReleaseCache() = 0;
};

class V2D_API ImageBufferT
{
public:
	ImageBufferT() { buffclr(*this); }
	ImageBufferT(const image_data_t & data) :
		buffer(data.buffer), length(data.length), width(data.width), height(data.height),
		pitch(data.pitch), bits(data.bits), mode(data.dst_mode)
	{
	}
	byte_t * buffer;
	int_x length;

	int_x width;
	int_x height;
	int_x pitch;
	int_x bits;
	cmode_e mode;
};

class V2D_API IImage : public IObject
{
public:
	virtual ~IImage() {}

	virtual bool Prepare(sizeix size) = 0;

	virtual ImageBufferT GetBuffer() const = 0;
	virtual sizeix GetSize() const = 0;
	virtual cmode_e GetCMode() const = 0;

	virtual void SetCache(IImageCache * pCache) = 0;
	virtual IImageCache * GetCache() const = 0;
};

/// <summary>
/// CImage 本身只是一个图片的位图化信息，只包含位图 buffer，IImageCache 才是真正被显示的图片，
/// IImageCache 由 IPaint 在 DrawImage 时生成。
/// CImage 在销毁时会调用 IImageCache 的 Release 以销毁缓存。
/// </summary>
class V2D_API CImage : public ObjectT<IImage>
{
public:
	CImage();
	~CImage();

	void Load(const byte_t * pBuffer, int_x iLength, image_convert_rule_fun_t pfn_convert = nullptr);
	void Load(const char_16 * szFile, image_convert_rule_fun_t pfn_convert = nullptr);

	bool Prepare(sizeix size);
	void SetBuffer(const ImageBufferT & buffer);
	ImageBufferT GetBuffer() const;
	sizeix GetSize() const;
	cmode_e GetCMode() const;

	void SetCache(IImageCache * pCache);
	IImageCache * GetCache() const;

protected:
	image_data_t m_data;
	IImageCache * m_pCache;
};

VENUS_END