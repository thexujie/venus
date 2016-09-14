#pragma once

#include "../venus/venus.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#if defined _WINDLL && !defined VENUS_NO_EXPORT
#define WIN32_API __declspec (dllexport)
#else
#define WIN32_API
#endif

//  «∑Ò±‡“Î GDI
#define WIN32_DEVICE_GDI
//  «∑Ò±‡“Î GDI+
#define WIN32_DEVICE_GDIP
//  «∑Ò±‡“Î Direct2D
#define WIN32_DEVICE_D2D

VENUS_BEG

const uint_32 WIN32_IMAGE_TAG_ICON = make_fourcc('i', 'c', 'o', ':');
const uint_32 WIN32_IMAGE_TAG_BITMAP = make_fourcc('b', 'm', 'p', ':');

struct win32_image_t
{
	uint_32 tag;
	union
	{
		HICON hicon;
		HBITMAP hbitmap;
	};
};

// use HRESULT_FROM_WIN32(ERROR_*(LSTATUS)) to generate a E_*(HRESULT) from a standard win32 error.
struct winerr_t
{
	winerr_t() :hResult(0) {}
	winerr_t(int_32 error) :hResult(error) {}

	winerr_t & operator = (int_32 error)
	{
		hResult = error;
		return *this;
	}

	operator HRESULT() const { return hResult; }
	bool ok() const { return hResult == 0; }
	bool error() const { return hResult != 0; }
	bool succeeded() { return SUCCEEDED(hResult); }
	bool failed() { return FAILED(hResult); }

	HRESULT hResult;
};

struct winrect_t : public RECT
{
	winrect_t() {}
	winrect_t(int_x x, int_x y, int_x w, int_x h)
	{
		left = (int_32)x; top = (int_32)y;
		right = (int_32)(x + w); bottom = (int_32)(y + h);
	}
	winrect_t(const rectix & rect)
	{
		left = (int_32)rect.x; top = (int_32)rect.y;
		right = (int_32)rect.right(); bottom = (int_32)rect.bottom();
	}

	int_x width() const { return right - left; }
	int_x height() const { return bottom - top; }
};

const version_t winver_windows_new = {11, 0, 0, 0};
const version_t winver_windows_10 = {10, 0, 0, 0};
const version_t winver_windows_8_1 = {6, 3, 0, 0};
const version_t winver_windows_8 = {6, 2, 0, 0};
const version_t winver_windows_7 = {6, 1, 0, 0};
const version_t winver_windows_vista = {6, 0, 0, 0};
const version_t winver_windows_xp_x64 = {5, 2, 0, 0};
const version_t winver_windows_xp_x86 = {5, 1, 0, 0};
const version_t winver_windows_2000 = {5, 0, 0, 0};
const version_t winver_windows_old = {4, 0, 0, 0};
const version_t winver_windows_unknown = {0, 0, 0, 0};

class WIN32_API Win32
{
public:
	Win32() = delete;
	~Win32() = delete;

	static HINSTANCE GetCurrentInstance();
	static void Quit(int_x iReturn);
	static version_t GetVersion();
	static chbufferw<MAX_FONTNAME> GetDefaultFontName();
	static void GetSysColors(uint_32 * colors, int_x iCount);

	static bool FormatWinError(char_16 * szBuffer, int_x iSize, uint_32 uiErrorCode = UI32_MAX);
	static const char_16 * FormatWinError(uint_32 uiErrorCode = UI32_MAX);
	static void DebugSetLastError(uint_32 uiErroeCode = 0);
	static const char_16 * DebugFormatLastError();
};


VENUS_END

// tools
#include "tools/guid.h"
#include "tools/bitmap.h"
#include "tools/ResTools.h"
#include "tools/CHandle.h"
#include "tools/CBmpBuffer.h"
#include "tools/CDropTarget.h"
#include "tools/CGlobalData.h"
#include "tools/CInputContext.h"
#include "tools/CRegKey.h"
#include "tools/CSimpleStdOut.h"
#include "tools/CModule.h"
#include "tools/CMonitor.h"
#include "tools/UnknownT.h"
#include "tools/process/CProcessSnapshot.h"
#include "tools/process/CProcess.h"

// uniscribe
#include "uniscribe/CUspFactory.h"
#include "uniscribe/CTextLayoutUsp.h"

// shell
#include "shell/CShellItemImage.h"
#include "shell/CSHFileInfoBuffer.h"

// ui
#include "ui/Window.h"
#include "ui/CWindow.h"
#include "ui/controls/CFolderListView.h"
#include "ui/controls/CFolderTreeView.h"
#include "ui/CWin32UILoaderXml.h"

// paint
#include "dxgi/CDxgiSurface.h"

#include "gdi/C2DDeviceGdi.h"
#include "gdip/C2DDeviceGdip.h"
#include "d2d/C2DDeviceD2D.h"

#include "CWin32App.h"

