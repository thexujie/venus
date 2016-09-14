#include "stdafx.h"
#include "CStatic.h"

VENUS_BEG

CStatic::CStatic() :
	m_pImage(nullptr), m_eTextAlign(AlignLeftTop), m_eTextWrap(TextWrapNone)
{
	m_bStatic = true;
	m_bDisSelectAble = true;
	m_bNotAcceptScroll = true;
	m_bRepaintTextChanged = true;
	m_bRepaintFontChanged = true;
}

CStatic::~CStatic()
{
	SafeRelease(m_pImage);
}

const oid_t & CStatic::GetOid() const
{
	return OID_Static;
}

void CStatic::SetImage(IImage * pImage)
{
	if(pImage != m_pImage)
	{
		SafeRelease(m_pImage);
		m_pImage = pImage;
		SafeAddRef(m_pImage);
		Repaint();
	}
}

void CStatic::SetTextAlign(AlignE eTextAlign)
{
	if(m_eTextAlign != eTextAlign)
	{
		m_eTextAlign = eTextAlign;
		Repaint();
	}
}

void CStatic::SetTextWrap(TextWrapE eTextWrap)
{
	if(m_eTextWrap != eTextWrap)
	{
		m_eTextWrap = eTextWrap;
		Repaint();
	}
}

void CStatic::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	sizeix szClient = GetClient().size;
	if(m_pImage)
		pPaint->DrawImage(m_pImage, 0, 0, szClient.w, szClient.h);

	if(m_text.is_valid())
	{
		textformat_t format(m_font, m_eTextAlign, m_eTextWrap, TextTrimmingNone, AxisX);
		pPaint->DrawString(m_text, m_text.length(), format, rectix(0, 0, szClient.w, szClient.h), m_uiForeColor);
	}
}

void CStatic::QueryDebugMenu(int_x & iBase, IMenu * pMenu) const
{
	CControl::QueryDebugMenu(iBase, pMenu);
	object_ptr<MenuT> pMenuThis(new MenuT());
	object_ptr<MenuT> pMenuAlign(new MenuT());
	object_ptr<MenuT> pMenuWrap(new MenuT());

	pMenuThis->AddItem(0, L"TextAlign", pMenuAlign);
	pMenuThis->AddItem(0, L"TextWrap", pMenuWrap);

	pMenuAlign->AddItem(iBase + 1, L"LeftTop", MenuGetFlags(m_eTextAlign == AlignLeftTop));
	pMenuAlign->AddItem(iBase + 2, L"TopRight", MenuGetFlags(m_eTextAlign == AlignTopRight));
	pMenuAlign->AddItem(iBase + 3, L"RightBottom", MenuGetFlags(m_eTextAlign == AlignRightBottom));
	pMenuAlign->AddItem(iBase + 4, L"BottomLeft", MenuGetFlags(m_eTextAlign == AlignBottomLeft));
	pMenuAlign->AddItem(iBase + 5, L"TopCenterX", MenuGetFlags(m_eTextAlign == AlignTopCenterX));
	pMenuAlign->AddItem(iBase + 6, L"BottomCenterX", MenuGetFlags(m_eTextAlign == AlignBottomCenterX));
	pMenuAlign->AddItem(iBase + 7, L"LeftCenterY", MenuGetFlags(m_eTextAlign == AlignLeftCenterY));
	pMenuAlign->AddItem(iBase + 8, L"RightCenterY", MenuGetFlags(m_eTextAlign == AlignRightCenterY));
	pMenuAlign->AddItem(iBase + 9, L"CenterXY", MenuGetFlags(m_eTextAlign == AlignCenter));

	pMenuWrap->AddItem(iBase + 10, L"None", MenuGetFlags(m_eTextWrap == TextWrapNone));
	pMenuWrap->AddItem(iBase + 11, L"Char", MenuGetFlags(m_eTextWrap == TextWrapChar));
	pMenuWrap->AddItem(iBase + 12, L"Word", MenuGetFlags(m_eTextWrap == TextWrapWord));

	pMenu->AddItem(0, L"Static", pMenuThis);
	iBase += 20;
}

void CStatic::OnDebugMenu(int_x iBase, int_x iResult)
{
	iBase -= 20;
	switch(iResult - iBase)
	{
	case 1: SetTextAlign(AlignLeftTop); break;
	case 2: SetTextAlign(AlignTopRight); break;
	case 3: SetTextAlign(AlignRightBottom); break;
	case 4: SetTextAlign(AlignBottomLeft); break;
	case 5: SetTextAlign(AlignTopCenterX); break;
	case 6: SetTextAlign(AlignBottomCenterX); break;
	case 7: SetTextAlign(AlignLeftCenterY); break;
	case 8: SetTextAlign(AlignRightCenterY); break;
	case 9: SetTextAlign(AlignCenter); break;
		
	case 10: SetTextWrap(TextWrapNone); break;
	case 11: SetTextWrap(TextWrapChar); break;
	case 12: SetTextWrap(TextWrapWord); break;
	default:
		CControl::OnDebugMenu(iBase, iResult);
		break;
	}
}

VENUS_END
