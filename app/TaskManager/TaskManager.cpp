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

Device2DTypeE g_iDevice2DType = Device2DTypeUnknown;

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


namespace test
{
	
}

bool Test()
{
	suid uid(L"233B3C25-D116-488B-BE5D-1C1A5E9913DA", -1);
	suid guid0(L"233B3C25D116488BBE5D1C1A5E9913DA", -1);
	suid guid1(L"{233B3C25-D116-488B-BE5D-1C1A5E9913DA}", -1);
	suid guid2(L"0x233b3c25, 0xd116, 0x488b, 0xbe, 0x5d, 0x1c, 0x1a, 0x5e, 0x99, 0x13, 0xda", -1);
	suid guid3(L"{0x233b3c25, 0xd116, 0x488b, {0xbe, 0x5d, 0x1c, 0x1a, 0x5e,   0x99, 0x13, 0xda}}", -1);
	textw guid4 = uid.to_text(suid::n);
	textw guid5 = uid.to_text(suid::d);
	textw guid6 = uid.to_text(suid::b);
	textw guid7 = uid.to_text(suid::p);
	textw guid8 = uid.to_text(suid::x);

	textw guid9 = uid.to_text(suid::N);
	textw guid10 = uid.to_text(suid::D);
	textw guid11 = uid.to_text(suid::B);
	textw guid12 = uid.to_text(suid::P);
	textw guid13 = uid.to_text(suid::X);


	suid guid101 = suid(guid9);

	verify(uid == suid(guid9));
	verify(uid == suid(guid10));
	verify(uid == suid(guid11));
	verify(uid == suid(guid12));
	verify(uid == suid(guid13));

	CoInitialize(nullptr);
	CWin32App app;
	if(g_iDevice2DType != Device2DTypeUnknown)
		app.Create2DDevice(g_iDevice2DType);
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
		g_iDevice2DType = Device2DTypeUnknown;
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
