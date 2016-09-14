#include "stdafx.h"
#include "GdipModule.h"

using namespace Gdiplus;
using namespace DllExports;

#include <GdiPlus.h>

#ifdef WIN32_DEVICE_GDIP

VENUS_BEG

GdipModule::GdipModule():
	m_hModule(NULL)
{

}

GdipModule::~GdipModule()
{
}


err_t GdipModule::Initialize()
{
	if(m_hModule)
		return err_ok;

	//typedef Status (WINAPI * GdiplusStartupFunT)(
	//	OUT ULONG_PTR *token,
	//	const GdiplusStartupInput *input,
	//	OUT GdiplusStartupOutput *output);

	typedef decltype(GdiplusStartup) * GdiplusStartupFunT;
	GdiplusStartupFunT GdiplusStartupFun = (GdiplusStartupFunT)GetProcAddress(m_hModule, "GdiplusStartup");
}

VENUS_END

#endif //WIN32_DEVICE_GDIP
