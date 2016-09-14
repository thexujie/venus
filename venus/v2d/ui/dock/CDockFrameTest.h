#pragma once

#include "../controls/CControl.h"
#include "IDock.h"

VENUS_BEG

class V2D_API frame_node_t
{
public:
	static const int_x DRAGBAR_SIZE = 4;
	static const int_x MINSIZE = 30;

	frame_node_t();
	frame_node_t(frame_node_t * _pParent);
	~frame_node_t();
	/**
	 * _horizontal true 子单元水平布局，false 子单元垂直布局。
	 * _weight 单元的权重，如果为 0，则自动计算权重。
	 * _size 单元的初始大小，如果为 0，则根据权重计算初始大小。
	 * _fixed 单元大小固定。
	 * _weight 为零时，首先检查 _size 是否为零，_size 非零，则根据 _size 计算 _weight，否则，均分父单元来计算 _weight。
	 */
	frame_node_t * insert(frame_node_t * _before, IControl * _control,
		bool _horizontal = false, float_32 _weight = 0.0f, int_x _size = 0, bool _fixed_size = false);

	void update();
	void set_rect(rectix _rect);

	int_x get_count() const;
	sizeix get_minsize() const;
	void get_weight(float_32 & weight, int_x & fixed_size) const;
public:
	frame_node_t * parent;
	frame_node_t * last;
	frame_node_t * next;

	bool fixed_size;
	bool manual_resize;
	float_32 weight;

	rectix rect;

	IControl * control;

	bool horizontal;
	frame_node_t * child;
};

const oid_t OID_DockFrame = {L"dockframe"};
class V2D_API CDockFrameTest : public CControl
{
public:
	CDockFrameTest();
	~CDockFrameTest();
	const oid_t & GetOid() const;

	IControl * GetDockTargetControl();
	frame_node_t * InitialRoot(bool bHorizontal);
	frame_node_t * Insert(frame_node_t * pParent, frame_node_t * pBefore, IControl * pControl, bool bHorizontal = false,
		float_32 fWeight = 0.0f, int_x iSize = 0, bool bFixedSize = false);

	frame_node_t & GetUnitRoot() { return m_root; }
	const frame_node_t & GetUnitRoot() const { return m_root; }

	void OnVisualSizeChanged();

	void OnMouseMove(pointix point);
	void OnMouseOut(pointix point);
	void OnMouseDownL(pointix point);
	void OnMouseUpL(pointix point);
	void RequestCursor(pointix point, CursorE & eCursor) const;
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;
	void AftOnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;
	void PaintDragBar(IPaint * pPaint, const frame_node_t * node) const;

	frame_node_t * FindHovingBorder(frame_node_t * node, pointix point) const;
protected:
	frame_node_t m_root;

	pointix m_hovingPoint;
	frame_node_t * m_pNodeHoving;
};

VENUS_END
