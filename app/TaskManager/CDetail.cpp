#include "stdafx.h"
#include "CDetail.h"

CDetailWindow::CDetailWindow(int_x iWidth, int_x iHeight)
{
	m_eFormType = FormTypePopup;
	SetCaptionHeight(26);
	SetMinBox(true);
	SetMaxBox(true);
	SetCloseBox(true);
	SetBorderType(BorderType2Inactive);
	SetBackColor(Colors::White);

	SetName(L"CDetailWindow");
	m_txlName.SetName(L"m_txlName");
	m_txlName.SetDefaultText(L"无法读取进程信息，可能需要管理员权限。");
	m_txlPath.SetName(L"m_txlPath");
	m_txlCmd.SetName(L"m_txlCmd");
	m_txlParent.SetName(L"m_txlParent");

	SetRect(0, 0, iWidth, iHeight);
	m_grpBase.SetRect(10, 10, 580, 134);
	m_staName.SetRect(10, 5, 50, 20);
	m_txlName.SetRect(70, 5, 500, 24);
	m_staPath.SetRect(10, 32, 50, 20);
	m_txlPath.SetRect(70, 32, 500, 24);
	m_staCmd.SetRect(10, 59, 50, 20);
	m_txlCmd.SetRect(70, 59, 500, 24);
	m_staParent.SetRect(10, 86, 50, 20);
	m_txlParent.SetRect(70, 86, 500, 24);
	m_txbDetail.SetRect(10, 145, 580, 200);

	m_txlName.SetTextReadOnly(true);
	m_staName.SetText(L"进程名称");
	m_staPath.SetText(L"程序路径");
	m_staCmd.SetText(L"命令行");
	m_staParent.SetText(L"父进程");
	m_grpBase.SetText(L"基本信息");

	m_grpBase.AddControl(&m_staName);
	m_grpBase.AddControl(&m_staPath);
	m_grpBase.AddControl(&m_staCmd);
	m_grpBase.AddControl(&m_staParent);

	m_grpBase.AddControl(&m_txlName);
	m_grpBase.AddControl(&m_txlPath);
	m_grpBase.AddControl(&m_txlCmd);
	m_grpBase.AddControl(&m_txlParent);

	AddControl(&m_grpBase);
	AddControl(&m_txbDetail);

	m_txlName.SelectedChanged += bind(this, &CDetailWindow::OnTextLineSelectedChanged);
	m_txlPath.SelectedChanged += bind(this, &CDetailWindow::OnTextLineSelectedChanged);
	m_txlCmd.SelectedChanged += bind(this, &CDetailWindow::OnTextLineSelectedChanged);
	m_txlParent.SelectedChanged += bind(this, &CDetailWindow::OnTextLineSelectedChanged);
}

void CDetailWindow::SetProcessId(uint_32 uiId)
{
	const int_x BUFF_S = 512;
	char_16 szBuffer[BUFF_S];
	CProcess process;
	process.Open(uiId, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ);
	process.GetFilePath(szBuffer, BUFF_S);
	m_txlPath.SetText(szBuffer);
	char_16 szFile[64];
	textsplitpath_name(szBuffer, -1, szFile, 64);
	m_txlName.SetText(szFile);
	SetText(szFile);

	process.GetCmdLine(szBuffer, BUFF_S);
	m_txlCmd.SetText(szBuffer);
}

void CDetailWindow::PreOnKeyDown(KeyCodeE eKeyCode)
{
	if(eKeyCode == KeyCodeEscape)
		Hide();
	CForm::PreOnKeyDown(eKeyCode);
}

int_x CDetailWindow::OnTextLineSelectedChanged(IControl * pControl, bool bSelected)
{
	CTextLine * pTl = (CTextLine *)pControl;
	if(bSelected)
	{
		pTl->SelectAll();
		//pTl->SetCaretIndex(pTl->GetCText().GetLength());
		CMessageBox::ShowMessage(this, L"szMessage", L"szName", L"szTitle");
	}
	return 0;
}

void CDetailWindow::OnClose()
{
	Hide();
}

