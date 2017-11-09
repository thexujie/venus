#pragma once

VENUS_BEG

struct suid
{
	enum format_e
	{
		n, // "233b3c25d116488bbe5d1c1a5e9913da"
		d, // "233b3c25-d116-488b-be5d-1c1a5e9913da"
		b, // "{233b3c25-d116-488b-be5d-1c1a5e9913da}"
		p, // "(233b3c25-d116-488b-be5d-1c1a5e9913da)"
		x, // "{0x233b3c25, 0xd116, 0x488b, {0xbe, 0x5d, 0x1c, 0x1a, 0x5e, 0x99, 0x13, 0xda}}"
		i, // "0x233b3c25, 0xd116, 0x488b, 0xbe, 0x5d, 0x1c, 0x1a, 0x5e, 0x99, 0x13, 0xda"
		m, // "{0x233b3c25, 0xd116, 0x488b, 0xbe, 0x5d, 0x1c, 0x1a, 0x5e, 0x99, 0x13, 0xda}"

		N, // "233B3C25D116488BBE5D1C1A5E9913DA"
		D, // "233B3C25-D116-488B-BE5D-1C1A5E9913DA"
		B, // "{233B3C25-D116-488B-BE5D-1C1A5E9913DA}"
		P, // "(233B3C25-D116-488B-BE5D-1C1A5E9913DA)"
		X,  // "{0X233B3C25, 0XD116, 0X488B, {0XBE, 0X5D, 0X1C, 0X1A, 0X5E,   0X99, 0X13, 0XDA}}"
		I, // "0X233B3C25, 0XD116, 0X488B, 0XBE, 0X5D, 0X1C, 0X1A, 0X5E, 0X99, 0X13, 0XDA"
		M, // "{0X233B3C25, 0XD116, 0X488B, 0XBE, 0X5D, 0X1C, 0X1A, 0X5E, 0X99, 0X13, 0XDA}"
	};

	suid() { buffclr(*this); }
	suid(const suid & _guid) : suid(_guid.data1, _guid.data2, _guid.data3, _guid.data4) { }

	// {0x34256d4b, 0xe67f, 0x4e05, {0x93, 0x60, 0xc, 0x86, 0x3a, 0x1c, 0x42, 0xc1}}
	suid(uint_32 _data1, uint_16 _data2, uint_16 _data3, const uint_8(&_data4)[8])
	{
		data1 = _data1;
		data2 = _data2;
		data3 = _data3;
		buffcpy8(data4, 8, _data4, 8);
	}

	// {0x34256d4b, 0xe67f, 0x4e05, 0x93, 0x60, 0xc, 0x86, 0x3a, 0x1c, 0x42, 0xc1}
	suid(uint_32 _data1, uint_16 _data2, uint_16 _data3,
		uint_8 _data40, uint_8 _data41, uint_8 _data42, uint_8 _data43,
		uint_8 _data44, uint_8 _data45, uint_8 _data46, uint_8 _data47)
	{
		data1 = _data1;
		data2 = _data2;
		data3 = _data3;
		data4[0] = _data40; data4[1] = _data41; data4[2] = _data42; data4[3] = _data43;
		data4[4] = _data44; data4[5] = _data45; data4[6] = _data46; data4[7] = _data47;
	}

	suid(const char_16 * text, int_x length = -1)
	{
		buffclr(*this);
		if(length << 0)
			length = textlen(text);

		if(textch(text, length, L',') > 0)
		{
			//0x233b3c25, 0xd116, 0x488b, 0xbe, 0x5d, 0x1c, 0x1a, 0x5e, 0x99, 0x13, 0xda
			int index_last = 0;
			int index = 0;
			int curr = 0;
			while(index < length)
			{
				bool parse = false;
				char_16 ch = text[index];
				if(ch == '{' || ch == '(')
				{
					index_last = index + 1;
				}
				else if(ch == ',')
				{
					parse = true;
				}
				else if(index == length - 1)
				{
					parse = true;
					++index;
				}
				else {}

				if(parse && index > index_last)
				{
					switch(curr)
					{
					case 0:
						data1 = texttoi<char_16, uint_32>(text + index_last, index - index_last);
						break;
					case 1:
						data2 = texttoi<char_16, uint_16>(text + index_last, index - index_last);
						break;
					case 2:
						data3 = texttoi<char_16, uint_16>(text + index_last, index - index_last);
						break;
					case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10:
						data4[curr - 3] = texttoi<char_16, uint_8>(text + index_last, index - index_last);
						break;
					default:
						break;
					}
					++curr;
					index_last = index + 1;
				}
				++index;
			}

		}
		//×¢²á±íÐÎÊ½
		else if(textch(text, length, L'-') > 0)
		{
			//233B3C25-D116-488B-BE5D-1C1A5E9913DA
			int index_last = 0;
			int index_next = 0;
			int index = 0;
			int curr = 0;
			while(index < length)
			{
				bool parse = false;
				char_16 ch = text[index];
				if(ch == '{' || ch == '(')
				{
					index_last = index + 1;
				}
				else if(ch == '-')
				{
					parse = true;
					index_next = index + 1;
				}
				else if(index == length - 1)
				{
					parse = true;
					++index;
				}
				else if(curr > 2 && (index - index_last >= 2))
				{
					parse = true;
					index_next = index;
				}
				else {}

				if(parse && index > index_last)
				{
					switch(curr)
					{
					case 0:
						data1 = texttoi<char_16, uint_32>(text + index_last, index - index_last, 16);
						break;
					case 1:
						data2 = texttoi<char_16, uint_16>(text + index_last, index - index_last, 16);
						break;
					case 2:
						data3 = texttoi<char_16, uint_16>(text + index_last, index - index_last, 16);
						break;
					case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10:
						data4[curr - 3] = texttoi<char_16, uint_8>(text + index_last, index - index_last, 16);
						break;
					default:
						break;
					}
					++curr;
					index_last = index_next;
				}
				++index;
			}
		}
		else
		{
			//233B3C25D116488BBE5D1C1A5E9913DA
			if(length >= 32)
			{
				data1 = texttoi<char_16, uint_32>(text + 0, 8, 16);
				data2 = texttoi<char_16, uint_16>(text + 8, 4, 16);
				data3 = texttoi<char_16, uint_16>(text + 12, 4, 16);
				for(int cnt = 0; cnt < 8; ++cnt)
					data4[cnt] = texttoi<char_16, uint_8>(text + 16 + cnt * 2, 2, 16);
			}
		}
	}

	textw to_text(format_e format)
	{
		switch(format)
		{
		default:
		case n: return to_text(L"%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x");
		case d: return to_text(L"%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x");
		case b: return to_text(L"{%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x}");
		case p: return to_text(L"(%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x)");
		case x: return to_text(L"{0x%08x, 0x%04x, 0x%04x, {0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x}}");
		case i: return to_text(L"0x%08x, 0x%04x, 0x%04x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x");
		case m: return to_text(L"{0x%08x, 0x%04x, 0x%04x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x}");

		case N: return to_text(L"%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X");
		case D: return to_text(L"%08X-%04X-%04X-%02X%02X%02X%02X%02X%02X%02X%02X");
		case B: return to_text(L"{%08X-%04X-%04X-%02X%02X%02X%02X%02X%02X%02X%02X}");
		case P: return to_text(L"(%08X-%04X-%04X-%02X%02X%02X%02X%02X%02X%02X%02X)");
		case X: return to_text(L"{0x%08X, 0x%04X, 0x%04X, {0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X}}");
		case I: return to_text(L"0X%08X, 0X%04X, 0X%04X, 0X%02X, 0X%02X, 0X%02X, 0X%02X, 0X%02X, 0X%02X, 0X%02X, 0X%02X");
		case M: return to_text(L"{0X%08X, 0X%04X, 0X%04X, 0X%02X, 0X%02X, 0X%02X, 0X%02X, 0X%02X, 0X%02X, 0X%02X, 0X%02X}");
		}
	}

	textw to_text(const char_16 * format)
	{
		return textw().format(format,
			data1,
			data2, data3,
			data4[0], data4[1], data4[2], data4[3],
			data4[4], data4[5], data4[6], data4[7]);
	}

	bool operator == (const suid & another) const
	{
		return data1 == another.data1 &&
			data2 == another.data2 &&
			data3 == another.data3 &&
			data4[0] == another.data4[0] &&
			data4[1] == another.data4[1] &&
			data4[2] == another.data4[2] &&
			data4[3] == another.data4[3] &&
			data4[4] == another.data4[4] &&
			data4[5] == another.data4[5] &&
			data4[6] == another.data4[6] &&
			data4[7] == another.data4[7];
	}

	bool operator != (const suid & another) const
	{
		return !operator==(another);
	}

	uint_32 data1;
	uint_16 data2;
	uint_16 data3;
	uint_8 data4[8];
};

VENUS_END
