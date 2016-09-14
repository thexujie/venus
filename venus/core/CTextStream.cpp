#include "stdafx.h"
#include "CTextStream.h"
#include "codepage.h"

VENUS_BEG

CTextInputStream::CTextInputStream(IInputStream * pInputStream)
	: m_pInputStream(pInputStream), m_eEncoding(TextEncodingInvalid), m_iCodePage(GetCRTCodePage()), m_pfnReadChar(nullptr)
{

}

CTextInputStream::~CTextInputStream()
{
	m_pInputStream = nullptr;
	m_eEncoding = TextEncodingInvalid;
}

bool CTextInputStream::CanRead() const
{
	ConfirmInputStream();
	return m_pInputStream->CanRead();
}

int_x CTextInputStream::ReadAviliable()
{
	ConfirmInputStream();
	return m_pInputStream->ReadAviliable();
}

byte_t CTextInputStream::ReadByte()
{
	ConfirmInputStream();
	return m_pInputStream->ReadByte();
}

int_x CTextInputStream::Read(void * pBytes, int_x iLength)
{
	ConfirmInputStream();
	return m_pInputStream->Read(pBytes, iLength);
}

bool CTextInputStream::SeekSurpport(StreamSeekE seek) const
{
	ConfirmInputStream();
	return m_pInputStream->SeekSurpport(seek);
}
int_x CTextInputStream::Seek(StreamSeekE seek, int_x iSeek)
{
	ConfirmInputStream();
	return m_pInputStream->Seek(seek, iSeek);
}

IInputStream * CTextInputStream::GetInputStream()
{
	return m_pInputStream;
}
IInputStream * CTextInputStream::ResetInputStream(IInputStream * pNewInputStream)
{
	IInputStream * pOldInputStream = m_pInputStream;
	m_pInputStream = pNewInputStream;
	m_eEncoding = TextEncodingInvalid;
	if(m_pInputStream)
		ReadEncoding();
	return pOldInputStream;
}

void CTextInputStream::SetTextInputEncoding(TextEncodingE eEncoding)
{
	m_eEncoding = eEncoding;
	m_pfnReadChar = nullptr;
	switch(m_eEncoding)
	{
	case TextEncodingAnsi:
		switch(m_iCodePage)
		{
		case CodePageGB2312:
			m_pfnReadChar = &CTextInputStream::ReadCharAnsiGB2312;
			break;
		case CodePageGBK:
			m_pfnReadChar = &CTextInputStream::ReadCharAnsiGBK;
			break;
		}
		break;
	case TextEncodingUnicode16:
		m_pfnReadChar = &CTextInputStream::ReadCharUnicode16;
		break;
	case TextEncodingUnicode16BigEndian:
		m_pfnReadChar = &CTextInputStream::ReadCharUnicode16BigEndian;
		break;
	case TextEncodingUTF8:
		m_pfnReadChar = &CTextInputStream::ReadCharUTF8;
		break;
	default:
		break;
	}
}

TextEncodingE CTextInputStream::GetTextInputEncoding() const
{
	ConfirmInputStream();
	return m_eEncoding;
}

void CTextInputStream::SetCodePage(int_x eCodePage)
{
	m_iCodePage = eCodePage;
	if(m_eEncoding == TextEncodingAnsi)
	{
		switch(m_iCodePage)
		{
		case CodePageGBK:
			m_pfnReadChar = &CTextInputStream::ReadCharAnsiGBK;
			break;
		case CodePageGB2312:
			m_pfnReadChar = &CTextInputStream::ReadCharAnsiGB2312;
			break;
		default:
			m_pfnReadChar = nullptr;
		}
	}
	else {}
}

int_x CTextInputStream::GetCodepage() const
{
	return m_iCodePage;
}

TextEncodingE CTextInputStream::ReadEncoding()
{
	ConfirmInputStream();
	TextEncodingE eEncoding = TextEncodingInvalid;
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
				switch(m_pInputStream->ReadByte())
				{
				case 0xBF:
					eEncoding = TextEncodingUTF8;
					break;
				default:
					throw exp_not_supported();
				}
			}
			break;
		case 0xFEFF:
			eEncoding = TextEncodingUnicode16;
			break;
		case 0xFFFE:
			eEncoding = TextEncodingUnicode16BigEndian;
			break;
		default:
			eEncoding = TextEncodingAnsi;
			m_pInputStream->Seek(StreamSeekCurr, -2);
			break;
		}
	}
	else // 字节数不够，无法判断
		eEncoding = TextEncodingInvalid;
	SetTextInputEncoding(eEncoding);
	return eEncoding;
}

void CTextInputStream::ConfirmInputStream() const
{
	Verify(m_pInputStream);
}


int_32 CTextInputStream::ReadChar()
{
	Verify(m_pInputStream);
	if(m_pfnReadChar)
		return (this->*m_pfnReadChar)();
	else
		return m_pInputStream->ReadByte();
}

int_x CTextInputStream::ReadLine(char_16 * szBuffer, int_x iSize)
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
			if(ch == L'\n')
				bRuning = false;
			else
			{
				if(ch != L'\r' && iIndex < iSize)
					szBuffer[iIndex++] = ch;
				else {}
			}
		}
		else
			bRuning = false;
	}
	szBuffer[iIndex] = L'\0';
	return iIndex;
}

texta CTextInputStream::ReadLineA()
{
	texta text;
	char_8 ch = 0;
	bool bRuning = true;
	while(bRuning)
	{
		if(m_pInputStream->ReadAviliable())
		{
			ch = (char_8)ReadByte();
			if(ch == '\n')
				bRuning = false;
			else
			{
				if(ch != '\r')
					text.append(ch);
				else {}
			}
		}
		else
			bRuning = false;
	}
	return text;
}

textw CTextInputStream::ReadLineW()
{
	textw text;
	char_16 ch = 0;
	bool bRuning = true;
	while(bRuning)
	{
		if(m_pInputStream->ReadAviliable())
		{
			ch = ReadChar();
			if(ch == L'\n')
				bRuning = false;
			else
			{
				if(ch != L'\r')
					text.append(ch);
				else {}
			}
		}
		else
			bRuning = false;
	}
	return text;
}

int_32 CTextInputStream::ReadCharUnicode16()
{
	ConfirmInputStream();
	m_adapter.a = m_pInputStream->ReadByte();
	m_adapter.b = m_pInputStream->ReadByte();
	return m_adapter.c16Val;
}
int_32 CTextInputStream::ReadCharUnicode16BigEndian()
{
	ConfirmInputStream();
	m_adapter.b = m_pInputStream->ReadByte();
	m_adapter.a = m_pInputStream->ReadByte();
	return m_adapter.c16Val;
}
int_32 CTextInputStream::ReadCharUTF8()
{
	ConfirmInputStream();

	byte_t bVal = m_pInputStream->ReadByte();
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
			bVal = m_pInputStream->ReadByte();
			iChar <<= 6;
			iChar |= (bVal & 0x3F);
		}
		return iChar;
	}
	else
		return bVal;
}

int_32 CTextInputStream::ReadCharAnsiGB2312()
{
	ConfirmInputStream();

	byte_t bVal = m_pInputStream->ReadByte();
	if(bVal >= 0x80)
	{
		byte_t bVal2 = m_pInputStream->ReadByte();
		return CP20936ToUnicode(bVal, bVal2);
	}
	else
		return bVal;
}

int_32 CTextInputStream::ReadCharAnsiGBK()
{
	ConfirmInputStream();

	byte_t bVal = m_pInputStream->ReadByte();
	if(bVal >= 0x80)
	{
		byte_t bVal2 = m_pInputStream->ReadByte();
		return CP936ToUnicode(bVal, bVal2);
	}
	else
		return bVal;
}



CTextOutputStream::CTextOutputStream(IOutputStream * pOutputStream) :
m_pOutputStream(pOutputStream)
{
}

CTextOutputStream::~CTextOutputStream()
{
}

bool CTextOutputStream::CanWrite() const
{
	if(m_pOutputStream)
		return m_pOutputStream->CanWrite();
	else
		return false;
}

int_x CTextOutputStream::WriteAviliable()
{
	if(m_pOutputStream)
		return m_pOutputStream->WriteAviliable();
	else
		return 0;
}

void CTextOutputStream::WriteByte(byte_t byte)
{
	if(m_pOutputStream)
		return m_pOutputStream->WriteByte(byte);
}

void CTextOutputStream::Write(const void * pData, int_x iLength)
{
	if(m_pOutputStream)
		return m_pOutputStream->Write(pData, iLength);
}

void CTextOutputStream::Flush()
{
	if(m_pOutputStream)
		m_pOutputStream->Flush();
}

bool CTextOutputStream::SeekSurpport(StreamSeekE seek) const
{
	if(m_pOutputStream)
		return m_pOutputStream->SeekSurpport(seek);
	else
		return false;
}

int_x CTextOutputStream::Seek(StreamSeekE seek, int_x iSeek)
{
	if(m_pOutputStream)
		return m_pOutputStream->Seek(seek, iSeek);
	else
		return 0;
}

void CTextOutputStream::WriteText(const char_16 * pText, int_x iLength, LineTagE eLineTag)
{
	if(iLength < 0)
		iLength = textlen(pText);
	m_pOutputStream->Write(pText, iLength * 2);
	WriteLineTag(eLineTag);
}

void CTextOutputStream::WriteText(const textw & text, LineTagE eLineTag)
{
	WriteText(text, text.length(), eLineTag);
}

void CTextOutputStream::WriteLineTag(LineTagE eLineTag)
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

void CTextOutputStream::WriteFormat(const char_16 * szFormat, ...)
{
	va_list pArgs = nullptr;
	va_start(pArgs, szFormat);
	m_bufferW.format_args(szFormat, pArgs);
	va_end(pArgs);
	WriteText(m_bufferW, LineTagNone);
}

VENUS_END
