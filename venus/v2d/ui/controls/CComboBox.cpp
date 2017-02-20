#include "stdafx.h"
#include "CComboBox.h"
#include "CListBox.h"

VENUS_BEG

CComboBox::CComboBox() : m_iButtonState(0), m_bDropDown(false)
{
	m_padding.set(3, 4, 3 + m_font.size, 4);
	m_listBox.SetShowImage(false);
	m_listBox.ItemSelected += bind(this, &CComboBox::OnListBoxItemSelected);
}

CComboBox::~CComboBox()
{

}

int_x CComboBox::AddItem(const char_16 * szText, IImage * pImage, int_x iId)
{
	m_listBox.AddItem(szText, pImage, iId);
	return 0;
}

void CComboBox::OnMouseMove(pointix point)
{
	sizeix szClient = GetClient().size;
	int_x iFontH = m_font.size;
	if(point.x > szClient.w - iFontH)
	{
		SetButtonHoving(true);
		CTextLine::OnMouseMove(point);
	}
	else
	{
		SetButtonHoving(false);
		CTextLine::OnMouseMove(point);
	}
}

void CComboBox::OnMouseOut(pointix point)
{
	CTextLine::OnMouseOut(point);
	SetButtonHoving(false);
}

void CComboBox::OnMouseDownL(pointix point)
{
	CTextLine::OnMouseDownL(point);
	if(m_listBox.GetItemCount())
	{
		sizeix size = m_listBox.GetPreferedSize();
		if(size.w < GetWidth())
			size.w = GetWidth();
		m_listBox.SetSize(size);
		//m_listBox.SetSelectIndex(-1);
		if(m_bDropDown || IsButtonHoving())
		{
			if(IsButtonLocked())
				SetButtonLocked(false);
			else
			{
				SetButtonSelected(true);
				PopupControl(pointix(), &m_listBox);
				SetButtonSelected(false);
				if(IsMouseNcIn())
					SetButtonLocked(true);
			}
		}
		else
		{
			SetButtonLocked(false);
		}
	}
}

HitTestE CComboBox::NcHitTest(pointix point) const
{
	return CControl::NcHitTest(point);
}

void CComboBox::RequestCursor(pointix point, CursorE & eCursor) const
{
	sizeix szClient = GetClient().size;
	if(m_bDropDown)
		eCursor = CursorNormal;
	else
	{
		int_x iFontH = m_font.size;
		if(point.x > szClient.w - iFontH)
			eCursor = CursorNormal;
		else
			CTextLine::RequestCursor(point, eCursor);
	}
}

void CComboBox::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	if(m_bDropDown)
		pPaint->FillRect(rcClip, IsButtonSelected() ? 
			SysColorBackSelected : (IsMouseIn() ? SysColorBackHoving : SysColorBackNormal));
	CTextLine::OnPaint(pPaint, rcClip, pTheme);
	sizeix szClient = GetClient().size;
	int_x iFontH = m_font.size;
	if(szClient.w >= iFontH)
	{
		rectix rcArrow(szClient.w - iFontH, (szClient.h - iFontH) / 2, iFontH, iFontH);
		if(!m_bDropDown)
		{
			rectix rcTag(szClient.w - iFontH, 0, iFontH, szClient.h);

			uint_32 clrFill = 0, clrDraw = 0;
			if(IsButtonSelected())
			{
				clrFill = SysColorBackSelected;
				clrDraw = SysColorFrameSelected;
			}
			else if(IsButtonPressed())
			{
				clrFill = SysColorBackPressed;
				clrDraw = SysColorFramePressed;
			}
			else if(IsButtonHoving())
			{
				clrFill = SysColorBackHoving;
				clrDraw = SysColorFrameHoving;
			}
			else {}
			pPaint->FillRect(rcTag, clrFill);
			pPaint->DrawRect(rcTag.x, rcTag.y, rcTag.w - 1, rcTag.h - 1, clrDraw);
		}
		//pPaint->DrawLine(rcArrow.x, 0, rcArrow.x, szClient.h, Colors::Red);

		PaintArrowB(pPaint, rcArrow, Colors::Black);
	}
}

void CComboBox::SetButtonHoving(bool bHoving)
{
	if(bHoving != IsButtonHoving())
	{
		if(bHoving)
			m_iButtonState |= ItemStateHoving;
		else
			m_iButtonState &= ~ItemStateHoving;
		Repaint();
	}
}

bool CComboBox::IsButtonHoving() const
{
	return (m_iButtonState & ItemStateHoving) != 0;
}


void CComboBox::SetButtonPressed(bool bPressed)
{
	if(bPressed != IsButtonHoving())
	{
		if(bPressed)
			m_iButtonState |= ItemStatePressed;
		else
			m_iButtonState &= ~ItemStatePressed;
		Repaint();
	}
}

bool CComboBox::IsButtonPressed() const
{
	return (m_iButtonState & ItemStatePressed) != 0;
}

void CComboBox::SetButtonSelected(bool bSelected)
{
	if(bSelected != IsButtonSelected())
	{
		if(bSelected)
			m_iButtonState |= ItemStateSelected;
		else
			m_iButtonState &= ~ItemStateSelected;
		Repaint();
	}
}

bool CComboBox::IsButtonSelected() const
{
	return (m_iButtonState & ItemStateSelected) != 0;
}

void CComboBox::SetButtonLocked(bool bLocked)
{
	if(bLocked != IsButtonLocked())
	{
		if(bLocked)
			m_iButtonState |= BUTTON_STATE_LOCK;
		else
			m_iButtonState &= ~BUTTON_STATE_LOCK;
		Repaint();
	}
}

bool CComboBox::IsButtonLocked() const
{
	return (m_iButtonState & BUTTON_STATE_LOCK) != 0;
}

void CComboBox::SetDropDown(bool bDropDown)
{
	if(bDropDown != m_bDropDown)
	{
		m_bDropDown = bDropDown;
		SetTextSelectAble(!bDropDown);
		SetTextReadOnly(bDropDown);
	}
}

bool CComboBox::IsDropDown() const
{
	return m_bDropDown;
}

void CComboBox::Select(int_x iIndex)
{
	m_listBox.SetSelectedIndex(iIndex);
}

void CComboBox::SelectByText(const char_16 * szText)
{
	m_listBox.SelectItemByText(szText);
}

void CComboBox::QueryDebugMenu(int_x & iBase, IMenu * pMenu) const
{
	CTextLine::QueryDebugMenu(iBase, pMenu);
	object_ptr<MenuT> pMenuThis(new MenuT());
	pMenuThis->AddItem(iBase + 1, L"DropDown", m_bDropDown);
	pMenu->AddItem(0, L"ComboBox", pMenuThis);
	iBase += 10;
}

void CComboBox::OnDebugMenu(int_x iBase, int_x iResult)
{
	iBase -= 10;
	switch(iResult - iBase)
	{
	case 1:
		SetDropDown(!m_bDropDown);
		break;
	default:
		CTextLine::OnDebugMenu(iBase, iResult);
		break;
	}
}

int_x CComboBox::OnListBoxItemSelected(IControl * pListBox, ListBoxItemT * pItem)
{
	PopupControl(pointix(), nullptr);
	if(pItem->Index >= 0)
		SetText(pItem->Text);
	if(m_bShown)
		OnItemSelected(this, pItem->Index);
	SetButtonLocked(false);
	return 0;
}

VENUS_END
