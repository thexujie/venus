#pragma once

#include "IDock.h"

VENUS_BEG

class V2D_API CDockNode : public ObjectT<IDockNode>
{
public:
	CDockNode(IControl * pControl);
	~CDockNode();

	IDockNode * DockNodeGetParent() const;
	IDockNode * DockNodeGetNext() const;
	IDockNode * DockNodeGetLast() const;
	IDockNode * DockNodeGetChild() const;

	void DockNodeSetParent(IDockNode * pNode);
	void DockNodeSetNext(IDockNode * pNode);
	void DockNodeSetLast(IDockNode * pNode);

	void DockNodeSetFixedSize(int_x iFixedSize);
	int_x DockNodeGetFixedSize() const;

	void DockNodeSetHorizontal(bool bHorizontal);
	bool DockNodeIsHorizontal() const;

	void DockNodeSetRect(rectix rect);
	rectix DockNodeGetRect() const;

	void DockNodeSetWeight(float_32 fWeight);
	float_32 DockNodeGetWeight() const;

	void DockNodeSetControl(IControl * pControl);
	IControl * DockNodeGetDockControl() const;

	void DockNodeSetContainer(IControl * pContainer);

	IDockNode * DockNodeInsert(IDockNode * pBefore, IDockNode * pNode, bool bUpdate);
	IDockNode * DockNodeAdd(IDockNode * pNode, bool bUpdate);
	int_x DockNodeGetChildCount() const;
	void DockNodeUpdateFramework();

protected:
	IDockNode * m_pParent;
	IDockNode * m_pLast;
	IDockNode * m_pNext;
	IDockNode * m_pChild;

	rectix m_rcNode;

	int_x m_iFixedSize;
	bool m_bHorizontal;
	float_32 m_fWeight;

	IControl * m_pControl;
	IControl * m_pContainer;

public:
	static const int_x DRAGBAR_SIZE = 3;
	static const int_x MINSIZE = 30;
};

VENUS_END
