#include "stdafx.h"
#include "CMainWindow.h"
#include <shlobj.h> 
#include "CShellLink.h"

static BOOL CALLBACK MainWindowEnumWindowsFunc(HWND hWnd, LPARAM lParam);

CMainWindow::CMainWindow():
	m_iSortIndex(0)
{
	bool bIsAdmin = false;

#if MSC_VER >= MSC_2013
	OSVERSIONINFOEXW osver = {sizeof(OSVERSIONINFOEXW)};
	DWORDLONG dwlConditionMask = 0;
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
	if(VerifyVersionInfo(&osver, VER_MAJORVERSION, dwlConditionMask))
#else
	OSVERSIONINFOW osver = {sizeof(OSVERSIONINFOW)};
	GetVersionExW(&osver);
	if(osver.dwMajorVersion >= 6)
#endif
	{
		bIsAdmin = CProcess(GetCurrentProcessId(), PROCESS_QUERY_INFORMATION).IsAdmin();
		m_chbShowAllUser.SetChecked(bIsAdmin);
		m_chbShowAllUser.SetEnable(!bIsAdmin);
	}
	else
	{
		bIsAdmin = CProcess(GetCurrentProcessId(), PROCESS_QUERY_INFORMATION).IsAdminUser();
		// XP 下没有权限来调整。
		m_chbShowAllUser.SetChecked(bIsAdmin);
		m_chbShowAllUser.SetEnable(false);
	}
	
	CProcess process(GetCurrentProcessId(), PROCESS_QUERY_INFORMATION);
	if(!process.SetPrivilige(SE_DEBUG_NAME, true))
	{
		log1(L"Warning:无法获得 SE_DEBUG_NAME 权限，%s\n", Win32::FormatWinError());
	}

	arrayfill(m_arrSortAscend, 20, false);
}

void CMainWindow::GetMeta(IMetaHelper * pHelper)
{
	CForm::GetMeta(pHelper);
	pHelper->help_fun(L"OnTabSelectChanged", bind(this, &CMainWindow::OnTabSelectChanged));
	pHelper->help_fun(L"OnDetailListViewMouseDownR", bind(this, &CMainWindow::OnDetailListViewMouseDownR));
	pHelper->help_fun(L"OnDetailShowDetail", bind(this, &CMainWindow::OnDetailShowDetail));
	pHelper->help_fun(L"OnDetailListViewHeaderPressed", bind(this, &CMainWindow::OnDetailListViewHeaderPressed));
	pHelper->help_fun(L"OnDetailListViewSelectingRow", bind(this, &CMainWindow::OnDetailListViewSelectingRow));
	pHelper->help_fun(L"OnDetailListViewSelectRow", bind(this, &CMainWindow::OnDetailListViewSelectRow));
	pHelper->help_fun(L"OnDetailShowAllProcess", bind(this, &CMainWindow::OnDetailShowAllProcess));
	pHelper->help_fun(L"OnDetailShowDetail", bind(this, &CMainWindow::OnDetailShowDetail));

	pHelper->help(L"lstProgram", &m_lstProgram);
	pHelper->help(L"lstDetail", &m_lstDetail);
	pHelper->help(L"chbShowAllUser", &m_chbShowAllUser);
	pHelper->help(L"btnShowDetail", &m_btnShowDetail);
	pHelper->help(L"lstAutoStart", &m_lstAutoStart);
	pHelper->help(L"lstService", &m_lstService);
}

int_x CMainWindow::OnTabSelectChanged(IControl * pTabControl, int_x iIndex)
{
	switch(iIndex)
	{
	case 0:
		RefreshProgram();
		break;
	case 3:
		if(m_lstAutoStart.GetRowCount() == 0)
			RefreshAutoStart();
		break;
	case 5:
		RefreshDetail();
		break;
	case 6:
		if(m_lstService.GetRowCount() == 0)
			RefreshServices();
		break;
	default:
		break;
	}
	return 0;
}
int_x CMainWindow::OnDetailListViewHeaderPressed(IControl * pListView, pointix point)
{
	if(m_lstDetail.GetHovingRow() != LISTVIEW_HEADER_ROW ||
		m_lstDetail.GetSizingCol() >= 0)
		return 0;

	int_x iCol = m_lstDetail.GetHovingCol();
	const ListViewColT & col = m_lstDetail.GetCol(iCol);
	m_iSortIndex = col.Col;
	bool & bDescend = m_arrSortAscend[m_iSortIndex];
	m_bSortAscend = !bDescend;
	bDescend = !bDescend;

	m_lstDetail.Sort(bind(this, &CMainWindow::DetailListViewSortCmpFunc));
	return 0;
}

int_x CMainWindow::OnDetailListViewMouseDownR(IControl * pListView, pointix point)
{
	static MenuFlag eFlags[6] = {MenuFlagNone, MenuFlagNone, MenuFlagCheck, MenuFlagNone, MenuFlagNone, MenuFlagNone};
	textw text;
	ProcessAllInfo * pInfo = nullptr;
	int_x iProcessId = 0;
	int_x iIndex = m_lstDetail.GetSelectedRow();
	if(iIndex >= 0)
	{
		pInfo = (ProcessAllInfo *)m_lstDetail.GetRowUserData(iIndex);
		iProcessId = pInfo->prcInfo.ID;
		text.format(L"第 %d 个进程, ID：%d(%s)", pInfo->iIndex, iProcessId, pInfo->file);
	}
	else
	{
		text = L"未选中进程";
	}

	// 重叠引用会导致析构不明。
	//MenuT menuPriority;
	//MenuT menuLineSpace;
	//MenuT menu;

	MenuT * pMenuPriority = new MenuT();
	MenuT * pMenuLineSpace = new MenuT();
	MenuT * pMenuLineSpace2 = new MenuT();
	MenuT * pMenuViewMode = new MenuT();
	MenuT * pMenuDevice2D = new MenuT();
	MenuT * pMenu = new MenuT();

	MenuT & menuPriority = *pMenuPriority;
	MenuT & menuLineSpace = *pMenuLineSpace;
	MenuT & menuLineSpace2 = *pMenuLineSpace2;
	MenuT & menu = *pMenu;

	menuPriority.AddItem(50, L"实时", eFlags[0]);
	menuPriority.AddItem(51, L"高", eFlags[1]);
	menuPriority.AddItem(52, L"高于标准", eFlags[2]);
	menuPriority.AddItem(53, L"标准", eFlags[3]);
	menuPriority.AddItem(54, L"低于标准", eFlags[4]);
	menuPriority.AddItem(55, L"低", eFlags[5]);
	// menuPriority.AddItem(55, L"主菜单", pMenu); // 循环吧，菜单。

	menuLineSpace.AddItem(60, L"增加");
	menuLineSpace.AddItem(61, L"减小");
	menuLineSpace2.AddItem(62, L"增加");
	menuLineSpace2.AddItem(63, L"减小");

	menu.AddItem(0, text, MenuFlagDisable);
	menu.AddItem(1, L"查看进程详细信息(&D)", pInfo ? MenuFlagNone : MenuFlagDisable);
	menu.AddSplit();
	menu.AddItem(2, L"结束进程(&O)");
	menu.AddItem(3, L"挂起进程(&S)");
	menu.AddItem(4, L"唤醒进程(&R)");
	menu.AddItem(5, L"设置优先级(&P)", &menuPriority, pInfo ? MenuFlagNone : MenuFlagDisable);
	menu.AddSplit();
	menu.AddItem(6, L"显示标题", m_lstDetail.IsShowHeader() ? MenuFlagSelected : 0);
	menu.AddItem(7, L"显示横线", m_lstDetail.IsShowFrameX() ? MenuFlagSelected : 0);
	menu.AddItem(8, L"显示竖线", m_lstDetail.IsShowFrameY() ? MenuFlagSelected : 0);
	menu.AddSplit();
	menu.AddItem(9, L"移除全部项");
	menu.AddItem(10, L"重新获取");
	menu.AddItem(0, L"行间距", &menuLineSpace);
	menu.AddItem(0, L"行高", &menuLineSpace2);
	menu.AddItem(0, L"查看", pMenuViewMode);
	menu.AddSplit();
	menu.AddItem(0, L"Device2D", pMenuDevice2D);

	Device2DTypeE eType = Get2DDevice()->GetType();
	pMenuDevice2D->AddItem(100, L"GDI", MenuGetFlags(eType == Device2DTypeGdi));
	pMenuDevice2D->AddItem(101, L"GDI+", MenuGetFlags(eType == Device2DTypeGdip));
	pMenuDevice2D->AddItem(102, L"Direct2D", MenuGetFlags(eType == Device2DTypeDirect2D));
	pMenuViewMode->AddItem(103, L"详细信息", MenuGetFlags(m_lstDetail.GetViewMode() == ListViewModeDetails));
	pMenuViewMode->AddItem(104, L"平铺", MenuGetFlags(m_lstDetail.GetViewMode() == ListViewModeTile));
	pMenuViewMode->AddItem(105, L"小图标", 
		MenuGetFlags(m_lstDetail.GetViewMode() == ListViewModeIcon && m_lstDetail.GetIconSize() == 16));
	pMenuViewMode->AddItem(106, L"中等图标",
		MenuGetFlags(m_lstDetail.GetViewMode() == ListViewModeIcon && m_lstDetail.GetIconSize() == 48));
	pMenuViewMode->AddItem(107, L"大图标",
		MenuGetFlags(m_lstDetail.GetViewMode() == ListViewModeIcon && m_lstDetail.GetIconSize() == 128));
	int_x iRet = pListView->PopupMenu(point, pMenu);
	//int_x iRet = 0;
	switch(iRet)
	{
	case 1:
		m_wndDetail.SetProcessId((uint_32)iProcessId);
		m_wndDetail.Show(ShowModeNormal, HostInitPosCenterParent);
		m_wndDetail.Active(true);
		break;
	case 6:
		m_lstDetail.SetShowHeader(!m_lstDetail.IsShowHeader());
		break;
	case 7:
		m_lstDetail.SetShowFrameX(!m_lstDetail.IsShowFrameX());
		break;
	case 8:
		m_lstDetail.SetShowFrameY(!m_lstDetail.IsShowFrameY());
		break;
	case 9:
		m_lstDetail.RemoveAllRows();
		break;
	case 10:
		RefreshDetail();
		break;
	case 60:
		m_lstDetail.SetRowSpace(m_lstDetail.GetRowSpace() + 1);
		break;
	case 61:
		m_lstDetail.SetRowSpace(m_lstDetail.GetRowSpace() - 1);
		break;
	case 62:
		break;
	case 63:
		break;
	case 50: case 51: case 52: case 53: case 54: case 55:
		{
			int_x iCnt = iRet - 50;
			for(int cnt = 0; cnt != 6; ++cnt)
				eFlags[cnt] = (cnt == iCnt ? MenuFlagCheck : MenuFlagNone);
			//CMessageBox mb;
			//mb.SetParentHandle(m_hWnd);
			//mb.ShowMessage(L"确定", L"真的要改变优先级吗？");
		}
		//CProcess(uiProcId, PROCESS_SET_INFORMATION).SetPriority(ProcessPriorityHigh);
		break;
	case 100:
		EndRun(100);
		break;
	case 101:
		EndRun(101);
		break;
	case 102:
		EndRun(102);
		break;
	case 103:
		m_lstDetail.SetViewMode(ListViewModeDetails);
		break;
	case 104:
		m_lstDetail.SetViewMode(ListViewModeTile);
		break;
	case 105:
		m_lstDetail.SetViewMode(ListViewModeIcon, 16);
		break;
	case 106:
		m_lstDetail.SetViewMode(ListViewModeIcon, 48);
		break;
	case 107:
		m_lstDetail.SetViewMode(ListViewModeIcon, 128);
		break;
	}

	SafeRelease(pMenuPriority);
	SafeRelease(pMenuLineSpace);
	SafeRelease(pMenuLineSpace2);
	SafeRelease(pMenuDevice2D);
	SafeRelease(pMenuViewMode);
	delete pMenu;

	return 0;
}

int_x CMainWindow::OnDetailListViewSelectingRow(IControl * pListView, int_x iIndex)
{
	return 0;
	//return 1; // 就是不让你选。
}

int_x CMainWindow::OnDetailListViewSelectRow(IControl * pListView, int_x iIndex)
{
	m_btnShowDetail.SetEnable(iIndex >= 0);
	return 0;
}

int_x CMainWindow::DetailListViewSortCmpFunc(const ListViewRowT * pLeftItem, const ListViewRowT * pRightItem)
{
	ProcessAllInfo * pLeft = (ProcessAllInfo *)pLeftItem->UserData;
	ProcessAllInfo * pRight = (ProcessAllInfo *)pRightItem->UserData;
	bool bRet = false;
	switch(m_iSortIndex)
	{
	case 0:
		bRet = pLeft->iIndex > pRight->iIndex;
		break;
	case 1:
		bRet = textcmp(pLeft->prcInfo.Name.buffer, -1, pRight->prcInfo.Name.buffer, -1) > 0;
		break;
	case 2:
		bRet = pLeft->prcInfo.ID > pRight->prcInfo.ID;
		break;
	case 3:
		bRet = pLeft->prcInfo.ParentID > pRight->prcInfo.ParentID;
		break;
	case 4:
		bRet = pLeft->prcInfo.ThreadCount > pRight->prcInfo.ThreadCount;
		break;
	case 5:
		bRet = pLeft->iHandleCount > pRight->iHandleCount;
		break;
	case 6:
		bRet = pLeft->memInfo.Working > pRight->memInfo.Working;
		break;
	case 7:
		bRet = pLeft->handleInfo.Default > pRight->handleInfo.Default;
		break;
	case 8:
		bRet = pLeft->handleInfo.User > pRight->handleInfo.User;
		break;
	case 9:
		bRet = pLeft->ioInfo.ReadBytes > pRight->ioInfo.ReadBytes;
		break;
	case 10:
		bRet = pLeft->priority> pRight->priority;
		break;
	default:
		bRet = false;
		break;
	}
	return m_bSortAscend ? bRet : !bRet;
}

int_x CMainWindow::OnDetailShowAllProcess(IControl * pListView, bool bCheck, bool * pbChecked)
{
	if(bCheck)
	{
		if(CMessageBox::ShowMessage(this, L"应用程序将重新启动，以获取管理员权限。", L"重新启动", L"任务管理器",
			MessageBoxButtonOkCancel) == DialogResultOk)
		{
			EndRun(103);
		}
	}
	*pbChecked = false;
	return 0;
}

int_x CMainWindow::OnDetailShowDetail(IControl * pListView, pointix point)
{
	int_x iIndex = m_lstDetail.GetSelectedRow();
	if(iIndex >= 0)
	{
		ProcessAllInfo * pInfo = (ProcessAllInfo *)m_lstDetail.GetRowUserData(iIndex);
		m_wndDetail.SetOwner(this);
		m_wndDetail.SetProcessId(pInfo->prcInfo.ID);
		m_wndDetail.Show(ShowModeNormal, HostInitPosCenterParent);
		m_wndDetail.Active(true);
		//GetApp()->Run(m_wndDetail.GetFormId(), AppRunDialog);
	}
	return 0;
}

int_x CMainWindow::OnShowServices(int_x iMouseX, int_x iMouseY, IControl * pControl)
{
	system("services.msc");
	return 0;
}

void CMainWindow::OnClose()
{
	Hide();
	Win32::Quit(0);
}

void CMainWindow::OnLoaded()
{
	m_lstDetail.SetViewMode(ListViewModeTile);
	m_lstDetail.SetMainCol(1);
}

void CMainWindow::RefreshProgram()
{
	m_lstProgram.RemoveAllRows();
	EnumWindows(MainWindowEnumWindowsFunc, (LPARAM)&m_lstProgram);
}

void CMainWindow::RefreshDetail()
{
	m_lstDetail.RemoveAllRows();

	ListViewItemT item;

	textw text(L"TheText");

	CProcessSnapshot snap;
	ProcessAllInfo allInfo;
	ProcessInfoT prcInfo;

	int_x iIndex = 0;
	m_arrAllInfos.clear();
	if(snap.GetFirstProcessInfo(prcInfo))
	{
		CSHFileInfoBuffer * pShellBuffer = GetService<CSHFileInfoBuffer>(OID_CSHFileInfoBuffer);
		m_lstDetail.BegUpdate();
		do
		{
			ProcessAllInfo & allInfo = m_arrAllInfos.add();
			allInfo.prcInfo = prcInfo;
			allInfo.iIndex = iIndex++;

			CProcess process;
			if(process.Open(prcInfo.ID, PROCESS_QUERY_INFORMATION | PROCESS_VM_READ).ok())
			{
				process.GetMemoryUsage(allInfo.memInfo);
				process.GetGdiHandleInfo(allInfo.handleInfo);
				process.GetIoInfo(allInfo.ioInfo);
				process.GetPriority(allInfo.priority);
				process.GetHandleCount(allInfo.iHandleCount);
				process.GetFilePath(allInfo.path, MAX_PATH);
				textsplitpath<char_16>(allInfo.path, MAX_PATH,
					allInfo.drive, MAX_PATH, allInfo.folder, MAX_PATH, allInfo.file, MAX_PATH, allInfo.ext, MAX_PATH, nullptr, 0);
			}

			int_x iRow = m_lstDetail.AddRow();

			text.format(L"%03d", allInfo.iIndex);
			m_lstDetail.AddItem(iRow, text);

			IImage * pImage = nullptr;
			if(allInfo.path[0])
				pImage = pShellBuffer->GetShellIcon(allInfo.path);
			else
				pImage = pShellBuffer->GetShellIcon(L".exe");
			if(process.IsWow64())
			{
#ifdef _UNICODE
				text.format(L"%s *32", allInfo.prcInfo.Name.buffer);
#else
				text.format(L"%S *32", allInfo.prcInfo.Name.buffer);
#endif
				m_lstDetail.AddItem(iRow, text, pImage);
			}
			else
				m_lstDetail.AddItem(iRow, allInfo.prcInfo.Name, pImage);

			text.format(L"%d", allInfo.prcInfo.ID);
			m_lstDetail.AddItem(iRow, text);
			text.format(L"%d", allInfo.prcInfo.ParentID);
			m_lstDetail.AddItem(iRow, text);
			text.format(L"%d", allInfo.prcInfo.ThreadCount);
			m_lstDetail.AddItem(iRow, text);


			CFileVersion fileInfo(allInfo.path);
			if(fileInfo.IsValid())
			{
				fileInfo.QueryVerInfo(VerQueryCompanyName, allInfo.company, MAX_PATH);
				fileInfo.GetDescription(allInfo.desc, MAX_PATH);
			}
			else
			{
				textempty(allInfo.desc, MAX_PATH);
				textempty(allInfo.company, MAX_PATH);
			}

			text.format(L"%d", allInfo.iHandleCount);
			m_lstDetail.AddItem(iRow, text);

			if(allInfo.memInfo.Working > 1000000)
				text.format(L"%.1fMB", (float_32)allInfo.memInfo.Working / 1000000.0f);
			else if(allInfo.memInfo.Working > 1000)
				text.format(L"%.1fKB", (float_32)allInfo.memInfo.Working / 1000.0f);
			else
				text.format(L"%dB", allInfo.memInfo.Working);

			m_lstDetail.AddItem(iRow, text);

			text.format(L"%d", allInfo.handleInfo.Default);
			m_lstDetail.AddItem(iRow, text);
			text.format(L"%d", allInfo.handleInfo.User);
			m_lstDetail.AddItem(iRow, text);


			if(allInfo.ioInfo.ReadBytes > 1000000000)
				text.format(L"%.1fGB", (float_32)allInfo.ioInfo.ReadBytes / 1000000000.0f);
			else if(allInfo.ioInfo.ReadBytes > 1000000)
				text.format(L"%.1fMB", (float_32)allInfo.ioInfo.ReadBytes / 1000000.0f);
			else if(allInfo.ioInfo.ReadBytes > 1000)
				text.format(L"%.1fKB", (float_32)allInfo.ioInfo.ReadBytes / 1000.0f);
			else
				text.format(L"%dB", allInfo.ioInfo.ReadBytes);
			m_lstDetail.AddItem(iRow, text);

			switch(allInfo.priority)
			{
			case ProcessPriorityLow:
				text.set(L"低");
				break;
			case ProcessPriorityBelowNormal:
				text.set(L"低于标准");
				break;
			case ProcessPriorityNormal:
				text.set(L"标准");
				break;
			case ProcessPriorityAboveNormal:
				text.set(L"高于标准");
				break;
			case ProcessPriorityHigh:
				text.set(L"高");
				break;
			case ProcessPriorityRealTime:
				text.set(L"实时");
				break;
			case ProcessPriorityUnknown:
				text.set(L"未知");
				break;
			default:
				break;
			}
			m_lstDetail.AddItem(iRow, text);

			m_lstDetail.AddItem(iRow, allInfo.folder);
			m_lstDetail.AddItem(iRow, allInfo.desc);
			m_lstDetail.AddItem(iRow, allInfo.company);
		}
		while(snap.GetNextProcessInfo(prcInfo));

		m_lstDetail.EndUpdate();
		for(int_x cnt = 0; cnt != iIndex; ++cnt)
			m_lstDetail.SetRowUserData(cnt, (int_x)&m_arrAllInfos[cnt]);
	}
}

void CMainWindow::RefreshServices()
{
	m_lstService.RemoveAllRows();
	SC_HANDLE hScm = OpenSCManagerW(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);
	byte_t * pMngBuffer = nullptr;
	DWORD dwBytesNeeded = 0, dwReturned = 0;
	
	EnumServicesStatusEx(hScm, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, 
		NULL, 0, &dwBytesNeeded, &dwReturned, NULL, NULL);  
	pMngBuffer = new BYTE[dwBytesNeeded];
	buffset8(pMngBuffer, dwBytesNeeded, 0);
	EnumServicesStatusEx(hScm, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL, 
		pMngBuffer, dwBytesNeeded, &dwBytesNeeded, &dwReturned, NULL, NULL); 

	ENUM_SERVICE_STATUS_PROCESS * pSSP = (ENUM_SERVICE_STATUS_PROCESS *)pMngBuffer;
	textw text;

	//#define SERVICE_STOPPED                        0x00000001
	const char_16 STATUS[][16] = {L"", L"已停止", L"正在启动", L"正在停止", L"正在运行", L"正在继续", L"正在暂停", L"已暂停"};
	const char_16 START[][32] = {L"SERVICE_BOOT_START", L"SERVICE_SYSTEM_START", L"自动", L"手动", L"已禁用"};
	byte_t * pBuffer = nullptr;
	DWORD dwBufferSize = 0;
	for(DWORD dwCnt = 0; dwCnt != dwReturned; ++dwCnt, ++pSSP)
	{
		int_x iRow = m_lstService.AddRow();
		m_lstService.AddItem(iRow, pSSP->lpDisplayName);

		SC_HANDLE hService = OpenService(hScm, pSSP->lpServiceName, SERVICE_QUERY_CONFIG);
		// -----------
		QueryServiceConfig(hService, NULL, 0, &dwBytesNeeded);
		if(dwBytesNeeded > dwBufferSize)
		{
			delete pBuffer;
			pBuffer = new byte_t[dwBytesNeeded];
			dwBufferSize = dwBytesNeeded;
		}
		QueryServiceConfig(hService, (QUERY_SERVICE_CONFIG *)pBuffer, dwBufferSize, &dwBytesNeeded);

		QUERY_SERVICE_CONFIG * pConfig = (QUERY_SERVICE_CONFIG *)pBuffer;
		m_lstService.AddItem(iRow, STATUS[pSSP->ServiceStatusProcess.dwCurrentState]);
		m_lstService.AddItem(iRow, START[pConfig->dwStartType]);

		// -----------
		QueryServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, NULL, 0, &dwBytesNeeded);
		if(dwBytesNeeded > dwBufferSize)
		{
			delete pBuffer;
			pBuffer = new byte_t[dwBytesNeeded];
			dwBufferSize = dwBytesNeeded;
		}

		QueryServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, pBuffer, dwBufferSize, &dwBytesNeeded);
		SERVICE_DESCRIPTION * pDesc = (SERVICE_DESCRIPTION *)pBuffer;
		m_lstService.AddItem(iRow, pDesc->lpDescription);
		CloseServiceHandle(hService);
	}

	CloseServiceHandle(hScm);
	delete pBuffer;
	delete pMngBuffer;
}

void CMainWindow::RefreshAutoStart()
{
	CSHFileInfoBuffer * pShellBuffer = GetService<CSHFileInfoBuffer>(OID_CSHFileInfoBuffer);
	CShObj sobj;
	CRegKey reg;
	reg.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"));
	int_x iIndex = 0;
	char_x szName[512] = {0};
	char_x szValue[512] = {0};
	while(reg.DoValueIter(iIndex++, szName, 512))
	{
		reg.ReadValue(szName, szValue, 512);
		int_x iRow = m_lstAutoStart.AddRow();
		m_lstAutoStart.AddItem(iRow, szName, pShellBuffer->GetShellIcon(szValue));
		m_lstAutoStart.AddItem(iRow, L"已启用");
		m_lstAutoStart.AddItem(iRow, szValue);
	}

	reg.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"));
	iIndex = 0;
	while(reg.DoValueIter(iIndex++, szName, 512))
	{
		int_x iRow = m_lstAutoStart.AddRow();
		m_lstAutoStart.AddItem(iRow, szName);
		m_lstAutoStart.AddItem(iRow, L"已启用");
		reg.ReadValue(szName, szValue, 512);
		m_lstAutoStart.AddItem(iRow, szValue);
	}

	// 其他的启动项。
	char_x szBuffer[MAX_PATH]; 
	SHGetSpecialFolderPath(NULL, szBuffer, CSIDL_STARTUP, FALSE);
	folderinfo_t dirInfo(szBuffer);
	vector<fileinfo_t> arrFiles = dirInfo.get_files();
	CShellLink link;
	SHGetSpecialFolderPath(NULL, szBuffer, CSIDL_COMMON_STARTUP, FALSE);

	dirInfo.generate(szBuffer);
	dirInfo.get_files(arrFiles);

	for(int_x cnt = 0, len = arrFiles.size(); cnt != len; ++cnt)
	{
		const fileinfo_t & fileInfo = arrFiles[cnt];
		fileInfo.extention(szBuffer, MAX_PATH);
		if(fileInfo.exists && !fileInfo.system &&
			textcmp(szBuffer, -1, _T("lnk"), 3) == 0)
		{
			int_x iRow = m_lstAutoStart.AddRow();
			link.Load(fileInfo.path);

			fileInfo.filename(szBuffer, MAX_PATH);
			m_lstAutoStart.AddItem(iRow, szBuffer);
			link.GetLinkFile(szBuffer, MAX_PATH);
			m_lstAutoStart.AddItem(iRow, L"");
			m_lstAutoStart.AddItem(iRow, szBuffer);
		}
	}
}

static BOOL CALLBACK MainWindowEnumWindowsFunc(HWND hWnd, LPARAM lParam)
{
	if(GetParent(hWnd) == NULL && IsWindowVisible(hWnd))
	{
		char_16 szText[512];
		GetWindowTextW(hWnd, szText, 512);
		if(szText[0] && textcmp(szText, -1, L"Program Manager", -1))
		{
			CListView * pListView = (CListView *)lParam;
			int_x iRow = pListView->AddRow();
			pListView->AddItem(iRow, szText);
#if MSC_VER > MSC_2003
			if(IsHungAppWindow(hWnd))
				pListView->AddItem(iRow, L"未响应");
			else
#endif
				pListView->AddItem(iRow, L"正在运行");
			log0(L"hWnd = 0x%p, Text = %s.", hWnd, szText);
		}
	}
	return TRUE;
}