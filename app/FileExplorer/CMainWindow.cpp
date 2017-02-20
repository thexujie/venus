#include "stdafx.h"
#include "CMainWindow.h"

CMainWindow::CMainWindow(int_x iWidth, int_x iHeight)
{
	SetBoxes(true, true, true);
	SetRect(0, 0, iWidth, iHeight);

	m_textLine.SetRect(10, 5, 780, 25);
	m_treeView.SetRect(10, 40, 208, iHeight - 60);
	m_listBox.SetRect(220, 40, 200, iHeight - 60);
	m_textBox.SetRect(430, 40, 360, iHeight - 60);
	//SetMouseWheelMode(MouseWheelModeFocus);
	m_treeView.SetExpandMode(TreeExpandModeClick);
	m_listBox.SetImeMode(ImeModeOn);
	//m_listBox.SetShowCheck(true);

	//AddChild(&m_textBox);

	pFolderView = new CFolderListView();
	pFolderView->SetDynamic(true);
	pFolderView->SetRect(430, 40, 360, iHeight - 60);
	pFolderView->SetPath(L"F:\\test");

	m_textLine.SetWidthMode(WHModeFill);
	m_treeView.SetHeightMode(WHModeFill);
	m_listBox.SetHeightMode(WHModeFill);
	pFolderView->SetHeightMode(WHModeFill);

	SetLayoutMode(LayoutModeVertical);
	CControl * pControl = new CControl();
	pControl->SetWidthMode(WHModeFill);
	pControl->SetWeight(1);
	pControl->SetDynamic(true);
	pControl->SetLayoutMode(LayoutModeHorizontal);
	pControl->AddControl(&m_treeView);
	pControl->AddControl(&m_listBox);
	pControl->AddControl(pFolderView);
	pFolderView->SetWeight(1);

	AddControl(&m_textLine);
	AddControl(pControl);

	//m_textBox.SetFontSize(30);
	//AddChild(&m_ctlSize);

	m_treeView.MouseDownR += bind(this, &CMainWindow::OnTreeMouseDownR);
	m_treeView.SelectedItemChanged += bind(this, &CMainWindow::OnTreeItemSelected);
	m_listBox.SelectedItemChanged += bind(this, &CMainWindow::OnListItemSelected);
	pFolderView->MouseDBClickL += bind(this, &CMainWindow::OnListDBClick);
	m_treeView.SetAttrFilter(FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN, 0);
	m_textLine.Select(true);
}

CMainWindow::~CMainWindow()
{
}

void CMainWindow::OnCreate()
{
	char_x szDir[MAX_FILE_PATH];
	GetCurrentDirectory(MAX_FILE_PATH, szDir);
	char_x szDir2[MAX_FILE_PATH];
	textmakepath(szDir2, MAX_FILE_PATH, szDir, -1, L"../../bin", -1);
	//m_treeView.SetRoot(L"F:\\firestone\\heroq1\\shdata");
	//m_treeView.SelectPath(L"800config\\UI2\\give");
	//m_treeView.SetRoot(L"F:\\");
	m_treeView.SetRoot(L"../../");
	//m_treeView.SelectPath(L"F:\\firestone\\heroq1\\shdata\\800config\\UI2\\give");
}

void CMainWindow::OnClose()
{
	Show(ShowModeHide);
	Win32::Quit(0);
}

int_x CMainWindow::OnTreeItemSelected(IControl * pControl, TreeItemT * pItem)
{
	m_listBox.RemoveAll();
	if(pItem)
	{
		CSHFileInfoBuffer * pShellBuffer = GetService<CSHFileInfoBuffer>(OID_CSHFileInfoBuffer);
		chbufferw<MAX_FILE_PATH> szName;
		char_x szExt[MAX_FILE_PATH] = {0};
		szName = CFolderTreeView::GetItemPath(pItem);
		pFolderView->SetPath(szName);

		folderinfo_t dirInfo(szName);
		vector<fileinfo_t> files = dirInfo.get_files(FILE_ATTRIBUTE_SYSTEM, 0);
		m_listBox.BegUpdate();
		for(int_x cnt = 0, len = files.size(); cnt != len; ++cnt)
		{
			const fileinfo_t & file = files[cnt];
			file.name(szName, MAX_FILE_PATH);
			file.extention(szExt, MAX_FILE_PATH);
			IWin32ShellItemImage * pshlImage = pShellBuffer->GetShellIcon(szExt);
			IImage * pImage = pshlImage ? pshlImage->GetSpecificImage(sizeix(16, 16), false) : nullptr;
			m_listBox.AddItem(szName, pImage, false);
			SafeRelease(pImage);
		}
		m_listBox.EndUpdate();
	}
	return 0;
}

int_x CMainWindow::OnListItemSelected(IControl * pControl, ListBoxItemT * pItem)
{
	return 0;
}

int_x CMainWindow::OnListDBClick(IControl * pControl, pointix point)
{
	FolderListViewRowT * pRow = (FolderListViewRowT *)pFolderView->GetRowSelected();
	if(!pRow)
		return 0;

	pathinfo_t pinfo(pRow->path);
	if(pinfo.exists)
		m_treeView.SelectPath(pinfo.path);
	return 0;
}

int_x CMainWindow::OnTreeMouseDownR(IControl * pControl, pointix point)
{
	TreeItemT * pItem = m_treeView.FindItem(point);
	m_treeView.SelectItem(pItem);

	MenuT conv;
	conv.AddItem(3001, L"bmp格式");
	conv.AddItem(3002, L"jpg格式");
	conv.AddItem(3003, L"tga格式");
	conv.AddItem(3004, L"png格式");

	MenuT expandMode;
	expandMode.AddItem(11000, L"已禁用", m_treeView.GetExpandMode() == TreeExpandModeNone ? MenuFlagCheck : 0);
	expandMode.AddItem(11001, L"单击展开", m_treeView.GetExpandMode() == TreeExpandModeClick ? MenuFlagCheck : 0);
	expandMode.AddItem(11002, L"双击展开", m_treeView.GetExpandMode() == TreeExpandModeDBClick ? MenuFlagCheck : 0);
	expandMode.AddItem(11003, L"点击节点展开", m_treeView.GetExpandMode() == TreeExpandModeStrick ? MenuFlagCheck : 0);
	expandMode.AddItem(11004, L"点击节点/双击展开",
					   m_treeView.GetExpandMode() == TreeExpandModeStrickOrDBClick ? MenuFlagCheck : 0);

	MenuT menuFont;
	menuFont.AddItem(21000, L"宋体");
	menuFont.AddItem(21001, L"楷体");
	menuFont.AddItem(21002, L"微软雅黑");
	menuFont.AddItem(21003, L"华文行楷");
	menuFont.AddItem(21004, L"华文琥珀");
	menuFont.AddItem(21005, L"华文彩云");

	int_x iFlags = pItem ? 0 : MenuFlagDisable;
	MenuT menu;
	menu.AddItem(-1, pItem ? pItem->Text : L"<未选择>", iFlags);
	menu.AddSplit();
	menu.AddItem(1, L"打开(&O)", iFlags);
	menu.AddItem(2, L"打开方式(&H)", iFlags);
	menu.AddItem(3, L"转换为...(&E)", &conv, iFlags);
	menu.AddSplit();
	menu.AddItem(11, L"展开方式(&E)", &expandMode);
	menu.AddSplit();
	menu.AddItem(4, L"删除(&D)", iFlags);
	menu.AddSplit();
	menu.AddItem(5, L"复制(&C)", iFlags);
	menu.AddItem(6, L"剪切(&X)", iFlags);
	menu.AddSplit();
	menu.AddItem(7, L"显示折叠标记(&FunT)", m_treeView.IsShowExpand());
	menu.AddSplit();
	menu.AddItem(0, L"TextLine 字体(&F)", &menuFont);

	int_x iResult = pControl->PopupMenu(ToNoneCl(point), &menu);
	switch(iResult)
	{
	case 7:
		m_treeView.SetShowExpand(!m_treeView.IsShowExpand());
		break;
	case 11000:
		m_treeView.SetExpandMode(TreeExpandModeNone);
		break;
	case 11001:
		m_treeView.SetExpandMode(TreeExpandModeClick);
		break;
	case 11002:
		m_treeView.SetExpandMode(TreeExpandModeDBClick);
		break;
	case 11003:
		m_treeView.SetExpandMode(TreeExpandModeStrick);
		break;
	case 11004:
		m_treeView.SetExpandMode(TreeExpandModeStrickOrDBClick);
		break;

	case 21000:
		m_textLine.SetFontName(L"宋体");
		break;
	case 21001:
		m_textLine.SetFontName(L"楷体");
		break;
	case 21002:
		m_textLine.SetFontName(L"微软雅黑");
		break;
	case 21003:
		m_textLine.SetFontName(L"华文行楷");
		break;
	case 21004:
		m_textLine.SetFontName(L"华文琥珀");
		break;
	case 21005:
		m_textLine.SetFontName(L"华文彩云");
		break;

	default:
		break;
	}
	return 0;
}
