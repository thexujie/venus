#include "stdafx.h"
#include "CLabel.h"

VENUS_BEG

CLabel::CLabel()
{

}

CLabel::~CLabel()
{

}

const oid_t & CLabel::GetOid() const
{
	return OID_Lable;
}

void CLabel::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	PaintMenuString(pPaint, m_text, m_text.length(), m_font, 0, 0, IsAviliable() ? m_uiForeColor : SysColorForeDisable);
}

VENUS_END
