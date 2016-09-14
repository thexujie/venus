#pragma once

VENUS_BEG

struct ProcessInfoT
{
	uint_32 ID;
	uint_32 ParentID;
	FilePathX Name;
	uint_32 ThreadCount;
};

class WIN32_API CProcessSnapshot
{
public:
	CProcessSnapshot();
	~CProcessSnapshot();

	bool GetFirstProcessInfo(ProcessInfoT & info);
	bool GetNextProcessInfo(ProcessInfoT & info);
private:
	HANDLE m_hSnap;
};

VENUS_END
