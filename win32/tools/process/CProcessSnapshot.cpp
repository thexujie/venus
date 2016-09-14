#include "stdafx.h"
#include "CProcessSnapshot.h"
#include "CProcess.h"

#include <TlHelp32.h>

VENUS_BEG

CProcessSnapshot::CProcessSnapshot()
{
	m_hSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
}

CProcessSnapshot::~CProcessSnapshot()
{
	if(m_hSnap != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(m_hSnap);
		m_hSnap = INVALID_HANDLE_VALUE;
	}
}

bool CProcessSnapshot::GetFirstProcessInfo(ProcessInfoT & info)
{
	if(m_hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe32 = {sizeof(PROCESSENTRY32)};
		if(::Process32First(m_hSnap,&pe32))
		{
			info.ID = pe32.th32ProcessID;
			info.ParentID = pe32.th32ParentProcessID;
			info.Name = pe32.szExeFile;
			info.ThreadCount = pe32.cntThreads;
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

bool CProcessSnapshot::GetNextProcessInfo(ProcessInfoT & info)
{
	if(m_hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe32 = {sizeof(PROCESSENTRY32)};
		if(::Process32Next(m_hSnap,&pe32))
		{
			info.ID = pe32.th32ProcessID;
			info.ParentID = pe32.th32ParentProcessID;
			info.Name = pe32.szExeFile;
			info.ThreadCount = pe32.cntThreads;
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

VENUS_END
