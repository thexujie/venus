#include "stdafx.h"
#include "C3DFile.h"

VENUS_BEG

C3DFile::C3DFile():
	m_size(0), m_curr(0), m_buffer(nullptr)
{
}

C3DFile::~C3DFile()
{
	SafeDelete(m_buffer);
}

err_t C3DFile::Load(const char_16 * szFile)
{
	if(!szFile || !szFile[0])
		return err_invalidarg;

	SafeDelete(m_buffer);
	m_size = 0;

	CFileStream fs(szFile, StreamModeRead);
	if(!fs.IsValid())
		return 1;
#ifdef _DEBUG
	m_path = szFile;
#endif
	m_size = fs.GetFileSize();
	m_buffer = new byte_t[m_size];
	fs.Read(m_buffer, m_size);
	fs.Close();
	return 0;
}

int_x C3DFile::GetSize() const
{
	return m_size;
}

const byte_t * C3DFile::GetBuffer() const
{
	return m_buffer;
}

bool C3DFile::CanRead() const
{
	return true;
}

int_x C3DFile::ReadAviliable()
{
	return m_size - m_curr;
}

int_x C3DFile::Read(void * pBytes, int_x iLength)
{
	if(iLength > m_size - m_curr)
		return 0;
	buffcpy8(pBytes, iLength, m_buffer + m_curr, iLength);
	m_curr += iLength;
	return iLength;
}

byte_t C3DFile::ReadByte()
{
	if(m_size - m_curr > 0)
	{
		byte_t b = m_buffer[m_curr];
		++m_curr;
		return b;
	}
	else
		throw exp_end_of_stream();
}

bool C3DFile::SeekSurpport(StreamSeekE seek) const
{
	return false;
}

int_x C3DFile::Seek(StreamSeekE seek, int_x iSeek)
{
	return m_curr;
}

VENUS_END
