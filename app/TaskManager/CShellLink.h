#pragma once

#include "BaseInc.h"

#include <ShObjIdl.h>

class CShellLink
{
public:
	CShellLink();
	~CShellLink();

	bool Load(const char_x * szFile);
	bool GetLinkFile(char_x * szFile, int_x iSize);
	bool GetIcon(char_x * szFile, int_x iSize, int_x & iIndex);

	IShellLinkW * m_pShellLink;
	IPersistFile * m_pPersistFile;
};


class CShObj
{
public:
	CShObj()
	{
		CoInitializeEx(NULL, 0);
	}

	~CShObj()
	{
		CoUninitialize();
	}
};