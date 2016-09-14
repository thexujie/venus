#include "stdafx.h"
#include "CPane.h"

VENUS_BEG

CPane::CPane() : m_iCaptionHeight(CAPTION_HEIGHT_DEFAULT), m_bHideCaption(false), m_iSelectedIndex(-1)
{
	m_bStatic = true;
	m_bClipChild = true;
	m_bNotAcceptScroll = true;
	m_uiForeColor = 0xFFFFFFFF;
	m_bRepaintSelectedChanged = true;
}

CPane::~CPane()
{

}

const oid_t & CPane::GetOid() const
{
	return OID_Pane;
}

pointix CPane::GetOffset() const
{
	return GetScrollPos();
}

rectix CPane::GetVisual() const
{
	rectix rcVisual = CControl::GetVisual();
	if(!m_bHideCaption)
	{
		rcVisual.y += m_iCaptionHeight;
		rcVisual.h -= m_iCaptionHeight;
	}
	return rcVisual;
}

HitTestE CPane::NcHitTest(pointix point) const
{
	return CControl::NcHitTest(point);
}

void CPane::SetCaptionVisible(bool bCaptionVisible)
{
	if(bCaptionVisible == m_bHideCaption)
	{
		m_bHideCaption = !bCaptionVisible;
		OnVisualSizeChanged();
	}
}

bool CPane::IsShowCaptionVisible() const
{
	return !m_bHideCaption;
}

void CPane::SetCaptionHeight(int_x iCaptionHeight)
{
	if(iCaptionHeight != m_iCaptionHeight)
	{
		m_iCaptionHeight = iCaptionHeight;
		OnVisualSizeChanged();
	}
}

int_x CPane::GetCaptionHeight() const
{
	if(m_bHideCaption)
		return 0;
	return m_iCaptionHeight;
}

void CPane::AddPanel(IControl * pPanel)
{
	pPanel->SetVisible(false);
	AddControl(pPanel);
	rectix rcVisual = GetVisual();
	pPanel->SetRect(0, 0, rcVisual.w, rcVisual.h);
	pPanel->SetWidthMode(WHModeFill);
	pPanel->SetHeightMode(WHModeFill);
	//pPanel->SetAnchor(AlignLTRB);
	m_panels.add(pPanel);
	if(m_iSelectedIndex == -1)
		SetSelectedIndex(0);
}

void CPane::SetSelectedIndex(int_x iIndex)
{
	if(m_iSelectedIndex != iIndex)
	{
		if(m_iSelectedIndex >= 0)
			m_panels[m_iSelectedIndex]->SetVisible(false);
		m_iSelectedIndex = iIndex;
		if(m_iSelectedIndex >= 0)
		{
			SetText(m_panels[m_iSelectedIndex]->GetText());
			m_panels[m_iSelectedIndex]->SetVisible(true);
		}
		else
			SetText(L"");
	}
}

void CPane::PreOnNcMouseDown(pointix point, MouseButtonE eButton)
{
	CControl::PreOnNcMouseDown(point, eButton);
}

void CPane::OnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	CControl::OnNcPaint(pPaint, rcClip, pTheme);
	if(!m_bHideCaption)
	{
		edgeix border = CControl::GetBorder();
		rectix rcControl = GetRect();
		rectix rcCaption(border.left, border.top, rcControl.w - border.height(), m_iCaptionHeight);
		if(m_bSelected)
		{
			pPaint->FillRect(rcCaption, SysColorPaneCaptionBackSelected);
			pPaint->DrawString(m_text, textformat_t(m_font, AlignLeft | AlignCenterY), rcCaption, m_uiForeColor);
		}
		else
		{
			pPaint->FillRect(rcCaption, SysColorPaneCaptionBackNormal);
			pPaint->DrawString(m_text, textformat_t(m_font, AlignLeft | AlignCenterY), rcCaption, m_uiForeColor);
		}
	}
}

void CPane::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{

}

void CPane::QueryDebugMenu(int_x & iBase, IMenu * pMenu) const
{
	CControl::QueryDebugMenu(iBase, pMenu);
	MenuT * pSubMenu = new MenuT();
	pSubMenu->AddItem(iBase + 1, L"ShowCaption", !m_bHideCaption);
	pSubMenu->AddSplit();
	pSubMenu->AddItem(iBase + 2, L"Add caption height");
	pSubMenu->AddItem(iBase + 3, L"Cut caption height");
	pMenu->AddSplit();
	pMenu->AddItem(0, L"Pane", pSubMenu);
	SafeRelease(pSubMenu);
	iBase += 10;
}

void CPane::OnDebugMenu(int_x iBase, int_x iResult)
{
	iBase -= 10;
	switch(iResult - iBase)
	{
	case 1:
		SetCaptionVisible(m_bHideCaption);
		break;
	case 2:
		SetCaptionHeight(m_iCaptionHeight + 1);
		break;
	case 3:
		SetCaptionHeight(m_iCaptionHeight - 1);
		break;
	default:
		CControl::OnDebugMenu(iBase, iResult);
		break;
	}
}

VENUS_END