#include "stdafx.h"
#include "CDataStream.h"

VENUS_BEG

CBinaryReader::CBinaryReader()
	:m_pInputStream(nullptr), m_endian(EndianSmall)
{

}
CBinaryReader::CBinaryReader(IInputStream * m_pInputStream)
	:m_pInputStream(m_pInputStream), m_endian(EndianSmall)
{ 
}

CBinaryReader::CBinaryReader(IInputStream * m_pInputStream, EndianE endian)
	:m_pInputStream(m_pInputStream), m_endian(endian)
{
}

CBinaryReader::~CBinaryReader()
{
	m_pInputStream = nullptr;
}

bool CBinaryReader::CanRead() const noexcept
{
	if(!m_pInputStream)
		return false;

	return m_pInputStream->CanRead();
}

int_x CBinaryReader::ReadAviliable() const noexcept
{
	if(!m_pInputStream)
		return 0;

	return m_pInputStream->ReadAviliable();
}

byte_t CBinaryReader::Read()
{
	_Ready();
	return m_pInputStream->Read();
}

int_x CBinaryReader::Read(void * pBytes, int_x iLength)
{
	_Ready();
	if(m_pInputStream)
		return m_pInputStream->Read(pBytes, iLength);
	else
		return 0;
}

bool CBinaryReader::CanSeek() const
{
	_Ready();
	return m_pInputStream->CanSeek();
}

int_x CBinaryReader::Seek(SeekE seek, int_x iSeek)
{
	_Ready();
	return m_pInputStream->Seek(seek, iSeek);
}

void CBinaryReader::SetEndian(EndianE endian)
{
	m_endian = endian;
}
EndianE CBinaryReader::GetEndian() const
{
	return m_endian;
}

int_8 CBinaryReader::ReadInt8()
{
	_Ready();
	return _Read8();
}

void CBinaryReader::ReadInt8Array(char_8 * pi8Array, int_x iLength)
{
	_Ready();
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		pi8Array[cnt] = _Read8();
	}
}
uint_8 CBinaryReader::ReadUInt8()
{
	_Ready();
	return _Read8();
}

void CBinaryReader::ReadUInt8Array(uint_8 * pui8Array, int_x iLength)
{
	_Ready();
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		pui8Array[cnt] = _Read8();
	}
}

int_16 CBinaryReader::ReadInt16()
{
	_Ready();
	return _Read16();
}

void CBinaryReader::ReadInt16Array(int_16 * pi16Array, int_x iLength)
{
	_Ready();
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		pi16Array[cnt] = _Read16();
	}
}
uint_16 CBinaryReader::ReadUInt16()
{
	_Ready();
	return _Read16();
}

void CBinaryReader::ReadUInt16Array(uint_16 * pui16Array, int_x iLength)
{
	_Ready();
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		pui16Array[cnt] = _Read16();
	}
}

int_32 CBinaryReader::ReadInt32()
{
	_Ready();
	return _Read32();
}
void CBinaryReader::ReadInt32Array(int_32 * pi32Array, int_x iLength)
{
	_Ready();
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		pi32Array[cnt] = _Read32();
	}
}

uint_32 CBinaryReader::ReadUInt32()
{
	_Ready();
	return _Read32();
}

void CBinaryReader::ReadUInt32Array(uint_32 * pui32Array, int_x iLength)
{
	_Ready();
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		pui32Array[cnt] = _Read32();
	}
}

// ----------------------------------- String
int_x CBinaryReader::ReadText(char_16 * pText, int_x iSize/* = IX_MAX*/)
{
	_Ready();

	if(!pText || !iSize)
		throw exp_illegal_argument();

	int_x iRead = 0;
	int_x iLen = iSize;
	char_16 cTemp = 0;

	do
	{
		cTemp = _Read16();

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

int_x CBinaryReader::ReadText(char_8 * pText, int_x iSize/* = IX_MAX*/)
{
	_Ready();

	if(!pText || iSize < 0)
		throw exp_illegal_argument();

	int_x iRead = 0;
	char_8 ch = 0;
	bool bReading = true;

	do
	{
		if(m_pInputStream->ReadAviliable() >= 1)
		{
			ch = _Read8();

			if(ch && --iSize)
			{
				pText[iRead++] = ch;
				--iSize;
			}
		}
		else
			bReading = false;
	}
	while(bReading && ch);

	pText[iRead] = 0;
	return iRead;
}

int_x CBinaryReader::CSharpReadText(char_16 * pText, int_x iSize/* = IX_MAX*/)
{
	int_x iLength = 0;
	int_x iLenTmp = ReadInt8();
	if(iLenTmp >= 0)
		iLength = iLenTmp;
	else
	{
		iLength = iLenTmp + 256;
		int_x iNumber = 1 << 7;

		while(true)
		{
			iLenTmp = _Read8();
			if(iLenTmp > 0)
				break;
			iLength += (iLenTmp + 255) * iNumber;
			iNumber <<= 7;
		}

		iLength += (iLenTmp - 1) * iNumber;
	}
	iLength >>= 1;

	char_16 ch = 0;
	char_16 * pTemp = pText;
	int_x iRead = 0;
	while(iLength-- && --iSize)
	{
		ch = _Read16();
		if(ch)
			pTemp[iRead++] = ch;
		else
			break;
	}
	pTemp[iRead++] = 0;
	return iRead;
}

int_x CBinaryReader::AS3ReadText(char_16 * pText, int_x iLength/* = IX_MAX*/)
{
	int_x iLen = ReadInt32() >> 1; 
	char_16 * pTemp = pText;
	char_16 ch = 0;
	int_x iRead = 0;
	while(iLen-- && --iLength)
	{
		ch = _Read16();
		if(ch)
			pTemp[iRead++] = ch;
		else
			break;
	}
	pTemp[iRead++] = 0;
	return iRead;
}

void CBinaryReader::_Ready() const
{
	if(!m_pInputStream)
		throw exp_nullptr();
	if(!m_pInputStream->CanRead())
		throw exp_illegal_operation();
}

uint_8 CBinaryReader::_Read8()
{
	return m_pInputStream->Read();
}

uint_16 CBinaryReader::_Read16()
{
	union
	{
		uint_16 val;
		struct
		{
			uint_8 aval;
			uint_8 bval;
		};
	};

	if(m_endian == EndianSmall)
	{
		aval = m_pInputStream->Read();
		bval = m_pInputStream->Read();
	}
	else
	{
		bval = m_pInputStream->Read();
		aval = m_pInputStream->Read();
	}
	return val;
}

uint_32 CBinaryReader::_Read32()
{
	union
	{
		uint_32 val;
		struct
		{
			uint_8 aval;
			uint_8 bval;
			uint_8 cval;
			uint_8 dval;
		};
	};

	if(m_endian == EndianSmall)
	{
		aval = m_pInputStream->Read();
		bval = m_pInputStream->Read();
		cval = m_pInputStream->Read();
		dval = m_pInputStream->Read();
	}
	else
	{
		dval = m_pInputStream->Read();
		cval = m_pInputStream->Read();
		bval = m_pInputStream->Read();
		aval = m_pInputStream->Read();
	}
	return val;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////////////////////// 


CBinaryWriter::CBinaryWriter()
	: m_pOutputStream(nullptr), m_endian(EndianSmall)
{

}

CBinaryWriter::CBinaryWriter(IOutputStream * pOutputStream, EndianE endian)
	: m_pOutputStream(pOutputStream), m_endian(endian)
{
}

CBinaryWriter::~CBinaryWriter()
{
}

bool CBinaryWriter::CanSeek() const
{
	_Ready();
	return m_pOutputStream->CanSeek();
}

int_x CBinaryWriter::Seek(SeekE seek, int_x iSeek)
{
	_Ready();
	return m_pOutputStream->Seek(seek, iSeek);
}

bool CBinaryWriter::CanWrite() const noexcept
{
	if(!m_pOutputStream)
		return false;

	return m_pOutputStream->CanWrite();
}

int_x CBinaryWriter::WriteAviliable() const noexcept
{
	if(!m_pOutputStream)
		return 0;

	return m_pOutputStream->WriteAviliable();
}

void CBinaryWriter::Write(byte_t byte)
{
	_Ready();
	m_pOutputStream->Write(byte);
}

void CBinaryWriter::Write(const void * pBytes, int_x iLength)
{
	_Ready();
	m_pOutputStream->Write(pBytes, iLength);
}

void CBinaryWriter::Flush()
{
	_Ready();
	m_pOutputStream->Flush();
}

void CBinaryWriter::WriteInt8(int_8 i8)
{
	_Ready();
	_Write8(i8);
}

void CBinaryWriter::WriteInt8Array(const int_8 * pi8Array, int_x iLength)
{
	_Ready();
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		_Write8(pi8Array[cnt]);
	}
}

void CBinaryWriter::WriteUInt8(uint_8 ui8)
{
	_Ready();
	_Write8(ui8);
}

void CBinaryWriter::WriteUInt8Array(const uint_8 * pui8Array, int_x iLength)
{
	_Ready();
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		_Write8(pui8Array[cnt]);
	}
}

void CBinaryWriter::WriteInt16(int_16 i16)
{
	_Ready();
	_Write16(i16);
}

void CBinaryWriter::WriteInt16Array(const int_16 * pi16Array, int_x iLength)
{
	_Ready();
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		_Write16(pi16Array[cnt]);
	}
}

void CBinaryWriter::WriteUInt16(uint_16 ui16)
{
	_Ready();
	_Write16(ui16);
}

void CBinaryWriter::WriteUInt16Array(const uint_16 * pui16Array, int_x iLength)
{
	_Ready();
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		_Write16(pui16Array[cnt]);
	}
}

void CBinaryWriter::WriteInt32(int_32 i32)
{
	_Ready();
	_Write32(i32);
}

void CBinaryWriter::WriteInt32Array(const int_32 * pi32Array, int_x iLength)
{
	_Ready();
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		_Write32(pi32Array[cnt]);
	}
}

void CBinaryWriter::WriteUInt32(uint_32 ui32)
{
	_Ready();
	_Write32(ui32);
}

void CBinaryWriter::WriteUInt32Array(const uint_32 * pui32Array, int_x iLength)
{
	_Ready();
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		_Write32(pui32Array[cnt]);
	}
}

void CBinaryWriter::WriteFloat32(float_32 f32)
{
	_Ready();
	_Write32(*reinterpret_cast<const uint_32 *>(&f32));
}

void CBinaryWriter::WriteFloatArray(const float_32 * pf32, int_x iLength)
{
	_Ready();
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		_Write32(*reinterpret_cast<const uint_32 *>(pf32 + cnt));
	}
}

void CBinaryWriter::WriteString(const char_16 * pText, int_x iLength)
{
	_Ready();
	if(iLength < 0)
		iLength = textlen(pText);
	WriteInt16Array(reinterpret_cast<const int_16 *>(pText), iLength);
	_Write16(0);
}

void CBinaryWriter::WriteString(const char_8 * pText, int_x iLength)
{
	_Ready();
	if(iLength < 0)
		iLength = textlen(pText);
	WriteInt8Array(reinterpret_cast<const int_8 *>(pText), iLength);
	_Write8(0);
}

void CBinaryWriter::CSharpWriteText(const char_16 * pText, int_x iLength)
{
	_Ready();
	if(iLength < 0)
		iLength = textlen(pText);

	int_x iLen = iLength;
	iLen <<= 1;
	while(iLen >= 0x80)
	{
		_Write8(static_cast<uint_8>((iLen % 0x80) - 0x80));
		iLen >>= 7;
	}
	WriteInt8(static_cast<uint_8>(iLen));
	WriteUInt16Array(reinterpret_cast<const uint_16 *>(pText), iLength);
}

void CBinaryWriter::AS3WriteText(const char_16 * pText, int_x iLength)
{
	_Ready();
	if(iLength < 0)
		iLength = textlen(pText);
	WriteInt32(static_cast<int_32>(iLength));
	WriteUInt16Array(reinterpret_cast<const uint_16 *>(pText), iLength);
}

void CBinaryWriter::_Ready() const
{
	if(!m_pOutputStream)
		throw exp_nullptr();
	if(!m_pOutputStream->CanWrite())
		throw exp_illegal_operation();
}

void CBinaryWriter::_Write8(uint_8 val)
{
	m_pOutputStream->Write(val);
}

void CBinaryWriter::_Write16(uint_16 val)
{
	union
	{
		uint_16 _val;
		struct
		{
			uint_8 aval;
			uint_8 bval;
		};
	};

	_val = val;

	if(m_endian == EndianSmall)
	{
		m_pOutputStream->Write(aval);
		m_pOutputStream->Write(bval);
	}
	else
	{
		m_pOutputStream->Write(bval);
		m_pOutputStream->Write(aval);
	}
}

void CBinaryWriter::_Write32(uint_32 val)
{
	union
	{
		uint_32 val;
		struct
		{
			uint_8 aval;
			uint_8 bval;
			uint_8 cval;
			uint_8 dval;
		};
	};
	if(m_endian == EndianSmall)
	{
		m_pOutputStream->Write(aval);
		m_pOutputStream->Write(bval);
		m_pOutputStream->Write(cval);
		m_pOutputStream->Write(dval);
	}
	else
	{
		m_pOutputStream->Write(dval);
		m_pOutputStream->Write(cval);
		m_pOutputStream->Write(bval);
		m_pOutputStream->Write(aval);
	}
}

VENUS_END
