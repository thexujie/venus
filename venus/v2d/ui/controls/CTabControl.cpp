#include "stdafx.h"
#include "CTabControl.h"

VENUS_BEG

const int_x TAB_CONTROL_SPACE_X = 6;
const int_x TAB_CONTROL_SPACE_Y = 3;
const int_x TAB_CONTROL_SPACE_OFF = 2;

CTabControl::CTabControl()
	: m_iPaintIndex(0), m_iPaintIndexMax(0)
{
	m_bStatic = true;
	m_iHovingIndex = -1;
	m_iSelectedIndex = -1;
	m_iPaintIndex = 0;
	m_bDisAutoShowChild = true;
	m_margin.set(3, 3, 3, 3);
}

CTabControl::~CTabControl()
{

}

const oid_t & CTabControl::GetOid() const
{
	return OID_TabControl;
}

void CTabControl::GetMeta(IMetaHelper * pHelper)
{
	CControl::GetMeta(pHelper);
	pHelper->help_evt(L"ItemSelecting", ItemSelecting);
	pHelper->help_evt(L"ItemSelected", ItemSelected);
}

int_x CTabControl::AddTab(const char_16 * szText)
{
	int_x iIndex = m_panels.size();
	TabPanelT & panel = m_panels.add(TabPanelT(szText));
	if(m_iSelectedIndex < 0)
		m_iSelectedIndex = 0;

	panel.WidthOrg = Get2DDevice()->GetTextSize(panel.Name, panel.Name.length(), m_font).w;
	panel.Width = TAB_CONTROL_SPACE_X + panel.WidthOrg + TAB_CONTROL_SPACE_X;
	return iIndex;
}

void CTabControl::AddTabControl(int_x iIndex, IControl * pControl)
{
	TabPanelT & panel = m_panels[iIndex];
	AddControl(pControl);
	if(iIndex == m_iSelectedIndex)
	{
		if(m_bShown && pControl->IsVisible())
			pControl->SetShown(true);
	}
	panel.Controls.add(pControl);
}

int_x CTabControl::GetTabCount() const
{
	return m_panels.size();
}

void CTabControl::SetShown(bool bShown)
{
	if(bShown)
		CControl::SetShown(true);

	if(m_iSelectedIndex >= 0)
	{
		const TabPanelT & panel = m_panels[m_iSelectedIndex];
		for(int_x cnt = 0, len = panel.Controls.size(); cnt < len; ++cnt)
			panel.Controls[cnt]->SetShown(bShown);
	}

	NcRepaint();
	if(!bShown)
		CControl::SetShown(false);
}

void CTabControl::OnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	CControl::OnNcPaint(pPaint, rcClip, pTheme);
	rectix rect = GetRect();

	int_x iItemH = m_font.size;
	int_x iBaseX = TAB_CONTROL_SPACE_OFF, iX = 0, iY = 0, iWidth = 0, iHeight = 0;
	iY = TAB_CONTROL_SPACE_OFF + TAB_CONTROL_SPACE_Y + iItemH + TAB_CONTROL_SPACE_Y;
	pPaint->DrawLine(0, iY, rect.w, iY, SysColorFrameNormal);

	int_x iViewW = rect.w;
	if(m_iPaintIndexMax)
		iViewW -= iItemH + 1 + iItemH;
	for(int_x cnt = m_iPaintIndex, len = m_panels.size(); cnt < len; ++cnt)
	{
		const TabPanelT & panel = m_panels[cnt];

		if(cnt > m_iPaintIndex && iBaseX + panel.Width > iViewW)
			break;
		uint_32 uiBack = cnt == m_iHovingIndex ? SysColorBackHoving : SysColorBackNormal;
		if(cnt == m_iSelectedIndex)
		{
			iX = iBaseX - TAB_CONTROL_SPACE_OFF;
			iY = 0;
			iWidth = TAB_CONTROL_SPACE_OFF + panel.Width + TAB_CONTROL_SPACE_OFF;
			iHeight = TAB_CONTROL_SPACE_Y + iItemH + TAB_CONTROL_SPACE_Y + TAB_CONTROL_SPACE_OFF;
			pPaint->FillRect(iX, iY, iWidth, iHeight, SysColorForm);
			pPaint->DrawRect(iX, iY, iWidth, iHeight, SysColorFrameNormal);
			pPaint->DrawLine(iX + 1, iY + iHeight, iX + iWidth, iY + iHeight, SysColorForm);
		}
		else if(cnt == m_iSelectedIndex - 1)
		{
			iX = iBaseX;
			iY = TAB_CONTROL_SPACE_OFF;
			iWidth = panel.Width - TAB_CONTROL_SPACE_OFF;
			iHeight = TAB_CONTROL_SPACE_Y + iItemH + TAB_CONTROL_SPACE_Y;
			pPaint->FillRect(iX, TAB_CONTROL_SPACE_OFF, iWidth, iHeight, uiBack);
			pPaint->DrawRect(iX, iY, iWidth, iHeight, SysColorFrameNormal);
		}
		else if(cnt == m_iSelectedIndex + 1)
		{
			iX = iBaseX + TAB_CONTROL_SPACE_OFF;
			iY = TAB_CONTROL_SPACE_OFF;
			iWidth = panel.Width - TAB_CONTROL_SPACE_OFF;
			iHeight = TAB_CONTROL_SPACE_Y + iItemH + TAB_CONTROL_SPACE_Y;
			pPaint->FillRect(iX, TAB_CONTROL_SPACE_OFF, iWidth, iHeight, uiBack);
			pPaint->DrawRect(iX, iY, iWidth, iHeight, SysColorFrameNormal);
		}
		else
		{
			iX = iBaseX;
			iY = TAB_CONTROL_SPACE_OFF;
			iWidth = panel.Width;
			iHeight = TAB_CONTROL_SPACE_Y + iItemH + TAB_CONTROL_SPACE_Y;
			pPaint->FillRect(iX, TAB_CONTROL_SPACE_OFF, iWidth, iHeight, uiBack);
			pPaint->DrawRect(iX, iY, iWidth, iHeight, SysColorFrameNormal);
		}
		pPaint->DrawString(panel.Name.buffer, m_font, pointix(iBaseX + TAB_CONTROL_SPACE_X, iY + TAB_CONTROL_SPACE_Y), m_uiForeColor);

		iBaseX += panel.Width;
	}

	if(m_iPaintIndexMax)
	{
		rectix rcLeft(rect.w - iItemH * 2 - 1, TAB_CONTROL_SPACE_OFF, iItemH, iItemH + TAB_CONTROL_SPACE_Y);
		rectix rcRight(rect.w - iItemH, TAB_CONTROL_SPACE_OFF, iItemH, iItemH + TAB_CONTROL_SPACE_Y);
		if(m_iPaintIndex == 0)
		{
			pPaint->FillRect(rcLeft, SysColorBackDisable);
			pPaint->DrawRect(rcLeft.x, rcLeft.y, rcLeft.w - 1, rcLeft.h, SysColorFrameDisable);
			PaintArrowL(pPaint, rcLeft, SysColorForeDisable);
		}
		else if(m_iHovingIndex == TAB_INDEX_BUTTON_LEFT)
		{
			pPaint->FillRect(rcLeft, SysColorBackHoving);
			pPaint->DrawRect(rcLeft.x, rcLeft.y, rcLeft.w - 1, rcLeft.h, SysColorFrameHoving);
			PaintArrowL(pPaint, rcLeft, m_uiForeColor);
		}
		else
		{
			pPaint->FillRect(rcLeft, SysColorBackNormal);
			pPaint->DrawRect(rcLeft.x, rcLeft.y, rcLeft.w - 1, rcLeft.h, SysColorFrameNormal);
			PaintArrowL(pPaint, rcLeft, m_uiForeColor);
		}

		if(m_iPaintIndex == m_iPaintIndexMax)
		{
			pPaint->FillRect(rcRight, SysColorBackDisable);
			pPaint->DrawRect(rcRight.x, rcRight.y, rcRight.w - 1, rcRight.h, SysColorFrameDisable);
			PaintArrowR(pPaint, rcRight, SysColorForeDisable);
		}
		else if(m_iHovingIndex == TAB_INDEX_BUTTON_RIGHT)
		{
			pPaint->FillRect(rcRight, SysColorBackHoving);
			pPaint->DrawRect(rcRight.x, rcRight.y, rcRight.w - 1, rcRight.h, SysColorFrameHoving);
			PaintArrowR(pPaint, rcRight, m_uiForeColor);
		}
		else
		{
			pPaint->FillRect(rcRight, SysColorBackNormal);
			pPaint->DrawRect(rcRight.x, rcRight.y, rcRight.w - 1, rcRight.h, SysColorFrameNormal);
			PaintArrowR(pPaint, rcRight, m_uiForeColor);
		}
	}
}

rectix CTabControl::GetClient() const
{
	rectix rcClient = CControl::GetClient();
	int_x iOffset = TAB_CONTROL_SPACE_OFF + TAB_CONTROL_SPACE_Y + m_font.size + TAB_CONTROL_SPACE_Y;
	rcClient.y += iOffset;
	rcClient.h -= iOffset;
	if(rcClient.w < 0)
		rcClient.w = 0;
	if(rcClient.h < 0)
		rcClient.h = 0;
	return rcClient;
}

HitTestE CTabControl::NcHitTest(pointix point) const
{
	if(!rectix(pointix(), m_rect.size).is_point_in(point))
		return HitTestNoWhere;

	int_x iIndex = NcFindTab(point);
	if(iIndex >= 0 || 
		iIndex == TAB_INDEX_BUTTON_LEFT ||
		iIndex == TAB_INDEX_BUTTON_RIGHT)
		return HitTestInner;
	else
		return CControl::NcHitTest(point);
}

void CTabControl::OnNcMouseDown(pointix point, MouseButtonE eButton)
{
	if(!m_pMouseControl && !m_pCaptureControl && eButton == MouseButtonL)
	{
		if(m_iHovingIndex == TAB_INDEX_BUTTON_LEFT)
			SetPaintIndex(m_iPaintIndex - 1);
		else if(m_iHovingIndex == TAB_INDEX_BUTTON_RIGHT)
			SetPaintIndex(m_iPaintIndex + 1);
		else if(m_iHovingIndex >= 0)
			SetSelectedIndex(m_iHovingIndex);
		else {}
	}
	CControl::OnNcMouseDown(point, eButton);
}

void CTabControl::OnNcMouseMove(pointix point)
{
	if(!m_pMouseControl && !m_pCaptureControl)
		SetHovingIndex(NcFindTab(point));
	CControl::OnNcMouseMove(point);
}

void CTabControl::OnNcMouseOut(pointix point)
{
	SetHovingIndex(-1);
	CControl::OnNcMouseOut(point);
}

void CTabControl::OnSizeChanged()
{
	CControl::OnSizeChanged();

	sizeix szClient = GetClient().size;
	int_x iLength = m_panels.size();
	int_x iAllWidth = GetAllWidth();
	if(iLength <= 1 || iAllWidth <= szClient.w)
	{
		SetPaintIndexMax(0);
		SetPaintIndex(0);
	}
	else
	{
		int_x iItemH = m_font.size;
		int_x iButtonW = iItemH + 1 + iItemH;
		int_x iWidth = szClient.w - iButtonW;
		int_x iIndexMax = iLength;
		for(int_x cnt = iLength - 1; cnt > -1; --cnt)
		{
			const TabPanelT & panel = m_panels[cnt];
			if(iWidth >= panel.Width)
			{
				--iIndexMax;
				iWidth -= panel.Width;
			}
			else
				break;
		}
		SetPaintIndexMax(iIndexMax);
	}
}

void CTabControl::SetSelectedIndex(int_x iIndex)
{
	if(iIndex != m_iSelectedIndex)
	{
		ItemSelecting(this, iIndex, &iIndex);
		if(iIndex != m_iSelectedIndex)
		{
			if(m_iSelectedIndex >= 0)
			{
				const TabPanelT & panel = m_panels[m_iSelectedIndex];
				for(int_x cnt = 0, len = panel.Controls.size(); cnt < len; ++cnt)
					panel.Controls[cnt]->SetShown(false);
			}

			m_iSelectedIndex = iIndex;
			ItemSelected(this, iIndex);

			if(m_iSelectedIndex >= 0)
			{
				const TabPanelT & panel = m_panels[m_iSelectedIndex];
				for(int_x cnt = 0, len = panel.Controls.size(); cnt < len; ++cnt)
					panel.Controls[cnt]->SetShown(true);
			}
			NcRepaint();
		}
	}
}

void CTabControl::SetHovingIndex(int_x iIndex)
{
	if(iIndex != m_iHovingIndex)
	{
		m_iHovingIndex = iIndex;
		NcRepaint();
	}
}

int_x CTabControl::GetAllWidth() const
{
	int_x iWidth = TAB_CONTROL_SPACE_X;
	for(int_x cnt = 0, len = m_panels.size(); cnt < len; ++cnt)
		iWidth += m_panels[cnt].Width;
	return iWidth;
}

int_x CTabControl::NcFindTab(pointix point) const
{
	sizeix szClient = GetClient().size;
	int_x iItemH = m_font.size;
	int_x iHeaderH = TAB_CONTROL_SPACE_OFF + TAB_CONTROL_SPACE_Y + iItemH + TAB_CONTROL_SPACE_Y;
	if(point.y > iHeaderH || point.x < 0 || point.x >= szClient.w)
		return -1;

	int_x iViewW = szClient.w;
	int_x iLength = m_panels.size();
	if(m_iPaintIndexMax)
	{
		if(point.x > szClient.w - iItemH)
			return TAB_INDEX_BUTTON_RIGHT;
		else if(point.x > szClient.w - iItemH - iItemH - 1)
			return TAB_INDEX_BUTTON_LEFT;
		else
			iViewW -= iItemH + 1 + iItemH;
	}
	int_x iBaseX = TAB_CONTROL_SPACE_OFF, iX = 0, iWidth = 0;
	for(int_x cnt = m_iPaintIndex; cnt < iLength; ++cnt)
	{
		const TabPanelT & panel = m_panels[cnt];

		if(cnt > m_iPaintIndex && iBaseX + panel.Width > iViewW)
			break;

		if(cnt == m_iSelectedIndex)
		{
			iX = iBaseX - TAB_CONTROL_SPACE_OFF;
			iWidth = TAB_CONTROL_SPACE_OFF + panel.Width + TAB_CONTROL_SPACE_OFF;
		}
		else if(cnt == m_iSelectedIndex - 1)
		{
			iX = iBaseX;
			iWidth = panel.Width - TAB_CONTROL_SPACE_OFF;
		}
		else if(cnt == m_iSelectedIndex + 1)
		{
			iX = iBaseX + TAB_CONTROL_SPACE_OFF;
			iWidth = panel.Width - TAB_CONTROL_SPACE_OFF;
		}
		else
		{
			iX = iBaseX;
			iWidth = panel.Width;
		}

		if(iX <= point.x && point.x < iX + iWidth)
			return cnt;
		else
			iBaseX += panel.Width;
	}
	return -1;
}

void CTabControl::SetPaintIndex(int_x iIndex)
{
	if(iIndex < 0)
		iIndex = 0;
	else if(iIndex > m_iPaintIndexMax)
		iIndex = m_iPaintIndexMax;
	else{}

	if(iIndex != m_iPaintIndex)
	{
		m_iPaintIndex = iIndex;
		NcRepaint();
	}
}

void CTabControl::SetPaintIndexMax(int_x iIndexMax)
{
	if(iIndexMax < 0)
		iIndexMax = 0;

	if(iIndexMax != m_iPaintIndexMax)
	{
		m_iPaintIndexMax = iIndexMax;
		if(m_iPaintIndex > m_iPaintIndexMax)
			SetPaintIndex(iIndexMax);
	}
}

VENUS_END
