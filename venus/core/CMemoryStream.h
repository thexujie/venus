#pragma once

#include "IStream.h"

VENUS_BEG

/**
 * �����������ֽ��������Ҫʹ�ö������ֽ�������ʹ�� CByteStream��
 * �����ﵽĩ��ʱ�������д�룬��������Ҫ��չ���Ĵ�С������Ƕ�ȡ�����׳� CEndOfStreamException �쳣��
 */
class CORE_API CMemoryStream : public IInputStream, public IOutputStream
{
public:
	CMemoryStream(int_x iSize = 0);
	~CMemoryStream();

	bool CanRead() const noexcept;
	bool CanWrite() const noexcept;

	int_x ReadAviliable() const noexcept;
	byte_t Read();
	int_x Read(void * pArray, int_x iLength);

	int_x WriteAviliable() const noexcept;
	void Write(byte_t byte);
	void Write(const void * pArray, int_x iLength);
	void Flush();
	
	bool CanSeek(SeekE seek) const;
	int_x Seek(SeekE seek, int_x iSeek);

	byte_t * GetBuffer();
	const byte_t * GetBuffer() const;
	
	//! ֻ������
	int_x SetSizeIncrease(int_x iSize);
private:
	byte_t * m_pBuffer;
	int_x m_iSize;
	int_x m_iLength;
};

VENUS_END
