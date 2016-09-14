#include "stdafx.h"
#include "CShellLink.h"

#define DEFINE_SHLGUID(name, l, w1, w2) DEFINE_GUID(name, l, w1, w2, 0xC0,0,0,0,0,0,0,0x46)

DEFINE_SHLGUID(CLSID_ShellDesktop,      0x00021400L, 0, 0); // 00021400-0000-0000-C000-000000000046
DEFINE_SHLGUID(CLSID_ShellLink,         0x00021401L, 0, 0); // 00021401-0000-0000-C000-000000000046

CShellLink::CShellLink():m_pPersistFile(nullptr), m_pShellLink(nullptr)
{
	HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (void**)&m_pShellLink);
	if(m_pShellLink)
		m_pShellLink->QueryInterface(IID_IPersistFile, (void **)&m_pPersistFile);
}

CShellLink::~CShellLink()
{
	SafeRelease(m_pPersistFile);
	SafeRelease(m_pShellLink);
}

bool CShellLink::Load(const char_x * szFile)
{
	if(!m_pShellLink)
		return false;

#ifdef _UNICODE
	HRESULT hr = m_pPersistFile->Load(szFile, STGM_READ);
#else
	FilePathW szFileW;
	AnsiToUnicode(szFile, -1, szFileW, MAX_FILE_PATH);
	HRESULT hr = m_pPersistFile->Load(szFileW, STGM_READ);
#endif
	return hr == S_OK;
}

bool CShellLink::GetLinkFile(char_x * szFile, int_x iSize)
{
	if(!m_pShellLink)
		return false;
#ifdef _UNICODE
	HRESULT hr = m_pShellLink->GetPath(szFile, (int_32)iSize, NULL, SLGP_SHORTPATH);
#else
	FilePathW szFileW;
	HRESULT hr = m_pShellLink->GetPath(szFileW, (int_32)MAX_FILE_PATH, NULL, SLGP_SHORTPATH);
	UnicodeToAnsi(szFileW, -1, szFile, iSize);
#endif
	return hr == S_OK;
}

bool CShellLink::GetIcon(char_x * szFile, int_x iSize, int_x & iIndex)
{
	if(!m_pShellLink)
		return false;

	int_32 iIcon = -1;
	HRESULT hr = m_pShellLink->GetIconLocation(szFile, (int_32)iSize, &iIcon);
	iIndex = iIcon;
	return hr == S_OK;
}
