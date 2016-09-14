#include "stdafx.h"
#include "CUIDevelopWindow.h"

CUIDevelopWindow::CUIDevelopWindow()
{
	SetSize(sizeix(800, 600));
	SetFormType(FormTypePopup);
	SetBorderType(BorderType2Inactive);
	SetCaptionHeight(-1);
	SetBoxes(true, true, true);
	SetText(L"UIDevelopWindow");

	m_txl.SetRect(10, 10, 400, 24);
	m_txl.SetAnchor(AlignLR);

	CTextBox * ptxb = new CTextBox();
	ptxb->SetDynamic(true);
	ptxb->SetRect(10, 40, 600, 400);
	ptxb->SetAnchor(AlignLTRB);
	ptxb->SetFontSize(40);

	//AddControl(&m_txl);
	AddControl(ptxb);
	SetBackColor(0xff333333);
}

CUIDevelopWindow::~CUIDevelopWindow()
{

}

void CUIDevelopWindow::OnCreate()
{
	CForm::OnCreate();
}

void CUIDevelopWindow::OnLoaded()
{


}

void CUIDevelopWindow::OnShow()
{
	CForm::OnShow();
}

void CUIDevelopWindow::OnVisualSizeChanged()
{
	CForm::OnVisualSizeChanged();
}

void CUIDevelopWindow::GetMeta(IMetaHelper * pHelper)
{
}

void CUIDevelopWindow::OnMouseDownL(pointix point)
{
	CForm::OnMouseDownL(point);
}

void CUIDevelopWindow::OnKeyDown(KeyCodeE key)
{
	CForm::OnKeyDown(key);
}

void CUIDevelopWindow::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
}
