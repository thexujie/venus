#include "stdafx.h"
#include "CTextStream.h"
#include "codepage.h"

VENUS_BEG

CTextReader::CTextReader(IInputStream * pInputStream)
	: m_pInputStream(pInputStream), m_eEncoding(EncodingUnknown)
{

}

CTextReader::~CTextReader()
{
	m_pInputStream = nullptr;
	m_eEncoding = EncodingUnknown;
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

void CTextReader::SetEncoding(EncodingE eEncoding)
{
	m_eEncoding = eEncoding;
}

EncodingE CTextReader::GetEncoding() const
{
	_Ready();
	return m_eEncoding;
}

EncodingE CTextReader::ReadEncoding()
{
	_Ready();
	EncodingE eEncoding = EncodingDefault;
	int_x iAviliable = m_pInputStream->ReadAviliable();
	if(iAviliable >= 2)
	{
		uint_16 uiCoding = 0;
		m_pInputStream->Read(&uiCoding, 2);
		switch(uiCoding)
		{
		case 0xBBEF:
			if(m_pInputStream->ReadAviliable())
			{
				switch(m_pInputStream->Read())
				{
				case 0xBF:
					eEncoding = EncodingUtf8;
					break;
				default:
					throw exp_not_supported();
				}
			}
			break;
		case 0xFEFF:
			eEncoding = EncodingUnicode16;
			break;
		case 0xFFFE:
			eEncoding = EncodingUnicode16BigEndian;
			break;
		default:
			eEncoding = EncodingAnsi;
			m_pInputStream->Seek(SeekCurr, -2);
			break;
		}
	}
	else // 字节数不够，无法判断
		eEncoding = EncodingDefault;
	SetEncoding(eEncoding);
	return eEncoding;
}

void CTextReader::_Ready() const
{
	Verify(m_pInputStream);
}

int_32 CTextReader::_ReadChar()
{
	int_32 ch = 0;
	switch(m_eEncoding)
	{
	case EncodingAnsi:
		// TODO：检测默认的编码
		ch = ReadCharGBK();
		break;
	case EncodingGBK:
		ch = ReadCharGBK();
		break;
	case EncodingUnicode16:
		ch = ReadCharUtf16LE();
		break;
	case EncodingUnicode16BigEndian:
		ch = ReadCharUtf16BE();
		break;
	case EncodingUtf8:
		ch = ReadCharUtf8();
		break;
	default:
		ch = ReadCharUtf8();
		break;
	}
	return ch;
}

int_32 CTextReader::ReadChar()
{
	Verify(m_pInputStream);

	if(m_eEncoding == EncodingUnknown)
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
			m_eEncoding = EncodingUtf8;
			return _ReadChar();
		}
		else if(ch == 0xFF)
		{
			ch = Read();
			if(ch != 0xFE)
				throw exp_io();
			m_eEncoding = EncodingUnicode16;
			return _ReadChar();
		}
		else if(ch == 0xFE)
		{
			ch = Read();
			if(ch != 0xFF)
				throw exp_io();
			m_eEncoding = EncodingUnicode16BigEndian;
			return _ReadChar();
		}
		else if(ch == 0x5c)
		{
			ch = Read();
			if(ch != 0x75)
				throw exp_io();
			m_eEncoding = EncodingAnsi;
			return _ReadChar();
		}
		else
		{
			m_eEncoding = EncodingDefault;
			return ch;
		}
	}
	else
		return _ReadChar();
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

int_32 CTextReader::ReadCharGBK()
{
	_Ready();

	byte_t bVal = m_pInputStream->Read();
	if(bVal >= 0x80)
	{
		byte_t bVal2 = m_pInputStream->Read();
		return CP936ToUnicode(bVal, bVal2);
	}
	else
		return bVal;
}



CTextWriter::CTextWriter(IOutputStream * pOutputStream) :
	m_pOutputStream(pOutputStream)
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
	if(m_pOutputStream)
		return m_pOutputStream->Write(byte);
}

void CTextWriter::Write(const void * data, int_x size)
{
	if(m_pOutputStream)
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

void CTextWriter::WriteText(const char_16 * pText, int_x iLength)
{
	if(iLength < 0)
		iLength = textlen(pText);
	m_pOutputStream->Write(pText, iLength * 2);
}

void CTextWriter::WriteText(const textw & text)
{
	m_pOutputStream->Write(text, text.length() * 2);
}

void CTextWriter::WriteTextA(const texta & text)
{
	m_pOutputStream->Write(text, text.length());
}

void CTextWriter::WriteLine(const char_16 * pText, int_x iLength, LineTagE eLineTag)
{
	WriteText(pText, iLength);
	WriteLineTag(eLineTag);
}

void CTextWriter::WriteLine(const textw & text, LineTagE eLineTag)
{
	WriteText(text);
	WriteLineTag(eLineTag);
}

void CTextWriter::WriteLineA(const texta & text, LineTagE eLineTag)
{
	WriteTextA(text);
	WriteLineTagA(eLineTag);
}

void CTextWriter::WriteLineTag(LineTagE eLineTag)
{
	static const char_16 LINE_TAG_0[] = L"\0";
	static const char_16 LINE_TAG_R[] = L"\r";
	static const char_16 LINE_TAG_N[] = L"\n";
	static const char_16 LINE_TAG_RN[] = L"\r\n";

	switch(eLineTag)
	{
	case LineTagNone:
		break;
	case LineTag0:
		m_pOutputStream->Write(LINE_TAG_0, 2);
		break;
	case LineTagR:
		m_pOutputStream->Write(LINE_TAG_R, 2);
		break;
	case LineTagN:
		m_pOutputStream->Write(LINE_TAG_N, 2);
		break;
	case LineTagRN:
		m_pOutputStream->Write(LINE_TAG_RN, 4);
		break;
	default:
		break;
	}
}

void CTextWriter::WriteLineTagA(LineTagE eLineTag)
{
	static const char_8 LINE_TAG_0[] = "\0";
	static const char_8 LINE_TAG_R[] = "\r";
	static const char_8 LINE_TAG_N[] = "\n";
	static const char_8 LINE_TAG_RN[] = "\r\n";

	switch(eLineTag)
	{
	case LineTagNone:
		break;
	case LineTag0:
		m_pOutputStream->Write(LINE_TAG_0, 1);
		break;
	case LineTagR:
		m_pOutputStream->Write(LINE_TAG_R, 1);
		break;
	case LineTagN:
		m_pOutputStream->Write(LINE_TAG_N, 1);
		break;
	case LineTagRN:
		m_pOutputStream->Write(LINE_TAG_RN, 2);
		break;
	default:
		break;
	}
}

void CTextWriter::WriteFormat(const char_16 * szFormat, ...)
{
	va_list pArgs = nullptr;
	va_start(pArgs, szFormat);
	m_bufferW.format_args(szFormat, pArgs);
	va_end(pArgs);
	WriteLine(m_bufferW, LineTagNone);
}

VENUS_END
