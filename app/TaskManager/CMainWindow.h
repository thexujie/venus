#pragma once

#include "BaseInc.h"
#include "CDetail.h"
#include "CFileVersion.h"

class ProcessAllInfo
{
public:
	int_x iIndex;

	ProcessInfoT prcInfo;
	ProcessMemInfoT memInfo;
	GdiHandleInfoT handleInfo;
	ProcessIoInfoT ioInfo;

	char_16 path[MAX_PATH];
	char_16 drive[MAX_PATH];
	char_16 folder[MAX_PATH];
	char_16 file[MAX_PATH];
	char_16 ext[MAX_PATH];
	char_16 desc[MAX_PATH];
	char_16 company[MAX_PATH];
	ProcessPriorityE  priority;

	int_x iHandleCount;
};

class CMainWindow : public CForm
{
public:
	CMainWindow();

	void OnClose();
	void OnLoaded();

public:
	void GetMeta(IMetaHelper * pHelper);
	int_x OnTabSelectChanged(IControl * pTabControl, int_x iIndex);
	// 应用程序。

	// 详细信息。
	int_x OnDetailListViewHeaderPressed(IControl * pListView, pointix point);
	int_x OnDetailListViewMouseDownR(IControl * pListView, pointix point);
	int_x OnDetailListViewSelectingRow(IControl * pListView, int_x iIndex);
	int_x OnDetailListViewSelectRow(IControl * pListView, int_x iIndex);
	int_x DetailListViewSortCmpFunc(const ListViewRowT * pLeftItem, const ListViewRowT * pRightItem);
	int_x OnDetailShowAllProcess(IControl * pListView, bool bCheck, bool * pbChecked);
	int_x OnDetailShowDetail(IControl * pListView, pointix point);
	// 启动。

	// 服务。
	int_x OnShowServices(int_x iMouseX, int_x iMouseY, IControl * pControl);

	void RefreshProgram();
	void RefreshDetail();
	void RefreshServices();
	void RefreshAutoStart();

private:
	// 应用程序。
	CListView m_lstProgram;

	// 详细信息页。
	CListView m_lstDetail;
	CCheckBox m_chbShowAllUser;
	CButton m_btnShowDetail;
	// 启动。
	CListView m_lstAutoStart;
	// 服务。
	CListView m_lstService;

	CDetailWindow m_wndDetail;
	vector<ProcessAllInfo> m_arrAllInfos;
	int_x m_iSortIndex;
	bool m_bSortAscend;
	bool m_arrSortAscend[20];
};
