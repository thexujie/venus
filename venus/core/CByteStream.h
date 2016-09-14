#pragma once
#include "IStream.h"

VENUS_BEG
/**
 * �������ֽ��������Ҫʹ�ò��������ֽ�������ʹ�� CMemoryStream��   
 */
class CORE_API CByteStream : public IInputStream, public IOutputStream
{
public:
	CByteStream();
	CByteStream(int_x iLength);
	CByteStream(byte_t * pArray, int_x iLength, bool bAutoDelete = false);
	~CByteStream();

	//! �滻����������ǰλ�ñ�����Ϊ 0��
	void ResetByteArray(byte_t * pArray, int_x iLength, bool bAutoDelete = false);

	bool CanRead() const noexcept;
	bool CanWrite() const noexcept;

	int_x ReadAviliable() const noexcept;
	byte_t Read();
	int_x Read(void * pBytes, int_x iLength);

	int_x WriteAviliable() const noexcept;
	void Write(byte_t byte);
	void Write(const void * pBytes, int_x iLength);
	void Flush();
	
	bool CanSeek(SeekE seek) const;
	int_x Seek(SeekE seek, int_x iSeek);

	byte_t * GetBuffer();
	const byte_t * GetBuffer() const;

private :
	byte_t * m_pBuffer;
	int_x m_iSize;
	int_x m_iLength;

	bool m_bAutoDelete;
};

VENUS_END
