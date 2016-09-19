#pragma once

#include "IStream.h"

VENUS_BEG

class CORE_API CTextReader : public IInputStream, public ITextStream
{
public:
	CTextReader(IInputStream * pInputStream, EncodingE eEncoding = EncodingUnknown);
	~CTextReader();

	bool CanRead() const noexcept;
	int_x ReadAviliable() const noexcept;

	byte_t Read();
	int_x Read(void * data, int_x size);

	bool CanSeek() const;
	int_x Seek(SeekE seek, int_x iSeek);

	void SetEncoding(EncodingE eEncoding);
	EncodingE GetEncoding() const;

	int_32 ReadChar();

	int_x ReadText(char_16 * szBuffer, int_x iLength);
	textw ReadText(int_x iLength);
	texta ReadTextA(int_x iLength);

	int_x ReadLine(char_16 * szBuffer, int_x iSize);
	textw ReadLine();
	texta ReadLineA();
	EncodingE ReadEncoding();

protected:
	void _Ready() const;

	int_32 ReadCharUtf8();
	int_32 ReadCharUtf16LE();
	int_32 ReadCharUtf16BE();

	int_32 ReadCharGBK();

protected:
	IInputStream * m_pInputStream;
	EncodingE m_eEncoding;
};

class CORE_API CTextWriter : public IOutputStream
{
public:
	CTextWriter(IOutputStream * pOutputStream, EncodingE eEncoding = EncodingUnknown);
	~CTextWriter();

	bool CanWrite() const noexcept;

	int_x WriteAviliable() const noexcept;
	void Write(byte_t byte);
	void Write(const void * data, int_x size);
	void Flush();

	bool CanSeek() const;
	int_x Seek(SeekE seek, int_x iSeek);

	void SetEncoding(EncodingE eEncoding);
	EncodingE GetEncoding() const;

	void WriteChar(int_32 ch);

	void WriteText(const char_16 * pText, int_x iLength = -1);
	void WriteText(const textw & text);
	void WriteTextA(const texta & text);

	void WriteLine(const char_16 * pText, int_x iLength = -1, LineTagE eLineTag = LineTagN);
	void WriteLine(const textw & text, LineTagE eLineTag = LineTagN);
	void WriteLineA(const texta & text, LineTagE eLineTag = LineTagN);

	void WriteFormat(const char_16 * szFormat, ...);
	void WriteLineTag(LineTagE eLineTag = LineTagN);
	void WriteLineTagA(LineTagE eLineTag = LineTagN);

protected:
	void _Ready() const;

	void WriteCharUtf8(int_32 ch);
	void WriteCharUtf16LE(int_32 ch);
	void WriteCharUtf16BE(int_32 ch);

	void WriteCharGBK(int_32 ch);

protected:
	IOutputStream * m_pOutputStream;
	textw m_bufferW;
	EncodingE m_eEncoding;
};

VENUS_END
