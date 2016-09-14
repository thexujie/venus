#pragma once

#include "IStream.h"

VENUS_BEG

/************************************************************************/
/* 输入缓冲流
/************************************************************************/

class CORE_API CBufferedInputStream : public IBufferedInputStream
{
public :
	CBufferedInputStream(IInputStream * pInputStream, int_x iSize);
	CBufferedInputStream(IInputStream * pInputStream, byte_t * pBuffer, int_x iSize);
	~CBufferedInputStream();

	bool CanRead() const;
	int_x ReadAviliable();

	byte_t ReadByte();
	int_x Read(void * pBytes, int_x iLength);

	bool SeekSurpport(StreamSeekE seek) const;
	int_x Seek(StreamSeekE seek, int_x iSeek);

	void SetBufferSize(int_x iSize);
	int_x GetBufferSize() const;

	IInputStream * SetInputStream(IInputStream * pNewInputStream);
	IInputStream * GetInputStream() const;

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

class CORE_API CBufferedOutputStream : public IBufferedOutputStream
{
public :
	CBufferedOutputStream(IOutputStream * pOutputStream, int_x iSize);
	CBufferedOutputStream(IOutputStream * pOutputStream, byte_t * pBuffer, int_x iSize);
	~CBufferedOutputStream();

	bool CanWrite() const;
	int_x WriteAviliable();

	void WriteByte(byte_t bVal);
	void Write(const void * pData, int_x iLength);
	void Flush();

	bool SeekSurpport(StreamSeekE seek) const;
	int_x Seek(StreamSeekE seek, int_x iSeek);

	void SetBufferSize(int_x iSize);

	IOutputStream * SetOutputStream(IOutputStream * pNewOutputStream);
	IOutputStream * GetOutputStream() const;

	int_x GetBufferLength() const;
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
