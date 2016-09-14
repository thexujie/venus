#include "stdafx.h"
#include "Resources.h"


static const char_16 RES_TYPES[ResTypeCount][16] = {L"FILE"};
IResFile::IResFile(ResTypeE eType, int_x iId)
{
	HRSRC hRes = FindResourceW(NULL, MAKEINTRESOURCE(iId), RES_TYPES[eType]);
	if(hRes)
	{
		HGLOBAL hData = LoadResource(NULL, hRes);
		if(hData)
		{
			m_pData = (const byte_t *)LockResource(hData);
			m_iSize = SizeofResource(NULL, hRes);
		}
	}
}

IResFile::~IResFile()
{
}

IResFile * Resources::GetFile(int_x iId)
{
	return new IResFile(ResTypeFile, iId);
}
