#pragma once

VENUS_BEG

class WIN32_API CGlobalText
{
public:
	CGlobalText(HANDLE hGlobal);
	~CGlobalText();

	int_x GetText8Length();
	int_x GetText8(char_8 * szText, int_x iSize);
	int_x GetText16Length();
	int_x GetText16(char_16 * szText, int_x iSize);

private:
	HGLOBAL m_hGlobal;
};

class WIN32_API CGlobalDataHandle
{
public:
	CGlobalDataHandle(HGLOBAL hGlobal);
	~CGlobalDataHandle();
	void * GetData();
	int_x GetSize();
protected:
	HGLOBAL m_hGlobal;
	void * m_pData;
};

VENUS_END
