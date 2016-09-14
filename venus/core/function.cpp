#include "stdafx.h"
#include "function.h"

VENUS_BEG

void make_fun(int_x & m_this, int_x & m_func, const void * p_fun)
{
	m_this = 0;
	m_func = (int_x)p_fun;
}

void make_fun(int_x & m_this, int_x & m_func, const void * p_this, const void * p_fun, int_x size)
{
	//Printf(L"this = 0x%p, fun = 0x%p, size = %d\n", p_this, pFunc, size);
	m_this = 0;
	m_func = 0;

	const byte_t * pbThis = (const byte_t *)p_this;

	if(size == sizeof(int_x))
	{
		struct FunData1
		{
			int_x iFunc;
		}fun;
		fun = *(FunData1 *)p_fun;
		m_this = (int_x)pbThis;
		m_func = fun.iFunc;
	}
	else if(size == sizeof(int_x)* 2)
	{
#ifdef BIT64
		struct FunData2
		{
			int_x iFunc;
			int_32 ivfIndex;
			int_32 ivbIndex;
		}fun;
#elif defined BIT32
		struct FunData2
		{
			int_32 iFunc;
			int_32 ivfIndex;
		}fun;
#else
#error(not compatible for current target machine.)
#endif
		fun = *(FunData2 *)p_fun;
		m_this = (int_x)pbThis;
		// 有虚基偏移
		//if(fun.ivbIndex)
		{
			//const byte_t * vtbl = (const byte_t *)(*(int_x *)pbThis);
			//m_this = (int_x)(pbThis + *(int_32 *)(vtbl + fun.ivbIndex));
			m_this = (int_x)pbThis;
			//按道理应该处理，不处理的结果是：
			//虚继承了一个虚函数，且重载了这个虚函数，这个虚函数无法生成 function。
		}
		//else
		{
			m_this = (int_x)pbThis;
		}
		m_func = fun.iFunc;
	}
	else if(size == sizeof(int_x)* 3)
	{
		struct FunData3
		{
			int_x iFunc;
			int_x ivfIndex;
			int_x ivbIndex;
		}fun;

		fun = *(FunData3 *)p_fun;

		const int_x * piThis = (const int_x *)p_this;

		int_x iOffBase = 0;
		if(fun.ivbIndex)
		{
			int_x iBaseThis = piThis[1];
			iOffBase = *(int_x *)(iBaseThis + fun.ivbIndex) + sizeof(int_x);
		}
		pbThis += iOffBase + fun.ivfIndex;

		m_this = (int_x)pbThis;
		m_func = fun.iFunc;
	}
	else if(size == sizeof(int_x)* 4)
	{
		struct FunData4
		{
			int_x iFunc;
			int_x ivfIndex;
			int_x ivbIndex;
			int_x ivtIndex;
		}fun;

		fun = *(FunData4 *)p_fun;

		const int_x * piThis = (const int_x *)p_this;

		int_x iOffBase = 0;
		if(fun.ivtIndex)
			iOffBase += fun.ivbIndex + *((int_x *)(*((int_x *)(pbThis + fun.ivbIndex)) + fun.ivtIndex));
		else {}
		pbThis += iOffBase + fun.ivfIndex;

		m_this = (int_x)pbThis;
		m_func = fun.iFunc;
	}
	else
		Assert(false);
}

VENUS_END
