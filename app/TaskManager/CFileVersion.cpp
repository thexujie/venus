#include "stdafx.h"
#include "CFileVersion.h"

#pragma comment(lib, "version.lib")

static const char_16 VER_QUERY[][20] = 
{
	L"Comments",
	L"InternalName",
	L"ProductName",
	L"CompanyName",
	L"LegalCopyright",
	L"ProductVersion",
	L"FileDescription",
	L"LegalTrademarks",
	L"PrivateBuild",
	L"FileVersion",
	L"OriginalFilename",
	L"SpecialBuild",
};
CFileVersion::CFileVersion(const char_16 * szFile)
	: m_pBlock(nullptr)
{
	DWORD dwHandle = 0;
	m_uiBlockSize = (uint_32)GetFileVersionInfoSizeW(szFile, &dwHandle);
	if(m_uiBlockSize)
	{
		m_pBlock = new byte_t[m_uiBlockSize];
		Assert(m_pBlock);
		if(!GetFileVersionInfoW(szFile, 0, (DWORD)m_uiBlockSize, (VOID *)m_pBlock))
		{
			SafeDelete(m_pBlock);
			m_uiBlockSize = 0;
		}
	}
}

CFileVersion::~CFileVersion()
{
	SafeDelete(m_pBlock);
	m_uiBlockSize = 0;
}

bool CFileVersion::IsValid() const
{
	return m_pBlock != nullptr;
}

bool CFileVersion::GetLanguageAndCodePage(int_x & iLanguage, int_x & iCodePage) const
{
	ConfirmValid();

	struct TranslationT 
	{
		int_16 iLanguage;
		int_16 iCodePage;
	};

	TranslationT * pTrans = nullptr;
	UINT uiOtherSize = 0;
	if(VerQueryValueW(m_pBlock, L"\\VarFileInfo\\Translation", (VOID **)&pTrans, &uiOtherSize))
	{
		iLanguage = pTrans->iLanguage;
		iCodePage = pTrans->iCodePage;
		return true;
	}
	else
	{
		iLanguage = iCodePage = 0;
		return false;
	}
}

bool CFileVersion::QueryVerInfo(VerQuery eQuery, char_16 * szText, int_x iSize) const
{
	ConfirmValid();
	int_x iLanguage = 0, iCodePage = 0;
	GetLanguageAndCodePage(iLanguage, iCodePage);
	char_16 szQuery[MAX_PATH];
	textformat(szQuery, MAX_PATH, L"StringFileInfo\\%04x%04x\\%s", iLanguage, iCodePage, VER_QUERY[eQuery]);

	char_16 * pTemp = nullptr;
	UINT uiLen = 0;
	if(VerQueryValueW(m_pBlock, szQuery, (VOID **)&pTemp, &uiLen))
	{
		textcpy(szText, iSize, pTemp, uiLen);
		return true;
	}
	else
	{
		textempty(szText, iSize);
		return false;
	}
}

bool CFileVersion::GetDescription(char_16 * szText, int_x iSize) const
{
	return QueryVerInfo(VerQueryFileDescription, szText, iSize);
}

void CFileVersion::ConfirmValid() const
{
	if(!m_pBlock)
		throw exp_bad_state();
}

