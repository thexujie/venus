#pragma once

#include "CControl.h"

VENUS_BEG

class V2D_API CCheckBox : public CControl
{
public:
	CCheckBox();
	~CCheckBox();
	const oid_t & GetOid() const override;
	void GetMeta(IMetaHelper * pHelper) override;

public:
	void OnTextChanged() override;
	void OnMouseClickL(pointix point) override;
	sizeix GetPreferedSize() const override;
public:
	void SetChecked(bool bChecked);
	bool IsChecked() const;
	void CheckedChanged();

public:
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;

protected:
	bool m_bChecked;
	int_x m_iBoxSize;

public:
	//! 标记大小
	static const int_x BOX_SIZE = 12;
	//! 标记和文本的间距
	static const int_x BOX_SPACE = 3;
public:
	event<IControl *, bool, bool *> Checking;
	event<IControl *, bool> Checked;
};

VENUS_END
