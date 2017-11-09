#include "stdafx.h"
#include "support.h"

VENUS_BEG

static thread_local IApp * gApp = nullptr;

CORE_API IApp & App()
{
	if(!gApp)
		throw exp_nullptr();
	return *gApp;
}

IApp * GetApp()
{
	return gApp;
}

IApp * SetApp(IApp * pApp)
{
	gApp = pApp;
	return pApp;
}

void log(int_x iLevel, const char_16 * szFormat, ...)
{
#ifndef _DEBUG
	if(iLevel == 0)
		return;
#endif
	va_list pArgs = nullptr;
	va_start(pArgs, szFormat);
	App().StdIO().out(iLevel, szFormat, pArgs);
	va_end(pArgs);
}

void log(int_x iLevel, const char_8 * szFormat, ...)
{
#ifndef _DEBUG
	if(iLevel == 0)
		return;
#endif
	va_list pArgs = nullptr;
	va_start(pArgs, szFormat);
	App().StdIO().out(iLevel, szFormat, pArgs);
	va_end(pArgs);
}

#ifdef _DEBUG
void log0(const char_16 * szFormat, ...)
{
	va_list pArgs = nullptr;
	va_start(pArgs, szFormat);
	App().StdIO().out(0, szFormat, pArgs);
	va_end(pArgs);
}

void log0(const char_8 * szFormat, ...)
{
	va_list pArgs = nullptr;
	va_start(pArgs, szFormat);
	App().StdIO().out(0, szFormat, pArgs);
	va_end(pArgs);
}

#endif // _DEBUG

void log1(const char_16 * szFormat, ...)
{
	va_list pArgs = nullptr;
	va_start(pArgs, szFormat);
	App().StdIO().out(1, szFormat, pArgs);
	va_end(pArgs);
}

void log1(const char_8 * szFormat, ...)
{
	va_list pArgs = nullptr;
	va_start(pArgs, szFormat);
	App().StdIO().out(1, szFormat, pArgs);
	va_end(pArgs);
}
void log2(const char_16 * szFormat, ...)
{
	va_list pArgs = nullptr;
	va_start(pArgs, szFormat);
	App().StdIO().out(2, szFormat, pArgs);
	va_end(pArgs);
}

void log2(const char_8 * szFormat, ...)
{
	va_list pArgs = nullptr;
	va_start(pArgs, szFormat);
	App().StdIO().out(2, szFormat, pArgs);
	va_end(pArgs);
}

VENUS_END
