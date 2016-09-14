#pragma once

VENUS_BEG

// 仅仅是建议的大小
const int_x MAX_KEY_PATH = 512;

enum RegValueTypeE
{
	RegValueTypeInvalid,
	RegValueTypeText,
	RegValueTypeExpandText,
	RegValueTypeUint32,
};

struct WIN32_API regvalue_t
{
	regvalue_t():name(nullptr), type(RegValueTypeInvalid), value(0)
	{

	}
	regvalue_t(const char_16 * _name) :name(_name), type(RegValueTypeInvalid), value(0)
	{

	}

	regvalue_t(const char_16 * _name, const char_16 * _value, bool _expand = false)
	{
		name = _name;
		if(_expand)
			type = RegValueTypeExpandText;
		else
			type = RegValueTypeText;
		value_text = _value;
	}

	regvalue_t(const char_16 * _name, uint_32 _value)
	{
		name = _name;
		type = RegValueTypeUint32;
		value_uint32 = _value;
	}

	const char_16 * name;
	RegValueTypeE type;
	union
	{
		int_x value;
		const char_16 * value_text;
		uint_32 value_uint32;
	};
};

struct WIN32_API regitems_t
{
	const char_16 * name;
	regvalue_t value;
};

class WIN32_API CRegKey
{
public:
	CRegKey();
	CRegKey(HKEY hKey, const char_x * szKey);
	~CRegKey();

	winerr_t DoValueIter(int_x iIndex, char_x * szName, int_x iSize, int_x * piLength = NULL, FILETIME * pftLastWriteTime = NULL);
	/** 读取键值，负数表示发生错误。*/
	winerr_t FindValue(const char_x * szName, uint_32 * puiType = nullptr);

	winerr_t ReadValue(const char_x * szName, char_x * szValue, int_x iValueSize, int_x * piValueLength = nullptr);
	winerr_t SetText(const char_x * szName, const char_x * szValue, int_x iValueSize);
	winerr_t SetExpandText(const char_x * szName, const char_x * szValue, int_x iValueSize);
	winerr_t SetUInt32(const char_x * szName, uint_32 uiValue);

	winerr_t DeleteValue(const char_x * szName);
	
	winerr_t Open(HKEY hRoot, const char_x * szKey, uint_32 uiAccess = KEY_READ, bool bCreateIfNotExists = false);
	winerr_t Create(HKEY hRoot, const char_x * szKey, uint_32 uiAccess = KEY_READ | KEY_WRITE);
	void Close();

	operator HKEY() const { return m_hKey;}
	bool IsValid() const { return m_hKey != NULL; }


	winerr_t RegisterValue(const regvalue_t & value);
	winerr_t RegisterValues(const regvalue_t * pValues, int_x iCount);
	winerr_t RegisterItem(const regitems_t & key);
	winerr_t RegisterItems(const regitems_t * pKeys, int_x iCount);
protected:
	HKEY m_hKey;

public:
	static winerr_t DeleteTree(HKEY hRoot, const char_x * szKey);
};

VENUS_END
