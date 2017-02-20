#include "stdafx.h"
#include "CMonitor.h"

VENUS_BEG

static BOOL CALLBACK MonitorManagerEnumFunc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);

CMonitor::CMonitor(HMONITOR hMonitor) :
m_hMonitor(hMonitor)
{
	MONITORINFOEX miex;
	buffclr(miex);
	miex.cbSize = sizeof(miex);
	::GetMonitorInfoW(hMonitor, &miex);

	DISPLAY_DEVICE dd = {0};
	dd.cb = sizeof(dd);
	EnumDisplayDevices(miex.szDevice, 0, &dd, 0);
}

int_x CMonitor::GetWidth() const
{
	MONITORINFO mi = {sizeof(MONITORINFO)};
	::GetMonitorInfoW(m_hMonitor, &mi);
	return mi.rcMonitor.right - mi.rcMonitor.left;
}

int_x CMonitor::GetHeight() const
{
	MONITORINFO mi = {sizeof(MONITORINFO)};
	::GetMonitorInfoW(m_hMonitor, &mi);
	return mi.rcMonitor.bottom - mi.rcMonitor.top;
}

sizeix CMonitor::GetSize() const
{
	MONITORINFO mi = {sizeof(MONITORINFO)};
	::GetMonitorInfoW(m_hMonitor, &mi);
	return sizeix(mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top);
}

rectix CMonitor::GetRect() const
{
	MONITORINFO mi = {sizeof(MONITORINFO)};
	::GetMonitorInfoW(m_hMonitor, &mi);
	return rectix(mi.rcMonitor.left,
					 mi.rcMonitor.top,
					 mi.rcMonitor.right - mi.rcMonitor.left,
					 mi.rcMonitor.bottom - mi.rcMonitor.top);
}

rectix CMonitor::GetWorkRect() const
{
	MONITORINFO mi = {sizeof(MONITORINFO)};
	::GetMonitorInfoW(m_hMonitor, &mi);
	return rectix(mi.rcWork.left,
		mi.rcWork.top,
		mi.rcWork.right - mi.rcWork.left,
		mi.rcWork.bottom - mi.rcWork.top);
}

HMONITOR CMonitor::GetHandle() const
{
	return m_hMonitor;
}


CMonitorManager::CMonitorManager()
{
	Update();
}

CMonitorManager::~CMonitorManager()
{

}

IMonitor * CMonitorManager::GetMonitorFromWindowId(int_x iWindowId) const
{
	if(m_monitors.empty())
		return nullptr;

	if(!iWindowId)
		return (IMonitor *)(&m_monitors[0]);

	HMONITOR hTheMonitor = ::MonitorFromWindow((HWND)iWindowId, MONITOR_DEFAULTTOPRIMARY);
	for(int_x cnt = 0, len = m_monitors.size(); cnt < len; ++cnt)
	{
		if(hTheMonitor == m_monitors[cnt].GetHandle())
			return (IMonitor *)(&m_monitors[cnt]);

	}
	return nullptr;
}

IMonitor * CMonitorManager::GetMonitor(int_x iIndex) const
{
	if(m_monitors.empty())
		return nullptr;

	if(iIndex < 0 || iIndex >= m_monitors.size())
		iIndex = 0;
	return (IMonitor *)(&m_monitors[iIndex]);
}

IMonitor * CMonitorManager::GetMonitor(int_x iX, int_x iY) const
{
	if(m_monitors.empty())
		return nullptr;

	POINT point = {(int)iX, (int)iY};
	HMONITOR hTheMonitor = ::MonitorFromPoint(point, MONITOR_DEFAULTTOPRIMARY);
	for(int_x cnt = 0, len = m_monitors.size(); cnt < len; ++cnt)
	{
		if(hTheMonitor == m_monitors[cnt].GetHandle())
			return (IMonitor *)(&m_monitors[cnt]);

	}
	return nullptr;
}

void CMonitorManager::Update()
{
	m_monitors.clear();
	EnumDisplayMonitors(NULL, NULL, MonitorManagerEnumFunc, (LPARAM)this);
}

void CMonitorManager::Add(HMONITOR hMonitor)
{
	m_monitors.add(CMonitor(hMonitor));
}

static BOOL CALLBACK MonitorManagerEnumFunc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	((CMonitorManager *)dwData)->Add(hMonitor);
	return TRUE;
}

VENUS_END
