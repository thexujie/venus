#include "stdafx.h"
#include "BaseInc.h"
#include "CUIDevelopWindow.h"
#include "resource.h"

#include <GdiPlus.h>
using namespace Gdiplus;
using namespace DllExports;


class GdipModule
{
public:
	GdipModule();
	~GdipModule();

	err_t Initialize();

private:
	HMODULE m_hModule;
};

GdipModule::GdipModule() :
	m_hModule(NULL)
{

}

GdipModule::~GdipModule()
{
}

int _vectorcall test()
{
}

#include <windows.h>
err_t GdipModule::Initialize()
{
	CModule gm;
	gm.Load(L"gdiplus.dll");

	auto pfn = gm.GetProc<decltype(GdiplusStartup)>("GdiplusStartup");
	auto pfn2 = gm.GetProc<decltype(test)>("GdiplusStartup");

	if(m_hModule)
		return err_ok;
	
	m_hModule = LoadLibraryW(L"gdiplus.dll");
	if(!m_hModule)
		return err_no_file;

	ULONG_PTR token;
	GdiplusStartupInput input;
	return pfn(&token, &input, nullptr);

	//typedef Status (WINAPI * GdiplusStartupFunT)(
	//	OUT ULONG_PTR *token,
	//	const GdiplusStartupInput *input,
	//	OUT GdiplusStartupOutput *output);

	//proc_ptr<decltype(GdiplusStartup)> pfnGdiplusStartup = GetProcAddress(m_hModule, "GdiplusStartup");

	//typedef decltype(GdiplusStartup) * GdiplusStartupFunT;
	//GdiplusStartupFunT pfnGdiplusStartupF = (GdiplusStartupFunT)GetProcAddress(m_hModule, "GdiplusStartup");
	//typedef decltype(GdiplusShutdown) * GdiplusShutdownFunT;
	//GdiplusStartupFunT pfnGdiplusStartupF = (GdiplusStartupFunT)GetProcAddress(m_hModule, "GdiplusStartup");
}

void Test()
{
	GdipModule gm;
	gm.Initialize();

	CWin32App app;
	//app.Create2DDevice(Device2DTypeDirect2D);
	app.Create2DDevice(Device2DTypeGdip);

	CUIDevelopWindow wnd;
	wnd.Show(ShowModeNormal, HostInitPosCenterScreen);
	GetApp()->Run(wnd.GetFormId(), AppRunDialog);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//_CrtDbgReportW(_CRT_WARN, 0, 0, 0, L"这是 _CRT_WARN 内容。\n");
	//_CrtDbgReportW(_CRT_ERROR, 0, 0, 0, L"这是 _CRT_ERROR 内容。\n");
	//_CrtDbgReportW(_CRT_ASSERT, 0, 0, 0, L"这是 _CRT_ASSERT 内容。\n");
	//_CrtDbgReportW(_CRT_ERRCNT, 0, 0, 0, L"这是 _CRT_WARN 内容。\n");
	//DEBUG_EVENT = OUTPUT_DEBUG_STRING_INFO
	//try
	//{
	//	throw exp_out_of_bound(L"索引超出范围！");
	//}
	//catch(exp_base & exp)
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
