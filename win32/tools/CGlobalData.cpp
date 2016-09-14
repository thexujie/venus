#include "stdafx.h"
#include "CGlobalData.h"

VENUS_BEG

CGlobalText::CGlobalText(HANDLE hGlobal) : m_hGlobal(hGlobal)
{

}
CGlobalText::~CGlobalText()
{

}

int_x CGlobalText::GetText8Length()
{
	CGlobalDataHandle handle(m_hGlobal);
	char_8 * pText = (char_8 *)handle.GetData();
	if(pText)
		return textlen(pText, handle.GetSize());
	else
		return 0;
}

int_x CGlobalText::GetText8(char_8 * szText, int_x iSize)
{
	CGlobalDataHandle handle(m_hGlobal);
	char_8 * pText = (char_8 *)handle.GetData();
	if(pText)
		return textcpy(szText, iSize, pText, handle.GetSize());
	else
		return 0;
}
int_x CGlobalText::GetText16Length()
{
	CGlobalDataHandle handle(m_hGlobal);
	char_16 * pText = (char_16 *)handle.GetData();
	if(pText)
		return textlen(pText, handle.GetSize());
	else
		return 0;
}

int_x CGlobalText::GetText16(char_16 * szText, int_x iSize)
{
	CGlobalDataHandle handle(m_hGlobal);
	char_16 * pText = (char_16 *)handle.GetData();
	if(pText)
		return textcpy(szText, iSize, pText, handle.GetSize() / 2);
	else
		return 0;
}



CGlobalDataHandle::CGlobalDataHandle(HGLOBAL hGlobal) : m_hGlobal(NULL), m_pData(nullptr)
{
	if(hGlobal)
	{
		m_pData = GlobalLock(hGlobal);
		if(m_pData)
			m_hGlobal = hGlobal;
	}
}
CGlobalDataHandle::~CGlobalDataHandle()
{
	if(m_pData && m_hGlobal)
		::GlobalUnlock(m_hGlobal);
	m_pData = nullptr;
	m_hGlobal = NULL;
}

void * CGlobalDataHandle::GetData()
{
	return m_pData;
}

int_x CGlobalDataHandle::GetSize()
{
	if(m_hGlobal)
		return ::GlobalSize(m_hGlobal);
	else
		return 0;
}

VENUS_END
