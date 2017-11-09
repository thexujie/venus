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

/*!@brief �� src �е� size �� uint_8 ��ֵΪ val��*/
inline void buffset8(void * dst, int_x count, uint_8 val)
{
	if(count > 0)
		memset(dst, val, count);
}

/*!@brief �� src �е� size �� uint_16 ��ֵΪ val��*/
inline void buffset16(void * dst, int_x count, uint_16 val)
{
	uint_16 * u16 = (uint_16 *)dst;
	for(int_x cnt = 0; cnt < count; ++cnt)
		u16[cnt] = val;
}

/*!@brief �� src �е� size �� uint_32 ��ֵΪval��*/
inline void buffset32(void * dst, int_x count, uint_32 val)
{
	uint_32 * u32 = (uint_32 *)dst;
	for(int_x cnt = 0; cnt < count; ++cnt)
		u32[cnt] = val;
}

/*!@brief �� src �е� count �� uint_64 ��ֵΪval��*/
inline void buffset64(void * dst, int_x count, uint_64 val)
{
	uint_64 * u64 = (uint_64 *)dst;
	for(int_x cnt = 0; cnt < count; ++cnt)
		u64[cnt] = val;
}

/*!@brief �Ƚ� src �� dst �е�ǰ count ���ֽڡ�*/
inline int_x buffcmp8(void * dst, const void * src, int_x count)
{
	if(count > 0)
		return memcmp(dst, src, count);
	else
		return 0;
}

/**!@brief �� val ���ֽ�����Ϊ0��*/
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

/*!brief ���� �� src ���ֽڸ��Ƶ� dst��*/
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

/*!brief ���� �� count �� src<ValT> ���ֽڸ��Ƶ� dst��*/
template<typename ValT>
inline void buffcpy(ValT * dst, int_x size, const ValT * src, int_x count)
{
	buffcpy(dst, src, minof(size, count));
}

/*!@brief �ƶ� �� src �е� count ���ֽڵ�����Ϊ count �ֽڵ� count �С�*/
template<typename ValT>
inline void buffmov(ValT * dst, const ValT * src, int_x count)
{
	buffmov8(dst, src, count * sizeof(ValT));
}

/**
* @brief �Զ����Ƶ���ʽ����ָ��λ�á�ָ�����ȵ��������ƶ��ķ�ʽ���Ƶ�Ŀ��λ�á�
* @param pDst Ŀ��λ�á�
* @param iSize Ŀ��������
* @param pSrc Դ���顣
* @param iCount Ҫ���Ƶ����ݵĸ�����
*/
template<typename EleT>
void buffmov(EleT * dst, int_x size, const EleT * src, int_x count)
{
	buffmov(dst, src, minof(size, count));
}

#pragma endregion // buff

#pragma region array
/**
* @defgroup VenusCore-Base-Array ���鹤��
* @ingroup VenusCore-Until
* @brief �ṩ��һЩ���õ����������
* @{
*/

/*
 * !@brief ����̶�����ĳ���
 */
template<typename EleT, int_x LENGTH>
constfun int_x arraysize(const EleT(&arr)[LENGTH])
{
	return LENGTH;
}

/*!@brief �Գ���Ϊ length ���� arr ���и�ֵ��*/
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

/*!@brief ��pArray�е�iLength��EleT��ֵΪval��*/
template<typename EleT>
void arrayfill(EleT * dst, int_x size, const EleT & val)
{
	for(int_x cnt = 0; cnt < size; ++cnt)
		dst[cnt] = val;
}

/*!@brief ��pArray�е�iLength��EleT��ֵΪval��*/
template<typename EleT, int_x LENGTH>
void arrayfill(EleT(&dst)[LENGTH], const EleT & val)
{
	arrayfill(dst, LENGTH, val);
}

/*!@brief ���� src �е� count �� EleT ������Ϊ size �� EleT �� dst �С�*/
template<typename EleT>
void arraycpy(EleT * dst, const EleT * src, int_x count)
{
	for(int_x cnt = 0; cnt < count; ++cnt)
		dst[cnt] = src[cnt];
}

/*!@brief ���� src �е� count �� EleT ������Ϊ size �� EleT �� dst �С�*/
template<typename EleT>
void arraycpy(EleT * dst, int_x size, const EleT * src, int_x count)
{
	arraycpy(dst, src, minof(size, count));
}

/*!@brief ���� src �е� count �� EleT ������Ϊ size �� EleT �� dst �С�*/
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
* @brief ��ָ��λ�á�ָ�����ȵ��������ƶ��ķ�ʽ���Ƶ�Ŀ��λ�á�
* @param pDst Ŀ��λ�á�
* @param iSize Ŀ��������
* @param pSrc Դ���顣
* @param iCount Ҫ���Ƶ����ݵĸ�����
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

/*!@brief ������Ϊ length ������ arr �д� index ������ʼ�Ĺ� count ��Ԫ��ɾ����*/
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
* @brief �������е�Ԫ�ؽ��в������������������pfnCmp��
* @param arr �����������ĵ�һ��Ԫ�ص�ָ�롣
* @param length �����������ĳ��ȡ�
* @param pfnCmp �ȽϹ��򣬿����Ǻ�����Lamda���ʽ��������operator(left, right)���࣬�ȽϹ���
*  �ķ���ֵ������left �������е�λ���Ƿ�Ӧ����right��ǰ�档
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
* @brief �������е�Ԫ�ؽ�����������������ǱȽϺ���CmpFunc��
* @param arr �����������ĵ�һ��Ԫ�ص�ָ�롣
* @param length �����������ĳ��ȡ�
* @param pfnCmp �ȽϺ����������ķ���ֵ������left �������е�λ���Ƿ�Ӧ����right��ǰ�档
*  Ĭ�ϵıȽϺ�����С�� small_then���������е�Ԫ���ǰ��մ�С�����˳�����С�
* @see ArraySortInsertEx
*/
template<typename EleT>
void arraysortinsert(EleT * arr, int_x length)
{
	arraysortinsertex(arr, length, small_then<EleT>);
}

/**
* @brief ���Բ���ָ����Χ�ڵ�Ԫ�أ�ʹ�� == ��ƥ��Ԫ�ء�
* @param begin ������ʼ�㡣
* @param end �����յ㡣
* @return ���ز���ֹͣλ�á�
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
* @brief ���Բ���ָ����Χ�ڵ�Ԫ�أ�ʹ�� match ��ƥ��Ԫ�ء�
* @param begin ������ʼ�㡣
* @param end �����յ㡣
* @param match ƥ�����
* @return ���ز���ֹͣλ�á�
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
