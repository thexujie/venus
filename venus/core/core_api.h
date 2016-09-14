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
CORE_API void log0(const char_16 * format, ...);
CORE_API void log0(const char_8 * format, ...);
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
template<typename FunT>
void Assert(const FunT & val)
{
#ifdef _DEBUG
	if(val == 0)
		breakpoint();
#endif
}
/*!@brief 断言，如果失败，会触发一个调试断点。非Debug版该函数也会起作用。*/
template<typename FunT>
void Verify(const FunT & val)
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
template<typename FunT>
void SafeDelete(FunT *& ptr)
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
template<typename FunT>
void SafeDeleteArray(FunT *& ptr)
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
template<typename FunT>
void SafeDeleteArray(FunT * ptr[], int_x iLength)
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
template<typename FunT>
void SafeRelease(FunT *& ptr)
{
	if(ptr)
	{
		ptr->Release();
		ptr = nullptr;
	}
}

template<typename FunT>
void SafeReleasePtr(FunT ** ptr)
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
template<typename FunT>
void SafeNull(FunT *& ptr)
{
	ptr = nullptr;
}

/**
* @brief 安全地 调用 Release 一个数组，并将数组中每个指针赋 nullptr。
* @param ptr 需要被 Release 的指针的引用。
*/
template<typename FunT>
void SafeReleaseArray(FunT ** pPtr, int_x iLength)
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
template<typename FunT>
void SafeAddRef(FunT * const & ptr)
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
template<typename FunT>
void FakeAddRef(FunT *& ptr)
{

}

/**
* @brief 不调用 Release，仅赋空，部分情况下，防止造成循环引用，
*        在确信子对象生命周期短与父对象时，不减少父对象
*        的引用计数
* @param ptr 目标指针
*/
template<typename FunT>
void FakeRelease(FunT *& ptr)
{
	if(ptr)
		ptr = nullptr;
}

template<typename FunT>
void SafeAddRefPtr(FunT ** ptr)
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
template<typename FunT>
constfun const FunT & minof(const FunT & a, const FunT & b)
{
	return a > b ? b : a;
}

/**
* @brief 求若干个数中较小的一个。
* @param params,v 要比较的数
* @return 返回较小的一个数的 const 引用。
*/
template<typename ...ParamsT, typename FunT>
constfun const FunT & minof(const FunT & a, const FunT & b, const ParamsT & ...params)
{
	return minof(minof(a, b), params...);
}

/**
* @brief 求两个数种较大的一个。
* @param a,b 要比较的两个数的 const 引用。
* @return 返回较大的一个数的 const 引用。
*/
template<typename FunT>
constfun const FunT & maxof(const FunT & a, const FunT & b)
{
	return a > b ? a : b;
}

/**
* @brief 求若干个数中较大的一个。
* @param params,v 要比较的数
* @return 返回较大的一个数的 const 引用。
*/
template<typename ...ParamsT, typename FunT>
constfun const FunT & maxof(const FunT & a, const FunT & b, const ParamsT & ...params)
{
	return maxof(maxof(a, b), params...);
}


template<typename FunT>
constfun const FunT & saturate(const FunT & value, const FunT & min, const FunT & max)
{
	return value < min ? value : (value > max ? max : value);
}
/**
* @brief 交换两个数字的值。
* @param a,b 要交换的两个数的 const 引用。
*/
template<typename FunT>
void swap(FunT & a, FunT & b)
{
	FunT tmp = a; a = b; b = tmp;
}
/**
* @brief 交换两个指针所指向的数据的值。
* @param a,b 要交换的两个数的 const 引用。
*/
template<typename FunT>
void swap_ptr(FunT * pA, FunT * pB)
{
	FunT temp = *pA; *pA = *pB; *pB = temp;
}

/**
* @brief 求数字的绝对值。
* @param val 要求绝对值得数。
* @return 返回输入数字的绝对值。
* @warning 对无符号数调用 Abs 没有意义，并且会触发编译器警告。
*/
template<typename FunT>
FunT abs(const FunT & val)
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
#ifdef BIT_64
	return i64li32(iVal);
#else
	return i32li16(iVal);
#endif
}

/*! @brief 获取 int_x 类型值的高16/32位。*/
inline int_h ixhih(int_x val)
{
#ifdef BIT_64
	return i64hi32(val);
#else
	return i32hi16(val);
#endif
}

/*! @brief 获取 int_x 类型值的低16/32位。*/
inline uint_h ixluh(int_x iVal)
{
#ifdef BIT_64
	return i64lu32(iVal);
#else
	return i32lu16(iVal);
#endif
}

/*! @brief 获取 int_x 类型值的高16/32位。*/
inline int_h ixhuh(int_x val)
{
#ifdef BIT_64
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
#ifdef BIT_64
	return u64lu32(val);
#else
	return u32lu16(val);
#endif
}

/*! @brief 获取 uint_x 类型值的高16/32位。*/
inline uint_h uxhuh(uint_x val)
{
#ifdef BIT_64
	return u64hu32(val);
#else
	return u32hu16(val);
#endif
}

/*! @brief 获取 uint_x 类型值的高16/32位。*/
inline int_h uxlih(uint_x val)
{
#ifdef BIT_64
	return u64li32(val);
#else
	return u32li16(val);
#endif
}

/*! @brief 获取 uint_x 类型值的低16/32位。*/
inline int_h uxhih(uint_x val)
{
#ifdef BIT_64
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

#pragma endregion

#pragma region buff
inline void buffcpy8(void * dst, const void * src, int_x count)
{
	if(count > 0)
		memcpy_s(dst, count, src, count);
}

inline void buffcpy8(void * dst, int_x size, const void * src, int_x count)
{
	buffcpy8(dst, src, minof(size, count));
}

inline void buffcpy16(void * dst, const void * src, int_x count)
{
	buffcpy8(dst, src, count * 2);
}

inline void buffcpy16(void * dst, int_x size, const void * src, int_x count)
{
	buffcpy8(dst, src, minof(size, count));
}

inline void buffcpy32(void * dst, const void * src, int_x count)
{
	buffcpy8(dst, src, count * 4);
}

inline void buffcpy32(void * dst, int_x size, const void * src, int_x count)
{
	buffcpy32(dst, src, minof(size, count));
}

inline void buffmov8(void * dst, const void * src, int_x count)
{
	if(dst && src && count > 0 && dst != src)
		memmove_s(dst, count, src, count);
}

inline void buffmov8(void * dst, int_x size, const void * src, int_x count)
{
	buffmov8(dst, src, minof(size, count));
}

/*!@brief 将 src 中的 size 个 uint_8 赋值为 val。*/
inline void buffset8(void * dst, int_x count, uint_8 val)
{
	if(count > 0)
		memset(dst, val, count);
}

/*!@brief 将 src 中的 size 个 uint_16 赋值为 val。*/
inline void buffset16(void * dst, int_x count, uint_16 val)
{
	uint_16 * u16 = (uint_16 *)dst;
	for(int_x cnt = 0; cnt < count; ++cnt)
		u16[cnt] = val;
}

/*!@brief 将 src 中的 size 个 uint_32 赋值为val。*/
inline void buffset32(void * dst, int_x count, uint_32 val)
{
	uint_32 * u32 = (uint_32 *)dst;
	for(int_x cnt = 0; cnt < count; ++cnt)
		u32[cnt] = val;
}

/*!@brief 将 src 中的 count 个 uint_64 赋值为val。*/
inline void buffset64(void * dst, int_x count, uint_64 val)
{
	uint_64 * u64 = (uint_64 *)dst;
	for(int_x cnt = 0; cnt < count; ++cnt)
		u64[cnt] = val;
}

/*!@brief 比较 src 和 dst 中的前 count 个字节。*/
inline int_x buffcmp8(void * dst, const void * src, int_x count)
{
	if(count > 0)
		return memcmp(dst, src, count);
	else
		return 0;
}

/**!@brief 将 val 按字节重置为0。*/
template<typename FunT>
inline void buffclr(FunT & val)
{
	buffset8(&val, sizeof(FunT), 0);
}

template<typename FunT>
inline void buffset(FunT * dst, int_x size, const FunT & val)
{
	for(int_x cnt = 0; cnt < size; ++cnt)
		dst[cnt] = val;
}

/*!brief 复制 将 src 按字节复制到 dst。*/
template<typename FunT>
inline void buffcpy(FunT & dst, const FunT & src)
{
	buffcpy8(&dst, &src, sizeof(FunT));
}

template<typename FunT>
inline void buffcpy(FunT * dst, const FunT * src, int_x count)
{
	buffcpy8(dst, src, sizeof(FunT) * count);
}

/*!brief 复制 将 count 个 src<FunT> 按字节复制到 dst。*/
template<typename FunT>
inline void buffcpy(FunT * dst, int_x size, const FunT * src, int_x count)
{
	buffcpy(dst, src, minof(size, count));
}

/*!@brief 移动 将 src 中的 count 个字节到容量为 count 字节的 count 中。*/
template<typename FunT>
inline void buffmov(FunT * dst, const FunT * src, int_x count)
{
	buffmov8(dst, src, count * sizeof(FunT));
}

/**
* @brief 以二进制的形式，将指定位置、指定长度的数组以移动的方式复制到目标位置。
* @param pDst 目标位置。
* @param iSize 目标容量。
* @param pSrc 源数组。
* @param iCount 要复制的数据的个数。
*/
template<typename EleT>
void buffmov(EleT * dst, int_x size, const EleT * src, int_x count)
{
	buffmov(dst, src, minof(size, count));
}

#pragma endregion // buff

#pragma region array
/**
* @defgroup VenusCore-Base-Array 数组工具
* @ingroup VenusCore-Until
* @brief 提供了一些常用的数组操作。
* @{
*/

/*
 * !@brief 计算固定数组的长度
 */
template<typename EleT, int_x LENGTH>
constfun int_x arraysize(const EleT(&arr)[LENGTH])
{
	return LENGTH;
}

/*!@brief 对长度为 length 数组 arr 进行赋值。*/
template<typename EleT>
void arrayset(EleT * arr, int_x length, ...)
{
	EleT * value = (EleT *)(&length + 1);
	for(int_x cnt = 0; cnt < length; ++cnt)
	{
		arr[cnt] = *value;
		++value;
	}
}

/*!@brief 将pArray中的iLength个EleT赋值为val。*/
template<typename EleT>
void arrayfill(EleT * dst, int_x size, const EleT & val)
{
	for(int_x cnt = 0; cnt < size; ++cnt)
		dst[cnt] = val;
}

/*!@brief 将pArray中的iLength个EleT赋值为val。*/
template<typename EleT, int_x LENGTH>
void arrayfill(EleT(&dst)[LENGTH], const EleT & val)
{
	arrayfill(dst, LENGTH, val);
}

/*!@brief 复制 src 中的 count 歌 EleT 到容量为 size 个 EleT 的 dst 中。*/
template<typename EleT>
void arraycpy(EleT * dst, const EleT * src, int_x count)
{
	for(int_x cnt = 0; cnt < count; ++cnt)
		dst[cnt] = src[cnt];
}

/*!@brief 复制 src 中的 count 歌 EleT 到容量为 size 个 EleT 的 dst 中。*/
template<typename EleT>
void arraycpy(EleT * dst, int_x size, const EleT * src, int_x count)
{
	arraycpy(dst, src, minof(size, count));
}

/*!@brief 复制 src 中的 count 歌 EleT 到容量为 size 个 EleT 的 dst 中。*/
template<typename EleT, int_x LENGTH>
void arraycpy(EleT(&dst)[LENGTH], const EleT(&src)[LENGTH])
{
	arraycpy<EleT>(dst, src, LENGTH);
}

template<typename EleT>
void arraymov(EleT * dst, const EleT * src, int_x count)
{
	if(dst == src)
		return;
	for(int_x cnt = 0; cnt < count; ++cnt)
		dst[cnt] = src[cnt];
}

/**
* @brief 将指定位置、指定长度的数组以移动的方式复制到目标位置。
* @param pDst 目标位置。
* @param iSize 目标容量。
* @param pSrc 源数组。
* @param iCount 要复制的数据的个数。
*/
template<typename EleT>
void arraymov(EleT * dst, int_x size, const EleT * src, int_x count)
{
	if(dst == src)
		return;
	if(size >= count)
	{
		for(int_x cnt = 0; cnt < count; ++cnt)
			dst[cnt] = src[cnt];
	}
}

/*!@brief 将长度为 length 的数组 arr 中从 index 索引开始的共 count 个元素删除。*/
template<typename EleT>
void arrayremove(EleT * arr, int_x length, int_x index, int_x count)
{
	if(index >= 0 && index < length &&
	   count > 0 && index + count <= length)
	{
		arraymov(arr + index, length - index, arr + index + count, length - (index + count));
	}
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

/**
* @brief 对数组中的元素进行插入排序，排序的依据是pfnCmp。
* @param arr 被排序的数组的第一个元素的指针。
* @param length 被排序的数组的长度。
* @param pfnCmp 比较规则，可以是函数、Lamda表达式和重载了operator(left, right)的类，比较规则
*  的返回值决定了left 在数组中的位置是否应该在right的前面。
*/
template<typename EleT, typename CmpFuncT>
void arraysortinsertex(EleT * arr, int_x length, CmpFuncT pfnCmp)
{
	if(!arr || length <= 0 || !pfnCmp)
		return;

	const EleT * end = arr + length;
	int_x count = end - arr;
	if(count == 2)
	{
		EleT * curr = arr + 1;
		if(pfnCmp(*curr, *arr))
		{
			EleT ele = *arr;
			*arr = *curr;
			*curr = ele;
		}
	}
	else if(count > 2)
	{
		EleT ele;
		EleT * insert = 0;
		for(EleT * curr = arr + 1; curr != end; ++curr)
		{
			ele = *curr;
			if(pfnCmp(ele, *arr))
			{
				buffmov(arr + 1, arr, curr - arr);
				*arr = ele;
			}
			else
			{
				insert = curr - 1;
				if(pfnCmp(ele, *insert))
				{
					while(pfnCmp(ele, *insert))
					{
						*(insert + 1) = *insert;
						--insert;
					}
					*(insert + 1) = ele;
				}
			}

		}
	}
	else {}
}

/**
* @brief 对数组中的元素进行排序，排序的依据是比较函数CmpFunc。
* @param arr 被排序的数组的第一个元素的指针。
* @param length 被排序的数组的长度。
* @param pfnCmp 比较函数，函数的返回值决定了left 在数组中的位置是否应该在right的前面。
*  默认的比较函数是小于 small_then，即数组中的元素是按照从小到大的顺序排列。
* @see ArraySortInsertEx
*/
template<typename EleT>
void arraysortinsert(EleT * arr, int_x length)
{
	arraysortinsertex(arr, length, small_then<EleT>);
}

/**
* @brief 线性查找指定范围内的元素，使用 == 来匹配元素。
* @param begin 查找起始点。
* @param end 查找终点。
* @return 返回查找停止位置。
*/
template<typename IterT, typename ValT>
IterT find(IterT begin, IterT end, const ValT & val)
{
	for(; begin != end; ++begin)
	{
		if(*begin == val)
			break;
	}
	return begin;
};

/**
* @brief 线性查找指定范围内的元素，使用 match 来匹配元素。
* @param begin 查找起始点。
* @param end 查找终点。
* @param match 匹配规则。
* @return 返回查找停止位置。
*/
template<typename IterT, typename MatchT>
IterT find_match(IterT begin, IterT end, MatchT match)
{
	for(; begin != end; ++begin)
	{
		if(match(*begin))
			break;
	}
	return begin;
};

#pragma endregion

#pragma region text

template<typename CharT>
void textempty(CharT * text, int_x size = -1)
{
	if(text && size)
		text[0] = 0;
}

template<typename CharT>
bool textisempty(CharT * text, int_x size = -1)
{
	return !size || !text || !text[0];
}

/**
* @brief 计算文本的长度。
* @param pText 要计算长度的文本。
* @param iSize 容纳文本的缓冲区大小，即文本最大长度为 iSize - 1。默认为-1。
*              如果为负数，则计算过程不考虑文本缓冲区大小。。
* @return 返回计算的文本的长度。
*/
template<typename CharT>
int_x textlen(const CharT * text, int_x size = -1)
{
	if(!text || !size)
		return 0;

	int_x len = 0;
	while(*text++ && --size)
		++len;
	return len;
}

/**
* @brief 将源字符窜的指定长度复制到目标字符串，并添加终止符(除非目标字符串大小为0)。
* @param szText 目标字符串。
* @param iSize 目标字符串的大小，包括终止符，它就是目标字符串的容量。
* @param szSrc 源字符串。
* @param iLength 要拷贝的字符的个数，不包括终止符，如果为负数，则拷贝直至终止符为止。
*                就算 iLength 指定的长度大于 pSrc 的真实长度，那也只会拷贝直至终止符为止。
* @return 返回拷贝的字符的个数，不包括终止符。
*/
template<typename CharT>
int_x textcpy(CharT * dst, int_x size, const CharT * src, int_x length = -1)
{
	if(!size)
		return 0;
	else if(size == 1)
	{
		*dst = 0;
		return 0;
	}
	else
	{
		if(length < 0)
			length = minof(textlen(src), size - 1);
		else
		{
			int_x lengh_max = textlen(src, length + 1);
			if(length > lengh_max)
				length = minof(lengh_max, size - 1);
			else
				length = minof(length, size - 1);
		}


		if(length)
			buffcpy(dst, src, minof(size, length));
		dst[length] = 0;
		return length;
	}
}

inline int_x textcpyw(char_16 * dst, int_x size, const char_16 * src, int_x length = -1)
{
	return textcpy<char_16>(dst, size, src, length);
}

inline int_x textcpya(char_8 * dst, int_x size, const char_8 * src, int_x length = -1)
{
	return textcpy<char_8>(dst, size, src, length);
}

inline int_x textcpyx(char_x * dst, int_x size, const char_x * src, int_x length = -1)
{
	return textcpy<char_x>(dst, size, src, length);
}


template<typename CharT>
CharT chlower(CharT ch)
{
	if('A' <= ch && ch <= 'Z')
		return 'a' + (ch - 'A');
	else
		return ch;
}
template<typename CharT>
CharT chupper(CharT ch)
{
	if('a' <= ch && ch <= 'a')
		return 'A' + (ch - 'a');
	else
		return ch;
}

/**
* @brief 将源字符串的指定长度里的大写字母转换为小写。
* @param szText 目标字符串。
* @param iLength 要转换的长度，不包括终止符。
* @return 被转换的字符的个数，不包括终止符。
*/
template<typename CharT>
int_x textlower(CharT * text, int_x length = -1)
{
	if(length < 0)
		length = textlen(text);

	int_x iCount = 0;
	for(int_x cnt = 0; cnt < length; ++cnt)
	{
		CharT & ch = text[cnt];
		if('A' <= ch && ch <= 'Z')
		{
			ch = 'a' + (ch - 'A');
			++iCount;
		}
	}
	return iCount;
}

/**
* @brief 将源字符串的指定长度里的大写字母转换为大写。
* @param szText 目标字符串。
* @param iLength 要转换的长度，不包括终止符。
* @return 被转换的字符的个数，不包括终止符。
*/
template<typename CharT>
int_x textupper(CharT * text, int_x length = -1)
{
	if(length < 0)
		length = textlen(text);

	int_x iCount = 0;
	for(int_x cnt = 0; cnt < length; ++cnt)
	{
		CharT & ch = text[cnt];
		if('a' <= ch && ch <= 'z')
		{
			ch = 'A' + (ch - 'a');
			++iCount;
		}
	}
	return iCount;
}

// -------------------------------------------------------------------------------------------------
// -------------------- 对文本进行增、删、改的操作 -----------------------------------------------------
// -------------------------------------------------------------------------------------------------

template<typename CharT>
int_x textch(const CharT * text, int_x length, const CharT & ch)
{
	if(!text || !length)
		return -1;
	const CharT * szTemp = text;
	while(*szTemp && length--)
	{
		if(*szTemp == ch)
			return szTemp - text;
		else
			++szTemp;
	}
	return -1;
}

template<typename CharT>
const CharT * textpch(const CharT * text, int_x length, const CharT & ch)
{
	int_x index = textch(text, length, ch);
	if(index < 0)
		return nullptr;
	else
		return text + index;
}

template<typename CharT>
int_x textrch(const CharT * text, int_x length, const CharT & ch)
{
	if(!text || !length)
		return -1;

	if(length < 0)
		length = textlen(text);

	const CharT * pLast = text + (length - 1);
	const CharT * pEnd = text - 1;

	while(length-- && pLast > pEnd)
	{
		if(*pLast == ch)
			return pLast - text;
		else
			--pLast;
	}
	return -1;
}

template<typename CharT>
const CharT * textprch(const CharT * text, int_x length, const CharT & ch)
{
	int_x index = textrch(text, length, ch);
	if(index < 0)
		return nullptr;
	else
		return text + index;
}

template<typename CharT>
int_x textcmp(const CharT * text, int_x length, const CharT * pattern, int_x pattern_length, bool caps = false)
{
	if(!text && !pattern)
		return 0;
	if(!length && !pattern_length)
		return 0;

	CharT ch1, ch2;
	while(length-- && pattern_length--)
	{
		ch1 = *text++;
		ch2 = *pattern++;
		if(!caps)
		{
			ch1 = chlower(ch1);
			ch2 = chlower(ch2);
		}
		if(ch1 != ch2)
			return ch1 - ch2;
		if(ch1 == 0)
			return 0;
	}
	return 0;
}

template<typename CharT>
int_x textstr(const CharT * text, int_x length, const CharT * pattern, int_x pattern_length, bool caps = true)
{
	if(length < 0)
		length = textlen(text);
	if(!text || !length)
		return -1;

	if(pattern_length < 0)
		pattern_length = textlen(pattern);
	if(!pattern || !pattern_length)
		return -1;

	if(length < pattern_length)
		return -1;

	const CharT * beg = text;
	const CharT * end = text + (length - pattern_length + 1);
	while(beg < end)
	{
		if(textequalex(beg, pattern_length, pattern, pattern_length, caps))
			return beg - text;
		else
			++beg;
	}
	return -1;
}

template<typename CharT>
int_x textrstr(const CharT * text, int_x length, const CharT * pattern, int_x pattern_length, bool caps = true)
{
	if(length < 0)
		length = textlen(text);
	if(!text || !length)
		return -1;

	if(pattern_length < 0)
		pattern_length = textlen(pattern);
	if(!pattern || !pattern_length)
		return -1;

	if(length < pattern_length)
		return -1;

	const CharT * beg = text + (length - pattern_length);
	const CharT * end = text;
	while(beg >= end)
	{
		if(textequalex(text, pattern_length, pattern, pattern_length, caps))
			return beg - text;
		else
			--beg;
	}
	return -1;
}

template<typename CharT>
int_x textrplch(CharT * text, int_x length, const CharT & src, const CharT & dst)
{
	if(!text || !text[0] || !length)
		return 0;

	int_x iCount = 0;
	while(*text && length--)
	{
		if(*text == src)
		{
			*text = dst;
			++iCount;
		}
		else {}
		++text;
	}
	return iCount;
}

template<typename CharT>
int_x textrplstr(CharT * text, int_x length, int_x size,
				 int_x index_beg, int_x index_end, const CharT * dst, int_x dst_length)
{
	if(length < 0) 
		length = textlen(text);
	if(dst_length < 0) 
		dst_length = textlen(dst);
	if(!length)
		return -1;

	if(length >= size)
		return -1;

	int_x count = index_end - index_beg;
	if(index_beg < 0 || count <= 0 || index_end > length)
		return -1;

	int_x length_new = length - count + dst_length;
	if(length_new >= size)
		return length_new - size + 1;

	buffmov(text + (index_beg + dst_length), text + index_end, size - index_end + 1);
	buffcpy(text + index_beg, dst, dst_length);
	return 0;
}

/**
* @brief 将 szText 中的 szSrc 替换为 szDst。
* @param szText 待替换的文本。
* @param iLength 待替换为本的长度。
* @param iSize 待替换为本的缓冲区最大长度，包括终止符。
* @param szSrc 要查找的文本。
* @param iSrcLength 要查找文本的长度。
* @param szDst 替换成的文本。
* @param iDstLength 替换成文本的长度。
* @param iMaxCount 最多替换的数量。
* @return 返回成功替换掉的文本的数量。
*/
template<typename CharT>
int_x textrplstr(CharT * text, int_x length, int_x size,
				 const CharT * src, int_x src_length, const CharT * dst, int_x dst_length,
				 bool caps = true, int_x count_max = IX_MAX)
{
	if(length < 0) 
		length = textlen(text);
	if(src_length < 0) 
		src_length = textlen(src);
	if(dst_length < 0) 
		dst_length = textlen(dst);
	if(!length || !src_length || count_max <= 0)
		return 0;

	int_x diff = dst_length - src_length;
	int_x count = 0;
	int_x length_new = length;
	CharT * beg = text;
	const CharT * end = text + length;
	while(beg < end)
	{
		int_x index = textstr(beg, length, src, src_length, caps);
		if(index < 0)
			break;

		if(length + diff >= size)
			break;
		length -= src_length + index;

		CharT * szCurr = beg + index;
		buffmov(szCurr + dst_length, szCurr + src_length, length);
		buffcpy(szCurr, dst, dst_length);
		length_new += diff;
		beg += index + dst_length;
		++count;
	}
	text[length_new] = (CharT)'\0';
	return count;
}

template<typename CharT>
int_x textcntstr(CharT * text, int_x length, const CharT * pattern, int_x patter_length, bool caps = true)
{
	if(length < 0)
		length = textlen(text);
	if(patter_length < 0)
		patter_length = textlen(pattern);
	if(!length || !patter_length)
		return 0;

	int_x count = 0;
	const CharT * beg = text;
	const CharT * end = text + (length - patter_length + 1);
	while(beg < end)
	{
		int_x index = textstr(beg, length, pattern, patter_length, caps);
		if(index < 0)
			break;

		++count;
		length -= patter_length + index;
		beg += patter_length + index;
	}
	return count;
}

template<typename CharT>
bool textbegwith(const CharT * text, int_x length, const CharT & ch, bool caps = true)
{
	if(length < 0)
		length = textlen(text);

	if(!length)
		return false;
	else if(caps)
		return text[0] == ch;
	else
		return tolower(text[0]) == tolower(ch);
}

template<typename CharT>
bool textbegwith(const CharT * text, int_x length, const CharT * pattern, int_x pattern_length, bool caps = true)
{
	if(length < 0)
		length = textlen(text);
	if(pattern_length < 0)
		pattern_length = textlen(pattern);

	if(!length || !pattern_length || length < pattern_length)
		return false;

	if(textcmp(text, pattern_length, pattern, pattern_length, caps) == 0)
		return true;
	else
		return false;
}

template<typename CharT>
bool textendwith(const CharT * text, int_x length, const CharT & ch)
{
	if(length < 0)
		length = textlen(text);

	if(!length)
		return false;
	else if(caps)
		return text[length - 1] == ch;
	else
		return tolower(text[length - 1]) == tolower(ch);
}

template<typename CharT>
bool textendwith(const CharT * text, int_x length, const CharT * pattern, int_x pattern_length, bool caps = true)
{
	if(length < 0)
		length = textlen(text);
	if(pattern_length < 0)
		pattern_length = textlen(pattern);

	if(!length || !pattern_length || length < pattern_length)
		return false;

	const CharT * pText = text + (length - pattern_length);
	if(textcmp(pText, pattern_length, pattern, pattern_length, caps) == 0)
		return true;
	else
		return false;
}

template<typename CharT>
int_x textcat(CharT * dst, int_x size, const CharT * src, int_x length = -1)
{
	if(length < 0)
		length = textlen(src);
	if(!dst || size <= 1 || !src || !length)
		return 0;
	else
	{
#ifdef _DEBUG
		bool found_null = false;
		for(CharT * pTemp = dst, *pEnd = dst + size; pTemp < pEnd; ++pTemp)
		{
			if(*pTemp == 0)
			{
				found_null = true;
				break;
			}
		}
		if(!found_null)
			Assert(false);
#endif
		int_x dst_length = textlen(dst, size);
		return textcpy(dst + dst_length, size - dst_length, src, length);
	}
}

template<typename CharT>
int_x textcat_format(CharT * buffer, int_x size, const CharT * format, ...)
{
	if(!buffer || size <= 1 || !format)
		return 0;
	else
	{
		int_x length = textlen(buffer, size);
		va_list pArgs = nullptr;
		va_start(pArgs, format);
		int_x length2 = textformat_args(buffer + length, size - length, format, pArgs);
		va_end(pArgs);

		return length + length2;
	}
}

// 合并 szSrcA 和 szSrcB 到 szDst
template<typename CharT>
int_x textcompose(CharT * dst, int_x size,
				  const CharT * src1, int_x src1_length,
				  const CharT * src2, int_x src2_length)
{
	if(!dst || size <= 0)
		return 0;
	else if(size == 1)
	{
		*dst = 0;
		return 0;
	}
	if(!src1 || !src1_length)
		return textcpy(dst, size, src2, src2_length);
	else if(!src2 || !src2_length)
		return textcpy(dst, size, src1, src1_length);
	else
	{
		int_x length = 0;
		while(size > 1 && src1_length && *src1)
		{
			*dst++ = *src1++;
			--size;
			--src1_length;
			++length;
		}
		while(size > 1 && src2_length && *src2)
		{
			*dst++ = *src2++;
			--size;
			--src2_length;
			++length;
		}
		*dst = 0;
		return length;
	}
}

// 合并 piLength 个 pszSrc 到 szDst
template<typename CharT>
int_x textcomposeex(CharT * dst, int_x size,
					const CharT ** arr_src, int_x * arr_length, int_x count)
{
	if(!dst || size <= 0)
		return 0;
	else if(size == 1)
	{
		*dst = 0;
		return 0;
	}
	else
	{
		int_x length = 0;
		const CharT * pSrc = nullptr;
		for(int_x cnt = 0; cnt < count && size > 1; ++cnt)
		{
			int_x src_length = arr_length[cnt];
			pSrc = arr_src[cnt];

			while(size > 1 && src_length && *pSrc)
			{
				*dst++ = *pSrc++;
				--size;
				--src_length;
				++length;
			}
		}
		*dst = 0;
		return length;
	}
}

template<typename CharT>
int_x texttrim(CharT * text, int_x length = -1, const CharT * trim_chars = nullptr, int_x trim_chars_length = -1, bool caps = true)
{
	if(length < 0)
		length = textlen(text);
	if(!length)
		return 0;

	if(trim_chars)
	{
		if(trim_chars_length < 0)
			trim_chars_length = textlen(trim_chars);
		if(!trim_chars_length)
			return 0;
		CharT * left = text;
		CharT * end = text + length;
		CharT ch = 0;

		while(left != end)
		{
			ch = *left;
			if(textch(trim_chars, trim_chars_length, ch, caps) >= 0)
				++left;
			else
				break;
		}

		if(left == end)
		{
			*text = 0;
			return length;
		}
		else
		{
			CharT * right = end;
			while(right != left)
			{
				ch = *(right - 1);
				if(textch(trim_chars, trim_chars_length, ch) >= 0)
					--right;
				else
					break;
			}

			int_x ret = length - (right - left);
			CharT * dst = text;
			while(left != right)
				*dst++ = *left++;
			*dst = 0;
			return ret;
		}
	}
	else
	{
		CharT * left = text;
		CharT * end = left + length;
		CharT ch = 0;
		while(left != end)
		{
			ch = *left;
			if(ch == ' ' || ch == '\t')
				++left;
			else
				break;
		}

		if(left == end)
		{
			*text = 0;
			return length;
		}
		else
		{
			CharT * right = end;
			while(right != left)
			{
				ch = *(right - 1);
				if(ch == ' ' || ch == '\t')
					--right;
				else
					break;
			}

			int_x ret = length - (right - left);
			CharT * dst = text;
			while(left != right)
				*dst++ = *left++;
			*dst = 0;
			return ret;
		}
	}
	return 0;
}














// ---------------------------------------------- 文件路径相关。
const int_x MAX_FILE_PATH = 260;

/**
* @brief 修正路径中的 "./" 和 "../" 标记。
* "./" 表示当前目录。
* "../" 表示上一层目录。
* 路径中的 '\' 会被替换为 '/'。
*/
template<typename CharT>
int_x textpathnormalize(CharT * path, int_x length = -1)
{
	if(length < 0)
		length = textlen(path);

	textrplch(path, length, (CharT)'\\', (CharT)'/');
	if(!length)
		return -1;

	int_x point = 0;

	CharT * pBeg = path;
	CharT * pEnd = pBeg + length;

	CharT * pPath = pBeg;
	CharT * pSlash = pBeg;
	CharT * pWrite = pBeg;
	CharT ch = 0;

	while(pPath != pEnd)
	{
		ch = *pPath;
		if(ch == '.')
			++point;
		else if(ch == '/')
		{
			// 忽略表示当前目录的 "./"。
			if(point == 1)
			{
				point = 0;
			}
			else if(point == 2)
			{
				if(pSlash == pBeg)
					break; // 没有更上层的目录了。

				--pSlash;
				// 回溯至上一个'/'。
				while(pSlash != pBeg)
				{
					if(*pSlash == '/')
						break;
					else
						--pSlash;
				}

				if(pSlash != pBeg)
					pWrite = pSlash + 1;
				else
					pWrite = pSlash;
				point = 0;
			}
			else
			{
				if(point > 2)
				{
					if(pWrite < pPath)
					{
						for(int_x cnt = 0; cnt < point; ++cnt)
							pWrite[cnt] = '.';
						pWrite += point;
						point = 0;
						*pWrite = ch;
						++pWrite;
					}
					point = 0;
				}
				else
				{
					if(pWrite < pPath)
						*pWrite = ch;
					++pWrite;
				}
				pSlash = pPath;
			}
		}
		else
		{
			if(pWrite < pPath)
			{
				for(int_x cnt = 0; cnt < point; ++cnt)
					pWrite[cnt] = '.';
				pWrite += point;
				point = 0;
				*pWrite = ch;
				++pWrite;
			}
			else
			{
				pWrite += point;
				++pWrite;
			}
		}
		++pPath;
	}

	if(pWrite > pBeg && *(pWrite - 1) == '/')
		--pWrite;

	if(pWrite < pEnd)
		*pWrite = 0;
	return pWrite - path;
}

/**
* @brief 将文件夹路径和相对目录生成完整目录。
* 内部会把 '\' 替换成 '/'。
*/
template<typename CharT>
int_x textmakepath(CharT * szPath, int_x iSize,
				   const CharT * szDirectory, int_x iDirectoryLength,
				   const CharT * szRelative, int_x iRelativeLength)
{
	textempty(szPath, iSize);
	if(!iSize)
		return 0;

	if(iDirectoryLength < 0)
		iDirectoryLength = textlen(szDirectory, MAX_FILE_PATH);
	if(iRelativeLength < 0)
		iRelativeLength = textlen(szRelative, MAX_FILE_PATH);

	const int_x COUNT = MAX_FILE_PATH * 2 + 4;
	CharT szTemp[COUNT] = {0};

	textcpy(szTemp, COUNT, szDirectory, iDirectoryLength);
	textrplch(szTemp, iDirectoryLength, (CharT)'\\', (CharT)'/');

	// 防止 '/' 重复。
	bool bEndL = szTemp[iDirectoryLength - 1] == '/';
	bool bEndR = szRelative[0] == '/';
	if(bEndL && bEndR)
	{
		--iDirectoryLength;
	}
	else if(!bEndL && !bEndR)
	{
		szTemp[iDirectoryLength] = '/';
		++iDirectoryLength;
	}
	else {}

	textcpy(szTemp + iDirectoryLength, COUNT - iDirectoryLength, szRelative, iRelativeLength);
	textrplch(szTemp + iDirectoryLength, iRelativeLength, (CharT)'\\', (CharT)'/');

	int_x iLength = iDirectoryLength + iRelativeLength;
	iLength = textpathnormalize(szTemp, iLength);
	if(iLength >= 0)
		return textcpy(szPath, iSize, szTemp, iLength);
	else
		return 0;
}

template<typename CharT>
void textsplitpath(const CharT * szPath, int_x iLength,
				   CharT * szDrive, int_x iDriveSize,
				   CharT * szFolder, int_x iFolderSize,
				   CharT * szName, int_x iNameSize,
				   CharT * szExt, int_x iExtSize,
				   CharT * szFileName, int_x iFileNameSize,
				   int_x * pDriveLength = nullptr,
				   int_x * pDirLength = nullptr,
				   int_x * pNameLength = nullptr,
				   int_x * pExtLength = nullptr,
				   int_x * pFileNameLength = nullptr)
{
	textempty(szDrive, iDriveSize);
	textempty(szFolder, iFolderSize);
	textempty(szName, iNameSize);
	textempty(szExt, iExtSize);
	textempty(szFileName, iFileNameSize);

	if(iLength < 0)
		iLength = textlen(szPath);

	if(!iLength || !szPath)
		return;

	if(szPath[iLength - 1] == '/' || szPath[iLength - 1] == '\\')
		--iLength;

	const CharT * pPath = szPath;
	const CharT * pEnd = pPath + iLength;
	int_x iDriveLength = 0, iDirLength = 0, iNameLength = 0, iExtLength = 0, iFileNameLength = 0;

	if((!szDrive && iDriveSize) || (szDrive && !iDriveSize) ||
	   (!szFolder && iFolderSize) || (szFolder && !iFolderSize) ||
	   (!szName && iNameSize) || (szName && !iNameSize) ||
	   (!szExt && iExtSize) || (szExt && !iExtSize))
	   return;

	// 驱动器
	const CharT * pTemp = pPath;
	const CharT * pColon = nullptr;
	while(pTemp < pEnd && *pTemp)
	{
		if(*pTemp == ':')
		{
			pColon = pTemp;
			++pTemp;
			break;
		}
		else
			++pTemp;
	}

	if(!pColon)
		pTemp = pPath;
	else
		iDriveLength = textcpy(szDrive, iDriveSize, pPath, (pColon - pPath) + 1);

	// 目录
	pPath = pTemp;
	const CharT * pSlash = nullptr;
	while(pTemp < pEnd && *pTemp)
	{
		if(*pTemp == '\\' || *pTemp == '/')
			pSlash = pTemp;
		++pTemp;
	}

	if(!pSlash)
		pTemp = pPath;
	else
	{
		iDirLength = textcpy(szFolder, iFolderSize, szPath, pSlash - szPath);
		pTemp = pSlash + 1;
	}

	// 名字
	pPath = pTemp;
	iNameLength = textcpy(szName, iNameSize, pPath, pEnd - pPath);
	const CharT * pDot = nullptr;
	while(pTemp < pEnd && *pTemp)
	{
		if(*pTemp == '.')
			pDot = pTemp;
		++pTemp;
	}

	if(!pDot)
		iFileNameLength = textcpy(szFileName, iFileNameSize, pPath, pEnd - pPath);
	else
	{
		iFileNameLength = textcpy(szFileName, iFileNameSize, pPath, pDot - pPath);
		iExtLength = textcpy(szExt, iExtSize, pDot, pEnd - pDot);
	}

	if(pDriveLength) *pDriveLength = iDriveLength;
	if(pDirLength) *pDirLength = iDirLength;
	if(pNameLength) *pNameLength = iNameLength;
	if(pExtLength) *pExtLength = iExtLength;
	if(pFileNameLength) *pFileNameLength = iFileNameLength;
}

template<typename CharT>
int_x textsplitpath_drive(const CharT * szPath, int_x iLength, CharT * szDrive, int_x iDriveSize)
{
	int_x iDriveLength = 0;
	textsplitpath<CharT>(szPath, iLength,
						 szDrive, iDriveSize, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0,
						 &iDriveLength, nullptr, nullptr, nullptr, nullptr);
	return iDriveLength;
}

template<typename CharT>
int_x textsplitpath_folder(const CharT * szPath, int_x iLength, CharT * szFolder, int_x iFolderSize)
{
	int_x iFolderLength = 0;
	textsplitpath<CharT>(szPath, iLength,
						 nullptr, 0, szFolder, iFolderSize, nullptr, 0, nullptr, 0, nullptr, 0,
						 nullptr, &iFolderLength, nullptr, nullptr, nullptr);
	return iFolderLength;
}

template<typename CharT>
int_x textsplitpath_name(const CharT * szPath, int_x iLength, CharT * szName, int_x iNameSize)
{
	int_x iNameLength = 0;
	textsplitpath<CharT>(szPath, iLength,
						 nullptr, 0, nullptr, 0, szName, iNameSize, nullptr, 0, nullptr, 0,
						 nullptr, nullptr, &iNameLength, nullptr, nullptr);
	return iNameLength;
}

template<typename CharT>
int_x textsplitpath_ext(const CharT * szPath, int_x iLength, CharT * szExt, int_x iExtSize)
{
	int_x iExtLength = 0;
	textsplitpath<CharT>(szPath, iLength,
						 nullptr, 0, nullptr, 0, nullptr, 0, szExt, iExtSize, nullptr, 0,
						 nullptr, nullptr, nullptr, &iExtLength, nullptr);
	return iExtLength;
}

template<typename CharT>
int_x textsplitpath_filename(const CharT * szPath, int_x iLength, CharT * szFileName, int_x iFileNameSize)
{
	int_x iFileNameLength = 0;
	textsplitpath<CharT>(szPath, iLength,
						 nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, szFileName, iFileNameSize,
						 nullptr, nullptr, nullptr, nullptr, &iFileNameLength);
	return iFileNameLength;
}

/**
* @brief 解析一个完整 URL 中的协议、域名和文件名部分。
*/
template<typename CharT>
void textspliturl(const CharT * szUrl, int_x iLength,
				  CharT * szProtocol, int_x iProtocolSize,
				  CharT * szDomain, int_x iDomainSize,
				  CharT * szFile, int_x iFileSize,
				  int_x * piProtocolLength = nullptr,
				  int_x * piDomainLength = nullptr,
				  int_x * piFileLength = nullptr)
{
	textempty(szProtocol, iProtocolSize);
	textempty(szDomain, iDomainSize);
	textempty(szFile, iFileSize);

	const CharT * pUrl = szUrl;

	int_x iProtocolLength = 0, iDomainLength = 0, iFileLength = 0;
	const char * pSlashSlash = textstr(pUrl, iLength, "//", 2);
	if(pSlashSlash)
	{
		iProtocolLength = textcpy(szProtocol, iProtocolSize, pUrl, pSlashSlash - pUrl);
		pUrl = pSlashSlash + 2;
		int_x iSlash = textch(pUrl, iLength, '/');
		if(iSlash >= 0)
		{
			iDomainLength = textcpy(szDomain, iDomainSize, pUrl, iSlash);
			iFileLength = textcpy(szFile, iFileSize, szFile + iSlash + 1, szUrl + (iLength - iSlash + 1));
		}
		else {}
	}
	else {}
	if(piProtocolLength) *piProtocolLength = iProtocolLength;
	if(piDomainLength) *piDomainLength = iDomainLength;
	if(piFileLength) *piFileLength = iFileLength;
}

template<typename CharT>
void textspliturlprotocol(const CharT * szUrl, int_x iLength,
						  CharT * szProtocol, int_x iProtocolSize,
						  int_x * piProtocolLength = nullptr)
{
	textspliturl<CharT>(szUrl, iLength, szProtocol, iProtocolSize, nullptr, 0, nullptr, 0, piProtocolLength, nullptr, nullptr);
}

template<typename CharT>
void textspliturldomain(const CharT * szUrl, int_x iLength,
						CharT * szDomain, int_x iDomainSize,
						int_x * piDomainLength = nullptr)
{
	textspliturl<CharT>(szUrl, iLength, nullptr, 0, szDomain, iDomainSize, nullptr, 0, nullptr, piDomainLength, nullptr);
}

template<typename CharT>
void textspliturlfile(const CharT * szUrl, int_x iLength,
					  CharT * szFile, int_x iFileSize,
					  int_x * piFileLength = nullptr)
{
	textspliturl<CharT>(szUrl, iLength, nullptr, 0, nullptr, 0, szFile, iFileSize, nullptr, nullptr, piFileLength);
}

template<typename CharT>
bool texttobool(const CharT * szText, int_x iLength = -1)
{
	const CharT TRUE_STR[5] = {'t', 'r', 'u', 'e', '\0'};
	return textequalex<CharT>(szText, iLength, TRUE_STR, 4, false);
}

/// 将文本转换为整数
template<typename CharT, typename IntT>
IntT texttoi(const CharT * text, int_x length, int_x scale = 0)
{
	if(length < 0)
		length = textlen(text);

	while(length && *text == ' ')
	{
		++text;
		--length;
	}

	IntT sign = 1;
	if(text[0] == L'-')
	{
		sign = (IntT)-1;
		++text;
	}

	if(!length || !text)
		return 0;

	const CharT * curr = text;
	if(scale <= 0)
	{
		if(curr[0] == '0' && length >= 2)
		{
			if(curr[1] == 'b' || curr[1] == 'B' || curr[1] == '0')
			{
				scale = 2;
				curr += 2;
			}
			else if(curr[1] == 'x' || curr[1] == 'X')
			{
				scale = 16;
				curr += 2;
			}
			else if(curr[1] == 'h' || curr[1] == 'H')
			{
				scale = 8;
				curr += 2;
			}
			else if(curr[1] == 'd' || curr[1] == 'D')
			{
				scale = 10;
				curr += 2;
			}
			else
				scale = 10;
		}
		else
			scale = 10;
	}

	IntT integer = 0;
	IntT number = 0;
	CharT ch = 0;

	while(length--)
	{
		ch = *curr++;
		if(!ch)
			break;

		if(ch >= '0' && ch <= '9')
			number = ch - '0';
		else if(ch >= 'A' && ch <= 'Z')
			number = ch - 'A' + 10;
		else if(ch >= 'a' && ch <= 'z')
			number = ch - 'a' + 10;
		else
			break;

		if(number >= (IntT)scale)
			break;

		integer = integer * (IntT)scale + number;
	}
	return integer * sign;
}

template<typename CharT>
int_x texttoix(const CharT * text, int_x length, int_x scale = 0)
{
	return texttoi<CharT, int_x>(text, length, scale);
}

template<typename CharT>
int_32 texttoi32(const CharT * text, int_x length, int_x scale = 0)
{
	return texttoi<CharT, int_32>(text, length, scale);
}

template<typename CharT>
int_64 texttoi64(const CharT * text, int_x length, int_x scale = 0)
{
	return texttoi<CharT, int_64>(text, length, scale);
}

/// 将文本转换为整数
template<typename CharT, typename IntT>
int_x texttoiarr(const CharT * text, int_x length, IntT * arr, int_x count, CharT split = ',', int_x scale = 0)
{
	if(!text || !length || !arr || count <= 0)
		return 0;

	if(length < 0)
		length = textlen(text, 64);

	int_x index = 0;
	const CharT * beg = text;
	const CharT * end = text + length;
	for(int_x cnt = 0; cnt < count; ++cnt)
	{
		int_x count = textch<CharT>(beg, end - beg, split);
		if(count < 0)
		{
			if(beg < end)
				count = end - beg;
			else
				break;
		}

		arr[index++] = texttoi<CharT, IntT>(beg, count, scale);
		beg += count + 1;
	}
	return index;
}

/// 将文本转换为浮点数
template<typename CharT, typename FloatT>
FloatT texttof(const CharT * text, int_x length = -1)
{
	if(length < 0)
		length = textlen<CharT>(text, 32);

	if(!length)
		return (FloatT)0;

	const CharT * end = text + length;
	bool sign_radix = true;
	bool sign_exp = true;
	// reading integer part.
	bool read_integer = true;
	// reading exp part.
	bool read_exp = false;

	// radix = integer + decimal.
	// integer part.
	FloatT integer = 0;
	// decimal part.
	FloatT decimal = 0;
	// exp part.
	int_x exp = 0;
	// use to caculate decimal.
	FloatT decimal_index = (FloatT)1 / (FloatT)10;

	if(text[0] == '-')
	{
		sign_radix = false;
		++text;
	}
	else {}

	while(text < end)
	{
		CharT ch = *text++;
		if(ch == '-')
		{
			if(read_exp)
				sign_exp = false;
			else
				break;
		}
		else if(ch == '.')
		{
			read_integer = false;
		}
		else if(ch == 'E' || ch == 'e')
		{
			read_integer = false;
			read_exp = true;
		}
		else if('0' <= ch && ch <= '9')
		{
			int_x number = (int_x)(ch - '0');
			if(read_exp)
			{
				exp *= 10;
				exp += number;
			}
			else if(read_integer)
			{
				integer *= 10;
				integer += number;
			}
			else
			{
				decimal += number * decimal_index;
				decimal_index *= (FloatT)0.1;
			}
		}
		else
			break;
	}

	FloatT result = (FloatT)(integer + decimal);
	if(!sign_radix)
		result = -result;

	if(sign_exp)
	{
		for(int_x cnt = 0; cnt < exp; ++cnt)
			result *= 10;
	}
	else
	{
		for(int_x cnt = 0; cnt < exp; ++cnt)
			result *= (FloatT)0.1;
	}

	return result;
}

template<typename CharT>
float_32 texttof32(const CharT * text, int_x length = -1)
{
	return texttof<CharT, float_32>(text, length);
}

template<typename CharT>
float_64 texttof64(const CharT * text, int_x length = -1)
{
	return texttof<CharT, float_64>(text, length);
}

template<typename CharT, typename FloatT>
int_x texttofarr(const CharT * text, int_x length, FloatT * arr, int_x count, CharT split = ',')
{
	if(!text || !length || !arr || count <= 0)
		return 0;

	if(length < 0)
		length = textlen(text, 64);

	int_x index = 0;
	const CharT * beg = text;
	const CharT * end = text + length;
	for(int_x cnt = 0; cnt < count; ++cnt)
	{
		int_x count = textch<CharT>(beg, end - beg, split);
		if(count < 0)
		{
			if(beg < end)
				count = end - beg;
			else
				break;
		}

		arr[index++] = texttof<CharT, FloatT>(beg, count);
		beg += count + 1;
	}
	return index;
}


/// 将整数转换为文本
template<typename CharT, typename IntT>
int_x textfromi(CharT * text, int_x size, IntT value, int_x scale = 0, bool caps = false)
{
	if(scale <= 0)
		scale = 10;

	// 进制不合法
	if(scale < 2 || scale > 35)
	{
		if(text && size)
			text[0] = 0;
		return 0;
	}
	int_x length = 0;
	bool sign = true;
	if(value < 0)
	{
		value = value * (IntT)(-1);
		sign = false;
		++length;
	}

	IntT temp = value;
	do
	{
		++length;
		temp /= scale;
	}
	while(temp);

	if(!text)
		return length;
	else if(length >= size)
	{
		if(text && size)
			text[0] = 0;
		return length;
	}
	else {}

	int_x index = length;
	if(!sign)
	{
		text[0] = '-';
	}

	CharT ch_a = caps ? 'A' : 'a';
	IntT number = 0;
	do
	{
		number = value % scale;
		value /= scale;

		if(text)
		{
			if(number > 9)
				text[--index] = (CharT)(ch_a + (number - 10));
			else
				text[--index] = (CharT)('0' + number);
		}
	}
	while(value);
	text[length] = '\0';
	return length;
}

template<typename CharT>
int_x textfromix(CharT * text, int_x size, int_x value, int_x scale = 0, bool caps = false)
{
	return textfromi(text, size, value, scale, caps);
}

/// 判断两个文本是否相等
template<typename CharT>
bool textequalex(const CharT * text, int_x length, const CharT * pattern, int_x pattern_length, bool caps = true)
{
	if(!text || !pattern)
		return false;

	if(length < 0)
		length = textlen<CharT>(text);
	if(pattern_length < 0)
		pattern_length = textlen<CharT>(pattern);

	if(length != pattern_length)
		return false;
	else
	{
		if(caps)
		{
			while(length--)
			{
				if(*text != *pattern)
					return false;

				++text;
				++pattern;
			}
			return true;
		}
		else
		{
			while(length--)
			{
				if(chlower(*text) != chlower(*pattern))
					return false;

				++text;
				++pattern;
			}
			return true;
		}
	}
}

//! @brief 判断两个文本是否相等
template<typename CharT>
bool textequal(const CharT * text, const CharT * pattern)
{
	return textequalex(text, -1, pattern, -1, true);
}

/// 判断两个文本是否部分相等
template<typename CharT>
bool textnequal(const CharT * text, int_x length, const CharT * pattern, int_x pattern_length, bool caps = true)
{
	if(!text || !pattern)
		return false;

	if(length < 0)
		length = textlen<CharT>(text);
	if(pattern_length < 0)
		pattern_length = textlen<CharT>(pattern);

	if(pattern_length > length)
		return false;
	else
	{
		if(caps)
		{
			while(pattern_length--)
			{
				if(*text != *pattern)
					return false;

				++text;
				++pattern;
			}
			return true;
		}
		else
		{
			while(pattern_length--)
			{
				if(*text != *pattern)
				{
					if(chlower(*text) != chlower(*pattern))
						return false;
					else {}
				}

				++text;
				++pattern;
			}
			return true;
		}
	}
}

/**
* @brief 移除文本中的一个字符。
* @param pText 要操作的文本。
* @param iLength 要操作的文本的长度。
* @param iSize 要操作的文本的容量。
* @param iIndex 移除的字符索引。
*/
template<typename CharT>
void textremove(CharT * text, int_x length, int_x size, int_x index)
{
	if(length <= size &&
	   index >= 0 && index < length)
	{
		if(index != length - 1)
			buffmov(text + index, length - index, text + index + 1, length - (index + 1));
		text[length - 1] = '\0';
	}
}

/**
* @brief 移除文本的一部分。
* @param pText 要操作的文本。
* @param iLength 要操作的文本的长度。
* @param iSize 要操作的文本的容量。
* @param iIndex 移除部分的起始索引。
* @param iCount 移除的字符个数。
*/
template<typename CharT>
void textremove(CharT * text, int_x length, int_x size, int_x index, int_x count)
{
	if(length < 0)
		length = textlen(text);

	if(length < size &&
	   index >= 0 && index <= length &&
	   count > 0 && index + count <= length)
	{
		buffmov(text + index, length - index, text + index + count, length - (index + count));
		text[length - count] = '\0';
	}
}

/**
* @brief 在文本中插入一个字符。
* @param pText 要操作的文本。
* @param iLength 要操作的文本的长度。
* @param iSize 要操作的文本的容量。
* @param iIndex 插入的位置索引。
*/
template<typename CharT>
int_x textinsert(CharT * text, int_x length, int_x size, int_x index, const CharT & ch)
{
	if(length < 0)
		length = textlen(text);

	if(length + 1 <= size &&
		index >= 0 && index <= length)
	{
		if(index != length)
			buffmov(text + index + 1, size - index - 1, text + index, length - index);
		text[index] = ch;
		text[length + 1] = '\0';
		return length + 1;
	}
	else
		return -1;
}

/**
* @brief 在文本中插入另一段文本。
* @param pText 要操作的文本。
* @param iLength 要操作的文本的长度。
* @param iSize 要操作的文本的容量。
* @param pSrc 插入的文本。
* @param iCount 插入的字符数量，如果小于 0，则插入全部字符。
* @param iIndex 插入的位置索引。
*/
template<typename CharT>
int_x textinsert(CharT * text, int_x length, int_x size, int_x index, const CharT * src, int_x src_length = -1)
{
	if(length < 0)
		length = textlen(text);

	if(src_length < 0)
		src_length = textlen(src);

	if(length + src_length < size &&
		index >= 0 && index <= length)
	{
		buffmov(text + index + src_length, size - (index + src_length), text + index, length - index);
		buffcpy(text + index, size - index, src, src_length);
		text[length + src_length] = '\0';
		return length + src_length;
	}
	else
		return -1;
}

/**
* @brief 计算指定的文本和参数格式化后所需的缓冲区大小(包括终止符)。
* @param format 格式化字符串。
* @param ... 参数。
*/
int_x CORE_API textformat_calc(const char_8 * format, ...);

/**
* @copydoc textformat_calc(const char_8 * format, ...) 。
*/
int_x CORE_API textformat_calc(const char_16 * format, ...);

/**
* @brief 计算指定的文本和参数列表格式化后所需的缓冲区大小(包括终止符)。
* @param format 格式化字符串。
* @param args 参数列表。
*/
int_x CORE_API textformatargs_calc(const char_8 * format, void * args);

/**
* @copydoc TextFormatCalcArgs(const char_8 * format, void * args) 。
*/
int_x CORE_API textformatargs_calc(const char_16 * format, void * args);

/**
* @brief 将格式啊字符串格式化至指定缓冲区内。
* @param buffer 目标缓冲区。
* @param size 目标缓冲区的大小(包括终止符)。
* @param format 格式化字符串。
* @param ... 参数列表。
*/
int_x CORE_API textformat(char_8 * buffer, int_x size, const char_8 * format, ...);

/**
* @copydoc TextFormatCalcArgs(const char_16 * format, void * args);
*/
int_x CORE_API textformat(char_16 * buffer, int_x size, const char_16 * format, ...);

/**
* @brief 将格式啊字符串格式化至指定缓冲区内。
* @param buffer 目标缓冲区。
* @param size 目标缓冲区的大小(包括终止符)。
* @param format 格式化字符串。
* @param args 参数列表指针。
*/
int_x CORE_API textformat_args(char_8 * buffer, int_x size, const char_8 * format, void * args);

/**
* @copydoc TextFormatArgs(char_8 * buffer, int_x size, const char_8 * format, void * args);
*/
int_x CORE_API textformat_args(char_16 * buffer, int_x size, const char_16 * format, void * args);

/**
* @brief 判断字符是否是可显示的。
* 可显示字符包括标点符号、大小写字母、数字以及大于0xFF的所有字符。
*/
CORE_API bool isprintchar(char_32 ch);

CORE_API int_x ansitounicode(const char_8 * src, int_x length, char_16 * dst, int_x size);
CORE_API int_x unicodetoansi(const char_16 * src, int_x length, char_8 * dst, int_x size);

#pragma endregion

#pragma region hash

const int_32 DEF_BASE_32 = 0x01000193u;
const int_32 DEF_SEED_32 = 0x811C9DC5u;
const int_64 DEF_BASE_64 = 0x0100019301000193u;
const int_64 DEF_SEED_64 = 0x811C9DC5811C9DC5u;
#ifdef BIT_64
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

#pragma endregion


template<typename _Ty>
struct remove_reference { typedef _Ty type;};
template<typename _Ty>
struct remove_reference<_Ty&> { typedef _Ty type;};
template<typename _Ty> struct remove_reference<_Ty&&> { typedef _Ty type;};
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