#pragma once

VENUS_BEG

const cid_t OID_IDockNode = {L"IDockNode"};

class IDockNode;
class IDockFrame;
class IDockPane;

/**
 * ͣ���ڵ㣬�� IDockFrame �еķָԪ������״��ϵ��
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
* ͣ�����
* ͣ�������һ������Ľӿڣ��ýӿڿ��Խ��� IDockPane��ͣ�����񣩣�������Щ IDockPane ���ŵ�Ԥ���Ĳ��ֿ���С�
*/
class V2D_API IDockFrame : public IDockNode
{
public:
	virtual ~IDockFrame() {}
};

/**
* ͣ������
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
