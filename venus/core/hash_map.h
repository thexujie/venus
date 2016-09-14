#pragma once

#include "linear_map.h"

VENUS_BEG

template<
	typename KeyT,
	typename ValT,
	int_x(*HashFunc)(const KeyT &) = hash_def<KeyT>,
	typename EleAllocator = DefEleAllocator<pair<int_x, ValT> > >
class CORE_API hash_map : public linear_map<int_x, ValT, EleAllocator>
{
	typedef linear_map<int_x, ValT, EleAllocator> BaseT;
public:
	hash_map()
	{

	}

	~hash_map()
	{

	}

	ValT & operator[](const KeyT & key)
	{
		int_x uiHash = HashFunc(key);
		return BaseT::operator[](uiHash);
	}

	bool has_key(const KeyT & key) const
	{
		int_x uiHash = HashFunc(key);
		return find(PairCmpKey<int_x, ValT>, uiHash) >= 0;
	}

	bool get_value(const KeyT & key, ValT & value) const
	{
		int_x hash = HashFunc(key);
		return linear_map::get_value(hash, value);
	}
};

VENUS_END
