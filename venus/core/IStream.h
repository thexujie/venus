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

// ------------------------- �ļ��Ĵ�ģʽ��
enum StreamModeE
{
	StreamModeNone = 0,

	//! �ɶ���
	StreamModeRead = 0x1,
	//! ��д��
	StreamModeWrite = 0x2,
	//! ���ģʽ��
	StreamModeAppend = 0x4, 
};

declare_flags(StreamModeE)

// -------------------------���ӵĴ�/������ǡ�
enum OpenModeE
{
	//! �����ļ�����ʱ���ļ���
	OpenModeOpen,
	//! ���Ǵ��ļ�������ļ������ڣ��ʹ�����
	OpenModeAlways,
	//! �����ļ�������ʱ�����ļ���
	OpenModeCreateNew,
	//! ���Ǵ����ļ�����������ļ�������ɾ����
	OpenModeCreateAlways,
	//! ���ǽض��ļ�Ϊ���ļ���
	OpenModeTrunkExisting,
};

// ---------------------------�ļ��Ĺ���ģʽ��
enum ShareModeE
{
	//! ����ɶ�д��
	ShareModeAll = 0,
	//! ����ɶ�ȡ��
	ShareModeReadOnly = 0x1,
	//! �����д�롣
	ShareModeWriteOnly = 0x2,
	//! ������
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
/* ������
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
/* �����
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
/* ���뻺����
/************************************************************************/
class CORE_API IBufferedInputStream : public IInputStream, virtual public IBufferedStream
{
public:
	virtual ~IBufferedInputStream() {}
};

/************************************************************************/
/* ���������
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
