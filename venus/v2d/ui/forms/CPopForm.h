#pragma once

#include "CForm.h"

VENUS_BEG

class V2D_API CPopForm : public CForm
{
public:
	CPopForm();
	~CPopForm();
	const cid_t & GetOid() const;

	bool MouseActiving(pointix point) const;

	void OnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;

	void SetControl(pointix point, IControl * pControl);
protected:
};

VENUS_END
