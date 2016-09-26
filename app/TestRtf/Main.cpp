#include "stdafx.h"
#include "BaseInc.h"

#include "Uniscribe.h"
#include "CRtf.h"

const char_x APP_NAME[] = _T("TestRtf");
const char_x RTF_FILE[] = _T("../bin/rtf.rtf");

LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam);
void Main()
{
	fileinfo_t fileInfo(RTF_FILE);
	if(!fileInfo.exists)
	{
		MessageBox(NULL, _T("无法找到文件\n../bin/rtf.rtf"), _T("错误"), MB_ICONERROR);
		return;
	}

	CWin32App app;

	WNDCLASSEX wcex = {sizeof(WNDCLASSEX)};
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = wcex.hIcon;
	wcex.hInstance = Win32::GetCurrentInstance();
	wcex.lpfnWndProc = WndProc;
	wcex.lpszClassName= APP_NAME;
	wcex.lpszMenuName = NULL;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&wcex);

	HWND hWnd = CreateWindowEx(0, APP_NAME, APP_NAME, WS_OVERLAPPEDWINDOW | WS_VSCROLL, 300, 150, 800, 600,
							   NULL, NULL, Win32::GetCurrentInstance(), NULL);

	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);

	MSG msg = {};
	while(::GetMessage(&msg, NULL, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

CScriptEngine engine;
CScriptDocument doc;
int iBaseY = 0;

void OnCreate(HWND hWnd)
{
	CRtfParser rtf;
	CFileStream sfs(RTF_FILE, StreamModeRead);
	int_x iSize = sfs.GetFileSize();
	texta text(iSize + 1);
	text.resize(iSize);
	sfs.Read((byte_t *)text.buffer(), iSize);
	rtf.LoadRtf(&doc, text, text + text.length());
	doc.Generate(&engine);
}

void OnDestroy(HWND hWnd)
{
	doc.Destroy();
	engine.Destroy();
	PostQuitMessage(0);
}

void OnSize(HWND hWnd)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	doc.GenerateLine(&engine, rc.right);

	SCROLLINFO si = {};
	si.cbSize = sizeof(si);
	si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
	si.nPos = iBaseY;
	si.nPage = rc.bottom;
	si.nMin = 0;
	si.nMax = (int_32)doc.GetHeight();

	SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
	if(iBaseY > si.nMax - (int)si.nPage)
		iBaseY = si.nMax - si.nPage;
	if(iBaseY < 0)
		iBaseY = 0;
}

void OnPaint(HWND hWnd)
{
	RECT rc;
	GetClientRect(hWnd, &rc);

	PAINTSTRUCT ps = {};
	HDC hdc = BeginPaint(hWnd, &ps);
	HDC hdc2 = CreateCompatibleDC(hdc);
	HBITMAP hBitmap = CreateBitmap(rc.right, rc.bottom, 1, 32, 0);
	SelectObject(hdc2, hBitmap);
	SetBkMode(hdc2, TRANSPARENT);
	
	recti32 rect(0, 0, rc.right, rc.bottom);
	Rectangle(hdc2, rect.x, rect.y, rect.right(), rect.bottom());

	doc.Draw(&engine, hdc2, 0, rect.y - iBaseY, rect.w, rect.h);

	BitBlt(hdc, 0, 0, rc.right, rc.bottom, hdc2, 0, 0, SRCCOPY);

	DeleteObject(hBitmap);
	DeleteDC(hdc2);
	EndPaint(hWnd, &ps);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam)
{
	switch(uiMessage)
	{
	case WM_MOUSEWHEEL:
		if((int_16)(wParam >> 16) > 0)
		{
			SCROLLINFO si = {sizeof(SCROLLINFO)};
			si.fMask = SIF_PAGE | SIF_POS;
			GetScrollInfo(hWnd, SB_VERT, &si);
			SetScrollPos(hWnd, SB_VERT, si.nPos - si.nPage / 10, TRUE);
			iBaseY = si.nPos;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		else
		{
			SCROLLINFO si = {sizeof(SCROLLINFO)};
			si.fMask = SIF_PAGE | SIF_POS;
			GetScrollInfo(hWnd, SB_VERT, &si);
			SetScrollPos(hWnd, SB_VERT, si.nPos + si.nPage / 10, TRUE);
			iBaseY = si.nPos;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;
	case WM_ERASEBKGND:
		return 0;
	case WM_CREATE:
		OnCreate(hWnd);
		break;
	case WM_SIZE:
		OnSize(hWnd);
		break;
	case WM_DESTROY:
		OnDestroy(hWnd);
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_PAINT:
		OnPaint(hWnd);
		break;
	case WM_VSCROLL:
		if(LOWORD(wParam) == SB_THUMBTRACK)
		{
			SCROLLINFO si = {sizeof(si)};
			si.fMask = SIF_ALL;
			GetScrollInfo(hWnd, SB_VERT, &si);
			iBaseY = si.nTrackPos;
			SetScrollPos(hWnd, SB_VERT, iBaseY, TRUE);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;
	default:
		return DefWindowProc(hWnd, uiMessage, wParam, lParam);
	}
	return 0;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
#ifdef _DEBUG
	_CrtMemState stateOld, stateNew, stateDiff;
	_CrtMemCheckpoint(&stateOld);
	Main();
	_CrtMemCheckpoint(&stateNew);
	if(_CrtMemDifference(&stateDiff, &stateOld, &stateNew))
		_CrtMemDumpAllObjectsSince(&stateDiff);
#else 
	Main();
#endif // _DEBUG
	return 0;
}
