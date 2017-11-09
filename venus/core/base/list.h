#pragma once

VENUS_BEG

template<typename EleT>
class CORE_API list
{
private:
	class node_t
	{
	public:
		node_t() : pNext(nullptr), pNextBuffer(nullptr) {}
		// 使用 operator = 赋值
		EleT data;
		// 下一个节点
		node_t * pNext;
		// 缓存中的下一个
		node_t * pNextBuffer;
	};

public : // --------------------------------------
	list(bool bBuffer = false)
		: m_head(nullptr), m_size(0), m_buffered(bBuffer), m_buffer(nullptr), m_buffer_end(nullptr)
	{

	}

	list(const list & another, bool bBuffer = false)
		: m_head(nullptr), m_size(0), m_buffered(bBuffer), m_buffer(nullptr), m_buffer_end(nullptr)
	{
		node_t * pNode = another.m_head;
		while(pNode)
		{
			add(pNode->data);
			pNode = pNode->pNext;
		}
	}

	~list()
	{
		destroy();

		if(m_buffered && m_buffer)
		{
			node_t * pTemp = nullptr;
			while(pTemp = m_buffer)
			{
				m_buffer = m_buffer->pNextBuffer;
				delete pTemp;
			}
			m_buffer = nullptr;
			m_buffer_end = nullptr;
		}
	}
public :
	class iterator
	{
	public :
		iterator(node_t * pNode) : m_pNode(pNode) {}
		iterator(const iterator & iter) { m_pNode = iter.m_pNode;}

		iterator & operator = (const iterator & iter)
		{
			m_pNode = iter.m_pNode;
			return *this;
		}
		bool operator == (const iterator & iter) const { return m_pNode == iter.m_pNode;}
		bool operator != (const iterator & iter) const { return m_pNode != iter.m_pNode;}
		iterator & operator + (int_x iOff)
		{
			if(iOff < 0)
				throw exp_illegal_argument();
			else if(!iOff)
				return *this;
			else
			{
				node_t * pNode = m_pNode;
				while(iOff && pNode)
				{
					pNode = pNode->pNext;
					--iOff;
				}

				if(!iOff)
				{
					m_pNode = pNode;
					return *this;
				}
				else
					throw exp_out_of_bound(L"迭代器超出范围！");
			}
		}
		iterator & operator ++()
		{
			if(m_pNode)
			{
				m_pNode = m_pNode->pNext;
				return *this;
			}
			else
				throw exp_out_of_bound(L"迭代器超出范围！");
		}

		const iterator & operator ++(int)
		{
			if(m_pNode)
			{
				iterator iter(m_pNode);
				m_pNode = m_pNode->pNext;
				return iter;
			}
			else
				throw exp_out_of_bound(L"迭代器超出范围！");
		}

		const EleT & operator * () const { return m_pNode->data;}
		EleT & operator * () { return m_pNode->data;}
		bool has_next() const { return m_pNode && m_pNode->pNext;}
		EleT * operator -> () { return &(m_pNode->data);}
		const EleT * operator -> () const { return &(m_pNode->data);}
		const EleT * operator & () const { return &(m_pNode->data);}
		EleT * operator & () { return &(m_pNode->data);}
	private :
		node_t * m_pNode;
	};
public : // --------------------------------------
	iterator begin() const
	{
		return iterator(m_head);
	}
	iterator end() const
	{
		return iterator(nullptr);
	}

	void add(const EleT & ele)
	{
		node_t * pNode = nullptr;
		node_create(&pNode, ele);
		if(!m_head)
			m_head = pNode;
		else
		{
			node_t * pTemp = m_head;
			while(pTemp->pNext)
				pTemp = pTemp->pNext;
			pTemp->pNext = pNode;
		}
		++m_size;
	}

	void add_front(const EleT & ele)
	{
		node_t * pNode = nullptr;
		node_create(&pNode, ele);
		pNode->pNext = m_head;
		m_head = pNode;
		++m_size;
	}

	bool remove(const EleT & ele)
	{
		if(!m_head)
			return false;
		else if(m_head->data == ele)
		{
			node_t * pNode = m_head;
			m_head = m_head->pNext;
			node_destroy(pNode);
			--m_size;
			return true;
		}
		else
		{
			node_t * pNode = m_head;
			while(pNode->pNext && pNode->pNext->data != ele)
				pNode = pNode->pNext;

			if(pNode->pNext)
			{
				node_t * pTemp = pNode->pNext->pNext;
				node_destroy(pNode->pNext);
				--m_size;
				pNode->pNext = pTemp;
				return true;
			}
			else
				return false;
		}
	}

	void remove_at(int_x iIndex)
	{
		if(!m_head || iIndex < 0 || iIndex >= m_size)
			throw exp_out_of_bound();

		node_t * pNode = m_head;
		// 从第一个开始删除
		if(!iIndex)
		{
			m_head = m_head->pNext;
			node_destroy(pNode);
			--m_size;
		}
		else
		{
			while(--iIndex)
				pNode = pNode->pNext;

			node_t * pTemp = pNode->pNext;
			pNode->pNext = pNode->pNext->pNext;
			node_destroy(pTemp);
			--m_size;
		}
	}

	void remove_at(int_x iIndex, int_x iCount)
	{
		if(iCount <= 0)
			return ;

		if(iIndex < 0 || iIndex + iCount > m_size)
			throw exp_out_of_bound(_T("无法移除元素，索引超出范围！"));
		else
		{
			node_t * pNode = m_head;
			if(iIndex)
			{
				while(--iIndex)
					pNode = pNode->pNext;

				node_t * pStart = pNode;
				node_t * pTemp = nullptr;

				pNode = pNode->pNext;
				while(iCount--)
				{
					pTemp = pNode;
					pNode = pNode->pNext;
					node_destroy(pTemp);
					--m_size;
				}

				pStart->pNext = pNode;
			}
			// 从第一个开始删除
			else
			{
				while(iCount--)
				{
					m_head = m_head->pNext;
					node_destroy(pNode);
					pNode = m_head;
					--m_size;
				}
			}

		}
	}

	void removeall()
	{
		node_t * pTemp = nullptr;
		while((pTemp = m_head) && m_size)
		{
			m_head = m_head->pNext;
			node_destroy(pTemp);
			--m_size;
		}
	}

	template<typename CalcT>
	int_x remove_by(CalcT pCalc)
	{
		if(!m_head)
			return 0;
		else
		{
			int_x iCountRemoved = 0;
			node_t * pRemove = nullptr;
			// 移除头结点
			while(pCalc(m_head->data))
			{
				pRemove = m_head;
				m_head = m_head->pNext;
				node_destroy(pRemove);
				--m_size;
				++iCountRemoved;

				if(!m_head)
					return iCountRemoved;
			}

			//assert(m_pHead);
			//assert(!calc.NeedRemove(m_pHead->ele));
			node_t * pNode = m_head;
			while(pNode->pNext)
			{
				if(pCalc(pNode->pNext->data))
				{
					pRemove = pNode->pNext;
					pNode->pNext = pRemove->pNext; // pNode->pNext->pNext
					node_destroy(pRemove);
					--m_size;
					++iCountRemoved;
				}
				else
					pNode = pNode->pNext;
			}
			return iCountRemoved;
		}
	}

	template<typename CalcT>
	bool find(CalcT pCalc, EleT * pEle)
	{
		if(!m_head)
		{
			return false;
		}
		else
		{
			node_t * pNode = m_head;
			while(pNode && !pCalc(pNode->data))
				pNode = pNode->pNext;
			if(pNode)
			{
				*pEle = pNode->data;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	bool find(const EleT & ele)
	{
		if(!m_head)
		{
			return false;
		}
		else
		{
			node_t * pNode = m_head;
			while(pNode && (pNode->data != ele))
				pNode = pNode->pNext;
			return pNode != nullptr;
		}
	}

	bool has(const EleT & ele)
	{
		node_t * pNode = m_head;
		while(!pNode)
		{
			if(m_head->data == ele)
				return true;
			else
				pNode = pNode->pNext;
		}
		return false;
	}

	void destroy()
	{
		node_t * pNode = nullptr;
		while(m_head)
		{
			pNode = m_head;
			m_head = m_head->pNext;
			node_destroy(pNode);
		}
		m_size = 0;
	}
	EleT & operator [](int_x iIndex)
	{
		if(iIndex < m_size)
		{
			node_t * pNode = m_head;
			while(iIndex)
				pNode = pNode->pNext;
			return pNode->data;
		}
		else
			throw exp_out_of_bound(_T("无法找到元素"));
	}
	int_x size() const { return m_size;}
private :
	void node_create(node_t ** ppNode, const EleT & ele)
	{
		if(!m_buffered || !m_buffer)
		{
			*ppNode = new node_t;

			if(!(*ppNode))
				throw exp_out_of_memory(_T("内存不足，无法创建节点！"));
		}
		else
		{
			*ppNode = m_buffer;
			m_buffer = m_buffer->pNextBuffer;
		}

		(*ppNode)->data = ele;
		(*ppNode)->pNext = nullptr;
		(*ppNode)->pNextBuffer = nullptr;
	}

	void node_destroy(node_t * pNode)
	{
		if(m_buffered)
		{
			if(!m_buffer)
			{
				m_buffer = pNode;
			}
			else
			{
				m_buffer_end->pNextBuffer = pNode;
			}
			m_buffer_end = pNode;
		}
		else
		{
			delete pNode;
		}
	}
private:
	node_t * m_head;
	int_x m_size;

	// 先进后出的队列。
	bool m_buffered;
	node_t * m_buffer;
	node_t * m_buffer_end;
};

VENUS_END
