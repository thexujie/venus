#pragma once

#include "BaseInc.h"

class CDetailWindow : public CForm
{
public:
	CDetailWindow(int_x iWidth = 600, int_x iHeight = 400);

	void SetProcessId(uint_32 uiId);

	void PreOnKeyDown(KeyCodeE eKeyCode);

	int_x OnTextLineSelectedChanged(IControl * pControl, bool bSelected);

	void OnClose();
private:
	CGroupBox m_grpBase;
	CStatic m_staName;
	CStatic m_staPath;
	CStatic m_staCmd;
	CStatic m_staParent;

	CTextLine m_txlName;
	CTextLine m_txlPath;
	CTextLine m_txlCmd;
	CTextLine m_txlParent;

	CTextBox m_txbDetail;
};
