#include "stdafx.h"
#include "BaseInc.h"
//int WINAPI _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#include "CMainWindow.h"

#include <memory>
#include <locale.h>
int Main()
{
	setlocale(0, nullptr);
	CWin32App app;
	app.Create2DDevice(Device2DTypeGdip);
	//app.Create2DDevice(Device2DTypeDirect2D);

	CMainWindow wndMain;
	wndMain.Show(ShowModeNormal, HostInitPosCenterScreen);
	return (int)GetApp()->Run(0, AppRunLoop);
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
#ifdef _DEBUG
	AllocConsole();
#endif
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	int iRet = 0;
#ifdef _DEBUG
	_CrtMemState stateOld, stateNew, stateDiff;
	_CrtMemCheckpoint(&stateOld);
	//try
	{
		iRet = Main();
	}
	//catch(CException & exp)
	//{
	//	_CrtDbgReportW(_CRT_ERROR, 0, 0, 0, L"%s\n%s", exp.GetName(), exp.GetWhat());
	//}
	_CrtMemCheckpoint(&stateNew);
	if(_CrtMemDifference(&stateDiff, &stateOld, &stateNew))
		_CrtMemDumpAllObjectsSince(&stateDiff);
#else
	iRet = Main();
#endif 
	CoUninitialize();
	return iRet;
}
