#include "stdafx.h"
#include "CMemoryStream.h"

VENUS_BEG

CMemoryStream::CMemoryStream(int_x iSize) : m_pBuffer(nullptr), m_iLength(0), m_iSize(0)
{
	if(iSize > 0)
	{
		m_pBuffer = new byte_t[iSize];
		m_iSize = iSize;
	}
}

CMemoryStream::~CMemoryStream()
{
	SafeDelete(m_pBuffer);
}

bool CMemoryStream::CanRead() const { return true;}
bool CMemoryStream::CanWrite() const { return true;}
// ------------------------------ ∂¡»°
int_x CMemoryStream::ReadAviliable()
{
	return m_iSize - m_iLength;
}

byte_t CMemoryStream::ReadByte()
{
	if(m_iLength < m_iSize)
		return m_pBuffer[m_iLength++];
	else
		throw exp_end_of_stream();
}

int_x CMemoryStream::Read(void * pArray, int_x iLength)
{
	int_x iAviliable = m_iSize - m_iLength;
	if(iLength <= iAviliable)
	{
		buffcpy8(pArray, iLength, m_pBuffer + m_iLength, iLength);
		m_iLength += iLength;
		return iLength;
	}
	else
		return 0;
}

// ------------------------------ –¥»Î
int_x CMemoryStream::WriteAviliable()
{
	return m_iSize - m_iLength;
}

void CMemoryStream::WriteByte(byte_t byte)
{
	if(m_iLength >= m_iSize)
		SetSizeIncrease(m_iLength + 1);
	m_pBuffer[m_iLength++] = byte;
}

void CMemoryStream::Write(const void * pArray, int_x iLength)
{
	if(m_iLength + iLength > m_iSize)
		SetSizeIncrease(m_iLength + iLength);

	buffcpy8(m_pBuffer + m_iLength, iLength, pArray, iLength);
	m_iLength += iLength;
}

void CMemoryStream::Flush()
{

}

bool CMemoryStream::SeekSurpport(StreamSeekE seek) const
{
	return true;
}

int_x CMemoryStream::Seek(StreamSeekE seek, int_x iSeek)
{
	if(seek != StreamSeekTell)
	{
		int_x iPosition = 0;
		switch(seek)
		{
		case StreamSeekBegin:
			iPosition = iSeek;
			break;
		case StreamSeekEnd:
			iPosition = m_iSize - iSeek;
			break;
		case StreamSeekCurr:
			iPosition = m_iLength + iSeek;
			break;
		default:
			iPosition = m_iLength;
			break;
		}

		if(iPosition < 0 || iPosition > m_iSize)
			throw exp_end_of_stream();
		m_iLength = iPosition;
	}

	return m_iLength;
}

byte_t * CMemoryStream::GetBuffer()
{
	return m_pBuffer;
}
const byte_t * CMemoryStream::GetBuffer() const
{
	return m_pBuffer;
}

int_x CMemoryStream::SetSizeIncrease(int_x iSize)
{
	int_x iTemp = m_iSize;
	if(iSize > m_iSize)
	{
		if(!iTemp)
			iTemp = 1;

		while(iTemp < iSize)
			iTemp <<= 1;

		byte_t * pBuffer = new byte_t[iTemp];
		buffcpy8(pBuffer, iTemp, m_pBuffer, m_iLength);
		SafeDelete(m_pBuffer);
		m_pBuffer = pBuffer;
		m_iSize = iTemp;
	}
	return m_iSize;
}

VENUS_END
