#include "stdafx.h"
#include "CButton.h"

VENUS_BEG

static const int_x DEF_ICON_TEXT_SPACE = 4;

CButton::CButton() :
	m_eIconAlign(AlignLeft), m_eContextAlign(AlignCenter), m_pIcon(nullptr), m_eDialogResult(DialogResultNone)
{
	m_bRepaintMouseInOut = true;
	m_bRepaintMouseDownUpL = true;
	m_bRepaintTextChanged = true;
	m_bRepaintFontChanged = true;
	m_bRepaintEnabledChanged = true;
	m_bRepaintFocusedChanged = true;
	m_bFocusAble = true;
	m_bNotAcceptScroll = true;
	m_margin.set(3, 3, 3, 3);
	m_padding.set(3, 3, 3, 3);
}

CButton::~CButton()
{
	SafeRelease(m_pIcon);
}

const oid_t & CButton::GetOid() const
{
	return OID_Button;
}


void CButton::SetIcon(IImage * pIcon)
{
	if(pIcon != m_pIcon)
	{
		m_pIcon = pIcon;
		SafeAddRef(m_pIcon);
		Repaint();
	}
}

void CButton::SetIconAlign(AlignE eIconAlign)
{
	switch(eIconAlign)
	{
	case AlignLeft:
	case AlignTop:
	case AlignRight:
	case AlignBottom:
		break;
	default:
		throw exp_illegal_argument();
	}
	if(eIconAlign != m_eIconAlign)
	{
		m_eIconAlign = eIconAlign;
		Repaint();
	}
}

void CButton::SetContextAlign(AlignE eContextAlign)
{
	if(eContextAlign != m_eContextAlign)
	{
		m_eContextAlign = eContextAlign;
		Repaint();
	}
}

void CButton::OnTextChanged()
{
	CControl::OnTextChanged();
}

sizeix CButton::GetContentSize() const
{
	sizeix size = MeasureMenuString(m_text, m_text.length(), m_font);
	size += m_padding.size();
	size.w += m_font.size;
	return size;
}

void CButton::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	uint_32 clrBack = SysColorAuto;
	uint_32 clrFrame = SysColorAuto;
	if(!IsAviliable())
	{
		clrBack = SysColorBackDisable;
		clrFrame = SysColorFrameDisable;
	}
	else if(CheckState(ButtonStatePressed))
	{
		clrBack = SysColorBackPressed;
		clrFrame = SysColorFramePressed;
	}
	else if(CheckState(ButtonStateHoving))
	{
		clrBack = SysColorBackHoving;
		clrFrame = SysColorFrameHoving;
	}
	else if(IsFocused())
	{
		clrBack = SysColorBackFocused;
		clrFrame = SysColorFrameFocused;
	}
	else
	{
		clrBack = SysColorBackNormal;
		clrFrame = SysColorFrameNormal;
	}
	rectix rcClient = GetClient();
	pPaint->FillRect(rectix(pointix(), rcClient.size), clrBack);
	pPaint->DrawRect(0, 0, rcClient.w - 1, rcClient.h - 1, clrFrame);

	// 画图标和文字
	if(m_pIcon && !m_text.is_empty())
		DrawIconText(pPaint);
	// 只有图标
	else if(m_pIcon)
		DrawIconOnly(pPaint);
	// 只有文字
	else if(!m_text.is_empty())
		DrawTextOnly(pPaint);
	else {/* 什么都没有 */}
}

void CButton::DrawIconOnly(IPaint * pPaint) const
{
	int_x iTextWidth = GetTextSize();
	int_x iHeight = m_font.size;

	int_x iX = 0, iY = 0;
	if(m_eContextAlign & AlignRight)
		iX = m_rect.w - iTextWidth - m_padding.right;
	else if(m_eContextAlign & AlignCenterX)
		iX = (m_rect.w - iTextWidth) / 2;
	else
		iX = m_padding.left;

	if(m_eContextAlign & AlignBottom)
		iY = m_rect.h - iHeight - m_padding.bottom;
	else if(m_eContextAlign & AlignCenterY)
		iY = (m_rect.h - iHeight) / 2;
	else
		iY = m_padding.top;

	DrawButtonIcon(pPaint, iX, iY);
}

void CButton::DrawTextOnly(IPaint * pPaint) const
{
	rectix rcClient = GetClient();
	int_x iTextWidth = GetTextSize();
	int_x iHeight = m_font.size;

	int_x iX = 0, iY = 0;
	if(m_eContextAlign & AlignRight)
		iX = rcClient.w - iTextWidth - m_padding.right;
	else if(m_eContextAlign & AlignCenterX)
		iX = (rcClient.w - iTextWidth) / 2;
	else
		iX = m_padding.left;

	if(m_eContextAlign & AlignBottom)
		iY = rcClient.h - iHeight - m_padding.bottom;
	else if(m_eContextAlign & AlignCenterY)
		iY = (rcClient.h - iHeight) / 2;
	else
		iY = m_padding.top;

	DrawButtonText(pPaint, iX, iY);
}

void CButton::DrawIconText(IPaint * pPaint) const
{
	// 文本的宽度
	int_x iTextSize = GetTextSize();
	// 文本的高度、图标的宽度和高度
	int_x iTextHeight = m_font.size;
	// 图标的宽度和高度，iIconSize = iTextHeight。
	int_x iIconSize = iTextHeight;

	sizeix siTotal;

	switch(m_eIconAlign)
	{
	case AlignLeft:
	case AlignRight:
		siTotal.w = iTextHeight + DEF_ICON_TEXT_SPACE + iTextSize;
		siTotal.h = iTextHeight;
		break;
	case AlignTop:
	case AlignBottom:
		siTotal.w = maxof(iTextHeight, iIconSize);
		siTotal.h = iTextHeight + DEF_ICON_TEXT_SPACE + iIconSize;
		break;
	default:
		throw exp_bad_state();
	}

	int_x iX = 0, iY = 0;
	if(m_eContextAlign & AlignRight)
		iX = m_rect.w - m_padding.right - siTotal.w;
	else if(m_eContextAlign & AlignCenterX)
		iX = (m_rect.w - siTotal.w) / 2;
	else
		iX = m_padding.left;

	if(m_eContextAlign & AlignBottom)
		iY = m_rect.h - m_padding.bottom - siTotal.h;
	else if(m_eContextAlign & AlignCenterY)
		iY = (m_rect.h - siTotal.h) / 2;
	else
		iY = m_padding.top;

	if(m_eIconAlign == AlignLeft)
	{
		DrawButtonIcon(pPaint, iX, iY);
		DrawButtonText(pPaint, iX + iTextHeight + DEF_ICON_TEXT_SPACE, iY);
	}
	else if(m_eIconAlign == AlignTop)
	{
		DrawButtonIcon(pPaint, iX + (siTotal.w - iIconSize) / 2, iY);
		DrawButtonText(pPaint, iX + (siTotal.w - iTextSize) / 2, iY + iIconSize + DEF_ICON_TEXT_SPACE);
	}
	else if(m_eIconAlign == AlignRight)
	{
		DrawButtonText(pPaint, iX, iY);
		DrawButtonIcon(pPaint, iX + iTextSize + DEF_ICON_TEXT_SPACE, iY);
	}
	else if(m_eIconAlign == AlignBottom)
	{
		DrawButtonText(pPaint, iX + (siTotal.w - iTextSize) / 2, iY);
		DrawButtonIcon(pPaint, iX + (siTotal.w - iIconSize) / 2, iY + iTextHeight + DEF_ICON_TEXT_SPACE);
	}
	else
		throw exp_bad_state();
}

int_x CButton::GetTextSize() const
{
	return MeasureMenuString(m_text, m_text.length(), m_font).w;
}

sizeix CButton::GetIconSize() const
{
	if(m_pIcon)
		return m_pIcon->GetSize();
	else
		return sizeix();
}

void CButton::DrawButtonText(IPaint * pPaint, int_x iX, int_x iY) const
{
	PaintMenuString(pPaint, m_text, m_text.length(), m_font, iX, iY, IsAviliable() ? m_uiForeColor : SysColorForeDisable);
}

void CButton::OnKeyDown(KeyCodeE eKeyCode)
{
	CControl::OnKeyDown(eKeyCode);
}

void CButton::OnKeyUp(KeyCodeE eKeyCode)
{
	switch(eKeyCode)
	{
	case KeyCodeEnter:
	case KeyCodeSpace:
		if(m_eDialogResult != DialogResultNone)
		{
			IForm * pForm = GetAncestor()->ToIForm();
			if(pForm)
				GetApp()->EndRun(pForm->GetFormId(), m_eDialogResult);
		}
	default:
		break;
	}
	CControl::OnKeyUp(eKeyCode);
}

bool CButton::OnHotKey()
{
	if(m_eDialogResult != DialogResultNone)
	{
		IForm * pForm = GetAncestor()->ToIForm();
		if(pForm)
		{
			GetApp()->EndRun(pForm->GetFormId(), m_eDialogResult);
			return true;
		}
	}
	return false;
}

void CButton::OnMouseClickL(pointix point)
{
	CControl::OnMouseClickL(point);
	if(m_eDialogResult != DialogResultNone)
	{
		IForm * pForm = GetAncestor()->ToIForm();
		if(pForm)
			pForm->EndRun(m_eDialogResult);
	}
}

void CButton::SetDialogResult(DialogResultE eDialogResult)
{
	m_eDialogResult = eDialogResult;
}

DialogResultE CButton::GetDialogResult() const
{
	return m_eDialogResult;
}

void CButton::DrawButtonIcon(IPaint * pPaint, int_x iX, int_x iY) const
{
	ensure(m_pIcon);
	if(IsAviliable())
		pPaint->DrawImage(m_pIcon, iX, iY);
	else
	{
		matrix5f matrx;
		matrx.MakeColorGray();
		pPaint->SetImageColorMatrix(matrx);
		pPaint->DrawImage(m_pIcon, iX, iY);
		pPaint->ResetImageColorMatrix();
	}
}

VENUS_END
