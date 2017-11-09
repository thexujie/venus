#pragma once

VENUS_BEG

/**
* @defgroup VenusCore-Base-Fun 基本函数
* @ingroup VenusCore-Base
* @brief 定义了一些常用的基本函数。
* @{
*/

CORE_API void log(int_x iLevel, const char_16 * format, ...);
CORE_API void log(int_x iLevel, const char_8 * format, ...);
#ifdef _DEBUG
CORE_API void log0(const char_16 * format, ...);
CORE_API void log0(const char_8 * format, ...);
#else
CORE_API void log0(const char_16 * format, ...){ }
CORE_API void log0(const char_8 * format, ...){ }
#endif
CORE_API void log1(const char_16 * format, ...);
CORE_API void log1(const char_8 * format, ...);
CORE_API void log2(const char_16 * format, ...);
CORE_API void log2(const char_8 * format, ...);

#if MSC_VER >= MSC_2015
inline constfun uint_16 make_twocc(char_8 ch0, char_8 ch1)
{
	return ch0 | (ch1 << 8);
}

inline constfun uint_32 make_fourcc(char_8 ch0, char_8 ch1, char_8 ch2, char_8 ch3)
{
	return ch0 | (ch1 << 8) | (ch2 << 16) | (ch3 << 24);
}
#else
#define make_twocc(ch0, ch1) (ch0 | (ch1 << 8))
#define make_fourcc(ch0, ch1, ch2, ch3) (ch0 | (ch1 << 8) | (ch2 << 16) | (ch3 << 24))
#endif

/*!@brief 触发一个调试断点。*/
CORE_API void breakpoint();

/*!@brief 断言，如果失败，会触发一个调试断点。非Debug版该函数不起任何作用。*/
template<typename ValT>
void verify(const ValT & val)
{
#ifdef _DEBUG
	if(val == 0)
		breakpoint();
#endif
}
/*!@brief 断言，如果失败，会触发一个调试断点。非Debug版该函数也会起作用。*/
template<typename ValT>
void ensure(const ValT & val)
{
	if(val == 0)
		breakpoint();
}

/**
* @} // VenusCore-Base-Fun
*/

/**
* @brief 安全地 delete 一个指针，并将指针赋 nullptr。
删除方式为 delete ptr
* @param ptr 需要被 delete 的指针的引用。
*/
template<typename ValT>
void SafeDelete(ValT *& ptr)
{
	if(ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

/**
*@brief 安全地 delete 一个数组指针，并将指针赋 nullptr。
* 删除方式为 delete[] ptr
*@param ptr 需要被 delete 的数组指针的引用。
*/
template<typename ValT>
void SafeDeleteArray(ValT *& ptr)
{
	if(ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}

/**
*@brief 安全地 delete 一个数组里的每一个指针，并将指针赋 nullptr。
* 删除方式为 delete ptr[n]
*@param ptr 需要被 delete 的数组指针的引用。
*/
template<typename ValT>
void SafeDeleteArray(ValT * ptr[], int_x iLength)
{
	if(ptr)
	{
		for(int_x cnt = 0; cnt < iLength; ++cnt)
			delete ptr[cnt];
	}
}

/**
* @brief 安全地 调用 Release 指针，并将指针赋 nullptr。
* @param ptr 需要被 Release 的指针的引用。
*/
template<typename ValT>
void SafeRelease(ValT *& ptr)
{
	if(ptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}

template<typename ValT>
void SafeReleasePtr(ValT ** ptr)
{
	if(ptr && *ptr)
	{
		(*ptr)->Release();
		(*ptr) = nullptr;
	}
}

/**
* @brief 安全地将指针赋 nullptr。
* @param ptr 需要被赋 nullptr 的指针的引用。
*/
template<typename ValT>
void SafeNull(ValT *& ptr)
{
	ptr = nullptr;
}

/**
* @brief 安全地 调用 Release 一个数组，并将数组中每个指针赋 nullptr。
* @param ptr 需要被 Release 的指针的引用。
*/
template<typename ValT>
void SafeReleaseArray(ValT ** pPtr, int_x iLength)
{
	for(int_x cnt = 0; cnt < iLength; ++cnt)
	{
		if(pPtr[cnt])
		{
			(pPtr[cnt])->Release();
			pPtr[cnt] = nullptr;
		}
	}
}

/**
* @brief 安全地 调用 AddRef。
* @param ptr 需要被 AddRef 的指针的引用。
*/
template<typename ValT>
void SafeAddRef(ValT * const & ptr)
{
	if(ptr)
	{
		ptr->AddRef();
	}
}

/**
* @brief 不调用 AddRef，部分情况下，防止造成循环引用，
*        在确信子对象生命周期短与父对象时，不增加父对象
*        的引用计数
* @param ptr 目标指针
*/
template<typename ValT>
void FakeAddRef(ValT *& ptr)
{

}

/**
* @brief 不调用 Release，仅赋空，部分情况下，防止造成循环引用，
*        在确信子对象生命周期短与父对象时，不减少父对象
*        的引用计数
* @param ptr 目标指针
*/
template<typename ValT>
void FakeRelease(ValT *& ptr)
{
	if(ptr)
		ptr = nullptr;
}

template<typename ValT>
void SafeAddRefPtr(ValT ** ptr)
{
	if(ptr && *ptr)
	{
		(*ptr)->AddRef();
	}
}

/**
* @brief 求两个数种较小的一个。
* @param a,b 要比较的两个数的 const 引用。
* @return 返回较小的一个数的 const 引用。
*/
template<typename ValT>
constfun const ValT & minof(const ValT & a, const ValT & b)
{
	return a > b ? b : a;
}

/**
* @brief 求若干个数中较小的一个。
* @param params,v 要比较的数
* @return 返回较小的一个数的 const 引用。
*/
template<typename ...ParamsT, typename ValT>
constfun const ValT & minof(const ValT & a, const ValT & b, const ParamsT & ...params)
{
	return minof(minof(a, b), params...);
}

/**
* @brief 求两个数种较大的一个。
* @param a,b 要比较的两个数的 const 引用。
* @return 返回较大的一个数的 const 引用。
*/
template<typename ValT>
constfun const ValT & maxof(const ValT & a, const ValT & b)
{
	return a > b ? a : b;
}

/**
* @brief 求若干个数中较大的一个。
* @param params,v 要比较的数
* @return 返回较大的一个数的 const 引用。
*/
template<typename ...ParamsT, typename ValT>
constfun const ValT & maxof(const ValT & a, const ValT & b, const ParamsT & ...params)
{
	return maxof(maxof(a, b), params...);
}


template<typename ValT>
constfun const ValT & saturate(const ValT & value, const ValT & min, const ValT & max)
{
	return value < min ? value : (value > max ? max : value);
}
/**
* @brief 交换两个数字的值。
* @param a,b 要交换的两个数的 const 引用。
*/
template<typename ValT>
void swap(ValT & a, ValT & b)
{
	ValT tmp = a; a = b; b = tmp;
}
/**
* @brief 交换两个指针所指向的数据的值。
* @param a,b 要交换的两个数的 const 引用。
*/
template<typename ValT>
void swap_ptr(ValT * pA, ValT * pB)
{
	ValT temp = *pA; *pA = *pB; *pB = temp;
}

/**
* @brief 求数字的绝对值。
* @param val 要求绝对值得数。
* @return 返回输入数字的绝对值。
* @warning 对无符号数调用 Abs 没有意义，并且会触发编译器警告。
*/
template<typename ValT>
ValT abs(const ValT & val)
{
	return val > 0 ? val : -val;
}

#pragma region lowhigh

/*! @brief 获取 int_32 类型值的高16位。*/
inline int_16 i32li16(int_32 val)
{
	return ((int_16 *)&val)[0];
}

/*! @brief 获取 int_32 类型值的低16位。*/
inline int_16 i32hi16(int_32 val)
{
	return ((int_16 *)&val)[1];
}

/*! @brief 获取 int_32 类型值的高16位。*/
inline uint_16 i32lu16(int_32 val)
{
	return ((uint_16 *)&val)[0];
}

/*! @brief 获取 int_32 类型值的低16位。*/
inline uint_16 i32hu16(int_32 val)
{
	return ((uint_16 *)&val)[1];
}

/*! @brief 使用指定的低位和高位生成一个 int_32 类型的值*/
inline int_32 i32fromi16(int_16 lval, int_16 hval)
{
	int_32 val;
	((int_16 *)&val)[0] = lval;
	((int_16 *)&val)[1] = hval;
	return val;
}

/*! @brief 使用指定的低位和高位生成一个 int_32 类型的值*/
inline int_32 i32fromu16(uint_16 lval, uint_16 hval)
{
	int_32 val;
	((uint_16 *)&val)[0] = lval;
	((uint_16 *)&val)[1] = hval;
	return val;
}

/*! @brief 获取 uint_32 类型值的低16位。*/
inline uint_16 u32lu16(uint_32 val)
{
	return ((uint_16 *)&val)[0];
}

/*! @brief 获取 uint_32 类型值的高16位。*/
inline uint_16 u32hu16(uint_32 val)
{
	return ((uint_16 *)&val)[1];
}

/*! @brief 获取 uint_32 类型值的低16位。*/
inline int_16 u32li16(uint_32 val)
{
	return ((int_16 *)&val)[0];
}

/*! @brief 获取 uint_32 类型值的高16位。*/
inline int_16 u32hi16(uint_32 val)
{
	return ((int_16 *)&val)[1];
}

/*! @brief 使用指定的低位和高位生成一个 uint_32 类型的值*/
inline uint_32 u32fromu16(uint_16 lval, uint_16 hval)
{
	uint_32 val;
	((uint_16 *)&val)[0] = lval;
	((uint_16 *)&val)[1] = hval;
	return val;
}

/*! @brief 使用指定的低位和高位生成一个 uint_32 类型的值*/
inline uint_32 u32fromi16(int_16 lval, int_16 hval)
{
	uint_32 val;
	((int_16 *)&val)[0] = lval;
	((int_16 *)&val)[1] = hval;
	return val;
}

/*! @brief 获取 int_64 类型值的低32位。*/
inline int_32 i64li32(int_64 val)
{
	return ((int_32 *)&val)[0];
}

/*! @brief 获取 int_64 类型值的高32位。*/
inline int_32 i64hi32(int_64 val)
{
	return ((int_32 *)&val)[1];
}

/*! @brief 获取 int_64 类型值的低32位。*/
inline uint_32 i64lu32(int_64 val)
{
	return ((uint_32 *)&val)[0];
}

/*! @brief 获取 int_64 类型值的高32位。*/
inline uint_32 i64hu32(int_64 val)
{
	return ((uint_32 *)&val)[1];
}

/*! @brief 使用指定的低位和高位生成一个 int_64 类型的值*/
inline int_64 i64fromi32(int_32 lval, int_32 hval)
{
	int_64 val;
	((int_32 *)&val)[0] = lval;
	((int_32 *)&val)[1] = hval;
	return val;
}

/*! @brief 使用指定的低位和高位生成一个 int_64 类型的值*/
inline int_64 i64fromu32(uint_32 lval, uint_32 hval)
{
	int_64 val;
	((uint_32 *)&val)[0] = lval;
	((uint_32 *)&val)[1] = hval;
	return val;
}

/*! @brief 获取 uint_64 类型值的低32位。*/
inline uint_32 u64lu32(uint_64 val)
{
	return ((uint_32 *)&val)[0];
}

/*! @brief 获取 uint_64 类型值的高32位。*/
inline uint_32 u64hu32(uint_64 val)
{
	return ((uint_32 *)&val)[1];
}

/*! @brief 获取 uint_64 类型值的低32位。*/
inline int_32 u64li32(uint_64 val)
{
	return ((int_32 *)&val)[0];
}

/*! @brief 获取 uint_64 类型值的高32位。*/
inline int_32 u64hi32(uint_64 val)
{
	return ((int_32 *)&val)[1];
}

/*! @brief 使用指定的低位和高位生成一个 int_64 类型的值*/
inline uint_64 u64fromu32(uint_32 lval, uint_32 hval)
{
	uint_64 val;
	((uint_32 *)&val)[0] = lval;
	((uint_32 *)&val)[1] = hval;
	return val;
}

/*! @brief 使用指定的低位和高位生成一个 int_64 类型的值*/
inline uint_64 u64fromi32(int_32 lval, int_32 hval)
{
	uint_64 val;
	((int_32 *)&val)[0] = lval;
	((int_32 *)&val)[1] = hval;
	return val;
}

/*! @brief 获取 int_x 类型值的低16/32位。*/
inline int_h ixlih(int_x iVal)
{
#ifdef BIT64
	return i64li32(iVal);
#else
	return i32li16(iVal);
#endif
}

/*! @brief 获取 int_x 类型值的高16/32位。*/
inline int_h ixhih(int_x val)
{
#ifdef BIT64
	return i64hi32(val);
#else
	return i32hi16(val);
#endif
}

/*! @brief 获取 int_x 类型值的低16/32位。*/
inline uint_h ixluh(int_x iVal)
{
#ifdef BIT64
	return i64lu32(iVal);
#else
	return i32lu16(iVal);
#endif
}

/*! @brief 获取 int_x 类型值的高16/32位。*/
inline int_h ixhuh(int_x val)
{
#ifdef BIT64
	return i64hu32(val);
#else
	return i32hu16(val);
#endif
}

/*! @brief 使用指定的低位和高位生成一个 int_x 类型的值*/
inline int_x ixfromih(int_h lval, int_h hval)
{
	int_x val;
	((int_h *)&val)[0] = lval;
	((int_h *)&val)[1] = hval;
	return val;
}

/*! @brief 使用指定的低位和高位生成一个 int_x 类型的值*/
inline int_x ixfromuh(uint_h lval, uint_h hval)
{
	int_x val;
	((uint_h *)&val)[0] = lval;
	((uint_h *)&val)[1] = hval;
	return val;
}

/*! @brief 获取 uint_x 类型值的低16/32位。*/
inline uint_h uxluh(uint_x val)
{
#ifdef BIT64
	return u64lu32(val);
#else
	return u32lu16(val);
#endif
}

/*! @brief 获取 uint_x 类型值的高16/32位。*/
inline uint_h uxhuh(uint_x val)
{
#ifdef BIT64
	return u64hu32(val);
#else
	return u32hu16(val);
#endif
}

/*! @brief 获取 uint_x 类型值的高16/32位。*/
inline int_h uxlih(uint_x val)
{
#ifdef BIT64
	return u64li32(val);
#else
	return u32li16(val);
#endif
}

/*! @brief 获取 uint_x 类型值的低16/32位。*/
inline int_h uxhih(uint_x val)
{
#ifdef BIT64
	return u64hi32(val);
#else
	return u32hi16(val);
#endif
}

/*! @brief 使用指定的低位和高位生成一个 uint_x 类型的值*/
inline uint_x uxfromuh(uint_h lval, uint_h hval)
{
	uint_x val;
	((uint_h *)&val)[0] = lval;
	((uint_h *)&val)[1] = hval;
	return val;
}

/*! @brief 使用指定的低位和高位生成一个 uint_x 类型的值*/
inline uint_x uxfromih(int_h lval, int_h hval)
{
	uint_x val;
	((int_h *)&val)[0] = lval;
	((int_h *)&val)[1] = hval;
	return val;
}

template<typename IntT>
inline bool bittest(const IntT & value, int_x index)
{
	return !!(value & (1 << index));
}

inline int_x intx_bits()
{
	return 0;
}

template<typename ...Args>
inline int_x intx_bits(int_x bit, Args... args)
{
	return ((int_x)1 << bit) | intx_bits(args...);
}


/*!@brief 判断left是否>right。*/
template<typename EleT>
bool bigger_then(const EleT & left, const EleT & right)
{
	return left > right;
}

/*!@brief 判断*pLeft是否>*pRight。*/
template<typename EleT>
bool bigger_then_ptr(const EleT * left, const EleT * right)
{
	return *left > *right;
}

/*!@brief 判断left是否<right。*/
template<typename EleT>
bool small_then(const EleT & left, const EleT & right)
{
	return left < right;
}

/*!@brief 判断 *left是否小于 *right。*/
template<typename EleT>
bool small_then_ptr(const EleT * left, const EleT * right)
{
	return *left < *right;
}

const int_32 DEF_BASE_32 = 0x01000193u;
const int_32 DEF_SEED_32 = 0x811C9DC5u;
const int_64 DEF_BASE_64 = 0x0100019301000193u;
const int_64 DEF_SEED_64 = 0x811C9DC5811C9DC5u;
#ifdef BIT64
const int_64 DEF_BASE_X = DEF_BASE_64;
const int_64 DEF_SEED_X = DEF_SEED_64;
#else
const int_32 DEF_BASE_X = DEF_BASE_32;
const int_32 DEF_SEED_X = DEF_SEED_32;
#endif

CORE_API int_x hashtext8(const char_8 * const & text, int_x len = -1);
CORE_API int_x hashtext16(const char_16 * const & text, int_x len = -1);
CORE_API int_x hashtextx(const char_x * const & text, int_x len = -1);

template<typename ValT>
int_32 hash32(const ValT & val, int_32 curr = DEF_SEED_32)
{
	int_32 iVal = (int_32)val;
	return DEF_BASE_32 * curr ^ iVal;
}

template<typename ValT>
int_64 hash64(const ValT & val, int_64 curr = DEF_SEED_64)
{
	int_64 iVal = (int_64)val;
	return DEF_BASE_64 * curr ^ iVal;
}

template<typename ValT>
int_x hashx(const ValT & val, int_x curr = DEF_SEED_X)
{
	int_x iVal = (int_x)val;
	return DEF_BASE_X * curr ^ iVal;
}

CORE_API int_x hashbool(const bool & val, int_x curr = DEF_SEED_X);

CORE_API int_x hashbytes(const byte_t * bytes, int_x len);

template<typename KeyT>
int_x hash_def(const KeyT & key)
{
	return hashbytes((const byte_t *)&key, sizeof(KeyT));
}

template<typename _Ty>
struct remove_reference { typedef _Ty type; };
template<typename _Ty>
struct remove_reference<_Ty&> { typedef _Ty type; };
template<typename _Ty> struct remove_reference<_Ty&&> { typedef _Ty type; };
template<typename _Ty>
typename remove_reference<_Ty>::type && move(_Ty && val)
{
	return static_cast<typename remove_reference<_Ty>::type &&>(val);
}

/**
* 顺向遍历
*/
template<typename VT, typename OP>
OP for_each(VT & vt, OP op)
{
	typedef typename VT::iterator it;
	for(auto beg = vt.begin(), end = vt.end(); beg != end; ++beg)
	{
		op(*beg);
	}
	return move(op);
}

/**
* 顺向遍历
*/
template<typename VT, typename OP>
OP for_cis(VT & vt, OP op)
{
	typedef typename VT::iterator it;
	for(auto beg = vt.begin(), end = vt.end(); beg != end; ++beg)
	{
		op(*beg);
	}
	return move(op);
}

/**
* 反向遍历
*/
template<typename VT, typename OP>
OP for_rev(VT & vt, OP op)
{
	for(auto beg = vt.begin(), end = vt.end(); beg != end; --end)
	{
		op(*end);
	}
	return move(op);
}

VENUS_END

#include "core_api_buff.h"
#include "core_api_text.h"

