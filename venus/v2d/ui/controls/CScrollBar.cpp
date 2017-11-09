#include "stdafx.h"
#include "CScrollBar.h"

VENUS_BEG

CScrollBar::CScrollBar() :
	m_eDirection(AxisX), m_iMin(0), m_iMax(0), m_iValue(0), m_iLine(0),
	m_iPage(0), m_eScrollPoint(ScrollPointInvalid), m_bDraging(false), m_iDragOff(0),
	m_pScrollTarget(nullptr)
{
	m_bRepaintMouseDownUpL = true;
	m_bRepaintEnabledChanged = true;
	m_bDisSelectAble = true;
}

CScrollBar::~CScrollBar()
{

}

IScroll * CScrollBar::ToIScroll() const
{
	return (IScroll *)this;
}

const cid_t & CScrollBar::GetOid() const
{
	return OID_ScrollBar;
}

void CScrollBar::SetDirection(AxisE eDirection)
{
	switch(eDirection)
	{
	case AxisX:
	case AxisY:
		break;
	default:
		throw exp_illegal_argument();
	}

	m_eDirection = eDirection;
}

void CScrollBar::SetMinValue(int_x iMin)
{
	if(iMin > m_iValue)
		throw exp_illegal_argument();

	if(iMin != m_iMin)
	{
		m_iMin = iMin;
		MinValueChanged();
	}
}

int_x CScrollBar::GetMinValue() const
{
	return m_iMin;
}

int_x CScrollBar::GetRangeValue() const
{
	return m_iMax - m_iMin;
}
void CScrollBar::SetMaxValue(int_x iMax)
{
	if(iMax < m_iValue)
		throw exp_illegal_argument();

	if(iMax != m_iMax)
	{
		m_iMax = iMax;
		MaxValueChanged();
	}
}

int_x CScrollBar::GetMaxValue() const
{
	return m_iMax;
}

void CScrollBar::SetValue(int_x iValue)
{
	if(iValue < m_iMin)
		iValue = m_iMin;
	else if(iValue > m_iMax)
		iValue = m_iMax;
	else {}

	if(iValue != m_iValue)
	{
		int_x iOffset = iValue - m_iValue;
		m_iValue = iValue;
		ValueChanged(iOffset);
	}
}

int_x CScrollBar::GetValue() const
{
	return m_iValue;
}

void CScrollBar::SetRangeInfo(int_x iMin, int_x iMax, int_x iValue)
{
	if((iMin > iMax) || (iValue < iMin) || (iValue > iMax))
		throw exp_illegal_argument();

	bool bMin = iMin != m_iMin;
	bool bMax = iMax != m_iMax;
	bool bVal = iValue != m_iValue;

	if(bMin || bMax || bVal)
	{
		int_x iOffset = iValue - m_iValue;
		m_iMin = iMin;
		m_iMax = iMax;
		m_iValue = iValue;
		if(bMin)
			MinValueChanged();
		if(bMax)
			MaxValueChanged();
		if(bVal)
			ValueChanged(iOffset);
	}
}

void CScrollBar::SetRangeInfo(const RangeInfoT & range)
{
	SetRangeInfo(range.MinValue, range.MaxValue, range.Value);
}

void CScrollBar::GetRangeInfo(int_x * piMin, int_x * piMax, int_x * piValue) const
{
	if(piMin)
		*piMin = m_iMin;
	if(piMax)
		*piMax = m_iMax;
	if(piValue)
		*piValue = m_iValue;
}

RangeInfoT CScrollBar::GetRangeInfo() const
{
	return RangeInfoT(m_iMin, m_iMax, m_iValue);
}

float_32 CScrollBar::GetRate() const
{
	return float_32(m_iValue - m_iMin) / float_32(m_iMax - m_iMin);
}

AxisE CScrollBar::GetDirection() const
{
	return m_eDirection;
}

void CScrollBar::SetLineValue(int_x iLine)
{
	if(iLine != m_iLine)
	{
		m_iLine = iLine;
		LineValueChanged();
	}
}

int_x CScrollBar::GetLineValue() const
{
	return m_iLine;
}

void CScrollBar::SetPageValue(int_x iPage)
{
	if(iPage != m_iPage)
	{
		m_iPage = iPage;
		PageValueChanged();
	}
}

int_x CScrollBar::GetPageValue() const
{
	return m_iPage;
}

void CScrollBar::SetScrollInfo(int_x iMin, int_x iMax, int_x iValue, int_x iLine, int_x iPage)
{
	int_x iTotal = iMax - iMin;
	if(iMin > iMax || iValue < iMin || iValue > iMax || 
		iLine < 0 || iPage < 0)
		throw exp_illegal_argument();

	SetRangeInfo(iMin, iMax, iValue);

	bool bLile = iLine != m_iLine;
	bool bPage = iPage != m_iPage;
	if(bLile || bPage)
	{
		m_iLine = iLine;
		m_iPage = iPage;
		if(bLile)
			LineValueChanged();
		if(bPage)
			PageValueChanged();
	}
}
void CScrollBar::SetScrollInfo(const ScrollInfoT & scroll)
{
	SetScrollInfo(scroll.MinValue, scroll.MaxValue, scroll.Value, scroll.LineValue, scroll.PageValue);
}

ScrollInfoT CScrollBar::GetScrollInfo() const
{
	return ScrollInfoT(m_iMin, m_iMax, m_iValue, m_iLine, m_iPage);
}

void CScrollBar::LineUp()
{
	SetValue(m_iValue - m_iLine);
}

void CScrollBar::PageUp()
{
	SetValue(m_iValue - m_iPage);
}

void CScrollBar::LineDown()
{
	SetValue(m_iValue + m_iLine);
}

void CScrollBar::PageDown()
{
	SetValue(m_iValue + m_iPage);
}

void CScrollBar::MinValueChanged()
{
	Repaint();
}

void CScrollBar::MaxValueChanged()
{
	Repaint();
}

void CScrollBar::ValueChanged(int_x iOffset)
{
	Repaint();
	if(m_pScrollTarget)
		m_pScrollTarget->PreOnScroll(this, iOffset);

	OnValueChanged(this, m_iValue, iOffset);
}

void CScrollBar::LineValueChanged()
{
	Repaint();
}

void CScrollBar::PageValueChanged()
{
	Repaint();
}

void CScrollBar::SetScrollTarget(IControl * pScrollTarget)
{
	m_pScrollTarget = pScrollTarget;
}

IControl * CScrollBar::GetScrollTarget() const
{
	return m_pScrollTarget;
}

void CScrollBar::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	//if(!m_pParent->IsMouseNcIn())
	//	return;

	switch(m_eDirection)
	{
	case AxisX:
		DrawScrollX(pPaint);
		break;
	case AxisY:
		DrawScrollY(pPaint);
		break;
	default:
		break;
	}
}

void CScrollBar::OnMouseDownL(pointix point)
{
	CControl::OnMouseDownL(point);
	m_bDraging = true;
	switch(m_eScrollPoint)
	{
	case ScrollPointBar:
		Capture(true);
		Repaint();
		break;
	case ScrollPointLineUp:
		LineUp();
		Repaint();
		break;
	case ScrollPointPageUp:
		PageUp();
		Repaint();
		break;
	case ScrollPointPageDown:
		PageDown();
		Repaint();
		break;
	case ScrollPointLineDown:
		LineDown();
		Repaint();
		break;
	default:
		break;
	}
}

void CScrollBar::OnMouseUpL(pointix point)
{
	Capture(false);
	Repaint();
	m_bDraging = false;
	SetScrollPoint(GetMouseScrollPoint(point));
	CControl::OnMouseUpL(point);
}

void CScrollBar::OnMouseMove(pointix point)
{
	if(m_bDraging && m_eScrollPoint == ScrollPointBar)
	{
		sizeix szClient = GetClient().size;
		if(m_eDirection == AxisY)
		{
			int_x iBarY = point.y - m_iDragOff;
			int_x iLineH = 0, iBarH = 0;
			GetBarInfoY(m_iValue, &iLineH, nullptr, &iBarH);
			int_x iValue = m_iMin + (m_iMax - m_iMin) * (iBarY - iLineH) / (szClient.h - iLineH * 2 - iBarH);
			SetValue(iValue);
		}
		else if(m_eDirection == AxisX)
		{
			int_x iBarX = point.x - m_iDragOff;
			int_x iLineW = 0, iBarW = 0;
			GetBarInfoX(m_iValue, &iLineW, nullptr, &iBarW);
			int_x iValue = m_iMin + (m_iMax - m_iMin) * (iBarX - iLineW) / (szClient.w - iLineW * 2 - iBarW);
			SetValue(iValue);
		}
		else
			throw exp_bad_state();
	}
	else if(!m_bDraging)
		SetScrollPoint(GetMouseScrollPoint(point, &m_iDragOff));
	else{}
	CControl::OnMouseMove(point);
}

void CScrollBar::OnMouseOut(pointix point)
{
	m_bDraging = false;
	SetScrollPoint(ScrollPointInvalid);
	CControl::OnMouseOut(point);
}

ScrollPointE CScrollBar::GetMouseScrollPoint(pointix point, int_x * piBarOff/* = nullptr*/) const
{
	sizeix siClient = GetSize();
	if(m_eDirection == AxisY)
	{
		if(point.x < 0 || point.x > siClient.w || point.y < 0)
			return ScrollPointInvalid;

		int_x iLineH = 0, iBarY = 0, iBarH = 0;
		GetBarInfoY(m_iValue, &iLineH, &iBarY, &iBarH);
		if(point.y < iLineH)
			return ScrollPointLineUp;
		else if(point.y < siClient.h - iLineH)
		{
			if(iBarH == 0)
				return ScrollPointInvalid;
			else if(point.y < iBarY)
				return ScrollPointPageUp;
			else if(point.y < iBarY + iBarH)
			{
				if(piBarOff)
					*piBarOff = point.y - iBarY;
				return ScrollPointBar;
			}
			else
				return ScrollPointPageDown;
		}
		else if(point.y < siClient.h)
			return ScrollPointLineDown;
		else
			return ScrollPointInvalid;
	}
	else if(m_eDirection == AxisX)
	{
		if(point.y < 0 || point.y > siClient.h|| point.x < 0)
			return ScrollPointInvalid;

		int_x iLineW = 0, iBarX = 0, iBarW = 0;
		GetBarInfoX(m_iValue, &iLineW, &iBarX, &iBarW);

		if(point.x < iLineW)
			return ScrollPointLineUp;
		else if(point.x < siClient.w - iLineW)
		{
			if(iBarW == 0)
				return ScrollPointInvalid;
			else if(point.x < iBarX)
				return ScrollPointPageUp;
			else if(point.x < iBarX + iBarW)
			{
				if(piBarOff)
					*piBarOff = point.x - iBarX;
				return ScrollPointBar;
			}
			else
				return ScrollPointPageDown;
		}
		else if(point.x < siClient.w)
			return ScrollPointLineDown;
		else
			return ScrollPointInvalid;
	}
	else
		throw exp_bad_state();
}

void CScrollBar::SetScrollPoint(ScrollPointE eScrollPoint)
{
	if(eScrollPoint != m_eScrollPoint)
	{
		m_eScrollPoint = eScrollPoint;
		Repaint();
	}
}

void CScrollBar::GetBarInfoX(int_x iValue, int_x * piLineW, int_x * piBarX, int_x * piBarW) const
{
	if(piLineW || piBarX || piBarW)
	{
		if(iValue < m_iMin) iValue = m_iMin;
		else if(iValue > m_iMax) iValue = m_iMax;

		int_x iLineW = 0, iBarX = 0, iBarW = 0;
		sizeix siClient = GetClient().size;
		int_x iSliderW = siClient.w - siClient.h * 2;
		if(iSliderW <= siClient.h || m_iMin >= m_iMax)
		{
			if(iSliderW <= 0)
			{
				iLineW = siClient.w / 2;
				iBarX = 0;
				iBarW = 0;
			}
			else
			{
				iLineW = siClient.h;
				iBarX = 0;
				iBarW = 0;
			}
		}
		else
		{
			int_x iTotal = m_iMax - m_iMin + m_iPage;
			iLineW = siClient.h;
			iBarW = iSliderW * m_iPage / iTotal;
			if(iBarW < siClient.h)
				iBarW = siClient.h;
			iBarX = iLineW + (iSliderW - iBarW) * (iValue - m_iMin) / (m_iMax - m_iMin);
		}

		if(piLineW)
			*piLineW = iLineW;
		if(piBarX)
			*piBarX = iBarX;
		if(piBarW)
			*piBarW = iBarW;
	}
}

void CScrollBar::GetBarInfoY(int_x iValue, int_x * piLineH, int_x * piBarY, int_x * piBarH) const
{
	if(piLineH || piBarY || piBarH)
	{
		if(iValue < m_iMin) iValue = m_iMin;
		else if(iValue > m_iMax) iValue = m_iMax;

		int_x iLineH = 0, iBarY = 0, iBarH = 0;
		sizeix siClient = GetClient().size;
		int_x iSliderH = siClient.h - siClient.w * 2;
		if(iSliderH <= siClient.w || m_iMin >= m_iMax)
		{
			if(iSliderH <= 0)
			{
				iLineH = siClient.h / 2;
				iBarY = 0;
				iBarH = 0;
			}
			else
			{
				iLineH = siClient.w;
				iBarY = 0;
				iBarH = 0;
			}
		}
		else
		{
			int_x iTotal = m_iMax - m_iMin + m_iPage;
			iLineH = siClient.w;
			iBarH = iSliderH * m_iPage / iTotal;
			if(iBarH < siClient.w)
				iBarH = siClient.w;

			iBarY = iLineH + (iSliderH - iBarH) * (iValue - m_iMin) / (m_iMax - m_iMin);
		}

		if(piLineH)
			*piLineH = iLineH;
		if(piBarY)
			*piBarY = iBarY;
		if(piBarH)
			*piBarH = iBarH;
	}
}

void CScrollBar::DrawScrollX(IPaint * pPaint) const
{
	sizeix siClient = GetClient().size;

	int_x iLineW = 0, iBarX = 0, iBarW = 0;
	GetBarInfoX(m_iValue, &iLineW, &iBarX, &iBarW);
	rectix rcLineUp(0, 0, iLineW, siClient.h);
	rectix rcLineDown(siClient.w - iLineW, 0, iLineW, siClient.h);

	// 背景色。
	pPaint->FillRect(0, 0, siClient.w, siClient.h, SysColorBarBackNormal);

	// 上移行、下一行。
	if(m_eScrollPoint == ScrollPointLineUp)
	{
		if(m_bDraging)
		{
			pPaint->FillRect(rcLineUp, SysColorBarPressed);
			PaintArrowL(pPaint, rcLineUp, SysColorBarHighlight);
		}
		else
		{
			pPaint->FillRect(rcLineUp, SysColorBarHighlight);
			PaintArrowL(pPaint, rcLineUp, SysColorBarPressed);
		}
	}
	else if(!IsAviliable())
		PaintArrowL(pPaint, rcLineUp, SysColorBarDisabled);
	else
		PaintArrowL(pPaint, rcLineUp, SysColorBarPressed);

	if(m_eScrollPoint == ScrollPointLineDown)
	{
		if(m_bDraging)
		{
			pPaint->FillRect(rcLineDown, SysColorBarPressed);
			PaintArrowR(pPaint, rcLineDown, SysColorBarHighlight);
		}
		else
		{
			pPaint->FillRect(rcLineDown, SysColorBarHighlight);
			PaintArrowR(pPaint, rcLineDown, SysColorBarPressed);
		}
	}
	else if(!IsEnabled())
		PaintArrowR(pPaint, rcLineDown, SysColorBarDisabled);
	else
		PaintArrowR(pPaint, rcLineDown, SysColorBarPressed);

	if(IsEnabled())
	{
		rectix rcBar(iBarX, 0, iBarW, siClient.h);
		if(m_eScrollPoint == ScrollPointBar)
		{
			if(m_bDraging)
				pPaint->FillRect(rcBar, SysColorBarPressed);
			else
				pPaint->FillRect(rcBar, SysColorBarHighlight);
		}
		else
			pPaint->FillRect(rcBar, SysColorBarNormal);
	}
}

void CScrollBar::DrawScrollY(IPaint * pPaint) const
{
	sizeix siClient = GetClient().size;

	int_x iLineH = 0, iBarY = 0, iBarH = 0;
	GetBarInfoY(m_iValue, &iLineH, &iBarY, &iBarH);
	rectix rcLineUp(0, 0, siClient.w, iLineH);
	rectix rcLineDown(0, siClient.h - iLineH, siClient.w, iLineH);

	// 背景色。
	pPaint->FillRect(0, 0, siClient.w, siClient.h, SysColorBackNormal);

	// 上移行、下一行。
	if(m_eScrollPoint == ScrollPointLineUp)
	{
		if(m_bDraging)
		{
			pPaint->FillRect(rcLineUp, SysColorBarPressed);
			PaintArrowT(pPaint, rcLineUp, SysColorBarHighlight);
		}
		else
		{
			pPaint->FillRect(rcLineUp, SysColorBarHighlight);
			PaintArrowT(pPaint, rcLineUp, SysColorBarPressed);
		}
	}
	else if(!IsEnabled())
		PaintArrowT(pPaint, rcLineUp, SysColorBarDisabled);
	else
		PaintArrowT(pPaint, rcLineUp, SysColorBarPressed);

	if(m_eScrollPoint == ScrollPointLineDown)
	{
		if(m_bDraging)
		{
			pPaint->FillRect(rcLineDown, SysColorBarPressed);
			PaintArrowB(pPaint, rcLineDown, SysColorBarHighlight);
		}
		else
		{
			pPaint->FillRect(rcLineDown, SysColorBarHighlight);
			PaintArrowB(pPaint, rcLineDown, SysColorBarPressed);
		}
	}
	else if(!IsEnabled())
		PaintArrowB(pPaint, rcLineDown, SysColorBarDisabled);
	else
		PaintArrowB(pPaint, rcLineDown, SysColorBarPressed);

	if(IsEnabled())
	{
		rectix rcBar(0, iBarY, siClient.w, iBarH);
		if(m_eScrollPoint == ScrollPointBar)
		{
			if(m_bDraging)
				pPaint->FillRect(rcBar, SysColorBarPressed);
			else
				pPaint->FillRect(rcBar, SysColorBarHighlight);
		}
		else
			pPaint->FillRect(rcBar, SysColorBarNormal);
	}
}

VENUS_END
