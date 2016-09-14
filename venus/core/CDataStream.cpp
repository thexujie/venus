#include "stdafx.h"
#include "CDataStream.h"

VENUS_BEG

CDataInputStream::CDataInputStream()
	:m_pInputStream(0), m_endian(StreamEndianSmall)
{

}
CDataInputStream::CDataInputStream(IInputStream * m_pInputStream)
	:m_pInputStream(m_pInputStream), m_endian(StreamEndianSmall)
{

}

CDataInputStream::CDataInputStream(IInputStream * m_pInputStream, StreamEndianE endian)
	:m_pInputStream(m_pInputStream), m_endian(endian)
{

}
CDataInputStream::~CDataInputStream()
{
	m_pInputStream = 0;
}
bool CDataInputStream::CanRead() const
{
	ConfirmInputStream();
	return m_pInputStream->CanRead();
}
int_x CDataInputStream::ReadAviliable()
{
	ConfirmInputStream();
	return m_pInputStream->ReadAviliable();
}

byte_t CDataInputStream::ReadByte()
{
	ConfirmInputStream();
	return m_pInputStream->ReadByte();
}

int_x CDataInputStream::Read(void * pBytes, int_x iLength)
{
	if(m_pInputStream)
		return m_pInputStream->Read(pBytes, iLength);
	else
		return 0;
}

bool CDataInputStream::SeekSurpport(StreamSeekE seek) const
{
	ConfirmInputStream();
	return m_pInputStream->SeekSurpport(seek);
}
int_x CDataInputStream::Seek(StreamSeekE seek, int_x iSeek)
{
	ConfirmInputStream();
	return m_pInputStream->Seek(seek, iSeek);
}

void CDataInputStream::SetDataInputEndian(StreamEndianE endian)
{
	ConfirmInputStream();
	m_endian = endian;
}
StreamEndianE CDataInputStream::GetDataInputEndian() const
{
	ConfirmInputStream();
	return m_endian;
}

int_8 CDataInputStream::ReadInt8()
{
	ConfirmAvilibale(1);
	Read8();
	return m_adapter.i8Val;
}
void CDataInputStream::ReadInt8Array(char_8 * pi8Array, int_x iLength)
{
	ConfirmAvilibale(iLength);
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		Read8();
		pi8Array[cnt] = m_adapter.i8Val;
	}
}
uint_8 CDataInputStream::ReadUInt8()
{
	ConfirmAvilibale(1);
	Read8();
	return m_adapter.ui8Val;
}
void CDataInputStream::ReadUInt8Array(uint_8 * pui8Array, int_x iLength)
{
	ConfirmAvilibale(iLength);
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		Read8();
		pui8Array[cnt] = m_adapter.ui8Val;
	}
}

int_16 CDataInputStream::ReadInt16()
{
	ConfirmAvilibale(2);
	Read16();
	return m_adapter.i16Val;
}

void CDataInputStream::ReadInt16Array(int_16 * pi16Array, int_x iLength)
{
	ConfirmAvilibale(iLength << 1);
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		Read16();
		pi16Array[cnt] = m_adapter.i16Val;
	}
}
uint_16 CDataInputStream::ReadUInt16()
{
	ConfirmAvilibale(2);
	Read16();
	return m_adapter.ui16Val;
}

void CDataInputStream::ReadUInt16Array(uint_16 * pui16Array, int_x iLength)
{
	ConfirmAvilibale(iLength << 1);
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		Read16();
		pui16Array[cnt] = m_adapter.ui16Val;
	}
}

int_32 CDataInputStream::ReadInt32()
{
	ConfirmAvilibale(4);
	Read32();
	return m_adapter.i32Val;
}
void CDataInputStream::ReadInt32Array(int_32 * pi32Array, int_x iLength)
{
	ConfirmAvilibale(iLength << 2);
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		Read32();
		pi32Array[cnt] = m_adapter.i32Val;
	}
}

uint_32 CDataInputStream::ReadUInt32()
{
	ConfirmAvilibale(4);
	Read32();
	return m_adapter.ui32Val;
}
void CDataInputStream::ReadUInt32Array(uint_32 * pui32Array, int_x iLength)
{
	ConfirmAvilibale(iLength << 2);
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		Read32();
		pui32Array[cnt] = m_adapter.ui32Val;
	}
}

IInputStream * CDataInputStream::GetInputStream()
{
	return m_pInputStream;
}
IInputStream * CDataInputStream::ResetInputStream(IInputStream * pNewInputStream)
{
	IInputStream * pOldInputStream  = m_pInputStream;
	m_pInputStream = pNewInputStream;
	return pOldInputStream;
}

// ----------------------------------- String
int_x CDataInputStream::ReadText(char_16 * pText, int_x iSize/* = IX_MAX*/)
{
	ConfirmInputStream();
	if(!pText || !iSize)
		throw exp_illegal_argument(_T("参数错误！"));

	int_x iRead = 0;
	int_x iLen = iSize;
	char_16 cTemp = 0;

	do
	{
		Read16();
		cTemp = m_adapter.c16Val;

		if(iSize)
		{
			pText[iRead++] = cTemp;
			--iSize;
		}
	}
	while(cTemp);
	pText[iRead] = 0;
	return iRead;
}

int_x CDataInputStream::ReadText(char_8 * pText, int_x iSize/* = IX_MAX*/)
{
	ConfirmInputStream();
	if(!pText || !iSize)
		throw exp_illegal_argument(_T("参数错误！"));

	int_x iRead = 0;
	int_x iLen = iSize;
	char_8 cTemp = 0;

	do
	{
		Read8();
		cTemp = m_adapter.c8Val;

		if(iSize)
		{
			pText[iRead++] = cTemp;
			--iSize;
		}
	}
	while(cTemp);
	pText[iRead] = 0;
	return iRead;
}

int_x CDataInputStream:: ReadLine(char_16 * pText, int_x iSize/* = IX_MAX*/)
{
	ConfirmInputStream();
	if(!pText || !iSize)
		throw exp_illegal_argument(_T("参数错误！"));

	int_x iRead = 0;
	int_x iLen = iSize;
	char_16 cTemp = 0;
	bool bReading = true;
	do
	{
		if(m_pInputStream->ReadAviliable() > 1)
		{
			Read16();

			cTemp = m_adapter.c16Val;

			switch(cTemp)
			{
			case L'\r':
				break;
			case L'\n':
				bReading = false;
				break;
			default:
				if(iSize)
				{
					pText[iRead++] = cTemp;
					--iSize;
				}
				break;
			}
		}
		else
			bReading = false;
	}
	while(bReading);

	pText[iRead] = 0;
	return iRead;
}

int_x CDataInputStream::ReadLine(char_8 * pText, int_x iSize/* = IX_MAX*/)
{
	ConfirmInputStream();
	if(!pText || !iSize)
		throw exp_illegal_argument(_T("参数错误！"));

	int_x iRead = 0;
	int_x iLen = iSize;
	char_8 cTemp = 0;
	bool bReading = true;
	do
	{
		if(m_pInputStream->ReadAviliable() > 0)
		{
			Read8();

			cTemp = m_adapter.c8Val;

			switch(cTemp)
			{
			case '\r':
				break;
			case '\n':
				bReading = false;
				break;
			default:
				if(iSize)
				{
					pText[iRead++] = cTemp;
					--iSize;
				}
				break;
			}
		}
		else
			bReading = false;
	}
	while(bReading);

	pText[iRead] = 0;
	return iRead;
}
int_x CDataInputStream::CSharpReadText(char_16 * pText, int_x iLength/* = IX_MAX*/)
{
	int_x iLen = 0;
	int_x iLenTmp = ReadInt8();
	if(iLenTmp >= 0)
		iLen = iLenTmp;
	else
	{
		iLen = iLenTmp + 256;
		int_x iNumber = 1 << 7;

		while(true)
		{
			iLenTmp = ReadInt8();
			if(iLenTmp > 0)
				break;
			iLen += (iLenTmp + 255) * iNumber;
			iNumber <<= 7;
		}
		iLen += (iLenTmp - 1) * iNumber;
	}
	iLen >>= 1;

	char_16 ch = 0;
	char_16 * pTemp = pText;
	int_x iRead = 0;
	while(iLen-- && --iLength)
	{
		ch = ReadUInt16();
		if(ch)
			pTemp[iRead++] = ch;
		else
			break;
	}
	pTemp[iRead++] = 0;
	return iRead;
}

int_x CDataInputStream::AS3ReadText(char_16 * pText, int_x iLength/* = IX_MAX*/)
{
	int_x iLen = ReadInt32() >> 1; 
	char_16 * pTemp = pText;
	char_16 ch = 0;
	int_x iRead = 0;
	while(iLen-- && --iLength)
	{
		ch = ReadUInt16();
		if(ch)
			pTemp[iRead++] = ch;
		else
			break;
	}
	pTemp[iRead++] = 0;
	return iRead;
}

void CDataInputStream::ConfirmInputStream() const
{
	if(!m_pInputStream)
		throw exp_null_pointer(_T("无效的输入流！"));
	if(!m_pInputStream->CanRead())
		throw exp_illegal_operation(_T("流不支持输入！"));
}

void CDataInputStream::ConfirmAvilibale(int_x iLength) const
{
	if(!m_pInputStream)
		throw exp_null_pointer(_T("无效的输入流！"));
	if(!m_pInputStream->CanRead())
		throw exp_illegal_operation(_T("流不支持输入！"));
	if(m_pInputStream->ReadAviliable() < iLength)
		throw exp_end_of_stream(_T("剩余字节数不足！"));
}
void CDataInputStream::Read8()
{
	m_adapter.a = m_pInputStream->ReadByte();
}

void CDataInputStream::Read16()
{
	if(m_endian == StreamEndianSmall)
	{
		m_adapter.a = m_pInputStream->ReadByte();
		m_adapter.b = m_pInputStream->ReadByte();
	}
	else
	{
		m_adapter.b = m_pInputStream->ReadByte();
		m_adapter.a = m_pInputStream->ReadByte();
	}
}
void CDataInputStream::Read32()
{
	if(m_endian == StreamEndianSmall)
	{
		m_adapter.a = m_pInputStream->ReadByte();
		m_adapter.b = m_pInputStream->ReadByte();
		m_adapter.c = m_pInputStream->ReadByte();
		m_adapter.d = m_pInputStream->ReadByte();
	}
	else
	{
		m_adapter.d = m_pInputStream->ReadByte();
		m_adapter.c = m_pInputStream->ReadByte();
		m_adapter.b = m_pInputStream->ReadByte();
		m_adapter.a = m_pInputStream->ReadByte();
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////////////////////// 



CDataOutputStream::CDataOutputStream()
	: m_pOutputStream(0), m_endian(StreamEndianSmall)
{

}
CDataOutputStream::CDataOutputStream(IOutputStream * pOutputStream)
	: m_pOutputStream(pOutputStream), m_endian(StreamEndianSmall)
{

}
bool CDataOutputStream::CanWrite() const
{
	if(!m_pOutputStream)
		throw exp_null_pointer(_T("无效的输出流！"));
	return m_pOutputStream->CanWrite();
}
CDataOutputStream::CDataOutputStream(IOutputStream * pOutputStream, StreamEndianE endian)
	: m_pOutputStream(pOutputStream), m_endian(endian)
{

}
CDataOutputStream::~CDataOutputStream()
{
	m_pOutputStream = 0;
}
int_x CDataOutputStream::WriteAviliable()
{
	ConfirmOutputStream();
	return m_pOutputStream->WriteAviliable();
}

void CDataOutputStream::WriteByte(byte_t byte)
{
	ConfirmOutputStream();
	m_pOutputStream->WriteByte(byte);
}

void CDataOutputStream::Write(const void * pBytes, int_x iLength)
{
	ConfirmWriteAvilibale(iLength);
	m_pOutputStream->Write(pBytes, iLength);
}

void CDataOutputStream::Flush()
{
	ConfirmOutputStream();
	m_pOutputStream->Flush();
}

bool CDataOutputStream::SeekSurpport(StreamSeekE seek) const
{
	ConfirmOutputStream();
	return m_pOutputStream->SeekSurpport(seek);
}
int_x CDataOutputStream::Seek(StreamSeekE seek, int_x iSeek)
{
	ConfirmOutputStream();
	return m_pOutputStream->Seek(seek, iSeek);
}
void CDataOutputStream::WriteInt8(int_8 i8)
{
	ConfirmWriteAvilibale(1);
	m_adapter.i8Val = i8;
	Write8();
}
void CDataOutputStream::WriteInt8Array(const int_8 * pi8Array, int_x iLength)
{
	ConfirmWriteAvilibale(iLength);
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		m_adapter.i8Val = pi8Array[cnt];
		Write8();
	}
}
void CDataOutputStream::WriteUInt8(uint_8 ui8)
{
	ConfirmWriteAvilibale(1);
	m_adapter.ui8Val = ui8;
	Write8();
}
void CDataOutputStream::WriteUInt8Array(const uint_8 * pui8Array, int_x iLength)
{
	ConfirmWriteAvilibale(iLength);
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		m_adapter.ui8Val = pui8Array[cnt];
		Write8();
	}
}

void CDataOutputStream::WriteInt16(int_16 i16)
{
	ConfirmWriteAvilibale(2);
	m_adapter.i16Val = i16;
	Write16();
}
void CDataOutputStream::WriteInt16Array(const int_16 * pi16Array, int_x iLength)
{
	ConfirmWriteAvilibale(iLength << 1);
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		m_adapter.i16Val = pi16Array[cnt];
		Write16();
	}
}
void CDataOutputStream::WriteUInt16(uint_16 ui16)
{
	ConfirmWriteAvilibale(2);
	m_adapter.ui16Val = ui16;
	Write16();
}
void CDataOutputStream::WriteUInt16Array(const uint_16 * pui16Array, int_x iLength)
{
	ConfirmWriteAvilibale(iLength << 1);
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		m_adapter.ui16Val = pui16Array[cnt];
		Write16();
	}
}

void CDataOutputStream::WriteInt32(int_32 i32)
{
	ConfirmWriteAvilibale(4);
	m_adapter.i32Val = i32;
	Write32();
}
void CDataOutputStream::WriteInt32Array(const int_32 * pi32Array, int_x iLength)
{
	ConfirmWriteAvilibale(iLength << 2);
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		m_adapter.i32Val = pi32Array[cnt];
		Write32();
	}
}
void CDataOutputStream::WriteUInt32(uint_32 ui32)
{
	ConfirmWriteAvilibale(4);
	m_adapter.ui32Val = ui32;
	Write32();
}
void CDataOutputStream::WriteUInt32Array(const uint_32 * pui32Array, int_x iLength)
{
	ConfirmWriteAvilibale(iLength << 2);
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		m_adapter.ui32Val = pui32Array[cnt];
		Write32();
	}
}

void CDataOutputStream::WriteFloat32(float_32 f32)
{
	ConfirmWriteAvilibale(4);
	m_adapter.f32Val = f32;
	Write32();
}

void CDataOutputStream::WriteFloatArray(const float_32 * pf32, int_x iLength)
{
	ConfirmWriteAvilibale(iLength << 2);
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		m_adapter.f32Val = pf32[cnt];
		Write32();
	}
}

void CDataOutputStream::WriteText(const char_16 * pText, int_x iLength)
{
	ConfirmOutputStream();
	if(iLength < 0)
		iLength = textlen(pText);
	WriteInt16Array((const int_16 *)pText, iLength + 1);
}
void CDataOutputStream::WriteText(const char_8 * pText, int_x iLength)
{
	ConfirmOutputStream();
	if(iLength < 0)
		iLength = textlen(pText);
	WriteInt8Array((const int_8 *)pText, iLength);
	WriteInt8(0);
}

void CDataOutputStream::WriteString(const char_16 * pText, int_x iLength)
{
	ConfirmOutputStream();
	if(iLength < 0)
		iLength = textlen(pText);
	WriteInt16Array((const int_16 *)pText, iLength);
}

void CDataOutputStream::WriteString(const char_8 * pText, int_x iLength)
{
	ConfirmOutputStream();
	if(iLength < 0)
		iLength = textlen(pText);
	WriteInt8Array((const int_8 *)pText, iLength);
}

void CDataOutputStream::CSharpWriteText(const char_16 * pText, int_x iLength)
{
	ConfirmOutputStream();
	if(iLength < 0)
		iLength = textlen(pText);

	int_x iLen = iLength;
	iLen <<= 1;
	while(iLen >= 0x80)
	{
		WriteInt8((int_8)((iLen % 0x80) - 0x80));
		iLen >>= 7;
	}
	WriteInt8((int_8)iLen);
	WriteUInt16Array((const uint_16 *)pText, iLength);
}
void CDataOutputStream::AS3WriteText(const char_16 * pText, int_x iLength)
{
	ConfirmOutputStream();
	if(iLength < 0)
		iLength = textlen(pText);
	WriteInt32((int_32)iLength);
	WriteUInt16Array((const uint_16 *)pText, iLength);
}

IOutputStream * CDataOutputStream::GetOutputStream()
{
	return m_pOutputStream;
}
IOutputStream * CDataOutputStream::SetOutputStream(IOutputStream * pNewOutputStream)
{
	IOutputStream * pOldOutputStream = m_pOutputStream;
	m_pOutputStream = pNewOutputStream;
	return pOldOutputStream;
}
void CDataOutputStream::ConfirmOutputStream() const
{
	if(!m_pOutputStream)
		throw exp_null_pointer(_T("无效的输出流！"));
	if(!m_pOutputStream->CanWrite())
		throw exp_illegal_operation(_T("流不支持输出！"));
}
void CDataOutputStream::ConfirmWriteAvilibale(int_x iLength) const
{
	if(!m_pOutputStream)
		throw exp_null_pointer(_T("无效的输出流！"));
	if(!m_pOutputStream->CanWrite())
		throw exp_illegal_operation(_T("流不支持输出！"));
	if(m_pOutputStream->WriteAviliable() < iLength)
		throw exp_null_pointer(_T("剩余字节数不足！"));
}

void CDataOutputStream::Write8()
{
	m_pOutputStream->WriteByte(m_adapter.bVal);
}
void CDataOutputStream::Write16()
{
	if(m_endian == StreamEndianSmall)
	{
		m_pOutputStream->WriteByte(m_adapter.a);
		m_pOutputStream->WriteByte(m_adapter.b);
	}
	else
	{
		m_pOutputStream->WriteByte(m_adapter.b);
		m_pOutputStream->WriteByte(m_adapter.a);
	}
}
void CDataOutputStream::Write32()
{
	if(m_endian == StreamEndianSmall)
	{
		m_pOutputStream->WriteByte(m_adapter.a);
		m_pOutputStream->WriteByte(m_adapter.b);
		m_pOutputStream->WriteByte(m_adapter.c);
		m_pOutputStream->WriteByte(m_adapter.d);
	}
	else
	{
		m_pOutputStream->WriteByte(m_adapter.d);
		m_pOutputStream->WriteByte(m_adapter.c);
		m_pOutputStream->WriteByte(m_adapter.b);
		m_pOutputStream->WriteByte(m_adapter.a);
	}
}

VENUS_END
