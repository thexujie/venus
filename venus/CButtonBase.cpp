#include "stdafx.h"
#include "CButtonBase.h"

VENUS_BEG

static const int_x DEF_ICON_TEXT_SPACE = 4;

CButtonBase::CButtonBase() :
m_iState(ButtonStateNone)
{
}

CButtonBase::CButtonBase(int_x iX, int_x iY, int_x iWidth, int_x iHeight) :
CControl(iX, iY, iWidth, iHeight),
m_iState(ButtonStateNone)
{
}

CButtonBase::~CButtonBase()
{

}

void CButtonBase::ModifyState(ButtonStateE eState, bool bKeep)
{
	if(bKeep)
		m_iState |= eState;
	else
		m_iState &= (~eState);
	Repaint();
}

bool CButtonBase::CheckState(ButtonStateE eState) const
{
	return (m_iState & eState) == eState;
}

void CButtonBase::OnMouseIn(pointix point)
{
	m_iState |= ButtonStateHoving;
	Repaint();
	CControl::OnMouseIn(point);
}

void CButtonBase::OnMouseOut(pointix point)
{
	m_iState &= (~ButtonStateHoving);
	m_iState = 0;
	Repaint();
	CControl::OnMouseOut(point);
}

void CButtonBase::OnMouseDownL(pointix point)
{
	m_iState |= ButtonStatePressed;
	Repaint();
	CControl::OnMouseDownL(point);

}

void CButtonBase::OnMouseUpL(pointix point)
{
	m_iState &= (~ButtonStatePressed);
	Repaint();
	CControl::OnMouseUpL(point);
}

VENUS_END
