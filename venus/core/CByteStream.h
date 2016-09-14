#pragma once
#include "IStream.h"

VENUS_BEG
/**
 * �������ֽ��������Ҫʹ�ò��������ֽ�������ʹ�� CMemoryStream��   
 */
class CORE_API CByteStream : public IIOStream
{
public:
	CByteStream();
	CByteStream(int_x iLength);
	CByteStream(byte_t * pArray, int_x iLength, bool bAutoDelete = false);
	~CByteStream();

	//! �滻����������ǰλ�ñ�����Ϊ 0��
	void ResetByteArray(byte_t * pArray, int_x iLength, bool bAutoDelete = false);

	bool CanRead() const;
	bool CanWrite() const;

	int_x ReadAviliable();
	byte_t ReadByte();
	int_x Read(void * pBytes, int_x iLength);

	int_x WriteAviliable();
	void WriteByte(byte_t byte);
	void Write(const void * pBytes, int_x iLength);
	void Flush();
	
	bool SeekSurpport(StreamSeekE seek) const;
	int_x Seek(StreamSeekE seek, int_x iSeek);

	byte_t * GetBuffer();
	const byte_t * GetBuffer() const;

private :
	byte_t * m_pBuffer;
	int_x m_iSize;
	int_x m_iLength;

	bool m_bAutoDelete;
};

VENUS_END
