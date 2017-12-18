#pragma once
#include "IStream.h"

VENUS_BEG

class CORE_API CByteInputStreamm : public IInputStream
{
public:
	CByteInputStreamm();
	CByteInputStreamm(const byte_t * pArray, int_x iLength);
	~CByteInputStreamm();

	bool CanRead() const noexcept;

	int_x ReadAviliable() const noexcept;
	byte_t Read();
	int_x Read(void * pBytes, int_x iLength);

	bool CanSeek() const;
	int_x Seek(SeekE seek, int_x iSeek);

	const byte_t * GetBuffer() const;

private:
	const byte_t * m_pBuffer;
	int_x m_iSize;
	int_x m_iLength;
};

/**
 * 定长的字节流，如果要使用不定长的字节流，请使用 CMemoryStream。   
 */
class CORE_API CByteStream : public IInputStream, public IOutputStream
{
public:
	CByteStream();
	CByteStream(int_x iLength);
	CByteStream(byte_t * pArray, int_x iLength);
	~CByteStream();

	bool CanRead() const noexcept;
	int_x ReadAviliable() const noexcept;
	byte_t Read();
	int_x Read(void * pBytes, int_x iLength);

	bool CanWrite() const noexcept;
	int_x WriteAviliable() const noexcept;
	void Write(byte_t byte);
	void Write(const void * pBytes, int_x iLength);
	void Flush();
	
	bool CanSeek() const;
	int_x Seek(SeekE seek, int_x iSeek);

	byte_t * GetBuffer();
	const byte_t * GetBuffer() const;

private :
	byte_t * m_pBuffer;
	int_x m_iSize;
	int_x m_iLength;
};

VENUS_END
