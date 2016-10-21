#pragma once

VENUS_BEG

template<typename CharT>
class CORE_API text_data_t
{
public:
	int_x addref() { return ++m_ref; }
	int_x release()
	{
		verify(m_ref);
		int_x iRef = --m_ref;
		if(!m_ref)
			delete this;
		return iRef;
	}
	int_x ref() const { return m_ref; }

	// 构造空的文本数据
	text_data_t() : m_ref(1),m_buffer(nullptr), m_capability(0), m_size(0) {}
	text_data_t(const text_data_t & another) : m_ref(1)
	{
		int_x _capability = another.m_capability;
		m_buffer = new CharT[_capability];
		textcpy(m_buffer, _capability, another.m_buffer, another.m_size);
		m_size = another.m_size;
		m_capability = another.m_capability;
	}

	explicit text_data_t(int_x _capability) : m_ref(1)
	{
		if(_capability)
		{
			m_buffer = new CharT[_capability];
			m_buffer[0] = 0;
			m_size = 0;
			m_capability = _capability;
		}
		else
		{
			m_buffer = nullptr;
			m_size = 0;
			m_capability = 0;
		}
	}

	text_data_t(int_x _size, int_x _capability) : m_ref(1)
	{
		if(_size > 0)
		{
			if(_capability < _size + 1)
			{
				if(_size == _capability)
					_capability = _size + 1;
				else
					_capability = fit_capability(_size);
			}

			m_buffer = new CharT[_capability];
			m_buffer[_size] = 0;

			m_size = _size;
			m_capability = _capability;
		}
		else
		{
			if(_capability > 0)
			{
				m_buffer = new CharT[_capability];
				m_buffer[0] = 0;
			}
			else
			{
				m_buffer = nullptr;
			}
			m_size = 0;
			m_capability = 0;
		}
	}

	text_data_t(const CharT * _text, int_x _length = -1, int_x _capability = -1) : m_ref(1)
	{
		if(_length < 0)
			_length = textlen(_text);
		if(_length)
		{
			if(_capability < _length + 1)
				_capability = fit_capability(_length);

			m_buffer = new CharT[_capability];
			ensure(m_buffer);
			m_size = _length;
			m_capability = _capability;
			textcpy(m_buffer, m_capability, _text, _length);
		}
		else
		{
			m_buffer = nullptr;
			m_size = 0;
			m_capability = 0;
		}
	}

	~text_data_t()
	{
		if(m_buffer)
		{
			delete[] m_buffer;
			m_buffer = nullptr;
		}
		m_capability = 0;
		m_size = 0;
		m_ref = 0;
	}

	//! 保留缓冲区大小，使其不小于指定大小。
	void reserve(int_x _capability)
	{
		if(_capability > m_capability)
		{
			CharT * _buffer = new CharT[_capability];
			buffcpy(_buffer, m_buffer, m_size + 1);
			delete[] m_buffer;
			m_buffer = _buffer;
			m_capability = _capability;
		}
	}

	//! 强制重置缓冲区大小。
	void reallocate(int_x _capability)
	{
		CharT * _buffer = new CharT[_capability];
		int_x _size = textcpy(_buffer, _capability, m_buffer, m_size);
		delete[] m_buffer;
		m_buffer = _buffer;
		m_size = _size;
		m_capability = _capability;
	}

	//! 强制改变文本长度，缓冲区大小自增长。
	void resize(int_x _size)
	{
		if(m_size == _size)
			return;

		if(_size < m_capability)
		{
			m_buffer[_size] = 0;
			m_size = _size;
		}
		else
		{
			int_x _capability = fit_capability(_size);
			CharT * _buffer = new CharT[_capability];
			buffcpy(_buffer, _capability, m_buffer, m_size);
			_buffer[_size] = 0;

			delete[] m_buffer;
			m_buffer = _buffer;
			m_size = _size;
			m_capability = _capability;
		}
	}

	/// 重新设置字符串的内容，内存自动增长。
	void set(const CharT * _text, int_x _length = -1)
	{
		if(_length < 0)
			_length = textlen(_text);
		int_x _capability = _length + 1;
		if(_capability > m_capability)
		{
			CharT * _buffer = new CharT[_capability];
			if(!_buffer)
				throw exp_out_of_memory();

			delete[] m_buffer;
			m_buffer = _buffer;
			m_capability = _capability;
		}
		else {}

		textcpy(m_buffer, m_capability, _text, _length);
		m_size = _length;
	}

	void remove(int_x iIndex)
	{
		textremove(m_buffer, m_size, m_capability, iIndex);
		--m_size;
	}

	void remove(int_x iIndex, int_x iLength)
	{
		textremove(m_buffer, m_size, m_capability, iIndex, iLength);
		m_size -= iLength;
	}

	void insert(int_x iIndex, const CharT * pText, int_x iCount)
	{
		int_x iSize = fit_capability(m_size + iCount);
		if(iSize > m_capability)
		{
			CharT * pNewBuffer = new CharT[iSize];
			if(!pNewBuffer)
				throw exp_out_of_memory();
			buffcpy(pNewBuffer, iSize, m_buffer, iIndex);
			buffcpy(pNewBuffer + iIndex, iSize - iIndex, pText, iCount);
			textcpy(pNewBuffer + iIndex + iCount, iSize - iIndex - iCount, m_buffer + iIndex, m_size - iIndex);

			delete[] m_buffer;
			m_buffer = pNewBuffer;
			m_capability = iSize;
		}
		else
			textinsert(m_buffer, m_size, m_capability, iIndex, pText, iCount);
		m_size += iCount;
	}

	void insert(int_x iIndex, const CharT & ch)
	{
		int_x iSize = fit_capability(m_size + 1);
		if(iSize > m_capability)
		{
			CharT * pNewBuffer = new CharT[iSize];
			ensure(pNewBuffer);
			buffcpy(pNewBuffer, iSize, m_buffer, iIndex);
			pNewBuffer[iIndex] = ch;
			textcpy(pNewBuffer + iIndex + 1, iSize - iIndex - 1, m_buffer + iIndex, m_size - iIndex);

			delete[] m_buffer;
			m_buffer = pNewBuffer;
			m_capability = iSize;
		}
		else
			textinsert(m_buffer, m_size, m_capability, iIndex, ch);
		++m_size;
	}

	/// ------------------------------ Format
	void format(const CharT * szFormat, ...)
	{
		va_list pArgs = nullptr;
		va_start(pArgs, szFormat);
		format_args(szFormat, pArgs);
		va_end(pArgs);
	}
	void format_args(const CharT * szFormat, void * pArgs)
	{
		int_x _length = TextCalcFormatArgs(szFormat, pArgs);
		if(m_capability <= _length)
		{
			int_x _capability = fit_capability(_length);

			CharT * _buffer = new CharT[_capability];
			if(!_buffer)
				throw exp_out_of_memory();
			delete[] m_buffer;
			m_buffer = _buffer;
			m_capability = _capability;
		}
		textformat_args(m_buffer, m_capability, szFormat, pArgs);
		m_size = _length;
	}

	operator CharT * ()
	{
		return m_buffer;
	}
	operator const CharT * ()
	{
		return m_buffer;
	}

	static int_x fit_capability(int_x _size)
	{
		int_x _capability = 1;
		int_x _need = _size + 1;
		int_x rate = 0;
		while(_capability < _need)
		{
			++rate;
			_capability <<= rate;
		}
		verify(_capability >= _size);
		return _capability;
	}

	CharT * buffer() { return m_buffer; }
	const CharT * buffer() const { return m_buffer; }

	int_x size() const { return m_size; }
	int_x capability() const { return m_capability; }

	int_x m_ref;
	CharT * m_buffer;
	int_x m_capability;
	int_x m_size;
};


template<typename CharT, int_x CodePage>
class CORE_API text_base
{
public:
	typedef text_data_t<CharT> textdata_t;
public:
	text_base() : m_data(nullptr) {}
	text_base(const text_base & another)
	{
		m_data = another.m_data;
		if(m_data)
			m_data->addref();
	}

	text_base(const CharT * _text, int_x _length = -1)
	{
		m_data = new textdata_t(_text, _length, -1);
	}

	explicit text_base(int_x _length, int_x _capability)
	{
		m_data = new textdata_t(_length, _capability);
	}

	explicit text_base(int_x _capability)
	{
		m_data = new textdata_t(_capability);
	}

	text_base(textdata_t * _data)
	{
		m_data = _data;
	}

	~text_base()
	{
		if(m_data)
		{
			m_data->release();
			m_data = nullptr;
		}
	}

	text_base & set(const CharT * _text, int_x _length = -1)
	{
		if(_length < 0)
			_length = textlen(_text);

		if(!_length)
		{
			if(m_data)
			{
				if(m_data->ref() > 1)
				{
					m_data->release();
					m_data = nullptr;
				}
				else
					m_data->resize(0);
			}
			else {}
		}
		else if(!m_data)
			m_data = new textdata_t(_text, _length, -1);
		else if(m_data->ref() > 1)
		{
			m_data->release();
			m_data = new textdata_t(_text, _length, -1);
		}
		else
			m_data->set(_text, _length);
		return *this;
	}

	text_base & append(CharT ch)
	{
		insert(length(), ch);
		return *this;
	}

	text_base & append(const CharT * _text, int_x _length = -1)
	{
		insert(length(), _text, _length);
		return *this;
	}

	void insert(int_x _index, const CharT & _ch)
	{
		if(!m_data)
		{
			if(_index)
				throw exp_out_of_bound();

			CharT _text[2] = {_ch, 0};
			m_data = new textdata_t(_text, 1, -1);
		}
		else if(m_data->ref() > 1)
		{
			int_x capability = textdata_t::fit_capability(m_data->m_size + 1);
			textdata_t * _data = new textdata_t(capability);
			buffcpy(_data->m_buffer, m_data->m_buffer, _index);
			_data->m_buffer[_index] = _ch;
			buffcpy(_data->m_buffer + _index + 1, m_data->m_buffer + _index, m_data->m_size - _index);
			_data->m_size = m_data->m_size + 1;
			_data->m_buffer[_data->m_size] = 0;
			m_data->release();
			m_data = _data;
		}
		else
			m_data->insert(_index, _ch);
	}

	void remove(int_x _index)
	{
		int_x iLength = length();
		if(_index < 0 || _index >= iLength)
			throw exp_out_of_bound();

		if(m_data->ref() > 1)
		{
			int_x capability = m_data->m_capability;
			textdata_t * _data = new textdata_t(capability);

			CharT * pNewText = _data->m_buffer;
			buffcpy(_data->m_buffer, m_data->m_buffer, _index);
			buffcpy(_data->m_buffer + _index, m_data->m_buffer + _index + 1, m_data->m_size - _index - 1);

			_data->m_size = m_data->m_size - 1;
			_data->m_buffer[_data->m_size] = 0;
			m_data->release();
			m_data = _data;
		}
		else
			m_data->remove(_index);
	}

	void insert(int_x _index, const CharT * _text, int_x _length = -1)
	{
		if(_index < 0 || _index > length())
			throw exp_out_of_bound();
		if(_length < 0)
			_length = textlen(_text);

		if(!m_data)
			m_data = new textdata_t(_text, _length);
		else if(m_data->ref() > 1)
		{
			int_x capability = textdata_t::fit_capability(m_data->m_size + _length);
			textdata_t * _data = new textdata_t(capability);

			buffcpy(_data->m_buffer, m_data->m_buffer, _index);
			buffcpy(_data->m_buffer + _index, _text, _length);
			buffcpy(_data->m_buffer + _index + _length, m_data->m_buffer + _index, m_data->m_size - _index);

			_data->m_size = m_data->m_size + _length;
			_data->m_buffer[_data->m_size] = 0;

			m_data->release();
			m_data = _data;
		}
		else
		{
			m_data->insert(_index, _text, _length);
		}
	}

	void remove(int_x _index, int_x _length)
	{
		if(!_length)
			return;

		if(_length < 0)
			throw exp_illegal_argument();

		int_x iLen = length();
		if(_index < 0 || _index >= iLen || _index + _length > iLen)
			throw exp_out_of_bound();

		if(_length)
		{
			if(m_data->ref() > 1)
			{
				textdata_t * _data = new textdata_t(m_data->m_capability);
				buffcpy(_data->m_buffer, m_data->m_buffer, _index);
				buffcpy(_data->m_buffer + _index, m_data->m_buffer + _index + iLen, m_data->m_size - _index - _length);

				_data->m_size = m_data->m_size - _length;
				_data->m_buffer[_data->m_size] = 0;

				m_data->release();
				m_data = _data;
			}
			else
				m_data->remove(_index, _length);
		}
	}

	void clear()
	{
		if(m_data)
		{
			m_data->release();
			m_data = nullptr;
		}
	}

	void destroy()
	{
		if(m_data)
		{
			m_data->release();
			m_data = nullptr;
		}
	}

	bool equal(const CharT * szText, int_x iLength = -1, bool bCase = true) const
	{
		if(iLength < 0)
			iLength = textlen(szText);
		if(iLength != length())
			return false;
		if(!iLength)
			return true;

		return textequalex<CharT>(szText, iLength, m_data->buffer(), iLength, bCase);
	}

	bool tobool(bool defval = false) const
	{
		if(!length())
			return defval;
		return texttobool<CharT>(m_data->buffer(), m_data->size());
	}

	int_32 toi32(int_32 defval = 0, int_x radix = 0) const
	{
		if(!length())
			return defval;
		return texttoi<CharT, int_32>(m_data->buffer(), m_data->size(), radix);
	}

	int_64 toi64(int_64 defval = 0, int_x radix = 0) const
	{
		if(!length())
			return defval;
		return texttoi<CharT, int_64>(m_data->buffer(), m_data->size(), radix);
	}

	int_x toix(int_x defval = 0, int_x radix = 0) const
	{
		if(!length())
			return defval;
		return texttoi<CharT, int_x>(m_data->buffer(), m_data->size(), radix);
	}

	uint_32 toui32(uint_32 defval = 0, int_x radix = 0) const
	{
		if(!length())
			return defval;
		return texttoi<CharT, uint_32>(m_data->buffer(), m_data->size(), radix);
	}

	uint_64 toui64(uint_64 defval = 0, int_x radix = 0) const
	{
		if(!length())
			return defval;
		return texttoi<CharT, uint_64>(m_data->buffer(), m_data->size(), radix);
	}

	uint_x touix(uint_x defval = 0, int_x radix = 0) const
	{
		if(!length())
			return defval;
		return texttoi<CharT, uint_x>(m_data->buffer(), m_data->size(), radix);
	}

	float_32 tof32(float_32 defval = 0.0f) const
	{
		if(!length())
			return defval;
		return texttof<CharT, float_32>(m_data->buffer(), m_data->size());
	}

	float_64 tof64(float_64 defval = 0.0) const
	{
		if(!length())
			return defval;
		return texttof<CharT, float_64>(m_data->buffer(), m_data->size());
	}

#if defined _MSC_VER && (_MSC_VER < MSC_2015)
	template<typename CalcT, typename ...ParamT>
	auto convert(CalcT conv, ParamT ...params) const ->decltype(conv(*(text_base *)0, params...))
	{
		return conv(*this, params...);
	}
	template<typename CalcT, typename ...ParamT>
	auto totype(CalcT conv, ParamT ...params) const ->decltype(conv(*(text_base *)0, params...))
	{
		return conv(*this, params...);
	}
#else
	template<typename CalcT, typename ...ParamT>
	auto convert(CalcT conv, ParamT ...params) const
	{
		return conv(*this, params...);
	}

	template<typename CalcT, typename ...ParamT>
	auto totype(CalcT conv, ParamT ...params) const
	{
		return conv(buffer(), length(), params...);
	}
#endif

	text_base & format(const CharT * szFormat, ...)
	{
		va_list pArgs = nullptr;
		va_start(pArgs, szFormat);
		format_args(szFormat, pArgs);
		va_end(pArgs);
		return *this;
	}

	text_base & format_args(const CharT * szFormat, void * pArgs)
	{
		int_x _size = textformatargs_calc(szFormat, pArgs);
		if(!_size)
		{
			resize(0);
			return *this;
		}

		if(!m_data)
			m_data = new textdata_t(_size, -1);
		else if(m_data->ref() > 1)
		{
			textdata_t * _data = new textdata_t(_size, -1);
			m_data->release();
			m_data = _data;
		}
		else
		{
			m_data->resize(_size);
		}

		m_data->m_size = textformat_args(m_data->m_buffer, _size, szFormat, pArgs);
		return *this;
	}

	text_base & operator = (const text_base & another)
	{
		if(another.m_data != m_data)
		{
			if(m_data)
				m_data->release();

			m_data = another.m_data;

			if(m_data)
				m_data->addref();
		}
		return *this;
	}
	text_base & operator = (const CharT * _text)
	{
		resize(textlen(_text));
		if(m_data)
			m_data->set(_text);
		return *this;
	}

	bool operator == (const text_base & another) const
	{
		if(m_data == another.m_data)
			return true;
		else if(!m_data || !another.m_data)
			return false;
		else
			return textequalex(m_data->m_buffer, -1, another.m_data->m_buffer, -1, true);
	}
	bool operator != (const text_base & another) const
	{
		return !operator==(another);
	}

	bool operator == (const CharT * _text) const
	{
		if(!_text)
			throw exp_nullptr();
		else if(!m_data)
			return false;
		else
			return textequal(m_data->m_buffer, _text);
	}

	bool operator != (const CharT * _text) const
	{
		return !operator==(_text);
	}

	text_base & operator += (const text_base & another)
	{
		return append(another);
	}

	text_base & operator += (const CharT * _text)
	{
		return append(_text);
	}
	// 尽量使用 += 而不是 + 
	text_base operator + (const text_base & another)
	{
		if(!another.length())
			return *this;
		else if(!length())
			return another;
		else
		{
			int_x _size = size() + another.size();
			int_x capability = textdata_t::fit_capability(_size);
			textdata_t * _data = new textdata_t(capability);
			buffcpy(_data->m_buffer, buffer(), size());
			buffcpy(_data->m_buffer + size(), another.buffer(), another.size());
			_data->m_size = _size;
			_data->m_buffer[_size] = 0;
			return text_base(_data);
		}
	}

	text_base operator + (const CharT * _text)
	{
		if(!_text)
			return *this;
		else if(!length())
			return text_base(_text);
		else
		{
			int_x _length = textlen(_text);
			int_x _size = size() + _length;
			int_x capability = textdata_t::fit_capability(_size);
			textdata_t * _data = new textdata_t(capability);
			buffcpy(_data->m_buffer, buffer(), size());
			buffcpy(_data->m_buffer + size(), _text, _length);
			_data->m_size = _size;
			_data->m_buffer[_size] = 0;
			return text_base(_data);
		}
	}

	const CharT & operator [](int_x iIndex) const
	{
		if(iIndex < 0 || iIndex >= length())
			throw exp_out_of_bound();
		else
			return m_data->m_buffer[iIndex];
	}

	CharT & operator [](int_x iIndex)
	{
		if(iIndex < 0 || iIndex >= length())
			throw exp_out_of_bound();
		else
			return m_data->m_buffer[iIndex];
	}

	void resize(int_x _size)
	{
		if(_size < 0 || _size == size())
			return;

		if(!_size)
		{
			if(m_data->ref() == 1)
				m_data->resize(0);
			else
			{
				m_data->release();
				m_data = nullptr;
			}
		}
		else if(!m_data)
			m_data = new textdata_t(_size, 0);
		else if(m_data->ref() > 1)
		{
			textdata_t * _data = new textdata_t(_size, 0);
			textcpy(_data->m_buffer, _data->m_capability, m_data->m_buffer, m_data->m_size);
			m_data->release();
			m_data = _data;
		}
		else
			m_data->resize(_size);
	}

	void reallocate(int_x _capability)
	{
		if(_capability <= capability())
			return;

		if(!m_data)
			m_data = new textdata_t(_capability);
		else if(m_data->ref() > 1)
		{
			textdata_t * _data = new textdata_t(m_data->m_buffer, m_data->m_size, _capability);
			m_data->release();
			m_data = _data;
		}
		else
			m_data->reallocate(_capability);
	}

	text_base sub_text(int_x iBegin, int_x iEnd) const
	{
		int_x iLength = length();
		iBegin = saturate(iBegin, (int_x)0, iLength);
		iEnd = saturate(iEnd, (int_x)0, iLength);
		if(iBegin > iEnd)
			throw exp_illegal_argument();
		else if(iBegin == iEnd)
			return text_base();
		else/* if(iBegin < iEnd)*/
			return text_base(m_data->m_buffer + iBegin, iEnd - iBegin);
	}

	int_x find_first(const CharT & ch) const
	{
		const CharT * pBuffer = buffer();
		return textch(pBuffer, length(), ch);
	}

	int_x find_last(const CharT & ch) const
	{
		const CharT * pBuffer = buffer();
		return textrch(pBuffer, length(), ch);
	}

	bool replace(int_x index_beg, int_x index_end, const CharT * pattern, int_x pattern_length)
	{
		if(index_beg < 0 || index_beg >= index_end || index_end > length())
			return false;

		int_x length_new = length() + (pattern_length - (index_end - index_beg));
		reallocate(length_new + 1);
		return textrplstr(buffer(), length(), capability(), index_beg, index_end, pattern, pattern_length);
	}

	int_x replace(const CharT * src, int_x src_length, const CharT * dst, int_x dst_length, bool caps = true)
	{
		if(src_length < 0) 
			src_length = textlen(src);
		if(dst_length < 0)
			dst_length = textlen(dst);

		if(!src_length || !dst)
			return 0;

		int_x length_old = length();
		int_x count = textcntstr(buffer(), length_old, src, src_length, caps);
		if(!count)
			return 0;

		int_x length_new = length_old + (dst_length - src_length) * count;
		int_x _capability = textdata_t::fit_capability(length_new);
		reallocate(_capability);
		int_x count2 = textrplstr(buffer(), length_old, capability(), src, src_length, dst, dst_length, caps);
		verify(count == count2);
		resize(length_new);
		return count;
	}

	int_x trim(const CharT * trim_chars = nullptr, int_x trim_chars_length = -1)
	{
		if(!m_data || !m_data->m_size)
			return 0;

		if(m_data->ref() > 1)
		{
			textdata_t * pNewData = new textdata_t(*m_data);
			m_data->release();
			m_data = pNewData;
		}

		int_x iLength = texttrim(m_data->m_buffer, m_data->m_size, trim_chars, trim_chars_length);
		m_data->m_size -= iLength;
		return iLength;
	}

	operator const CharT * () const { return m_data ? m_data->m_buffer : nullptr; }
	operator CharT * () { return m_data ? m_data->m_buffer : nullptr; }
	bool is_empty() const { return m_data ? !(m_data->m_size > 0) : true; }
	bool is_valid() const { return m_data ? (m_data->m_size > 0) : false; }

	int_x size() const { return m_data ? m_data->m_size : 0; }
	int_x length() const { return size(); }
	int_x capability() const { return m_data ? m_data->m_capability : 0; }
	const CharT * buffer() const { return m_data ? m_data->m_buffer : nullptr; }
	CharT * buffer() { return m_data ? m_data->m_buffer : nullptr; }

	void swap(text_base & another) { ::swap(m_data, another.m_data); }

protected:
	textdata_t * m_data;
};

typedef text_base<char_16, encodings::utf16.codepage> textw;
typedef text_base<char_8, encodings::ansi.codepage> texta;

namespace encodings
{
	CORE_API textw encode(const char_8 * text, int_x length, encoding_t src_encoding, encoding_t dst_encoding = encodings::utf16);
	CORE_API texta encode(const char_16 * text, int_x length, encoding_t src_encoding, encoding_t dst_encoding = encodings::ansi);

	CORE_API textw encode(texta text, encoding_t src_encoding = encodings::ansi, encoding_t dst_encoding = encodings::utf16);
	CORE_API texta encode(textw text, encoding_t src_encoding = encodings::utf16, encoding_t dst_encoding = encodings::ansi);
}

#ifdef UNICODE
typedef textw textx;
#else
typedef texta textx;
#endif // UNICODE

/**
* @brief 简单的管理字符数组的类。
* CharT —— 字符的类型。
* COUNT —— 字符数组的大小。
* 如果要使用变长的字符数组，请使用 @ref CTextTmpl 。
*/
template<typename CharT, int_x COUNT, int_x CodePage = 0>
class CORE_API chbuffer
{
public:
	chbuffer()
	{
		buffer[0] = 0;
	}

	explicit chbuffer(int_x length)
	{
		if(length >= COUNT)
			length = COUNT - 1;
		buffer[length] = 0;
	}

	chbuffer(const CharT * text, int_x length = -1)
	{
		textcpy(buffer, COUNT, text, length);
	}

	chbuffer(const chbuffer & another)
	{
		textcpy(buffer, COUNT, another.buffer, -1);
	}
	~chbuffer()
	{

	}

	void set(const CharT * text, int_x length = -1)
	{
		if(length < 0)
			length = textlen(text);
		if(!text || !length)
			buffer[0] = 0;
		else
			textcpy(buffer, COUNT, text, length);
	}

	bool equal(const CharT * text, int_x _length = -1, bool caps = true) const
	{
		if(_length < 0)
			_length = textlen(text);
		if(_length != length())
			return false;
		if(!_length)
			return true;

		return textequalex<CharT>(text, _length, buffer, -1, caps);
	}

	chbuffer & operator = (const CharT * szText)
	{
		textcpy(buffer, COUNT, szText);
		return *this;
	}
	chbuffer & operator = (const chbuffer * another)
	{
		textcpy(buffer, COUNT, another->buffer);
		return *this;
	}

	chbuffer & operator += (const CharT * szText)
	{
		textcat(buffer, COUNT, szText, -1);
		return *this;
	}

	bool operator == (const CharT * szText) const
	{
		return textequal(buffer, szText);
	}
	bool operator != (const CharT * szText) const
	{
		return !operator==(szText);
	}

	bool operator == (const chbuffer & another) const
	{
		return textequal(buffer, another.buffer);
	}
	bool operator != (const chbuffer & another) const
	{
		return !operator==(another);
	}

	int_x size() const
	{
		return textlen(buffer, COUNT);
	}
	int_x length() const { return size(); }
	int_x capability() const { return COUNT; }

	bool is_empty() const { return !buffer[0]; }
	bool is_valid() const { return !!buffer[0]; }

	operator CharT * () { return buffer; }
	operator const CharT * () const { return buffer; }

	chbuffer & format(const CharT * format, ...)
	{
		va_list args = nullptr;
		va_start(args, format);
		format_args(format, args);
		va_end(args);
		return *this;
	}

	chbuffer & format_args(const CharT * format, void * args)
	{
		int_x iSize = textformatargs_calc(format, args) + 1;
		if(iSize == 1)
			buffer[0] = 0;
		else
			textformat_args(buffer, COUNT, format, args);
		return *this;
	}

public:
	CharT buffer[COUNT];
};

template<int_x COUNT>
using chbufferw = chbuffer<char_16, COUNT>;
template<int_x COUNT>
using chbuffera = chbuffer<char_8, COUNT>;
template<int_x COUNT>
using chbufferx = chbuffer<char_x, COUNT>;

VENUS_END
