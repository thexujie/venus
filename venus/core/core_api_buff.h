#pragma once

VENUS_BEG

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
template<typename ValT>
inline void buffclr(ValT & val)
{
	buffset8(&val, sizeof(ValT), 0);
}

template<typename ValT>
inline void buffset(ValT * dst, int_x size, const ValT & val)
{
	for(int_x cnt = 0; cnt < size; ++cnt)
		dst[cnt] = val;
}

/*!brief 复制 将 src 按字节复制到 dst。*/
template<typename ValT>
inline void buffcpy(ValT & dst, const ValT & src)
{
	buffcpy8(&dst, &src, sizeof(ValT));
}

template<typename ValT>
inline void buffcpy(ValT * dst, const ValT * src, int_x count)
{
	buffcpy8(dst, src, sizeof(ValT) * count);
}

/*!brief 复制 将 count 个 src<ValT> 按字节复制到 dst。*/
template<typename ValT>
inline void buffcpy(ValT * dst, int_x size, const ValT * src, int_x count)
{
	buffcpy(dst, src, minof(size, count));
}

/*!@brief 移动 将 src 中的 count 个字节到容量为 count 字节的 count 中。*/
template<typename ValT>
inline void buffmov(ValT * dst, const ValT * src, int_x count)
{
	buffmov8(dst, src, count * sizeof(ValT));
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
		EleT * insert = nullptr;
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

VENUS_END
