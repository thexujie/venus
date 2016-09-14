#pragma once

#include "IStream.h"

VENUS_BEG

class CORE_API CDataInputStream : public IDataInputStream
{
public:
	CDataInputStream();
	CDataInputStream(IInputStream * m_pInputStream);
	CDataInputStream(IInputStream * m_pInputStream, StreamEndianE endian);
	~CDataInputStream();
public:
	bool CanRead() const;
	int_x ReadAviliable();

	byte_t ReadByte();
	int_x Read(void * pBytes, int_x iLength);

	bool SeekSurpport(StreamSeekE seek) const;
	int_x Seek(StreamSeekE seek, int_x iSeek);

public :
	void SetDataInputEndian(StreamEndianE endian);
	StreamEndianE GetDataInputEndian() const;

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

	IInputStream * GetInputStream();
	IInputStream * ResetInputStream(IInputStream * pNewInputStream);

	// ----------------------------------- String
	int_x ReadText(char_16 * pText, int_x iSize = IX_MAX);
	int_x ReadText(char_8 * pText, int_x iSize = IX_MAX);
	int_x ReadLine(char_16 * pText, int_x iSize = IX_MAX); // \r \n
	int_x ReadLine(char_8 * pText, int_x iSize = IX_MAX); // \r \n
	int_x CSharpReadText(char_16 * pText, int_x iSize = IX_MAX);
	int_x AS3ReadText(char_16 * pText, int_x iSize = IX_MAX);

protected:
	void ConfirmInputStream() const;
	void ConfirmAvilibale(int_x iLength) const;

	void Read8();
	void Read16();
	void Read32();
protected:
	IInputStream * m_pInputStream;
	StreamEndianE m_endian;
	DataAdapter32 m_adapter;
};

/************************************************************************/
/* 数据输出流
/************************************************************************/
class CORE_API CDataOutputStream : public IDataOutputStream
{
public:
	CDataOutputStream();
	CDataOutputStream(IOutputStream * pOutputStream);
	CDataOutputStream(IOutputStream * pOutputStream, StreamEndianE endian);
	~CDataOutputStream();
public :
	bool CanWrite() const;

	int_x WriteAviliable();
	void WriteByte(byte_t byte);
	void Write(const void * pData, int_x iLength);

	void Flush();

	bool SeekSurpport(StreamSeekE seek) const;
	int_x Seek(StreamSeekE seek, int_x iSeek);
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

	//- Text会写入终止符，WriteString不会写入终止符。
	void WriteText(const char_16 * pText, int_x iLength = -1);
	void WriteText(const char_8 * pText, int_x iLength = -1);
	void WriteString(const char_16 * pText, int_x iLength = -1);
	void WriteString(const char_8 * pText, int_x iLength = -1);

	void CSharpWriteText(const char_16 * pText, int_x iLength = -1);
	void AS3WriteText(const char_16 * pText, int_x iLength = -1);

	IOutputStream * GetOutputStream();
	IOutputStream * SetOutputStream(IOutputStream * pNewOutputStream);
protected:
	void ConfirmOutputStream() const;
	void ConfirmWriteAvilibale(int_x iLength) const;

	void Write8();
	void Write16();
	void Write32();
protected:
	IOutputStream * m_pOutputStream;
	StreamEndianE m_endian;
	DataAdapter32 m_adapter;
};

VENUS_END
