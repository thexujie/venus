#include "stdafx.h"
#include "CTextStream.h"
#include "codepage.h"

VENUS_BEG

CTextReader::CTextReader(IInputStream * pInputStream, encoding_t encoding)
	: m_pInputStream(pInputStream), m_encoding(encoding)
{
	
}

CTextReader::~CTextReader()
{
	m_pInputStream = nullptr;
}

bool CTextReader::CanRead() const noexcept
{
	return m_pInputStream->CanRead();
}

int_x CTextReader::ReadAviliable() const noexcept
{
	_Ready();
	return m_pInputStream->ReadAviliable();
}

byte_t CTextReader::Read()
{
	_Ready();
	return m_pInputStream->Read();
}

int_x CTextReader::Read(void * data, int_x size)
{
	_Ready();
	return m_pInputStream->Read(data, size);
}

bool CTextReader::CanSeek() const
{
	_Ready();
	return m_pInputStream->CanSeek();
}
int_x CTextReader::Seek(SeekE seek, int_x iSeek)
{
	_Ready();
	return m_pInputStream->Seek(seek, iSeek);
}

void CTextReader::SetEncoding(encoding_t encoding)
{
	m_encoding = encoding;
}

encoding_t CTextReader::GetEncoding() const
{
	_Ready();
	return m_encoding;
}

void CTextReader::_Ready() const
{
	Verify(m_pInputStream);
}

int_32 CTextReader::ReadChar()
{
	Verify(m_pInputStream);

	if(m_encoding == encodings::unknown)
	{
		int_32 ch = Read();

		// le tag
		if(ch == 0xEF)
		{
			ch = Read();
			if(ch != 0xBB)
				throw exp_io();
			ch = Read();
			if(ch != 0xBF)
				throw exp_io();
			m_encoding = encodings::utf8;
		}
		else if(ch == 0xFF)
		{
			ch = Read();
			if(ch != 0xFE)
				throw exp_io();
			m_encoding = encodings::utf16;
		}
		else if(ch == 0xFE)
		{
			ch = Read();
			if(ch != 0xFF)
				throw exp_io();
			m_encoding = encodings::utf16_be;
		}
		else if(ch == 0x5c)
		{
			ch = Read();
			if(ch != 0x75)
				throw exp_io();
			m_encoding = encodings::ansi;
		}
		else
		{
			m_encoding = encodings::ansi;
			return ReadCharAnsi(ch);
		}
	}
	else{}

	int_32 ch = 0;
	switch(m_encoding.codepage)
	{
	case encodings::ansi.codepage:
		// TODO：检测默认的编码
		ch = ReadCharAnsi(0);
		break;
	case encodings::utf16.codepage:
		ch = ReadCharUtf16LE();
		break;
	case encodings::utf16_be.codepage:
		ch = ReadCharUtf16BE();
		break;
	case encodings::utf8.codepage:
		ch = ReadCharUtf8();
		break;
	default:
		ch = Read();
		break;
	}
	return ch;

}

int_x CTextReader::ReadText(char_16 * szBuffer, int_x iLength)
{
	char_16 ch = 0;
	bool bRuning = true;
	int_x iIndex = 0;
	while(bRuning)
	{
		if(m_pInputStream->ReadAviliable() && iLength-- > 0)
		{
			ch = ReadChar();
			szBuffer[iIndex++] = ch;
		}
		else
			bRuning = false;
	}
	szBuffer[iIndex] = L'\0';
	return iIndex;
}

textw CTextReader::ReadText(int_x iLength)
{
	textw text;
	bool bRuning = true;
	while(bRuning)
	{
		if(m_pInputStream->ReadAviliable() && iLength-- > 0)
		{
			char_16 ch = ReadChar();
			text.append(ch);
		}
		else
			bRuning = false;
	}
	return text;
}

texta CTextReader::ReadTextA(int_x iLength)
{
	texta text;
	bool bRuning = true;
	while(bRuning)
	{
		if(m_pInputStream->ReadAviliable() && iLength-- > 0)
		{
			char_8 ch = Read();
			text.append(ch);
		}
		else
			bRuning = false;
	}
	return text;
}

int_x CTextReader::ReadLine(char_16 * szBuffer, int_x iSize)
{
	--iSize;
	char_16 ch = 0;
	bool bRuning = true;
	int_x iIndex = 0;
	while(bRuning)
	{
		if(m_pInputStream->ReadAviliable())
		{
			ch = ReadChar();
			if(ch == L'\r')
				continue;

			switch(ch)
			{
			case L'\u000A': // LF 换行
			case L'\u000B': // VT 垂直定位
			case L'\u000C': // FF 换页
			case L'\u2028': // LS 分行
			case L'\u2029': // PS 分段
				bRuning = false;
				break;
			default:
				if(iIndex < iSize)
					szBuffer[iIndex++] = ch;
				break;
			}
		}
		else
			bRuning = false;
	}
	szBuffer[iIndex] = L'\0';
	return iIndex;
}

textw CTextReader::ReadLine()
{
	textw text;
	char_16 ch = 0;
	bool bRuning = true;
	while(bRuning)
	{
		if(m_pInputStream->ReadAviliable())
		{
			ch = ReadChar();
			if(ch == L'\r')
				continue;

			switch(ch)
			{
			case L'\u000A': // LF 换行
			case L'\u000B': // VT 垂直定位
			case L'\u000C': // FF 换页
			case L'\u2028': // LS 分行
			case L'\u2029': // PS 分段
				bRuning = false;
				break;
			default:
				text.append(ch);
				break;
			}
		}
		else
			bRuning = false;
	}
	return text;
}

texta CTextReader::ReadLineA()
{
	texta text;
	char_8 ch = 0;
	bool bRuning = true;
	while(bRuning)
	{
		if(m_pInputStream->ReadAviliable())
		{
			ch = Read();
			if(ch == L'\r')
				continue;

			switch(ch)
			{
			case L'\u000A': // LF 换行
			case L'\u000B': // VT 垂直定位
			case L'\u000C': // FF 换页
				bRuning = false;
				break;
			default:
				text.append(ch);
				break;
			}
		}
		else
			bRuning = false;
	}
	return text;
}

int_32 CTextReader::ReadCharUtf16LE()
{
	_Ready();
	union
	{
		uint_16 val;
		struct
		{
			uint_8 aval;
			uint_8 bval;
		};
	};

	aval = m_pInputStream->Read();
	bval = m_pInputStream->Read();
	return val;
}

int_32 CTextReader::ReadCharUtf16BE()
{
	_Ready();
	union
	{
		uint_16 val;
		struct
		{
			uint_8 aval;
			uint_8 bval;
		};
	};

	bval = m_pInputStream->Read();
	aval = m_pInputStream->Read();
	return val;
}

int_32 CTextReader::ReadCharAnsi(byte_t last)
{
	_Ready();

	byte_t bVal = last ? last : m_pInputStream->Read();
	if(bVal >= 0x80)
	{
		byte_t bVal2 = m_pInputStream->Read();
		byte_t vals[2] = { bVal, bVal2 };
		char_16 ch;
		int_x length = 0;
		err_t err = encodings::utf16.encode(encodings::ansi, vals, 2, &ch, 1, &length);
		return ch;
	}
	else
		return bVal;
}

int_32 CTextReader::ReadCharUtf8()
{
	_Ready();

	byte_t bVal = m_pInputStream->Read();
	if(bVal & 0x80)
	{
		int_x iCount = 1;
		int_32 iChar = 0;
		if((bVal & 0xFE) == 0xFC)
		{
			iCount = 6;
			iChar = bVal & 0x1;
		}
		else if((bVal & 0xFC) == 0xF8)
		{
			iCount = 5;
			iChar = bVal & 0x3;
		}
		else if((bVal & 0xF8) == 0xF0)
		{
			iCount = 4;
			iChar = bVal & 0x7;
		}
		else if((bVal & 0xF0) == 0xE0)
		{
			iCount = 3;
			iChar = bVal & 0xF;
		}
		else if((bVal & 0xE0) == 0xC0)
		{
			iCount = 2;
			iChar = bVal & 0x1F;
		}
		else
			throw exp_bad_state();

		while(--iCount)
		{
			bVal = m_pInputStream->Read();
			iChar <<= 6;
			iChar |= (bVal & 0x3F);
		}
		return iChar;
	}
	else
		return bVal;
}



CTextWriter::CTextWriter(IOutputStream * pOutputStream, encoding_t encoding) :
	m_pOutputStream(pOutputStream), m_encoding(encoding), m_bom_ready(false)
{
}

CTextWriter::~CTextWriter()
{
}

bool CTextWriter::CanWrite() const noexcept
{
	if(m_pOutputStream)
		return m_pOutputStream->CanWrite();
	else
		return false;
}

int_x CTextWriter::WriteAviliable() const noexcept
{
	if(m_pOutputStream)
		return m_pOutputStream->WriteAviliable();
	else
		return 0;
}

void CTextWriter::Write(byte_t byte)
{
	_Ready();
	return m_pOutputStream->Write(byte);
}

void CTextWriter::Write(const void * data, int_x size)
{
	_Ready();
	return m_pOutputStream->Write(data, size);
}

void CTextWriter::Flush()
{
	if(m_pOutputStream)
		m_pOutputStream->Flush();
}

bool CTextWriter::CanSeek() const
{
	if(m_pOutputStream)
		return m_pOutputStream->CanSeek();
	else
		return false;
}

int_x CTextWriter::Seek(SeekE seek, int_x iSeek)
{
	if(m_pOutputStream)
		return m_pOutputStream->Seek(seek, iSeek);
	else
		return 0;
}

void CTextWriter::SetEncoding(encoding_t encoding)
{
	m_encoding = encoding;
}

encoding_t CTextWriter::GetEncoding() const
{
	return m_encoding;
}

void CTextWriter::Write(const char_16 * pText, int_x iLength)
{
	_Ready();
	_WriteBOM();

	if(iLength < 0)
		iLength = textlen(pText);

	if(m_encoding.singlebyte())
	{
		texta text = encodings::encocde(pText, iLength, encodings::utf16, m_encoding);
		m_pOutputStream->Write(text.buffer(), text.length());
	}
	else if(m_encoding == encodings::utf16)
	{
		m_pOutputStream->Write(pText, iLength * 2);
	}
	else
		throw exp_not_supported();
}

void CTextWriter::Write(const textw & text)
{
	_Ready();
	_WriteBOM();

	if(m_encoding.singlebyte())
	{
		texta dst_text = encodings::encocde(text, encodings::utf16, m_encoding);
		m_pOutputStream->Write(dst_text.buffer(), dst_text.length());
	}
	else if(m_encoding == encodings::utf16)
	{
		m_pOutputStream->Write(text, text.length() * 2);
	}
	else
		throw exp_not_supported();
}

void CTextWriter::Write(const char_8 * pText, int_x iLength)
{
	_Ready();
	_WriteBOM();

	if(iLength < 0)
		iLength = textlen(pText);

	if(m_encoding.singlebyte())
	{
		m_pOutputStream->Write(pText, iLength);
	}
	else
	{
		textw dst_text = encodings::encocde(pText, iLength, encodings::ansi, m_encoding);
		m_pOutputStream->Write(dst_text.buffer(), dst_text.length() * 2);
	}
}

void CTextWriter::Write(const texta & text)
{
	_Ready();
	_WriteBOM();

	if(m_encoding.singlebyte())
	{
		m_pOutputStream->Write(text, text.length());
	}
	else
	{
		textw dst_text = encodings::encocde(text, encodings::ansi, m_encoding);
		m_pOutputStream->Write(dst_text.buffer(), dst_text.length() * 2);
	}
}

void CTextWriter::WriteLine(const char_16 * pText, int_x iLength, LineTagE eLineTag)
{
	Write(pText, iLength);
	NewLine(eLineTag);
}

void CTextWriter::WriteLine(const textw & text, LineTagE eLineTag)
{
	Write(text);
	NewLine(eLineTag);
}

void CTextWriter::WriteLine(const char_8 * pText, int_x iLength, LineTagE eLineTag)
{
	Write(pText, iLength);
	NewLine(eLineTag);
}

void CTextWriter::WriteLine(const texta & text, LineTagE eLineTag)
{
	Write(text);
	NewLine(eLineTag);
}

void CTextWriter::NewLine(LineTagE eLineTag)
{
	static const char_16 LINE_TAG_0[] = L"\0";
	static const char_16 LINE_TAG_R[] = L"\r";
	static const char_16 LINE_TAG_N[] = L"\n";
	static const char_16 LINE_TAG_RN[] = L"\r\n";

	static const char_8 A_LINE_TAG_0[] = "\0";
	static const char_8 A_LINE_TAG_R[] = "\r";
	static const char_8 A_LINE_TAG_N[] = "\n";
	static const char_8 A_LINE_TAG_RN[] = "\r\n";

	switch(eLineTag)
	{
	case LineTagNone:
		break;
	case LineTag0:
		if(m_encoding.singlebyte())
			Write(A_LINE_TAG_0, 1);
		else
			Write(LINE_TAG_0, 1);
		break;
	case LineTagR:
		if(m_encoding.singlebyte())
			Write(A_LINE_TAG_R, 1);
		else
			Write(LINE_TAG_R, 1);
		break;
	case LineTagN:
		if(m_encoding.singlebyte())
			Write(A_LINE_TAG_N, 1);
		else
			Write(LINE_TAG_N, 1);
		break;
	case LineTagRN:
		if(m_encoding.singlebyte())
			Write(A_LINE_TAG_RN, 1);
		else
			Write(LINE_TAG_RN, 1);
		break;
	default:
		break;
	}
}

void CTextWriter::WriteFormat(const char_16 * szFormat, ...)
{
	textw buffer;
	va_list pArgs = nullptr;
	va_start(pArgs, szFormat);
	buffer.format_args(szFormat, pArgs);
	va_end(pArgs);
	WriteLine(buffer, LineTagNone);
}

void CTextWriter::WriteFormat(const char_8 * szFormat, ...)
{
	texta buffer;
	va_list pArgs = nullptr;
	va_start(pArgs, szFormat);
	buffer.format_args(szFormat, pArgs);
	va_end(pArgs);
	WriteLine(buffer, LineTagNone);
}

void CTextWriter::_Ready() const
{
	if(!m_pOutputStream)
		throw exp_nullptr();
}

void CTextWriter::_WriteBOM()
{
	if(m_bom_ready)
		return;

	_Ready();
	m_bom_ready = true;

	const byte_t BOM_ANSI[] = { 0x5c, 0x75 };
	const byte_t BOM_UTF8[] = { 0xef, 0xbb, 0xbf };
	const byte_t BOM_UTF16[] = { 0xff, 0xfe };
	const byte_t BOM_UTF16_BE[] = { 0xfe, 0xff };
	switch(m_encoding.codepage)
	{
	case encodings::unknown.codepage:
		m_encoding = encodings::ansi;
		break;
	case encodings::ansi.codepage:
		Write(BOM_ANSI, arraysize(BOM_ANSI));
		break;
	case encodings::utf8.codepage:
		Write(BOM_UTF8, arraysize(BOM_UTF8));
		break;
	case encodings::utf16.codepage:
		Write(BOM_UTF16, arraysize(BOM_UTF16));
		break;
	case encodings::utf16_be.codepage:
		Write(BOM_UTF16_BE, arraysize(BOM_UTF16_BE));
		break;
	default:
		break;
	}
}



VENUS_END
