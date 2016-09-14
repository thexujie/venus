#include "stdafx.h"
#include "CMessageBox.h"

VENUS_BEG

CMessageBox::CMessageBox()
{
	SetRect(0, 0, 350, 155);
	m_name.SetRect(10, 5, 330, 18);
	m_detail.SetRect(10, 40, 330, 70);
	m_btnOk.SetRect(0, 0, 78, 23);
	m_btnCancel.SetRect(0, 0, 78, 23);

	SetFormType(FormTypeDefault);
	SetBackColor(Colors::White);
	SetMinSize(sizeix(300, 80));
	SetMaxSize(sizeix(600, 300));
	SetImeMode(ImeModeDisable);

	m_btnOk.SetDialogResult(DialogResultOk);
	m_btnOk.SetText(L"确定(&O)");
	m_btnOk.SetAnchor(AlignRightBottom);
	m_btnOk.SetHotKey(KeyCodeO);

	m_btnCancel.SetDialogResult(DialogResultCancel);
	m_btnCancel.SetText(L"取消(&C)");
	m_btnCancel.SetAnchor(AlignRightBottom);
	m_btnCancel.SetHotKey(KeyCodeC);

	m_name.SetFontSize(18);
	//m_name.SetFontFlag(FontFlagBold, true);
	m_name.SetForeColor(SysColor);

	m_detail.SetAutoHideScrollBarX(true);
	m_detail.SetAutoHideScrollBarY(true);
	m_detail.SetAnchor(AlignLTRB);
	//m_detail.SetReadOnly(true);
	m_detail.SetAcceptEnter(false);
	m_detail.SetAcceptTab(false);
	m_detail.SetTextWrap(TextWrapWord);
	m_detail.SetBorderType(BorderTypeNone);
	//m_detail.SetEnable(false);
	//m_detail.SetSelectAble(false);
	//m_detail.SetFocusAble(false);

	AddControl(&m_name);
	AddControl(&m_detail);
}

void CMessageBox::SetShowText(const char_16 * szName, const char_16 * szText, MessageBoxButtonE eButton)
{
	m_name.SetText(szName);
	m_detail.SetText(szText);
	if(eButton == MessageBoxButtonOk)
	{
		m_btnOk.SetPosition(262, 123);
		AddControl(&m_btnOk);
		m_btnOk.Select(true);
	}
	else if(eButton == MessageBoxButtonCancel)
	{
		m_btnCancel.SetPosition(262, 123);
		AddControl(&m_btnCancel);
		m_btnCancel.Select(true);
	}
	else if(eButton == MessageBoxButtonOkCancel)
	{
		m_btnOk.SetPosition(170, 123);
		m_btnCancel.SetPosition(262, 123);
		AddControl(&m_btnOk);
		AddControl(&m_btnCancel);
		m_btnOk.Select(true);
	}
	else
		throw exp_illegal_argument();
}

void CMessageBox::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	sizeix szClient = GetClient().size;
	pPaint->FillRect(rectix(pointix(), szClient), m_uiBackColor);

	pPaint->FillRect(0, 0, szClient.w, 28, 0xFFF0F0FF);
	pPaint->DrawLine(0, 28, szClient.w, 28, 0xFFDFDFDF);

	int_x iLine = szClient.h - 42;
	pPaint->FillRect(0, iLine, szClient.w, 42, 0xFFF0F0F0);
	pPaint->DrawLine(0, iLine, szClient.w, iLine, 0xFFDFDFDF);
}

DialogResultE CMessageBox::ShowMessage(IForm * pOwner, const char_16 * szMessage,
						  const char_16 * szName, const char_16 * szTitle, 
						  MessageBoxButtonE eButton)
{
	CMessageBox mb;
	mb.SetOwner(pOwner);
	mb.SetShowText(szName, szMessage, eButton);
	mb.SetText(szTitle);

	mb.Show(ShowModeNormal, HostInitPosCenterParent);
	return (DialogResultE)GetApp()->Run(mb.GetFormId(), AppRunDialog);
};

VENUS_END
