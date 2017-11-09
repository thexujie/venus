#include "stdafx.h"
#include "CGroupBox.h"

VENUS_BEG

CGroupBox::CGroupBox()
	: m_iTextLeft(GROUPBOX_DEF_LEFT)
{
	m_bStatic = true;
}

CGroupBox::~CGroupBox()
{

}

const cid_t & CGroupBox::GetOid() const
{
	return OID_GroupBox;
}

rectix CGroupBox::GetClient() const
{
	rectix rcClient = CControl::GetClient();
	int_x iOffset = m_font.size;
	rcClient.y += iOffset;
	rcClient.h -= iOffset;
	if(rcClient.w < 0)
		rcClient.w = 0;
	if(rcClient.h < 0)
		rcClient.h = 0;
	return rcClient;
}

void CGroupBox::OnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	CControl::OnNcPaint(pPaint, rcClip, pTheme);
	IDevice2D * pService = Device2D();
	sizeix size = pService->GetTextSize(m_text, m_text.length(), m_font);
	int_x iTextWidth = size.w;
	int_x iTextHeight = size.h;
	int_x iTextLeft = m_iTextLeft;
	int_x iSpace = iTextHeight / 2;

	if(m_text.is_empty())
		pPaint->DrawRect(0, 0, m_rect.w - 1, m_rect.h - 1, SysColorFrameNormal);
	else
	{
		const int_x SPACE = 3;
		pointix points[6];
		points[0].set(iTextLeft - SPACE, iSpace);
		points[1].set(0, iSpace);
		points[2].set(0, m_rect.h - 1);
		points[3].set(m_rect.w - 1, m_rect.h - 1);
		points[4].set(m_rect.w - 1, iSpace);
		points[5].set(iTextLeft + iTextWidth + SPACE, iSpace);

		pPaint->DrawPolygon(points, 6, SysColorFrameNormal);
		pPaint->DrawString(m_text, m_font, pointix(iTextLeft, 0), m_uiForeColor);
	}
}

VENUS_END
