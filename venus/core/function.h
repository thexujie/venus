#pragma once

#include "vector.h"

VENUS_BEG

CORE_API void make_fun(int_x & m_this, int_x & m_func, const void * p_fun);
CORE_API void make_fun(int_x & m_this, int_x & m_func, const void * p_this, const void * p_fun, int_x size, int_x base_offset);

class CORE_API function_base
{
public:
	function_base() : m_this(0), m_func(0) {}
	function_base(const function_base & another) : m_this(another.m_this), m_func(another.m_func) {}
	function_base(int_x _this, int_x _func) : m_this(_this), m_func(_func) {}
	template<typename ...Args>
	function_base(int_x(*p_fun)(Args...)) { make(p_fun); }
	~function_base() {}

	function_base & operator = (const function_base & another)
	{
		m_this = another.m_this;
		m_func = another.m_func;
		return *this;
	}

	bool operator == (const function_base & another) const
	{
		return m_this == another.m_this && m_func == another.m_func;
	}

	bool operator != (const function_base & another)
	{
		return m_this != another.m_func || m_func != another.m_func;
	}

	void make(const void * p_fun)
	{
		make_fun(m_this, m_func, p_fun);
	}

	void make(const void * p_this, const void * p_fun, int_x size, int_x base_offset)
	{
		make_fun(m_this, m_func, p_this, p_fun, size, base_offset);
	}

	template<typename ...Args>
	void call_base(Args... args) const
	{
		if(!m_func)
			return;
		else if(m_this != 0)
		{
			class CallClass {};
			typedef int_x(CallClass::*CallFuncT)(Args...);
			if(sizeof(CallFuncT) == sizeof(int_x))
			{
				union
				{
					int_x iFunc;
					CallFuncT pmfn;
				};

				iFunc = m_func;
				CallClass * pThis = (CallClass *)m_this;
				(pThis->*pmfn)(args...);
			}
			else if(sizeof(CallFuncT) == sizeof(int_x)* 4)
			{
				union
				{
					struct
					{
						int_x iFunc;
						int_x iNotUsed1;
						int_x iNotUsed2;
						int_x iNotUsed3;
					};
					CallFuncT pmfn;
				};

				iFunc = m_func;
				iNotUsed1 = iNotUsed2 = iNotUsed3 = 0;
				CallClass * pThis = (CallClass *)m_this;
				(pThis->*pmfn)(args...);
			}
			else
			{
				verify(false);
				return;
			}
		}
		else
		{
			typedef int_x(*CallFuncT)(Args...);
			CallFuncT pfn = (CallFuncT)m_func;
			pfn(args...);
		}
	}

protected:
	int_x m_this;
	int_x m_func;
};


/**
 *
 * class Base
 * {
 * public:
 *     virtual int_x Get(int_x a, int_x b) { return a - b; }
 * };

 * class Derived : public Base
 * {
 * public:
 *     int_x Get(int_x a, int_x b) { return a + b; }
 * };
 *
 * Derived derived;
 * auto fun0 = function(&t, &Derived::Get);
 * auto fun1 = function((Base *)&t, &Derived::Get);
 * auto fun2 = function(&t, &Base::Get);
 * auto fun3 = function((Base *)&t, &Base::Get);
 * 这四个 function 是相同的，都是调用了重载版的 Get.
 */
template<typename RetT, typename ...Args>
class CORE_API function : public function_base
{
public:
	typedef RetT (*fun_global_t)(Args...);
	typedef RetT (function::*fun_class_t)(Args...);

	function() 
	{
#ifdef _DEBUG
		_global_ = nullptr;
		_class_ = nullptr;
#endif
	}
	explicit function(const function_base & another) : function_base(another)
	{
#ifdef _DEBUG
		if(m_this)
			_class_ = *(fun_class_t *)&m_func;
		else
			_global_ = *(fun_global_t *)&m_func;
#endif
	}
	function(const function & another) : function_base(another.m_this, another.m_func)
	{
#ifdef _DEBUG
		if(m_this)
			_class_ = another._class_;
		else
			_global_ = another._global_;
#endif
	}
	~function() {}

	function(RetT(*p_fun)(Args...))
	{
		make(p_fun);
	}

	template<typename ClassT, typename BaseT>
	function(const ClassT * p_this, RetT(BaseT::* p_fun)(Args...))
	{
		//const BaseT * p_far = static_cast<const BaseT *>(p_this);
		//make((void *)p_far, (void *)&p_fun, sizeof(p_fun));
		int_x base_offset = (byte_t *)(BaseT *)p_this - (byte_t *)p_this;
		make((void *)p_this, (void *)&p_fun, sizeof(p_fun), base_offset);
	}

	function & operator = (const function & another)
	{
		m_this = another.m_this;
		m_func = another.m_func;
#ifdef _DEBUG
		if(m_this)
			_class_ = another._class_;
		else
			_global_ = another._global_;
#endif
		return *this;
	}

	bool operator == (const function & another) const
	{
		return m_this == another.m_this && m_func == another.m_func;
	}

	bool operator != (const function & another)
	{
		return m_this != another.m_func || m_func != another.m_func;
	}

	void make(const void * p_fun)
	{
		make_fun(m_this, m_func, p_fun);
#ifdef _DEBUG
		_global_ = *(fun_global_t *)&m_func;
#endif
	}

	void make(const void * p_this, const void * p_fun, int_x size, int_x base_offset)
	{
		make_fun(m_this, m_func, p_this, p_fun, size, base_offset);
#ifdef _DEBUG
		_class_ = *(fun_class_t *)&m_func;
#endif
	}

	RetT operator()(Args... args) const
	{
		if(!m_func)
			return RetT();
		else if(m_this != 0)
		{
			class CallClass {};
			typedef RetT(CallClass::*CallFuncT)(Args...);
			if(sizeof(CallFuncT) == sizeof(int_x))
			{
				union
				{
					int_x iFunc;
					CallFuncT pmfn;
				};

				iFunc = m_func;
				CallClass * pThis = (CallClass *)m_this;
				return (pThis->*pmfn)(args...);
			}
			else if(sizeof(CallFuncT) == sizeof(int_x)* 4)
			{
				union
				{
					struct
					{
						int_x iFunc;
						int_x iNotUsed1;
						int_x iNotUsed2;
						int_x iNotUsed3;
					};
					CallFuncT pmfn;
				};

				iFunc = m_func;
				iNotUsed1 = iNotUsed2 = iNotUsed3 = 0;
				CallClass * pThis = (CallClass *)m_this;
				return (pThis->*pmfn)(args...);
			}
			else
			{
				verify(false);
				return RetT();
			}
		}
		else
		{
			typedef RetT(*CallFuncT)(Args...);
			CallFuncT pfn = (CallFuncT)m_func;
			return pfn(args...);
		}
	}
#ifdef _DEBUG
	union
	{
		fun_global_t _global_;
		fun_class_t _class_;
	};
#endif
};

template<typename RetT, typename ...Args>
function<RetT, Args...> bind(int_x(*p_fun)(Args...))
{
	return function<RetT, Args...>(p_fun);
}

template<typename RetT, typename ClassT, typename BaseT, typename ...Args>
function<RetT, Args...> bind(const ClassT * p_this, RetT(BaseT::* p_fun)(Args...))
{
	return function<RetT, Args...>(p_this, p_fun);
}

class CORE_API event_base
{
public:
	event_base() {}
	~event_base() {}
	void connect_base(const function_base & fun) { m_functions.add(fun); }

protected:
	vector<function_base> m_functions;
};

template<typename ...Args>
class CORE_API event : public event_base
{
public:
	typedef function<int_x, Args...> function_type;

	event() {}
	~event() {}

	void connect(const function_type & fun) { m_functions.add(fun); }
	void disconnect(const function_type & fun) { m_functions.remove(fun); }
	void operator += (const function_type & fun) { m_functions.add(fun); }
	void operator -= (const function_type & fun) { m_functions.remove(fun); }

	int_x operator()(Args...args)
	{
		for(int_x cnt = 0, len = m_functions.size(); cnt < len; ++cnt)
		{
			function_type fun(m_functions[cnt]);
			int_x result = fun(args...);
			if(result)
				return result;
		}
		return 0;
	}

	template<typename ClassT, typename BaseT>
	void connect(ClassT * p_this, int_x(BaseT::* p_fun)(Args...))
	{
		connect(function<int_x, Args...>(p_this, p_fun));
	}

	template<typename ClassT, typename BaseT>
	void disconnect(ClassT * p_this, int_x(BaseT::* p_fun)(Args...))
	{
		disconnect(function<int_x, Args...>(p_this, p_fun));
	}
};

VENUS_END
