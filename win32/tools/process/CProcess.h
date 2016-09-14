#pragma once

VENUS_BEG

// ǿ�ƴӽ��̵� PEB �ж�ȡ������Ϣ�����Խ�� WOW32 �� ���� API �޷���ȡ������Ϣ�����⡣
#define _FORCE_READ_FROM_PEB_

enum ProcessPriorityE
{
	ProcessPriorityUnknown = 0,
	ProcessPriorityLow = IDLE_PRIORITY_CLASS,
	ProcessPriorityBelowNormal = BELOW_NORMAL_PRIORITY_CLASS,
	ProcessPriorityNormal = NORMAL_PRIORITY_CLASS,
	ProcessPriorityAboveNormal = ABOVE_NORMAL_PRIORITY_CLASS,
	ProcessPriorityHigh = HIGH_PRIORITY_CLASS,
	ProcessPriorityRealTime = REALTIME_PRIORITY_CLASS,
};

struct ProcessMemInfoT
{
	int_x PageFault; // ȱҳ�жϴ�����

	int_x Working; // ��ǰ�ڴ�ʹ�á�
	int_x PeakWorking; // �ڴ�ʹ�ø߷塣

	int_x Page; // ҳ���ļ���
	int_x PeakPage; // ҳ���ļ���

	int_x QuotaPage; // ��ǰҳ�滺���ʹ�á�
	int_x QuotaPeakPage; // ҳ�滺���ʹ�ø߷塣

	int_x QuotaNonePage; // ��ǰ��ҳ�滺���ʹ�á�
	int_x QuotaPeakNonePage; // ��ҳ�滺���ʹ�ø߷塣
};

struct GdiHandleInfoT
{
	uint_32 Default;
	uint_32 User;
	uint_32 PeakDefault;
	uint_32 PeakUser;
};

struct ProcessIoInfoT
{
	int_64 ReadCount;
	int_64 ReadBytes;
	int_64 WriteCount;
	int_64 WriteBytes;
	int_64 OtherCount;
	int_64 OtherBytes;
};

class WIN32_API CProcess
{
public:
	CProcess();
	CProcess(uint_32 uiProcessId, uint_32 uiAccess);
	~CProcess();

	winerr_t Open(uint_32 uiProcessId, uint_32 uiAccess);
	winerr_t ReOpen(uint_32 uiAccess);
	void Close();

	bool Is64() const;
	bool IsWow64() const;

	bool GetCmdLine(char_16 * szText, int_x iSize) const;
	bool GetFilePath(char_16 * szText, int_x iSize) const;
	bool GetFileDetails(char_16 * szText, int_x iSize) const;
	bool GetPriority(ProcessPriorityE & priority) const;
	bool GetHandleCount(int_x & iHandleCount) const;
	bool GetGdiHandleInfo(GdiHandleInfoT & info) const;
	bool GetMemoryUsage(ProcessMemInfoT & info) const;
	bool GetIoInfo(ProcessIoInfoT & info) const;

	bool SetPriority(ProcessPriorityE priority);

	// ---------------- Ȩ��
	bool IsAdminUser();
	bool IsAdmin();
	bool IsElevation();
	bool SetPrivilige(const char_x * szPrivilige, bool bSet);

protected:
	uint_32 m_uiProcessId;
	HANDLE m_hProcess;
};

VENUS_END
