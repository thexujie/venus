#include "stdafx.h"
#include "CRadio.h"

VENUS_BEG

static const int_x RADIO_SPACE = 3;

CRadio::CRadio():
m_iGroupIndex(0)
{
}

CRadio::~CRadio()
{

}

const cid_t & CRadio::GetOid() const
{
	return OID_Radio;
}

void CRadio::OnMouseClickL(pointix point)
{
	SetChecked(true);
	CControl::OnMouseClickL(point);
}

void CRadio::SetChecked(bool bChecked)
{
	if(bChecked != CheckState(ButtonStateChecked))
	{
		if(bChecked && OnChecking(this) == 0)
		{
			if(bChecked && m_pParent && m_iGroupIndex > 0)
			{
				for_each(m_pParent->GetChildren(), [&](IControl * pControl) 
				{ 
					if(pControl != this) 
						pControl->OnCheckGroup(m_iGroupIndex); 
				});
			}
			ModifyState(ButtonStateChecked, true);
			CheckedChanged();
		}
	}
}

bool CRadio::IsChecked() const
{
	return CheckState(ButtonStateChecked);
}

void CRadio::OnCheckGroup(int_x iGroup)
{
	if(iGroup == m_iGroupIndex)
		SetChecked(false);
}

void CRadio::CheckedChanged()
{
	OnCheckedChanged(this, CheckState(ButtonStateChecked));
}

void CRadio::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	uint_32 uiColors[] = 
	{
		0xFFFFA8C8, 0xFF9B214E, 0xFF9B834E, 0xFFE3CFE0,
		0xFF9B214E, 0xFFFFA8C8, 0xFFFA80AE, 0xFF808080,
		0xFF000000, 0xFFFFA8C8, 0xFF9B214E, 0xFF808080,
		0xFF000000, 0xFFFFA8C8, 0xFF9B214E, 0xFF808080,
	};
	sizeix szClient = GetClient().size;
	int_x iIndex = 0;
	if(m_bDisabled) iIndex = 3;
	else if(CheckState(ButtonStatePressed)) iIndex = 2;
	else if(CheckState(ButtonStateHoving)) iIndex = 1;
	else iIndex = 0;

	int_x iFontSize = GetFontSize();
	int_x iBaseY = (szClient.h - iFontSize) / 2;

	rectix rcIcon(0, iBaseY, iFontSize, iFontSize);
	// ±³¾°
	pPaint->FillCircle(rcIcon, uiColors[0 + iIndex]);
	// Íâ¿ò
	if(m_bMouseIn)
		pPaint->DrawCircle(rcIcon, uiColors[4 + iIndex], 1.0f);
	else
		pPaint->DrawCircle(rcIcon, uiColors[4 + iIndex], 1.0f);
	// ±ê¼Ç
	if(CheckState(ButtonStateChecked))
		pPaint->FillCircle(rcIcon.expand(-2, -2), uiColors[8 + iIndex]);

	iBaseY = (szClient.h - iFontSize) / 2;
	pPaint->DrawString(m_text, m_font, pointix(iFontSize + RADIO_SPACE, iBaseY), uiColors[12 + iIndex]);
	//todo:use theme colors
}

VENUS_END
