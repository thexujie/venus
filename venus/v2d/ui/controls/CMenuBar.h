#pragma once

#include "CControl.h"

VENUS_BEG

class V2D_API CMenuBar : public CControl, public IPopupHost
{
public:
	CMenuBar();
	~CMenuBar();
	const cid_t & GetOid() const override;
	void GetMeta(IMetaHelper * pHelper) override;

	void OnMouseMove(pointix point) override;
	void OnMouseOut(pointix point) override;

	void OnMouseDownL(pointix point) override;
	void OnMouseDownR(pointix point) override;

	void SetShowMenu(IMenu * pMenu);
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;
	void SetHovingIndex(int_x iIndex);
	void SetResult(int_x iId) override;

	sizeix GetPreferedSize() const override;
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
