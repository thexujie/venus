#pragma once

#include "IStream.h"

VENUS_BEG

class CORE_API CFileStream : public IIOStream
{
private:
	CFileStream();
public:
	CFileStream(textw filePath, StreamModeE streamMode, OpenModeE openMode = OpenModeOpen, ShareModeE shareMode = ShareModeAll);
	CFileStream(texta filePath, StreamModeE streamMode, OpenModeE openMode = OpenModeOpen, ShareModeE shareMode = ShareModeAll);
	~CFileStream();

public:
	bool IsValid() const;
	bool CanRead() const;
	bool CanWrite() const;

	bool SeekSurpport(StreamSeekE seek) const;
	int_x Seek(StreamSeekE seek, int_x iSeek = 0);

	int_x ReadAviliable();
	byte_t ReadByte();
	int_x Read(void * pBytes, int_x iLength);

	int_x WriteAviliable();
	void WriteByte(byte_t byte);
	void Write(const void * pData, int_x iLength);
	void Flush();

	byte_buffer_t ReadFile();
public:
	int_x GetCurr();
	int_x GetFileSize() const;
	bool SetFileSize(int_x iSize);

	bool Open(textw filePath, StreamModeE streamMode, OpenModeE openMode = OpenModeOpen, ShareModeE shareMode = ShareModeAll);
	bool Open(texta filePath, StreamModeE streamMode, OpenModeE openMode = OpenModeOpen, ShareModeE shareMode = ShareModeAll);
	void Close();

protected:
	int m_iFile;
	struct
	{
		StreamModeE m_streamMode : 8;
		OpenModeE m_openMode : 8;
		ShareModeE m_shareMode : 8;
	};
};

VENUS_END
