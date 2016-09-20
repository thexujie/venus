#pragma once

#if defined(VENUS_MEMORY_CHYECK) && defined(_DEBUG)
#include <crtdbg.h>
#define debug_new new(_CLIENT_BLOCK, __FILE__,__LINE__)
#define new debug_new

#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define calloc(c, s) _calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define realloc(p, s) _realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define _expand(p, s) _expand_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
#define free(p) _free_dbg(p, _NORMAL_BLOCK)
#define _msize(p) _msize_dbg(p, _NORMAL_BLOCK)
#endif

#define $in // 参数用于输入
#define $out // 参数用于输出
#define $inout // 参数用于输入和输出

VENUS_BEG

#define declare_flags(name) \
constfun name operator | (name left, name right) { return (name)((int_x)left | (int_x)right);}\
constfun name operator & (name left, name right) { return (name)((int_x)left & (int_x)right); }\
constfun name operator ^ (name left, name right) { return (name)((int_x)left ^ (int_x)right);}\
constfun name operator !(name left) { return (name)(!(int_x)left);}\
constfun name operator ~(name left) { return (name)(~(int_x)left);}

/////////////////////////////////////////////////////////////////////////////////////////////////
// 类型定义

/**
* @defgroup VenusCore-Base-Type 基本类型
* @ingroup VenusCore-Base
* @brief 定义了一些常用的基本类型。
* @details 为了统一化数据类型，重新定义这些基本类型。
* @{
*/

//! 无符号的 8 位大小的字节类型，取值范围为[0, @ref UI8_MAX]。
typedef unsigned char byte_t;

//! 有符号的 8 位整数，取值范围为[@ref I8_MIN, @ref I8_MAX]。
typedef signed char int_8;
//! 有符号的 16 位整数，取值范围为[@ref I16_MIN, @ref I16_MAX]。
typedef signed short int_16;
//! 有符号的 32 位整数，取值范围为[@ref I32_MIN, @ref I32_MAX]。
typedef signed int int_32;
//! 有符号的 64 位整数，取值范围为[@ref I64_MIN, @ref I64_MAX]。
typedef signed long long int int_64;
//! 有符号的 32 位整数，同@ref int_32，取值范围为[@ref I32_MIN, @ref I32_MAX]。
typedef signed long long_32;
//! 有符号的 64 位整数，同@ref int_64，取值范围为[@ref I64_MIN, @ref I64_MAX]。
typedef signed long long long_64;

//! 无符号的 8 位整数，取值范围为[0, @ref UI8_MAX]。
typedef unsigned char uint_8;
//! 无符号的 16 位整数，取值范围为[0, @ref UI16_MAX]。
typedef unsigned short uint_16;
//! 无符号的 32 位整数，取值范围为[0, @ref UI32_MAX]。
typedef unsigned int uint_32;
//! 无符号的 64 位整数，取值范围为[0, @ref UI64_MAX]。
typedef unsigned long long int uint_64;
//! 无符号的 32 位整数，同@ref uint_32。取值范围为[0, @ref UI32_MAX]。
typedef unsigned long ulong_32;
//! 无符号的 64 位整数，同@ref uint_64。取值范围为[0, @ref UI64_MAX]。
typedef unsigned long long ulong_64;

#ifndef BIT64
//! 有符号的、等于机器位宽(32/64)的整数，取值范围为[@ref IX_MIN, @ref IX_MAX]。
typedef int_32 int_x;
//! 无符号的、等于机器位宽(32/64)的整数，取值范围为[@ref IX_MIN, @ref IX_MAX]。
typedef uint_32 uint_x;
//! 有符号的、等于机器位宽(32/64)的整数，取值范围为[@ref IX_MIN, @ref IX_MAX]。
typedef long_32 long_x;
//! 无符号的、等于机器位宽(32/64)的整数，取值范围为[@ref IX_MIN, @ref IX_MAX]。
typedef ulong_32 ulong_x;
//! 有符号的、等于机器位宽(32/64)一半的整数，取值范围为[@ref IX_MIN, @ref IX_MAX]。
typedef int_16 int_h;
//! 无符号的、等于机器位宽(32/64)一半的整数，取值范围为[@ref IX_MIN, @ref IX_MAX]。
typedef uint_16 uint_h;
//! 有符号的、等于机器位宽(32/64)一半的整数，取值范围为[@ref IX_MIN, @ref IX_MAX]。
typedef int_16 long_h;
//! 无符号的、等于机器位宽(32/64)一半的整数，取值范围为[@ref IX_MIN, @ref IX_MAX]。
typedef uint_16 ulong_h;
#else
//! 有符号的、等于机器位宽(32/64)的整数，取值范围为[@ref IX_MIN, @ref IX_MAX]。
typedef int_64 int_x;
//! 无符号的、等于机器位宽(32/64)的整数，取值范围为[@ref IX_MIN, @ref IX_MAX]。
typedef uint_64 uint_x;
//! 有符号的、等于机器位宽(32/64)的整数，取值范围为[@ref IX_MIN, @ref IX_MAX]。
typedef long_64 long_x;
//! 无符号的、等于机器位宽(32/64)的整数，取值范围为[@ref IX_MIN, @ref IX_MAX]。
typedef ulong_64 ulong_x;
//! 有符号的、等于机器位宽(32/64)一半的整数，取值范围为[@ref IX_MIN, @ref IX_MAX]。
typedef int_32 int_h;
//! 无符号的、等于机器位宽(32/64)一半的整数，取值范围为[@ref IX_MIN, @ref IX_MAX]。
typedef uint_32 uint_h;
//! 有符号的、等于机器位宽(32/64)一半的整数，取值范围为[@ref IX_MIN, @ref IX_MAX]。
typedef int_32 long_h;
//! 无符号的、等于机器位宽(32/64)一半的整数，取值范围为[@ref IX_MIN, @ref IX_MAX]。
typedef uint_32 ulong_h;
#endif

//! 错误编号的类型，err_t 等同于 @ref int_x。
typedef int_x err_t;

//! 32 位的浮点数类型，取值范围为[@ref F32_MIN, @ref F32_MAX]。
typedef float float_32;
//! 64 为的浮点数类型，取值范围为[@ref F64_MIN, @ref F64_MAX]。
typedef double float_64;

//! 8 位的有符号字符类型。
typedef char char_8;
//! 8 位的无符号字符类型。
typedef unsigned char uchar_8;
//! 16 位的有符号字符类型。
typedef wchar_t char_16;
//! 16 位的无符号字符类型。
typedef unsigned short uchar_16;
//! 32 位的有符号字符类型。
typedef int_32 char_32;
//! 32 位的无符号字符类型。
typedef uint_32 uchar_32;

#ifdef UNICODE
//! 根据平台字符集确定的有符号字符类型。参见@ref char_8 和 @ref char_16 。
typedef char_16 char_x;
//! 根据平台字符集确定的无符号字符类型。参见@ref char_8 和 @ref char_16 。
typedef uchar_16 uchar_x;
#else
//! 根据平台字符集确定的有符号字符类型。参见@ref char_8 和 @ref char_16 。
typedef char_8 char_x;
//! 根据平台字符集确定的无符号字符类型。参见@ref char_8 和 @ref char_16 。
typedef uchar_8 uchar_x;
#endif // _UNICODE

/**
* @}
*/

const char_x PCHAR_EMPTY[1] = {0};

/**
* @defgroup VenusCore-Base-Const 基本常量
* @ingroup VenusCore-Base
* @brief 定义了一些常用的基本常量。
* @details 这些常量定义了对应值类型的固定值。
* @{
*/

//! int_8 类型的最大值。
const int_8 I8_MAX = 0x7F;
//! int_8 类型的最小。
const int_8 I8_MIN = -0x7F;
//! uint_8 类型的最大值。
const uint_8 UI8_MAX = 0xFF;
//! int_16 类型的最大值。
const int_16 I16_MAX = 0x7FFF;
//! int_16 类型的最小值。
const int_16 I16_MIN = -I16_MAX;
//! uint_16 类型的最大值。
const uint_16 UI16_MAX = 0xFFFF;
//! int_32 类型的最大值。
const int_32 I32_MAX = 0x7FFFFFFF;
//! int_32 类型的最小值。
const int_32 I32_MIN = -I32_MAX;
//! uint_32 类型的最大值。
const uint_32 UI32_MAX = 0xFFFFFFFF;
//! int_64 类型的最大值。
const int_64 I64_MAX = 0x7FFFFFFFFFFFFFFF;
//! int_64 类型的最小值。
const int_64 I64_MIN = -I64_MAX;
//! uint_64 类型的最大值。
const uint_64 UI64_MAX = 0xFFFFFFFFFFFFFFFF;

#ifdef BIT64
//! @ref int_x 类型的最小值。
const int_64 IX_MIN = I64_MIN;
//! @ref int_x 类型的最大值。
const int_64 IX_MAX = I64_MAX;
//! @ref uint_x 类型的最小值。
const uint_64 UIX_MAX = UI64_MAX;
#else
//! @ref int_x 类型的最小值。
const int_32 IX_MIN = I32_MIN;
//! @ref int_x 类型的最大值。
const int_32 IX_MAX = I32_MAX;
//! @ref uint_x 类型的最小值。
const uint_32 UIX_MAX = UI32_MAX;
#endif

//! float_32 类型的最小值。
const float_32 F32_MIN = 1.175494351e-38f;
//! float_32 类型的最大值。
const float_32 F32_MAX = 3.402823466e+38f;
//! float_32 类型的最小误差。
const float_32 F32_EPSILON = 1.192092896e-07f;
//! float_32 类型的圆周率。
const float_32 F32_PI = 3.141592654f;

/**
* @}
*/

#ifdef MSVC
#if (MSVC <= MSVC_2003 )
#include "vs2003.h"
#endif
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////
// 基础错误代码
enum err_e
{
	err_ok,
	err_false,
	err_failed,
	err_nullptr,
	err_outofmemory,
	err_invalidcall,
	err_invalidarg,
	err_bounds,
	err_end,

	//! 找不到所需要的文件
	err_no_file,
	//! 上下文不满足函数调用要求
	err_no_context,
	//! 缺失必要的数据
	err_no_data,
	//! 没有对应的权限
	err_no_access,
	//! 没有实现的功能
	err_no_impl,

	//! 数据错误
	err_bad_data,
	//! 数据格式错误
	err_bad_format,
	//! 状态错误
	err_bad_state,

	//! 调用的时机错误
	err_not_now,
	//! 不支持的操作
	err_not_support,
	//! 还未初始化
	err_not_initialized,

	err_user,
};

// venus format.
enum veformat_e
{
	veformat_invalid = 0,

	veformat_r1,
	veformat_r2,
	veformat_r4,
	veformat_r8,
	veformat_r8g8b8,
	veformat_r8g8b8a8,
	veformat_r8g8b8x8,

	veformat_r10g10b10a2,
	veformat_r10g10b10x2,

	veformat_r16,
	veformat_r16f,
	veformat_r16g16,
	veformat_r16g16f,

	veformat_r24g8,
	veformat_r24g8f,

	veformat_r32,
	veformat_r32f,
	veformat_r32g32,
	veformat_r32g32f,
	veformat_r32g32b32,
	veformat_r32g32b32f,
	veformat_r32g32b32a32,
	veformat_r32g32b32x32,
	veformat_r32g32b32a32f,
	veformat_r32g32b32x32f,
};


/**
* @defgroup VenusCore-Base-Tmpl 基本模板
* @ingroup VenusCore-Base
* @brief 定义了一些基本的模板（函数、类等）
* @details 这些模板函数提供了完成基本功能所需的支持。
* @{
*/


#pragma push_macro("new")
#undef new
/**
* @brief 一个简单的内存管理类。
* @details 分配、释放内存，构造、析构对象。
*/
template<class ValT> class DefEleAllocator
{
public:
	/**
	* @brief 创建一个实例对象指针。
	* @return 返回对象的一个实例指针。
	*/
	static ValT * Malloc()
	{
		return (ValT *)malloc(sizeof(ValT));
	}
	/**
	* @brief 创建一个实例对象数组（指针）。
	* @param iCount 指定的数组大小。
	* @return 返回对象的一个实例数组（指针）。
	*/
	static ValT * Malloc(int_x iCount)
	{
		return (ValT *)malloc(iCount * sizeof(ValT));
	}
	/**
	* @brief 释放一个实例对象。
	* @param data 实例对象的指针。
	*/
	static void Free(ValT * data)
	{
		free((void *)data);
	}
	/**
	* @brief 使用一个实例构造另外一个实例。
	* @param pData 将要被构造的实例指针。
	* @param data 构造的副本。
	* @note 该函数对 基本数据类型 也有作用。
	*/
	static void Construct(ValT * pData, const ValT & data)
	{
		new ((void *)pData) ValT(data);
	}
	/**
	* @brief 使用一个实例构造另外一个实例。
	* @param pData 将要被构造的实例指针。
	* @param data 构造的副本。
	* @note 该函数对 基本数据类型 也有作用。
	*/
	static void MoveConstruct(ValT * pData, ValT & data)
	{
		new ((void *)pData) ValT(move(data));
	}
	/**
	* @brief 调用实例的构造函数。
	* @param pData 将要被构造的实例指针。
	* @param data 构造的副本。
	* @note 该函数对 基本数据类型 也有作用。
	*/
	static void Construct(ValT * pData)
	{
		new ((void *)pData) ValT();
	}
	/**
	* @brief 析构一个实例对象。
	* @param pData 将要被析构的实例指针。
	* @note 该函数对 基本数据类型 也有作用。
	*/
	static void Destruct(ValT * pData)
	{
		pData->~ValT();
	}
};

#pragma pop_macro("new")
/**
* @}//VenusCore-Base-Tmpl
*/

union oid_t
{
	char_16 name[32];
	uint_32 value[16];
	bool operator == (const oid_t & another) const
	{
		for(int_x cnt = 0; cnt < 16; ++cnt)
		{
			if(value[cnt] != another.value[cnt])
				return false;
		}
		return true;
	}
	bool operator != (const oid_t & another) const { return !operator==(another); }
};

class CORE_API IObject
{
public:
	virtual ~IObject() {}
	virtual int_x AddRef() = 0;
	virtual int_x Release() = 0;
	virtual int_x GetRef() const = 0;
	virtual const oid_t & GetOid() const = 0;
	virtual IObject * GetInterface(const oid_t & oid) = 0;
};

const oid_t OID_Object = {L"Object"};
template<typename ...Args>
class ObjectT : public Args...
{
public:
	ObjectT() :m_iRef(1) {}
	~ObjectT() {}

	int_x AddRef() override { return ++m_iRef; }
	int_x Release() override
	{
		Assert(m_iRef);
		int_x iRef = --m_iRef;
		if(!m_iRef)
			delete this;
		return iRef;
	}
	int_x GetRef() const override { return m_iRef; }
	const oid_t & GetOid() const { return OID_Object; }
	IObject * GetInterface(const oid_t & oid) { return nullptr; }

protected:
	int_x m_iRef;
};

struct version_t
{
	int_x major;
	int_x minor;
	int_x revision;
	int_x build;

	version_t() : major(0), minor(0), revision(0), build(0){}

	version_t(int_x _major, int_x _minor, int_x _revision, int_x _build)
		:major(_major), minor(_minor), revision(_revision), build(_build)
	{
	}

	bool operator >= (const version_t & another) const { return compare(another) >= 0; }
	bool operator > (const version_t & another) const { return compare(another) > 0; }
	bool operator == (const version_t & another) const { return compare(another) == 0; }
	bool operator != (const version_t & another) const { return compare(another) != 0; }
	bool operator < (const version_t & another) const { return compare(another) < 0; }
	bool operator <= (const version_t & another) const { return compare(another) <= 0; }

	int_x compare(const version_t & another) const
	{
		int_x result = 0;
		if(major > another.major) result += 1000;
		else if(major < another.major) result += -1000;
		else {}

		if(minor > another.minor) result += 100;
		else if(minor < another.minor) result += -100;
		else {}

		if(revision > another.revision) result += 10;
		else if(revision < another.revision) result += -10;
		else {}

		if(build > another.build) result += 1;
		else if(build < another.build) result += -1;
		else {}

		return result;
	}
};

enum FileBomE
{
	FileBomInvalid = 0,
	FileBomAnsi,
	FileBomUtf8,
	FileBomUtf16SmallEdian,
	FileBomUtf16BigEdian,
};

enum linetag_e
{
	linetag_none,
	linetag_0,
	linetag_r,
	linetag_n,
	linetag_rn,
};

enum EndianE
{
	EndianSmall = 0,
	EndianBig,
};

VENUS_END
