#include "stdafx.h"
#include "CProcess.h"
#include "PEB.h"

#include <Psapi.h>
#if PSAPI_VERSION == 1 || _WIN32_WINNT < WINVER_WIN7
#pragma comment(lib, "psapi.lib")
#endif

VENUS_BEG

enum NtQueryInfoType
{
	NtQueryInfoTypeBasic,
	NtQueryInfoTypeBasicWow64,
};

CProcess::CProcess() : m_uiProcessId(0), m_hProcess(NULL)
{

}

CProcess::CProcess(uint_32 uiProcessId, uint_32 uiAccess) : m_uiProcessId(0), m_hProcess(NULL)
{
	Open(uiProcessId, uiAccess);
}

CProcess::~CProcess()
{
	Close();
}

winerr_t CProcess::Open(uint_32 uiProcessId, uint_32 uiAccess)
{
	Close();

	m_hProcess = OpenProcess(uiAccess, false, uiProcessId);
	if(m_hProcess)
	{
		m_uiProcessId = uiProcessId;
		return err_ok;
	}
	else
	{
		//log0(L"Info:Can not open the process.(PID=%d, Access=0x%X)，%s", uiProcessId, uiAccess, FormatLastErrorW());
		m_uiProcessId = 0;
		return GetLastError();
	}
}

winerr_t CProcess::ReOpen(uint_32 uiAccess)
{
	uint_32 uiProcessId = m_uiProcessId;
	Close();
	return Open(uiProcessId, uiAccess);
}


void CProcess::Close()
{
	if(m_hProcess)
		::CloseHandle(m_hProcess);

	m_uiProcessId = 0;
	m_hProcess = NULL;
}

bool CProcess::Is64() const
{
#ifdef BIT32
	return false;
#else
	if(IsWow64())
		return false;
	else
		return true;
#endif
}

bool CProcess::IsWow64() const
{
	if(!m_hProcess)
		return false;
	else
	{
		BOOL bWow64 = FALSE;
		::IsWow64Process(m_hProcess, &bWow64);
		return bWow64 == TRUE;
	}
}

bool CProcess::GetCmdLine(char_16 * szText, int_x iSize) const
{
	textempty(szText, iSize);
	if(!m_hProcess || iSize <= 0 || !szText)
		return false;

	CModule mdNtdll(_T("ntdll"));
	Win32::DebugSetLastError(0);

#ifdef BIT32
	BOOL bWow64 = FALSE;
	if(!IsWow64Process(GetCurrentProcess(), &bWow64))
		return false;
	if(bWow64)
	{
		typedef HRESULT(WINAPI * Wow64QueryInformationProcess64FunT)(HANDLE, uint_32, void *, uint_32, uint_32 *);
		typedef HRESULT(WINAPI * Wow64ReadVirtualMemory64FunT)(HANDLE, uint_64, void *, uint_64, uint_64 *);

		CModule mdNtdll(_T("ntdll"));
		auto pfnWow64QueryInformationProcess64 = mdNtdll.GetProc<Wow64QueryInformationProcess64FunT>("NtWow64QueryInformationProcess64");
		auto pfnWow64ReadVirtualMemory64 = mdNtdll.GetProc<Wow64ReadVirtualMemory64FunT>("NtWow64ReadVirtualMemory64");
		if(!pfnWow64QueryInformationProcess64 || !pfnWow64ReadVirtualMemory64)
			return false;

		PROCESS_BASIC_INFORMATION_64 pbi = {0};
		PEB_64 peb = {0};
		PROCESS_PARAMETERS_64 param = {0};

		uint_64 siRead = 0;
		HRESULT hr = S_OK;
		hr = pfnWow64QueryInformationProcess64(m_hProcess, NtQueryInfoTypeBasic, &pbi, sizeof(PROCESS_BASIC_INFORMATION_64), NULL);
		if(hr != S_OK)
		{
			Win32::DebugFormatLastError();
			return false;
		}

		hr = pfnWow64ReadVirtualMemory64(m_hProcess, pbi.PebBaseAddress, &peb, sizeof(PEB_64), &siRead);
		if(hr != S_OK)
		{
			Win32::DebugFormatLastError();
			return false;
		}

		hr = pfnWow64ReadVirtualMemory64(m_hProcess, peb.ProcessParameters, &param, sizeof(PROCESS_PARAMETERS_64), &siRead);
		if(hr != S_OK)
		{
			Win32::DebugFormatLastError();
			return false;
		}

		uint_64 uiLength = param.CommandLine.Length;
		uint_64 uiBuffer = param.CommandLine.Buffer;

		if(uiLength > (uint_64)iSize)
			uiLength = (uint_64)iSize - 1;

		if(uiLength <= 0)
			return false;

		hr = pfnWow64ReadVirtualMemory64(m_hProcess, uiBuffer, szText, uiLength * 2, &siRead);
		if(hr != S_OK)
		{
			Win32::DebugFormatLastError();
			return false;
		}

		szText[uiLength] = 0;
		return true;
	}
	else
#endif // BIT32
	{
		typedef HRESULT(WINAPI * QueryInformationProcessFunT)(HANDLE, uint_32, void *, uint_32, uint_32 *);
		typedef HRESULT(WINAPI * ReadVirtualMemoryFunT)(HANDLE, uint_x, void *, uint_x, uint_x *);

		CModule mdNtdll(_T("ntdll"));
		proc_ptr<HRESULT (HANDLE, uint_32, void *, uint_32, uint_32 *)> pfnQueryInformationProcess;
		proc_ptr<HRESULT (HANDLE, uint_x, void *, uint_x, uint_x *)> pfnReadVirtualMemory;
		mdNtdll.GetProc(pfnQueryInformationProcess, "NtQueryInformationProcess");
		mdNtdll.GetProc(pfnReadVirtualMemory, "NtReadVirtualMemory");

		if(!pfnQueryInformationProcess || !pfnReadVirtualMemory)
		{
			Win32::DebugFormatLastError();
			return false;
		}
		PROCESS_BASIC_INFORMATION_X pbi = {0};
		PEB_X peb = {0};
		PROCESS_PARAMETERS_X param = {0};

		HRESULT hr = S_OK;
		uint_x siRead = 0;

		hr = pfnQueryInformationProcess(m_hProcess, NtQueryInfoTypeBasic, &pbi, sizeof(PROCESS_BASIC_INFORMATION_X), NULL);
		if(hr != S_OK)
			return false;

		hr = pfnReadVirtualMemory(m_hProcess, pbi.PebBaseAddress, &peb, sizeof(PEB_X), &siRead);
		if(hr != S_OK)
		{
			Win32::DebugFormatLastError();
			return false;
		}

		hr = pfnReadVirtualMemory(m_hProcess, peb.ProcessParameters, &param, sizeof(PROCESS_PARAMETERS_X), &siRead);
		if(hr != S_OK)
		{
			Win32::DebugFormatLastError();
			return false;
		}

		uint_x uiLength = param.ImagePathName.Length;
		uint_x uiBuffer = param.ImagePathName.Buffer;

		if(uiLength > (uint_x)iSize)
			uiLength = (uint_x)iSize - 1;

		if(uiLength <= 0)
			return false;

		hr = pfnReadVirtualMemory(m_hProcess, uiBuffer, szText, uiLength * 2, &siRead);
		if(hr != S_OK)
		{
			Win32::DebugFormatLastError();
			return false;
		}

		szText[uiLength] = 0;
		return true;
	}
}

bool CProcess::GetFilePath(char_16 * szText, int_x iSize) const
{
	textempty(szText, iSize);
	if(!m_hProcess || iSize <= 0 || !szText)
		return false;

#ifndef _FORCE_READ_FROM_PEB_
	if(GetModuleFileNameExW(m_hProcess, NULL, szText, (DWORD)iSize))
		return true;
	else
	{
		TextSetEmpty(szText, iSize);
		return false;
	}
#else // _FORCE_READ_FROM_PEB_

	CModule mdNtdll(_T("ntdll"));
	Win32::DebugSetLastError(0);
#ifdef BIT32
	BOOL bWow64 = FALSE;
	if(!IsWow64Process(GetCurrentProcess(), &bWow64))
		return false;

	if(bWow64)
	{
		typedef HRESULT (WINAPI * Wow64QueryInformationProcess64FunT)(HANDLE, uint_32, void *, uint_32, uint_32 *);
		typedef HRESULT (WINAPI * Wow64ReadVirtualMemory64FunT)(HANDLE, uint_64, void *, uint_64, uint_64 *);

		// 找到函数先。
		auto pfnWow64QueryInformationProcess64 = mdNtdll.GetProc<Wow64QueryInformationProcess64FunT>("NtWow64QueryInformationProcess64");
		auto pfnWow64ReadVirtualMemory64 = mdNtdll.GetProc<Wow64ReadVirtualMemory64FunT>("NtWow64ReadVirtualMemory64");
		if(!pfnWow64QueryInformationProcess64 || !pfnWow64ReadVirtualMemory64)
			return false;

		PROCESS_BASIC_INFORMATION_64 pbi = {0};
		PEB_64 peb = {0};
		PROCESS_PARAMETERS_64 param = {0};

		uint_64 siRead = 0;
		HRESULT hr = S_OK;
		hr = pfnWow64QueryInformationProcess64(m_hProcess, NtQueryInfoTypeBasic, &pbi, sizeof(PROCESS_BASIC_INFORMATION_64), NULL);
		if(hr != S_OK)
		{
			Win32::DebugFormatLastError();
			return false;
		}

		hr = pfnWow64ReadVirtualMemory64(m_hProcess, pbi.PebBaseAddress, &peb, sizeof(PEB_64), &siRead);
		if(hr != S_OK)
		{
			Win32::DebugFormatLastError();
			return false;
		}

		hr = pfnWow64ReadVirtualMemory64(m_hProcess, peb.ProcessParameters, &param, sizeof(PROCESS_PARAMETERS_64), &siRead);
		if(hr != S_OK)
		{
			Win32::DebugFormatLastError();
			return false;
		}

		uint_64 uiLength = param.ImagePathName.Length;
		uint_64 uiBuffer = param.ImagePathName.Buffer;

		if(uiLength > (uint_64)iSize)
			uiLength = (uint_64)iSize - 1;

		if(uiLength <= 0)
			return false;

		hr = pfnWow64ReadVirtualMemory64(m_hProcess, uiBuffer, szText, uiLength * 2, &siRead);
		if(hr != S_OK)
		{
			Win32::DebugFormatLastError();
			return false;
		}

		szText[uiLength] = 0;
		return true;
	}
	else
#endif // BIT32
	{
		proc_ptr<HRESULT(HANDLE, uint_32, void *, uint_32, uint_32 *)> pfnQueryInformationProcess;
		proc_ptr<HRESULT(HANDLE, uint_x, void *, uint_x, uint_x *)> pfnReadVirtualMemory;
		mdNtdll.GetProc(pfnQueryInformationProcess, "NtQueryInformationProcess");
		mdNtdll.GetProc(pfnReadVirtualMemory, "NtReadVirtualMemory");

		if(!pfnQueryInformationProcess || !pfnReadVirtualMemory)
		{
			Win32::DebugFormatLastError();
			return false;
		}
		PROCESS_BASIC_INFORMATION_X pbi = {0};
		PEB_X peb = {0};
		PROCESS_PARAMETERS_X param = {0};

		HRESULT hr = S_OK;
		uint_x siRead = 0;

		hr = pfnQueryInformationProcess(m_hProcess, NtQueryInfoTypeBasic, &pbi, sizeof(PROCESS_BASIC_INFORMATION_X), NULL);
		if(hr != S_OK)
			return false;

		hr = pfnReadVirtualMemory(m_hProcess, pbi.PebBaseAddress, &peb, sizeof(PEB_X), &siRead);
		if(hr != S_OK)
		{
			Win32::DebugFormatLastError();
			return false;
		}

		hr = pfnReadVirtualMemory(m_hProcess, peb.ProcessParameters, &param, sizeof(PROCESS_PARAMETERS_X), &siRead);
		if(hr != S_OK)
		{
			Win32::DebugFormatLastError();
			return false;
		}

		uint_x uiLength = param.ImagePathName.Length;
		uint_x uiBuffer = param.ImagePathName.Buffer;

		if(uiLength > (uint_x)iSize)
			uiLength = (uint_x)iSize - 1;

		if(uiLength <= 0)
			return false;

		hr = pfnReadVirtualMemory(m_hProcess, uiBuffer, szText, uiLength * 2, &siRead);
		if(hr != S_OK)
		{
			Win32::DebugFormatLastError();
			return false;
		}

		szText[uiLength] = 0;
		return false;
	}

	return false;
#endif // _FORCE_READ_FROM_PEB_
}

bool CProcess::GetFileDetails(char_16 * szText, int_x iSize) const
{
	if(!m_hProcess)
		return false;
	textempty(szText, iSize);
	return false;
}

bool CProcess::GetPriority(ProcessPriorityE & priority) const
{
	if(m_hProcess)
	{
		priority = (ProcessPriorityE)GetPriorityClass(m_hProcess);
		return true;
	}
	else
	{
		priority = ProcessPriorityUnknown;
		return false;
	}
}

bool CProcess::GetHandleCount(int_x & iHandleCount) const
{
	DWORD dwCount = 0;
	if(m_hProcess && GetProcessHandleCount(m_hProcess, &dwCount))
	{
		iHandleCount = (int_x)dwCount;
		return true;
	}
	else
	{
		iHandleCount = 0;
		return false;
	}
}

bool CProcess::GetGdiHandleInfo(GdiHandleInfoT & info) const
{
	if(!m_hProcess)
	{
		info.Default = info.User = 0;
		info.PeakDefault = info.PeakUser = 0;
		return false;
	}
	else
	{
		info.Default = ::GetGuiResources(m_hProcess, GR_GDIOBJECTS);
		info.User = ::GetGuiResources(m_hProcess, GR_USEROBJECTS);
#if(WINVER >= 0x0601)
		info.PeakDefault = ::GetGuiResources(m_hProcess, GR_GDIOBJECTS_PEAK);
		info.PeakUser = ::GetGuiResources(m_hProcess, GR_USEROBJECTS_PEAK);
#else
		info.PeakDefault = info.PeakUser = 0;
#endif
		return true;
	}
}

bool CProcess::GetMemoryUsage(ProcessMemInfoT & info) const
{
	PROCESS_MEMORY_COUNTERS pmc = {sizeof(PROCESS_MEMORY_COUNTERS)};
	if(m_hProcess && GetProcessMemoryInfo(m_hProcess, &pmc, sizeof(pmc)))
	{
		info.PageFault = (int_x)pmc.PageFaultCount;

		info.Working = pmc.WorkingSetSize;
		info.PeakWorking = pmc.PeakWorkingSetSize;

		info.Page = pmc.PagefileUsage;
		info.PeakPage = pmc.PeakPagefileUsage;

		info.QuotaPage = pmc.QuotaPagedPoolUsage;
		info.QuotaPeakPage = pmc.QuotaPeakPagedPoolUsage;

		info.QuotaNonePage = pmc.QuotaNonPagedPoolUsage;
		info.QuotaPeakNonePage = pmc.QuotaPeakNonPagedPoolUsage;
		return true;
	}
	else
	{
		info.PageFault = 0;
		info.Working = info.PeakWorking = 0;
		info.Page = info.PeakPage = 0;
		info.QuotaPage = info.QuotaPeakPage = 0;
		info.QuotaNonePage = info.QuotaPeakNonePage = 0;
		return false;
	}
}

bool CProcess::GetIoInfo(ProcessIoInfoT & info) const
{
	IO_COUNTERS ioc = {0};
	if(m_hProcess && GetProcessIoCounters(m_hProcess, &ioc))
	{
		info.ReadCount = ioc.ReadOperationCount;
		info.ReadBytes = ioc.ReadTransferCount;
		info.WriteCount = ioc.WriteOperationCount;
		info.WriteBytes = ioc.WriteTransferCount;
		info.OtherCount = ioc.OtherOperationCount;
		info.OtherBytes = ioc.OtherTransferCount;
		return true;
	}
	else
	{
		info.ReadCount = info.ReadBytes = 0;
		info.WriteCount = info.WriteBytes = 0;
		info.OtherCount = info.OtherBytes = 0;
		return false;
	}
}

bool CProcess::SetPriority(ProcessPriorityE priority)
{
	return ::SetPriorityClass(m_hProcess, priority) == TRUE;
}

bool CProcess::IsAdminUser()
{
	if(!m_hProcess)
		return false;

	CHandle hToken;
	if(::OpenProcessToken(m_hProcess, TOKEN_QUERY, &hToken) == FALSE)
		return false;

	DWORD dwReturned = 0;
	GetTokenInformation(hToken, TokenGroups, NULL, 0, &dwReturned);

	byte_t * pvGroups = new byte_t[dwReturned];
	GetTokenInformation(hToken, TokenGroups, pvGroups, dwReturned, &dwReturned);
	TOKEN_GROUPS * pTokenGroups = (TOKEN_GROUPS *)pvGroups;

	byte_t adminSID[SECURITY_MAX_SID_SIZE] = {0};
	DWORD dwSize = sizeof(adminSID);   
	CreateWellKnownSid(/*WinNtAuthoritySid*/WinBuiltinAdministratorsSid, NULL, &adminSID, &dwSize);

	SID_NAME_USE snu;

	TCHAR tszName[256], tszDomain[256];
	DWORD cbName = 256, cbDomain = 256;
	bool bIsAdminUser = false;
	for(int_x cnt = 0, len = (int_x)pTokenGroups->GroupCount; cnt < len; ++cnt)
	{
		cbName = 256, cbDomain = 256;
		LookupAccountSid(NULL/* Local system */, 
			pTokenGroups->Groups[cnt].Sid, tszName, &cbName, tszDomain, &cbDomain, &snu);

		log0(L"Administrators: Group = %s, Domain = %s.", tszName, tszDomain);
		if(EqualSid(adminSID, pTokenGroups->Groups[cnt].Sid))
		{
			bIsAdminUser = true;
			break;
		}
	}

	delete pvGroups;
	return bIsAdminUser;
}

bool CProcess::IsAdmin()
{
	if(!m_hProcess)
		return false;

	CHandle hToken;
	if(::OpenProcessToken(m_hProcess, TOKEN_QUERY, &hToken) == FALSE)
		return false;

	DWORD dwSize = 0; 
	DWORD dwIsAdmin = 0;
	if(GetTokenInformation(hToken, TokenElevation,  &dwIsAdmin, sizeof(DWORD), &dwSize))
		return dwIsAdmin != 0;
	else
		return false;
}

bool CProcess::IsElevation()
{
#if MSVC > MSVC_2003
	if(!m_hProcess)
		return false;

	CHandle hToken;
	if(::OpenProcessToken(m_hProcess, TOKEN_QUERY, &hToken) == FALSE)
		return false;

	TOKEN_ELEVATION_TYPE eTekenType = TokenElevationTypeDefault;
	DWORD dwSize = 0; 

	if(GetTokenInformation(hToken, TokenElevationType,  &eTekenType, sizeof(TOKEN_ELEVATION_TYPE), &dwSize))
		return  eTekenType == TokenElevationTypeLimited;
	else
		return false;
#else
	return false;
#endif

	//if(GetTokenInformation(hToken, TokenElevationType,  &eTekenType, sizeof(TOKEN_ELEVATION_TYPE), &dwSize))
	//{
	//	BYTE adminSID[SECURITY_MAX_SID_SIZE] = {0};
	//	dwSize = sizeof(adminSID);   
	//	CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, &dwSize);   
	//	// 受限的权限运行（筛选过的）
	//	if(eTekenType == TokenElevationTypeLimited) 
	//	{   
	//		CHandle hUnfilteredToken;   
	//		if(GetTokenInformation(hToken, TokenLinkedToken, (void *)&hUnfilteredToken, sizeof(HANDLE), &dwSize))
	//		{
	//			DWORD dwIsAdmin = 0;
	//			if(GetTokenInformation(hToken, TokenElevation,  &dwIsAdmin, sizeof(DWORD), &dwSize))
	//				bIsAdmin =  dwIsAdmin != 0;
	//		}
	//	}
	//	else
	//	{
	//		DWORD dwIsAdmin = 0;
	//		if(GetTokenInformation(hToken, TokenElevation,  &dwIsAdmin, sizeof(DWORD), &dwSize))
	//			bIsAdmin =  dwIsAdmin != 0;
	//		//dwSize = sizeof(adminSID);   
	//		//CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, &dwSize);   

	//		//BOOL bIsAdmin = FALSE;
	//		//if(CheckTokenMembership(hToken, &adminSID, &bIsAdmin))
	//		//	return bIsAdmin != FALSE;
	//		//else
	//		//	return false;
	//	}  
	//}
	//else
	//	bIsAdmin = false;
	//return bIsAdmin;
}

bool CProcess::SetPrivilige(const char_x * szPrivilige, bool bSet)
{
	CHandle hToken;
	if(::OpenProcessToken(m_hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken) == FALSE)
	{
		log0(L"Error:无法打开进程访问令牌。\n");
		return false;
	}

	TOKEN_PRIVILEGES structTkp;
	if(::LookupPrivilegeValue(NULL, szPrivilige, &structTkp.Privileges[0].Luid) == FALSE)
		return false;
	structTkp.PrivilegeCount = 1;
	structTkp.Privileges[0].Attributes = bSet ? SE_PRIVILEGE_ENABLED : SE_PRIVILEGE_REMOVED;
	return ::AdjustTokenPrivileges(hToken, FALSE, &structTkp, sizeof(structTkp), NULL, NULL) != FALSE;
}

VENUS_END
