#pragma once

VENUS_BEG

const oid_t OID_IDockNode = {L"IDockNode"};

class IDockNode;
class IDockFrame;
class IDockPane;

/**
 * 停靠节点，是 IDockFrame 中的分割单元，呈树状关系。
 */
class V2D_API IDockNode : public IObject
{
public:
	virtual ~IDockNode() {}


	virtual IDockNode * DockNodeGetParent() const = 0;
	virtual IDockNode * DockNodeGetNext() const = 0;
	virtual IDockNode * DockNodeGetLast() const = 0;
	virtual IDockNode * DockNodeGetChild() const = 0;

	virtual void DockNodeSetParent(IDockNode * pNode) = 0;
	virtual void DockNodeSetNext(IDockNode * pNode) = 0;
	virtual void DockNodeSetLast(IDockNode * pNode) = 0;

	virtual void DockNodeSetFixedSize(int_x iFixedSize) = 0;
	virtual int_x DockNodeGetFixedSize() const = 0;

	virtual void DockNodeSetHorizontal(bool bHorizontal) = 0;
	virtual bool DockNodeIsHorizontal() const = 0;

	virtual void DockNodeSetRect(rectix rect) = 0;
	virtual rectix DockNodeGetRect() const = 0;

	virtual void DockNodeSetWeight(float_32 fWeight) = 0;
	virtual float_32 DockNodeGetWeight() const = 0;

	virtual void DockNodeSetControl(IControl * pControl) = 0;
	virtual IControl * DockNodeGetDockControl() const = 0;

	virtual void DockNodeSetContainer(IControl * pContainer) = 0;

	virtual IDockNode * DockNodeInsert(IDockNode * pBefore, IDockNode * pNode, bool bUpdate = false) = 0;
	virtual IDockNode * DockNodeAdd(IDockNode * pNode, bool bUpdate = false) = 0;
	virtual int_x DockNodeGetChildCount() const = 0;
	virtual void DockNodeUpdateFramework() = 0;
};

/**
* 停靠框架
* 停靠框架是一种特殊的接口，该接口可以接受 IDockPane（停靠窗格），并将这些 IDockPane 安放到预定的布局框架中。
*/
class V2D_API IDockFrame : public IDockNode
{
public:
	virtual ~IDockFrame() {}
};

/**
* 停靠窗格
*/
class V2D_API IDockPane
{
public:
	virtual ~IDockPane() {}
	virtual IControl * DockPaneGetControl() const = 0;
	virtual IDockNode * DockPaneGetNode() const = 0;
	virtual IDockFrame * DockPaneGetFrame() const = 0;
	virtual void DockPaneSetRect(rectix rect) = 0;
};

VENUS_END
