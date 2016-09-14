#pragma once

VENUS_BEG

template<typename EleT, int_x SIZE>
class CORE_API array
{
public:
	typedef EleT * iterator;
	typedef const EleT * const_iterator;
	typedef EleT & reference;
	typedef const EleT & const_reference;

	array() {}
	~array() {}

	array(const array & another)
	{
		arraycpy(buffer, another.buffer);
	}

	operator EleT * ()
	{
		return buffer;
	}

	operator const EleT * () const
	{
		return buffer;
	}

	EleT & operator [](int_x iIndex)
	{
		if(iIndex < 0 || iIndex >= SIZE)
			throw exp_out_of_bound();
		return buffer[iIndex];
	}

	const EleT & operator [](int_x iIndex) const
	{
		if(iIndex < 0 || iIndex >= SIZE)
			throw exp_out_of_bound();
		return buffer[iIndex];
	}
	EleT & at(int_x iIndex)
	{
		if(iIndex < 0 || iIndex >= SIZE)
			throw exp_out_of_bound();
		return buffer[iIndex];
	}

	const EleT & at(int_x iIndex) const
	{
		if(iIndex < 0 || iIndex >= SIZE)
			throw exp_out_of_bound();
		return buffer[iIndex];
	}


	void fill(const EleT & ele)
	{
		for(int_x cnt = 0; cnt < SIZE; ++cnt)
			buffer[cnt] = ele;
	}

	void fill(int_x index, int_x count, const EleT & ele)
	{
		if(count > 0 && 0 <= index && index + count <= SIZE)
		{
			for(int_x cnt = index, end = index + count; cnt < end; ++cnt)
				buffer[cnt] = ele;
		}
	}

	int_x size() const { return SIZE; }


	array & operator = (const array & another)
	{
		arraycpy(buffer, another.buffer);
		return *this;
	}

	iterator begin() { return buffer; }
	iterator end() { return buffer + SIZE; }
	const_iterator begin() const { return buffer; }
	const_iterator end() const { return buffer + SIZE; }

	EleT buffer[SIZE];
};

VENUS_END

