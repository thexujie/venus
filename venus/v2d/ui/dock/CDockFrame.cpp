#include "stdafx.h"
#include "CDockFrame.h"

VENUS_BEG

CDockFrame::CDockFrame() : CDockNode(nullptr)
{
	m_bClipChild = true;
	rectix rcClient = GetClient();
	DockNodeSetRect(rectix(pointix(), rcClient.size));
}

CDockFrame::~CDockFrame()
{

}

const oid_t & CDockFrame::GetOid() const
{
	return OID_CDockFrame;
}

IObject * CDockFrame::GetInterface(const oid_t & oid)
{
	if(oid == OID_IDockNode)
		return (IDockNode *)this;
	else
		return CControl::GetInterface(oid);
}

void CDockFrame::OnAdded(IControl * pContainer)
{
	CControl::OnAdded(pContainer);
	Layout();
}

void CDockFrame::Layout()
{
	rectix rcClient = GetClient();
	DockNodeSetRect(rectix(pointix(), rcClient.size));
}

void CDockFrame::OnMouseMove(pointix point)
{
	CControl::OnMouseMove(point);
	if(!m_bMouseDownL)
	{
		IDockNode * pHoving = FindHovingBorder(point);
		if(m_pNodeHoving != pHoving)
		{
			m_pNodeHoving = pHoving;
			m_hovingPoint = point;
			//Repaint();
		}
	}
	else if(m_pNodeHoving && 
		m_pNodeHoving->DockNodeGetParent() && 
		m_pNodeHoving->DockNodeGetNext())
	{
		IDockNode * pLeft = m_pNodeHoving;
		IDockNode * pRight = m_pNodeHoving->DockNodeGetNext();
		rectix rcLeft = pLeft->DockNodeGetRect();
		rectix rcRight = pRight->DockNodeGetRect();
		sizeix szLeftMin(CDockNode::MINSIZE, CDockNode::MINSIZE);
		sizeix szRightMin(CDockNode::MINSIZE, CDockNode::MINSIZE);

		if(m_pNodeHoving->DockNodeGetParent()->DockNodeIsHorizontal())
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

					float_32 fWeight = m_pNodeHoving->DockNodeGetWeight() + m_pNodeHoving->DockNodeGetNext()->DockNodeGetWeight();
					pLeft->DockNodeSetWeight(fWeight * (float_32)left.w / (float_32)(left.w + right.w));
					pRight->DockNodeSetWeight(fWeight * (float_32)right.w / (float_32)(left.w + right.w));
					pLeft->DockNodeSetRect(left);
					pRight->DockNodeSetRect(right);
					if(pLeft->DockNodeGetFixedSize())
						pLeft->DockNodeSetFixedSize(left.w);
					if(pRight->DockNodeGetFixedSize())
						pRight->DockNodeSetFixedSize(right.w);
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

					float_32 fWeight = m_pNodeHoving->DockNodeGetWeight() + m_pNodeHoving->DockNodeGetNext()->DockNodeGetWeight();
					pLeft->DockNodeSetWeight(fWeight * (float_32)left.h / (float_32)(left.h + right.h));
					pRight->DockNodeSetWeight(fWeight * (float_32)right.h / (float_32)(left.h + right.h));
					pLeft->DockNodeSetRect(left);
					pRight->DockNodeSetRect(right);
					if(pLeft->DockNodeGetFixedSize())
						pLeft->DockNodeSetFixedSize(left.h);
					if(pRight->DockNodeGetFixedSize())
						pRight->DockNodeSetFixedSize(right.h);
					m_hovingPoint = hoving;
				}
			}
			else {}
		}
	}
}

void CDockFrame::OnMouseOut(pointix point)
{
	if(m_pNodeHoving)
	{
		m_pNodeHoving = nullptr;
		//Repaint();
	}
}

void CDockFrame::OnMouseDownL(pointix point)
{
	CControl::OnMouseDownL(point);
	Capture(true);
	Repaint();
}

void CDockFrame::OnMouseUpL(pointix point)
{
	Capture(false);
	CControl::OnMouseUpL(point);
	Repaint();
}

void CDockFrame::RequestCursor(pointix point, CursorE & eCursor) const
{
	if(m_pNodeHoving && m_pNodeHoving->DockNodeGetParent())
		eCursor = m_pNodeHoving->DockNodeGetParent()->DockNodeIsHorizontal() ? CursorSizeLR : CursorSizeTB;
}

void CDockFrame::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	PaintDragBar(pPaint, this);
}


void CDockFrame::AftOnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{

}

void CDockFrame::PaintDragBar(IPaint * pPaint, const IDockNode * pNode) const
{
	bool bHorizontal = pNode->DockNodeIsHorizontal();
	const IDockNode * pChild = pNode->DockNodeGetChild();
	while(pChild)
	{
		PaintDragBar(pPaint, pChild);
		if(!pChild->DockNodeGetNext())
			break;

		rectix rcChild = pChild->DockNodeGetRect();
		rectix rcBar;
		if(bHorizontal)
			rcBar.set(rcChild.right(), rcChild.y, CDockNode::DRAGBAR_SIZE, rcChild.h);
		else
			rcBar.set(rcChild.x, rcChild.bottom(), rcChild.w, CDockNode::DRAGBAR_SIZE);
		uint_32 uiColor = /*0xFF355238*/Colors::Gray;
		if(m_bMouseDownL && pChild == m_pNodeHoving)
			uiColor = Colors::Black;
		pPaint->FillRect(rcBar, uiColor);
		pChild = pChild->DockNodeGetNext();
	}
}

IDockNode * CDockFrame::FindHovingBorder(pointix point) const
{
	IDockNode * pFind = nullptr;
	IDockNode * pScope = (IDockNode *)this;
	while(pScope && !pFind)
	{
		rectix rect = pScope->DockNodeGetRect();
		if(!rect.is_point_in(point))
			break;

		IDockNode * pChild = pScope->DockNodeGetChild();
		if(!pChild)
			break;

		bool bHorizontal = pScope->DockNodeIsHorizontal();
		pScope = nullptr;
		while(pChild)
		{
			rectix rcChild = pChild->DockNodeGetRect();
			if(rcChild.is_point_in(point))
			{
				pScope = pChild;
				break;
			}
			else
			{
				rectix rcBorder;
				if(bHorizontal)
					rcBorder.set(rcChild.right(), rcChild.y, CDockNode::DRAGBAR_SIZE, rcChild.h);
				else
					rcBorder.set(rcChild.x, rcChild.bottom(), rcChild.w, CDockNode::DRAGBAR_SIZE);
				if(rcBorder.is_point_in(point))
				{
					pFind = pChild;
					break;
				}
				else
					pChild = pChild->DockNodeGetNext();
			}
		}
	}
	return pFind;
}

VENUS_END

