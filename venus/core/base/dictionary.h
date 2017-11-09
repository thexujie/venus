#pragma once

#include "pair.h"
#include "vector.h"

VENUS_BEG

template<typename KeyT, typename ValT, typename EleAllocator = DefEleAllocator<pair<KeyT, ValT> > >
class CORE_API dictionary : public vector<pair<KeyT, ValT> >
{
	typedef pair<KeyT, ValT> PairT;
	typedef vector<pair<KeyT, ValT> > BaseT;
public:
	dictionary()
	{

	}

	~dictionary()
	{

	}

	ValT & operator[](const KeyT & key)
	{
		return value(key);
	}

	const ValT & operator[](const KeyT & key) const
	{
		return value(key);
	}

	ValT & value(const KeyT & key)
	{
		PairT & ele = _get(key);
		ele.key = key;
		return ele.value;
	}

	const ValT & value(const KeyT & key) const
	{
		const PairT & ele = _get(key);
		ele.key = key;
		return ele.value;
	}

	bool has_key(const KeyT & key) const
	{
		int_x iIndex = find(PairCmpKey<KeyT, ValT>, key);
		return iIndex >= 0;
	}

	bool get_value(const KeyT & key, ValT & value) const
	{
		int_x iIndex = find(PairCmpKey<KeyT, ValT>, key);
		if(iIndex >= 0)
		{
			value = at(iIndex).value;
			return true;
		}
		else
			return false;
	}

protected:
	PairT & _get(const KeyT & key)
	{
		int_x iIndex = find(PairCmpKey<KeyT, ValT>, key);
		if(iIndex < 0)
			return add();
		else
			return BaseT::operator[](iIndex);
	}
	const PairT & _get(const KeyT & key) const
	{
		int_x iIndex = find(PairCmpKey<KeyT, ValT>, key);
		if(iIndex < 0)
			return add();
		else
			return at(iIndex);
	}
};

VENUS_END
