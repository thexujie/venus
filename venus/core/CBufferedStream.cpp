#include "stdafx.h"
#include "CBufferedStream.h"

VENUS_BEG

CBufferedInputStream::CBufferedInputStream(IInputStream * pInputStream, int_x iSize)
{
	m_pBuffer = new byte_t[iSize];
	m_bNeedDelete = true;
	m_pInputStream = pInputStream;

	m_iSize = iSize;
	m_iLength = 0;
	m_iPosition = 0;
}
CBufferedInputStream::CBufferedInputStream(IInputStream * pInputStream, byte_t * pBuffer, int_x iSize)
{
	m_pBuffer = pBuffer;
	m_bNeedDelete = false;
	m_pInputStream = pInputStream;

	m_iSize = iSize;
	m_iLength = 0;
	m_iPosition = 0;
}

CBufferedInputStream::~CBufferedInputStream()
{
	if(m_pBuffer && m_bNeedDelete)
		delete[] m_pBuffer;
	m_pBuffer = nullptr;
	m_pInputStream = nullptr;
}

bool CBufferedInputStream::CanRead() const noexcept
{
	ConfirmBufferedInputStream();
	if(m_iPosition != m_iLength)
		return true;
	else
		return m_pInputStream->CanRead();
}

int_x CBufferedInputStream::ReadAviliable() const noexcept
{
	ConfirmBufferedInputStream();
	return m_iLength - m_iPosition + m_pInputStream->ReadAviliable();
}

byte_t CBufferedInputStream::Read()
{
	ConfirmBufferedInputStream();
	if(m_iPosition == m_iLength)
	{
		FlushBufferToThis();
	}
	return m_pBuffer[m_iPosition++];
}

int_x CBufferedInputStream::Read(void * pBytes, int_x iLength)
{
	ConfirmBufferedInputStream();
	int_x iRemain = m_iLength - m_iPosition;
	int_x iAviliable = iRemain + m_pInputStream->ReadAviliable();
	if(iAviliable < iLength)
		return 0;

	byte_t * pTemp = (byte_t *)pBytes;
	int_x iSize = 0;
	while(iLength)
	{
		iRemain = m_iLength - m_iPosition;
		if(!iRemain)
		{
			FlushBufferToThis();
			iRemain = m_iLength - m_iPosition;
		}

		if(iLength <= iRemain)
		{
			buffcpy8(pTemp, iLength, m_pBuffer + m_iPosition, iLength);
			m_iPosition += iLength;
			iLength = 0;
		}
		else
		{
			buffcpy8(pTemp, iLength, m_pBuffer + m_iPosition, iRemain);
			m_iPosition += iRemain;
			pTemp += iRemain;
			iLength -= iRemain;
		}
	}
	return iLength;
}

bool CBufferedInputStream::CanSeek() const
{
	ConfirmBufferedInputStream();
	return m_pInputStream->CanSeek();
}

int_x CBufferedInputStream::Seek(SeekE seek, int_x iSeek)
{
	ConfirmBufferedInputStream();
	if(seek == SeekTell)
		return m_pInputStream->Seek(SeekTell, iSeek) - (m_iLength - m_iPosition);
	else if(seek == SeekCurr)
	{
		if(iSeek < 0)
		{
			int_x iPosition = m_iPosition;
			if(m_iPosition + iSeek >= 0)
				m_iPosition += iSeek;
			else
			{
				m_iLength = 0;
				m_iPosition = 0;
				return m_pInputStream->Seek(seek, iSeek + iPosition);
			}
		}
		else if(iSeek > 0)
		{
			int_x iRemain = m_iLength - m_iPosition;
			if(iSeek <= iRemain)
				m_iPosition += iSeek;
			else
			{
				return m_pInputStream->Seek(seek, iSeek - iRemain);
			}
		}
		else {}
		return m_pInputStream->Seek(SeekTell, iSeek) + m_iPosition;
	}
	else
	{
		m_iLength = 0;
		m_iPosition = 0;
		return m_pInputStream->Seek(seek, iSeek);
	}
}

IInputStream * CBufferedInputStream::SetInputStream(IInputStream * pNewInputStream)
{
	IInputStream * pOldInputStream = m_pInputStream;
	m_pInputStream = pNewInputStream;
	return pOldInputStream;
}

IInputStream * CBufferedInputStream::GetInputStream() const
{
	return m_pInputStream;
}

void CBufferedInputStream::SetBufferSize(int_x iSize)
{
	if(m_pBuffer && m_bNeedDelete)
	{
		delete[] m_pBuffer;
		m_pBuffer = 0;
	}
	m_pBuffer = new byte_t[iSize];
	m_iSize = iSize;
	m_iLength = 0;
	m_iPosition = 0;
}

int_x CBufferedInputStream::GetBufferSize() const
{
	ConfirmBufferedInputStream();
	return m_iSize;
}

int_x CBufferedInputStream::GetBufferLength() const
{
	ConfirmBufferedInputStream();
	return m_iLength;
}

int_x CBufferedInputStream::GetBufferPosition() const
{
	ConfirmBufferedInputStream();
	return m_iPosition;
}

int_x CBufferedInputStream::GetBufferRemain() const
{
	ConfirmBufferedInputStream();
	return m_iLength - m_iPosition;
}

void CBufferedInputStream::ConfirmBufferedInputStream() const
{
	if(!m_pInputStream)
		throw exp_nullptr(_T("无效的输入流"));
	else if(m_iPosition > m_iLength)
		throw exp_bad_state(_T("缓冲流已经被破坏"));
	else {}
}

void CBufferedInputStream::FlushBufferToThis()
{
	ConfirmBufferedInputStream();

	int_x iAviliable = m_pInputStream->ReadAviliable();
	if(!iAviliable)
		throw exp_end_of_stream(_T("剩余字节数不足"));

	int_x iLength = iAviliable >= m_iSize ? m_iSize : iAviliable;
	m_pInputStream->Read(m_pBuffer, iLength);
	m_iLength = iLength;
	m_iPosition = 0;
}



// ------------------------------------------------------------------------------------
// CBufferedOutputStream
// ------------------------------------------------------------------------------------
CBufferedOutputStream::CBufferedOutputStream(IOutputStream * pOutputStream, int_x iSize)
{
	m_pOutputStream = pOutputStream;
	m_pBuffer = new byte_t[iSize];
	m_iSize = iSize;
	m_bNeedDelete = true;
	m_iPosition = 0;
}

CBufferedOutputStream::CBufferedOutputStream(IOutputStream * pOutputStream, byte_t * pBuffer, int_x iSize)
{
	m_pOutputStream = pOutputStream;
	m_pBuffer = pBuffer;
	m_iSize = iSize;
	m_bNeedDelete = false;
	m_iPosition = 0;
}
CBufferedOutputStream::~CBufferedOutputStream()
{
	if(m_pBuffer && m_bNeedDelete)
		delete[] m_pBuffer;
	m_pBuffer = nullptr;
	m_pOutputStream = nullptr;
	m_iSize = 0;
	m_iPosition = 0;
}

bool CBufferedOutputStream::CanWrite() const noexcept
{
	ConfirmBufferedOutputStream();
	if(!m_iPosition)
		return m_pOutputStream->CanWrite();
	else
		return m_pOutputStream->WriteAviliable() > m_iPosition;
}

int_x CBufferedOutputStream::WriteAviliable() const noexcept
{
	ConfirmBufferedOutputStream();
	int_x iAviliable = m_pOutputStream->WriteAviliable() - m_iPosition;
	return iAviliable < 0 ? 0 : iAviliable;
}

void CBufferedOutputStream::Write(byte_t bVal)
{
	ConfirmBufferedOutputStream();
	if(m_iPosition == m_iSize)
		FlushBufferToThat();
	m_pBuffer[m_iPosition++] = bVal;
}

void CBufferedOutputStream::Write(const void * pBytes, int_x iLength)
{
	ConfirmBufferedOutputStream();
	const byte_t * pTemp = (const byte_t *)pBytes;
	while(m_iPosition + iLength > m_iSize)
	{
		int_x iTemp = m_iSize - m_iPosition;
		buffcpy8(m_pBuffer + m_iPosition, iTemp, pTemp, iTemp);
		iLength -= iTemp;
		pTemp += iTemp;
		m_iPosition += iTemp;
		FlushBufferToThat();
	}
	
	if(iLength)
	{
		buffcpy8(m_pBuffer + m_iPosition, m_iSize - m_iPosition, pTemp, iLength);
		m_iPosition += iLength;
	}
}
void CBufferedOutputStream::Flush()
{
	ConfirmBufferedOutputStream();
	FlushBufferToThat();
	m_pOutputStream->Flush();
}

bool CBufferedOutputStream::CanSeek() const
{
	ConfirmBufferedOutputStream();
	return m_pOutputStream->CanSeek();
}
int_x CBufferedOutputStream::Seek(SeekE seek, int_x iSeek)
{
	ConfirmBufferedOutputStream();
	if(seek == SeekTell)
		return m_pOutputStream->Seek(SeekTell, iSeek) + m_iPosition;
	else if(seek == SeekCurr)
	{
		if(iSeek < 0)
		{
			int_x iPosition = m_iPosition;
			if(m_iPosition + iSeek >= 0)
				m_iPosition += iSeek;
			else
			{
				m_iPosition = 0;
				return m_pOutputStream->Seek(seek, iSeek + iPosition);
			}
		}
		else if(iSeek > 0)
		{
			int_x iRemain = m_iSize - m_iPosition;
			if(iSeek <= iRemain)
				m_iPosition += iSeek;
			else
			{
				return m_pOutputStream->Seek(seek, iSeek - iRemain);
			}
		}
		else {}
		return m_pOutputStream->Seek(SeekTell, iSeek) + m_iPosition;
	}
	else
	{
		m_iPosition = 0;
		return m_pOutputStream->Seek(seek, iSeek);
	}
}

void CBufferedOutputStream::SetBufferSize(int_x iSize)
{
	ConfirmBufferedOutputStream();
	if(m_pBuffer && m_bNeedDelete)
	{
		delete[] m_pBuffer;
		m_pBuffer = 0;
	}
	m_pBuffer = new byte_t[iSize];
	m_iSize = iSize;
	m_iPosition = 0;
}

IOutputStream * CBufferedOutputStream::SetOutputStream(IOutputStream * pNewOutputStream)
{
	IOutputStream * pOldOutputStream = m_pOutputStream;
	m_pOutputStream = pNewOutputStream;
	return pOldOutputStream;
}
IOutputStream * CBufferedOutputStream::GetOutputStream() const
{
	ConfirmBufferedOutputStream();
	return m_pOutputStream;
}

int_x CBufferedOutputStream::GetBufferPosition() const
{
	ConfirmBufferedOutputStream();
	return m_iPosition;
}

int_x CBufferedOutputStream::GetBufferRemain() const
{
	ConfirmBufferedOutputStream();
	return m_iSize - m_iPosition;
}


void CBufferedOutputStream::ConfirmBufferedOutputStream() const
{
	Verify(m_pOutputStream);
}

void CBufferedOutputStream::FlushBufferToThat()
{
	ConfirmBufferedOutputStream();
	if(m_iPosition)
	{
		m_pOutputStream->Write(m_pBuffer, m_iPosition);
		m_iPosition = 0;
	}
}

//
//CBufferedOutputStream::CBufferedOutputStream(IOutputStream * pOutputStream, int_x uiBufferSize)
//{
//	m_pBuffer = new UINT_8[uiBufferSize];
//	m_pOutputStream = pOutputStream;
//	if(pOutputStream)
//		pOutputStream->AddRef();
//
//	m_iBufferSize = uiBufferSize;
//	m_iBufferAviliable = 0;
//	m_iCurrPos = 0;
//	m_iWriteSum = 0;
//}
//CBufferedOutputStream::~CBufferedOutputStream()
//{
//	if(m_pBuffer)
//		delete [] m_pBuffer;
//	ResetOutputStream(NULL);
//}
//VOID CBufferedOutputStream::ResetOutputStream(IOutputStream * pNewOutputStream)
//{
//	if(m_pOutputStream)
//		m_pOutputStream->Release();
//
//
//	m_pOutputStream = pNewOutputStream;
//	if(pNewOutputStream)
//		pNewOutputStream->AddRef();
//}
//
//UINT_8 CBufferedOutputStream::WriteUInt8(UINT_8 byte)
//{
//	if(m_iCurrPos == m_iBufferAviliable)
//		this->Flush();
//	if(m_iBufferAviliable == 0)
//		return EndOfStream;
//	else
//	{
//		m_pBuffer[m_iCurrPos++] = byte;
//		++m_iWriteSum;
//		return byte;
//	}
//}
//int_x CBufferedOutputStream::WriteUInt8Array(UINT_8 * pBytes, int_x iNum)
//{
//	int_x count = 0;
//	for(int cnt = 0; cnt < iNum; ++cnt)
//	{
//		WriteUInt8(pBytes[cnt]);
//		if(m_iBufferAviliable)
//			++count;
//	}
//	return count;
//}
//// 剩余可写数
//INT_32 CBufferedOutputStream::WriteAviliable() const noexcept
//{
//	return m_pOutputStream->WriteAviliable() const noexcept - (m_iBufferAviliable - m_iCurrPos);
//}
//// 重置输出流
//VOID CBufferedOutputStream::Flush()
//{
//	if(m_pOutputStream->WriteAviliable() const noexcept > 0 && m_iBufferAviliable > 0)
//	{
//		m_iCurrPos = m_iCurrPos - m_pOutputStream->WriteUInt8Array(m_pBuffer, m_iCurrPos);
//		m_pOutputStream->Flush();
//	}
//
//	if(m_iCurrPos != 0)
//		return ;
//	else
//	{
//		m_iBufferAviliable = Min<int_x>(m_pOutputStream->WriteAviliable() const noexcept, m_iBufferSize);
//	}
//}

VENUS_END
