#pragma once

VENUS_BEG

template<typename KeyT, typename ValT>
class CORE_API pair
{
public:
	pair()
	{
		buffset8(this, sizeof(pair), 0);
	}
	pair(const KeyT & _key, const ValT & _val)
		: key(_key), value(_val)
	{

	}

	pair(const pair & another)
		: key(another.key), value(another.value)

	{

	}

	~pair()
	{

	}

	pair & operator = (const pair & another)
	{
		key = another.key;
		value = another.value;
		return *this;
	}

	bool operator == (const pair & anthoer) const { return key == anthoer.key && value == anthoer.value; }
	bool operator != (const pair & anthoer) const { return key != anthoer.key || value != anthoer.value; }

public:
	KeyT key;
	ValT value;
};


template<typename KeyT, typename ValT>
bool PairCmpKey(const pair<KeyT, ValT> & pair, const KeyT & key)
{
	return pair.key == key;
}

VENUS_END
