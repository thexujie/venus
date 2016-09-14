#include "stdafx.h"
#include "ResTools.h"

VENUS_BEG

static const char_16 RES_TYPES[ResTypeCount][16] = {L"FILE"};

class CResFile : public ObjectT<IResFile>
{
public:
	CResFile(ResTypeE eType, int_x iId) :m_pHandle(nullptr)
	{
		HRSRC hRes = FindResourceW(NULL, MAKEINTRESOURCE(iId), RES_TYPES[eType]);
		if(hRes)
		{
			HGLOBAL hData = LoadResource(NULL, hRes);
			if(hData)
			{
				m_pData = (const byte_t *)LockResource(hData);
				m_iSize = SizeofResource(NULL, hRes);
				m_pHandle = (void *)hData;
			}
		}
	}
	~CResFile()
	{
		HGLOBAL hData = (HANDLE)m_pHandle;
		UnlockResource(hData);
		FreeResource(hData);
	}


	int_x GetSize() const
	{
		return m_iSize;
	}

	const byte_t * GetBuffer() const
	{
		return m_pData;
	}

public:
	void * m_pHandle;
	const byte_t * m_pData;
	int_x m_iSize;
};

IResFile * Resources::GetFile(int_x iId)
{
	return new CResFile(ResTypeFile, iId);
}

int_x Resources::GetText(int_x iId, char_16 * szText, int_x iSize)
{
	int_32 iLen = LoadStringW(NULL, (uint_32)iId, szText, (int_32)iSize);
	return iLen;
}

VENUS_END
