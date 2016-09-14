#pragma once

#include "CControl.h"

VENUS_BEG

const int_x GROUPBOX_DEF_LEFT = 14;

class V2D_API CGroupBox : public CControl
{
public:
	CGroupBox();
	~CGroupBox();

public:
	const oid_t & GetOid() const;

	rectix GetClient() const;
	void OnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;

private:
	int_x m_iTextLeft;
};

VENUS_END
