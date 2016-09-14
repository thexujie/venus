#include "stdafx.h"
#include "support.h"

VENUS_BEG

static int_x g_iApp = 0;

IApp * GetApp()
{
	IApp * pService = nullptr;
	if(g_iApp)
	{
		int_x iValue = tls_get(g_iApp);
		return (IApp *)(void *)iValue;
	}
	else
		return nullptr;
}

IApp * SetApp(IApp * pApp)
{
	if(pApp)
	{
		if(!g_iApp)
			g_iApp = tls_create();

		tls_set(g_iApp, (int_x)(void *)pApp);
	}
	else
	{
		if(g_iApp)
		{
			tls_set(g_iApp, 0);
			tls_destroy(g_iApp);
			g_iApp = 0;
		}
	}
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
		va_list pArgs = NULL;
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
		va_list pArgs = NULL;
		va_start(pArgs, szFormat);
		pStdOut->out(iLevel, szFormat, pArgs);
		va_end(pArgs);
	}
	else {}
}

void log0(const char_16 * szFormat, ...)
{
#ifdef _DEBUG
	IStdOut * pStdOut = GetStdOut();

	if(pStdOut)
	{
		va_list pArgs = NULL;
		va_start(pArgs, szFormat);
		pStdOut->out(0, szFormat, pArgs);
		va_end(pArgs);
	}
	else {}
#endif // _DEBUG
}

void log0(const char_8 * szFormat, ...)
{
#ifdef _DEBUG
	IStdOut * pStdOut = GetStdOut();
	if(pStdOut)
	{
		va_list pArgs = NULL;
		va_start(pArgs, szFormat);
		pStdOut->out(0, szFormat, pArgs);
		va_end(pArgs);
	}
	else {}
#endif // _DEBUG
}

void log1(const char_16 * szFormat, ...)
{
	IStdOut * pStdOut = GetStdOut();
	if(pStdOut)
	{
		va_list pArgs = NULL;
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
		va_list pArgs = NULL;
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
		va_list pArgs = NULL;
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
		va_list pArgs = NULL;
		va_start(pArgs, szFormat);
		pStdOut->out(2, szFormat, pArgs);
		va_end(pArgs);
	}
	else {}
}

VENUS_END
