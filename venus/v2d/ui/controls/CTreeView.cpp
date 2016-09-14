#include "stdafx.h"
#include "CTreeView.h"
#include "CScrollBar.h"

VENUS_BEG

CTreeView::CTreeView()
	: m_pHoveringItem(nullptr), m_pSelectedItem(nullptr),
	m_iRowSpace(ROW_SPACE), m_iColSpace(COL_SPACE), m_iTagSize(TAG_SIZE),
	m_bShowExpand(true), m_iUpdating(0), m_bDisableSelectNull(false), m_eExpandMode(TreeExpandModeStrickOrDBClick)
{
	m_bFocusAble = true;
	m_bRepaintFocusedChanged = true;
	m_bRepaintEnabledChanged = true;
	m_eBorderType = BorderType2;
	m_margin.set(3, 3, 3, 3);

	CScrollBar * pScrollBarX = new CScrollBar();
	CScrollBar * pScrollBarY = new CScrollBar();
	pScrollBarX->SetDynamic(true);
	pScrollBarY->SetDynamic(true);
	AddControl(pScrollBarX);
	AddControl(pScrollBarY);
	CControl::SetScrollBarX(pScrollBarX);
	CControl::SetScrollBarY(pScrollBarY);
}

CTreeView::~CTreeView()
{
	for(int_x cnt = 0, size = m_items.size(); cnt < size; ++cnt)
		SafeRelease(m_items[cnt]);
	m_items.clear();
}

const oid_t & CTreeView::GetOid() const
{
	return OID_TreeView;
}

void CTreeView::GetMeta(IMetaHelper * pHelper)
{
	CControl::GetMeta(pHelper);
	pHelper->help_evt(L"SelectedItemChanged", SelectedItemChanged);
	pHelper->help_evt(L"ItemExpanding", ItemExpanding);
	pHelper->help_evt(L"ItemExpanded", ItemExpanded);
}

TreeItemT * CTreeView::AddItem(const char_16 * szText, TreeItemT * pParent, IImage * pImage, int_x iUserData)
{
	TreeItemT * pItem = nullptr;
	pItem = new TreeItemT(szText, pImage, pParent);
	pItem->UserData = iUserData;
	return AddItem(pItem);
}

TreeItemT * CTreeView::AddItem(TreeItemT * pItem)
{
	if(pItem->Parent)
	{
		Assert(HasItem(pItem->Parent));
		pItem->Parent->Items.add(object_ptr<TreeItemT>(pItem));
	}
	else
		m_items.add(pItem);
	
	InvalidItem(pItem, true);
	Update();
	return pItem;
}

void CTreeView::Remove(TreeItemT * pItem)
{
	Assert(HasItem(pItem));
	TreeItemT * pParent = pItem->Parent;
	if(pParent)
		pParent->Items.remove(object_ref(pItem));
	else
		m_items.remove(pItem);
	Update();
}

void CTreeView::Clear(TreeItemT * pItem)
{
	if(pItem)
	{
		if(pItem == m_pHoveringItem || HelperIsParent(pItem, m_pHoveringItem))
			SetHovingItem(nullptr);
		if(pItem == m_pSelectedItem || HelperIsParent(pItem, m_pSelectedItem))
			SetSelectedItem(nullptr);

		pItem->Items.clear();
	}
	else
	{
		SetHovingItem(nullptr);
		SetSelectedItem(nullptr);
		for(int_x cnt = 0, size = m_items.size(); cnt < size; ++cnt)
			SafeRelease(m_items[cnt]);
		m_items.clear();
	}
	Update();
}

void CTreeView::InvalidItem(TreeItemT * pItem, bool bContent)
{
	while(pItem)
	{
		if(bContent)
			pItem->ValidContent = false;
		pItem->Valid = false;
		bContent = false;
		if(pItem->Parent && pItem->Parent->Expanded)
			pItem = pItem->Parent;
		else
			break;
	}
}

void CTreeView::BegUpdate()
{
	m_iUpdating = true;
}

void CTreeView::EndUpdate()
{
	if(m_iUpdating)
	{
		m_iUpdating = false;
		Update();
	}
}

void CTreeView::Update()
{
	if(m_iUpdating <= 0)
	{
		for(int_x cnt = 0, size = m_items.size(); cnt < size; ++cnt)
		{
			TreeItemT * pItem = m_items[cnt];
			if(!pItem->Valid)
				_UpdateItem(pItem);
		}
		UpdateScroll();
		Repaint();
	}
	else
		++m_iUpdating;
}

bool CTreeView::HasItem(TreeItemT * pItem) const
{
	while(pItem->Parent)
		pItem = pItem->Parent;

	return m_items.find(pItem) >= 0;
}

TreeItemT * CTreeView::FindItem(pointix point) const
{
	int_x iX = point.x, iY = point.y;
	iY += GetScrollPosY();
	int_x iTop = 0, iBottom = 0;
	TreeItemT * pFind = nullptr;
	for(int_x cnt = 0, size = m_items.size(); cnt < size; ++cnt)
	{
		TreeItemT * pItem = m_items[cnt];
		iBottom = iTop + pItem->Height;
		if(iTop <= iY && iY < iBottom)
		{
			pFind = pItem;
			break;
		}
		else
			iTop = iBottom;
	}

	while(pFind)
	{
		iBottom = iTop + pFind->ItemHeight;
		if(iTop <= iY && iY < iBottom)
			break;

		iTop = iBottom;
		if(pFind->Expanded && pFind->Items.is_valid())
		{
			for(int_x cnt = 0, size = pFind->Items.size(); cnt < size; ++cnt)
			{
				TreeItemT * pItem = pFind->Items[cnt];
				iBottom = iTop + pItem->Height;
				if(iTop <= iY && iY < iBottom)
				{
					pFind = pItem;
					break;
				}
				else
					iTop = iBottom;
			}

		}
		else
			break;
	}
	return pFind;
}

void CTreeView::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	sizeix szClient = GetClient().size;
	int_x iX = -GetScrollPosX(), iY = -GetScrollPosY(), iHeight = 0, iBottom = 0;
	for(int_x cnt = 0, len = m_items.size(); cnt < len; ++cnt, iY = iBottom)
	{
		const TreeItemT * pItem = m_items[cnt];
		iBottom = iY + pItem->Height;
		if(iBottom < 0)
			continue;
		else if(iY >= szClient.h)
			break;
		else
			PaintItem(pPaint, iX, iY, pItem);
	}
}

void CTreeView::PaintItem(IPaint * pPaint, int_x iX, int_x iY, const TreeItemT * pItem) const
{
	if(!pItem)
		return;
	
	int_x iContentH = pItem->ContentHeight;
	int_x iItemH = m_iRowSpace + iContentH + m_iRowSpace;
	int_x iChildX = iX + m_iTagSize;
	int_x iChildY = iY + iItemH;
	sizeix szClient = GetClient().size;

	int_x iBottom = 0;

	// item 背景色
	if(iY + iItemH >= 0)
	{
		if(pItem->HighLighted)
		{
			pPaint->FillRect(0, iY, szClient.w, iItemH, SysColorBackHoving);
			if(pItem->Selected)
				pPaint->DrawRect(0, iY, szClient.w - 1, iItemH - 1, SysColorFrameHoving);
		}
		else if(pItem->Selected)
		{
			if(m_bFocused)
				pPaint->FillRect(0, iY, szClient.w, iItemH, SysColorBackSelected);
			else
				pPaint->FillRect(0, iY, szClient.w, iItemH, SysColorBackDisable);
		}
		else {}

		//pPaint->DrawLine(iX, iY + m_iLineSpace, iX, iY + m_iLineSpace + iItemH, Colors::Red);
		//pPaint->DrawRect(m_iItemSpace + iX, iY + m_iLineSpace, m_iTagSize, iItemH, Colors::Red);
		//pPaint->DrawRect(m_iItemSpace + iX + m_iTagSize + m_iItemSpace, iY + m_iLineSpace, iItemH, iItemH, Colors::Red);
		//pPaint->DrawRect(m_iItemSpace + iX + m_iTagSize + m_iItemSpace + iItemH + m_iItemSpace,
		//	iY + m_iLineSpace, pItem->ContentWidth, iItemH, Colors::Red);

		iX += m_iColSpace;
		iY += m_iRowSpace;
		// 折叠标记
		if(m_bShowExpand)
		{
			if(pItem->CanExpand())
			{
				uint_32 uiBack = SysColorBackNormal;
				uint_32 uiFore = SysColorFrameNormal;
				if(!IsEnabled())
				{
					uiBack = SysColorBackDisable;
					uiFore = SysColorFrameDisable;
				}
				else if(pItem->Expanded)
					uiBack = Colors::Black;
				else if(pItem == m_pHoveringItem)
				{
					uiBack = SysColorBackSelected;
					uiFore = SysColorFrameSelected;
				}
				else {}

				int_x iTagX = iX;
				int_x iTagY = iY + (iContentH - m_iTagSize) / 2;
				//PaintExpand(pPaint, iX + m_iItemSpace, iTagY, m_iTagSize, uiBack, uiFore, pItem->Expanded);
				if(pItem->Expanded)
					PaintArrowB(pPaint, rectix(iTagX, iTagY, m_iTagSize, m_iTagSize), uiFore);
				else
					PaintArrowR(pPaint, rectix(iTagX, iTagY, m_iTagSize, m_iTagSize), uiFore);
				//pPaint->FillRect(iTagX, iTagY, m_iTagSize, m_iTagSize, Colors::Red);
			}
			iX += m_iTagSize + m_iColSpace;
		}

		if(pItem->Image)
		{
			rectix rcImage(iX, iY + (iContentH - m_font.size) / 2, m_font.size, m_font.size);
			_PaintImage(pPaint, pItem->Image, rcImage);
			iX += m_font.size + m_iColSpace;
		}

		pPaint->DrawString(pItem->Text, m_font, {iX, iY}, IsEnabled() ? m_uiForeColor : SysColorForeDisable);
	}

	// 绘制子项
	if(pItem->Expanded && pItem->Items.is_valid())
	{
		for(int_x cnt = 0, len = pItem->Items.size(); cnt < len; ++cnt, iChildY = iBottom)
		{
			const TreeItemT * pSubItem = pItem->Items[cnt];
			iBottom = iChildY + pSubItem->Height;
			if(iBottom < 0)
				continue;
			else if(iChildY >= szClient.h)
				break;
			else
				PaintItem(pPaint, iChildX, iChildY, pSubItem);
		}
	}
}

void CTreeView::_InvalidAllItems(TreeItemT * pItem, bool bContent)
{
	if(!pItem)
	{
		for(int_x cnt = 0; cnt < m_items.size(); ++cnt)
			_InvalidAllItems(m_items[cnt], bContent);
	}
	else
	{
		for(int_x cnt = 0; cnt < pItem->Items.size(); ++cnt)
			_InvalidAllItems(pItem->Items[cnt], bContent);

		if(bContent)
			pItem->ValidContent = false;
		pItem->Valid = false;
	}
}

void CTreeView::_UpdateItem(TreeItemT * pItem)
{
	Assert(HasItem(pItem));
	if(m_iUpdating)
		return;

	if(pItem->Expanded)
	{
		for(int_x cnt = 0, size = pItem->Items.size(); cnt < size; ++cnt)
		{
			TreeItemT * pSubItem = pItem->Items[cnt];
			if(!pSubItem->Valid)
				_UpdateItem(pSubItem);
		}
	}

	if(!pItem->ValidContent)
	{
		pItem->ContentWidth = Get2DDevice()->GetTextSize(pItem->Text, pItem->Text.length(), m_font).w;
		pItem->ContentHeight = m_font.size;
		pItem->ValidContent = true;
	}

	if(!pItem->Valid)
	{
		pItem->ItemWidth = pItem->GetDepth() * m_iTagSize +  // tab
			m_iColSpace + m_iTagSize + // tag
			m_iColSpace + m_font.size + // image
			m_iColSpace + pItem->ContentWidth + m_iColSpace; // content
		pItem->ItemHeight = m_iRowSpace + pItem->ContentHeight + m_iRowSpace;

		int_x iWidth = pItem->ItemWidth;
		int_x iHeight = pItem->ItemHeight;

		if(pItem->Expanded && pItem->Items.is_valid())
		{
			for(int_x cnt = 0, len = pItem->Items.size(); cnt < len; ++cnt)
			{
				TreeItemT * pSubItem = pItem->Items[cnt];

				if(pSubItem->Width > iWidth)
					iWidth = pSubItem->Width;

				iHeight += pSubItem->Height;
			}
		}
		pItem->Width = iWidth;
		pItem->Height = iHeight;
		pItem->Valid = true;
	}
}
void CTreeView::_PaintImage(IPaint * pPaint, IImage * pImage, rectix rcImage) const
{
	if(!pPaint || !pImage || rcImage.is_empty())
		return;

	pImage->Prepare(rcImage.size);
	pPaint->DrawImage(pImage, rcImage);
}

void CTreeView::OnVisualSizeChanged()
{
	CControl::OnVisualSizeChanged();
	UpdateScroll();
}

void CTreeView::OnMouseMove(pointix point)
{
	CControl::OnMouseMove(point);
	SetHovingItem(FindItem(point));
}

void CTreeView::OnMouseOut(pointix point)
{
	CControl::OnMouseOut(point);
	SetHovingItem(nullptr);
}

void CTreeView::OnMouseDownL(pointix point)
{
	CControl::OnMouseDownL(point);
	if(m_pHoveringItem || !m_bDisableSelectNull)
		SelectItem(m_pHoveringItem);

	if(m_pHoveringItem && m_pHoveringItem->CanExpand())
	{
		if(m_eExpandMode == TreeExpandModeStrick ||
			m_eExpandMode == TreeExpandModeStrickOrDBClick)
		{
			if(m_bShowExpand)
			{
				int_x iScrollX = GetScrollPosX();
				int_x iLeft = m_iColSpace + m_iTagSize * m_pHoveringItem->GetDepth() - iScrollX;
				int_x iEndX = iLeft + m_iTagSize;
				if(iLeft <= point.x && point.x < iEndX)
					SetItemExpand(m_pHoveringItem, !m_pHoveringItem->Expanded);
			}
		}
		else if(m_eExpandMode == TreeExpandModeClick)
			SetItemExpand(m_pHoveringItem, !m_pHoveringItem->Expanded);
		else {}
	}
}

void CTreeView::OnMouseDBClickL(pointix point)
{
	CControl::OnMouseDBClickL(point);
	if(m_pSelectedItem && m_pSelectedItem->CanExpand())
	{
		if(m_eExpandMode == TreeExpandModeDBClick)
		{
			SetItemExpand(m_pSelectedItem, !m_pSelectedItem->Expanded);
		}
		else if(m_eExpandMode == TreeExpandModeStrickOrDBClick)
		{
			if(m_bShowExpand)
			{
				int_x iScrollX = GetScrollPosX();
				int_x iLeft = m_font.size * m_pSelectedItem->GetDepth() - iScrollX;
				int_x iEndX = iLeft + m_font.size;
				if(point.x < iLeft || point.x >= iEndX)
					SetItemExpand(m_pSelectedItem, !m_pSelectedItem->Expanded);
			}
			else
				SetItemExpand(m_pSelectedItem, !m_pSelectedItem->Expanded);
		}
		else {}
	}
}

void CTreeView::OnKeyDown(KeyCodeE eKeyCode)
{
	switch(eKeyCode)
	{
	case KeyCodeUp:
		SelectLastItem(m_pSelectedItem);
		break;
	case KeyCodeDown:
		SelectNextItem(m_pSelectedItem);
		break;
	case KeyCodeLeft:
		if(m_pSelectedItem)
		{
			if(m_pSelectedItem->CanExpand())
			{
				if(m_pSelectedItem->Expanded)
					SetItemExpand(m_pSelectedItem, false);
				else if(m_pSelectedItem->Parent)
					SelectItem(m_pSelectedItem->Parent);
				else {}
			}
			else if(m_pSelectedItem->Parent)
				SelectItem(m_pSelectedItem->Parent);
			else {}
		}
		break;
	case KeyCodeRight:
		if(m_pSelectedItem && m_pSelectedItem->CanExpand())
		{
			if(m_pSelectedItem->Expanded)
			{
				if(m_pSelectedItem->Items.is_valid())
					SelectItem(m_pSelectedItem->Items[0]);
			}
			else
				SetItemExpand(m_pSelectedItem, true);
		}
		break;
	default:
		break;
	}
	CControl::OnKeyDown(eKeyCode);
}

void CTreeView::RequestCursor(pointix point, CursorE & eCursor) const
{
	//if(!m_pHovingItem){}
	//else if(m_pHovingItem->SubItems.HasElement()) eCursor = CursorLink;
	//else{}
	CControl::RequestCursor(point, eCursor);
}

void CTreeView::OnScrollX(int_x iValue)
{
	Repaint();
}

void CTreeView::OnScrollY(int_x iValue)
{
	Repaint();
}

void CTreeView::UpdateScroll()
{
	if(m_iUpdating)
		return;

	int_x iWidth = 0, iHeight = 0;
	for(int_x cnt = 0, size = m_items.size(); cnt < size; ++cnt)
	{
		const TreeItemT * pItem = m_items[cnt];
		if(pItem->Width > iWidth)
			iWidth = pItem->Width;
		iHeight += pItem->Height;
	}
	if(!m_bShowExpand)
		iWidth -= m_iColSpace + m_iTagSize;

	SetScroll(iWidth, iHeight);
}

void CTreeView::QueryDebugMenu(int_x & iBase, IMenu * pMenu) const
{
	CControl::QueryDebugMenu(iBase, pMenu);
	MenuT * pSubMenu = new MenuT();
	MenuT * pMenuExpandMode = new MenuT();
	pSubMenu->AddItem(iBase + 1, L"ShowExpand", m_bShowExpand);
	pSubMenu->AddItem(0, L"Expand Mode", pMenuExpandMode);
	pSubMenu->AddSplit();
	pSubMenu->AddItem(iBase + 2, L"SelectNull", !m_bDisableSelectNull);
	pSubMenu->AddItem(iBase + 8, L"Inc RowSpace");
	pSubMenu->AddItem(iBase + 9, L"Dec RowSpace");
	pSubMenu->AddItem(iBase + 10, L"Inc ColSpace");
	pSubMenu->AddItem(iBase + 11, L"Dec ColSpace");
	pMenuExpandMode->AddItem(iBase + 3, L"None", m_eExpandMode == TreeExpandModeNone);
	pMenuExpandMode->AddItem(iBase + 4, L"Click", m_eExpandMode == TreeExpandModeClick);
	pMenuExpandMode->AddItem(iBase + 5, L"DBClick", m_eExpandMode == TreeExpandModeDBClick);
	pMenuExpandMode->AddItem(iBase + 6, L"Strick", m_eExpandMode == TreeExpandModeStrick);
	pMenuExpandMode->AddItem(iBase + 7, L"StrickOrDBClick", m_eExpandMode == TreeExpandModeStrickOrDBClick);
	pMenu->AddSplit();
	pMenu->AddItem(0, L"TreeView", pSubMenu);
	SafeRelease(pSubMenu);
	SafeRelease(pMenuExpandMode);
	iBase += 20;
}

void CTreeView::OnDebugMenu(int_x iBase, int_x iResult)
{
	iBase -= 20;
	switch(iResult - iBase)
	{
	case 1:
		SetShowExpand(!m_bShowExpand);
		break;
	case 2:
		SetCanSelectNull(m_bDisableSelectNull);
		break;
	case 3: SetExpandMode(TreeExpandModeNone); break;
	case 4: SetExpandMode(TreeExpandModeClick); break;
	case 5: SetExpandMode(TreeExpandModeDBClick); break;
	case 6: SetExpandMode(TreeExpandModeStrick); break;
	case 7: SetExpandMode(TreeExpandModeStrickOrDBClick); break;
	case 8: SetRowSpace(m_iRowSpace + 1); break;
	case 9: SetRowSpace(m_iRowSpace - 1); break;
	case 10: SetColSpace(m_iColSpace + 1); break;
	case 11: SetColSpace(m_iColSpace - 1); break;
	default:
		CControl::OnDebugMenu(iBase, iResult);
		break;
	}
}

void CTreeView::SetHovingItem(TreeItemT * pItem)
{
	TreeItemT * pTemp = nullptr;
	if(pItem != m_pHoveringItem)
	{
		if(m_pHoveringItem)
			m_pHoveringItem->HighLighted = false;

		m_pHoveringItem = pItem;

		if(m_pHoveringItem)
			m_pHoveringItem->HighLighted = true;
		Repaint();
	}
}

const TreeItemT * CTreeView::GetHovingItem() const
{
	return m_pHoveringItem;
}

void CTreeView::SetSelectedItem(TreeItemT * pItem)
{
	TreeItemT * pTemp = nullptr;
	if(pItem != m_pSelectedItem)
	{
		if(m_pSelectedItem)
			m_pSelectedItem->Selected = false;

		m_pSelectedItem = pItem;

		if(m_pSelectedItem)
			m_pSelectedItem->Selected = true;

		OnSelectedItemChanged();
		Repaint();
	}
}

TreeItemT * CTreeView::GetSelectedItem() const
{
	return m_pSelectedItem;
}

void CTreeView::SelectItem(TreeItemT * pItem)
{
#ifdef _DEBUG
	if(pItem && !HasItem(pItem))
		throw exp_illegal_argument();
#endif

	SetSelectedItem(pItem);

	if(pItem)
	{
		int_x iTop = GetItemTop(pItem);
		int_x iLineH = m_font.size + ROW_SPACE * 2;
		int_x iScrollY = GetScrollPosY();
		int_x iHeight = GetClient().h;
		if(iScrollY > iTop)
			SetScrollPosY(iTop);
		else if(iScrollY + iHeight < iTop + iLineH)
			SetScrollPosY(iTop + iLineH - iHeight);
		else {}
	}
}

void CTreeView::SelectLastItem(TreeItemT * pItem)
{
	TreeItemT * pTemp = HelperGetPrevItem(pItem);
	if(pTemp)
		SelectItem(pTemp);
}

void CTreeView::SetRowSpace(int_x iRowSpace)
{
	iRowSpace = saturate<int_x>(iRowSpace, 0, 0xFF);
	if(m_iRowSpace != iRowSpace)
	{
		m_iRowSpace = iRowSpace;
		_InvalidAllItems(nullptr, false);
		Update();
	}
}

int_x CTreeView::GetRowSpace() const
{
	return m_iRowSpace;
}

void CTreeView::SetColSpace(int_x iColSpace)
{
	iColSpace = saturate<int_x>(iColSpace, 0, 0xFF);
	if(m_iColSpace != iColSpace)
	{
		m_iColSpace = iColSpace;
		_InvalidAllItems(nullptr, false);
		Update();
	}
}

int_x CTreeView::GetColSpace() const
{
	return m_iColSpace;
}

void CTreeView::SelectNextItem(TreeItemT * pItem)
{
	TreeItemT * pTemp = HelperGetNextItem(pItem);
	if(pTemp)
		SelectItem(pTemp);
}

void CTreeView::SetItemExpand(TreeItemT * pItem, bool bExpand)
{
	if(bExpand != pItem->Expanded)
	{
		if(BeforeExpand(pItem))
		{
			pItem->Expanded = bExpand;
			ItemExpanded(this, pItem);
			InvalidItem(pItem, false);
			Update();
		}
	}
}

void CTreeView::EnsureVisible(TreeItemT * pItem)
{
	intx2 i2Scroll = GetScrollPos();
	rectix rcClient = GetClient();
	int_x iTop = GetItemTop(pItem);
	if(iTop - i2Scroll.y < 0)
		i2Scroll.y = iTop;
	else if(iTop + pItem->ItemHeight - i2Scroll.y > rcClient.h)
		i2Scroll.y = iTop + pItem->ItemHeight - rcClient.h;
	else {}
	SetScrollPos(i2Scroll.x, i2Scroll.y);
}

void CTreeView::SetCanSelectNull(bool bCanSelectNull)
{
	m_bDisableSelectNull = !bCanSelectNull;
}

bool CTreeView::CanSelectNull()
{
	return !m_bDisableSelectNull;
}

void CTreeView::SetShowExpand(bool bShowExpand)
{
	if(bShowExpand != m_bShowExpand)
	{
		m_bShowExpand = bShowExpand;
		_InvalidAllItems(nullptr, false);
		Update();
		Repaint();
	}
}

bool CTreeView::IsShowExpand() const
{
	return m_bShowExpand;
}


void CTreeView::SetExpandMode(TreeExpandModeE eExpandMode)
{
	m_eExpandMode = eExpandMode;
}

TreeExpandModeE CTreeView::GetExpandMode() const
{
	return m_eExpandMode;
}

int_x CTreeView::GetLineHeight() const
{
	return m_font.size + m_iRowSpace * 2;
}

int_x CTreeView::GetItemTop(TreeItemT * pItem) const
{
	int_x iTop = 0;
	TreeItemT * pParent = nullptr;
	TreeItemT * pTemp = nullptr;
	while(pItem->Parent)
	{
		pParent = pItem->Parent;
		for(int_x cnt = 0, len = pParent->Items.size(); cnt < len; ++cnt)
		{
			pTemp = pParent->Items[cnt];
			if(pItem != pTemp)
				iTop += pTemp->Height;
			else
				break;
		}
		iTop += pParent->ItemHeight;
		pItem = pParent;
	}

	for(int_x cnt = 0, len = m_items.size(); cnt < len; ++cnt)
	{
		pTemp = m_items[cnt];
		if(pItem != pTemp)
			iTop += pTemp->Height;
		else
			break;
	}

	return iTop;
}

bool CTreeView::BeforeExpand(TreeItemT * pItem)
{
	int_x iResult = ItemExpanding(this, pItem);
	return iResult == 0;
}

void CTreeView::OnSelectedItemChanged()
{
	SelectedItemChanged(this, m_pSelectedItem);
}

TreeItemT * CTreeView::HelperGetNextItem(TreeItemT * pItem)
{
	if(!pItem)
		return nullptr;
	else
	{
		if(pItem->Expanded && pItem->Items.is_valid())
			return pItem->Items[0];
		else if(pItem->Parent)
		{
			TreeItemT * pParent = pItem->Parent;
			while(pParent)
			{
				Assert(pParent->Items.is_valid());
				for(int_x cnt = 0, len = pParent->Items.size() - 1;
				cnt < len; ++cnt)
				{
					if(pItem == pParent->Items[cnt])
						return pParent->Items[cnt + 1];
				}
				pItem = pParent;
				pParent = pItem->Parent;
			}
			return nullptr;
		}
		else
			return nullptr;
	}
}

TreeItemT * CTreeView::HelperGetPrevItem(TreeItemT * pItem)
{
	if(!pItem)
		return nullptr;

	TreeItemT * pParent = pItem->Parent;
	TreeItemT * pPrev = nullptr;
	if(!pParent || !pParent->Items.is_valid())
		return nullptr;
	else if(!pItem)
		pPrev = pParent->Items[pParent->Items.size() - 1];
	else if(pItem->Parent)
	{
		// 先在同一层寻找。
		for(int_x cnt = 1, len = pParent->Items.size(); cnt < len; ++cnt)
		{
			if(pItem == pParent->Items[cnt])
			{
				pPrev = pParent->Items[cnt - 1];
				break;
			}
		}
	}
	else
	{
	}

	if(pPrev)
	{
		while(pPrev->Items.is_valid() && pPrev->Expanded)
			pPrev = pPrev->Items[pPrev->Items.size() - 1];
	}
	else
	{
		pPrev = pParent;
	}

	return pPrev;
}

int_x CTreeView::HelperGetItemCount(const TreeItemT::TreeItemCollection & items)
{
	int_x iCount = 0;
	for(int_x cnt = 0, size = items.size(); cnt < size; ++cnt)
	{
		iCount += HelperGetItemCount(items[cnt]->Items);
		++iCount;
	}
	return iCount;
}

bool CTreeView::HelperIsParent(const TreeItemT * pParent, const TreeItemT * pChild)
{
	const TreeItemT * pItem = pChild;
	while(pItem)
	{
		if(pItem->Parent == pParent)
			return true;
		pItem = pItem->Parent;
	}
	return false;
}

VENUS_END
