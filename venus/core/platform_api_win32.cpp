#include "stdafx.h"
#include "platform_api.h"

#include <windows.h>

VENUS_BEG

int_x tls_create()
{
	DWORD dwTls = TlsAlloc();
	return (int_x)dwTls;
}

void tls_destroy(int_x iTls)
{
	TlsFree((DWORD)iTls);
}

void tls_set(int_x iTls, int_x iValue)
{
	TlsSetValue((DWORD)iTls, (void *)iValue);
}

int_x tls_get(int_x iTls)
{
	return (int_x)TlsGetValue((DWORD)iTls);
}


void thread_set_name(int_x iThreadId, const char_8 * szName)
{
#ifdef _DEBUG
#pragma pack(push, 4)
	struct THREADNAME_INFO
	{
		DWORD dwType;        // must be 0x1000
		LPCSTR szName;       // pointer to name (in same addr space)
		DWORD dwThreadID;    // thread ID (-1 caller thread)
		DWORD dwFlags;       // reserved for future use, most be zero
	};
#pragma pack(pop)

	const DWORD MS_VC_EXCEPTION_SET_THREAD_NAME = 0x406d1388;
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = szName;
	info.dwThreadID = (DWORD)iThreadId;
	info.dwFlags = 0;
	__try
	{
		RaiseException(MS_VC_EXCEPTION_SET_THREAD_NAME, 0, sizeof(THREADNAME_INFO) / sizeof(DWORD), (ULONG_PTR *)&info);
	}
	__except(EXCEPTION_CONTINUE_EXECUTION)
	{

	}
#endif // _DEBUG
}

void thread_set_name(int_x iThreadId, const char_16 * szName)
{
#ifdef _DEBUG
	char_8 szNameA[128] = {0};
	unicodetoansi(szName, -1, szNameA, 128);
	thread_set_name(iThreadId, szNameA);
#endif // _DEBUG
}

VENUS_END
