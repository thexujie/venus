#pragma once

#include "CTextLine.h"
#include "CListBox.h"

VENUS_BEG

class V2D_API CComboBox : public CTextLine
{
public:
	CComboBox();
	~CComboBox();

	int_x AddItem(const char_16 * szText, IImage * pImage = nullptr, int_x iId = -1);
	
	void OnMouseMove(pointix point);
	void OnMouseOut(pointix point);
	void OnMouseDownL(pointix point);

	HitTestE NcHitTest(pointix point) const;
	void RequestCursor(pointix point, CursorE & eCursor) const;
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;

	void SetButtonHoving(bool bHoving);
	bool IsButtonHoving() const;
	void SetButtonPressed(bool bPressed);
	bool IsButtonPressed() const;
	void SetButtonSelected(bool bSelected);
	bool IsButtonSelected() const;
	void SetButtonLocked(bool bLocked);
	bool IsButtonLocked() const;
	void SetDropDown(bool bDropDown);
	bool IsDropDown() const;

	void Select(int_x iIndex);
	void SelectByText(const char_16 * szText);

	void QueryDebugMenu(int_x & iBase, IMenu * pMenu) const;
	void OnDebugMenu(int_x iBase, int_x iResult);
private:
	int_x OnListBoxItemSelected(IControl * pListBox, ListBoxItemT * pItem);
private:
	int_x m_iButtonState;
	CListBox m_listBox;
	bool m_bDropDown;

	static const int_x BUTTON_STATE_LOCK = 0x00010000;
public:
	event<IControl *, int_x> OnItemSelected;
};

VENUS_END
