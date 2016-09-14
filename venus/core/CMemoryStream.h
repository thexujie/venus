#pragma once

#include "IStream.h"

VENUS_BEG

/**
 * 不定定长的字节流，如果要使用定长的字节流，请使用 CByteStream。
 * 当流达到末端时，如果是写入，则会根据需要扩展流的大小，如果是读取，则抛出 CEndOfStreamException 异常。
 */
class CORE_API CMemoryStream : public IIOStream
{
public:
	CMemoryStream(int_x iSize = 0);
	~CMemoryStream();

	bool CanRead() const;
	bool CanWrite() const;

	int_x ReadAviliable();
	byte_t ReadByte();
	int_x Read(void * pArray, int_x iLength);

	int_x WriteAviliable();
	void WriteByte(byte_t byte);
	void Write(const void * pArray, int_x iLength);
	void Flush();
	
	bool SeekSurpport(StreamSeekE seek) const;
	int_x Seek(StreamSeekE seek, int_x iSeek);

	byte_t * GetBuffer();
	const byte_t * GetBuffer() const;
	
	//! 只能增大。
	int_x SetSizeIncrease(int_x iSize);
private:
	byte_t * m_pBuffer;
	int_x m_iSize;
	int_x m_iLength;
};

VENUS_END
