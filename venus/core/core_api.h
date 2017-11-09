#pragma once

VENUS_BEG

/**
* @defgroup VenusCore-Base-Fun ��������
* @ingroup VenusCore-Base
* @brief ������һЩ���õĻ���������
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

/*!@brief ����һ�����Զϵ㡣*/
CORE_API void breakpoint();

/*!@brief ���ԣ����ʧ�ܣ��ᴥ��һ�����Զϵ㡣��Debug��ú��������κ����á�*/
template<typename ValT>
void verify(const ValT & val)
{
#ifdef _DEBUG
	if(val == 0)
		breakpoint();
#endif
}
/*!@brief ���ԣ����ʧ�ܣ��ᴥ��һ�����Զϵ㡣��Debug��ú���Ҳ�������á�*/
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
* @brief ��ȫ�� delete һ��ָ�룬����ָ�븳 nullptr��
ɾ����ʽΪ delete ptr
* @param ptr ��Ҫ�� delete ��ָ������á�
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
*@brief ��ȫ�� delete һ������ָ�룬����ָ�븳 nullptr��
* ɾ����ʽΪ delete[] ptr
*@param ptr ��Ҫ�� delete ������ָ������á�
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
*@brief ��ȫ�� delete һ���������ÿһ��ָ�룬����ָ�븳 nullptr��
* ɾ����ʽΪ delete ptr[n]
*@param ptr ��Ҫ�� delete ������ָ������á�
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
* @brief ��ȫ�� ���� Release ָ�룬����ָ�븳 nullptr��
* @param ptr ��Ҫ�� Release ��ָ������á�
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
* @brief ��ȫ�ؽ�ָ�븳 nullptr��
* @param ptr ��Ҫ���� nullptr ��ָ������á�
*/
template<typename ValT>
void SafeNull(ValT *& ptr)
{
	ptr = nullptr;
}

/**
* @brief ��ȫ�� ���� Release һ�����飬����������ÿ��ָ�븳 nullptr��
* @param ptr ��Ҫ�� Release ��ָ������á�
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
* @brief ��ȫ�� ���� AddRef��
* @param ptr ��Ҫ�� AddRef ��ָ������á�
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
* @brief ������ AddRef����������£���ֹ���ѭ�����ã�
*        ��ȷ���Ӷ����������ڶ��븸����ʱ�������Ӹ�����
*        �����ü���
* @param ptr Ŀ��ָ��
*/
template<typename ValT>
void FakeAddRef(ValT *& ptr)
{

}

/**
* @brief ������ Release�������գ���������£���ֹ���ѭ�����ã�
*        ��ȷ���Ӷ����������ڶ��븸����ʱ�������ٸ�����
*        �����ü���
* @param ptr Ŀ��ָ��
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
* @brief ���������ֽ�С��һ����
* @param a,b Ҫ�Ƚϵ��������� const ���á�
* @return ���ؽ�С��һ������ const ���á�
*/
template<typename ValT>
constfun const ValT & minof(const ValT & a, const ValT & b)
{
	return a > b ? b : a;
}

/**
* @brief �����ɸ����н�С��һ����
* @param params,v Ҫ�Ƚϵ���
* @return ���ؽ�С��һ������ const ���á�
*/
template<typename ...ParamsT, typename ValT>
constfun const ValT & minof(const ValT & a, const ValT & b, const ParamsT & ...params)
{
	return minof(minof(a, b), params...);
}

/**
* @brief ���������ֽϴ��һ����
* @param a,b Ҫ�Ƚϵ��������� const ���á�
* @return ���ؽϴ��һ������ const ���á�
*/
template<typename ValT>
constfun const ValT & maxof(const ValT & a, const ValT & b)
{
	return a > b ? a : b;
}

/**
* @brief �����ɸ����нϴ��һ����
* @param params,v Ҫ�Ƚϵ���
* @return ���ؽϴ��һ������ const ���á�
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
* @brief �����������ֵ�ֵ��
* @param a,b Ҫ�������������� const ���á�
*/
template<typename ValT>
void swap(ValT & a, ValT & b)
{
	ValT tmp = a; a = b; b = tmp;
}
/**
* @brief ��������ָ����ָ������ݵ�ֵ��
* @param a,b Ҫ�������������� const ���á�
*/
template<typename ValT>
void swap_ptr(ValT * pA, ValT * pB)
{
	ValT temp = *pA; *pA = *pB; *pB = temp;
}

/**
* @brief �����ֵľ���ֵ��
* @param val Ҫ�����ֵ������
* @return �����������ֵľ���ֵ��
* @warning ���޷��������� Abs û�����壬���һᴥ�����������档
*/
template<typename ValT>
ValT abs(const ValT & val)
{
	return val > 0 ? val : -val;
}

#pragma region lowhigh

/*! @brief ��ȡ int_32 ����ֵ�ĸ�16λ��*/
inline int_16 i32li16(int_32 val)
{
	return ((int_16 *)&val)[0];
}

/*! @brief ��ȡ int_32 ����ֵ�ĵ�16λ��*/
inline int_16 i32hi16(int_32 val)
{
	return ((int_16 *)&val)[1];
}

/*! @brief ��ȡ int_32 ����ֵ�ĸ�16λ��*/
inline uint_16 i32lu16(int_32 val)
{
	return ((uint_16 *)&val)[0];
}

/*! @brief ��ȡ int_32 ����ֵ�ĵ�16λ��*/
inline uint_16 i32hu16(int_32 val)
{
	return ((uint_16 *)&val)[1];
}

/*! @brief ʹ��ָ���ĵ�λ�͸�λ����һ�� int_32 ���͵�ֵ*/
inline int_32 i32fromi16(int_16 lval, int_16 hval)
{
	int_32 val;
	((int_16 *)&val)[0] = lval;
	((int_16 *)&val)[1] = hval;
	return val;
}

/*! @brief ʹ��ָ���ĵ�λ�͸�λ����һ�� int_32 ���͵�ֵ*/
inline int_32 i32fromu16(uint_16 lval, uint_16 hval)
{
	int_32 val;
	((uint_16 *)&val)[0] = lval;
	((uint_16 *)&val)[1] = hval;
	return val;
}

/*! @brief ��ȡ uint_32 ����ֵ�ĵ�16λ��*/
inline uint_16 u32lu16(uint_32 val)
{
	return ((uint_16 *)&val)[0];
}

/*! @brief ��ȡ uint_32 ����ֵ�ĸ�16λ��*/
inline uint_16 u32hu16(uint_32 val)
{
	return ((uint_16 *)&val)[1];
}

/*! @brief ��ȡ uint_32 ����ֵ�ĵ�16λ��*/
inline int_16 u32li16(uint_32 val)
{
	return ((int_16 *)&val)[0];
}

/*! @brief ��ȡ uint_32 ����ֵ�ĸ�16λ��*/
inline int_16 u32hi16(uint_32 val)
{
	return ((int_16 *)&val)[1];
}

/*! @brief ʹ��ָ���ĵ�λ�͸�λ����һ�� uint_32 ���͵�ֵ*/
inline uint_32 u32fromu16(uint_16 lval, uint_16 hval)
{
	uint_32 val;
	((uint_16 *)&val)[0] = lval;
	((uint_16 *)&val)[1] = hval;
	return val;
}

/*! @brief ʹ��ָ���ĵ�λ�͸�λ����һ�� uint_32 ���͵�ֵ*/
inline uint_32 u32fromi16(int_16 lval, int_16 hval)
{
	uint_32 val;
	((int_16 *)&val)[0] = lval;
	((int_16 *)&val)[1] = hval;
	return val;
}

/*! @brief ��ȡ int_64 ����ֵ�ĵ�32λ��*/
inline int_32 i64li32(int_64 val)
{
	return ((int_32 *)&val)[0];
}

/*! @brief ��ȡ int_64 ����ֵ�ĸ�32λ��*/
inline int_32 i64hi32(int_64 val)
{
	return ((int_32 *)&val)[1];
}

/*! @brief ��ȡ int_64 ����ֵ�ĵ�32λ��*/
inline uint_32 i64lu32(int_64 val)
{
	return ((uint_32 *)&val)[0];
}

/*! @brief ��ȡ int_64 ����ֵ�ĸ�32λ��*/
inline uint_32 i64hu32(int_64 val)
{
	return ((uint_32 *)&val)[1];
}

/*! @brief ʹ��ָ���ĵ�λ�͸�λ����һ�� int_64 ���͵�ֵ*/
inline int_64 i64fromi32(int_32 lval, int_32 hval)
{
	int_64 val;
	((int_32 *)&val)[0] = lval;
	((int_32 *)&val)[1] = hval;
	return val;
}

/*! @brief ʹ��ָ���ĵ�λ�͸�λ����һ�� int_64 ���͵�ֵ*/
inline int_64 i64fromu32(uint_32 lval, uint_32 hval)
{
	int_64 val;
	((uint_32 *)&val)[0] = lval;
	((uint_32 *)&val)[1] = hval;
	return val;
}

/*! @brief ��ȡ uint_64 ����ֵ�ĵ�32λ��*/
inline uint_32 u64lu32(uint_64 val)
{
	return ((uint_32 *)&val)[0];
}

/*! @brief ��ȡ uint_64 ����ֵ�ĸ�32λ��*/
inline uint_32 u64hu32(uint_64 val)
{
	return ((uint_32 *)&val)[1];
}

/*! @brief ��ȡ uint_64 ����ֵ�ĵ�32λ��*/
inline int_32 u64li32(uint_64 val)
{
	return ((int_32 *)&val)[0];
}

/*! @brief ��ȡ uint_64 ����ֵ�ĸ�32λ��*/
inline int_32 u64hi32(uint_64 val)
{
	return ((int_32 *)&val)[1];
}

/*! @brief ʹ��ָ���ĵ�λ�͸�λ����һ�� int_64 ���͵�ֵ*/
inline uint_64 u64fromu32(uint_32 lval, uint_32 hval)
{
	uint_64 val;
	((uint_32 *)&val)[0] = lval;
	((uint_32 *)&val)[1] = hval;
	return val;
}

/*! @brief ʹ��ָ���ĵ�λ�͸�λ����һ�� int_64 ���͵�ֵ*/
inline uint_64 u64fromi32(int_32 lval, int_32 hval)
{
	uint_64 val;
	((int_32 *)&val)[0] = lval;
	((int_32 *)&val)[1] = hval;
	return val;
}

/*! @brief ��ȡ int_x ����ֵ�ĵ�16/32λ��*/
inline int_h ixlih(int_x iVal)
{
#ifdef BIT64
	return i64li32(iVal);
#else
	return i32li16(iVal);
#endif
}

/*! @brief ��ȡ int_x ����ֵ�ĸ�16/32λ��*/
inline int_h ixhih(int_x val)
{
#ifdef BIT64
	return i64hi32(val);
#else
	return i32hi16(val);
#endif
}

/*! @brief ��ȡ int_x ����ֵ�ĵ�16/32λ��*/
inline uint_h ixluh(int_x iVal)
{
#ifdef BIT64
	return i64lu32(iVal);
#else
	return i32lu16(iVal);
#endif
}

/*! @brief ��ȡ int_x ����ֵ�ĸ�16/32λ��*/
inline int_h ixhuh(int_x val)
{
#ifdef BIT64
	return i64hu32(val);
#else
	return i32hu16(val);
#endif
}

/*! @brief ʹ��ָ���ĵ�λ�͸�λ����һ�� int_x ���͵�ֵ*/
inline int_x ixfromih(int_h lval, int_h hval)
{
	int_x val;
	((int_h *)&val)[0] = lval;
	((int_h *)&val)[1] = hval;
	return val;
}

/*! @brief ʹ��ָ���ĵ�λ�͸�λ����һ�� int_x ���͵�ֵ*/
inline int_x ixfromuh(uint_h lval, uint_h hval)
{
	int_x val;
	((uint_h *)&val)[0] = lval;
	((uint_h *)&val)[1] = hval;
	return val;
}

/*! @brief ��ȡ uint_x ����ֵ�ĵ�16/32λ��*/
inline uint_h uxluh(uint_x val)
{
#ifdef BIT64
	return u64lu32(val);
#else
	return u32lu16(val);
#endif
}

/*! @brief ��ȡ uint_x ����ֵ�ĸ�16/32λ��*/
inline uint_h uxhuh(uint_x val)
{
#ifdef BIT64
	return u64hu32(val);
#else
	return u32hu16(val);
#endif
}

/*! @brief ��ȡ uint_x ����ֵ�ĸ�16/32λ��*/
inline int_h uxlih(uint_x val)
{
#ifdef BIT64
	return u64li32(val);
#else
	return u32li16(val);
#endif
}

/*! @brief ��ȡ uint_x ����ֵ�ĵ�16/32λ��*/
inline int_h uxhih(uint_x val)
{
#ifdef BIT64
	return u64hi32(val);
#else
	return u32hi16(val);
#endif
}

/*! @brief ʹ��ָ���ĵ�λ�͸�λ����һ�� uint_x ���͵�ֵ*/
inline uint_x uxfromuh(uint_h lval, uint_h hval)
{
	uint_x val;
	((uint_h *)&val)[0] = lval;
	((uint_h *)&val)[1] = hval;
	return val;
}

/*! @brief ʹ��ָ���ĵ�λ�͸�λ����һ�� uint_x ���͵�ֵ*/
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


/*!@brief �ж�left�Ƿ�>right��*/
template<typename EleT>
bool bigger_then(const EleT & left, const EleT & right)
{
	return left > right;
}

/*!@brief �ж�*pLeft�Ƿ�>*pRight��*/
template<typename EleT>
bool bigger_then_ptr(const EleT * left, const EleT * right)
{
	return *left > *right;
}

/*!@brief �ж�left�Ƿ�<right��*/
template<typename EleT>
bool small_then(const EleT & left, const EleT & right)
{
	return left < right;
}

/*!@brief �ж� *left�Ƿ�С�� *right��*/
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
* ˳�����
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
* ˳�����
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
* �������
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

