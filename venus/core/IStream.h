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

enum StreamSeekE
{
	StreamSeekBegin = 0,
	StreamSeekEnd,
	StreamSeekCurr,
	StreamSeekTell,
};

enum StreamEndianE
{
	StreamEndianSmall = 0,
	StreamEndianBig,
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

enum TextEncodingE
{
	TextEncodingInvalid,
	TextEncodingAnsi,
	TextEncodingUnicode16,
	TextEncodingUnicode16BigEndian,
	TextEncodingUnicode32,
	TextEncodingUnicode32BigEndian,
	TextEncodingUTF8,
};

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
	virtual bool SeekSurpport(StreamSeekE seek) const = 0;
	virtual int_x Seek(StreamSeekE seek, int_x iSeek) = 0;
};

/************************************************************************/
/* 输入流
/************************************************************************/
class CORE_API IInputStream : public IBaseStream
{
public :
	virtual ~IInputStream() {}
	virtual bool CanRead() const = 0;
	virtual int_x ReadAviliable() = 0;

	virtual int_x Read(void * pBytes, int_x iLength) = 0;
	virtual byte_t ReadByte() = 0;

	virtual bool SeekSurpport(StreamSeekE seek) const = 0;
	virtual int_x Seek(StreamSeekE seek, int_x iSeek) = 0;
};

/************************************************************************/
/* 输出流
/************************************************************************/
class CORE_API IOutputStream : public IBaseStream
{
public :
	virtual ~IOutputStream() {}
	virtual bool CanWrite() const = 0;
	virtual int_x WriteAviliable() = 0;
	virtual void WriteByte(byte_t byte) = 0;
	virtual void Write(const void * pData, int_x iLength) = 0;

	virtual void Flush() = 0;

	virtual bool SeekSurpport(StreamSeekE seek) const = 0;
	virtual int_x Seek(StreamSeekE seek, int_x iSeek) = 0;
};

class CORE_API IIOStream : public IInputStream, public IOutputStream
{
public:
	virtual ~IIOStream() {}

	virtual bool CanRead() const = 0;
	virtual bool CanWrite() const = 0;

	virtual bool SeekSurpport(StreamSeekE seek) const = 0;
	virtual int_x Seek(StreamSeekE seek, int_x iSeek) = 0;

	virtual int_x ReadAviliable() = 0;
	virtual int_x Read(void * pBytes, int_x iLength) = 0;
	virtual byte_t ReadByte() = 0;

	virtual int_x WriteAviliable() = 0;
	virtual void WriteByte(byte_t byte) = 0;
	virtual void Write(const void * pData, int_x iLength) = 0;

	virtual void Flush() = 0;
};

/************************************************************************/
/* 数据输入流
/************************************************************************/
class CORE_API IDataInputStream : public IInputStream
{
public:
	virtual ~IDataInputStream() {}
	virtual bool CanRead() const = 0;
	virtual int_x ReadAviliable() = 0;

	virtual int_x Read(void * pBytes, int_x iLength) = 0;
	virtual byte_t ReadByte() = 0;

	virtual bool SeekSurpport(StreamSeekE seek) const = 0;
	virtual int_x Seek(StreamSeekE seek, int_x iSeek) = 0;

public :
	virtual void SetDataInputEndian(StreamEndianE endian) = 0;
	virtual StreamEndianE GetDataInputEndian() const = 0;

	virtual int_8 ReadInt8() = 0;
	virtual void ReadInt8Array(char_8 * pi8Array, int_x iLength) = 0;
	virtual uint_8 ReadUInt8() = 0;
	virtual void ReadUInt8Array(uint_8 * pui8Array, int_x iLength) = 0;

	virtual int_16 ReadInt16() = 0;
	virtual void ReadInt16Array(int_16 * pi16Array, int_x iLength) = 0;
	virtual uint_16 ReadUInt16() = 0;
	virtual void ReadUInt16Array(uint_16 * pui16Array, int_x iLength) = 0;

	virtual int_32 ReadInt32() = 0;
	virtual void ReadInt32Array(int_32 * pi32Array, int_x iLength) = 0;
	virtual uint_32 ReadUInt32() = 0;
	virtual void ReadUInt32Array(uint_32 * pui32Array, int_x iLength) = 0;

	// ----------------------------------- String
	virtual int_x ReadText(char_16 * pText, int_x iLength = IX_MAX) = 0;
	virtual int_x ReadText(char_8 * pText, int_x iLength = IX_MAX) = 0;
	virtual int_x ReadLine(char_16 * pText, int_x iSize = IX_MAX) = 0;
	virtual int_x ReadLine(char_8 * pText, int_x iSize = IX_MAX) = 0;
	virtual int_x CSharpReadText(char_16 * pText, int_x iLength = IX_MAX) = 0;
	virtual int_x AS3ReadText(char_16 * pText, int_x iLength = IX_MAX) = 0;

	virtual IInputStream * GetInputStream() = 0;
	virtual IInputStream * ResetInputStream(IInputStream * pNewInputStream) = 0;
};

/************************************************************************/
/* 数据输出流
/************************************************************************/
class CORE_API IDataOutputStream : public IOutputStream
{
public :
	virtual ~IDataOutputStream() {}
	virtual bool CanWrite() const = 0;

	virtual int_x WriteAviliable() = 0;
	virtual void WriteByte(byte_t byte) = 0;
	virtual void Write(const void * pData, int_x iLength) = 0;
	virtual void Flush() = 0;

	virtual bool SeekSurpport(StreamSeekE seek) const = 0;
	virtual int_x Seek(StreamSeekE seek, int_x iSeek) = 0;
public :
	virtual void WriteInt8(int_8 i8) = 0;
	virtual void WriteInt8Array(const int_8 * pi8Array, int_x iLength) = 0;
	virtual void WriteUInt8(uint_8 ui8) = 0;
	virtual void WriteUInt8Array(const uint_8 * pui8Array, int_x iLength) = 0;

	virtual void WriteInt16(int_16 i16) = 0;
	virtual void WriteInt16Array(const int_16 * pi16Array, int_x iLength) = 0;
	virtual void WriteUInt16(uint_16 ui16) = 0;
	virtual void WriteUInt16Array(const uint_16 * pui16Array, int_x iLength) = 0;

	virtual void WriteInt32(int_32 i32) = 0;
	virtual void WriteInt32Array(const int_32 * pi32Array, int_x iLength) = 0;
	virtual void WriteUInt32(uint_32 ui32) = 0;
	virtual void WriteUInt32Array(const uint_32 * pui32Array, int_x iLength) = 0;

	virtual void WriteFloat32(float_32 f32) = 0;
	virtual void WriteFloatArray(const float_32 * pf32, int_x iLength) = 0;

	virtual void WriteText(const char_16 * pText, int_x iLength = 1) = 0;
	virtual void WriteText(const char_8 * pText, int_x iLength = 1) = 0;
	virtual void WriteString(const char_16 * pText, int_x iLength = -1) = 0;
	virtual void WriteString(const char_8 * pText, int_x iLength = -1) = 0;
	virtual void CSharpWriteText(const char_16 * pText, int_x iLength = 1) = 0;
	virtual void AS3WriteText(const char_16 * pText, int_x iLength = 1) = 0;

	virtual IOutputStream * GetOutputStream() = 0;
	virtual IOutputStream * SetOutputStream(IOutputStream * pNewOutputStream) = 0;
};

/************************************************************************/
/* 输入缓冲流
/************************************************************************/
class CORE_API IBufferedInputStream : public IInputStream
{
public:
	virtual ~IBufferedInputStream() {}
	virtual bool CanRead() const = 0;
	virtual int_x ReadAviliable() = 0;
	virtual byte_t ReadByte() = 0;
	virtual int_x Read(void * pBytes, int_x iLength) = 0;

	virtual bool SeekSurpport(StreamSeekE seek) const = 0;
	virtual int_x Seek(StreamSeekE seek, int_x iSeek) = 0;

	virtual void SetBufferSize(int_x iSize) = 0;
	virtual int_x GetBufferSize() const = 0;

	virtual IInputStream * SetInputStream(IInputStream * pNewInputStream) = 0;
	virtual IInputStream * GetInputStream() const = 0;
};

/************************************************************************/
/* 输出缓冲流
/************************************************************************/
class CORE_API IBufferedOutputStream : public IOutputStream
{
public :
	virtual ~IBufferedOutputStream() {}
	virtual bool CanWrite() const = 0;
	virtual int_x WriteAviliable() = 0;

	virtual void WriteByte(byte_t bVal) = 0;
	virtual void Write(const void * pBytes, int_x iLength) = 0;

	virtual void Flush() = 0;

	virtual bool SeekSurpport(StreamSeekE seek) const = 0;
	virtual int_x Seek(StreamSeekE seek, int_x iSeek) = 0;

	virtual void SetBufferSize(int_x iSize) = 0;

	virtual IOutputStream * SetOutputStream(IOutputStream * pNewOutputStream) = 0;
	virtual IOutputStream * GetOutputStream() const = 0;
};

class CORE_API ITextInputStream : public IInputStream
{
public:
	virtual ~ITextInputStream() {}

	virtual bool CanRead() const = 0;
	virtual int_x ReadAviliable() = 0;
	virtual byte_t ReadByte() = 0;
	virtual int_x Read(void * pBytes, int_x iLength) = 0;

	virtual bool SeekSurpport(StreamSeekE seek) const = 0;
	virtual int_x Seek(StreamSeekE seek, int_x iSeek) = 0;

	virtual IInputStream * GetInputStream() = 0;
	virtual IInputStream * ResetInputStream(IInputStream * pNewInputStream) = 0;

	virtual void SetTextInputEncoding(TextEncodingE eEncoding) = 0;
	virtual TextEncodingE GetTextInputEncoding() const = 0;

	virtual void SetCodePage(int_x eCodePage) = 0;
	virtual int_x GetCodepage() const = 0;

	virtual int_32 ReadChar() = 0;
	virtual int_x ReadLine(char_16 * szBuffer, int_x iSize) = 0;
	virtual textw ReadLineW() = 0;
	virtual TextEncodingE ReadEncoding() = 0;
};

class CORE_API ITextOutputStream : public IOutputStream
{
public:
	virtual ~ITextOutputStream() {}

	virtual void WriteText(const char_16 * pText, int_x iLength = -1, LineTagE eLineTag = LineTagRN) = 0;
	virtual void WriteText(const textw & text, LineTagE eLineTag = LineTagRN) = 0;
	virtual void WriteFormat(const char_16 * szFormat, ...) = 0;
	virtual void WriteLineTag(LineTagE eLineTag = LineTagRN) = 0;
};

class CORE_API IMemoryBuffer
{
public:

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
