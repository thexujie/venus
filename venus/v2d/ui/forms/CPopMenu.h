#pragma once

#include "CForm.h"

VENUS_BEG

class V2D_API CPopMenu : public CForm
{
public:
	CPopMenu(CPopMenu * pParent = nullptr);
	~CPopMenu();
	const oid_t & GetOid() const;

	void OnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;

	bool MouseActiving(pointix point) const;

	void PreOnKeyDown(KeyCodeE eKeyCode);
	void OnKeyDown(KeyCodeE eKeyCode);
	void OnMouseMove(pointix point);
	void OnMouseOut(pointix point);
	void OnMouseUpL(pointix point);

	void EndRun(int_x iResult);

	void SetShowMenu(IMenu * pMenu);
	int_x UpdateMenu(pointix point, IMenu * pMenu);
	void UpdateMenu(pointix point, int_x iParentW, int_x iItemH, IMenu * pMenu);

	void OnShow();
	void OnHide();

	void HovePrev();
	void HoveNext();
	void TryShowSubMenu(int_x iIndex);

protected:
	void SetHoverIndex(int_x iIndex);
protected:
	IMenu * m_pMenu;
	int_x m_iHoverIndex;

	CPopMenu * m_pMenuParent;
	CPopMenu * m_pMenuChild;
};

VENUS_END
