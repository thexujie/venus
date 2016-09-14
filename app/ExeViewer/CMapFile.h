#pragma once

#include "BaseInc.h"

class CMapFileView
{
public:
	CMapFileView();
	~CMapFileView();

public:
	void Attach(byte_t * pMapView, int_x iSize);
	void Detach();

	void UnMap();

	bool ReadFull(int_x iOffset, void * pBuffer, int_x iSize);
	bool ReadText(int_x iOffset, char_8 * szText, int_x iSize);

private:
	byte_t * m_pMapView;
	int_x m_iSze;
};

class CMapFile
{
public:
	CMapFile(const char_16 * szFile, 
		DWORD dwAccess = GENERIC_READ, 
		DWORD dwShareMode = FILE_SHARE_READ, 
		DWORD dwOpenMode = OPEN_EXISTING,
		DWORD dwDisposition = 0);
	~CMapFile();

public:
	HANDLE GetHandle() { return m_hFile;}
	int_x GetSize() const;
	HANDLE GetFile() const;
	HANDLE GetMapFile() const;

	bool MapView(CMapFileView & mapView, DWORD dwAccess = FILE_MAP_READ);

private:
	HANDLE m_hFile;
	HANDLE m_hMapFile;
	int_x m_iSize;
};
