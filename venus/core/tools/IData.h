#pragma once

VENUS_BEG

class CORE_API IData
{
public:
	virtual ~IData() {}
	virtual int_x GetType() const = 0;

	// 文本。
	virtual int_x GetText8Length() const = 0;
	virtual int_x GetText16Length() const = 0;
	virtual int_x GetText8(char_8 * szText, int_x iSize) const = 0;
	virtual int_x GetText16(char_16 * szText, int_x iSize) const = 0;
	
	// 文件。
	virtual int_x GetFileNameCount() const = 0;
	virtual int_x GetFileName8(int_x iIndex, char_8 * szText, int_x iSize) const = 0;
	virtual int_x GetFileName16(int_x iIndex, char_16 * szText, int_x iSize) const = 0;

	virtual IObject * GetDataObject(int_x iIndex) const = 0;

	virtual void IterReset() = 0;
	virtual bool IterNext() = 0;
};

VENUS_END
