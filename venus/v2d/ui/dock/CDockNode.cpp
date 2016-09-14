#include "stdafx.h"
#include "CDockNode.h"

VENUS_BEG

CDockNode::CDockNode(IControl * pControl) :m_pParent(nullptr), m_pLast(nullptr), m_pNext(nullptr), m_pChild(nullptr),
m_bHorizontal(false), m_iFixedSize(0), m_fWeight(1.0f), m_pControl(pControl), m_pContainer(nullptr)
{

}

CDockNode::~CDockNode()
{
	IDockNode * pChild = m_pChild;
	while(pChild)
	{
		IDockNode * pTemp = pChild;
		pChild = pChild->DockNodeGetNext();
		SafeDelete(pTemp);
	}
}

IDockNode * CDockNode::DockNodeGetParent() const
{
	return m_pParent;
}

IDockNode * CDockNode::DockNodeGetNext() const
{
	return m_pNext;
}

IDockNode * CDockNode::DockNodeGetLast() const
{
	return m_pLast;
}

IDockNode * CDockNode::DockNodeGetChild() const
{
	return m_pChild;
}

void CDockNode::DockNodeSetParent(IDockNode * pNode)
{
	m_pParent = pNode;
}

void CDockNode::DockNodeSetNext(IDockNode * pNode)
{
	m_pNext = pNode;
}

void CDockNode::DockNodeSetLast(IDockNode * pNode)
{
	m_pLast = pNode;
}

void CDockNode::DockNodeSetFixedSize(int_x iFixedSize)
{
	m_iFixedSize = iFixedSize;
}

int_x CDockNode::DockNodeGetFixedSize() const
{
	return m_iFixedSize;
}

void CDockNode::DockNodeSetHorizontal(bool bHorizontal)
{
	m_bHorizontal = bHorizontal;
}

bool CDockNode::DockNodeIsHorizontal() const
{
	return m_bHorizontal;
}

float_32 CDockNode::DockNodeGetWeight() const
{
	return m_fWeight;
}

void CDockNode::DockNodeSetWeight(float_32 fWeight)
{
	m_fWeight = fWeight;
}

void CDockNode::DockNodeSetRect(rectix rect)
{
	if(m_rcNode == rect)
		return;
	m_rcNode = rect;
	//if(m_iFixedSize > 0 && m_pParent)
	//	m_iFixedSize = m_pParent->DockNodeIsHorizontal() ? m_rcNode.w : m_rcNode.h;

	if(m_pControl)
		m_pControl->SetRect(rect);
	DockNodeUpdateFramework();
}

rectix CDockNode::DockNodeGetRect() const
{
	return m_rcNode;
}

void CDockNode::DockNodeSetControl(IControl * pControl)
{
	if(pControl != m_pControl)
	{
		m_pControl = pControl;
		if(pControl->GetWidthMode() != WHModeAbs)
		{
			log1(L"CDockNode::DockNodeSetControl pControl->GetWidthMode() != WHModeAbs.");
			pControl->SetWidthMode(WHModeAbs);
		}
		if(pControl->GetHeightMode() != WHModeAbs)
		{
			log1(L"CDockNode::DockNodeSetControl pControl->GetHeightMode() != WHModeAbs.");
			pControl->SetHeightMode(WHModeAbs);
		}

		if(m_pControl)
			m_pControl->SetRect(m_rcNode);
	}
}

IControl * CDockNode::DockNodeGetDockControl() const
{
	return m_pControl;
}

void CDockNode::DockNodeSetContainer(IControl * pContainer)
{
	m_pContainer = pContainer;
	if(m_pControl)
		m_pContainer->AddControl(m_pControl);
	
	IDockNode * pNode = m_pChild;
	while(pNode)
	{
		pNode->DockNodeSetContainer(pContainer);
		pNode = pNode->DockNodeGetNext();
	}
}

IDockNode * CDockNode::DockNodeInsert(IDockNode * pBefore, IDockNode * pNode, bool bUpdate)
{
	if(pBefore == pNode)
		return nullptr;
	if(!pNode)
		return nullptr;
	if(!pBefore)
		pBefore = m_pChild;

	if(!m_pChild)
		m_pChild = pNode;
	else
	{
		IDockNode * pLast = pBefore;
		IDockNode * pNext = pBefore->DockNodeGetNext();
		if(pLast)
			pLast->DockNodeSetNext(pNode);
		if(pNext)
			pNext->DockNodeSetLast(pNode);

		pNode->DockNodeSetLast(pLast);
		pNode->DockNodeSetNext(pNext);
	}
	if(bUpdate)
		DockNodeUpdateFramework();

	pNode->DockNodeSetParent(this);

	return pNode;
}

IDockNode * CDockNode::DockNodeAdd(IDockNode * pNode, bool bUpdate)
{
	if(!pNode)
		return nullptr;

	if(!m_pChild)
		m_pChild = pNode;
	else
	{
		IDockNode * pTemp = m_pChild;
		while(pTemp)
		{
			IDockNode * pNext = pTemp->DockNodeGetNext();
			if(pNext)
				pTemp = pNext;
			else
				break;
		}
		pTemp->DockNodeSetNext(pNode);
		pNode->DockNodeSetLast(pTemp);
	}
	if(bUpdate)
		DockNodeUpdateFramework();

	pNode->DockNodeSetParent(this);
	if(m_pContainer)
		pNode->DockNodeSetContainer(m_pContainer);

	return pNode;
}

int_x CDockNode::DockNodeGetChildCount() const
{
	int_x iCount = 0;
	const IDockNode * pNode = m_pChild;
	while(pNode)
	{
		++iCount;
		pNode = pNode->DockNodeGetNext();
	}
	return iCount;
}

void CDockNode::DockNodeUpdateFramework()
{
	int_x iCount = DockNodeGetChildCount();
	if(iCount <= 0)
		return;

	//节点间间距总和。
	int_x iSpaceSize = DRAGBAR_SIZE * (iCount - 1);
	//固定大小的节点大小总和。
	int_x iFixedSizeTotal = 0;
	//可变大小的节点权重总和。
	float_32 fWeightTotal = 0.0f;
	//最后一个可变节点，最后一个节点的大小要填满整个父单元。
	IDockNode * pLastWeightNode = nullptr;

	IDockNode * pTemp = m_pChild;
	while(pTemp)
	{
		int_x iFixedSize = pTemp->DockNodeGetFixedSize();
		if(iFixedSize > 0)
			iFixedSizeTotal += iFixedSize;
		else
			fWeightTotal += pTemp->DockNodeGetWeight();
		pLastWeightNode = pTemp;
		pTemp = pTemp->DockNodeGetNext();
	}
	int_x iSizeAlloc = 0;
	if(m_bHorizontal)
		iSizeAlloc = m_rcNode.w - iSpaceSize - iFixedSizeTotal;
	else
		iSizeAlloc = m_rcNode.h - iSpaceSize - iFixedSizeTotal;

	if(iSizeAlloc < 0)
		iSizeAlloc = 0;

	int_x iPos = 0;
	IDockNode * pNode = m_pChild;
	while(pNode)
	{
		int_x iNodeSize = 0;
		int_x iFixedSize = pNode->DockNodeGetFixedSize();
		if(iFixedSize > 0)
			iNodeSize = iFixedSize;
		else if(pNode != pLastWeightNode)
			iNodeSize = (int_x)(iSizeAlloc * pNode->DockNodeGetWeight() / fWeightTotal);
		else
		{
			if(m_bHorizontal)
				iNodeSize = m_rcNode.w - iPos;
			else
				iNodeSize = m_rcNode.h - iPos;
		}

		if(m_bHorizontal)
			pNode->DockNodeSetRect(rectix(m_rcNode.x + iPos, m_rcNode.y, iNodeSize, m_rcNode.h));
		else
			pNode->DockNodeSetRect(rectix(m_rcNode.x, m_rcNode.y + iPos, m_rcNode.w, iNodeSize));

		iPos += iNodeSize;
		iPos += DRAGBAR_SIZE;
		pNode = pNode->DockNodeGetNext();
	}
}

VENUS_END

