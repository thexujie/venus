#include "stdafx.h"
#include "CRegKey.h"

VENUS_BEG

CRegKey::CRegKey() : m_hKey(NULL) {}

CRegKey::CRegKey(HKEY hKey, const char_x * szKey)
{
	Open(hKey, szKey);
}

CRegKey::~CRegKey()
{
	Close();
}

winerr_t CRegKey::DoValueIter(int_x iIndex, char_x * szName, int_x iSize, int_x * piLength, FILETIME * pftLastWriteTime)
{
	textempty(szName, iSize);
	if(!m_hKey)
		return E_HANDLE;

	DWORD dwType = 0;
	DWORD dwNameLength = (DWORD)iSize;
	winerr_t err = ::RegEnumValue(m_hKey, (DWORD)iIndex, szName, &dwNameLength, 0, &dwType, NULL, NULL);
	if(err.ok())
	{
		if(piLength)
			*piLength = (int_x)dwNameLength;
		return err;
	}
	else
	{
		if(piLength)
			*piLength = 0;
		return err;
	}
}

winerr_t CRegKey::FindValue(const char_x * szName, uint_32 * puiType)
{
	if(!m_hKey)
		return E_HANDLE;

	DWORD dwType = 0;
	winerr_t err = RegQueryValueEx(m_hKey, szName, NULL, &dwType, NULL, NULL);
	if(err != ERROR_SUCCESS)
	{
		if(puiType) *puiType = 0;
		return err;
	}
	else
	{
		if(puiType) *puiType = dwType;
		return err;
	}
}

winerr_t CRegKey::ReadValue(const char_x * szName, char_x * szValue, int_x iValueSize, int_x * piValueLength)
{
	if(piValueLength)
		*piValueLength = 0;

	if(!m_hKey)
	{
		textempty(szValue, iValueSize);
		return E_HANDLE;
	}
	DWORD dwType = 0;
	winerr_t err = RegQueryValueEx(m_hKey, szName, NULL, &dwType, NULL, NULL);
	if(!err.ok())
		return err;
	if(dwType != REG_SZ)
		return ERROR_INVALID_DATATYPE;

	DWORD dwcbValue = (DWORD)(iValueSize * sizeof(char_x));
	err = ::RegQueryValueEx(m_hKey, szName, NULL, &dwType, (BYTE *)szValue, &dwcbValue);
	if(err.ok())
	{
		if(piValueLength)
			*piValueLength = dwcbValue / sizeof(char_x);
		return err;
	}
	else
	{
		textempty(szValue, iValueSize);
		return err;
	}
}

winerr_t CRegKey::SetText(const char_x * szName, const char_x * szValue, int_x iValueSize)
{
	if(!m_hKey)
		return E_HANDLE;

	if(iValueSize < 0)
		iValueSize = textlen(szValue);

	DWORD dwcbValue = (DWORD)(iValueSize * sizeof(char_x));
	winerr_t err = ::RegSetValueEx(m_hKey, szName, 0, REG_SZ, (BYTE *)szValue, dwcbValue);
	return err;
}

winerr_t CRegKey::SetExpandText(const char_x * szName, const char_x * szValue, int_x iValueSize)
{
	if(!m_hKey)
		return E_HANDLE;

	if(iValueSize < 0)
		iValueSize = textlen(szValue);

	DWORD dwcbValue = (DWORD)(iValueSize * sizeof(char_x));
	winerr_t err = ::RegSetValueEx(m_hKey, szName, 0, REG_EXPAND_SZ, (BYTE *)szValue, dwcbValue);
	return err;
}

winerr_t CRegKey::SetUInt32(const char_x * szName, uint_32 uiValue)
{
	if(!m_hKey)
		return E_HANDLE;

	winerr_t err = ::RegSetValueEx(m_hKey, szName, 0, REG_DWORD, (BYTE *)&uiValue, 4);
	return err;
}

winerr_t CRegKey::DeleteValue(const char_x * szName)
{
	if(!m_hKey)
		return E_HANDLE;

	winerr_t err = ::RegDeleteValue(m_hKey, szName);
	return err;
}

winerr_t CRegKey::Open(HKEY hRoot, const char_x * szKey, uint_32 uiAccess, bool bCreateIfNotExists)
{
	winerr_t err = ::RegOpenKeyExW(hRoot, szKey, 0, uiAccess, &m_hKey);
	if(err == ERROR_FILE_NOT_FOUND && bCreateIfNotExists)
	{
		DWORD dwDispition = 0;
		err = RegCreateKeyEx(hRoot, szKey, NULL, NULL, REG_OPTION_NON_VOLATILE,
			uiAccess, NULL, &m_hKey, &dwDispition);
	}
	return err;
}

winerr_t CRegKey::Create(HKEY hRoot, const char_x * szKey, uint_32 uiAccess)
{
	Close();
	DWORD dwDispition = 0;
	winerr_t err = RegCreateKeyEx(hRoot, szKey, NULL, NULL, REG_OPTION_NON_VOLATILE,
		uiAccess, NULL, &m_hKey, &dwDispition);
	return err;
}

void CRegKey::Close()
{
	if(m_hKey)
	{
		RegCloseKey(m_hKey);
		m_hKey = NULL;
	}
}

winerr_t CRegKey::RegisterValue(const regvalue_t & value)
{
	switch(value.type)
	{
	case RegValueTypeText:
		SetText(value.name, value.value_text, -1);
		break;
	case RegValueTypeExpandText:
		SetExpandText(value.name, value.value_text, -1);
		break;
	case RegValueTypeUint32:
		SetUInt32(value.name, value.value_uint32);
		break;
	default:
		break;
	}
	return S_OK;
}

winerr_t CRegKey::RegisterValues(const regvalue_t * pValues, int_x iCount)
{
	for(int_x cnt = 0; cnt < iCount; ++cnt)
	{
		winerr_t err = RegisterValue(pValues[cnt]);
		if(err)
			return err;
	}
	return S_OK;
}

winerr_t CRegKey::RegisterItem(const regitems_t & key)
{
	winerr_t err;
	if(!key.name)
	{
		err = RegisterValue(key.value);
		if(err.failed())
			return err;
	}
	else
	{
		CRegKey rKey;
		err = rKey.Open(m_hKey, key.name, KEY_WRITE, true);
		if(err.failed())
			return err;

		err = rKey.RegisterValue(key.value);
		if(err.failed())
			return err;
	}
	return err;
}

winerr_t CRegKey::RegisterItems(const regitems_t * pKeys, int_x iCount)
{
	if(!pKeys || iCount < 0)
		return err_invalidarg;

	if(!m_hKey)
		return err_not_initialized;

	winerr_t err;
	for(int_x cnt = 0; cnt < iCount; ++cnt)
	{
		const regitems_t & key = pKeys[cnt];
		if(!key.name)
		{
			err = RegisterValue(key.value);
			if(err.failed())
				return err;
		}
		else
		{
			CRegKey rKey;
			err = rKey.Open(m_hKey, key.name, KEY_WRITE, true);
			if(err.failed())
				return err;

			err = rKey.RegisterValue(key.value);
			if(err.failed())
				return err;
		}
	}
	return S_OK;
}

winerr_t CRegKey::DeleteTree(HKEY hRoot, const char_x * szKey)
{
#if _WIN32_WINNT >= WINVER_VISTA
	winerr_t err = RegDeleteTree(hRoot, szKey);
	return err;
#else
	return E_FAIL;
#endif
}

VENUS_END
