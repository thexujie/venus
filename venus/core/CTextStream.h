#pragma once

#include "IStream.h"

VENUS_BEG

class CORE_API CTextInputStream : public ITextInputStream
{
public:
	CTextInputStream(IInputStream * pInputStream);
	~CTextInputStream();

	bool CanRead() const;
	int_x ReadAviliable();

	int_x Read(void * pBytes, int_x iLength);
	byte_t ReadByte();

	bool SeekSurpport(StreamSeekE seek) const;
	int_x Seek(StreamSeekE seek, int_x iSeek);

	IInputStream * GetInputStream();
	IInputStream * ResetInputStream(IInputStream * pNewInputStream);

	void SetTextInputEncoding(TextEncodingE eEncoding);
	TextEncodingE GetTextInputEncoding() const;
	void SetCodePage(int_x eCodePage);
	int_x GetCodepage() const;

	int_32 ReadChar();
	int_x ReadLine(char_16 * szBuffer, int_x iSize);
	texta ReadLineA();
	textw ReadLineW();
	TextEncodingE ReadEncoding();

private:
	void ConfirmInputStream() const;

	int_32 ReadCharUnicode16();
	int_32 ReadCharUnicode16BigEndian();
	int_32 ReadCharUTF8();

	int_32 ReadCharAnsiGB2312();
	int_32 ReadCharAnsiGBK();
private:

private:
	IInputStream * m_pInputStream;
	TextEncodingE m_eEncoding;
	DataAdapter32 m_adapter;
	int_x m_iCodePage;

	typedef int_32 (CTextInputStream::*ReadCharFuncT)();
	ReadCharFuncT m_pfnReadChar;
};

class CORE_API CTextOutputStream : public ITextOutputStream
{
public:
	CTextOutputStream(IOutputStream * pOutputStream);
	~CTextOutputStream();

	bool CanWrite() const;

	int_x WriteAviliable();
	void Write(const void * pData, int_x iLength);
	void WriteByte(byte_t byte);
	void Flush();

	bool SeekSurpport(StreamSeekE seek) const;
	int_x Seek(StreamSeekE seek, int_x iSeek);

	void WriteText(const char_16 * pText, int_x iLength = -1, LineTagE eLineTag = LineTagRN);
	void WriteText(const textw & text, LineTagE eLineTag = LineTagRN);
	void WriteFormat(const char_16 * szFormat, ...);
	void WriteLineTag(LineTagE eLineTag = LineTagRN);
private:
	IOutputStream * m_pOutputStream;
	textw m_bufferW;
};

VENUS_END
