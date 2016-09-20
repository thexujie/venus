#pragma once

#include "IStream.h"

VENUS_BEG

class CORE_API CTextReader : public IInputStream, public ITextStream
{
public:
	CTextReader(IInputStream * pInputStream, encoding_t encoding = encodings::unknown);
	~CTextReader();

	bool CanRead() const noexcept;
	int_x ReadAviliable() const noexcept;

	byte_t Read();
	int_x Read(void * data, int_x size);

	bool CanSeek() const;
	int_x Seek(SeekE seek, int_x iSeek);

	void SetEncoding(encoding_t encoding);
	encoding_t GetEncoding() const;

	int_32 ReadChar();

	int_x ReadText(char_16 * szBuffer, int_x iLength);
	textw ReadText(int_x iLength);
	texta ReadTextA(int_x iLength);

	int_x ReadLine(char_16 * szBuffer, int_x iSize);
	textw ReadLine();
	texta ReadLineA();

protected:
	void _Ready() const;

	int_32 ReadCharAnsi(byte_t last);
	int_32 ReadCharUtf8();
	int_32 ReadCharUtf16LE();
	int_32 ReadCharUtf16BE();

protected:
	IInputStream * m_pInputStream;
	encoding_t m_encoding;
};

class CORE_API CTextWriter : public IOutputStream
{
public:
	CTextWriter(IOutputStream * pOutputStream, encoding_t encoding = encodings::unknown);
	~CTextWriter();

	bool CanWrite() const noexcept;

	int_x WriteAviliable() const noexcept;
	void Write(byte_t byte);
	void Write(const void * data, int_x size);
	void Flush();

	bool CanSeek() const;
	int_x Seek(SeekE seek, int_x iSeek);

	void SetEncoding(encoding_t encoding);
	encoding_t GetEncoding() const;

	void SetLineTag(linetag_e linetag);
	linetag_e GetLineTag() const;

	void Write(const char_16 * pText, int_x iLength = -1);
	void Write(const textw & text);
	void Write(const char_8 * pText, int_x iLength = -1);
	void Write(const texta & text);

	void WriteLine();
	void WriteLine(const char_16 * pText, int_x iLength = -1);
	void WriteLine(const textw & text);
	void WriteLine(const char_8 * pText, int_x iLength = -1);
	void WriteLine(const texta & text);

	void Format(const char_16 * szFormat, ...);
	void Format(const char_8 * szFormat, ...);
	void FormatLine(const char_16 * szFormat, ...);
	void FormatLine(const char_8 * szFormat, ...);


protected:
	void _Ready() const;
	void _WriteBOM();

protected:
	IOutputStream * m_pOutputStream;
	encoding_t m_encoding;
	linetag_e m_linetag;
	bool m_bom_ready;
};

VENUS_END
