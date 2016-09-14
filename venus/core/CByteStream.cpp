#include "stdafx.h"
#include "CByteStream.h"

VENUS_BEG

CByteStream::CByteStream()
{
	m_pBuffer = 0;
	m_iSize = 0;
	m_iLength = 0;
	m_bAutoDelete = false;
}
CByteStream::CByteStream(int_x iLength)
{
	m_pBuffer = new byte_t[iLength];
	if(m_pBuffer)
	{
		m_iSize = iLength;
		m_bAutoDelete = true;
	}
	else
	{
		m_iSize = 0;
		m_bAutoDelete = false;
	}
	m_iLength = 0;
}

CByteStream::CByteStream(byte_t * pArray, int_x iLength, bool bAutoDelete/* = false*/)
{
	m_pBuffer = pArray;
	m_iSize = iLength;
	m_iLength = 0;
	m_bAutoDelete = bAutoDelete;
}

CByteStream::~CByteStream()
{
	if(m_bAutoDelete)
		delete m_pBuffer;
}

void CByteStream::ResetByteArray(byte_t * pArray, int_x iLength, bool bAutoDelete/* = FALSE*/)
{
	if(bAutoDelete)
		delete m_pBuffer;

	m_pBuffer = pArray;
	m_iSize = iLength;
	m_iLength = 0;
	m_bAutoDelete = bAutoDelete;
}

bool CByteStream::CanRead() const { return true;}
bool CByteStream::CanWrite() const { return true;}

// ------------------------------ ∂¡»°
int_x CByteStream::ReadAviliable()
{
	return m_iSize - m_iLength;
}

byte_t CByteStream::ReadByte()
{
	if(m_iLength < m_iSize)
		return m_pBuffer[m_iLength++];
	else
		throw exp_end_of_stream();
}

int_x CByteStream::Read(void * pBytes, int_x iLength)
{
	int_x iAviliable = m_iSize - m_iLength;
	if(iLength <= iAviliable)
	{
		buffcpy8(pBytes, iLength, m_pBuffer + m_iLength, iLength);
		m_iLength += iLength;
		return iLength;
	}
	else
		return 0;
}

// ------------------------------ –¥»Î
int_x CByteStream::WriteAviliable()
{
	return m_iSize - m_iLength;
}

void CByteStream::WriteByte(byte_t byte)
{
	if(m_iLength < m_iSize)
		m_pBuffer[m_iLength++] = byte;
}

void CByteStream::Write(const void * pBytes, int_x iLength)
{
	int_x iAviliable = m_iSize - m_iLength;
	if(iLength > iAviliable)
		iLength = iAviliable;

	buffcpy8(m_pBuffer + m_iLength, iLength, pBytes, iLength);
	m_iLength += iLength;
}

void CByteStream::Flush()
{

}

bool CByteStream::SeekSurpport(StreamSeekE seek) const
{
	return true;
}

int_x CByteStream::Seek(StreamSeekE seek, int_x iSeek)
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

byte_t * CByteStream::GetBuffer()
{
	return m_pBuffer;
}
const byte_t * CByteStream::GetBuffer() const
{
	return m_pBuffer;
}

VENUS_END
