#include "stdafx.h"
#include "CForm.h"
#include "CPopMenu.h"
#include "CPopForm.h"

VENUS_BEG

CForm::CForm():
m_pOwner(nullptr), m_pHost(nullptr), m_eFormType(FormTypeDefault),
m_uiFormAttribute(0),
m_pPopupMenu(nullptr), m_pPopupForm(nullptr),
m_minSize(FONT_DEF_SIZE, FONT_DEF_SIZE), m_bdResize(BORDER_SIZE_DEFAULT),
m_iCaptionHeight(0), m_eCommand(FormCommandNone), m_eShowMode(ShowModeNormal)
{
	SetBackColor(SysColorForm);
	m_bRepaintActiveChanged = true;
	m_bRepaintFontChanged = true;
}

CForm::~CForm()
{
	SafeDelete(m_pPopupMenu);
	SafeRelease(m_pPopupForm);
	SafeRelease(m_pHost);
}

const cid_t & CForm::GetOid() const
{
	return OID_Form;
}

void CForm::GetMeta(IMetaHelper * pHelper)
{
	CControl::GetMeta(pHelper);
	pHelper->help_evt(L"ActiveChanged", ActiveChanged);
}

void CForm::SetParent(IControl * pParent)
{
	throw exp_illegal_operation();
}

IForm * CForm::GetForm() const
{
	return (IForm *)this;
}

IForm * CForm::ToIForm() const
{
	return (IForm *)this;
}

void CForm::SetPosition(pointix position)
{
	if(position != m_rect.position)
	{
		if(m_pHost)
			m_pHost->SetPosition(position);
		else
			CControl::SetPosition(position);
	}
}

void CForm::SetSize(sizeix size)
{
	if(m_minSize.w < m_maxSize.w)
		size.w = saturate(size.w, m_minSize.w, m_maxSize.w);

	if(m_minSize.h < m_maxSize.h)
		size.h = saturate(size.h, m_minSize.h, m_maxSize.h);

	if(size != m_rect.size)
	{
		if(m_pHost)
			m_pHost->SetSize(size);
		else
			CControl::SetSize(size);
	}
}

edgeix CForm::GetBorder() const
{
	if(m_eFormType != FormTypeDefault && m_eShowMode == ShowModeMaxisize)
		return edgeix();
	else
		return CControl::GetBorder();
}

rectix CForm::GetVisual() const
{
	rectix rcVisual = CControl::GetVisual();
	rcVisual.y += m_iCaptionHeight;
	rcVisual.h -= m_iCaptionHeight;
	return rcVisual;
}

HitTestE CForm::NcHitTest(pointix point) const
{
	sizeix size = GetSize();
	int_x iAlign = 0;

	if(!IsMaxSize() && !IsMinSize() && m_eFormType != FormTypeDefault)
	{
		if(point.x < m_bdResize.left)
			iAlign |= AlignLeft;
		else if(point.x >= size.w - m_bdResize.right)
			iAlign |= AlignRight;
		else {}

		if(point.y < m_bdResize.top)
			iAlign |= AlignTop;
		else if(point.y >= size.h - m_bdResize.bottom)
			iAlign |= AlignBottom;
		else {}
	}

	switch(iAlign)
	{
	case AlignLeft: return HitTestBorderL;
	case AlignLeftTop: return HitTestBorderLT;
	case AlignTop: return HitTestBorderT;
	case AlignTopRight: return HitTestBorderTR;
	case AlignRight: return HitTestBorderR;
	case AlignRightBottom: return HitTestBorderRB;
	case AlignBottom: return HitTestBorderB;
	case AlignBottomLeft: return HitTestBorderBL;
	case AlignNone:
		if(NcFindCommand(point) != FormCommandNone)
			return HitTestInner;
		else if(point.y < m_bdResize.top + m_iCaptionHeight)
			return HitTestCaption;
		else
			return CControl::NcHitTest(point);
	default:
		return HitTestNoWhere;
	}
}

void CForm::SetText(const char_16 * szText, int_x iLength)
{
	if(!szText)
		szText = L"";

	if(iLength < 0)
		iLength = textlen(szText);

	if(!textequalex(m_text.buffer(), m_text.length(), szText, iLength, true))
	{
		if(m_pHost)
			m_pHost->SetText(szText, iLength);
		else
			CControl::SetText(szText, iLength);
		NcRepaint();
	}
}

sizeix CForm::GetPreferedSize() const
{
	sizeix size = CControl::GetPreferedSize();
	size.h += m_iCaptionHeight;
	return size;
}

void CForm::UpdateIme()
{
	if(m_pHost)
		m_pHost->UpdateIme();
	else
		CControl::UpdateIme();
}

void CForm::Capture(bool bCapture)
{
	if(!m_pHost || m_pCaptureControl)
		return;

	if(bCapture != m_bCaptured)
	{
		m_pHost->Capture(bCapture);
		SetCaptured(bCapture);
	}
}

void CForm::SetCaptureControl(IControl * pControl)
{
	if(!m_pHost)
		return;

	CControl::SetCaptureControl(pControl);
	m_pHost->Capture(pControl != nullptr);
}

bool CForm::ClipboardCheckData(ClipboardDataTypeE eDataType) const
{
	if(m_pHost)
		return m_pHost->ClipboardCheckData(eDataType);
	else
		return CControl::ClipboardCheckData(eDataType);
}

textw CForm::ClipboardGetText16() const
{
	if(m_pHost)
		return m_pHost->ClipboardGetText16();
	else
		return CControl::ClipboardGetText16();
}

void CForm::ClipboardSetText16(const char_16 * szText, int_x iLength) const
{
	if(m_pHost)
		m_pHost->ClipboardSetText16(szText, iLength);
	else
		CControl::ClipboardSetText16(szText, iLength);
}

void CForm::Active(bool bActive)
{
	if(m_pHost)
		m_pHost->Active(bActive);
	else {}
}

void CForm::NcRepaint(const rectix & rect)
{
	if(m_bShown)
	{
		if(m_pHost)
		{
			rectix rcUpdate(0, 0, m_rect.w, m_rect.h);
			rcUpdate = rcUpdate.intersect(rect);
			if(rcUpdate.is_valid())
				m_pHost->Update(rcUpdate);
		}
		else
			CControl::NcRepaint(rect);
	}
}

pointix CForm::GetMousePosition() const
{
	if(m_pHost)
		return m_pHost->GetMousePosition();
	else
		return CControl::GetMousePosition();
}

bool CForm::IsKeyDown(KeyCodeE ekeyCode) const
{
	if(m_pHost)
		return m_pHost->IsKeyDown(ekeyCode);
	else
		return CControl::IsKeyDown(ekeyCode);
}

pointix CForm::ToScreen(const pointix & point) const
{
	if(m_pHost)
		return m_pHost->ClientToScreen(ToNoneCl(point));
	else
		return CControl::ToScreen(point);
}

void CForm::OnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	pPaint->FillRect(rcClip, 0xF0FFFFFF);
	if(m_eFormType != FormTypeDefault &&
		m_eShowMode != ShowModeMaxisize)
	{
		if(m_eBorderType != BorderTypeNone)
		{
			switch(m_eBorderType)
			{
			case BorderType1:
			case BorderType1Inactive:
				pPaint->DrawRect(0, 0, m_rect.w - 1, m_rect.h - 1, m_bActived ? Colors::DarkGreen : Colors::Gray);
				break;
			case BorderType2:
			case BorderType2Inactive:
				pPaint->DrawRect(0, 0, m_rect.w - 1, m_rect.h - 1, m_bActived ? Colors::LightGreen : Colors::LightGray);
				pPaint->DrawRect(1, 1, m_rect.w - 3, m_rect.h - 3, Colors::DarkGreen);
				break;
			case BorderType3:
			case BorderType3Inactive:
			case BorderType4:
			case BorderType4Inactive:
				pPaint->DrawRect(0, 0, m_rect.w - 1, m_rect.h - 1, m_bActived ? Colors::LightGreen : Colors::LightGray);
				pPaint->DrawRect(1, 1, m_rect.w - 3, m_rect.h - 3, Colors::DarkGreen);
				pPaint->DrawRect(2, 2, m_rect.w - 5, m_rect.h - 5, m_bActived ? Colors::LightGreen : Colors::LightGray);
				break;
			case BorderType5:
			case BorderType5Inactive:
				pPaint->DrawRect(0, 0, m_rect.w - 1, m_rect.h - 1, m_bActived ? Colors::LightGreen : Colors::LightGray);
				pPaint->DrawRect(1, 1, m_rect.w - 3, m_rect.h - 3, Colors::DarkGreen);
				pPaint->DrawRect(2, 2, m_rect.w - 5, m_rect.h - 5, Colors::DarkGreen);
				pPaint->DrawRect(3, 3, m_rect.w - 7, m_rect.h - 7, m_bActived ? Colors::LightGreen : Colors::LightGray);
				break;
			default:
				break;
			}
		}
	}

	if(m_iCaptionHeight > 0)
	{
		edgeix border = GetBorder();
		rectix rcCaption(border.left, border.top, m_rect.w - border.width(), m_iCaptionHeight);

		pPaint->FillRect(rcCaption, m_bActived ? SysColorCaptionActive : SysColorCaptionNoneActive);

		rectix rcText(border.left + m_font.size / 2, border.top, m_rect.w - border.width() - m_font.size / 2, m_iCaptionHeight);
		pPaint->DrawString(m_text, textformat_t(m_font, AlignLeft | AlignCenterY), 
			rcText, m_uiForeColor);

		int_x iSize = m_iCaptionHeight * 1 / 3;
		int_x iOffset = m_iCaptionHeight * 1 / 3;
		int_x iY = rcCaption.y + iOffset;
		// �رհ�ť
		int_x iX = rcCaption.right() - rcCaption.h;
		if(m_bCloseBox && iX > rcCaption.x)
		{
			if(m_eCommand == FormCommandClose)
			{
				pPaint->FillRect(iX, rcCaption.y, rcCaption.h, rcCaption.h, m_bMouseNcDownL ? Colors::IndianRed : SysColorBackHoving);
			}

			rectix rcClose(iX + iOffset, iY, iSize, iSize);
			pPaint->DrawLine(rcClose.x, rcClose.y, rcClose.right(), rcClose.bottom(), Colors::Black, 2.0f);
			pPaint->DrawLine(rcClose.x, rcClose.bottom(), rcClose.right(), rcClose.y, Colors::Black, 2.0f);
			iX -= rcCaption.h;
		}
		if(m_bMaxBox && iX > rcCaption.x)
		{
			bool bMax = IsMaxSize();
			if(m_eCommand == FormCommandMax)
				pPaint->FillRect(iX, rcCaption.y, rcCaption.h, rcCaption.h, m_bMouseNcDownL ? SysColorBackHoving : SysColorBackPressed);

			rectix rcMax(iX + iOffset, iY, iSize, iSize);
			if(bMax)
			{
				pPaint->DrawRect(rcMax, Colors::Black);
				int_x iSpace = round((float_32)iSize * 0.25f);
				rectix rcSmall(iX + iOffset + iSpace, iY + iSpace,
					iSize - iSpace * 2 + 1,
					iSize - iSpace * 2 + 1);
				pPaint->FillRect(rcSmall, Colors::Black);
			}
			else
			{
				pPaint->DrawRect(rcMax, Colors::Black);
				pPaint->DrawLine(rcMax.x + 1, rcMax.y + 1, rcMax.right(), rcMax.y + 1, Colors::Black, 1.0f);
				pPaint->DrawLine(rcMax.x + 1, rcMax.y + 2, rcMax.right(), rcMax.y + 2, Colors::Black, 1.0f);
			}
			iX -= rcCaption.h;
		}
		if(m_bMinBox && iX > rcCaption.x)
		{
			bool bMin = IsMinSize();
			if(m_eCommand == FormCommandMin)
				pPaint->FillRect(iX, rcCaption.y, rcCaption.h, rcCaption.h, m_bMouseNcDownL ? SysColorBackHoving : SysColorBackPressed);

			rectix rcMin(iX + iOffset, iY, iSize, iSize);
			if(bMin)
			{
				pPaint->DrawRect(rcMin, Colors::Black);
				int_x iSpace = round((float_32)iSize * 0.25f);
				rectix rcSmall(iX + iOffset + iSpace, iY + iSpace,
					iSize - iSpace * 2 + 1,
					iSize - iSpace * 2 + 1);
				pPaint->FillRect(rcSmall, Colors::Black);
			}
			else
			{
				pPaint->DrawLine(rcMin.x, rcMin.bottom() - 0, rcMin.right(), rcMin.bottom() - 0, Colors::Black, 1);
				pPaint->DrawLine(rcMin.x, rcMin.bottom() - 1, rcMin.right(), rcMin.bottom() - 1, Colors::Black, 1);
			}
			iX -= rcCaption.h;
		}
	}
}

void CForm::AftOnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
}

void CForm::OnNcMouseMove(pointix point)
{
	FormCommandE eCommand = NcFindCommand(point);
	if(eCommand != m_eCommand)
	{
		m_eCommand = eCommand;
		CControl::NcRepaint();
	}
	CControl::OnNcMouseMove(point);
}

void CForm::OnNcMouseOut(pointix point)
{
	if(m_eCommand != FormCommandNone)
	{
		m_eCommand = FormCommandNone;
		CControl::NcRepaint();
	}
	CControl::OnNcMouseOut(point);
}

void CForm::OnNcMouseDown(pointix point, MouseButtonE eButton)
{
	CControl::OnNcMouseDown(point, eButton);
	if(m_eCommand != FormCommandNone && eButton == MouseButtonL)
		CControl::NcRepaint();
}

void CForm::OnNcMouseUp(pointix point, MouseButtonE eButton)
{
	CControl::OnNcMouseUp(point, eButton);
	if(m_eCommand != FormCommandNone && eButton == MouseButtonL)
		CControl::NcRepaint();

}

void CForm::OnNcMouseClick(pointix point, MouseButtonE eButton)
{
	CControl::OnNcMouseClick(point, eButton);
	switch(m_eCommand)
	{
	case FormCommandMin:
		if(IsMinSize())
			Show(ShowModeRestore);
		else
			Show(ShowModeMinisize);
		break;
	case FormCommandMax:
		if(IsMaxSize())
			Show(ShowModeRestore);
		else
			Show(ShowModeMaxisize);
		break;
	case FormCommandClose:
		OnClose();
		break;
	default:
		break;
	}
}

void CForm::OnSizeChanged()
{
	return CControl::OnSizeChanged();
}

int_x CForm::PopupMenu(pointix point, IMenu * pMenu)
{
	if(!m_pHost)
		return CControl::PopupMenu(ToNoneCl(point), pMenu);

	if(!m_pPopupMenu)
	{
		m_pPopupMenu = new CPopMenu(nullptr);
		m_pPopupMenu->SetOwner(this);
	}

	point = ToScreen(point);
	m_pPopupMenu->UpdateMenu(point, pMenu);
	if(!m_pPopupMenu->IsShown() && pMenu)
	{
		SetPopuping(true);
		m_pPopupMenu->Show(ShowModeNormalNoActive, HostInitPosDefault);
		//int_x iResult = m_pMenu->RunPopup();
		int_x iResult = GetApp()->Run(m_pPopupMenu->GetFormId(), AppRunPopup);
		SetPopuping(false);
		m_pPopupMenu->UpdateMenu(pointix(), nullptr);
		return iResult;
	}
	else
	{
		if(!pMenu)
			GetApp()->EndRun(m_pPopupMenu->GetFormId(), DialogResultNone);
		return 0;
	}
}

int_x CForm::PopupControl(pointix point, IControl * pControl)
{
	if(!m_pHost)
		return CControl::PopupControl(point, pControl);

	if(!m_pPopupForm)
	{
		m_pPopupForm = new CPopForm();
		m_pPopupForm->SetOwner(this);
	}

	point = ToScreen(point);
	m_pPopupForm->SetControl(point, pControl);
	int_x iResult = 0;
	if(pControl)
	{
		//log2(L"beg popup");
		SetPopuping(true);
		m_pPopupForm->Show(ShowModeNormalNoActive, HostInitPosDefault);
		//int_x iResult = m_pMenu->RunPopup();
		iResult = GetApp()->Run(m_pPopupForm->GetFormId(), AppRunPopup);
		SetPopuping(false);
		m_pPopupForm->SetControl(pointix(), nullptr);
		//log2(L"end popup");
	}
	else
	{
		m_pPopupForm->Hide();
		//m_pForm->SetControl(pointix(), nullptr);
	}
	return iResult;
}

void CForm::OnActiveChanged()
{
	//if(!m_bActived && m_bMouseNcIn)
		//PreOnNcMouseOut(pointix(I32_MAX, I32_MAX));

	ActiveChanged(this, m_bActived);
	CControl::OnActiveChanged();
}

void CForm::QueryDebugMenu(int_x & iBase, IMenu * pMenu) const
{
	CControl::QueryDebugMenu(iBase, pMenu);
	IMenu * pFormMenu = new MenuT();
	pFormMenu->AddItem(iBase + 1, L"MinBox", m_bMinBox);
	pFormMenu->AddItem(iBase + 2, L"MaxBox", m_bMaxBox);
	pFormMenu->AddItem(iBase + 3, L"CloseBox", m_bCloseBox);
	pFormMenu->AddItem(iBase + 4, L"ShowCaption", m_iCaptionHeight > 0);
	pFormMenu->AddSplit();
	pFormMenu->AddItem(iBase + 5, L"Inc. CaptionHeight");
	pFormMenu->AddItem(iBase + 6, L"Dec. CaptionHeight");
	pMenu->AddSplit();
	pMenu->AddItem(0, L"Form", pFormMenu);
	SafeRelease(pFormMenu);
	iBase += 10;
}

void CForm::OnDebugMenu(int_x iBase, int_x iResult)
{
	iBase -= 10;
	switch(iResult - iBase)
	{
	case 1:
		SetMinBox(!m_bMinBox);
		break;
	case 2:
		SetMaxBox(!m_bMaxBox);
		break;
	case 3:
		SetCloseBox(!m_bCloseBox);
		break;
	case 4:
		if(m_iCaptionHeight > 0)
			SetCaptionHeight(0);
		else
			SetCaptionHeight(CAPTION_HEIGHT_DEFAULT);
		break;
	case 5:
		SetCaptionHeight(m_iCaptionHeight + 1);
		break;
	case 6:
		SetCaptionHeight(m_iCaptionHeight - 1);
		break;
	default:
		CControl::OnDebugMenu(iBase, iResult);
		break;
	}
}

int_x CForm::RunDialog()
{
	return GetApp()->Run(GetFormId(), AppRunDialog);
}

void CForm::EndRun(int_x iResult)
{
	GetApp()->EndRun(GetFormId(), iResult);
}

bool CForm::MouseActiving(pointix point) const
{
	return true;
}

bool CForm::IsActived() const
{
	return m_bActived;
}

// -----------------------------------------------------------------

void CForm::SetFormType(FormTypeE eHostType)
{
	m_eFormType = eHostType;
}

FormTypeE CForm::GetFormType() const
{
	return m_eFormType;
}

void CForm::CreateHost()
{
	if(!m_pHost)
	{
		m_pHost = (IHost *)GetApp()->CreateObject(OID_IHost);
		m_pHost->CreateHost(this, m_eFormType);
		m_pHost->SetBoxes(m_bMinBox, m_bMaxBox, m_bCloseBox);
	}
	else {}
}

void CForm::AttachHost(int_x iFormId)
{
	CreateHost();
	m_pHost->Attach(iFormId);
}

IHost * CForm::GetHost() const
{
	return m_pHost;
}

void CForm::SetResizeBorder(edgeix border)
{
	m_bdResize = border;
}

edgeix CForm::GetResizeBorder() const
{
	return m_bdResize;
}

int_x CForm::GetFormId() const
{
	if(m_pHost)
		return m_pHost->GetFormId();
	else
		return 0;
}

void CForm::SetOwner(IForm * pForm)
{
	m_pOwner = pForm;
}
IForm * CForm::GetOwner() const
{
	return m_pOwner;
}

void CForm::Show(ShowModeE eShow, HostInitPosE eInitPos)
{
	CreateHost();
	if(m_pHost)
		m_pHost->ShowHost(eShow, eInitPos);
}

void CForm::Hide()
{
	if(m_pHost)
		m_pHost->ShowHost(ShowModeHide, HostInitPosDefault);
}

ShowModeE CForm::GetShowMode() const
{
	return m_eShowMode;
}

bool CForm::IsMaxSize() const
{
	return m_eShowMode == ShowModeMaxisize ||
		m_eShowMode == ShowModeMaxisizeFake;
}

bool CForm::IsMinSize() const
{
	return m_eShowMode == ShowModeMinisize;
}

void CForm::OnRefresh(rectix rect)
{
	if(m_pHost)
		m_pHost->Refresh(rect);
}

void CForm::OnCreate()
{
}

void CForm::OnClose()
{
	GetApp()->EndRun(GetFormId(), DialogResultNone);
}

void CForm::OnDestroy()
{
}

void CForm::OnMaxSize()
{
}

void CForm::OnMinSize()
{
}

void CForm::OnRestore()
{
}

void CForm::NotifyPositionChanged(const pointix & position)
{
	if(position != pointix(m_rect.x, m_rect.y))
	{
		m_rect.x = position.x;
		m_rect.y = position.y;
		OnPositionChanged();
	}
}

void CForm::NotifySizeChanged(const sizeix & size, ShowModeE eShowMode)
{
	if(size != m_rect.size)
	{
		NcRepaint();
		m_rect.size = size;
		NcRepaint();

		m_eShowMode = eShowMode == ShowModeRestore ? ShowModeNormal : eShowMode;
		OnSizeChanged();
		switch(eShowMode)
		{
		case ShowModeMinisize:
			OnMinSize();
			break;
		case ShowModeMaxisize:
		case ShowModeMaxisizeFake:
			OnMaxSize();
			break;
		case ShowModeRestore:
			OnRestore();
			break;
		default:
			break;
		}
	}
}

void CForm::NotifyShownChanged(bool bShown)
{
	SetShown(bShown);
}

void CForm::NotifyFocusChanged(bool bFocus)
{
	if(m_bFocusAble)
	{
		m_bFocused = bFocus;
		OnFocusedChanged();
	}
}

void CForm::NotifyActiveChanged(bool bActive)
{
	if(bActive != m_bActived)
	{
		m_bActived = bActive;
		OnActiveChanged();
	}
}

void CForm::NotifyCaptureChanged(bool bCaptured)
{
	if(!bCaptured)
		SetCaptureControl(nullptr);
	//SetCaptured(bCaptured);
}

void CForm::NotifyTextChanged(const char_16 * szText, int_x iLength)
{
	m_text.set(szText, iLength);
	OnTextChanged();
}

void CForm::SetMinSize(const sizeix & size)
{
	sizeix newSize = size;
	if(m_maxSize.w > 0)
		newSize.w = minof(size.w, m_maxSize.w);
	if(m_maxSize.h > 0)
		newSize.h = minof(size.h, m_maxSize.h);

	m_minSize = newSize;
	int_x iWidth = maxof(m_minSize.w, GetWidth());
	int_x iHeight = maxof(m_minSize.h, GetHeight());
	//SetSize(sizeix(iWidth, iHeight));
}

void CForm::SetMaxSize(const sizeix & size)
{
	sizeix newSize = size;

	if(m_minSize.w > 0)
		newSize.w = maxof(size.w, m_maxSize.w);
	if(m_minSize.h > 0)
		newSize.h = maxof(size.h, m_maxSize.h);

	m_maxSize = newSize;
	int_x iWidth = minof(m_maxSize.w, GetWidth());
	int_x iHeight = minof(m_maxSize.h, GetHeight());
	//SetSize(sizeix(iWidth, iHeight));
}

const sizeix & CForm::GetMinSize() const
{
	return m_minSize;
}

const sizeix & CForm::GetMaxSize() const
{
	return m_maxSize;
}

void CForm::SetCaptionHeight(int_x iCaptionHeight)
{
	if(iCaptionHeight < 0)
		iCaptionHeight = CAPTION_HEIGHT_DEFAULT;

	if(iCaptionHeight != m_iCaptionHeight)
	{
		m_iCaptionHeight = iCaptionHeight;
		OnVisualSizeChanged();
		NcRepaint();
	}
}

int_x CForm::GetCaptionHeight() const
{
	return m_iCaptionHeight;
}

void CForm::SetMinBox(bool bMinSizeBox)
{
	if(bMinSizeBox != m_bMinBox)
	{
		m_bMinBox = bMinSizeBox;
		if(m_pHost)
			m_pHost->SetBoxes(m_bMinBox, m_bMaxBox, m_bCloseBox);
		CControl::NcRepaint();
	}
}

bool CForm::HasMinBox() const
{
	return m_bMinBox;
}

void CForm::SetMaxBox(bool bMaxSizeBox)
{
	if(m_bMaxBox != bMaxSizeBox)
	{
		m_bMaxBox = bMaxSizeBox;
		if(m_pHost)
			m_pHost->SetBoxes(m_bMinBox, m_bMaxBox, m_bCloseBox);
		CControl::NcRepaint();
	}
}

bool CForm::HasMaxBox() const
{
	return m_bMaxBox;
}

void CForm::SetCloseBox(bool bCloseBox)
{
	if(m_bCloseBox != bCloseBox)
	{
		m_bCloseBox = bCloseBox;
		CControl::NcRepaint();
	}
}

bool CForm::HasCloseBox() const
{
	return m_bCloseBox;
}

void CForm::SetBoxes(bool bMinSizeBox, bool bMaxSizeBox, bool bCloseBox)
{
	if(bMinSizeBox != m_bMinBox || m_bMaxBox != bMaxSizeBox || m_bCloseBox != bCloseBox)
	{
		m_bMinBox = bMinSizeBox;
		m_bMaxBox = bMaxSizeBox;
		m_bCloseBox = bCloseBox;
		if(m_pHost)
			m_pHost->SetBoxes(m_bMinBox, m_bMaxBox, m_bCloseBox);
		CControl::NcRepaint();
	}
}

FormCommandE CForm::NcFindCommand(pointix point) const
{
	if(m_iCaptionHeight <= 0)
		return FormCommandNone;

	FormCommandE eCommand = FormCommandNone;

	edgeix border = GetBorder();
	rectix rcCaption(border.left, border.top, m_rect.w - border.width(), m_iCaptionHeight);
	if(point.y >= rcCaption.bottom())
		return FormCommandNone;

	int_x iX = rcCaption.right() - rcCaption.h;
	if(m_bCloseBox && iX >= border.left)
	{
		if(point.x >= iX)
			eCommand = FormCommandClose;
		iX -= rcCaption.h;
	}
	if(m_bMaxBox && iX >= border.left && eCommand == FormCommandNone)
	{
		if(point.x >= iX)
			eCommand = FormCommandMax;
		iX -= rcCaption.h;
	}
	if(m_bMinBox && iX >= border.left && eCommand == FormCommandNone)
	{
		if(point.x >= iX)
			eCommand = FormCommandMin;
		iX -= rcCaption.h;
	}
	return eCommand;
}

VENUS_END
