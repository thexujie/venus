#pragma once

VENUS_BEG

/**
 ** @brief 数组容器。
 ** @details 容器内的大小（Size）指的是内部预留的空间大小，长度（Length）指的是当前已使用的数量。
 **  二者都是使用 int_x 数据类型。
 **/
template<typename EleT, typename EleAllocator = DefEleAllocator<EleT> >
class CORE_API vector
{
public:
	typedef EleT * iterator;
	typedef const EleT * const_iterator;
	typedef EleT & reference;
	typedef const EleT & const_reference;

	vector()
		: m_buffer(nullptr), m_size(0), m_capability(0)
	{
	}

	vector(const vector & another)
	{
		m_buffer = nullptr;
		m_size = 0;
		m_capability = 0;
		if(another.m_size)
		{
			reallocate(another.m_size);
			for(int_x cnt = 0; cnt < another.m_size; ++cnt)
				EleAllocator::Construct(m_buffer + cnt, another.m_buffer[cnt]);
			m_size = another.m_size;
		}
	}

	vector(vector && another)
	{
		m_buffer = another.m_buffer;
		m_size = another.m_size;
		m_capability = another.m_capability;
		another.m_buffer = nullptr;
		another.m_size = 0;
		another.m_capability = 0;
	}

	explicit vector(int_x size)
		: m_size(size), m_capability(size)
	{
		m_buffer = EleAllocator::Malloc(size);
		for(int_x cnt = 0; cnt < size; ++cnt)
			EleAllocator::Construct(m_buffer + cnt);
	}

	vector(int_x size, int_x capability)
		: m_size(size), m_capability(capability)
	{
		m_buffer = EleAllocator::Malloc(capability);
		for(int_x cnt = 0; cnt < size; ++cnt)
			EleAllocator::Construct(m_buffer + cnt);
	}

	vector(int_x size, int_x capability, const EleT & ele)
		: m_size(size), m_capability(capability)
	{
		m_buffer = EleAllocator::Malloc(capability);
		for(int_x cnt = 0; cnt < size; ++cnt)
			EleAllocator::Construct(m_buffer + cnt, ele);
	}

	~vector()
	{
		for(int_x cnt = 0; cnt < m_size; ++cnt)
			EleAllocator::Destruct(m_buffer + cnt);
		EleAllocator::Free(m_buffer);
		m_buffer = 0;
		m_size = 0;
		m_capability = 0;
	}

	void fill(const EleT & ele)
	{
		for(int_x cnt = 0; cnt < m_size; ++cnt)
			m_buffer[cnt] = ele;
	}

	void fill(int_x index, int_x count, const EleT & ele)
	{
		if(count > 0 && 0 <= index && index + count <= m_size)
		{
			for(int_x cnt = index, end = index + count; cnt < end; ++cnt)
				m_buffer[cnt] = ele;
		}
	}

	void assign(int_x index, const EleT * eles, int_x count)
	{
		if(index < 0 || count <= 0 || !eles || index + count > m_size)
			return;

		for(int_x cnt = 0; cnt < count; ++cnt)
			m_buffer[index + cnt] = eles[cnt];
	}

	void join(const vector & another)
	{
		if(another.empty())
			return;

		int_x size = m_size + another.m_size;
		if(size > m_capability)
			reallocate(size + 1);

		for(int_x cnt = m_size; cnt < size; ++cnt)
			EleAllocator::Construct(m_buffer + cnt, another.m_buffer[cnt - m_size]);
		m_size = size;
	}

	void join(vector && another)
	{
		if(another.empty())
			return;

		int_x size = m_size + another.m_size;
		if(size > m_capability)
			reallocate(size + 1);

		for(int_x cnt = m_size; cnt < size; ++cnt)
		{
			EleAllocator::MoveConstruct(m_buffer + cnt, another.m_buffer[cnt - m_size]);
			EleAllocator::Destruct(another.m_buffer + (cnt - m_size));
		}

		another.m_size = 0;
		m_size = size;
	}

	EleT & insert_at(int_x index)
	{
		if(index < 0 || index > m_size)
			throw exp_out_of_bound();

		if(m_size >= m_capability)
			reallocate(m_capability + 1);

		// 末尾(size - 1)的元素移动到下一个(size)
		if(m_size > 0)
			EleAllocator::MoveConstruct(m_buffer + m_size, m_buffer[m_size - 1]);

		// 中间的元素依次往后移动
		for(int_x cnt = m_size - 1; cnt > index; --cnt)
			m_buffer[cnt] = move(m_buffer[cnt - 1]);

		// index 的元素先析构，再重新构造。
		if(m_size > 0)
			EleAllocator::Destruct(m_buffer + index);

		// 构造一个全新的元素
		EleAllocator::Construct(m_buffer + index);
		++m_size;
		return m_buffer[index];
	}

	EleT & insert_at(int_x index, const EleT & ele)
	{
		if(index < 0 || index > m_size)
			throw exp_out_of_bound();

		if(m_size >= m_capability)
			reallocate(m_capability + 1);

		for(int_x cnt = m_size; cnt > index; --cnt)
			m_buffer[cnt] = move(m_buffer[cnt - 1]);

		EleAllocator::Construct(m_buffer + index, ele);
		++m_size;
		return m_buffer[index];
	}

	EleT & add(const EleT & ele)
	{
		if(m_size >= m_capability)
		{
			// 如果是将自己的一个元素 add 到自己当中，SetCapability 会破坏value。
			EleT ele(ele);
			reallocate(m_size * 2 + 1);
			EleAllocator::Construct(m_buffer + m_size, ele);
		}
		else
		{
			EleAllocator::Construct(m_buffer + m_size, ele);
		}
		++m_size;
		return m_buffer[m_size - 1];
	}

	EleT & add()
	{
		if(m_size >= m_capability)
		{
			// 如果是将自己的一个元素 add 到自己当中，SetCapability 会破坏value。
			reallocate(m_size * 2 + 1);
			EleAllocator::Construct(m_buffer + m_size);
		}
		else
		{
			EleAllocator::Construct(m_buffer + m_size);
		}
		++m_size;
		return m_buffer[m_size - 1];
	}

	vector & add(const vector & another)
	{
		int_x iSize = m_size + another.m_size;
		if(m_capability < iSize)
			reallocate(iSize);

		for(int_x cnt = 0; cnt < another.m_size; ++cnt)
			EleAllocator::Construct(m_buffer + m_size + cnt, another.m_buffer[cnt]);

		m_size += another.m_size;
		return *this;
	}

	EleT & add_front()
	{
		return insert_at(0);
	}

	EleT & add_front(const EleT & ele)
	{
		return insert_at(0, ele);
	}

	void push_back(const EleT & element) { add(element); }

	int_x find(const EleT & ele) const
	{
		for(int_x cnt = 0; cnt < m_size; ++cnt)
		{
			if(m_buffer[cnt] == ele)
			{
				return cnt;
			}
		}
		return -1;
	}

	template<typename CmpFuncT, typename ParamT>
	int_x find(CmpFuncT cmp, const ParamT & param) const
	{
		for(int_x cnt = 0; cnt < m_size; ++cnt)
		{
			if(cmp(m_buffer[cnt], param))
			{
				return cnt;
			}
		}
		return -1;
	}

	/**
	 * @return 如果找到元素，则将其删除并返回true，否则，不应用任何改变并返回false。
	 * @note 调用本函数，需要元素支持 == 等于操作符。
	 */
	bool remove(const EleT & ele)
	{
		for(int_x cnt = 0; cnt < m_size; ++cnt)
		{
			if(m_buffer[cnt] == ele)
			{
				remove_at(cnt);
				return true;
			}
		}
		return false;
	}

	/// 移除指定索引的元素
	/// @param iIndex 需要被移除的元素的索引
	/// @throw 如果指定的索引大于数组的长度，抛出 exp_out_of_bound 异常。
	void remove_at(int_x index)
	{
		if(index < 0 || index >= m_size)
			throw exp_out_of_bound();

		EleAllocator::Destruct(m_buffer + index);
		for(int_x cnt = index; cnt < m_size - 1; ++cnt)
			m_buffer[cnt] = move(m_buffer[cnt + 1]);
		EleAllocator::Destruct(m_buffer + m_size - 1);
		--m_size;
	}

	/// 移除指定起始索引、指定数量的元素
	/// @param iIndex 需要被移除的元素的起始索引
	/// @param iCount 需要被移除的元素数量
	/// @throw 如果指定的索引范围大于数组的长度，抛出 exp_out_of_bound 异常。
	void remove_at(int_x index, int_x count)
	{
		if(count <= 0)
			throw exp_illegal_argument();

		int_x end = index + count;
		if(index < 0 || index >= m_size || m_size < end)
			throw exp_out_of_bound();

		// num 个元素可以直接复制来的。
		int_x num_copy = m_size - end;
		for(int_x cnt = 0; cnt < num_copy; ++cnt)
			m_buffer[index + cnt] = move(m_buffer[end + cnt]);
		if(count > num_copy)
		{
			int_x num_del = count - num_copy;
			for(int_x cnt = index + num_copy; cnt < end; ++cnt)
				EleAllocator::Destruct(m_buffer + cnt);
		}
		m_size -= count;
	}

	void clear()
	{
		for(int_x cnt = 0; cnt < m_size; ++cnt)
			EleAllocator::Destruct(m_buffer + cnt);
		m_size = 0;
	}

	template<typename CalcT>
	int_x remove_by(CalcT pCalc)
	{
		if(!m_size)
			return 0;
		else
		{
			EleT * pLeft = m_buffer;
			EleT * pRight = m_buffer;
			EleT * pEnd = m_buffer + m_size;
			int_x iCountRemoved = 0;

			while(pRight != pEnd)
			{
				if(pCalc(*pRight))
				{
					EleAllocator::Destruct(pRight);
					++iCountRemoved;
					--m_size;

					if(pLeft != pRight)
					{
						EleAllocator::Construct(pLeft, *pRight);
						EleAllocator::Destruct(pRight);
						//++pLeft;
					}
					else {}
				}
				else
				{
					if(pLeft != pRight)
					{
						EleAllocator::Construct(pLeft, *pRight);
						EleAllocator::Destruct(pRight);
						++pLeft;
					}
					else
					{
						++pLeft;
					}
				}
				++pRight;
			}
			return iCountRemoved;
		}
	}

	bool empty() const
	{
		return m_size == 0;
	}
	bool valid() const
	{
		return m_size > 0;
	}

	int_x size() const
	{
		return m_size;
	}

	void _increase(int_x size)
	{
		if(size < 0)
			throw exp_illegal_argument();

		if(size > m_capability)
			reallocate(size);
		else {}
	}

	void resize(int_x size)
	{
		_increase(size);
		if(size > m_size)
		{
			for(int_x cnt = m_size; cnt < size; ++cnt)
				EleAllocator::Construct(m_buffer + cnt);
		}
		else if(size < m_size)
		{
			for(int_x cnt = size; cnt < m_size; ++cnt)
				EleAllocator::Destruct(m_buffer + cnt);
		}
		else {}

		m_size = size;
	}

	void resize(int_x size, const EleT & ele)
	{
		_increase(size);

		if(size <= m_size)
		{
			for(int_x cnt = 0; cnt < size; ++cnt)
				m_buffer[cnt] = ele;

			for(int_x cnt = size; cnt < m_size; ++cnt)
				EleAllocator::Destruct(m_buffer + cnt);

			m_size = size;
		}
		else
		{
			for(int_x cnt = 0; cnt < m_size; ++cnt)
				m_buffer[cnt] = ele;

			for(int_x cnt = m_size; cnt < size; ++cnt)
				EleAllocator::Construct(m_buffer + cnt, ele);

			m_size = size;
		}
	}

	//! 真实改变内部 m_iLength 的值。
	void set_size(int_x size)
	{
		m_size = size;
	}

	/// 设置数组的容量，改变容量会直接导致所有的数据丢失，
	/// 如果要改变数组中元素数量，应该调用 SetLength() 。
	void reallocate(int_x capability)
	{
		if(capability <= 0)
			return;

		EleT * buffer = EleAllocator::Malloc(capability);

		int_x size = m_size < capability ? m_size : capability;

		// 复制到新数组中
		for(int_x cnt = 0; cnt < size; ++cnt)
			EleAllocator::MoveConstruct(buffer + cnt, m_buffer[cnt]);

		// 销毁旧的数据
		for(int_x cnt = 0; cnt < m_size; ++cnt)
			EleAllocator::Destruct(m_buffer + cnt);

		EleAllocator::Free(m_buffer);

		m_size = size;
		m_capability = capability;
		m_buffer = buffer;
	}

	void reallocate(int_x size, int_x capability)
	{
		if(capability <= 0)
			return;

		if(size > capability)
			size = capability;

		EleT * buffer = EleAllocator::Malloc(capability);

		int_x length = m_size < size ? m_size : size;

		// 复制到新数组中
		for(int_x cnt = 0; cnt < length; ++cnt)
			EleAllocator::Construct(buffer + cnt, m_buffer[cnt]);

		for(int_x cnt = length; cnt < size; ++cnt)
			EleAllocator::Construct(buffer + cnt);

		// 销毁旧的数据
		for(int_x cnt = 0; cnt < m_size; ++cnt)
			EleAllocator::Destruct(m_buffer + cnt);

		EleAllocator::Free(m_buffer);

		m_size = size;
		m_capability = capability;
		m_buffer = buffer;
	}


	void reallocate(int_x size, int_x capability, const EleT & ele)
	{
		if(capability <= 0)
			return;

		if(size > capability)
			size = capability;

		EleT * buffer = EleAllocator::Malloc(capability);

		// 赋值到新数组中
		for(int_x cnt = 0; cnt < size; ++cnt)
			EleAllocator::Construct(buffer + cnt, ele);

		// 销毁旧的数据
		for(int_x cnt = 0; cnt < m_size; ++cnt)
			EleAllocator::Destruct(m_buffer + cnt);

		EleAllocator::Free(m_buffer);

		m_size = size;
		m_capability = capability;
		m_buffer = buffer;
	}

	/// 获取数组的容量
	int_x capability() const
	{
		return m_capability;
	}

	EleT * buffer()
	{
		return m_buffer;
	}
	const EleT * buffer() const
	{
		return m_buffer;
	}

	operator EleT * ()
	{
		return m_buffer;
	}

	operator const EleT * () const
	{
		return m_buffer;
	}

	EleT & operator [](int_x iIndex)
	{
		if(iIndex < 0 || iIndex >= m_size)
			throw exp_out_of_bound();
		return m_buffer[iIndex];
	}

	const EleT & operator [](int_x iIndex) const
	{
		if(iIndex < 0 || iIndex >= m_size)
			throw exp_out_of_bound();
		return m_buffer[iIndex];
	}
	EleT & at(int_x iIndex)
	{
		if(iIndex < 0 || iIndex >= m_size)
			throw exp_out_of_bound();
		return m_buffer[iIndex];
	}

	const EleT & at(int_x iIndex) const
	{
		if(iIndex < 0 || iIndex >= m_size)
			throw exp_out_of_bound();
		return m_buffer[iIndex];
	}

	const EleT & front() const
	{
		if(!m_size)
			throw exp_out_of_bound();
		return m_buffer[0];
	}

	EleT & front()
	{
		if(!m_size)
			throw exp_out_of_bound();
		return m_buffer[0];
	}

	const EleT & back() const
	{
		if(!m_size)
			throw exp_out_of_bound();
		return m_buffer[m_size - 1];
	}

	EleT & back()
	{
		if(!m_size)
			throw exp_out_of_bound();
		return m_buffer[m_size - 1];
	}

	// -------------------------------------------------------- 
	bool operator == (const vector<EleT> & another) const
	{
		if(m_size != another.m_iLength)
			return false;

		for(int_x cnt = 0; cnt < m_size; ++cnt)
		{
			if(m_buffer[cnt] != another.m_pBuffer[cnt])
				return false;
		}
		return true;
	}

	bool operator != (const vector<EleT> & another) const
	{
		return !(this->operator ==(another));
	}

	vector & operator = (const vector<EleT> & another)
	{
		for(int_x cnt = 0; cnt < m_size; ++cnt)
			EleAllocator::Destruct(m_buffer + cnt);
		m_size = 0;
		if(m_capability < another.m_size)
		{
			EleAllocator::Free(m_buffer);
			m_buffer = EleAllocator::Malloc(another.m_capability);
			m_capability = another.m_capability;
		}
		for(int_x cnt = 0; cnt < another.m_size; ++cnt)
			EleAllocator::Construct(m_buffer + cnt, another.m_buffer[cnt]);
		m_size = another.m_size;
		return *this;
	}

	vector & operator = (vector<EleT> && another)
	{
		for(int_x cnt = 0; cnt < m_size; ++cnt)
			EleAllocator::Destruct(m_buffer + cnt);
		EleAllocator::Free(m_buffer);

		m_buffer = another.m_buffer;
		m_size = another.m_size;
		m_capability = another.m_capability;
		another.m_buffer = nullptr;
		another.m_size = 0;
		another.m_capability = 0;
		return *this;
	}

	vector & swap(vector<EleT> & another)
	{
		EleT * tmp_buffer = m_buffer;
		int_x tmp_size = m_size;
		int_x tmp_capability = m_capability;

		m_buffer = another.m_buffer;
		m_size = another.m_size;
		m_capability = another.m_capability;

		another.m_buffer = tmp_buffer;
		another.m_size = tmp_size;
		another.m_capability = tmp_capability;
		return *this;
	}

	// ------------------ 支持 Stack 
	void push(const EleT & value)
	{
		add(value);
	}

	EleT pop()
	{
		if(!m_size)
			throw exp_out_of_bound(L"堆栈已空，无法出栈！");
		else
		{
			EleT ele = m_buffer[m_size - 1];
			remove_at(m_size - 1);
			return ele;
		}
	}

	// ------------------------------------------------------------------ 冒泡排序。
	// 使用模板参数提供的方法进行排序。
	// 考虑到模板匹配问题，比较和交换函数都采用元素指针的形式。
	template<typename CmpFuncT, typename SwapFuncT>
	void sort(CmpFuncT pfnCmp, SwapFuncT pfnSwap)
	{
		if(m_size <= 1)
			return;

		for(int_x cnt = m_size - 1, index = 0; cnt >= 0; --cnt)
		{
			for(index = 0; index < cnt; ++index)
			{
				if(!pfnCmp(m_buffer[index], m_buffer[index + 1]))
				{
					pfnSwap(m_buffer[index], m_buffer[index + 1]);
				}
			}
		}
	}

	template<typename CmpFuncT>
	void sort(const CmpFuncT & pfnCmp)
	{
		if(m_size <= 1)
			return;

		for(int_x cnt = m_size - 1, index = 0; cnt >= 0; --cnt)
		{
			for(index = 0; index < cnt; ++index)
			{
				if(!pfnCmp(m_buffer[index], m_buffer[index + 1]))
				{
					EleT temp = m_buffer[index];
					m_buffer[index] = m_buffer[index + 1];
					m_buffer[index + 1] = temp;
				}
			}
		}
	}

	iterator begin() { return m_buffer; }
	iterator end() { return m_buffer + m_size; }
	const_iterator begin() const { return m_buffer; }
	const_iterator end() const { return m_buffer + m_size; }

protected:
	EleT * m_buffer;
	int_x m_size;
	int_x m_capability;
};

VENUS_END
