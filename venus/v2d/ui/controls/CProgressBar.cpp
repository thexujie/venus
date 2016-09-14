#include "stdafx.h"
#include "CProgressBar.h"

VENUS_BEG

CProgressBar::CProgressBar() :m_iMarqueePeriod(0), m_iMarqueePoint(0), m_iMarqueeStep(0)
{
	m_bStatic = true;
}

CProgressBar::~CProgressBar()
{
	if(m_iMarqueePeriod > 0)
	{
		IApp * pApp = GetApp();
		if(pApp)
			pApp->KillTimer(bind(this, &CProgressBar::OnMarqueeTimer), 0);
		m_iMarqueePeriod = 0;
	}
}

const oid_t & CProgressBar::GetOid() const
{
	return OID_ProgressBar;
}

void CProgressBar::SetMaxValue(int_x iMaxValue)
{
	if(iMaxValue < m_range.Value)
		throw exp_illegal_argument();
	if(iMaxValue != m_range.MaxValue)
	{
		m_range.MaxValue = iMaxValue;
		Repaint();
	}
}

void CProgressBar::SetMinValue(int_x iMinValue)
{
	if(iMinValue > m_range.Value)
		throw exp_illegal_argument();
	if(iMinValue != m_range.MinValue)
	{
		m_range.MinValue = iMinValue;
		Repaint();
	}
}

void CProgressBar::SetValue(int_x iValue)
{
	if(iValue < m_range.MinValue || m_range.MaxValue < iValue)
		throw exp_illegal_argument();
	if(iValue != m_range.Value)
	{
		m_range.Value = iValue;
		Repaint();
	}
}

void CProgressBar::SetRange(const RangeInfoT & range)
{
	SetRange(range.MinValue, range.MaxValue, range.Value);
}

void CProgressBar::SetRange(int_x iMinValue, int_x iMaxValue, int_x iValue)
{
	if(iValue < iMinValue || iMaxValue < iValue || iMinValue > iMaxValue)
		throw exp_illegal_argument();

	if(iMinValue != m_range.MinValue || iValue != m_range.Value || iMaxValue != m_range.MaxValue)
	{
		m_range.MinValue = iMinValue;
		m_range.MaxValue = iMaxValue;
		m_range.Value = iValue;
		Repaint();
	}
}

void CProgressBar::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	rectix rcClient = GetClient();
	pPaint->FillRect(rcClip, SysColorBackNormal);
	if(m_iMarqueePeriod <= 0)
		pPaint->FillRect(rcClient.x, rcClient.y, rcClient.w * m_range.Value / m_range.range(), rcClient.h, SysColor);
	else
	{
		int_x iMarqueeLength = (int_x)(rcClient.w * 0.382f);
		pPaint->FillRect(m_iMarqueePoint - iMarqueeLength, rcClient.y, iMarqueeLength, rcClient.h, SysColor);
	}
	pPaint->DrawRect(rcClient.x, rcClient.y, rcClient.w - 1, rcClient.h - 1, SysColorFrameNormal);
}

void CProgressBar::SetMarquee(int_x iMarqueePeriod, int_x iMarqueeStep)
{
	if(iMarqueePeriod < 0)
		iMarqueePeriod = 0;

	if(!iMarqueeStep)
		iMarqueeStep = GetClient().w / 10;

	if(iMarqueePeriod != m_iMarqueePeriod)
	{
		m_iMarqueePeriod = iMarqueePeriod;
		m_iMarqueeStep = iMarqueeStep;
		m_iMarqueePoint = 0;
		IApp * pApp = GetApp();
		if(pApp)
		{
			if(m_iMarqueePeriod > 0)
				pApp->SetTimer(bind(this, &CProgressBar::OnMarqueeTimer), iMarqueePeriod, 0);
			else
				pApp->KillTimer(bind(this, &CProgressBar::OnMarqueeTimer), 0);
		}
		else

		Repaint();
	}
}

int_x CProgressBar::OnMarqueeTimer(int_x iTime)
{
	rectix rcClient = GetClient();
	int_x iMarqueeLength = (int_x)(rcClient.w * 1.382f);
	m_iMarqueePoint += m_iMarqueeStep;
	m_iMarqueePoint %= iMarqueeLength;
	Repaint();
	return 0;
}

VENUS_END
