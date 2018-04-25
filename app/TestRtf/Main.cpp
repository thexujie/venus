#include "stdafx.h"
#include "BaseInc.h"

#include "Uniscribe.h"
#include "CRtf.h"

const char_x APP_NAME[] = _T("TestRtf");
const char_x RTF_FILE[] = _T("rtf.rtf");

#include "document.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam);
void Main()
{
	HMODULE hModule = LoadLibraryA("E:\\games\\majiang\\server\\Debug_Win32_v140\\GamePrivateService.dll");
	Win32::DebugFormatLastError();

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

class DocSource : public IDocSource
{
public:
	textformat_t GetDefFormat() const
	{
		return textformat_t();
	}
	doc_source_t GetDocSource() const
	{
		return { m_text.length(), m_text.buffer() };
	}

	textw m_text;

};

CScriptEngine engine;
CScriptDocument doc;
int iBaseY = 0;
DocSource source;
DocTextObject dto;
void OnCreate(HWND hWnd)
{
	//::CreateCaret(hWnd, NULL, 1, 22);
	//::SetCaretPos(10, 10);
	//::ShowCaret(hWnd);

	//dto.SetText(L"𪚥𪚥ยิ้ยิ้ABCتىلى");
	//dto.SetText(L"𪚥𪚥𪚥ยิ้ยิ้تىلىABCہاں");
	uint_32 colors[] = { Colors::Red, Colors::Green, Colors::Blue, Colors::Purple };
	//char_16 chs[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ一二三四五六七八九十";
	//char_16 chs[] = L"一二三四五六七八九十ABCD EFGHI𪚥𪚥𪚥ยิ้ยิ้אאאא 1234 דדד לל شەھىرىدە تەكشۈرۈپ 123456 تەتقىق قىلدى";
    char_16 chs[] = L"一二三‮四五六七八九十ABCD EFGHI𪚥𪚥𪚥ยิ้ยิ้אאאא 1234 דדד לל شەھىرىدە تەكشۈرۈپ 123456 تەتقىق قىلدى";
	//char_16 chs[] = L"一二三四五六七八九十ABCDEFGHI";
	//char_16 chs[] = L"abcd efg להגדיל את המונה על ידי אחד";
	textw text;
	for(int_x cnt = 0; cnt < 1000 * 2; ++cnt)
	{
		text.append(chs[cnt % (arraysize(chs) - 1)]);
	}

	//text = L"تىلى تىلى تىلى تىلى تىلىشۆھرەت زاكىر ئۈرۈمچى شەھىرىدە تەكشۈرۈپ تەتقىق قىلدى";
	//text = L"abc def ghi jkl mno pqr stu vwx yz";
	//text = L"abcdefghijklmnopqrstuvwxyz";
	//text = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	//text = L"ABCD许许多多";
	//text = L"you say אאאא דדד לל   just come on.";
	//text = L"you say אאאא דדד לל אאאא דדד לל 一二三四just come on.";
	//text = L"אאאא 1234 דדד לל";
	//text = L"تىلىشۆھرەت زاكىر ئۈرۈمچى شەھىرىدە تەكشۈرۈپ 123456 تەتقىق قىلدى";
	//text = L"一二三四五六七八九十ABCD EFGHI𪚥𪚥𪚥ยิ้ยิ้تىلىشۆھرەت زاكىر ئۈرۈمچى شەھىرىدە تەكشۈرۈپ 123456 تەتقىق قىلدى";
	//dto.SetText(L"ษาไทยรอยยิ้มนักสู้ กเสียก่อน한국어조선말ئۇيغۇر تىلى𪚥𪚥𪚥𪚥𪚥");
	source.m_text = text;
	dto.Initialize(&source);
	dto.SetText(text);
	dto.Analyse();
	int_x step = 3;
	for(int_x cnt = 0; cnt < (dto.GetClusterCount() / step) * step; cnt += step)
	{
		dto.SetColor({ cnt, step }, colors[cnt % arraysize(colors)]);
	}

	dto.Slice();
	dto.Shape();

	//CRtfParser rtf;
	//CFileStream sfs(RTF_FILE, StreamModeRead);
	//int_x iSize = sfs.GetFileSize();
	//texta text(iSize + 1);
	//text.resize(iSize);
	//sfs.Read((byte_t *)text.buffer(), iSize);
	//rtf.LoadRtf(&doc, text, text + text.length());
	//doc.Generate(&engine);
}

void OnDestroy(HWND hWnd)
{
	doc.Destroy();
	engine.Destroy();
	dto.Destroy();

	PostQuitMessage(0);
}

int_x frameSize = 60;
int_x layoutStart = 100;

void OnSize(HWND hWnd)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	doc.GenerateLine(&engine, rc.right);

	dto.Layout(layoutStart, { 0, 0, rc.right - frameSize * 2, rc.bottom }, wrapmode_char);

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
	MoveToEx(hdc2, frameSize, frameSize, nullptr);
	LineTo(hdc2, frameSize, rect.bottom() - frameSize);

	MoveToEx(hdc2, rect.right() - frameSize, frameSize, nullptr);
	LineTo(hdc2, rect.right() - frameSize, rect.bottom() - frameSize);

	MoveToEx(hdc2, frameSize, frameSize, nullptr);
	LineTo(hdc2, rect.right() - frameSize, frameSize);

	MoveToEx(hdc2, frameSize, rect.bottom() - frameSize, nullptr);
	LineTo(hdc2, rect.right() - frameSize, rect.bottom() - frameSize);

	//doc.Draw(&engine, hdc2, 0, rect.y - iBaseY, rect.w, rect.h);
	dto.Draw(hdc2, frameSize, frameSize, { frameSize, frameSize, rc.right - frameSize * 2, rc.bottom - frameSize * 2});

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
