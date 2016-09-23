#pragma once

#include "BaseInc.h"

class CMainWindow : public CForm
{
public:
	CMainWindow(int_x iWidth = 800, int_x iHeight = 600);
	~CMainWindow();

	void OnCreate();
	void OnClose();

	int_x OnTreeItemSelected(IControl * pControl, TreeItemT * pItem);
	int_x OnListItemSelected(IControl * pControl, ListBoxItemT * pItem);
	int_x OnListDBClick(IControl * pControl, pointix point);
	int_x OnTreeMouseDownR(IControl * pControl, pointix point);

private:
	CFolderTreeView m_treeView;
	CListBox m_listBox;
	CTextLine m_textLine;
	CTextBox m_textBox;
	CFolderListView * pFolderView;
};
