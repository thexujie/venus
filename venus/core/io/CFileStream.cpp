#include "stdafx.h"
#include "CFileStream.h"

#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <sys/stat.h>

VENUS_BEG

static const int_x INVALID_FILE = -1;

static int FileOpenFlagFromStreamMode(StreamModeE streamMode, OpenModeE openMode, ShareModeE shareMode)
{
	int iOpenFlag = _O_BINARY;
	switch(streamMode)
	{
	case StreamModeNone:
		break;
	case StreamModeRead:
		iOpenFlag |= _O_RDONLY;
		break;
	case StreamModeWrite:
		iOpenFlag |= _O_WRONLY;
		break;
	case StreamModeRead | StreamModeWrite:
		iOpenFlag |= _O_RDWR;
		break;
	case StreamModeAppend:
		iOpenFlag |= _O_APPEND;
		break;
	default:
		iOpenFlag |= _O_RDONLY;
		break;
	}

	switch(openMode)
	{
	case OpenModeOpen:
		iOpenFlag |= _O_EXCL;
		break;
	case OpenModeAlways:
		iOpenFlag |= _O_CREAT;
		break;
	case OpenModeCreateNew:
		iOpenFlag |= _O_CREAT | _O_EXCL;
		break;
	case OpenModeCreateAlways:
		iOpenFlag |= _O_CREAT | _O_TRUNC;
		break;
	case OpenModeTrunkExisting:
		iOpenFlag |= _O_TRUNC;
		break;
	default:
		iOpenFlag |= _O_EXCL;
		break;
	}
	return iOpenFlag;
}

static int FileShareFlagFromStreamMode(StreamModeE streamMode, OpenModeE openMode, ShareModeE shareMode)
{
	int iShareFlag = 0;

	switch(shareMode)
	{
	case ShareModeReadOnly:
		iShareFlag |= _SH_DENYWR;
		break;
	case ShareModeWriteOnly:
		iShareFlag |= _SH_DENYRD;
		break;
	case ShareModeAll:
		iShareFlag |= _SH_DENYNO;
		break;
	default:
		iShareFlag |= _SH_DENYRW;
		break;
	}
	return iShareFlag;
}


CFileStream::CFileStream(textw filePath, StreamModeE streamMode, OpenModeE openMode, ShareModeE shareMode)
	:m_iFile(INVALID_FILE), m_streamMode(StreamModeNone), m_openMode(OpenModeOpen), m_shareMode(ShareModeAll)
{
	Open(filePath, streamMode, openMode, shareMode);
}

CFileStream::CFileStream(texta filePath, StreamModeE streamMode, OpenModeE openMode, ShareModeE shareMode)
{
	Open(filePath, streamMode, openMode, shareMode);
}

CFileStream::~CFileStream()
{
	Close();
}

bool CFileStream::IsValid() const
{
	return m_iFile != -1;
}

bool CFileStream::CanRead() const noexcept
{
	if(m_iFile == INVALID_FILE)
		return false;

	return (m_streamMode & StreamModeRead) == StreamModeRead;
}

bool CFileStream::CanWrite() const noexcept
{
	if(m_iFile == INVALID_FILE)
		return false;

	return IsValid() && (m_streamMode & (StreamModeWrite | StreamModeAppend));
}

bool CFileStream::CanSeek() const
{
	if(m_iFile == INVALID_FILE)
		return false;
	return true;
}
int_x CFileStream::Seek(SeekE seek, int_x iSeek/* = 0*/)
{
	if(m_iFile == INVALID_FILE)
		return -1;

	switch(seek)
	{
	case SeekBegin:
		_lseek(m_iFile, (int_32)iSeek, SEEK_SET);
		break;
	case SeekEnd:
		_lseek(m_iFile, (int_32)iSeek, SEEK_END);
		break;
	case SeekCurr:
		_lseek(m_iFile, (int_32)iSeek, SEEK_CUR);
		break;
	case SeekTell:
		break;
	default:
		Assert(0);
		break;
	}
	return (int_x)_tell(m_iFile);
}

int_x CFileStream::ReadAviliable() const noexcept
{
	if(m_iFile == INVALID_FILE)
		return 0;

	if(!(m_streamMode & StreamModeRead))
		return 0;
	int_x iCurr = _tell(m_iFile);
	_lseek(m_iFile, 0, SEEK_END);
	int_x iAviliable = _tell(m_iFile) - iCurr;
	_lseek(m_iFile, static_cast<int_32>(iCurr), SEEK_SET);
	return iAviliable;
}

byte_t CFileStream::Read()
{
	if(m_iFile == INVALID_FILE)
		return 0;

	if(ReadAviliable())
	{
		byte_t temp = 0;
		int_x iRead = _read(m_iFile, &temp, 1);
		if(iRead == 1)
			return temp;
		else
			throw exp_bad_state();
	}
	else
		throw exp_end_of_stream();
}

int_x CFileStream::Read(void * pBytes, int_x iLength)
{
	if(m_iFile == INVALID_FILE)
		return 0;

	int_x iRead = (int_x)_read(m_iFile, pBytes, (int_32)iLength);
	return iRead;
}

int_x CFileStream::WriteAviliable() const noexcept
{
	if(m_iFile == INVALID_FILE)
		return 0;

	if(!(m_streamMode & (StreamModeWrite | StreamModeAppend)))
		return 0;
	return IX_MAX;
}


void CFileStream::Write(byte_t byte)
{
	if(!WriteAviliable())
		throw exp_end_of_stream(_T("已达文件末尾！"));
	int_x iWriten = _write(m_iFile, &byte, 1);
	if(iWriten != 1)
		throw exp_bad_state(_T("写入出错"));
}

void CFileStream::Write(const void * pBytes, int_x iLength)
{
	if(WriteAviliable() < iLength)
		throw exp_end_of_stream(_T("文件剩余字节数不足！"));

	int_x iWriten = (int_x)_write(m_iFile, pBytes, (int_32)iLength);
	if(iWriten != iLength)
		throw exp_bad_state(_T("写入出错"));
}
void CFileStream::Flush()
{
	if(m_iFile == INVALID_FILE)
		return;

	_commit(m_iFile);
}

byte_buffer_t CFileStream::ReadFile()
{
	byte_buffer_t buffer;
	int_x iFileSize = GetFileSize();
	buffer.reallocate(iFileSize);
	buffer.resize(iFileSize);

	Seek(SeekBegin, 0);
	Read(buffer.buffer, iFileSize);
	return buffer;
}

int_x CFileStream::GetCurr()
{
	if(m_iFile == INVALID_FILE)
		return 0;
	return Seek(SeekTell);
}

int_x CFileStream::GetFileSize() const
{
	if(m_iFile == INVALID_FILE)
		return 0;

	int_x iCurr = _tell(m_iFile);
	_lseek(m_iFile, 0, SEEK_END);
	int_x iSize = _tell(m_iFile);
	_lseek(m_iFile, (int_32)iCurr, SEEK_SET);
	return iSize;
}

bool CFileStream::SetFileSize(int_x iSize)
{
	if(m_iFile == INVALID_FILE)
		return false;

	if(iSize < 0 || iSize > I32_MAX)
		throw exp_illegal_argument();
	throw exp_not_supported();
}

bool CFileStream::Open(textw filePath, StreamModeE streamMode, OpenModeE openMode, ShareModeE shareMode)
{
	Close();
	errno_t err = _wsopen_s(&m_iFile, filePath,
		FileOpenFlagFromStreamMode(streamMode, openMode, shareMode),
		FileShareFlagFromStreamMode(streamMode, openMode, shareMode),
		_S_IREAD | _S_IWRITE);
	if(err)
	{
		log2(L"Can not open file \"%s\"", filePath);
		m_iFile = INVALID_FILE;
		m_streamMode = StreamModeNone;
		m_openMode = OpenModeOpen;
		m_shareMode = ShareModeNone;
		return false;
	}
	else
	{
		m_streamMode = streamMode;
		m_openMode = openMode;
		m_shareMode = shareMode;
		return true;
	}
}

bool CFileStream::Open(texta filePath, StreamModeE streamMode, OpenModeE openMode, ShareModeE shareMode)
{
	Close();
	errno_t err = _sopen_s(&m_iFile, filePath,
		FileOpenFlagFromStreamMode(streamMode, openMode, shareMode),
		FileShareFlagFromStreamMode(streamMode, openMode, shareMode),
		_S_IREAD | _S_IWRITE);
	if(err)
	{
		log2(L"Can not open file \"%s\"", filePath);
		m_iFile = INVALID_FILE;
		m_streamMode = StreamModeNone;
		m_openMode = OpenModeOpen;
		m_shareMode = ShareModeNone;
		return false;
	}
	else
	{
		m_streamMode = streamMode;
		m_openMode = openMode;
		m_shareMode = shareMode;
		return true;
	}
}

void CFileStream::Close()
{
	if(m_iFile != INVALID_FILE)
	{
		_close(m_iFile);
		m_iFile = -1;
		m_streamMode = StreamModeNone;
		m_openMode = OpenModeOpen;
		m_shareMode = ShareModeNone;
	}
}

VENUS_END
