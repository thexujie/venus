#pragma once

#include "CButtonBase.h"

VENUS_BEG

class V2D_API CRadio : public CButtonBase
{
public:
	CRadio();
	~CRadio();
	const oid_t & GetOid() const override;

public:
	void OnMouseClickL(pointix point) override;

public:
	void SetChecked(bool bChecked);
	bool IsChecked() const;
	void OnCheckGroup(int_x iGroup) override;
	void CheckedChanged();

public:
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;

protected:
	int_x m_iGroupIndex;

public:
	event<IControl *> OnChecking;
	event<IControl *, bool> OnCheckedChanged;
};

VENUS_END
