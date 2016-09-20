#include "stdafx.h"
#include "BaseInc.h"
#include "CMainWindow.h"
#include "resource.h"

void Test()
{
	const char_8 text[] = "ABCD不是我，bcd挺好的";
	char_16 dst[64];
	err_t err = encodings::utf16.convert(encodings::ansi, text, textlen(text), dst, arraysize(dst), nullptr);

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
