#pragma once

#include "IStream.h"

VENUS_BEG

/************************************************************************/
/* 输入缓冲流
/************************************************************************/

class CORE_API CBufferedInputStream : public IInputStream, virtual public IBufferedStream
{
public :
	CBufferedInputStream(IInputStream * pInputStream, int_x iSize);
	CBufferedInputStream(IInputStream * pInputStream, byte_t * pBuffer, int_x iSize);
	~CBufferedInputStream();

	bool CanRead() const noexcept;
	int_x ReadAviliable() const noexcept;

	byte_t Read();
	int_x Read(void * pBytes, int_x iLength);

	bool CanSeek() const;
	int_x Seek(SeekE seek, int_x iSeek);

	void SetBufferSize(int_x iSize);
	int_x GetBufferSize() const;
	int_x GetBufferLength() const;
	int_x GetBufferPosition() const;
	int_x GetBufferRemain() const;
protected:
	void ConfirmBufferedInputStream() const;
	void FlushBufferToThis();
protected:
	IInputStream * m_pInputStream;
	DataAdapter32 adapter;
	byte_t * m_pBuffer;
	bool m_bNeedDelete;
	int_x m_iSize;
	int_x m_iLength;
	int_x m_iPosition;
};

/************************************************************************/
/* 输出缓冲流
/************************************************************************/

class CORE_API CBufferedOutputStream : public IOutputStream, virtual public IBufferedStream
{
public :
	CBufferedOutputStream(IOutputStream * pOutputStream, int_x iSize);
	CBufferedOutputStream(IOutputStream * pOutputStream, byte_t * pBuffer, int_x iSize);
	~CBufferedOutputStream();

	bool CanWrite() const noexcept;
	int_x WriteAviliable() const noexcept;

	void Write(byte_t bVal);
	void Write(const void * pData, int_x iLength);
	void Flush();

	bool CanSeek() const;
	int_x Seek(SeekE seek, int_x iSeek);

	void SetBufferSize(int_x iSize);
	int_x GetBufferSize() const;
	int_x GetBufferPosition() const;
	int_x GetBufferRemain() const;
protected:
	void ConfirmBufferedOutputStream() const;
	void FlushBufferToThat();
protected:
	IOutputStream * m_pOutputStream;
	DataAdapter32 adapter;
	byte_t * m_pBuffer;
	bool m_bNeedDelete;
	int_x m_iSize;
	int_x m_iPosition;
};

VENUS_END
