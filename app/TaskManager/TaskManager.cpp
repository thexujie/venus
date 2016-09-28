#include "stdafx.h"
#include "BaseInc.h"

#include "CMainWindow.h"
#include <shellapi.h>

void RestartApp()
{
	char_16 szPath[256];
	CProcess process;
	if(process.Open(GetCurrentProcessId(), PROCESS_QUERY_INFORMATION | PROCESS_VM_READ).ok())
	{
		process.GetFilePath(szPath, 256);
		SHELLEXECUTEINFOW sheinfo = {sizeof(SHELLEXECUTEINFOW)};
		sheinfo.lpVerb = L"runas";
		sheinfo.lpFile = szPath;
		sheinfo.nShow = SW_SHOWNORMAL;
		if(!ShellExecuteExW(&sheinfo))
		{
			MessageBoxW(NULL, L"无法重新启动。", L"错误", MB_ICONERROR);
		}

	}
}

Device2DTypeE g_iDevice2DType = Device2DTypeGdi;

class BaseT
{
public:
	virtual int fun(int a, int b)
	{
		return a + b;
	}
	virtual int fun2(int a, int b)
	{
		return a + b + 20;
	}
};

class Base2T
{
public:
	virtual int fun222(int a, int b)
	{
		return a + b;
	}
	virtual int fun2222(int a, int b)
	{
		return a + b + 20;
	}
};


class DerivedT : virtual public BaseT, virtual public Base2T
{
public:
	virtual int fun(int a, int b)
	{
		return a + b + 20;
	}
	virtual int fun3(int a, int b)
	{
		return a - b;
	}

	int fun4(int a, int b)
	{
		return a * b;
	}

};

class Derived2T : public BaseT
{
public:
	virtual int fun(int a, int b)
	{
		return a + b + 20;
	}
	virtual int fun3(int a, int b)
	{
		return a - b;
	}

	int fun4(int a, int b)
	{
		return a * b;
	}

};

bool Test()
{
	CoInitialize(nullptr);

	CWin32App app;
	I2DDevice * p2DDevice = Win32::Create2DDevice(Device2DTypeUnknown);
	Set2DDevice(p2DDevice);
	SafeRelease(p2DDevice);
	g_iDevice2DType = Device2DTypeUnknown;
	CMainWindow window;
	CUILoaderXml loader;
	loader.Load(L"TaskManager.xml", &window);
	window.Show(ShowModeNormal, HostInitPosCenterScreen);
	switch(GetApp()->Run(window.GetFormId(), AppRunDialog))
	{
	case 100:
		g_iDevice2DType = Device2DTypeGdi;
		break;
	case 101:
		g_iDevice2DType = Device2DTypeGdip;
		break;
	case 102:
		g_iDevice2DType = Device2DTypeDirect2D;
		break;
	case 103:
		RestartApp();
		break;
	default:
		break;
	}
	bool bRet = g_iDevice2DType != Device2DTypeUnknown;
	CoUninitialize();
	return bRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nShowCmd)
{
#ifdef _DEBUG
	AllocConsole();
#endif

#ifdef _DEBUG
	_CrtMemState stateOld, stateNew, stateDiff;
	_CrtMemCheckpoint(&stateOld);
	//try
	{
		while(Test())
		{

		}
	}
	//catch(CException & exp)
	//{
	//	_CrtDbgReportW(_CRT_ERROR, 0, 0, 0, L"%s\n%s", exp.GetName(), exp.GetWhat());
	//}
	_CrtMemCheckpoint(&stateNew);
	if(_CrtMemDifference(&stateDiff, &stateOld, &stateNew))
		_CrtMemDumpAllObjectsSince(&stateDiff);
#else
	Test();
#endif 
	return 0;
}
