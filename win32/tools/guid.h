#pragma once

#include <guiddef.h>

VENUS_BEG

namespace guid_helper
{
	#pragma pack(push, 1)
	// E357FCCD-A995-4576-B01F-234630154E96\0
	struct guid_char37
	{
		char_16 data1[2][2][2];
		char_16 _s2;
		char_16 data2[2][2];
		char_16 _s3;
		char_16 data3[2][2];
		char_16 _s4;
		char_16 data4[2][2];
		char_16 _s5;
		char_16 data5[6][2];
	};

	// {E357FCCD-A995-4576-B01F-234630154E96}\0
	struct guid_char39
	{
		char_16 _s1;
		guid_char37 text_37;
		char_16 _s6;
	};
	#pragma pack(pop)

#pragma region ch_to_xx
	inline uint_8 ch_to_uint_4(char_16 ch)
	{
		if('A' <= ch && ch <= 'F')
			return 10 + ch - 'A';
		else if('a' <= ch && ch <= 'f')
			return 10 + ch - 'a';
		else if('0' <= ch && ch <= '9')
			return ch - '0';
		else
			return 0;
	}

	inline uint_8 text_to_uint_8(const char_16(&text)[2])
	{
		union
		{
			uint_8 value;
			struct
			{
				uint_8 high : 4;
				uint_8 low : 4;
			};
		};

		low = ch_to_uint_4(text[0]);
		high = ch_to_uint_4(text[1]);
		return value;
	}

	inline uint_16 text_to_uint_16(const char_16(&text)[2][2])
	{
		union
		{
			uint_16 value;
			struct
			{
				uint_8 high;
				uint_8 low;
			};
		};
		low = text_to_uint_8(text[0]);
		high = text_to_uint_8(text[1]);
		return value;
	}

	inline uint_32 text_to_uint_32(const char_16(&text)[2][2][2])
	{
		union
		{
			uint_32 value;
			struct
			{
				uint_16 high;
				uint_16 low;
			};
		};
		low = text_to_uint_16(text[0]);
		high = text_to_uint_16(text[1]);
		return value;
	}
#pragma endregion

#pragma region ch_from_xx

	inline char_16 ch_from_uint_4(uint_8 _value, bool upper = false)
	{
		if(0 <= _value && _value <= 9)
			return '0' + _value;
		else if(_value < 16)
			return (upper ? 'A' : 'a') + (_value - 10);
		else
			return 0;
	}

	inline void text_from_uint_8(uint_8 _value, char_16(&text)[2], bool upper = false)
	{
		union
		{
			uint_8 value;
			struct
			{
				uint_8 high : 4;
				uint_8 low : 4;
			};
		};

		value = _value;
		text[0] = ch_from_uint_4(low, upper);
		text[1] = ch_from_uint_4(high, upper);
	}

	inline void text_from_uint_16(uint_16 _value, char_16(&text)[2][2], bool upper = false)
	{
		union
		{
			uint_16 value;
			struct
			{
				uint_8 high;
				uint_8 low;
			};
		};
		value = _value;
		text_from_uint_8(low, text[0], upper);
		text_from_uint_8(high, text[1], upper);
	}

	inline void text_from_uint_32(uint_32 _value, char_16(&text)[2][2][2], bool upper = false)
	{
		union
		{
			uint_32 value;
			struct
			{
				uint_16 high;
				uint_16 low;
			};
		};
		value = _value;
		text_from_uint_16(low, text[0], upper);
		text_from_uint_16(high, text[1], upper);
	}
#pragma endregion // ch_from_xx

}

struct guid_t : public GUID
{
	guid_t() { buffclr(*this); }
	guid_t(const GUID & guid):GUID(guid) { }

	// {0x34256d4b, 0xe67f, 0x4e05, {0x93, 0x60, 0xc, 0x86, 0x3a, 0x1c, 0x42, 0xc1}}
	guid_t(uint_32 data1, uint_16 data2, uint_16 data3, const uint_8 (&data4)[8])
	{
		Data1 = data1;
		Data2 = data2;
		Data3 = data3;
		buffcpy8(Data4, 8, data4, 8);
	}

	// {0x34256d4b, 0xe67f, 0x4e05, 0x93, 0x60, 0xc, 0x86, 0x3a, 0x1c, 0x42, 0xc1}
	guid_t(uint_32 data1, uint_16 data2, uint_16 data3,
		uint_8 data40, uint_8 data41, uint_8 data42, uint_8 data43,
		uint_8 data44, uint_8 data45, uint_8 data46, uint_8 data47)
	{
		Data1 = data1;
		Data2 = data2;
		Data3 = data3;
		Data4[0] = data40; Data4[1] = data41; Data4[2] = data42; Data4[3] = data43;
		Data4[4] = data44; Data4[5] = data45; Data4[6] = data46; Data4[7] = data47;
	}


	// 34256D4B-E67F-4E05-9360-0C863A1C42C1
	guid_t(const char_16(&szGuid)[37])
	{
		from_char37(szGuid);
	}

	// {34256D4B-E67F-4E05-9360-0C863A1C42C1}
	guid_t(const char_16(&szGuid)[39])
	{
		from_char39(szGuid);
	}

	guid_t & from_char37(const char_16 szGuid[37])
	{
		const guid_helper::guid_char37 & text = *(const guid_helper::guid_char37 *)(&szGuid[0]);
		Data1 = guid_helper::text_to_uint_32(text.data1);
		Data2 = guid_helper::text_to_uint_16(text.data2);
		Data3 = guid_helper::text_to_uint_16(text.data3);
		Data4[0] = guid_helper::text_to_uint_8(text.data4[0]);
		Data4[1] = guid_helper::text_to_uint_8(text.data4[1]);
		for(int_x cnt = 0; cnt < 6; ++cnt)
			Data4[2 + cnt] = guid_helper::text_to_uint_8(text.data5[cnt]);
		return *this;
	}

	guid_t & from_char39(const char_16 szGuid[39])
	{
		return from_char37(szGuid + 1);
	}

	void to_char37(char_16 szGuid[37], bool bUpper = false) const
	{
		guid_helper::guid_char37 & text = *(guid_helper::guid_char37 *)(&szGuid[0]);
		guid_helper::text_from_uint_32(Data1, text.data1, bUpper);
		guid_helper::text_from_uint_16(Data2, text.data2, bUpper);
		guid_helper::text_from_uint_16(Data3, text.data3, bUpper);
		guid_helper::text_from_uint_8(Data4[0], text.data4[0], bUpper);
		guid_helper::text_from_uint_8(Data4[1], text.data4[1], bUpper);
		for(int_x cnt = 0; cnt < 6; ++cnt)
			guid_helper::text_from_uint_8(Data4[2 + cnt], text.data5[cnt], bUpper);
		text._s2 = text._s3 = text._s4 = text._s5 = L'-';
		szGuid[36] = 0;
	}

	void to_char39(char_16 szGuid[39], bool bUpper = false) const
	{
		guid_helper::guid_char39 & text = *(guid_helper::guid_char39 *)(&szGuid[0]);
		to_char37(szGuid + 1, bUpper);
		text._s1 = L'{';
		text._s6 = L'}';
		szGuid[38] = 0;
	}
};

VENUS_END
