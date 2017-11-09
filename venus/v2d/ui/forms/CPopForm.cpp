#include "stdafx.h"
#include "CPopForm.h"

VENUS_BEG

class CTextLine;
static const int_x ITEM_SPACE_Y = 3;
static const int_x ITEM_SPACE_X = 7;
static const int_x SPLIT_HEIGHT = 1;

static const cid_t OID_PopForm= {L"PopForm"};

CPopForm::CPopForm()
{
	m_eBorderType = BorderTypeNone;
	m_eFormType = FormTypePopup;
	m_bBuffered = true;
	m_bdResize.set(0, 0, 0, 0);
}

CPopForm::~CPopForm()
{
}

const cid_t & CPopForm::GetOid() const
{
	return OID_PopForm;
}

bool CPopForm::MouseActiving(pointix point) const
{
		return false;
}

void CPopForm::OnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	CControl::OnNcPaint(pPaint, rcClip, pTheme);
}

void CPopForm::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
}

void CPopForm::SetControl(pointix point, IControl * pControl)
{
	if(!pControl)
		RmvAllControl();
	else
	{
		sizeix size = pControl->GetSize();
		SetRect(point.x, point.y, size.w, size.h);
		pControl->SetPosition(0, 0);
		AddControl(pControl);
	}
}

VENUS_END
