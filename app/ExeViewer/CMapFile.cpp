#include "stdafx.h"
#include "CMapFile.h"

CMapFile::CMapFile(const char_16 * szFile, 
	DWORD dwAccess/* = GENERIC_READ*/, 
	DWORD dwShareMode/* = FILE_SHARE_READ*/, 
	DWORD dwOpenMode/* = OPEN_EXISTING*/,
	DWORD dwDisposition/* = 0*/) 
	: m_hFile(NULL), m_hMapFile(NULL), m_iSize(0)
{
	m_hFile = CreateFileW(szFile, dwAccess, dwShareMode, NULL, dwOpenMode, dwDisposition, 0);
	if(m_hFile != INVALID_HANDLE_VALUE)
	{
		m_hMapFile = CreateFileMappingW(m_hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		if(!m_hMapFile)
		{
			CloseHandle(m_hFile);
			m_hFile = NULL;
		}
		// 返回的句柄属于一个现成的文件映射对象，在这种情况下，文件映射的长度就是现有对象的长度，而不是这个函数指定的尺寸。
		else if(GetLastError() == ERROR_ALREADY_EXISTS)
		{
			CloseHandle(m_hMapFile);
			CloseHandle(m_hFile);
			m_hFile = m_hMapFile = NULL;
		}
		else
		{
			LARGE_INTEGER lint = {0};
			::GetFileSizeEx(m_hFile, &lint);
#ifdef BIT_32
			if(lint.HighPart)
			{
				CloseHandle(m_hMapFile);
				CloseHandle(m_hFile);
				m_hFile = m_hMapFile = NULL;
				//Printf(L"文件大小超过 %d, 现在还不支持!\n", I32_MAX);
				throw exp_not_supported();
			}
#endif
			m_iSize = (int_x)lint.QuadPart;
		}
	}
	else
		m_hFile = NULL;
}

CMapFile::~CMapFile()
{
	if(m_hMapFile)
		CloseHandle(m_hMapFile);
	if(m_hFile)
		CloseHandle(m_hFile);

	m_hFile = m_hMapFile = NULL;
}

bool CMapFile::MapView(CMapFileView & mapView, DWORD dwAccess/* = FILE_MAP_READ*/)
{
	if(m_hMapFile)
	{
		void * pView = MapViewOfFile(m_hMapFile, dwAccess, 0, 0, 0);
		if(pView)
			mapView.Attach((byte_t *)pView, m_iSize);
		return true;
	}
	else
		return false;
}

int_x CMapFile::GetSize() const
{
	return m_iSize;
}

HANDLE CMapFile::GetFile() const
{
	return m_hFile;
}

HANDLE CMapFile::GetMapFile() const
{
	return m_hMapFile;
}


CMapFileView::CMapFileView() : m_pMapView(nullptr), m_iSze(0)
{

}

CMapFileView::~CMapFileView()
{

}

void CMapFileView::Attach(byte_t * pMapView, int_x iSize)
{
	UnMap();
	if(iSize <= 0)
		throw exp_illegal_argument();

	m_pMapView = pMapView;
	m_iSze = iSize;
}

void CMapFileView::Detach()
{
	m_pMapView = nullptr;
	m_iSze = 0;
}

void CMapFileView::UnMap()
{
	if(m_pMapView)
		UnmapViewOfFile(m_pMapView);
	m_pMapView = nullptr;
	m_iSze = 0;
}

bool CMapFileView::ReadFull(int_x iOffset, void * pBuffer, int_x iSize)
{
	if(iOffset < 0 || iSize <= 0 || iOffset + iSize > m_iSze)
		return false;
	else
	{
		buffcpy8(pBuffer, iSize, m_pMapView + iOffset, iSize);
		return true;
	}
}

bool CMapFileView::ReadText(int_x iOffset, char_8 * szText, int_x iSize)
{
	if(iOffset < 0 || iSize <= 0 || iOffset > m_iSze || !szText)
		return false;
	else
	{
		char_8 * pText = (char_8 *)(m_pMapView + iOffset);
		int_x iMax = m_iSze - iOffset;
		while(iMax-- && --iSize && *pText)
			*szText++ = *pText++;

		if(iSize)
			*szText = 0;
		else
			szText[0] = 0;
		return true;
	}
}
