#pragma once

VENUS_BEG

enum FileBomE
{
	FileBomInvalid = 0,
	FileBomAnsi,
	FileBomUtf8,
	FileBomUtf16SmallEdian,
	FileBomUtf16BigEdian,
};

enum SeekE
{
	SeekBegin = 0,
	SeekEnd,
	SeekCurr,
	SeekTell,
};

enum EndianE
{
	EndianSmall = 0,
	EndianBig,
};

// ------------------------- 文件的打开模式。
enum StreamModeE
{
	StreamModeNone = 0,

	//! 可读。
	StreamModeRead = 0x1,
	//! 可写。
	StreamModeWrite = 0x2,
	//! 添加模式。
	StreamModeAppend = 0x4, 
};

declare_flags(StreamModeE)

// -------------------------附加的打开/创建标记。
enum OpenModeE
{
	//! 仅当文件存在时打开文件。
	OpenModeOpen,
	//! 总是打开文件，如果文件不存在，就创建。
	OpenModeAlways,
	//! 仅当文件不存在时创建文件。
	OpenModeCreateNew,
	//! 总是创建文件，如果存在文件，则先删除。
	OpenModeCreateAlways,
	//! 总是截断文件为空文件。
	OpenModeTrunkExisting,
};

// ---------------------------文件的共享模式。
enum ShareModeE
{
	//! 共享可读写。
	ShareModeAll = 0,
	//! 共享可读取。
	ShareModeReadOnly = 0x1,
	//! 共享可写入。
	ShareModeWriteOnly = 0x2,
	//! 不共享。
	ShareModeNone = 0x4,
};
declare_flags(ShareModeE)

enum LineTagE
{
	LineTagNone,
	LineTag0,
	LineTagR,
	LineTagN,
	LineTagRN,
};

class CORE_API IBaseStream
{
public:
	virtual ~IBaseStream() {}
	virtual bool CanSeek() const = 0;
	virtual int_x Seek(SeekE seek, int_x iSeek) = 0;
};

class CORE_API ITextStream
{
public:
	virtual ~ITextStream() {}
	virtual void SetEncoding(encoding_t encoding) = 0;
	virtual encoding_t GetEncoding() const = 0;
};

class CORE_API IDataStream
{
public:
	virtual ~IDataStream() {}
	virtual void SetEndian(EndianE endian) = 0;
	virtual EndianE GetEndian() const = 0;
};

class CORE_API IBufferedStream
{
public:
	virtual ~IBufferedStream() {}

	virtual void SetBufferSize(int_x iSize) = 0;
	virtual int_x GetBufferSize() const = 0;
};

/************************************************************************/
/* 输入流
/************************************************************************/
class CORE_API IInputStream : public IBaseStream
{
public :
	virtual ~IInputStream() {}
	virtual bool CanRead() const noexcept = 0;
	virtual int_x ReadAviliable() const noexcept = 0;

	virtual byte_t Read() = 0;
	virtual int_x Read(void * data, int_x size) = 0;
};

/************************************************************************/
/* 输出流
/************************************************************************/
class CORE_API IOutputStream : public IBaseStream
{
public :
	virtual ~IOutputStream() {}

	virtual bool CanWrite() const noexcept = 0;
	virtual int_x WriteAviliable() const noexcept = 0;
	virtual void Write(byte_t byte) = 0;
	virtual void Write(const void * pData, int_x iLength) = 0;
	virtual void Flush() = 0;
};

/************************************************************************/
/* 输入缓冲流
/************************************************************************/
class CORE_API IBufferedInputStream : public IInputStream, virtual public IBufferedStream
{
public:
	virtual ~IBufferedInputStream() {}
};

/************************************************************************/
/* 输出缓冲流
/************************************************************************/
class CORE_API IBufferedOutputStream : public IOutputStream, virtual public IBufferedStream
{
public :
	virtual ~IBufferedOutputStream() {}
};

union DataAdapter32
{
	union
	{
		byte_t bVal;
		int_8 i8Val;
		uint_8 ui8Val;
		int_16 i16Val;
		uint_16 ui16Val;
		int_32 i32Val;
		uint_32 ui32Val;

		float_32 f32Val;

		char_8 c8Val;
		char_16 c16Val;
	};

	struct
	{
		unsigned char a;
		unsigned char b;
		unsigned char c;
		unsigned char d;
	};
	unsigned char arr[4];
};

VENUS_END
