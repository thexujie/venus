#include "stdafx.h"
#include "CDockFloatForm.h"

CDockFloatForm::CDockFloatForm(IControl * pControl)
{
	SetCaptionHeight(24);
	SetCloseBox(true);
	SetMaxBox(true);
	SetStatic(true);
	SetFormType(FormTypePopup);
	SetBorderType(BorderType3Inactive);
	AddControl(&m_frame);
	m_frame.SetAnchor(AlignLTRB);
	SetRect(rectix(pointix(), pControl->GetSize()));

	m_frame.DockNodeSetControl(pControl);
	m_frame.AddControl(pControl);
	SetText(pControl->GetText());

	pControl->Select(true);
}

CDockFloatForm::~CDockFloatForm()
{

}

int_x CDockFloatForm::GetCaptionHeight() const
{
	return 0;
}

void CDockFloatForm::OnCreate()
{
	//IWindow * pWindow = (IWindow *)GetHost()->GetInterface(OID_IWindow);
	//if(pWindow)
	//{
	//	((CWindow *)pWindow)->SetShowMinBox(true);
	//	((CWindow *)pWindow)->SetShowMaxBox(true);
	//}
}

void CDockFloatForm::OnPositionChanged()
{
	CForm::OnPositionChanged();
}