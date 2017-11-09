#include "stdafx.h"
#include "CCheckBox.h"

VENUS_BEG

CCheckBox::CCheckBox()
: m_bChecked(false), m_iBoxSize(BOX_SIZE)
{
	m_bRepaintMouseDownUpL = true;
	m_bRepaintMouseInOut = true;
	m_bRepaintEnabledChanged = true;
	m_margin.set(3, 3, 3, 3);
}

CCheckBox::~CCheckBox()
{

}

const cid_t & CCheckBox::GetOid() const
{
	return OID_CheckBox;
}

void CCheckBox::GetMeta(IMetaHelper * pHelper)
{
	CControl::GetMeta(pHelper);
	pHelper->help_evt(L"Checking", Checking);
	pHelper->help_evt(L"Checked", Checked);
}

sizeix CCheckBox::GetPreferedSize() const
{
	IDevice2D * pDevice = Device2D();
	sizeix size;
	size.w = m_iBoxSize + BOX_SPACE +
		pDevice->GetTextSize(m_text, m_text.length(), m_font).w;

	size.h = m_font.size;
	if(size.h < m_iBoxSize)
		size.h = m_iBoxSize;
	return size;
}

void CCheckBox::OnTextChanged()
{
	CControl::OnTextChanged();
}

void CCheckBox::OnMouseClickL(pointix point)
{
	SetChecked(!m_bChecked);
	CControl::OnMouseClickL(point);
}

void CCheckBox::SetChecked(bool bChecked)
{
	if(bChecked != m_bChecked)
	{
		Checking(this, bChecked, &bChecked);
		if(bChecked != m_bChecked)
		{
			m_bChecked = bChecked;
			CheckedChanged();
		}
	}
}

bool CCheckBox::IsChecked() const
{
	return m_bChecked;
}

void CCheckBox::CheckedChanged()
{
	Checked(this, m_bChecked);
}

void CCheckBox::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	sizeix szInner = GetClient().size;
	rectix rcIcon(0, (szInner.h - m_iBoxSize) / 2, m_iBoxSize, m_iBoxSize);

	//SysColorE eBack = SysColorAuto;
	//SysColorE eFore = SysColorAuto;
	//if(!IsAviliable())
	//{
	//	uiBackColor = ControlColors.BackDisable;
	//	uiFrameColor = ControlColors.ForeDisable;
	//}
	//else if(IsMouseDownL())
	//{
	//	uiBackColor = ControlColors.BackSelected;
	//	uiFrameColor = ControlColors.FrameSelected;
	//}
	//else if(IsMouseIn())
	//{
	//	uiBackColor = ControlColors.BackHoving;
	//	uiFrameColor = ControlColors.FrameHoving;
	//}
	//else
	//{
	//	uiBackColor = m_uiBackColor;
	//	uiFrameColor = m_uiForeColor;
	//}

	uint_32 clrFore = SysColorAuto;
	uint_32 clrBack = SysColorAuto;
	uint_32 clrFrame = SysColorAuto;

	if(!IsAviliable())
		clrFrame = SysColorFrameDisable;
	else if(IsMouseDownL())
		clrFrame = SysColorFramePressed;
	else if(IsMouseIn())
		clrFrame = SysColorFrameHoving;
	else
		clrFrame = SysColorFrameNormal;

	if(IsAviliable())
	{
		clrFore = m_uiForeColor;
		clrBack = m_uiBackColor;
	}
	else
	{
		clrFore = SysColorForeDisable;
		clrBack = SysColorBackDisable;
	}

	PaintCheck(pPaint, rcIcon, clrBack, clrFrame, clrFore, m_bChecked);
	pPaint->DrawString(m_text, m_font, pointix(m_iBoxSize + BOX_SPACE, (szInner.h - m_font.size) / 2), clrFore);
}

VENUS_END
