#include "stdafx.h"
#include "CListBox.h"
#include "CScrollBar.h"

VENUS_BEG

CListBox::CListBox()
	:m_bShowSelect(false), m_bFullCheck(true), m_bHideImage(false), m_bDisableSelectNull(false), m_bMultiCheck(false), 
	m_iHovingIndex(-1), m_iLineSpace(LINE_SPACE), m_iIconSize(ICON_SIZE), m_iItemSpace(ITEM_SPACE), m_iUpdating(0)
{
	m_bFocusAble = true;
	m_bRepaintScrollX = true;
	m_bRepaintScrollY = true;
	m_bRepaintFocusedChanged = true;
	m_bRepaintEnabledChanged = true;
	m_eBorderType = BorderType2;
	m_margin.set(3, 3, 3, 3);

	m_bShowSelect = false;
	m_bFullCheck = true;

	CScrollBar * pScrollBarX = new CScrollBar();
	CScrollBar * pScrollBarY = new CScrollBar();
	pScrollBarX->SetDynamic(true);
	pScrollBarY->SetDynamic(true);
	AddControl(pScrollBarX);
	AddControl(pScrollBarY);
	CControl::SetScrollBarX(pScrollBarX);
	CControl::SetScrollBarY(pScrollBarY);
}

CListBox::~CListBox()
{

}

const oid_t & CListBox::GetOid() const
{
	return OID_ListBox;
}

int_x CListBox::AddItem(const char_16 * szText, IImage * pImage, int_x iId)
{
	int_x iIndex = m_items.size();
	m_items.add(ListBoxItemT(iIndex, szText, iId, pImage));
	Update();
	return iIndex;
}

void CListBox::RemoveAll()
{
	SetHovingIndex(-1);
	SetSelectedIndex(-1);
	m_items.clear();
	Update();
}

int_x CListBox::GetItemCount() const
{
	return m_items.size();
}

const char_16 * CListBox::GetItemText(int_x iIndex) const
{
	return m_items[iIndex].Text;
}

void CListBox::BegUpdate()
{
	m_iUpdating = 1;
}

void CListBox::EndUpdate()
{
	if(m_iUpdating > 1)
	{
		m_iUpdating = 0;
		Update();
	}
}

void CListBox::Update()
{
	if(m_iUpdating > 0)
	{
		++m_iUpdating;
		return;
	}

	I2DDevice * pService = Get2DDevice();
	int_x iWidthExt = m_iItemSpace;
	if(m_bShowSelect)
		iWidthExt += m_iIconSize + m_iItemSpace;
	if(!m_bHideImage)
		iWidthExt += m_iIconSize + m_iItemSpace;
	iWidthExt += m_iItemSpace;
	for(int_x cnt = 0, len = m_items.size(); cnt < len; ++cnt)
	{
		ListBoxItemT & item = m_items[cnt];
		if(item.ContentSize.is_empty())
		{
			textsize_t tSize = pService->GetTextSize(item.Text, item.Text.length(), m_font);
			item.ContentSize.set(tSize.w, tSize.h);
		}
		item.Size.set(item.ContentSize.w + iWidthExt, item.ContentSize.h + m_iLineSpace * 2);
	}
	UpdateScroll();
	Repaint();
}

void CListBox::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	sizeix szClient = GetClient().size;
	int_x iTempX = -GetScrollPosX();
	int_x iCheckX = 0;
	int_x iIconX = 0;
	int_x iContentX = 0;
	if(m_bShowSelect)
	{
		iCheckX = iTempX + m_iItemSpace;
		iTempX += m_iItemSpace + m_iIconSize;
	}
	if(!m_bHideImage)
	{
		iIconX = iTempX + m_iItemSpace;
		iTempX += m_iItemSpace + m_iIconSize;
	}

	iContentX = iTempX + m_iItemSpace;
	int_x iY = -GetScrollPosY(), iItemH = 0;
	for(int_x cnt = 0, size = m_items.size(); cnt < size; ++cnt, iY += iItemH)
	{
		const ListBoxItemT & item = m_items[cnt];
		//iItemH = item.Size.h;
		iItemH = m_font.size + m_iLineSpace * 2;

		if(iY >= szClient.h)
			break;
		else if(iY + iItemH <= 0)
			continue;

		int_x iItemY = iY + m_iLineSpace;

		if(cnt == m_iHovingIndex)
		{
			pPaint->FillRect(0, iY, szClient.w, iItemH, SysColorBackHoving);
			if(item.Selected)
				pPaint->DrawRect(0, iY, szClient.w - 1, iItemH - 1, SysColorFrameHoving);
		}
		else if(item.Selected)
		{
			pPaint->FillRect(0, iY, szClient.w, iItemH, IsFocused() ? SysColorBackSelected : SysColorBackDisable);
			//if(cnt >0 && m_items[cnt - 1].Selected && cnt - 1 != m_iHovingIndex)
			//	pPaint->DrawLine(0, iY, szClient.w, iY, IsFocused() ? SysColorFrameHoving : SysColorFrameDisable);
		}
		else {}

		if(m_bShowSelect)
		{
			uint_32 clrBack = 0;
			uint_32 clrFrame = 0;
			if(!IsAviliable())
			{
				clrBack = SysColorBackDisable;
				clrFrame = SysColorFrameDisable;
			}
			else if(cnt == m_iHovingIndex)
			{
				if(m_bFocused)
				{
					clrBack = SysColorBackHoving;
					clrFrame = SysColorFrameHoving;
				}
				else
				{
					clrBack = SysColorBackDisable;
					clrFrame = SysColorFrameDisable;
				}
			}
			else
			{
				clrBack = SysColorAuto;
				clrFrame = SysColorFrameNormal;
			}

			PaintCheck(pPaint, rectix(iCheckX, iItemY, m_iIconSize, m_iIconSize).expand(-1),
				clrBack, clrFrame, m_uiForeColor, item.Selected);
		}

		if(item.Image && !m_bHideImage)
			pPaint->DrawImage(item.Image, iIconX, iItemY);

		pPaint->DrawString(item.Text, m_font, pointix(iContentX, iItemY), IsAviliable() ? m_uiForeColor : SysColorForeDisable);
	}
}

sizeix CListBox::GetPreferedSize() const
{
	int_x iWidth = m_font.size, iHeight = 0;
	for(int_x cnt = 0, len = m_items.size(); cnt < len; ++cnt)
	{
		const ListBoxItemT & item = m_items[cnt];
		if(item.Size.w > iWidth)
			iWidth = item.Size.w;
		iHeight += item.Size.h;
	}
	return sizeix(iWidth, iHeight) + GetBorder().size();
}

void CListBox::OnVisualSizeChanged()
{
	CControl::OnVisualSizeChanged();
	UpdateScroll();
}

void CListBox::OnMouseMove(pointix point)
{
	CControl::OnMouseMove(point);
	SetHovingIndex(FindItem(point));
}

void CListBox::OnMouseOut(pointix point)
{
	CControl::OnMouseOut(point);
	SetHovingIndex(-1);
}

void CListBox::OnMouseDownL(pointix point)
{
	CControl::OnMouseDownL(point);
	if(m_iHovingIndex >= 0 || !m_bDisableSelectNull)
	{
		if(m_iHovingIndex < 0)
		{
			if(!m_bMultiCheck || !IsKeyDown(KeyCodeControl))
				SelectNone();
			else {}
		}
		else
		{
			if(m_bMultiCheck && IsKeyDown(KeyCodeControl))
				SelectItem(m_iHovingIndex, !IsItemSelected(m_iHovingIndex), true);
			else
				SelectItem(m_iHovingIndex, true, false);
		}
	}
}


void CListBox::OnMouseClickL(pointix point)
{
	CControl::OnMouseClickL(point);
	//if(m_iHovingIndex >= 0 && m_bShowSelect)
	//{
	//	int_x iIndex = -1;
	//	if(m_bFullCheck)
	//		iIndex = m_iHovingIndex;
	//	else
	//	{
	//		if(ITEM_SPACE <= point.x && point.x < ITEM_SPACE + m_font.size)
	//			iIndex = m_iHovingIndex;
	//	}
	//	if(iIndex >= 0)
	//	{
	//		bool bSelect = true;
	//		if(m_bMultiCheck && IsItemSelected(iIndex))
	//			bSelect = false;
	//		SelectItem(iIndex, bSelect);
	//	}
	//}
}

void CListBox::OnKeyDown(KeyCodeE eKeyCode)
{
	switch(eKeyCode)
	{
	case KeyCodeUp:
		//SelectItemPrev(m_iSelectIndex);
		break;
	case KeyCodeDown:
		//SelectItemNext(m_iSelectIndex);
		break;
	default:
		break;
	}
	return CControl::OnKeyDown(eKeyCode);
}

void CListBox::SelectItem(int_x iIndex, bool bSelect, bool bMultiSelect)
{
	bool bChanged = false;
	ListBoxItemT * pItem = nullptr;
	// 针对所有项
	if(iIndex < 0 || m_items.size() <= iIndex)
	{
		// SelectNone
		if(!bSelect)
		{
			for(int_x cnt = 0, size = m_items.size(); cnt < size; ++cnt)
			{
				ListBoxItemT * pTemp = &m_items[cnt];
				if(pTemp->Selected)
				{
					if(ItemSelecting(this, pTemp) == 0)
					{
						pTemp->SetSelected(false);
						ItemSelected(this, pTemp);
						bChanged = true;
					}
					else {}
				}
			}
		}
		// SelectAll
		else if(bMultiSelect)
		{

			for(int_x cnt = 0, size = m_items.size(); cnt < size; ++cnt)
			{
				ListBoxItemT * pTemp = &m_items[cnt];
				if(!pTemp->Selected)
				{
					if(ItemSelecting(this, pTemp) == 0)
					{
						pTemp->SetSelected(true);
						ItemSelected(this, pTemp);
						bChanged = true;
					}
					else
						break;
				}
			}
		}
		else
			throw exp_illegal_argument();
	}
	else
	{
		pItem = &m_items[iIndex];
		// 单选
		if(!bMultiSelect)
		{
			bool bSuccess = true;
			// 取消其他已选择项
			for(int_x cnt = 0, size = m_items.size(); cnt < size; ++cnt)
			{
				if(cnt == iIndex)
					continue;

				ListBoxItemT * pTemp = &m_items[cnt];
				if(pTemp->Selected)
				{
					if(ItemSelecting(this, pTemp) == 0)
					{
						pTemp->SetSelected(false);
						ItemSelected(this, pTemp);
						bChanged = true;
					}
					else
					{
						bSuccess = false;
						break;
					}
				}
			}

			if(bSuccess && pItem->Selected != bSelect)
			{
				if(ItemSelecting(this, pItem) == 0)
				{
					pItem->SetSelected(bSelect);
					ItemSelected(this, pItem);
					bChanged = true;
				}
				else {}
			}
		}
		// 多选
		else if(pItem->Selected != bSelect)
		{
			if(ItemSelecting(this, pItem) == 0)
			{
				pItem->SetSelected(bSelect);
				ItemSelected(this, pItem);
				bChanged = true;
			}
			else {}
		}
		else
		{
			// 多选，但重复选择同一项，忽略。
		}
	}

	if(bChanged)
	{
		SelectedItemChanged(this, pItem);
		Repaint();
	}
}

void CListBox::SelectItemNext(int_x iIndex)
{
	if(0 <= iIndex && iIndex < m_items.size() - 1)
		SelectItem(iIndex + 1);
}

void CListBox::SelectItemPrev(int_x iIndex)
{
	if(0 < iIndex && iIndex < m_items.size())
		SelectItem(iIndex - 1);
}

void CListBox::SelectItemByText(const char_16 * szText)
{
	if(textisempty(szText))
		return;

	int_x iIndex = 0;
	for(int_x cnt = 0, len = m_items.size(); cnt < len; ++cnt)
	{
		const ListBoxItemT & item = m_items[cnt];
		if(item.Text == szText)
		{
			iIndex = cnt;
			break;
		}
	}
	SelectItem(iIndex);
}

bool CListBox::IsItemSelected(int_x iIndex) const
{
	return m_items[iIndex].Selected;
}

void CListBox::SetHovingIndex(int_x iIndex)
{
	if(m_iHovingIndex != iIndex)
	{
		m_iHovingIndex = iIndex;
		Repaint();
	}
}

void CListBox::SetSelectedIndex(int_x iIndex)
{
	if(iIndex < 0)
		SelectNone();
	else
		SelectItem(iIndex, true, false);
}

int_x CListBox::GetSelectedIndex() const
{
	for(int_x cnt = 0, size = m_items.size(); cnt < size; ++cnt)
	{
		if(m_items[cnt].Selected)
			return cnt;
	}
	return -1;
}

void CListBox::SetCanSelectNull(bool bCanSelectNull)
{
	m_bDisableSelectNull = !bCanSelectNull;
}

bool CListBox::CanSelectNull() const
{
	return !m_bDisableSelectNull;
}

void CListBox::SetMultiSelect(bool bMultiSelect)
{
	m_bMultiCheck = bMultiSelect;
}

bool CListBox::IsMultiSelect() const
{
	return m_bMultiCheck;
}

void CListBox::SetShowSelect(bool bShowSelect)
{
	if(bShowSelect != m_bShowSelect)
	{
		m_bShowSelect = bShowSelect;
		Update();
	}
}

bool CListBox::IsShowSelect() const
{
	return m_bShowSelect;
}

void CListBox::SetFullCheck(bool bFullCheck)
{
	m_bFullCheck = bFullCheck;
}

bool CListBox::IsFullCheck() const
{
	return m_bFullCheck;
}

void CListBox::SetShowImage(bool bShowImage)
{
	if(m_bHideImage == bShowImage)
	{
		m_bHideImage = !bShowImage;
		Update();
	}
}

bool CListBox::IsShowImage() const
{
	return !m_bHideImage;
}

void CListBox::SelectNone()
{
	SelectItem(-1, false, false);
}

void CListBox::SelectAll()
{
	SelectItem(-1, true, true);
}

void CListBox::SelectReverse()
{
	if(m_items.empty())
		return;

	for(int_x cnt = 0, len = m_items.size(); cnt < len; ++cnt)
	{
		ListBoxItemT & item = m_items[cnt];
		if(ItemSelecting(this, &item) == 0)
		{
			item.SetSelected(!item.Selected);
			ItemSelected(this, &item);
		}
	}

	SelectedItemChanged(this, nullptr);
	Repaint();
}

void CListBox::UpdateScroll()
{
	int_x iWidth = 0, iHeight = 0;
	for(int_x cnt = 0, len = m_items.size(); cnt < len; ++cnt)
	{
		const ListBoxItemT & item = m_items[cnt];
		iHeight += item.Size.h;
		if(item.Size.w > iWidth)
			iWidth = item.Size.w;
	}
	SetScroll(iWidth, iHeight);
}

int_x CListBox::FindItem(pointix point) const
{
	point.y += GetScrollPosY();
	int_x iTop = 0, iBottom = 0, iHeight = 0, iIndex = -1;
	for(int_x cnt = 0, len = m_items.size(); cnt < len; ++cnt, iTop = iBottom)
	{
		const ListBoxItemT & item = m_items[cnt];
		iBottom = iTop + item.Size.h;
		if(iTop <= point.y && point.y < iBottom)
			return cnt;
	}
	return -1;
}

const ListBoxItemT & CListBox::GetItem(int_x iIndex) const
{
	return m_items[iIndex];
}

void CListBox::QueryDebugMenu(int_x & iBase, IMenu * pMenu) const
{
	CControl::QueryDebugMenu(iBase, pMenu);
	MenuT * pSubMenu = new MenuT();
	pSubMenu->AddItem(iBase + 1, L"ShowCheck", m_bShowSelect);
	pSubMenu->AddItem(iBase + 2, L"FullCheck", m_bFullCheck);
	pSubMenu->AddItem(iBase + 3, L"ShowImage", !m_bHideImage);
	pSubMenu->AddItem(iBase + 4, L"SelectNull", !m_bDisableSelectNull);
	pSubMenu->AddItem(iBase + 5, L"MultiCheck", m_bMultiCheck);
	pMenu->AddSplit();
	pMenu->AddItem(0, L"ListBox", pSubMenu);
	SafeRelease(pSubMenu);
	iBase += 10;
}

void CListBox::OnDebugMenu(int_x iBase, int_x iResult)
{
	iBase -= 10;
	switch(iResult - iBase)
	{
	case 1:
		SetShowSelect(!m_bShowSelect);
		break;
	case 2:
		SetFullCheck(!m_bFullCheck);
		break;
	case 3:
		SetShowImage(m_bHideImage);
		break;
	case 4:
		SetCanSelectNull(m_bDisableSelectNull);
		break;
	case 5:
		SetMultiSelect(!m_bMultiCheck);
		break;
	default:
		CControl::OnDebugMenu(iBase, iResult);
		break;
	}
}

VENUS_END
