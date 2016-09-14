#pragma once

#include "CControl.h"

VENUS_BEG

class V2D_API CLabel : public CControl
{
public:
	CLabel();
	~CLabel();

	const oid_t & GetOid() const;
public: // CControl
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;
};

VENUS_END
