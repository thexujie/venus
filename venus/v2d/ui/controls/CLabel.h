#pragma once

#include "CControl.h"

VENUS_BEG

class V2D_API CLabel : public CControl
{
public:
	CLabel();
	~CLabel();

	const cid_t & GetOid() const override;
public: // CControl
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;
};

VENUS_END
