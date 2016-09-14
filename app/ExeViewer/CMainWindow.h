#pragma once

#include "BaseInc.h"

class CMainWindow : public CForm
{
public:
	CMainWindow();
	~CMainWindow();

	void PreOnKeyDown(KeyCodeE eKeyCode);
	void OnCreate();
	void OnLoaded();

public:
	void GetMeta(IMetaHelper * pHelper);
	int_x OnTextLineDragRequest(IControl * pControl, IData * pData, DropResultE * peReault);
	int_x OnTextLineDragDrop(IControl * pControl, IData * pData);
	int_x OnTextBoxShowMenu(IControl * pControl, pointix point);

	int_x OnPaneMouseDown(IControl * pControl, pointix point);
public:
	void SetExeFileName(const char_16 * szExeFile);

private:
	CTextLine m_txlFile;
	CTextBox m_txbInfo;

	CDropTarget m_dropTarget;

	textw m_filePath;
	bool m_bUnDecorate;
};
