#include "stdafx.h"
#include "win32.h"

VENUS_BEG

HINSTANCE Win32::GetCurrentInstance()
{
	return (HINSTANCE)GetModuleHandleW(NULL);
}

void Win32::Quit(int_x iReturn)
{
	::PostQuitMessage((int_32)iReturn);
}

version_t Win32::GetVersion()
{
#if _WIN32_WINNT <= _WIN32_WINNT_VISTA || 1
	OSVERSIONINFOEXW osVer = {sizeof(OSVERSIONINFOEXW)};
#pragma warning(disable : 4996)
	GetVersionExW((OSVERSIONINFOW *)&osVer);
#pragma warning(default : 4996)
	return version_t((int_x)osVer.dwMajorVersion, (int_x)osVer.dwMinorVersion, 0, (int_x)osVer.dwBuildNumber);
#else
	struct win_version_check
	{
		bool operator > (const version_t & version)
		{
			OSVERSIONINFOEXW osvi = {sizeof(osvi), version.major, version.minor, version.build};
			ulong_64 ulConditionMask = 0;
			VER_SET_CONDITION(ulConditionMask, VER_MAJORVERSION, VER_GREATER);
			if(VerifyVersionInfo(&osvi, VER_MAJORVERSION, ulConditionMask))
				return true;
			VER_SET_CONDITION(ulConditionMask, VER_MAJORVERSION, VER_EQUAL);
			VER_SET_CONDITION(ulConditionMask, VER_MINORVERSION, VER_GREATER);
			return VerifyVersionInfo(&osvi, VER_MAJORVERSION, ulConditionMask) != FALSE;
		}

		bool operator < (const version_t & version)
		{
			OSVERSIONINFOEXW osvi = {sizeof(osvi), version.major, version.minor, version.build};
			ulong_64 ulConditionMask = 0;
			VER_SET_CONDITION(ulConditionMask, VER_MAJORVERSION, VER_LESS);
			if(VerifyVersionInfo(&osvi, VER_MAJORVERSION, ulConditionMask))
				return true;
			VER_SET_CONDITION(ulConditionMask, VER_MAJORVERSION, VER_EQUAL);
			VER_SET_CONDITION(ulConditionMask, VER_MINORVERSION, VER_LESS);
			return VerifyVersionInfo(&osvi, VER_MAJORVERSION, ulConditionMask) != FALSE;
		}

		bool operator == (const version_t & version)
		{
			OSVERSIONINFOEXW osvi = {sizeof(osvi), version.major, version.minor, version.build};
			ulong_64 ulConditionMask = 0;
			VER_SET_CONDITION(ulConditionMask, VER_MAJORVERSION, VER_EQUAL);
			VER_SET_CONDITION(ulConditionMask, VER_MINORVERSION, VER_EQUAL);
			return VerifyVersionInfo(&osvi, VER_MAJORVERSION, ulConditionMask) != FALSE;
		}
	};
	win_version_check check;
	if(check > winver_windows_10)
		return winver_windows_new;
	else if(check == winver_windows_8_1)
		return winver_windows_8_1;
	else if(check == winver_windows_8)
		return winver_windows_8;
	else if(check == winver_windows_7)
		return winver_windows_7;
	else if(check == winver_windows_vista)
		return winver_windows_vista;
	else if(check == winver_windows_xp_x64)
		return winver_windows_xp_x64;
	else if(check == winver_windows_xp_x86)
		return winver_windows_xp_x86;
	else if(check == winver_windows_2000)
		return winver_windows_2000;
	else if(check < winver_windows_2000)
		return winver_windows_old;
	else
		return winver_windows_unknown;
#endif
}

chbufferw<MAX_FONTNAME> Win32::GetDefaultFontName()
{
	NONCLIENTMETRICS metrics;
	metrics.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, metrics.cbSize, &metrics, 0);
	chbufferw<MAX_FONTNAME> name;
	name = metrics.lfMessageFont.lfFaceName;
	//name = L"ËÎÌו";
	return name;
}

void Win32::GetSysColors(uint_32 * colors, int_x iCount)
{
	if(iCount != SysColorCount)
		return;

	colors[SysColorAuto] = Colors::Auto;

	colors[SysColor] = 0xFF3A33BC;
	colors[SysColorForm] = 0xFFFFFFFF;

	colors[SysColorBarBackNormal] = 0xFFF0F0F0;
	colors[SysColorBarNormal] = 0xFFCDCDCD;
	colors[SysColorBarHighlight] = 0xFFA6A6A6;
	colors[SysColorBarPressed] = 0xFF606060;
	colors[SysColorBarDisabled] = 0xFF808080;

	colors[SysColorCaptionActive] = 0xFF50AE81;
	colors[SysColorCaptionNoneActive] = 0xFF508181;

	colors[SysColorBorder] = 0xFF979797;

	colors[SysColorBackNormal] = 0xFFF0F0F0;
	colors[SysColorBackHoving] = 0xFFE5F3FB;
	colors[SysColorBackPressed] = 0xFFCBE8F6;
	colors[SysColorBackSelected] = 0xFFCBE8F6;
	colors[SysColorBackFocused] = 0xFFD5E3EB;
	colors[SysColorBackDisable] = 0xFFD9D9D9;

	colors[SysColorForeNormal] = 0xFF000000;
	colors[SysColorForeDisable] = 0xFF6D6D6D;

	colors[SysColorFrameNormal] = 0xFFA0A0A0;
	colors[SysColorFrameHoving] = 0xFF70C0E7;
	colors[SysColorFramePressed] = 0xFF26A0DA;
	colors[SysColorFrameSelected] = 0xFF26A0DA;
	colors[SysColorFrameFocused] = 0xFF3399FF;
	//colors[SysColorFrameDisable] = 0xFFD9D9D9;
	colors[SysColorFrameDisable] = 0xFFADADAD;

	colors[SysColorTextSelectNormal] = 0xFFA7BDE2;
	colors[SysColorTextSelectDisable] = 0xFFEEEEEE;

	colors[SysColorLineNormal] = 0xAADBE4FE;
	colors[SysColorLineHighlight] = 0xFF3388FF;
	colors[SysColorLineDisable] = 0xFFDBDBDB;

	colors[SysColorPaneCaptionBackNormal] = 0xFF507354;
	colors[SysColorPaneCaptionBackSelected] = 0xFF80A354;

}

bool Win32::FormatWinError(char_16 * szBuffer, int_x iSize, uint_32 uiErrorCode/* = UI32_MAX*/)
{
	if(uiErrorCode == UI32_MAX)
		uiErrorCode = GetLastError();
	DWORD dwLen = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, uiErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		szBuffer,
		(DWORD)iSize,
		NULL
		);

	if(dwLen > 2)
	{
		szBuffer[dwLen - 1] = 0;
		szBuffer[dwLen - 2] = 0;
		dwLen -= 2;
	}
	return dwLen > 0;
}

const char_16 * Win32::FormatWinError(uint_32 uiErrorCode)
{
	if(uiErrorCode == UI32_MAX)
		uiErrorCode = GetLastError();
	static char_16 szError[1024] = {0};
	if(Win32::FormatWinError(szError, 1024, uiErrorCode))
		return szError;
	else
		return L"An error occurred while Formating last error.";
}

void Win32::DebugSetLastError(uint_32 uiErroeCode/* = 0*/)
{
#ifdef _DEBUG
	SetLastError(uiErroeCode);
#endif
}

const char_16 * Win32::DebugFormatLastError()
{
#ifdef _DEBUG
	return Win32::FormatWinError();
#else
	return nullptr;
#endif
}

VENUS_END
