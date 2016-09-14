#pragma once

#include "oleidl.h"

VENUS_BEG

const int_x MAX_DRAG_DROP_DATA_COUNT = 32;

struct WIN32_API DropDragMedium : public STGMEDIUM
{
	int_x m_iType;
};

class WIN32_API CDropDragData : public IData
{
public:
	CDropDragData();
	~CDropDragData();

	int_x GetType() const;

	int_x GetText8Length() const;
	int_x GetText16Length() const;

	// 文本
	int_x GetText8(char_8 * szText, int_x iSize) const;
	int_x GetText16(char_16 * szText, int_x iSize) const;

	// 文件
	int_x GetFileNameCount() const;
	int_x GetFileName8(int_x iIndex, char_8 * szText, int_x iSize) const;
	int_x GetFileName16(int_x iIndex, char_16 * szText, int_x iSize) const;

	IObject * GetDataObject(int_x iIndex) const;

	void IterReset();
	bool IterNext();

public:
	int_x Generate(IDataObject * pDataObj);
	int_x GetMediumCount() const;
	void ReleaseMediums();
private:
	DropDragMedium m_mediums[MAX_DRAG_DROP_DATA_COUNT];
	int_x m_iDataCount;
	int_x m_iIterIndex;
};

class WIN32_API CDropTarget : public IDropTarget
{
public:
	CDropTarget();
	~CDropTarget();

	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObject);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	HRESULT STDMETHODCALLTYPE DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
	HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);
	HRESULT STDMETHODCALLTYPE DragLeave();
	HRESULT STDMETHODCALLTYPE Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect);

public:
	bool Register(HWND hWnd, IControl * pControl);
private:
	HWND m_hTargetWnd;
	IControl * m_pControl;
	CDropDragData m_data;
};

VENUS_END
