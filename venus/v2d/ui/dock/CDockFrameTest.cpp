#include "stdafx.h"
#include "CDockFrameTest.h"

VENUS_BEG

CDockFrameTest::CDockFrameTest()
{
	//m_uiBackColor = Colors::LightGray;
}

CDockFrameTest::~CDockFrameTest()
{

}

const oid_t & CDockFrameTest::GetOid() const
{
	return OID_DockFrame;
}

IControl * CDockFrameTest::GetDockTargetControl()
{
	return this;
}

frame_node_t * CDockFrameTest::InitialRoot(bool bHorizontal)
{
	m_root.horizontal = bHorizontal;
	return &m_root;
}

frame_node_t * CDockFrameTest::Insert(frame_node_t * pParent, frame_node_t * pBefore, IControl * pControl, bool bHorizontal,
	float_32 fWeight, int_x iSize, bool bFixedSize)
{
	if(!pParent)
		pParent = &m_root;
	if(pControl)
		AddControl(pControl);
	return pParent->insert(pBefore, pControl, bHorizontal, fWeight, iSize, bFixedSize);
}

void CDockFrameTest::OnVisualSizeChanged()
{
	CControl::OnVisualSizeChanged();
	m_root.set_rect(rectix(pointix(), GetClient().size));
}

void CDockFrameTest::OnMouseMove(pointix point)
{
	CControl::OnMouseMove(point);
	if(!m_bMouseDownL)
	{
		frame_node_t * pHoving = FindHovingBorder(&m_root, point);
		m_pNodeHoving = pHoving;
		m_hovingPoint = point;
	}
	else if(m_pNodeHoving && m_pNodeHoving->parent && m_pNodeHoving->next)
	{
		frame_node_t * pLeft = m_pNodeHoving;
		frame_node_t * pRight = m_pNodeHoving->next;
		rectix rcLeft = pLeft->rect;
		rectix rcRight = pRight->rect;
		sizeix szLeftMin = pLeft->get_minsize();
		sizeix szRightMin = pRight->get_minsize();

		if(m_pNodeHoving->parent->horizontal)
		{
			// 无论如何宽度都不够
			if(rcLeft.w + rcRight.w >= szLeftMin.w + szRightMin.w)
			{
				rectix left = rcLeft;
				rectix right = rcRight;
				pointix hoving = m_hovingPoint;
				int_x iOffset = point.x - hoving.x;
				if(rcLeft.w + iOffset < szLeftMin.w)
				{
					iOffset = szLeftMin.w - rcLeft.w;
					hoving.x = m_hovingPoint.x + iOffset;
				}
				else if(rcRight.w - iOffset < szRightMin.w)
				{
					iOffset = rcRight.w - szRightMin.w;
					hoving.x = m_hovingPoint.x + iOffset;
				}
				else
					hoving = point;

				if(iOffset)
				{
					left.w = rcLeft.w + iOffset;
					right.x = rcRight.x + iOffset;
					right.w = rcRight.w - iOffset;

					float_32 fWeight = m_pNodeHoving->weight + m_pNodeHoving->next->weight;
					pLeft->weight = fWeight * (float_32)left.w / (float_32)(left.w + right.w);
					pRight->weight = fWeight * (float_32)right.w / (float_32)(left.w + right.w);
					pLeft->set_rect(left);
					pRight->set_rect(right);
					m_hovingPoint = hoving;
				}
			}
			else {}
		}
		else
		{
			// 无论如何高度都不够
			if(rcLeft.h + rcRight.h >= szLeftMin.h + szRightMin.h)
			{
				rectix left = rcLeft;
				rectix right = rcRight;
				pointix hoving = m_hovingPoint;
				int_x iOffset = point.y - hoving.y;
				if(rcLeft.h + iOffset < szLeftMin.h)
				{
					iOffset = szLeftMin.h - rcLeft.h;
					hoving.y = m_hovingPoint.y + iOffset;
				}
				else if(rcRight.h - iOffset < szRightMin.h)
				{
					iOffset = rcRight.h - szRightMin.h;
					hoving.y = m_hovingPoint.y + iOffset;
				}
				else
					hoving = point;

				if(iOffset)
				{
					left.h = rcLeft.h + iOffset;
					right.y = rcRight.y + iOffset;
					right.h = rcRight.h - iOffset;

					float_32 fWeight = m_pNodeHoving->weight + m_pNodeHoving->next->weight;
					pLeft->weight = fWeight * (float_32)left.h / (float_32)(left.h + right.h);
					pRight->weight = fWeight * (float_32)right.h / (float_32)(left.h + right.h);
					pLeft->set_rect(left);
					pRight->set_rect(right);
					m_hovingPoint = hoving;
				}
			}
			else {}
		}
	}
}

void CDockFrameTest::OnMouseOut(pointix point)
{
	m_pNodeHoving = nullptr;
}

void CDockFrameTest::OnMouseDownL(pointix point)
{
	CControl::OnMouseDownL(point);
	Capture(true);
}

void CDockFrameTest::OnMouseUpL(pointix point)
{
	Capture(false);
	CControl::OnMouseUpL(point);
}

void CDockFrameTest::RequestCursor(pointix point, CursorE & eCursor) const
{
	if(m_pNodeHoving && m_pNodeHoving->parent)
		eCursor = m_pNodeHoving->parent->horizontal ? CursorSizeLR : CursorSizeTB;
}

void CDockFrameTest::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	PaintDragBar(pPaint, &m_root);
}


void CDockFrameTest::AftOnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{

}

void CDockFrameTest::PaintDragBar(IPaint * pPaint, const frame_node_t * node) const
{
	if(node->horizontal)
	{
		const frame_node_t * temp = node->child;
		while(temp)
		{
			if(!temp->next)
				break;

			rectix rcBar(temp->rect.right(), temp->rect.y, frame_node_t::DRAGBAR_SIZE, temp->rect.h);
			uint_32 uiColor = 0xFF355238;
			if(m_bMouseDownL && temp == m_pNodeHoving)
				uiColor = Colors::Black;
			pPaint->FillRect(rcBar, uiColor);
			temp = temp->next;
		}
	}
	else
	{
		const frame_node_t * temp = node->child;
		while(temp)
		{
			if(!temp->next)
				break;

			rectix rcBar(temp->rect.x, temp->rect.bottom(), temp->rect.w, frame_node_t::DRAGBAR_SIZE);
			uint_32 uiColor = Colors::LightGray;
			if(m_bMouseDownL && temp == m_pNodeHoving)
				uiColor = Colors::Black;
			pPaint->FillRect(rcBar, uiColor);
			temp = temp->next;
		}
	}
	const frame_node_t * temp = node->child;
	while(temp)
	{
		PaintDragBar(pPaint, temp);
		temp = temp->next;
	}
}

frame_node_t * CDockFrameTest::FindHovingBorder(frame_node_t * node, pointix point) const
{
	if(!node || !node->rect.is_point_in(point))
		return nullptr;

	if(node->horizontal)
	{
		frame_node_t * temp = node->child;
		while(temp)
		{
			if(!temp->next)
				break;

			rectix rcBorder(temp->rect.right(), temp->rect.y, frame_node_t::DRAGBAR_SIZE, temp->rect.h);
			if(rcBorder.is_point_in(point))
				return temp;
			temp = temp->next;
		}
	}
	else
	{
		frame_node_t * temp = node->child;
		while(temp)
		{
			if(!temp->next)
				break;

			rectix rcBorder(temp->rect.x, temp->rect.bottom(), temp->rect.w, frame_node_t::DRAGBAR_SIZE);
			if(rcBorder.is_point_in(point))
				return temp;
			temp = temp->next;
		}
	}
	frame_node_t * temp = node->child;
	while(temp)
	{
		frame_node_t * result = FindHovingBorder(temp, point);
		if(result)
			return result;
		temp = temp->next;
	}
	return nullptr;
}




frame_node_t::frame_node_t() :weight(0.0f), control(nullptr), horizontal(false),
parent(nullptr), last(nullptr), next(nullptr), child(nullptr)
{
}

frame_node_t::frame_node_t(frame_node_t * _pParent) : weight(0.0f), control(nullptr), horizontal(false),
parent(_pParent), last(nullptr), next(nullptr), child(nullptr)
{

}

frame_node_t::~frame_node_t()
{
	frame_node_t * node = child;
	while(node)
	{
		frame_node_t * temp = node;
		node = node->next;
		SafeDelete(temp);
	}
}

frame_node_t * frame_node_t::insert(frame_node_t * _before, IControl * _control,
	bool _horizontal, float_32 _weight, int_x _size, bool _fixed_size)
{
	frame_node_t * node_new = new frame_node_t(this);
	if(_fixed_size)
	{
		if(_size <= 0)
		{
			int_x count = get_count();
			float_32 weight = 0.0f;
			int_x fixed = 0;
			int_x space = count > 1 ? (count - 1) * DRAGBAR_SIZE : 0;

			get_weight(weight, fixed);
			_size = rect.w - fixed - space;
		}
		else {}
		node_new->fixed_size = true;
		if(horizontal)
			node_new->rect.w = _size;
		else
			node_new->rect.h = _size;
	}
	else
	{
		if(_weight < F32_EPSILON)
		{
			int_x count = get_count();
			if(count)
			{
				float_32 weight = 0.0f;
				int_x fixed = 0;
				get_weight(weight, fixed);
				_weight = weight / (float_32)count;
			}
			else
				_weight = 1.0f;
		}
		else {}
		if(_weight < F32_EPSILON)
			_weight = 1.0f;

		node_new->fixed_size = false;
		node_new->weight = _weight;
	}
	node_new->control = _control;
	node_new->horizontal = _horizontal;
	if(!_before)
	{
		frame_node_t * temp = child;
		child = node_new;
		child->next = temp;
		if(temp)
			temp->last = child;
	}
	else
	{
		frame_node_t * temp = _before->next;
		_before->next = node_new;
		node_new->next = temp;
		if(temp)
			temp->last = node_new;
		node_new->last = _before;
	}
	update();
	return node_new;
}

void frame_node_t::update()
{
	int_x iCount = get_count();
	if(iCount <= 0)
		return;

	// 横向
	if(horizontal)
	{
		//节点间间距总和。
		int_x iSpace = DRAGBAR_SIZE * (iCount - 1);
		//固定大小的节点大小总和。
		int_x iFixed = 0;
		//可变大小的节点权重总和。
		float_32 fWeight = 0.0f;
		//最后一个可变节点，最后一个节点的大小要填满整个父单元。
		frame_node_t * last_weight_node = nullptr;

		frame_node_t * temp = child;
		while(temp)
		{
			if(temp->fixed_size)
				iFixed += temp->rect.w;
			else
				fWeight += temp->weight;
			last_weight_node = temp;
			temp = temp->next;
		}
		int_x iWidthAlloc = rect.w - iSpace - iFixed;
		if(iWidthAlloc < 0)
			iWidthAlloc = 0;

		int_x iPos = 0;
		frame_node_t * node = child;
		while(node)
		{
			int_x iNodeW = 0;
			if(node->fixed_size)
				iNodeW = node->rect.w;
			else
			{
				if(node != last_weight_node)
					iNodeW = (int_x)(iWidthAlloc * node->weight / fWeight);
				else
					iNodeW = rect.w - iPos;
			}

			node->set_rect(rectix(rect.x + iPos, rect.y, iNodeW, rect.h));
			iPos += iNodeW;
			iPos += DRAGBAR_SIZE;
			node = node->next;
		}
	}
	else
	{
		//节点间间距总和。
		int_x iSpace = DRAGBAR_SIZE * (iCount - 1);
		//固定大小的节点大小总和。
		int_x iFixed = 0;
		//可变大小的节点权重总和。
		float_32 fWeight = 0.0f;
		//最后一个可变节点，最后一个节点的大小要填满整个父单元。
		frame_node_t * last_weight_node = nullptr;

		frame_node_t * temp = child;
		while(temp)
		{
			if(temp->fixed_size)
				iFixed += temp->rect.h;
			else
				fWeight += temp->weight;
			last_weight_node = temp;
			temp = temp->next;
		}
		int_x iHeightAlloc = rect.h - iSpace - iFixed;

		int_x iPos = 0;
		frame_node_t * node = child;
		while(node)
		{
			int_x iNodeH = 0;
			if(node->fixed_size)
				iNodeH = node->rect.h;
			else if(node != last_weight_node)
				iNodeH = (int_x)(iHeightAlloc * node->weight / fWeight);
			else
				iNodeH = rect.h - iPos;
			node->set_rect(rectix(rect.x, rect.y + iPos, rect.w, iNodeH));
			iPos += iNodeH;
			iPos += DRAGBAR_SIZE;
			node = node->next;
		}
	}
}

void frame_node_t::set_rect(rectix _rect)
{
	if(rect == _rect)
		return;
	rect = _rect;
	if(control)
		control->SetRect(rect);
	update();
}

int_x frame_node_t::get_count() const
{
	int_x iCount = 0;
	frame_node_t * node = child;
	while(node)
	{
		++iCount;
		node = node->next;
	}
	return iCount;
}

sizeix frame_node_t::get_minsize() const
{
	sizeix size;
	if(child)
	{
		if(horizontal)
		{
			size.h = MINSIZE;
			const frame_node_t * node = child;
			while(node)
			{
				sizeix size_child = node->get_minsize();
				size.w += size_child.w;
				if(size_child.h > size.h)
					size.h = size_child.h;
				node = node->next;
			}
		}
		else
		{
			size.w = MINSIZE;
			const frame_node_t * node = child;
			while(node)
			{
				sizeix size_child = node->get_minsize();
				size.h += size_child.h;
				if(size_child.w > size.w)
					size.w = size_child.w;
				node = node->next;
			}
		}
	}
	else
		size.set(MINSIZE, MINSIZE);
	return size;
}

void frame_node_t::get_weight(float_32 & weight, int_x & fixed_size) const
{
	fixed_size = 0;
	weight = 0.0f;
	frame_node_t * temp = child;
	while(temp)
	{
		if(temp->fixed_size)
			fixed_size += temp->rect.w;
		else
			weight += temp->weight;
		temp = temp->next;
	}
}

VENUS_END
