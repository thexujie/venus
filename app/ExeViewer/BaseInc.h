#pragma once

#include "../../venus/venus.h"
#include "../../win32/win32.h"
using namespace venus;

class IMessageMapper
{
public:
	void add(const char_16 * szName, function_base fun)
	{
		fun_t f = {szName, fun};
		m_funs.add(f);
	}

	template<typename FunT>
	void map(FunT ** ppControl, const char_16 * szName)
	{
		const type_info & info = typeid(FunT);
		
		map(&info, (IControl **)ppControl, szName);
	}
	void map(const type_info * pInfo, IControl ** ppControl, const char_16 * szName)
	{
		log0(L"name : %S.", pInfo->name());
		ctl_t ctl = {pInfo, ppControl, szName};
		m_ctls.add(ctl);
	}
private:
	struct ctl_t
	{
		const type_info * info;
		IControl ** control;
		chbufferw<64> name;
	};

	struct fun_t 
	{
		chbufferw<64> name;
		function_base fun;
	};
	vector<fun_t> m_funs;
	vector<ctl_t> m_ctls;
};
