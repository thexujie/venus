#include "stdafx.h"
#include "CMenuBar.h"

VENUS_BEG

const int_x MAIN_MENU_SPACE_X = 7;
const int_x MAIN_MENU_SPACE_Y = 1;

CMenuBar::CMenuBar():
m_pMenu(nullptr), m_iHovingIndex(-1), m_iShowingIndex(-1)
{
	m_bRepaintFocusedChanged = true;
	m_bRepaintCaptureChanged = true;
	m_uiBackColor = SysColorBackNormal;
}

CMenuBar::~CMenuBar()
{
	SafeRelease(m_pMenu);
}

const cid_t & CMenuBar::GetOid() const
{
	return OID_MenuBar;
}

void CMenuBar::GetMeta(IMetaHelper * pHelper)
{
	CControl::GetMeta(pHelper);
	pHelper->help_evt(L"ItemSelected", ItemSelected);
}

void CMenuBar::OnMouseMove(pointix point)
{
	if(NcHitTest(point) == HitTestClient)
	{
		int_x iIndex = -1, iLeft = 0, iRight = 0;
		for(int_x cnt = 0, len = m_pMenu->GetItemCount(); cnt < len; ++cnt)
		{
			iRight = iLeft + m_arrWidths[cnt];
			if(iLeft <= point.x && point.x< iRight)
			{
				iIndex = cnt;
				break;
			}
			else
				iLeft = iRight;
		}
		if(iIndex >= 0)
		{
			SetHovingIndex(iIndex);
			UpdateMenu(m_iHovingIndex);
		}
	}
}

void CMenuBar::OnMouseOut(pointix point)
{
	SetHovingIndex(-1);
	//UpdateMenu(-1);
	CControl::OnMouseOut(point);
}

void CMenuBar::OnMouseDownL(pointix point)
{
	if(m_iShowingIndex < 0)
	{
		int_x iIndex = TryShowMenu(m_iHovingIndex);
		if(iIndex > 0)
			ItemSelected(this, iIndex);
	}
	else
		TryShowMenu(-1);
	CControl::OnMouseDownL(point);
}

void CMenuBar::OnMouseDownR(pointix point)
{
	CControl::OnMouseDownR(point);
}

void CMenuBar::SetShowMenu(IMenu * pMenu)
{
	SafeRelease(m_pMenu);
	m_arrWidths.resize(pMenu->GetItemCount(), 0);
	m_pMenu = pMenu;
	SafeAddRef(m_pMenu);
	
	for(int_x cnt = 0, len = m_pMenu->GetItemCount(); cnt < len; ++cnt)
	{
		const IMenuItem * pItem = m_pMenu->GetItem(cnt);
		const char_16 * szText = pItem->GetText();
		m_arrWidths[cnt] = MeasureMenuString(szText, textlen(szText), m_font).w + MAIN_MENU_SPACE_X * 2;
	}
	Repaint();
}

void CMenuBar::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	int_x iItemH = m_font.size + MAIN_MENU_SPACE_Y * 2;
	pPaint->DrawLine(0, iItemH, GetClient().w, iItemH, SysColorFrameNormal);
	int_x iX = 0, iWidth = 0;
	for(int_x cnt = 0, len = m_pMenu->GetItemCount(); cnt < len; ++cnt)
	{
		const IMenuItem * pItem = m_pMenu->GetItem(cnt);
		iWidth = m_arrWidths[cnt];

		const char_16 * szText = pItem->GetText();
		if(cnt == m_iShowingIndex)
		{
			pPaint->FillRect(iX, 0, iWidth, iItemH, SysColorBackSelected);
			pPaint->DrawRect(iX, 0, iWidth, iItemH, SysColorFrameSelected);
		}
		else if(cnt == m_iHovingIndex)
		{
			pPaint->FillRect(iX, 0, iWidth, iItemH, SysColorBackHoving);
			pPaint->DrawRect(iX, 0, iWidth, iItemH, SysColorFrameHoving);
		}
		PaintMenuString(pPaint, szText, textlen(szText), m_font, iX + MAIN_MENU_SPACE_X, MAIN_MENU_SPACE_Y, m_uiForeColor);
		iX += m_arrWidths[cnt];
	}
}

void CMenuBar::SetHovingIndex(int_x iIndex)
{
	if(iIndex != m_iHovingIndex)
	{
		m_iHovingIndex = iIndex;
		Repaint();
	}
}

void CMenuBar::SetResult(int_x iId)
{
	SetHovingIndex(-1);
}

sizeix CMenuBar::GetPreferedSize() const
{
	int_x iHeight = m_font.size + MAIN_MENU_SPACE_Y * 2 + 1;
	int_x iWidth = 0;
	for(int_x cnt = 0; cnt < m_arrWidths.size(); ++cnt)
		iWidth += m_arrWidths[cnt];
	return sizeix(iWidth, iHeight);
}

void CMenuBar::UpdateMenu(int_x iIndex)
{
	if(m_iShowingIndex >= 0 && iIndex != m_iShowingIndex)
	{
		m_iShowingIndex = iIndex;
		Repaint();

		if(iIndex < 0)
			PopupMenu(pointix(), nullptr);
		else
		{
			IMenu * pSubMenu = m_pMenu->GetItem(iIndex)->GetSubMenu();
			if(pSubMenu)
			{
				pointix point(0, GetHeight());
				for(int_x cnt = 0; cnt < iIndex; ++cnt)
					point.x += m_arrWidths[cnt];
				PopupMenu(ToNoneCl(point), pSubMenu);
			}
			else
				PopupMenu(pointix(), nullptr);
		}
	}
}

int_x CMenuBar::TryShowMenu(int_x iIndex)
{
	if(m_iShowingIndex >= 0)
	{
		UpdateMenu(iIndex);
		return 0;
	}
	else if(iIndex >= 0)
	{
		m_iShowingIndex = iIndex;
		Repaint();

		IMenu * pSubMenu = m_pMenu->GetItem(iIndex)->GetSubMenu();
		if(pSubMenu)
		{
			pointix point(0, m_font.size + MAIN_MENU_SPACE_Y * 2);
			for(int_x cnt = 0; cnt < iIndex; ++cnt)
				point.x += m_arrWidths[cnt];
			m_bPupupResponse = true;
			int_x iRet = PopupMenu(point, pSubMenu);
			m_bPupupResponse = false;
			m_iShowingIndex = -1;
			Repaint();
			return iRet;
		}
		else
			return 0;
	}
	else
		return 0;
}

VENUS_END
