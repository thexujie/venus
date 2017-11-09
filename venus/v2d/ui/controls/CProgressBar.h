#pragma once

#include "CControl.h"

VENUS_BEG

class V2D_API CProgressBar : public CControl
{
public:
	CProgressBar();
	~CProgressBar();
	const cid_t & GetOid() const override;

	void SetMaxValue(int_x iMaxValue);
	void SetMinValue(int_x iMinValue);
	void SetValue(int_x iValue);
	void SetRange(const RangeInfoT & range);
	void SetRange(int_x iMinValue, int_x iMaxValue, int_x iValue);

	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;

	void SetMarquee(int_x iMarqueePeriod = 100, int_x iMarqueeStep = 0);

protected:
	int_x OnMarqueeTimer(int_x iTime);
protected:
	RangeInfoT m_range;
	int_x m_iMarqueePeriod;
	int_x m_iMarqueeStep;
	int_x m_iMarqueePoint;
};

VENUS_END
