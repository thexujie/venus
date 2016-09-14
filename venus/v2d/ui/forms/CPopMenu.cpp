#include "stdafx.h"
#include "CPopMenu.h"

VENUS_BEG

class CTextLine;
static const int_x ITEM_SPACE_Y = 3;
static const int_x ITEM_SPACE_X = 7;
static const int_x SPLIT_HEIGHT = 1;

static const oid_t OID_MENU = {L"CPopMenu"};

CPopMenu::CPopMenu(CPopMenu * pParent) :
m_pMenu(nullptr), m_iHoverIndex(-1), m_pMenuParent(pParent), m_pMenuChild(nullptr)
{
	m_eBorderType = BorderType3Inactive;
	m_eFormType = FormTypeMenu;
	//m_font.FontName = L"华文行楷";
	//m_font.FontSize = 24;
	//m_uiBackColor = SysColorBarBackNormal;
	m_bBuffered = true;
	m_bdResize.set(0, 0, 0, 0);

	//CButton * pButton = new CButton(0, 0, 100, 30);
	//pButton->SetText(L"测试按钮");
	//AddControl(pButton);

	//CTextLine * pTextLine = new CTextLine();
	//pTextLine->SetDynamic(true);
	//pTextLine->SetRect(0, 32, 100, 24);
	//AddChild(pTextLine);
	//SafeRelease(pTextLine);
}

CPopMenu::~CPopMenu()
{
	SafeRelease(m_pMenuChild);
	SafeRelease(m_pMenu);
}

const oid_t & CPopMenu::GetOid() const
{
	return OID_MENU;
}

bool CPopMenu::MouseActiving(pointix point) const
{
	IControl * pControl = FindChild(point);
	if(pControl && pControl->CanFocus())
		return true;
	else
		return false;
}

void CPopMenu::PreOnKeyDown(KeyCodeE eKeyCode)
{
	if(m_pMenuChild && m_pMenuChild->IsShown())
		m_pMenuChild->PreOnKeyDown(eKeyCode);
	else
		CForm::PreOnKeyDown(eKeyCode);
}

void CPopMenu::OnKeyDown(KeyCodeE eKeyCode)
{
	CControl::OnKeyDown(eKeyCode);

	int_x iIndex = m_iHoverIndex;
	switch(eKeyCode)
	{
	case KeyCodeUp:
		HovePrev();
		break;
	case KeyCodeDown:
		HoveNext();
		break;
	case KeyCodeLeft:
		if(m_pMenuParent)
			Show(ShowModeHide);
		break;
	case KeyCodeRight:
		TryShowSubMenu(m_iHoverIndex);
		if(m_pMenuChild)
			m_pMenuChild->SetHoverIndex(0);
		break;
	case KeyCodeEscape:
		if(m_pMenuParent)
			Show(ShowModeHide);
		else
			GetApp()->EndRun(GetFormId(), DialogResultNone);
		break;
	case KeyCodeEnter:
		if(m_iHoverIndex >= 0)
		{
			IMenuItem * pItem = m_pMenu->GetItem(m_iHoverIndex);
			if(!pItem->IsSpilit() && pItem->IsEnable())
			{
				if(pItem->GetSubMenu())
					TryShowSubMenu(m_iHoverIndex);
				else
					EndRun(pItem->GetId());
			}
		}
		break;
	default:
		break;
	}
}

void CPopMenu::OnMouseMove(pointix point)
{
	CControl::OnMouseMove(point);

	if(m_pMenu)
	{
		int_x iFontH = m_font.size;
		int_x iMenuH = ITEM_SPACE_Y + iFontH + ITEM_SPACE_Y;
		int_x iSplitH = ITEM_SPACE_Y + SPLIT_HEIGHT + ITEM_SPACE_Y;
		int_x iCount = m_pMenu->GetItemCount();
		if(point.y < 0)
			SetHoverIndex(-1);
		else
		{
			int_x iIndex = -1;
			int_x iTop = 0;
			IMenuItem * pItem = nullptr;
			for(int_x cnt = 0;cnt < iCount; ++cnt)
			{
				pItem = m_pMenu->GetItem(cnt);

				if(pItem->IsSpilit())
					iTop += iSplitH;
				else
					iTop += iMenuH;

				if(iTop >= point.y)
				{
					iIndex = cnt;
					break;
				}
			}

			if(iIndex != m_iHoverIndex)
			{
				SetHoverIndex(iIndex);
				TryShowSubMenu(iIndex);
			}
		}
		
	}
	else
		SetHoverIndex(-1);
}

void CPopMenu::OnMouseOut(pointix point)
{
	if(!m_pMenuChild || !m_pMenuChild->IsShown())
		SetHoverIndex(-1);
	CControl::OnMouseOut(point);
}

void CPopMenu::OnMouseUpL(pointix point)
{
	if(m_iHoverIndex >= 0)
	{
		IMenuItem * pItem = m_pMenu->GetItem(m_iHoverIndex);
		if(pItem && !pItem->IsSpilit() && !pItem->GetSubMenu() && pItem->IsEnable())
			EndRun(pItem->GetId());
	}
	CControl::OnMouseUpL(point);
}

void CPopMenu::SetShowMenu(IMenu * pMenu)
{
	if(pMenu == m_pMenu)
		return;

	SetHoverIndex(-1);
	if(m_pMenu)
		SafeRelease(m_pMenu);

	if(m_pMenuChild && m_pMenuChild->IsShown())
	{
		m_pMenuChild->SetShowMenu(nullptr);
		m_pMenuChild->Show(ShowModeHide);
	}

	m_pMenu = pMenu;
	if(m_pMenu)
	{
		SafeAddRef(m_pMenu);
		I2DDevice * pService = Get2DDevice();
		int_x iWidth = 0, iHeight = 0;
		int_x iItemH = ITEM_SPACE_Y + m_font.size + ITEM_SPACE_Y;
		int_x iSplitH = ITEM_SPACE_Y + SPLIT_HEIGHT + ITEM_SPACE_Y;
		bool bHasSub = false;
		for(int_x cnt = 0, size = m_pMenu->GetItemCount(); cnt < size; ++cnt)
		{
			IMenuItem * pItem = m_pMenu->GetItem(cnt);
			if(pItem->GetSubMenu())
				bHasSub = true;
			if(pItem->IsSpilit())
				iHeight += iSplitH;
			else
			{
				int_x iItemWidth = pService->GetTextSize(pItem->GetText(), -1, m_font).w;
				if(iItemWidth > iWidth)
					iWidth = iItemWidth;

				iHeight += iItemH;
			}
		}

		iWidth = ITEM_SPACE_X + m_font.size + ITEM_SPACE_X +
			ITEM_SPACE_X + iWidth + ITEM_SPACE_X + m_font.size + ITEM_SPACE_X;

		sizeix size(iWidth, iHeight);
		size += GetBorder().size();
		SetSize(size);
		Repaint();
	}
	else
	{
	}
}

int_x CPopMenu::UpdateMenu(pointix point, IMenu * pMenu)
{
	SetShowMenu(pMenu);
	if(!pMenu)
		return 0;

	IMonitor * pMonitor = IMonitorManager::GetInstance()->GetMonitor(point.x, point.y);
	if(!pMonitor)
		return 0;

	rectix rcScreen = pMonitor->GetWorkRect();
	if(point.x + m_rect.w > rcScreen.right())
		point.x -= m_rect.w - 1;
	else
		point.x -= 1;
	if(point.y + m_rect.h > rcScreen.bottom())
		point.y -= m_rect.h - 1;
	else
		point.y -= 1;

	SetPosition(point);
	return 0;
}

void CPopMenu::UpdateMenu(pointix point, int_x iParentW, int_x iItemH, IMenu * pMenu)
{
	IMonitor * pMonitor = IMonitorManager::GetInstance()->GetMonitorFromWindowId(0);
	if(!pMonitor)
		return;

	SetShowMenu(pMenu);
	sizeix siScreen = pMonitor->GetSize();
	edgeix egBorder = GetBorder();

	if(point.x + m_rect.w > siScreen.w)
		point.x -= m_rect.w + iParentW;

	if(point.y + m_rect.h > siScreen.h)
	{
		point.y -= m_rect.h;
		point.y += iItemH;
		point.y += egBorder.bottom;
	}
	else
		point.y -= egBorder.top;

	SetPosition(point);
	Show(ShowModeNormalNoActive, HostInitPosDefault);
}

void CPopMenu::EndRun(int_x iResult)
{
	if(m_pMenuParent)
		m_pMenuParent->EndRun(iResult);
	else
		GetApp()->EndRun(GetFormId(), iResult);
}

void CPopMenu::OnShow()
{
	CControl::OnShow();
}

void CPopMenu::OnHide()
{
	SafeRelease(m_pMenu);
	if(m_pMenuChild && m_pMenuChild->IsShown())
	{
		m_pMenuChild->SetShowMenu(nullptr);
		m_pMenuChild->Hide();
	}
	CControl::OnHide();
}

void CPopMenu::OnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	CControl::OnNcPaint(pPaint, rcClip, pTheme);
	//pPaint->FillRect(0, 0, m_rect.w, m_rect.h, m_uiBackColor);
	//pPaint->DrawRect(0, 0, m_rect.w - 1, m_rect.h - 1, SysColorBackNormal);
}

void CPopMenu::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	if(!m_pMenu)
		return;

	rectix rcClient = GetClient();
	int_x iFontH = m_font.size;
	int_x iLeft = ITEM_SPACE_X + iFontH + ITEM_SPACE_X;

	int_x iItemH = ITEM_SPACE_Y + iFontH + ITEM_SPACE_Y;
	int_x iSplitH = ITEM_SPACE_Y + SPLIT_HEIGHT + ITEM_SPACE_Y;
	int_x iY = 0;
	IMenuItem * pItem = nullptr;
	IImage * pImage = nullptr;
	int_x iCount = m_pMenu->GetItemCount();
	for(int_x cnt = 0; cnt < iCount; ++cnt)
	{
		pItem = m_pMenu->GetItem(cnt);
		if(pItem->IsSpilit())
		{
			pPaint->DrawLine(iLeft + ITEM_SPACE_X, iY + ITEM_SPACE_Y,
							 rcClient.w - ITEM_SPACE_X, iY + ITEM_SPACE_Y, SysColorFrameDisable, (float_32)SPLIT_HEIGHT);
			iY += iSplitH;
		}
		else
		{
			uint_32 uiForeColor = pItem->IsEnable() ? m_uiForeColor : SysColorForeDisable;

			if(cnt == m_iHoverIndex)
				pPaint->FillRect(0, iY, rcClient.w, iItemH, pItem->IsEnable() ? SysColorBackSelected : SysColorBackDisable);

			PaintMenuString(pPaint, pItem->GetText(), -1, m_font, iLeft + ITEM_SPACE_X, iY + ITEM_SPACE_Y, uiForeColor);

			if(pImage = pItem->GetImage())
			{
				sizeix siImage = pImage->GetSize();
				int_x x = ITEM_SPACE_X + (iFontH - siImage.w) / 2;
				int_x y = (iItemH - siImage.h) / 2;
				pPaint->DrawImage(pImage, x, iY + y);
			}

			// 子菜单标记
			if(pItem->GetSubMenu())
			{
				pPaint->SetShapeRenderLevel(ShapeRenderLevelAnti);
				int_x x = rcClient.w - ITEM_SPACE_X - iFontH;
				int_x y = iY + iItemH / 2;

				if(pItem->GetSubMenu()->GetItemCount() == 0)
					pPaint->FillTriangle(x + 5, y - 5, x + 5 + 6, y, x + 5, y + 5, SysColorForeDisable);
				else
					pPaint->FillTriangle(x + 5, y - 5, x + 5 + 6, y, x + 5, y + 5, uiForeColor);
				//PaintArrowR(pPaint, rectix(x + 7, y - 7, 14, 14), uiForeColor);
				pPaint->SetShapeRenderLevel(ShapeRenderLevelDefault);
			}

			if(pItem->IsSelected())
				PaintYes(pPaint, rectix(ITEM_SPACE_X + 2, iY + ITEM_SPACE_Y + 2, iFontH - 4, iFontH - 4), uiForeColor);
			else if(pItem->IsChecked())
			{
				pPaint->SetShapeRenderLevel(ShapeRenderLevelAnti);
				pPaint->FillCircle(ITEM_SPACE_X + iFontH / 2, iY + iItemH / 2, iItemH / 4, uiForeColor);
				pPaint->SetShapeRenderLevel(ShapeRenderLevelDefault);
			}
			iY += iItemH;
		}
	}

	pPaint->DrawLine(iLeft, 0, iLeft, rcClient.h, SysColorFrameDisable, 1.0f);
	//pPaint->FillRect(rcClient, 0xFF50F050);
}

void CPopMenu::SetHoverIndex(int_x iIndex)
{
	if(iIndex != m_iHoverIndex)
	{
		m_iHoverIndex = iIndex;
		Repaint();
	}
}

void CPopMenu::HovePrev()
{
	if(!m_pMenu)
		return ;

	int_x iIndex = 0;
	if(m_iHoverIndex <= 0)
		iIndex = m_pMenu->GetItemCount();
	else
		iIndex = m_iHoverIndex;

	for(int_x cnt = iIndex - 1; cnt >= 0; --cnt)
	{
		if(!m_pMenu->GetItem(cnt)->IsSpilit())
		{
			SetHoverIndex(cnt);
			break;
		}
	}
}
void CPopMenu::HoveNext()
{
	if(!m_pMenu)
		return ;

	int_x iIndex = 0;
	int_x iCount = m_pMenu->GetItemCount();

	if(m_iHoverIndex < 0)
		iIndex = -1;
	else
	{
		iIndex = m_iHoverIndex;
		if(iIndex == iCount - 1)
			iIndex = -1;
	}

	for(int_x cnt = iIndex + 1; cnt < iCount; ++cnt)
	{
		if(!m_pMenu->GetItem(cnt)->IsSpilit())
		{
			SetHoverIndex(cnt);
			break;
		}
	}
}

void CPopMenu::TryShowSubMenu(int_x iIndex)
{
	if(!m_pMenu)
		return ;

	int_x iCount = m_pMenu->GetItemCount();
	if(0 <= iIndex && iIndex < iCount)
	{
		IMenuItem * pMenuItem = m_pMenu->GetItem(iIndex);
		IMenu * pMenu = pMenuItem ? pMenuItem->GetSubMenu() : nullptr;
		if(pMenu && pMenu->GetItemCount() > 0 && pMenuItem->IsEnable())
		{
			if(!m_pMenuChild)
			{
				m_pMenuChild = new CPopMenu(this);
				m_pMenuChild->SetOwner(this);
			}

			int_x iY = 0;
			int_x iItemH = ITEM_SPACE_Y + m_font.size + ITEM_SPACE_Y;
			int_x iSplitH = ITEM_SPACE_Y + SPLIT_HEIGHT + ITEM_SPACE_Y;
			for(int_x cnt = 0; cnt < iIndex; ++cnt)
			{
				if(m_pMenu->GetItem(cnt)->IsSpilit())
					iY += iSplitH;
				else
					iY += iItemH;
			}

			pointix point(GetClient().w + GetBorder().right, iY);
			point = ClientToScreen(point);
			m_pMenuChild->UpdateMenu(point, m_rect.w, iItemH, pMenu);
		}
		else if(m_pMenuChild && m_pMenuChild->IsShown())
			m_pMenuChild->Hide();
		else {}
	}
	else if(m_pMenuChild && m_pMenuChild->IsShown())
		m_pMenuChild->Hide();
	else {}
}

VENUS_END
