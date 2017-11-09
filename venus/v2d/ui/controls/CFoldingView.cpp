#include "stdafx.h"
#include "CFoldingView.h"

VENUS_BEG

CFoldingView::CFoldingView() :m_iUpdating(0)
{
	m_eBorderType = BorderType2Inactive;
	m_iRowSpace = 3;
	m_iColSpace = 3;
	m_iHovingTagIndex = -1;
	CScrollBar * pScrollBarX = new CScrollBar();
	CScrollBar * pScrollBarY = new CScrollBar();
	pScrollBarX->SetDynamic(true);
	pScrollBarY->SetDynamic(true);
	CControl::SetScrollBarX(pScrollBarX);
	CControl::SetScrollBarY(pScrollBarY);
	AddControl(pScrollBarX);
	AddControl(pScrollBarY);
	//SetAntoHideScrollBarY(false);
	m_font.weight = FONT_WEIGHT_BOLD;
	//m_font.size = 18;
}

CFoldingView::~CFoldingView()
{
	for_each(m_items, [](FoldingViewItemT *& pItem) { SafeRelease(pItem); });
	m_items.clear();
}

const cid_t & CFoldingView::GetOid() const
{
	return OID_FoldingView;
}

FoldingViewItemT * CFoldingView::AddItem(const char_16 * szText, int_x iMinWidth, IImage * pImage, int_x iUserData)
{
	FoldingViewItemT * pItem = new FoldingViewItemT();
	pItem->title = szText;
	pItem->minwidth = iMinWidth;
	pItem->userdata = iUserData;
	pItem->panel = new CPanel();
	pItem->panel->SetDynamic(true);
	return AddItem(pItem);
}

FoldingViewItemT * CFoldingView::AddItem(FoldingViewItemT * pItem)
{
	if(pItem->panel && pItem->panel->GetWidthMode() == WHModeFill)
	{
#ifdef _DEBUG
		log1(L"error, panel in CScrollView::AddItem shouldn't be WHModeFill.");
#endif
		pItem->panel->SetWidthMode(WHModeAbs);
	}
	pItem->index = m_items.size();
	pItem->siTitle.set_empty();
	pItem->siPanel.set_empty();
	pItem->siTitle.set_empty();
	m_items.add(pItem);
	AddControl(pItem->panel);
	Update();
	return pItem;
}

void CFoldingView::BegUpdate()
{
	if(m_iUpdating > 0)
		return;
	else
		m_iUpdating = 1;
}

void CFoldingView::EndUpdate()
{
	if(m_iUpdating > 1)
	{
		m_iUpdating = 0;
		Update();
	}
}

void CFoldingView::Update()
{
	if(m_iUpdating > 0)
	{
		++m_iUpdating;
		return;
	}

	IDevice2D * pDevice = Device2D();
	for(int_x cnt = 0; cnt < m_items.size(); ++cnt)
	{
		FoldingViewItemT * pItem = m_items[cnt];
		if(pItem->siTitle.is_empty())
		{
			textsize_t tsize = pDevice->GetTextSize(pItem->title, pItem->title.length(), m_font);
			pItem->siTitle.w = tsize.w;
			pItem->siTitle.h = m_font.size;

			pItem->siPanel.set_empty();
			pItem->siItem.set_empty();
		}

		if(pItem->panel && pItem->siPanel.is_empty())
		{
			pItem->siPanel.w = maxof(pItem->minwidth - m_iColSpace * 2, pItem->siTitle.w);
			pItem->siPanel.h = pItem->panel->GetSize().h;

			pItem->siItem.set_empty();
		}

		if(pItem->siItem.w == 0)
		{
			pItem->siItem.w = m_iColSpace + maxof(pItem->siTitle.w, pItem->siPanel.w) + m_iColSpace;
			if(pItem->collapsed)
				pItem->siItem.h = m_iRowSpace + pItem->siTitle.h + m_iRowSpace;
			else
				pItem->siItem.h = m_iRowSpace + pItem->siTitle.h + m_iRowSpace * 2 + pItem->siPanel.h + m_iRowSpace;
			pItem->siItem.h += m_iRowSpace;
		}
	}

	UpdateScroll();
	UpdateView();
	Repaint();
}

void CFoldingView::UpdateView()
{
	intx2 i2Scroll = GetScrollPos();
	int_x iViewW = _GetViewWidth();

	int_x iY = -i2Scroll.y;
	rectix rcPanel(m_iColSpace - i2Scroll.x, 0, iViewW - m_iColSpace * 2, 0);
	for(int_x cnt = 0; cnt < m_items.size(); ++cnt)
	{
		const FoldingViewItemT * pItem = m_items[cnt];
		pItem->panel->SetVisible(!pItem->collapsed);
		if(pItem->collapsed)
		{
			iY += pItem->siItem.h;
			continue;
		}
		rcPanel.y = iY;
		// title
		rcPanel.y += m_iRowSpace;
		rcPanel.y += pItem->siTitle.h;
		rcPanel.y += m_iRowSpace * 2;

		// panel
		rcPanel.h = pItem->siPanel.h;

		pItem->panel->SetRect(rcPanel);
		iY += pItem->siItem.h;
	}
}

void CFoldingView::UpdateScroll()
{
	intx2 i2Scroll;
	for(int_x cnt = 0; cnt < m_items.size(); ++cnt)
	{
		FoldingViewItemT * pItem = m_items[cnt];
		if(pItem->siItem.w > i2Scroll.w)
			i2Scroll.w = pItem->siItem.w;

		i2Scroll.h += pItem->siItem.h;
	}
	SetScroll(i2Scroll);
}

pointix CFoldingView::GetOffset() const
{
	//return -GetScrollPos();
	return pointix();
}

void CFoldingView::OnMouseMove(pointix point)
{
	CControl::OnMouseMove(point);
	point.y += GetScrollPosY();
	int_x iTagIndex = _FindTagIndex(point);
	_SetHovingTagIndex(iTagIndex);
}

void CFoldingView::OnMouseDownL(pointix point)
{
	CControl::OnMouseDownL(point);
	if(m_iHovingTagIndex >= 0)
		Repaint(_GetItemRect(m_iHovingTagIndex) - GetScrollPos());
}

void CFoldingView::OnMouseUpL(pointix point)
{
	CControl::OnMouseUpL(point);
	if(m_iHovingTagIndex >= 0)
		Repaint(_GetItemRect(m_iHovingTagIndex) - GetScrollPos());
}


void CFoldingView::OnMouseClickL(pointix point)
{
	if(m_iHovingTagIndex >= 0)
		SetItemExpanded(m_iHovingTagIndex, !IsItemExpanded(m_iHovingTagIndex));
	CControl::OnMouseClickL(point);
}

void CFoldingView::OnMouseOut(pointix point)
{
	if(m_iHovingTagIndex >= 0)
		_SetHovingTagIndex(-1);
	CControl::OnMouseOut(point);
}

void CFoldingView::OnScroll(intx2 scroll, intx2 offset)
{
	Repaint();
	CControl::OnScroll(scroll, offset);
}

void CFoldingView::OnVisualSizeChanged()
{
	CControl::OnVisualSizeChanged();
	for_each(m_items, [](FoldingViewItemT *& pItem) { pItem->siPanel.set_empty(); });
	Update();
}

void CFoldingView::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	rectix rcClient = GetClient();
	intx2 i2Scroll = GetScrollPos();
	int_x iViewW = _GetViewWidth();
	int_x iY = -i2Scroll.y;
	rectix rcTitle(m_iColSpace - i2Scroll.x, 0, rcClient.w - m_iColSpace * 2, 0);
	textformat_t format(m_font, AlignLeftCenterY, TextWrapNone);
	for(int_x cnt = 0; cnt < m_items.size(); ++cnt)
	{
		const FoldingViewItemT * pItem = m_items[cnt];
		rcTitle.y = iY + m_iRowSpace;
		rcTitle.h = pItem->siTitle.h;
		//pPaint->FillRect(rcTitle, Colors::Red);
		if(rcTitle.bottom() < 0)
		{
			iY += pItem->siItem.h;
			continue;
		}
		else if(rcTitle.y >= rcClient.h)
			break;
		else {}

		if(rcTitle.intersect(rcClip).is_valid())
		{
			//pPaint->FillRect(rcTitle, Colors::Red);
			pPaint->DrawString(pItem->title, format, rcTitle, SysColor);
			pPaint->DrawLine(0, rcTitle.bottom() + m_iRowSpace, rcClient.w, rcTitle.bottom() + m_iRowSpace, SysColor);
			rectix rcExpand(rcClient.w - m_font.size, rcTitle.y, m_font.size, m_font.size);
			if(pItem->hoving)
			{
				pPaint->FillRect(rcExpand, IsMouseDownL() ? SysColorBackPressed : SysColorBackHoving);
				pPaint->DrawRect(rcExpand.x, rcExpand.y, rcExpand.w - 1, rcExpand.h - 1,
					IsMouseDownL() ? SysColorFramePressed : SysColorFrameHoving);
			}
			if(pItem->collapsed)
				PaintArrowB(pPaint, rcExpand, m_uiForeColor);
			else
				PaintArrowT(pPaint, rcExpand, m_uiForeColor);
		}
		iY += pItem->siItem.h;
	}
}

void CFoldingView::QueryDebugMenu(int_x & iBase, IMenu * pMenu) const
{
	CControl::QueryDebugMenu(iBase, pMenu);
	chbufferw<64> temp;
	object_ptr<MenuT> pSubMenu(new MenuT());
	pSubMenu->AddItem(iBase + 1, temp.format(L"RowSpace Inc(%d + 1)", m_iRowSpace));
	pSubMenu->AddItem(iBase + 2, temp.format(L"RowSpace Dec(%d - 1)", m_iRowSpace));
	pSubMenu->AddItem(iBase + 3, temp.format(L"ColSpace Inc(%d + 1)", m_iColSpace));
	pSubMenu->AddItem(iBase + 4, temp.format(L"ColSpace Dec(%d - 1)", m_iColSpace));
	pMenu->AddSplit();
	pMenu->AddItem(0, L"ScrollView", pSubMenu);
	iBase += 10;
}

void CFoldingView::OnDebugMenu(int_x iBase, int_x iResult)
{
	iBase -= 10;
	switch(iResult - iBase)
	{
	case 1: SetRowSpace(m_iRowSpace + 1); break;
	case 2: SetRowSpace(m_iRowSpace - 1); break;
	case 3: SetColSpace(m_iColSpace + 1); break;
	case 4: SetColSpace(m_iColSpace - 1); break;
	default:
		CControl::OnDebugMenu(iBase, iResult);
		break;
	}
}

void CFoldingView::SetItemExpanded(int_x iIndex, bool bExpanded)
{
	if(iIndex < 0 || m_items.size() <= iIndex)
		throw exp_out_of_bound();

	FoldingViewItemT * pItem = m_items[iIndex];
	if(pItem->collapsed == bExpanded)
	{
		pItem->collapsed = !bExpanded;
		pItem->siItem.set_empty();
		Update();
	}
}

bool CFoldingView::IsItemExpanded(int_x iIndex) const
{
	if(iIndex < 0 || m_items.size() <= iIndex)
		throw exp_out_of_bound();

	const FoldingViewItemT * pItem = m_items[iIndex];
	return !pItem->collapsed;
}

void CFoldingView::SetRowSpace(int_x iRowSpace)
{
	if(iRowSpace < 0)
		iRowSpace = 0;

	if(m_iRowSpace != iRowSpace)
	{
		m_iRowSpace = iRowSpace;
		Update();
	}
}

int_x CFoldingView::GetRowSpace() const
{
	return m_iRowSpace;
}

void CFoldingView::SetColSpace(int_x iColSpace)
{
	if(iColSpace < 0)
		iColSpace = 0;

	if(m_iColSpace != iColSpace)
	{
		m_iColSpace = iColSpace;
		Update();
	}
}

int_x CFoldingView::GetColSpace() const
{
	return m_iColSpace;
}

int_x CFoldingView::_GetViewWidth() const
{
	rectix rcClient = GetClient();
	int_x iWidth = 0;
	for(int_x cnt = 0; cnt < m_items.size(); ++cnt)
	{
		const FoldingViewItemT * pItem = m_items[cnt];
		if(pItem->siPanel.w > iWidth)
			iWidth = pItem->siPanel.w;
	}
	if(iWidth < rcClient.w)
		iWidth = rcClient.w;
	return iWidth;
}

rectix CFoldingView::_GetItemRect(int_x iIndex) const
{
	if(iIndex < 0 || m_items.size() <= iIndex)
		return rectix();

	rectix rcItem(0, 0, GetClient().w, 0);
	for(int_x cnt = 0; cnt <= iIndex; ++cnt)
	{
		const FoldingViewItemT * pItem = m_items[cnt];
		if(cnt < iIndex)
			rcItem.y += pItem->siItem.h;
		else
		{
			rcItem.h = pItem->siItem.h;
		}
	}
	return rcItem;
}

rectix CFoldingView::_GetItemTitleRect(int_x iIndex)const
{
	if(iIndex < 0 || m_items.size() <= iIndex)
		return rectix();

	rectix rcItem(0, 0, _GetViewWidth(), 0);
	for(int_x cnt = 0; cnt <= iIndex; ++cnt)
	{
		const FoldingViewItemT * pItem = m_items[cnt];
		if(cnt < iIndex)
			rcItem.y += pItem->siItem.h;
		else
		{
			rcItem.y += m_iRowSpace;
			rcItem.h = pItem->siTitle.h;
		}
	}
	return rcItem;
}

rectix CFoldingView::_GetTagRect(rectix rcItem) const
{
	rectix rcClient = GetClient();
	rectix rcTag(rcClient.w - m_font.size, rcItem.y + m_iRowSpace, m_font.size, m_font.size);
	return rcTag;
}

int_x CFoldingView::_FindTagIndex(pointix point) const
{
	rectix rcClient = GetClient();
	int_x iOffsetY = GetScrollPosY();
	int_x iWidthMax = 0;
	for(int_x cnt = 0; cnt < m_items.size(); ++cnt)
	{
		const FoldingViewItemT * pItem = m_items[cnt];
		if(pItem->siPanel.w > iWidthMax)
			iWidthMax = pItem->siPanel.w;
	}

	rectix rcTag;
	int_x iTagIndex = -1;
	for(int_x cnt = 0; cnt < m_items.size(); ++cnt)
	{
		rectix rcTitle = _GetItemTitleRect(cnt);
		if(rcTitle.is_point_in(point))
		{
			rcTag.set(rcClient.w - m_font.size, rcTitle.y, m_font.size, m_font.size);
			if(rcTag.is_point_in(point))
			{
				iTagIndex = cnt;
				break;
			}
		}
	}
	return iTagIndex;
}

void CFoldingView::_SetHovingTagIndex(int_x iIndex)
{
	if(iIndex != m_iHovingTagIndex)
	{
		if(m_iHovingTagIndex >= 0)
		{
			FoldingViewItemT * pItemOld = m_items[m_iHovingTagIndex];
			pItemOld->hoving = false;
			rectix rcTitle = _GetItemTitleRect(pItemOld->index);
			Repaint(rcTitle - GetScrollPos());
		}
		m_iHovingTagIndex = iIndex;
		if(m_iHovingTagIndex >= 0)
		{
			FoldingViewItemT * pItemNew = m_items[m_iHovingTagIndex];
			pItemNew->hoving = true;
			rectix rcTitle = _GetItemTitleRect(pItemNew->index);
			Repaint(rcTitle - GetScrollPos());
		}
	}
}

VENUS_END
