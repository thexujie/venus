#pragma once

#include "IStream.h"

VENUS_BEG

class CORE_API CBinaryReader : public IInputStream, public IDataStream
{
public:
	CBinaryReader();
	CBinaryReader(IInputStream * m_pInputStream);
	CBinaryReader(IInputStream * m_pInputStream, EndianE endian);
	~CBinaryReader();
public:
	bool CanRead() const noexcept;
	int_x ReadAviliable() const noexcept;

	byte_t Read();
	int_x Read(void * pBytes, int_x iLength);

	bool CanSeek() const;
	int_x Seek(SeekE seek, int_x iSeek);

public :
	void SetEndian(EndianE endian);
	EndianE GetEndian() const;

	int_8 ReadInt8();
	void ReadInt8Array(char_8 * pi8Array, int_x iLength);
	uint_8 ReadUInt8();
	void ReadUInt8Array(uint_8 * pui8Array, int_x iLength);

	int_16 ReadInt16();
	void ReadInt16Array(int_16 * pi16Array, int_x iLength);
	uint_16 ReadUInt16();
	void ReadUInt16Array(uint_16 * pui16Array, int_x iLength);

	int_32 ReadInt32();
	void ReadInt32Array(int_32 * pi32Array, int_x iLength);
	uint_32 ReadUInt32();
	void ReadUInt32Array(uint_32 * pui32Array, int_x iLength);

	// ----------------------------------- String
	int_x ReadText(char_16 * pText, int_x iSize = IX_MAX);
	int_x ReadText(char_8 * pText, int_x iSize = IX_MAX);
	int_x CSharpReadText(char_16 * pText, int_x iSize = IX_MAX);
	int_x AS3ReadText(char_16 * pText, int_x iSize = IX_MAX);

protected:
	void _Ready() const;
	uint_8 _Read8();
	uint_16 _Read16();
	uint_32 _Read32();

protected:
	IInputStream * m_pInputStream;
	EndianE m_endian;
};

/************************************************************************/
/* 数据输出流
/************************************************************************/
class CORE_API CBinaryWriter : public IOutputStream, public IDataStream
{
public:
	CBinaryWriter();
	CBinaryWriter(IOutputStream * pOutputStream, EndianE endian = EndianSmall);
	~CBinaryWriter();

public :
	bool CanSeek() const;
	int_x Seek(SeekE seek, int_x iSeek);

	bool CanWrite() const noexcept;
	int_x WriteAviliable() const noexcept;
	void Write(byte_t byte);
	void Write(const void * pData, int_x iLength);

	void Flush();

public :
	void WriteInt8(int_8 i8);
	void WriteInt8Array(const int_8 * pi8Array, int_x iLength);
	void WriteUInt8(uint_8 ui8);
	void WriteUInt8Array(const uint_8 * pui8Array, int_x iLength);

	void WriteInt16(int_16 i16);
	void WriteInt16Array(const int_16 * pi16Array, int_x iLength);
	void WriteUInt16(uint_16 ui16);
	void WriteUInt16Array(const uint_16 * pui16Array, int_x iLength);

	void WriteInt32(int_32 i32);
	void WriteInt32Array(const int_32 * pi32Array, int_x iLength);
	void WriteUInt32(uint_32 ui32);
	void WriteUInt32Array(const uint_32 * pui32Array, int_x iLength);

	void WriteFloat32(float_32 f32);
	void WriteFloatArray(const float_32 * pf32, int_x iLength);

	void WriteString(const char_16 * pText, int_x iLength = -1);
	void WriteString(const char_8 * pText, int_x iLength = -1);

	void CSharpWriteText(const char_16 * pText, int_x iLength = -1);
	void AS3WriteText(const char_16 * pText, int_x iLength = -1);

protected:
	void _Ready() const;
	void _Write8(uint_8 val);
	void _Write16(uint_16 val);
	void _Write32(uint_32 val);
protected:
	IOutputStream * m_pOutputStream;
	EndianE m_endian;
};

VENUS_END
