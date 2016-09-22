#pragma once

#include "CControl.h"

VENUS_BEG

const int_x TAB_CONTROL_MAX_NAME = 32;
const int_x TAB_INDEX_BUTTON_LEFT = IX_MIN + 1;
const int_x TAB_INDEX_BUTTON_RIGHT = IX_MIN + 2;

class V2D_API TabPanelT
{
public:
	TabPanelT(const char_16 * szText) 
		: Name(szText), WidthOrg(0), Width(0) {}

	chbufferw<TAB_CONTROL_MAX_NAME> Name;
	vector<IControl *> Controls;
	int_x WidthOrg;
	int_x Width;
};

class V2D_API CTabControl : public CControl
{
public:
	CTabControl();
	~CTabControl();
	const oid_t & GetOid() const override;
	void GetMeta(IMetaHelper * pHelper) override;

	rectix GetClient() const override;
	HitTestE NcHitTest(pointix point) const override;
	void OnNcMouseDown(pointix point, MouseButtonE eButton) override;
	void OnNcMouseMove(pointix point) override;
	void OnNcMouseOut(pointix point) override;
	void OnSizeChanged() override;

	int_x AddTab(const char_16 * szText);
	void AddTabControl(int_x iIndex, IControl * pControl);
	int_x GetTabCount() const;
	void SetShown(bool bShown) override;
	void OnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;

	void SetSelectedIndex(int_x iIndex);
	void SetHovingIndex(int_x iIndex);

	int_x GetAllWidth() const;
	int_x NcFindTab(pointix point) const;
protected:
	void SetPaintIndex(int_x iIndex);
	void SetPaintIndexMax(int_x iIndexMax);
protected:
	vector<TabPanelT> m_panels;

	int_x m_iSelectedIndex;
	int_x m_iHovingIndex;

	int_x m_iPaintIndex;
	int_x m_iPaintIndexMax;
public:
	event<IControl *, int_x, int_x *> ItemSelecting;
	event<IControl *, int_x> ItemSelected;
};

VENUS_END
