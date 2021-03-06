#include "stdafx.h"
#include "BaseInc.h"
#include "CUIDevelopWindow.h"
#include "resource.h"


void Test()
{
	CWin32App app;
	app.Create2DDevice(Device2DTypeGdip);

	CUIDevelopWindow wnd;
	wnd.Show(ShowModeNormal, HostInitPosCenterScreen);
	GetApp()->Run(wnd.GetFormId(), AppRunDialog);
}

int _tmain(int argc, _TCHAR* argv[])
{
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
