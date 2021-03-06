#pragma once

VENUS_BEG

class WIN32_API CMonitor : public IMonitor
{
public:
	CMonitor(HMONITOR hMonitor);
	int_x GetWidth() const;
	int_x GetHeight() const;
	sizeix GetSize() const;
	rectix GetRect() const;
	rectix GetWorkRect() const;

	HMONITOR GetHandle() const;
private:
	HMONITOR m_hMonitor;
};

class WIN32_API CMonitorManager : public ObjectT<IMonitorManager>
{
public:
	CMonitorManager();
	~CMonitorManager();

	IMonitor * GetMonitorFromWindowId(int_x iWindowId) const;
	IMonitor * GetMonitor(int_x iIndex) const;
	IMonitor * GetMonitor(int_x iX, int_x iY) const;

	void Update();
	void Add(HMONITOR hMonitor);
private:
	vector<CMonitor> m_monitors;
};

VENUS_END
