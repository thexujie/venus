#include "stdafx.h"
#include "CModule.h"

VENUS_BEG

CModule::CModule(): 
m_hModule(NULL)
{

}

CModule::CModule(const char_x * szFile):
m_hModule(NULL)
{
	Load(szFile);
}

CModule::~CModule()
{
	Free();
}

bool CModule::Load(const char_x * szFile)
{
	Free();
	if(szFile)
		m_hModule = ::LoadLibrary(szFile);
	return m_hModule != NULL;
}

void CModule::Free()
{
	if(m_hModule)
	{
		::FreeLibrary(m_hModule);
		m_hModule = NULL;
	}
}

void * CModule::_GetProc(const char_8 * szProc)
{
	if(m_hModule)
	{
		return (void *)::GetProcAddress(m_hModule, szProc);
	}
	else
		return nullptr;
}

bool CModule::IsLoaded() const
{
	return m_hModule != NULL;
}

void * CModule::_GetProc(const char_16 * szProc)
{
	char_8 szProcA[256];
	unicodetoansi(szProc, -1, szProcA, 256);
	if(m_hModule)
	{
		return (void *)::GetProcAddress(m_hModule, szProcA);
	}
	else
		return nullptr;
}

VENUS_END
