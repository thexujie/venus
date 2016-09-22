#include "stdafx.h"
#include "CUIDevelopWindow.h"

vector<IControl *> ctrls;
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

	SetLayoutMode(LayoutModeVertical);

	for(int_x cnt = 0; cnt < 10; ++cnt)
	{
		CButton * pCtrl = new CButton();
		pCtrl->SetDynamic(true);
		pCtrl->SetFontSize(28);
		pCtrl->SetHeightMode(WHModeAuto);
		pCtrl->SetWidthMode(WHModeAuto);
		pCtrl->SetText(textw().format(L"textbox %02d", cnt));
		AddControl(pCtrl);
		ctrls.add(pCtrl);
	}

	//AddControl(&m_txl);
	//AddControl(pCtrl);
	//SetBackColor(0xff333333);
	SetWidthMode(WHModeAuto);
	SetHeightMode(WHModeAuto);
	SetSize(GetPreferedSize());
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
	static int_x index = 0;
	ctrls[index++]->SetVisible(false);
	CForm::OnMouseDownL(point);
}

void CUIDevelopWindow::OnKeyDown(KeyCodeE key)
{
	CForm::OnKeyDown(key);
}

void CUIDevelopWindow::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
}
