#include "stdafx.h"
#include "support.h"

VENUS_BEG

static thread_local IApp * gApp = nullptr;

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
	IStdOut * pStdOut = GetStdOut();
	if(pStdOut)
	{
		va_list pArgs = nullptr;
		va_start(pArgs, szFormat);
		pStdOut->out(iLevel, szFormat, pArgs);
		va_end(pArgs);
	}
	else {}
}

void log(int_x iLevel, const char_8 * szFormat, ...)
{
#ifndef _DEBUG
	if(iLevel == 0)
		return;
#endif
	IStdOut * pStdOut = GetStdOut();
	if(pStdOut)
	{
		va_list pArgs = nullptr;
		va_start(pArgs, szFormat);
		pStdOut->out(iLevel, szFormat, pArgs);
		va_end(pArgs);
	}
	else {}
}

#ifdef _DEBUG
void log0(const char_16 * szFormat, ...)
{
	IStdOut * pStdOut = GetStdOut();

	if(pStdOut)
	{
		va_list pArgs = nullptr;
		va_start(pArgs, szFormat);
		pStdOut->out(0, szFormat, pArgs);
		va_end(pArgs);
	}
	else {}
}

void log0(const char_8 * szFormat, ...)
{
	IStdOut * pStdOut = GetStdOut();
	if(pStdOut)
	{
		va_list pArgs = nullptr;
		va_start(pArgs, szFormat);
		pStdOut->out(0, szFormat, pArgs);
		va_end(pArgs);
	}
	else {}
}

#endif // _DEBUG

void log1(const char_16 * szFormat, ...)
{
	IStdOut * pStdOut = GetStdOut();
	if(pStdOut)
	{
		va_list pArgs = nullptr;
		va_start(pArgs, szFormat);
		pStdOut->out(1, szFormat, pArgs);
		va_end(pArgs);
	}
	else {}
}

void log1(const char_8 * szFormat, ...)
{
	IStdOut * pStdOut = GetStdOut();
	if(pStdOut)
	{
		va_list pArgs = nullptr;
		va_start(pArgs, szFormat);
		pStdOut->out(1, szFormat, pArgs);
		va_end(pArgs);
	}
	else {}
}
void log2(const char_16 * szFormat, ...)
{
	IStdOut * pStdOut = GetStdOut();
	if(pStdOut)
	{
		va_list pArgs = nullptr;
		va_start(pArgs, szFormat);
		pStdOut->out(2, szFormat, pArgs);
		va_end(pArgs);
	}
	else {}
}

void log2(const char_8 * szFormat, ...)
{
	IStdOut * pStdOut = GetStdOut();
	if(pStdOut)
	{
		va_list pArgs = nullptr;
		va_start(pArgs, szFormat);
		pStdOut->out(2, szFormat, pArgs);
		va_end(pArgs);
	}
	else {}
}

VENUS_END
