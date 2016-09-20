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

#define $in // ������������
#define $out // �����������
#define $inout // ����������������

VENUS_BEG

#define declare_flags(name) \
constfun name operator | (name left, name right) { return (name)((int_x)left | (int_x)right);}\
constfun name operator & (name left, name right) { return (name)((int_x)left & (int_x)right); }\
constfun name operator ^ (name left, name right) { return (name)((int_x)left ^ (int_x)right);}\
constfun name operator !(name left) { return (name)(!(int_x)left);}\
constfun name operator ~(name left) { return (name)(~(int_x)left);}

/////////////////////////////////////////////////////////////////////////////////////////////////
// ���Ͷ���

/**
* @defgroup VenusCore-Base-Type ��������
* @ingroup VenusCore-Base
* @brief ������һЩ���õĻ������͡�
* @details Ϊ��ͳһ���������ͣ����¶�����Щ�������͡�
* @{
*/

//! �޷��ŵ� 8 λ��С���ֽ����ͣ�ȡֵ��ΧΪ[0, @ref UI8_MAX]��
typedef unsigned char byte_t;

//! �з��ŵ� 8 λ������ȡֵ��ΧΪ[@ref I8_MIN, @ref I8_MAX]��
typedef signed char int_8;
//! �з��ŵ� 16 λ������ȡֵ��ΧΪ[@ref I16_MIN, @ref I16_MAX]��
typedef signed short int_16;
//! �з��ŵ� 32 λ������ȡֵ��ΧΪ[@ref I32_MIN, @ref I32_MAX]��
typedef signed int int_32;
//! �з��ŵ� 64 λ������ȡֵ��ΧΪ[@ref I64_MIN, @ref I64_MAX]��
typedef signed long long int int_64;
//! �з��ŵ� 32 λ������ͬ@ref int_32��ȡֵ��ΧΪ[@ref I32_MIN, @ref I32_MAX]��
typedef signed long long_32;
//! �з��ŵ� 64 λ������ͬ@ref int_64��ȡֵ��ΧΪ[@ref I64_MIN, @ref I64_MAX]��
typedef signed long long long_64;

//! �޷��ŵ� 8 λ������ȡֵ��ΧΪ[0, @ref UI8_MAX]��
typedef unsigned char uint_8;
//! �޷��ŵ� 16 λ������ȡֵ��ΧΪ[0, @ref UI16_MAX]��
typedef unsigned short uint_16;
//! �޷��ŵ� 32 λ������ȡֵ��ΧΪ[0, @ref UI32_MAX]��
typedef unsigned int uint_32;
//! �޷��ŵ� 64 λ������ȡֵ��ΧΪ[0, @ref UI64_MAX]��
typedef unsigned long long int uint_64;
//! �޷��ŵ� 32 λ������ͬ@ref uint_32��ȡֵ��ΧΪ[0, @ref UI32_MAX]��
typedef unsigned long ulong_32;
//! �޷��ŵ� 64 λ������ͬ@ref uint_64��ȡֵ��ΧΪ[0, @ref UI64_MAX]��
typedef unsigned long long ulong_64;

#ifndef BIT64
//! �з��ŵġ����ڻ���λ��(32/64)��������ȡֵ��ΧΪ[@ref IX_MIN, @ref IX_MAX]��
typedef int_32 int_x;
//! �޷��ŵġ����ڻ���λ��(32/64)��������ȡֵ��ΧΪ[@ref IX_MIN, @ref IX_MAX]��
typedef uint_32 uint_x;
//! �з��ŵġ����ڻ���λ��(32/64)��������ȡֵ��ΧΪ[@ref IX_MIN, @ref IX_MAX]��
typedef long_32 long_x;
//! �޷��ŵġ����ڻ���λ��(32/64)��������ȡֵ��ΧΪ[@ref IX_MIN, @ref IX_MAX]��
typedef ulong_32 ulong_x;
//! �з��ŵġ����ڻ���λ��(32/64)һ���������ȡֵ��ΧΪ[@ref IX_MIN, @ref IX_MAX]��
typedef int_16 int_h;
//! �޷��ŵġ����ڻ���λ��(32/64)һ���������ȡֵ��ΧΪ[@ref IX_MIN, @ref IX_MAX]��
typedef uint_16 uint_h;
//! �з��ŵġ����ڻ���λ��(32/64)һ���������ȡֵ��ΧΪ[@ref IX_MIN, @ref IX_MAX]��
typedef int_16 long_h;
//! �޷��ŵġ����ڻ���λ��(32/64)һ���������ȡֵ��ΧΪ[@ref IX_MIN, @ref IX_MAX]��
typedef uint_16 ulong_h;
#else
//! �з��ŵġ����ڻ���λ��(32/64)��������ȡֵ��ΧΪ[@ref IX_MIN, @ref IX_MAX]��
typedef int_64 int_x;
//! �޷��ŵġ����ڻ���λ��(32/64)��������ȡֵ��ΧΪ[@ref IX_MIN, @ref IX_MAX]��
typedef uint_64 uint_x;
//! �з��ŵġ����ڻ���λ��(32/64)��������ȡֵ��ΧΪ[@ref IX_MIN, @ref IX_MAX]��
typedef long_64 long_x;
//! �޷��ŵġ����ڻ���λ��(32/64)��������ȡֵ��ΧΪ[@ref IX_MIN, @ref IX_MAX]��
typedef ulong_64 ulong_x;
//! �з��ŵġ����ڻ���λ��(32/64)һ���������ȡֵ��ΧΪ[@ref IX_MIN, @ref IX_MAX]��
typedef int_32 int_h;
//! �޷��ŵġ����ڻ���λ��(32/64)һ���������ȡֵ��ΧΪ[@ref IX_MIN, @ref IX_MAX]��
typedef uint_32 uint_h;
//! �з��ŵġ����ڻ���λ��(32/64)һ���������ȡֵ��ΧΪ[@ref IX_MIN, @ref IX_MAX]��
typedef int_32 long_h;
//! �޷��ŵġ����ڻ���λ��(32/64)һ���������ȡֵ��ΧΪ[@ref IX_MIN, @ref IX_MAX]��
typedef uint_32 ulong_h;
#endif

//! �����ŵ����ͣ�err_t ��ͬ�� @ref int_x��
typedef int_x err_t;

//! 32 λ�ĸ��������ͣ�ȡֵ��ΧΪ[@ref F32_MIN, @ref F32_MAX]��
typedef float float_32;
//! 64 Ϊ�ĸ��������ͣ�ȡֵ��ΧΪ[@ref F64_MIN, @ref F64_MAX]��
typedef double float_64;

//! 8 λ���з����ַ����͡�
typedef char char_8;
//! 8 λ���޷����ַ����͡�
typedef unsigned char uchar_8;
//! 16 λ���з����ַ����͡�
typedef wchar_t char_16;
//! 16 λ���޷����ַ����͡�
typedef unsigned short uchar_16;
//! 32 λ���з����ַ����͡�
typedef int_32 char_32;
//! 32 λ���޷����ַ����͡�
typedef uint_32 uchar_32;

#ifdef UNICODE
//! ����ƽ̨�ַ���ȷ�����з����ַ����͡��μ�@ref char_8 �� @ref char_16 ��
typedef char_16 char_x;
//! ����ƽ̨�ַ���ȷ�����޷����ַ����͡��μ�@ref char_8 �� @ref char_16 ��
typedef uchar_16 uchar_x;
#else
//! ����ƽ̨�ַ���ȷ�����з����ַ����͡��μ�@ref char_8 �� @ref char_16 ��
typedef char_8 char_x;
//! ����ƽ̨�ַ���ȷ�����޷����ַ����͡��μ�@ref char_8 �� @ref char_16 ��
typedef uchar_8 uchar_x;
#endif // _UNICODE

/**
* @}
*/

const char_x PCHAR_EMPTY[1] = {0};

/**
* @defgroup VenusCore-Base-Const ��������
* @ingroup VenusCore-Base
* @brief ������һЩ���õĻ���������
* @details ��Щ���������˶�Ӧֵ���͵Ĺ̶�ֵ��
* @{
*/

//! int_8 ���͵����ֵ��
const int_8 I8_MAX = 0x7F;
//! int_8 ���͵���С��
const int_8 I8_MIN = -0x7F;
//! uint_8 ���͵����ֵ��
const uint_8 UI8_MAX = 0xFF;
//! int_16 ���͵����ֵ��
const int_16 I16_MAX = 0x7FFF;
//! int_16 ���͵���Сֵ��
const int_16 I16_MIN = -I16_MAX;
//! uint_16 ���͵����ֵ��
const uint_16 UI16_MAX = 0xFFFF;
//! int_32 ���͵����ֵ��
const int_32 I32_MAX = 0x7FFFFFFF;
//! int_32 ���͵���Сֵ��
const int_32 I32_MIN = -I32_MAX;
//! uint_32 ���͵����ֵ��
const uint_32 UI32_MAX = 0xFFFFFFFF;
//! int_64 ���͵����ֵ��
const int_64 I64_MAX = 0x7FFFFFFFFFFFFFFF;
//! int_64 ���͵���Сֵ��
const int_64 I64_MIN = -I64_MAX;
//! uint_64 ���͵����ֵ��
const uint_64 UI64_MAX = 0xFFFFFFFFFFFFFFFF;

#ifdef BIT64
//! @ref int_x ���͵���Сֵ��
const int_64 IX_MIN = I64_MIN;
//! @ref int_x ���͵����ֵ��
const int_64 IX_MAX = I64_MAX;
//! @ref uint_x ���͵���Сֵ��
const uint_64 UIX_MAX = UI64_MAX;
#else
//! @ref int_x ���͵���Сֵ��
const int_32 IX_MIN = I32_MIN;
//! @ref int_x ���͵����ֵ��
const int_32 IX_MAX = I32_MAX;
//! @ref uint_x ���͵���Сֵ��
const uint_32 UIX_MAX = UI32_MAX;
#endif

//! float_32 ���͵���Сֵ��
const float_32 F32_MIN = 1.175494351e-38f;
//! float_32 ���͵����ֵ��
const float_32 F32_MAX = 3.402823466e+38f;
//! float_32 ���͵���С��
const float_32 F32_EPSILON = 1.192092896e-07f;
//! float_32 ���͵�Բ���ʡ�
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
// �����������
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

	//! �Ҳ�������Ҫ���ļ�
	err_no_file,
	//! �����Ĳ����㺯������Ҫ��
	err_no_context,
	//! ȱʧ��Ҫ������
	err_no_data,
	//! û�ж�Ӧ��Ȩ��
	err_no_access,
	//! û��ʵ�ֵĹ���
	err_no_impl,

	//! ���ݴ���
	err_bad_data,
	//! ���ݸ�ʽ����
	err_bad_format,
	//! ״̬����
	err_bad_state,

	//! ���õ�ʱ������
	err_not_now,
	//! ��֧�ֵĲ���
	err_not_support,
	//! ��δ��ʼ��
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
* @defgroup VenusCore-Base-Tmpl ����ģ��
* @ingroup VenusCore-Base
* @brief ������һЩ������ģ�壨��������ȣ�
* @details ��Щģ�庯���ṩ����ɻ������������֧�֡�
* @{
*/


#pragma push_macro("new")
#undef new
/**
* @brief һ���򵥵��ڴ�����ࡣ
* @details ���䡢�ͷ��ڴ棬���졢��������
*/
template<class ValT> class DefEleAllocator
{
public:
	/**
	* @brief ����һ��ʵ������ָ�롣
	* @return ���ض����һ��ʵ��ָ�롣
	*/
	static ValT * Malloc()
	{
		return (ValT *)malloc(sizeof(ValT));
	}
	/**
	* @brief ����һ��ʵ���������飨ָ�룩��
	* @param iCount ָ���������С��
	* @return ���ض����һ��ʵ�����飨ָ�룩��
	*/
	static ValT * Malloc(int_x iCount)
	{
		return (ValT *)malloc(iCount * sizeof(ValT));
	}
	/**
	* @brief �ͷ�һ��ʵ������
	* @param data ʵ�������ָ�롣
	*/
	static void Free(ValT * data)
	{
		free((void *)data);
	}
	/**
	* @brief ʹ��һ��ʵ����������һ��ʵ����
	* @param pData ��Ҫ�������ʵ��ָ�롣
	* @param data ����ĸ�����
	* @note �ú����� ������������ Ҳ�����á�
	*/
	static void Construct(ValT * pData, const ValT & data)
	{
		new ((void *)pData) ValT(data);
	}
	/**
	* @brief ʹ��һ��ʵ����������һ��ʵ����
	* @param pData ��Ҫ�������ʵ��ָ�롣
	* @param data ����ĸ�����
	* @note �ú����� ������������ Ҳ�����á�
	*/
	static void MoveConstruct(ValT * pData, ValT & data)
	{
		new ((void *)pData) ValT(move(data));
	}
	/**
	* @brief ����ʵ���Ĺ��캯����
	* @param pData ��Ҫ�������ʵ��ָ�롣
	* @param data ����ĸ�����
	* @note �ú����� ������������ Ҳ�����á�
	*/
	static void Construct(ValT * pData)
	{
		new ((void *)pData) ValT();
	}
	/**
	* @brief ����һ��ʵ������
	* @param pData ��Ҫ��������ʵ��ָ�롣
	* @note �ú����� ������������ Ҳ�����á�
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
