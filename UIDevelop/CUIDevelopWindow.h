#pragma once

#include "BaseInc.h"

class CUIDevelopWindow : public CForm
{
public:
	CUIDevelopWindow();
	~CUIDevelopWindow();

	void OnCreate();
	void OnLoaded();
	void OnShow();
	void OnVisualSizeChanged();
	void GetMeta(IMetaHelper * pHelper);

	void OnMouseDownL(pointix point);
	void OnKeyDown(KeyCodeE key);
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;

private:
	CTextLine m_txl;
};
