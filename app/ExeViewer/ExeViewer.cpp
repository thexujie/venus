#include "stdafx.h"
#include "BaseInc.h"
#include "CMainWindow.h"
#include "resource.h"

void Test()
{
	version_t ver = Win32::GetVersion();
	if(ver > winver_windows_xp_x86)
		ver.major = 1;
	CWin32App app;
	//app.Create2DDevice(Device2DTypeGdip);
	app.Create2DDevice(Device2DTypeDirect2D);

	CMainWindow wnd;

	IResFile * pFile = Resources::GetFile(IDF_UI);
	IUILoader::GetInstance()->Load(pFile->GetBuffer(), pFile->GetSize(), &wnd);
	SafeRelease(pFile);

	//wnd.SetExeFileName(L"C:\\WINDOWS\\system32\\d3d9.dll");
	wnd.SetExeFileName(L"C:\\WINDOWS\\SysWow64\\d3d9.dll");
	wnd.Show(ShowModeNormal, HostInitPosCenterScreen);
	GetApp()->Run(wnd.GetFormId(), AppRunDialog);
}

int f_sub(int a, int b)
{
	return a - b;
}
int f_add(int a, int b)
{
	_asm
	{
		mov eax, 1
		mov eax, 2
		mov eax, 3
		mov eax, 4
		mov eax, 5
		mov eax, 6
		mov eax, 7
		mov eax, 8
		mov eax, 11
		mov ebx, 2
		mov ecx, 3
	}
	return a + b;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
#ifdef _DEBUG
	AllocConsole();
#endif
	//_CrtDbgReportW(_CRT_WARN, 0, 0, 0, L"这是 _CRT_WARN 内容。\n");
	//_CrtDbgReportW(_CRT_ERROR, 0, 0, 0, L"这是 _CRT_ERROR 内容。\n");
	//_CrtDbgReportW(_CRT_ASSERT, 0, 0, 0, L"这是 _CRT_ASSERT 内容。\n");
	//_CrtDbgReportW(_CRT_ERRCNT, 0, 0, 0, L"这是 _CRT_WARN 内容。\n");
	//DEBUG_EVENT = OUTPUT_DEBUG_STRING_INFO
	//try
	//{
	//	throw CIndexOutOfBoundsException(L"索引超出范围！");
	//}
	//catch(CException & exp)
	//{
	//	_CrtDbgReportW(_CRT_ERROR, L"FileName", 99, L"Main.dll", L"%s\n%s", exp.GetName(), exp.GetWhat());
	//}
	//int_32 ix;
#ifdef _DEBUG
	_CrtMemState stateOld, stateNew, stateDiff;
	_CrtMemCheckpoint(&stateOld);
	Test();
	_CrtMemCheckpoint(&stateNew);
	if(_CrtMemDifference(&stateDiff, &stateOld, &stateNew))
		_CrtMemDumpAllObjectsSince(&stateDiff);
#else
	Test();
#endif 
	return 0;
}
