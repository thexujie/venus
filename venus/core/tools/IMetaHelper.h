#pragma once

VENUS_BEG

const int_x META_NAME_MAX = 64;

struct CORE_API meta_t
{
	chbufferw<META_NAME_MAX> name;
	const type_info & info;
	void * data;
};

/**
 * "fun" will be copied.
 * "info" cannot be copied, "&info" instead.
 */
class CORE_API IMetaHelper
{
public:
	virtual ~IMetaHelper() {}
	/**
	 * 增加一个函数映射，function_base 是一种不安全的函数元数据，fun 会被复制。
	 */
	virtual void help_fun(const char_16 * szName, const function_base & fun) = 0;
	/**
	* 增加一个事件映射，event_base 是一种不安全的事件元数据，evt 不会被复制，会对其进行取地址操作。
	*/
	virtual void help_evt(const char_16 * szName, event_base & evt) = 0;

	/**
	* 增加一个数据映射，pData 指向的数据被映射。
	*/
	virtual void help(const char_16 * szName, const type_info & info, void * pData) = 0;

	template<typename ValT>
	void help(const char_16 * szName, ValT * pData)
	{
		const type_info & info = typeid(ValT);
		help(szName, info, (void *)pData);
	}

	virtual function_base * find_fun(const char_16 * szName) = 0;
	virtual event_base * find_evt(const char_16 * szName) = 0;
	virtual meta_t * find_meta(const char_16 * szName) = 0;
};

class CORE_API CMetaHelper : public IMetaHelper
{
public:
	CMetaHelper() {}
	~CMetaHelper() {}

	void help_fun(const char_16 * szName, const function_base & fun)
	{
		func_t f = {szName, fun};
		m_funcs.add(f);
	}

	void help_evt(const char_16 * szName, event_base & evt)
	{
		event_t e = {szName, evt};
		m_events.add(e);
	}

	void help(const char_16 * szName, const type_info & info, void * pData)
	{
		meta_t m = {szName, info, pData};
		m_metas.add(m);
	}

	function_base * find_fun(const char_16 * szName)
	{
		for(int_x cnt = 0, size = m_funcs.size(); cnt < size; ++cnt)
		{
			func_t & fun = m_funcs[cnt];
			if(fun.name == szName)
				return &(fun.fun);
		}
		return nullptr;
	}

	event_base * find_evt(const char_16 * szName)
	{
		for(int_x cnt = 0, size = m_events.size(); cnt < size; ++cnt)
		{
			event_t & evt = m_events[cnt];
			if(evt.name == szName)
				return &(evt.evt);
		}
		return nullptr;
	}
	meta_t * find_meta(const char_16 * szName)
	{
		for(int_x cnt = 0, size = m_metas.size(); cnt < size; ++cnt)
		{
			meta_t & meta = m_metas[cnt];
			if(meta.name == szName)
				return &meta;
		}
		return nullptr;
	}
protected:
	struct func_t
	{
		chbufferw<META_NAME_MAX> name;
		function_base fun;
	};

	struct event_t
	{
		chbufferw<META_NAME_MAX> name;
		event_base & evt;
	};

	vector<func_t> m_funcs;
	vector<event_t> m_events;
	vector<meta_t> m_metas;
};

VENUS_END

