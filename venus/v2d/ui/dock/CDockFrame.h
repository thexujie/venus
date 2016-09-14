#pragma once

#include "../controls/CControl.h"
#include "CDockNode.h"

VENUS_BEG

const oid_t OID_CDockFrame= {L"dockframe"};
class V2D_API CDockFrame : public CControl, public CDockNode
{
public:
	CDockFrame();
	~CDockFrame();
	const oid_t & GetOid() const;
	IObject * GetInterface(const oid_t & oid);

	void OnAdded(IControl * pContainer);
	void Layout();
	void OnMouseMove(pointix point);
	void OnMouseOut(pointix point);
	void OnMouseDownL(pointix point);
	void OnMouseUpL(pointix point);
	void RequestCursor(pointix point, CursorE & eCursor) const;
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;
	void AftOnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;
	void PaintDragBar(IPaint * pPaint, const IDockNode * node) const;

	IDockNode * FindHovingBorder(pointix point) const;

protected:
	pointix m_hovingPoint;
	IDockNode * m_pNodeHoving;
};

VENUS_END
