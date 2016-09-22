#pragma once

#include "CControl.h"

VENUS_BEG

class V2D_API CPane : public CControl
{
public:
	CPane();
	~CPane();
	const oid_t & GetOid() const override;

	pointix GetOffset() const override;
	rectix GetVisual() const override;
	HitTestE NcHitTest(pointix point) const override;

	void SetCaptionVisible(bool bCaptionVisible);
	bool IsShowCaptionVisible() const;
	void SetCaptionHeight(int_x iCaptionHeight);
	int_x GetCaptionHeight() const;

	void SetSelectedIndex(int_x iIndex);
	void AddPanel(IControl * pPanel);

	void PreOnNcMouseDown(pointix point, MouseButtonE eButton) override;
	void OnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;

	void QueryDebugMenu(int_x & iBase, IMenu * pMenu) const override;
	void OnDebugMenu(int_x iBase, int_x iResult) override;
protected:
	int_x m_iCaptionHeight;
	bool m_bHideCaption;

	int_x m_iSelectedIndex;
	vector<IControl *> m_panels;
	static const int_x CAPTION_HEIGHT_DEFAULT = 24;
};

VENUS_END
