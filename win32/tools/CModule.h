#pragma once

VENUS_BEG

template<typename ValT, typename RetT, typename... ArgsT>
class VENUS_API proc_base
{
public:
	typedef ValT fun_type;
	typedef RetT return_type;

	proc_base()
	{

	}
	proc_base(fun_type _pfn) :pfn(_pfn)
	{

	}

	proc_base(const proc_base & another) :pfn(another.pfn)
	{

	}

	proc_base & operator = (const proc_base & another)
	{
		pfn = another.pfn;
		return *this;
	}

	proc_base & operator = (fun_type _pfn)
	{
		pfn = _pfn;
		return *this;
	}

	proc_base & set(fun_type _pfn)
	{
		pfn = _pfn;
		return *this;
	}

	operator bool() const
	{
		return !!pfn;
	}

	RetT operator()(ArgsT... _Args) const
	{
		return pfn(_Args...);
	}

protected:
	fun_type pfn;
};

template<typename ValT>
struct VENUS_API proc_helper {};

#ifdef BIT32

template<typename RetT, typename... ArgsT>
struct VENUS_API proc_helper<RetT __stdcall(ArgsT...)>
{
	typedef RetT return_t;
	typedef proc_base<RetT(__stdcall *)(ArgsT...), RetT, ArgsT...> proc_base_t;
};

template<typename RetT, typename... ArgsT>
struct VENUS_API proc_helper<RetT __cdecl(ArgsT...)>
{
	typedef RetT return_t;
	typedef proc_base<RetT(__cdecl *)(ArgsT...), RetT, ArgsT...> proc_base_t;
};

#else

template<typename RetT, typename... ArgsT>
struct VENUS_API proc_helper<RetT(ArgsT...)>
{
	typedef RetT return_t;
	typedef proc_base<RetT(*)(ArgsT...), RetT, ArgsT...> proc_base_t;
};

#endif // BIT32

template<typename T>
class VENUS_API proc_ptr : public proc_helper<T>::proc_base_t
{
public:
	typedef typename proc_helper<T>::proc_base_t proc_base_t;
	typedef typename proc_base_t::fun_type fun_type;
	typedef typename proc_base_t::return_type return_type;

	proc_ptr()
	{

	}
	proc_ptr(fun_type _pfn) :proc_base_t(_pfn)
	{

	}

	proc_ptr(const proc_ptr & another) :proc_base_t(another)
	{

	}

	proc_ptr & operator = (const proc_ptr & another)
	{
		pfn = another.pfn;
		return *this;
	}

	proc_ptr & operator = (fun_type _pfn)
	{
		pfn = _pfn;
		return *this;
	}

	proc_ptr & set(fun_type _pfn)
	{
		pfn = _pfn;
		return *this;
	}

	bool is_null() const { return !!pfn; }
};

class WIN32_API CModule
{
public:
	CModule();
	CModule(const char_x * szFile);
	~CModule();

	bool Load(const char_x * szFile);
	void Free();
	bool IsLoaded() const;
	void * _GetProc(const char_8 * szProc);
	void * _GetProc(const char_16 * szProc);

	operator HMODULE() const { return m_hModule; }

	template<typename T>
	T GetProc(const char_8 * szProc)
	{
		void * pProc = _GetProc(szProc);
		return (T)pProc;
	}

	template<typename T>
	T GetProc(const char_16 * szProc)
	{
		void * pProc = _GetProc(szProc);
		return (T)pProc;
	}

	template<typename T>
	bool GetProc(proc_ptr<T> & proc, const char_8 * szProc)
	{
		void * pProc = _GetProc(szProc);
		proc.set(static_cast<proc_ptr<T>::fun_type>(pProc));
		return proc.is_null();
	}

	template<typename T>
	bool GetProc(proc_ptr<T> & proc, const char_16 * szProc)
	{
		void * pProc = _GetProc(szProc);
		proc.set(static_cast<proc_ptr<T>::fun_type(pProc));
		return proc.is_null();
	}

protected:
	HMODULE m_hModule;
};

VENUS_END
