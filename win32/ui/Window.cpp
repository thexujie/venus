#include "stdafx.h"
#include "Window.h"

VENUS_BEG

rectix Window::GetRect(HWND hWnd)
{
	RECT rc;
	::GetWindowRect(hWnd, &rc);
	return rectix(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
}

rectix Window::GetClient(HWND hWnd)
{
	RECT rcClient;
	::GetClientRect(hWnd, &rcClient);
	POINT ptClient = {0, 0};
	::ClientToScreen(hWnd, &ptClient);
	return rectix(ptClient.x, ptClient.y, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
}

winfo_t Window::GetInfo(HWND hWnd)
{
	winfo_t info;

	WINDOWINFO winfo;
	::GetWindowInfo(hWnd, &winfo);

	info.window = rectix(winfo.rcWindow.left, winfo.rcWindow.top,
						 winfo.rcWindow.right - winfo.rcWindow.left,
						 winfo.rcWindow.bottom - winfo.rcWindow.top);

	info.client = rectix(winfo.rcClient.left, winfo.rcClient.top,
						 winfo.rcClient.right - winfo.rcClient.left,
						 winfo.rcClient.bottom - winfo.rcClient.top);

	info.border = sizeix((int_x)winfo.cxWindowBorders, (int_x)winfo.cyWindowBorders);
	info.margin = edgeix(winfo.rcClient.left - winfo.rcWindow.left,
						 winfo.rcClient.top - winfo.rcWindow.top,
						 winfo.rcWindow.right - winfo.rcClient.right,
						 winfo.rcWindow.bottom - winfo.rcClient.bottom);
	return info;
}

edgeix Window::GetMargin(HWND hWnd)
{
	edgeix margin;
	WINDOWINFO winfo;
	::GetWindowInfo(hWnd, &winfo);
	margin = edgeix(winfo.rcClient.left - winfo.rcWindow.left,
					winfo.rcClient.top - winfo.rcWindow.top,
					winfo.rcWindow.right - winfo.rcClient.right,
					winfo.rcWindow.bottom - winfo.rcClient.bottom);
	return margin;
}

HWND Window::GetTop(HWND hWnd)
{
	HWND hTop = NULL;
	while(hWnd)
	{
		LONG_PTR lStyle = ::GetWindowLongPtrW(hWnd, GWL_STYLE);
		if(!(lStyle & WS_CHILD))
		{
			hTop = hWnd;
			break;
		}

		hWnd = GetParent(hWnd);
	}
	return hTop;
}

HWND Window::GetOwner(HWND hWnd)
{
	HWND hTop = Window::GetTop(hWnd);
	HWND hOwner = ::GetWindow(hTop, GW_OWNER);
	return hOwner;
}

bool Window::IsChildOf(HWND hWnd, HWND hWndParent)
{
	if(!hWndParent || !hWnd)
		return false;

	hWnd = ::GetParent(hWnd);
	while(hWnd)
	{
		if(hWnd == hWndParent)
			return true;
		else
			hWnd = ::GetParent(hWnd);
	}
	return false;
}

WNDPROC Window::GetDefaultWndProc(HWND hWnd)
{
	if(!hWnd)
		return NULL;

	WNDPROC pfnOldWndProc = (WNDPROC)GetPropW(hWnd, WINDOW_PROP_OLD_WNDPROC);
	if(!pfnOldWndProc)
		return DefWindowProcW;
	else
		return pfnOldWndProc;
}

void Window::PreRegisterClass()
{
	HINSTANCE hInstance = Win32::GetCurrentInstance();
	WNDCLASSEXW wcex = {sizeof(WNDCLASSEXW)};
	if(!GetClassInfoExW(hInstance, WINDOW_CLASS_NAME, &wcex))
	{
		wcex.style = 0;
		wcex.lpfnWndProc = Window::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = NULL;
		wcex.hCursor = /*::LoadCursor(NULL, IDC_ARROW)*/NULL;
		wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = WINDOW_CLASS_NAME;
		wcex.hIconSm = NULL;

		RegisterClassExW(&wcex);
	}
}

void Window::PreUnRegisterClass()
{
	HINSTANCE hInstance = Win32::GetCurrentInstance();
	WNDCLASSEXW wcex = {sizeof(WNDCLASSEXW)};

	if(GetClassInfoExW(hInstance, WINDOW_CLASS_NAME, &wcex))
		UnregisterClassW(WINDOW_CLASS_NAME, hInstance);
}

LRESULT CALLBACK Window::WndProc(HWND hWnd, UINT uiMessage, WPARAM uiParam, LPARAM iParam)
{
	IWindow * pWindow = Window::Search(hWnd);

	switch(uiMessage)
	{
	case WM_MOVE:
	case WM_MOUSEMOVE:
	case WM_NCMOUSEMOVE:
	case WM_SIZE:
	case WM_NCHITTEST:
	case WM_PAINT:
	case WM_SETCURSOR:
	case WM_WINDOWPOSCHANGED:
	case WM_WINDOWPOSCHANGING:
	case WM_GETMINMAXINFO:
	case WM_MOVING:
	case WM_IME_NOTIFY:
	case WM_ERASEBKGND:
	case WM_NCPAINT:
	case WM_NCMOUSELEAVE:
		break;
	default:
		//Printf("消息：[%02d:%02d:%02d:%03d][0x%04X][%s](%u, %d)\n", time.Hour, time.Minute, time.Second, time.MimiSecond, 
		//uiMessage, GetWmMessageText(uiMessage), uiParam, iParam);
		break;
	}

	if(pWindow)
		return pWindow->HandleMSG(uiMessage, uiParam, iParam);
	else
	{
		//TimeT time;
		//Printf(_T("未处理的消息：[%02d:%02d:%02d:%03d][0x%04X][%s](%u, %d)\n"), time.Hour, time.Minute, time.Second, time.MimiSecond,
		//	   uiMessage, GetWmMessageText(uiMessage), uiParam, iParam);
		switch(uiMessage)
		{
		case WM_PAINT:
			if(true)
			{
				PAINTSTRUCT ps;
				HDC hdc = ::BeginPaint(hWnd, &ps);
				EndPaint(hWnd, &ps);
			}
			break;
		case WM_DESTROY:
			Win32::Quit(0);
			break;
		default:
			return DefWindowProcW(hWnd, uiMessage, uiParam, iParam);
		}
		return 0;
	}
}

bool Window::Attatch(HWND hWnd, IWindow * pWnd)
{
	if(!hWnd || !pWnd)
		return false;
#ifdef _DEBUG
	if(GetPropW(hWnd, WINDOW_PROP_THIS_PTR))
		throw exp_bad_state();
	if(GetPropW(hWnd, WINDOW_PROP_DLG_RESULT))
		throw exp_bad_state();
	if(GetPropW(hWnd, WINDOW_PROP_OLD_WNDPROC))
		throw exp_bad_state();
#endif
	if(Window::Search(hWnd))
		return false;
	else
	{
		SetPropW(hWnd, WINDOW_PROP_THIS_PTR, (HANDLE)(void *)pWnd);
		SetPropW(hWnd, WINDOW_PROP_DLG_RESULT, (HANDLE)0);


		WNDPROC pfnWndProcOld = (WNDPROC)GetWindowLongPtrW(hWnd, GWLP_WNDPROC);
		if(pfnWndProcOld != Window::WndProc)
		{
			SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)Window::WndProc);
			SetPropW(hWnd, WINDOW_PROP_OLD_WNDPROC, (HANDLE)pfnWndProcOld);
		}
		return true;
	}
}

void Window::Detatch(HWND hWnd)
{
	if(hWnd)
	{
		RemovePropW(hWnd, WINDOW_PROP_THIS_PTR);
		RemovePropW(hWnd, WINDOW_PROP_DLG_RESULT);
		WNDPROC pfnWndProc = (WNDPROC)GetWindowLongPtrW(hWnd, GWLP_WNDPROC);
		if(pfnWndProc == Window::WndProc)
		{
			WNDPROC pfnOldWndProc = (WNDPROC)GetPropW(hWnd, WINDOW_PROP_OLD_WNDPROC);
			if(pfnOldWndProc)
			{
				SetWindowLongPtrW(hWnd, GWLP_WNDPROC, (LONG_PTR)pfnOldWndProc);
				SetPropW(hWnd, WINDOW_PROP_OLD_WNDPROC, (HANDLE)NULL);
			}
		}
	}
}

IWindow * Window::Search(HWND hWnd)
{
	return (IWindow *)(void *)GetPropW(hWnd, WINDOW_PROP_THIS_PTR);
}

int_x Window::SendMSG(HWND hWnd, uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	return (int_x)::SendMessageW(hWnd, uiMessage, uiParam, iParam);
}

bool Window::PostMSG(HWND hWnd, uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	return ::PostMessageW(hWnd, uiMessage, uiParam, iParam) != FALSE;
}

void Window::HandleMSG(MSG & msg)
{
	IWindow * pWindow = Window::Search(msg.hwnd);
	if(pWindow)
	{
		uint_x uiParam = (int_x)msg.wParam;
		int_x iParam = (int_x)msg.lParam;
		PreMessageResultE eResult = pWindow->PreHandleMSG(msg.message, uiParam, iParam);
		msg.wParam = (WPARAM)uiParam;
		msg.lParam = (LPARAM)iParam;
		switch(eResult)
		{
		case PreMessageResultOk:
			::TranslateMessage(&msg);
			::DispatchMessageW(&msg);
			break;
		case PreMessageResultNotTranslate:
			::DispatchMessageW(&msg);
			break;
		case PreMessageResultIgnore:
			break;
		default:
			break;
		}
	}
	else
	{
		::TranslateMessage(&msg);
		::DispatchMessageW(&msg);
	}
	if(pWindow)
		pWindow->AfterHandleMSG(msg.message, msg.wParam, msg.lParam);
}

void Window::ClearMSG()
{
	MSG msg = {0};
	bool bRunning = true;
	int_x iRet = 0;
	while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		Window::HandleMSG(msg);
}

bool Window::SetDialogResult(HWND hWnd, int_x iResult)
{
	if(hWnd)
		return SetPropW(hWnd, WINDOW_PROP_DLG_RESULT, (HANDLE)iResult) == TRUE;
	else
		return false;
}

int_x Window::GetDialogResult(HWND hWnd)
{
	if(hWnd)
		return (int_x)GetPropW(hWnd, WINDOW_PROP_DLG_RESULT);
	else
		return 0;
}

pointix Window::GetMousePos(HWND hWnd)
{
	POINT point = {0, 0};
	::GetCursorPos(&point);
	::ScreenToClient(hWnd, &point);
	return pointix(point.x, point.y);
}

#define MSG_TEXT(msg) case msg: return _T(#msg)

const char_x * Window::GetWmMessageText(uint_32 uiMessage)
{
	switch(uiMessage)
	{
		MSG_TEXT(WM_NULL);
		MSG_TEXT(WM_CREATE);
		MSG_TEXT(WM_DESTROY);
		MSG_TEXT(WM_MOVE);
		MSG_TEXT(WM_SIZE);
		MSG_TEXT(WM_ACTIVATE);
		MSG_TEXT(WM_SETFOCUS);
		MSG_TEXT(WM_KILLFOCUS);
		MSG_TEXT(WM_ENABLE);
		MSG_TEXT(WM_SETREDRAW);
		MSG_TEXT(WM_SETTEXT);
		MSG_TEXT(WM_GETTEXT);
		MSG_TEXT(WM_GETTEXTLENGTH);
		MSG_TEXT(WM_PAINT);
		MSG_TEXT(WM_CLOSE);
		MSG_TEXT(WM_QUERYENDSESSION);
		MSG_TEXT(WM_QUERYOPEN);
		MSG_TEXT(WM_ENDSESSION);
		MSG_TEXT(WM_QUIT);
		MSG_TEXT(WM_ERASEBKGND);
		MSG_TEXT(WM_SYSCOLORCHANGE);
		MSG_TEXT(WM_SHOWWINDOW);
		MSG_TEXT(WM_SETTINGCHANGE);

		MSG_TEXT(WM_DEVMODECHANGE);
		MSG_TEXT(WM_ACTIVATEAPP);
		MSG_TEXT(WM_FONTCHANGE);
		MSG_TEXT(WM_TIMECHANGE);
		MSG_TEXT(WM_CANCELMODE);
		MSG_TEXT(WM_SETCURSOR);
		MSG_TEXT(WM_MOUSEACTIVATE);
		MSG_TEXT(WM_CHILDACTIVATE);
		MSG_TEXT(WM_QUEUESYNC);
		MSG_TEXT(WM_GETMINMAXINFO);
		MSG_TEXT(WM_PAINTICON);
		MSG_TEXT(WM_NEXTDLGCTL);
		MSG_TEXT(WM_SPOOLERSTATUS);
		MSG_TEXT(WM_DRAWITEM);
		MSG_TEXT(WM_MEASUREITEM);
		MSG_TEXT(WM_DELETEITEM);
		MSG_TEXT(WM_VKEYTOITEM);
		MSG_TEXT(WM_CHARTOITEM);
		MSG_TEXT(WM_SETFONT);
		MSG_TEXT(WM_GETFONT);
		MSG_TEXT(WM_SETHOTKEY);
		MSG_TEXT(WM_GETHOTKEY);
		MSG_TEXT(WM_QUERYDRAGICON);
		MSG_TEXT(WM_COMPAREITEM);
		MSG_TEXT(WM_GETOBJECT);
		MSG_TEXT(WM_COMPACTING);
		MSG_TEXT(WM_COMMNOTIFY);
		MSG_TEXT(WM_WINDOWPOSCHANGING);
		MSG_TEXT(WM_WINDOWPOSCHANGED);
		MSG_TEXT(WM_POWER);
		MSG_TEXT(WM_COPYDATA);
		MSG_TEXT(WM_CANCELJOURNAL);
		MSG_TEXT(WM_NOTIFY);
		MSG_TEXT(WM_INPUTLANGCHANGEREQUEST);
		MSG_TEXT(WM_INPUTLANGCHANGE);
		MSG_TEXT(WM_TCARD);
		MSG_TEXT(WM_HELP);
		MSG_TEXT(WM_USERCHANGED);
		MSG_TEXT(WM_NOTIFYFORMAT);
		MSG_TEXT(WM_CONTEXTMENU);
		MSG_TEXT(WM_STYLECHANGING);
		MSG_TEXT(WM_STYLECHANGED);
		MSG_TEXT(WM_DISPLAYCHANGE);
		MSG_TEXT(WM_GETICON);
		MSG_TEXT(WM_SETICON);
		MSG_TEXT(WM_NCCREATE);
		MSG_TEXT(WM_NCDESTROY);
		MSG_TEXT(WM_NCCALCSIZE);
		MSG_TEXT(WM_NCHITTEST);
		MSG_TEXT(WM_NCPAINT);
		MSG_TEXT(WM_NCACTIVATE);
		MSG_TEXT(WM_GETDLGCODE);
		MSG_TEXT(WM_SYNCPAINT);
		MSG_TEXT(WM_NCMOUSEMOVE);
		MSG_TEXT(WM_NCLBUTTONDOWN);
		MSG_TEXT(WM_NCLBUTTONUP);
		MSG_TEXT(WM_NCLBUTTONDBLCLK);
		MSG_TEXT(WM_NCRBUTTONDOWN);
		MSG_TEXT(WM_NCRBUTTONUP);
		MSG_TEXT(WM_NCRBUTTONDBLCLK);
		MSG_TEXT(WM_NCMBUTTONDOWN);
		MSG_TEXT(WM_NCMBUTTONUP);
		MSG_TEXT(WM_NCMBUTTONDBLCLK);
		MSG_TEXT(WM_NCXBUTTONDOWN);
		MSG_TEXT(WM_NCXBUTTONUP);
		MSG_TEXT(WM_NCXBUTTONDBLCLK);
#if(_WIN32_WINNT >= _WIN32_WINNT_WS03)
		MSG_TEXT(WM_INPUT_DEVICE_CHANGE);
#endif
#if(_WIN32_WINNT >= _WIN32_WINNT_WINXP)
		MSG_TEXT(WM_INPUT);
#endif
		MSG_TEXT(WM_KEYDOWN);
		MSG_TEXT(WM_KEYUP);
		MSG_TEXT(WM_CHAR);
		MSG_TEXT(WM_DEADCHAR);
		MSG_TEXT(WM_SYSKEYDOWN);
		MSG_TEXT(WM_SYSKEYUP);
		MSG_TEXT(WM_SYSCHAR);
		MSG_TEXT(WM_SYSDEADCHAR);
#if(_WIN32_WINNT >= _WIN32_WINNT_WINXP)
		MSG_TEXT(WM_UNICHAR);
#endif

#if (WINVER >= 0x0400)
		MSG_TEXT(WM_IME_STARTCOMPOSITION);
		MSG_TEXT(WM_IME_ENDCOMPOSITION);
		MSG_TEXT(WM_IME_COMPOSITION);
#endif // WINVER >= 0x0400
		MSG_TEXT(WM_INITDIALOG);
		MSG_TEXT(WM_COMMAND);
		MSG_TEXT(WM_SYSCOMMAND);
		MSG_TEXT(WM_TIMER);
		MSG_TEXT(WM_HSCROLL);
		MSG_TEXT(WM_VSCROLL);
		MSG_TEXT(WM_INITMENU);
		MSG_TEXT(WM_INITMENUPOPUP);
#if (WINVER >= 0x0601)
		MSG_TEXT(WM_GESTURE);
		MSG_TEXT(WM_GESTURENOTIFY);
		MSG_TEXT(WM_MENUSELECT);
		MSG_TEXT(WM_MENUCHAR);
		MSG_TEXT(WM_ENTERIDLE);
#endif // WINVER >= 0x0601

#ifndef _WIN32_WCE
		MSG_TEXT(WM_MENURBUTTONUP);
		MSG_TEXT(WM_MENUDRAG);
		MSG_TEXT(WM_MENUGETOBJECT);
		MSG_TEXT(WM_UNINITMENUPOPUP);
		MSG_TEXT(WM_MENUCOMMAND);

#if (_WIN32_WINNT >= 0x0500)
		MSG_TEXT(WM_CHANGEUISTATE);
		MSG_TEXT(WM_UPDATEUISTATE);
		MSG_TEXT(WM_QUERYUISTATE);
#endif //(_WIN32_WINNT >= 0x0500)
#endif // _WIN32_WCE

		MSG_TEXT(WM_CTLCOLORMSGBOX);
		MSG_TEXT(WM_CTLCOLOREDIT);
		MSG_TEXT(WM_CTLCOLORLISTBOX);
		MSG_TEXT(WM_CTLCOLORBTN);
		MSG_TEXT(WM_CTLCOLORDLG);
		MSG_TEXT(WM_CTLCOLORSCROLLBAR);
		MSG_TEXT(WM_CTLCOLORSTATIC);
		MSG_TEXT(MN_GETHMENU);
		MSG_TEXT(WM_MOUSEMOVE);
		MSG_TEXT(WM_LBUTTONDOWN);
		MSG_TEXT(WM_LBUTTONUP);
		MSG_TEXT(WM_LBUTTONDBLCLK);
		MSG_TEXT(WM_RBUTTONDOWN);
		MSG_TEXT(WM_RBUTTONUP);
		MSG_TEXT(WM_RBUTTONDBLCLK);
		MSG_TEXT(WM_MBUTTONDOWN);
		MSG_TEXT(WM_MBUTTONUP);
		MSG_TEXT(WM_MBUTTONDBLCLK);
#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
		MSG_TEXT(WM_MOUSEWHEEL);
#endif
#if (_WIN32_WINNT >= 0x0500)
		MSG_TEXT(WM_XBUTTONDOWN);
		MSG_TEXT(WM_XBUTTONUP);
		//MSG_TEXT(WM_XBUTTONDBLCLK);
#endif
#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
		MSG_TEXT(WM_MOUSEHWHEEL);
#endif

		//MSG_TEXT(WM_MOUSELAST);

		MSG_TEXT(WM_PARENTNOTIFY);
		MSG_TEXT(WM_ENTERMENULOOP);
		MSG_TEXT(WM_EXITMENULOOP);
		MSG_TEXT(WM_NEXTMENU);
		MSG_TEXT(WM_SIZING);
		MSG_TEXT(WM_CAPTURECHANGED);
		MSG_TEXT(WM_MOVING);
		MSG_TEXT(WM_POWERBROADCAST);
		MSG_TEXT(WM_DEVICECHANGE);
		MSG_TEXT(WM_MDICREATE);
		MSG_TEXT(WM_MDIDESTROY);
		MSG_TEXT(WM_MDIACTIVATE);
		MSG_TEXT(WM_MDIRESTORE);
		MSG_TEXT(WM_MDINEXT);
		MSG_TEXT(WM_MDIMAXIMIZE);
		MSG_TEXT(WM_MDITILE);
		MSG_TEXT(WM_MDICASCADE);
		MSG_TEXT(WM_MDIICONARRANGE);
		MSG_TEXT(WM_MDIGETACTIVE);
		MSG_TEXT(WM_MDISETMENU);
		MSG_TEXT(WM_ENTERSIZEMOVE);
		MSG_TEXT(WM_EXITSIZEMOVE);
		MSG_TEXT(WM_DROPFILES);
		MSG_TEXT(WM_MDIREFRESHMENU);
#if(_WIN32_WINNT >= 0x0601)
		MSG_TEXT(WM_TOUCH);
#endif
		MSG_TEXT(WM_IME_SETCONTEXT);
		MSG_TEXT(WM_IME_NOTIFY);
		MSG_TEXT(WM_IME_CONTROL);
		MSG_TEXT(WM_IME_COMPOSITIONFULL);
		MSG_TEXT(WM_IME_SELECT);
		MSG_TEXT(WM_IME_CHAR);
		MSG_TEXT(WM_IME_REQUEST);
		MSG_TEXT(WM_IME_KEYDOWN);
		MSG_TEXT(WM_IME_KEYUP);
		MSG_TEXT(WM_MOUSEHOVER);
		MSG_TEXT(WM_MOUSELEAVE);
#if(WINVER >= 0x0500)
		MSG_TEXT(WM_NCMOUSEHOVER);
		MSG_TEXT(WM_NCMOUSELEAVE);
#endif /* WINVER >= 0x0500 */
#if(_WIN32_WINNT >= _WIN32_WINNT_WINXP)
		MSG_TEXT(WM_WTSSESSION_CHANGE);
		MSG_TEXT(WM_TABLET_FIRST);
		MSG_TEXT(WM_TABLET_LAST);
#endif
		MSG_TEXT(WM_CUT);
		MSG_TEXT(WM_COPY);
		MSG_TEXT(WM_PASTE);
		MSG_TEXT(WM_CLEAR);
		MSG_TEXT(WM_UNDO);
		MSG_TEXT(WM_RENDERFORMAT);
		MSG_TEXT(WM_RENDERALLFORMATS);
		MSG_TEXT(WM_DESTROYCLIPBOARD);
		MSG_TEXT(WM_DRAWCLIPBOARD);
		MSG_TEXT(WM_PAINTCLIPBOARD);
		MSG_TEXT(WM_VSCROLLCLIPBOARD);
		MSG_TEXT(WM_SIZECLIPBOARD);
		MSG_TEXT(WM_ASKCBFORMATNAME);
		MSG_TEXT(WM_CHANGECBCHAIN);
		MSG_TEXT(WM_HSCROLLCLIPBOARD);
		MSG_TEXT(WM_QUERYNEWPALETTE);
		MSG_TEXT(WM_PALETTEISCHANGING);
		MSG_TEXT(WM_PALETTECHANGED);
		MSG_TEXT(WM_HOTKEY);
		MSG_TEXT(WM_PRINT);
		MSG_TEXT(WM_PRINTCLIENT);
		MSG_TEXT(WM_APPCOMMAND);
#if(_WIN32_WINNT >= _WIN32_WINNT_WINXP)
		MSG_TEXT(WM_THEMECHANGED);
#endif
#if(_WIN32_WINNT >= _WIN32_WINNT_WS03)
		MSG_TEXT(WM_CLIPBOARDUPDATE);
#endif
#if(_WIN32_WINNT >= _WIN32_WINNT_WIN6)
		MSG_TEXT(WM_DWMCOMPOSITIONCHANGED);
		MSG_TEXT(WM_DWMNCRENDERINGCHANGED);
		MSG_TEXT(WM_DWMCOLORIZATIONCOLORCHANGED);
		MSG_TEXT(WM_DWMWINDOWMAXIMIZEDCHANGE);
#endif
#if(_WIN32_WINNT >= _WIN32_WINNT_WIN7)
		MSG_TEXT(WM_DWMSENDICONICTHUMBNAIL);
		MSG_TEXT(WM_DWMSENDICONICLIVEPREVIEWBITMAP);
#endif
#if(_WIN32_WINNT >= _WIN32_WINNT_VISTA)
		MSG_TEXT(WM_GETTITLEBARINFOEX);
#endif
		MSG_TEXT(WM_HANDHELDFIRST);
		MSG_TEXT(WM_HANDHELDLAST);
		MSG_TEXT(WM_AFXFIRST);
		MSG_TEXT(WM_AFXLAST);
		MSG_TEXT(WM_PENWINFIRST);
		MSG_TEXT(WM_PENWINLAST);
		MSG_TEXT(WM_APP);
		MSG_TEXT(WM_REFRESH);
	default:
		return _T("Unknown MSG");
	}
}

KeyCodeE Window::VirtualKeyToKeyCode(int_x iVirtualKey)
{
	if(iVirtualKey < 0 || iVirtualKey >= 0xFF)
		return KeyCodeInvalid;

	const KeyCodeE NOTMATCH = KeyCodeInvalid;
	const KeyCodeE RESERVED = KeyCodeInvalid;
	const KeyCodeE UNASSIGNED = KeyCodeInvalid;
	const KeyCodeE NONE = KeyCodeInvalid;

	static const KeyCodeE KEYS[0xFF] =
	{
		//! 不可用的按键码。
		/*0x00*/NONE,
		/*0x01*/NONE, // VK_LBUTTON
		/*0x02*/NONE, // VK_RBUTTON
		/*0x03*/NONE, // VK_CANCEL
		/*0x04*/NONE, // VM_MBUTTON
		/*0x05*/NONE, // VK_XBUTTON1
		/*0x06*/NONE, // VK_XBUTTON2
		/*0x07*/NONE,
		/*0x08*/KeyCodeBackspace, // VK_BACK
		/*0x09*/KeyCodeTab, // VK_TAB
		/*0x0A*/NONE,
		/*0x0B*/NONE,
		/*0x0C*/KeyCodeClear,
		/*0x0D*/KeyCodeEnter,
		/*0x0E*/NONE,
		/*0x0F*/NONE,

		/*0x10*/KeyCodeShift,
		/*0x11*/KeyCodeControl,
		/*0x12*/KeyCodeAlt,
		/*0x13*/KeyCodePauseBreak,
		/*0x14*/KeyCodeCapsLock,
		/*0x15*/KeyCodeImeKana,
		/*0x16*/KeyCodeImeHangul,
		/*0x17*/KeyCodeImeJunja,
		/*0x18*/KeyCodeImeFinal,
		/*0x19*/KeyCodeImeKanji,
		/*0x1A*/NONE,
		/*0x1B*/KeyCodeEscape,
		/*0x1C*/KeyCodeImeConvert,
		/*0x1D*/KeyCodeImeNonConvert,
		/*0x1E*/KeyCodeImeAccept,
		/*0x1F*/KeyCodeImeModeChange,

		/*0x20*/KeyCodeSpace,
		/*0x21*/KeyCodePageUp,
		/*0x22*/KeyCodePageDown,
		/*0x23*/KeyCodeEnd,
		/*0x24*/KeyCodeHome,
		/*0x25*/KeyCodeLeft,
		/*0x26*/KeyCodeUp,
		/*0x27*/KeyCodeRight,
		/*0x28*/KeyCodeDown,
		/*0x29*/KeyCodeSelect,
		/*0x2A*/KeyCodePrint,
		/*0x2B*/KeyCodeExcute,
		/*0x2C*/KeyCodePrintScreen,
		/*0x2D*/KeyCodeInsert,
		/*0x2E*/KeyCodeDelete,
		/*0x2F*/KeyCodeHelp,

		/*0x30*/KeyCode0,
		/*0x31*/KeyCode1,
		/*0x32*/KeyCode2,
		/*0x33*/KeyCode3,
		/*0x34*/KeyCode4,
		/*0x35*/KeyCode5,
		/*0x36*/KeyCode6,
		/*0x37*/KeyCode7,
		/*0x38*/KeyCode8,
		/*0x39*/KeyCode9,
		/*0x3A*/NONE,
		/*0x3B*/NONE,
		/*0x3C*/NONE,
		/*0x3D*/NONE,
		/*0x3E*/NONE,
		/*0x3F*/NONE,

		/*0x40*/NONE,
		/*0x41*/KeyCodeA,
		/*0x42*/KeyCodeB,
		/*0x43*/KeyCodeC,
		/*0x44*/KeyCodeD,
		/*0x45*/KeyCodeEe,
		/*0x46*/KeyCodeF,
		/*0x47*/KeyCodeG,
		/*0x48*/KeyCodeH,
		/*0x49*/KeyCodeI,
		/*0x4A*/KeyCodeJ,
		/*0x4B*/KeyCodeK,
		/*0x4C*/KeyCodeL,
		/*0x4D*/KeyCodeM,
		/*0x4E*/KeyCodeN,
		/*0x4F*/KeyCodeO,

		/*0x50*/KeyCodeP,
		/*0x51*/KeyCodeQ,
		/*0x52*/KeyCodeR,
		/*0x53*/KeyCodeS,
		/*0x54*/KeyCodeT,
		/*0x55*/KeyCodeU,
		/*0x56*/KeyCodeV,
		/*0x57*/KeyCodeW,
		/*0x58*/KeyCodeX,
		/*0x59*/KeyCodeY,
		/*0x5A*/KeyCodeZ,
		/*0x5B*/KeyCodeWinL,
		/*0x5C*/KeyCodeWinR,
		/*0x5D*/KeyCodeApps,
		/*0x5E*/NONE,
		/*0x5F*/KeyCodeSleep,

		/*0x60*/KeyCodeNum0,
		/*0x61*/KeyCodeNum1,
		/*0x62*/KeyCodeNum2,
		/*0x63*/KeyCodeNum3,
		/*0x64*/KeyCodeNum4,
		/*0x65*/KeyCodeNum5,
		/*0x66*/KeyCodeNum6,
		/*0x67*/KeyCodeNum7,
		/*0x68*/KeyCodeNum8,
		/*0x69*/KeyCodeNum9,
		/*0x6A*/KeyCodeNumMul,
		/*0x6B*/KeyCodeNumAdd,
		/*0x6C*/KeyCodeNumEnter,
		/*0x6D*/KeyCodeNumSub,
		/*0x6E*/KeyCodeNumDecimal,
		/*0x6F*/KeyCodeNumDiv,

		/*0x70*/KeyCodeF1,
		/*0x71*/KeyCodeF2,
		/*0x72*/KeyCodeF3,
		/*0x73*/KeyCodeF4,
		/*0x74*/KeyCodeF5,
		/*0x75*/KeyCodeF6,
		/*0x76*/KeyCodeF7,
		/*0x77*/KeyCodeF8,
		/*0x78*/KeyCodeF9,
		/*0x79*/KeyCodeF10,
		/*0x7A*/KeyCodeF11,
		/*0x7B*/KeyCodeF12,
		/*0x7C*/KeyCodeF13,
		/*0x7D*/KeyCodeF14,
		/*0x7E*/KeyCodeF15,
		/*0x7F*/KeyCodeF16,

		/*0x80*/KeyCodeF17,
		/*0x81*/KeyCodeF18,
		/*0x82*/KeyCodeF19,
		/*0x83*/KeyCodeF20,
		/*0x84*/KeyCodeF21,
		/*0x85*/KeyCodeF22,
		/*0x86*/KeyCodeF23,
		/*0x87*/KeyCodeF24,
		/*0x88*/NONE,
		/*0x89*/NONE,
		/*0x8A*/NONE,
		/*0x8B*/NONE,
		/*0x8C*/NONE,
		/*0x8D*/NONE,
		/*0x8E*/NONE,
		/*0x8F*/NONE,

		/*0x90*/KeyCodeNumlock,
		/*0x91*/KeyCodeScrollLock,
		/*0x92*/KeyCodeNumEqual,
		/*0x93*/NONE,
		/*0x94*/NONE,
		/*0x95*/NONE,
		/*0x96*/NONE,
		/*0x97*/NONE,
		/*0x98*/NONE,
		/*0x99*/NONE,
		/*0x9A*/NONE,
		/*0x9B*/NONE,
		/*0x9C*/NONE,
		/*0x9D*/NONE,
		/*0x9E*/NONE,
		/*0x9F*/NONE,

		/*0xA0*/KeyCodeShiftL,
		/*0xA1*/KeyCodeShiftR,
		/*0xA2*/KeyCodeControlL,
		/*0xA3*/KeyCodeControlR,
		/*0xA4*/KeyCodeAltL,
		/*0xA5*/KeyCodeAltR,
		/*0xA6*/KeyCodeBrowserBack,
		/*0xA7*/KeyCodeBrowserForward,
		/*0xA8*/KeyCodeBrowserForward,
		/*0xA9*/KeyCodeBrowserStop,
		/*0xAA*/KeyCodeBrowserSearch,
		/*0xAB*/KeyCodeBrowserFavorites,
		/*0xAC*/KeyCodeBrowserHome,
		/*0xAD*/KeyCodeVolumeMute,
		/*0xAE*/KeyCodeVolumeDown,
		/*0xAF*/KeyCodeVolumeUp,

		/*0xB0*/KeyCodeMediaNext,
		/*0xB1*/KeyCodeMediaPrev,
		/*0xB2*/KeyCodeMediaStop,
		/*0xB3*/KeyCodeMediaPlayPause,
		/*0xB4*/KeyCodeLaunchMail,
		/*0xB5*/KeyCodeLaunchMediaSelect,
		/*0xB6*/KeyCodeLaunchApp1,
		/*0xB7*/KeyCodeLaunchApp2,
		/*0xB8*/NONE,
		/*0xB9*/NONE,
		/*0xBA*/KeyCodeSemicolon,
		/*0xBB*/KeyCodeEqual,
		/*0xBC*/KeyCodeComma,
		/*0xBD*/KeyCodeSub,
		/*0xBE*/KeyCodePeriod,
		/*0xBF*/KeyCodeSlash,

		/*0xC0*/KeyCodeGrave,
		/*0xC1*/RESERVED,
		/*0xC2*/RESERVED,
		/*0xC3*/RESERVED,
		/*0xC4*/RESERVED,
		/*0xC5*/RESERVED,
		/*0xC6*/RESERVED,
		/*0xC7*/RESERVED,
		/*0xC8*/RESERVED,
		/*0xC9*/RESERVED,
		/*0xCA*/RESERVED,
		/*0xCB*/RESERVED,
		/*0xCC*/RESERVED,
		/*0xCD*/RESERVED,
		/*0xCE*/RESERVED,
		/*0xCF*/RESERVED,

		/*0xD0*/RESERVED,
		/*0xD1*/RESERVED,
		/*0xD2*/RESERVED,
		/*0xD3*/RESERVED,
		/*0xD4*/RESERVED,
		/*0xD5*/RESERVED,
		/*0xD6*/RESERVED,
		/*0xD7*/RESERVED,
		/*0xD8*/UNASSIGNED,
		/*0xD9*/UNASSIGNED,
		/*0xDA*/UNASSIGNED,
		/*0xDB*/KeyCodeBracketL,
		/*0xDC*/KeyCodeBackslash,
		/*0xDD*/KeyCodeBracketR,
		/*0xDE*/KeyCodeApostrophe,
		/*0xDF*/NONE,

		/*0xE0*/RESERVED,
		/*0xE1*/NOTMATCH,
		/*0xE2*/KeyCodeComma,
		/*0xE3*/NOTMATCH,
		/*0xE4*/NOTMATCH,
		/*0xE5*/NOTMATCH,
		/*0xE6*/NOTMATCH,
		/*0xE7*/NOTMATCH,
		/*0xE8*/NOTMATCH,
		/*0xE9*/NOTMATCH,
		/*0xEA*/NOTMATCH,
		/*0xEB*/NOTMATCH,
		/*0xEC*/NOTMATCH,
		/*0xED*/NOTMATCH,
		/*0xEE*/NOTMATCH,
		/*0xEF*/NOTMATCH,

		/*0xF0*/NOTMATCH,
		/*0xF1*/NOTMATCH,
		/*0xF2*/NOTMATCH,
		/*0xF3*/NOTMATCH,
		/*0xF4*/NOTMATCH,
		/*0xF5*/NOTMATCH,
		/*0xF6*/NOTMATCH,
		/*0xF7*/NOTMATCH,
		/*0xF8*/NOTMATCH,
		/*0xF9*/NOTMATCH,
		/*0xFA*/NOTMATCH,
		/*0xFB*/NOTMATCH,
		/*0xFC*/NOTMATCH,
		/*0xFD*/NOTMATCH,
		/*0xFE*/NOTMATCH,
		/*0xFF*/
	};

	KeyCodeE eKey = KEYS[iVirtualKey];
	return eKey;
}

int_x Window::KeyCodeToVirtualKey(KeyCodeE eKeyCode)
{
	const int_x NONE = 0;

	if(eKeyCode < 0 || eKeyCode >= 0xFF)
		return NONE;

	static const int_x KEYS[0xFF] =
	{
		NONE,
		'0',
		'1',
		'2',
		'3',
		'4',
		'5',
		'6',
		'7',
		'8',
		'9',

		VK_CAPITAL,
		'A',
		'B',
		'C',
		'D',
		'E',
		'F',
		'G',
		'H',
		'I',
		'J',
		'K',
		'L',
		'M',
		'N',
		'O',
		'P',
		'Q',
		'R',
		'S',
		'T',
		'U',
		'V',
		'W',
		'X',
		'Y',
		'Z',

		VK_BACK,
		VK_TAB,
		VK_RETURN,

		VK_ESCAPE,
		VK_SPACE,

		VK_NUMLOCK,
		VK_NUMPAD0,
		VK_NUMPAD1,
		VK_NUMPAD2,
		VK_NUMPAD3,
		VK_NUMPAD4,
		VK_NUMPAD5,
		VK_NUMPAD6,
		VK_NUMPAD7,
		VK_NUMPAD8,
		VK_NUMPAD9,

		VK_ADD,
		VK_SUBTRACT,
		VK_MULTIPLY,
		VK_DIVIDE,
		VK_DECIMAL,
		NONE,
		NONE,

		VK_F1,
		VK_F2,
		VK_F3,
		VK_F4,
		VK_F5,
		VK_F6,
		VK_F7,
		VK_F8,
		VK_F9,
		VK_F10,
		VK_F11,
		VK_F12,
		VK_F13,
		VK_F14,
		VK_F15,
		VK_F16,
		VK_F17,
		VK_F18,
		VK_F19,
		VK_F20,
		VK_F21,
		VK_F22,
		VK_F23,
		VK_F24,

		NONE,
		NONE,
		NONE,
		NONE,
		NONE,
		NONE,
		NONE,
		NONE,
		NONE,
		NONE,
		NONE,
		NONE,

		VK_INSERT,
		VK_DELETE,
		VK_HOME,
		VK_END,
		VK_PRIOR,
		VK_NEXT,

		VK_LEFT,
		VK_UP,
		VK_RIGHT,
		VK_DOWN,

		VK_PRINT,
		VK_SCROLL,
		VK_PAUSE,

		VK_LWIN,
		VK_RWIN,

		VK_CONTROL,
		VK_LCONTROL,
		VK_RCONTROL,

		VK_SHIFT,
		VK_LSHIFT,
		VK_RSHIFT,

		VK_MENU,
		VK_LMENU,
		VK_RMENU,

		VK_APPS,

		VK_SLEEP,
	};
	return KEYS[eKeyCode];

}

VENUS_END
