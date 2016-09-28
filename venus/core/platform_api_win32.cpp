#include "stdafx.h"
#include "platform_api.h"

#include <windows.h>

VENUS_BEG

CORE_API err_t text_encode(int_x src_codepage, const void * src, int_x src_length,
							int_x dst_codepage, void * dst, int_x dst_size, int_x & dst_length)
{
	err_t err = err_ok;
	int_32 length = 0;
	switch(dst_codepage)
	{
	case 936:
		switch(src_codepage)
		{
		case 1200:
			length = WideCharToMultiByte(936, 0, static_cast<LPCWCH>(src), static_cast<int_32>(src_length), static_cast<LPSTR>(dst), static_cast<int_32>(dst_size), nullptr, nullptr);
			if(length < dst_size && dst)
				*(static_cast<LPSTR>(dst) + length) = 0;
			break;
		default:
			err = err_bad_format;
			break;
		}
		break;
	case CP_ACP:
		switch(src_codepage)
		{
		case 1200:
			length = WideCharToMultiByte(CP_ACP, 0, static_cast<LPCWCH>(src), static_cast<int_32>(src_length), static_cast<LPSTR>(dst), static_cast<int_32>(dst_size), nullptr, nullptr);
			if(length < dst_size && dst)
				*(static_cast<LPSTR>(dst) + length) = 0;
			break;
		default:
			err = err_bad_format;
			break;
		}
		break;
		// to utf-8
	case CP_UTF8:
		switch(src_codepage)
		{
		case 1200:
			length = WideCharToMultiByte(CP_UTF8, 0, static_cast<LPCWCH>(src), static_cast<int_32>(src_length), static_cast<LPSTR>(dst), static_cast<int_32>(dst_size), nullptr, nullptr);
			if(length < dst_size && dst)
				*(static_cast<LPSTR>(dst) + length) = 0;
			break;
		default:
			err = err_bad_format;
			break;
		}
		break;
	case 1200:
		//switch(src_codepage)
		//{
		//case CP_ACP:
		//case 936:
		//case CP_UTF8:
			length = MultiByteToWideChar(static_cast<uint_32>(src_codepage), 0, static_cast<LPCCH>(src), static_cast<int_32>(src_length), static_cast<LPWSTR>(dst), static_cast<int_32>(dst_size));
			if(length < dst_size && dst)
				*(static_cast<LPWSTR>(dst) + length) = 0;
		//	break;
		//default:
		//	err = err_bad_format;
		//	break;
		//}
		break;
	default:
		break;
	}

	dst_length = length;
	if(!length)
	{
		int_32 lerr = GetLastError();
		switch(lerr)
		{
		case ERROR_INSUFFICIENT_BUFFER:
			err = err_bounds;
			break;
		case ERROR_NO_UNICODE_TRANSLATION:
			err = err_bad_format;
			break;
		default:
			break;
		}
	}
	return err;
}

void thread_set_name(int_x iThreadId, const char_8 * szName)
{
#ifdef _DEBUG
#pragma pack(push, 4)
	struct THREADNAME_INFO
	{
		DWORD dwType; // must be 0x1000
		LPCSTR szName; // pointer to name (in same addr space)
		DWORD dwThreadID; // thread ID (-1 caller thread)
		DWORD dwFlags; // reserved for future use, most be zero
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
