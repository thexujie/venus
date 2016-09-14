#pragma once

#include "CControl.h"

VENUS_BEG

class V2D_API CMenuBar : public CControl, public IPopupHost
{
public:
	CMenuBar();
	~CMenuBar();
	const oid_t & GetOid() const;
	void GetMeta(IMetaHelper * pHelper);

	void OnMouseMove(pointix point);
	void OnMouseOut(pointix point);

	void OnMouseDownL(pointix point);
	void OnMouseDownR(pointix point);

	void SetShowMenu(IMenu * pMenu);
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;
	void SetHovingIndex(int_x iIndex);
	void SetResult(int_x iId);

	sizeix GetPreferedSize() const;
	void UpdateMenu(int_x iIndex);
	int_x TryShowMenu(int_x iIndex);
private:
	IMenu * m_pMenu;
	vector<int_x> m_arrWidths;
	int_x m_iHovingIndex;
	int_x m_iShowingIndex;
public:
	event<IControl *, int_x> ItemSelected;
};

VENUS_END
