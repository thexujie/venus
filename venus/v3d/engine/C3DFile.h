#pragma once

VENUS_BEG

class C3DFile : public ObjectT<I3DFile>
{
public:
	C3DFile();
	~C3DFile();

	err_t Load(const char_16 * szFile);

	int_x GetSize() const;
	const byte_t * GetBuffer() const;

	bool CanRead() const;
	int_x ReadAviliable();
	int_x Read(void * pBytes, int_x iLength);
	byte_t ReadByte();
	bool SeekSurpport(StreamSeekE seek) const;
	int_x Seek(StreamSeekE seek, int_x iSeek);

protected:
	int_x m_size;
	int_x m_curr;
	byte_t * m_buffer;
#ifdef _DEBUG
	chbufferw<MAX_FILE_PATH> m_path;
#endif
};

VENUS_END
