#include "stdafx.h"
#include "CDropTarget.h"
#include "CGlobalData.h"
#include <ShellAPI.h>

VENUS_BEG

CDropTarget::CDropTarget()
{

}

CDropTarget::~CDropTarget()
{

}

HRESULT CDropTarget::QueryInterface(REFIID riid, void ** ppvObject)
{
	return E_NOTIMPL;
}

ULONG CDropTarget::AddRef()
{
	return E_NOTIMPL;
}

ULONG CDropTarget::Release()
{
	return E_NOTIMPL;
}

HRESULT CDropTarget::DragEnter(IDataObject * pDataObj, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
{
	if(m_data.Generate(pDataObj))
	{
		POINT point = {pt.x, pt.y};
		::ScreenToClient(m_hTargetWnd, &point);
		*pdwEffect = m_pControl->PreOnNcDragEnter(&m_data, pointix(point.x, point.y));
	}
	else
		*pdwEffect = DROPEFFECT_NONE;
	return S_OK;
}

HRESULT CDropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
{
	if(m_data.GetMediumCount())
	{
		POINT point = {pt.x, pt.y};
		::ScreenToClient(m_hTargetWnd, &point);
		*pdwEffect = m_pControl->PreOnNcDragMove(&m_data, pointix(point.x, point.y));
	}
	else
		*pdwEffect = DROPEFFECT_NONE;
	return S_OK;
}

HRESULT CDropTarget::DragLeave()
{
	if(m_data.GetMediumCount())
	{
		POINT point = {0, 0};
		::GetCursorPos(&point);
		::ScreenToClient(m_hTargetWnd, &point);
		m_pControl->PreOnNcDragLeave(&m_data, pointix(point.x, point.y));
	}
	m_data.ReleaseMediums();
	return S_OK;
}

HRESULT CDropTarget::Drop(IDataObject * pDataObj, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
{
	if(m_data.GetMediumCount())
	{
		POINT point = {pt.x, pt.y};
		::ScreenToClient(m_hTargetWnd, &point);
		*pdwEffect = m_pControl->PreOnNcDragDrop(&m_data, pointix(point.x, point.y));
		m_pControl->PreOnNcDragLeave(&m_data, pointix(point.x, point.y));
	}
	else
		*pdwEffect = DROPEFFECT_NONE;

	return S_OK;
}


bool CDropTarget::Register(HWND hWnd, IControl * pControl)
{
	OleInitialize(nullptr);
	HRESULT hr = ::RegisterDragDrop(hWnd, this);
	if(hr == S_OK)
	{
		m_pControl = pControl;
		m_hTargetWnd = hWnd;
		return true;
	}
	else
		return false;
}




CDropDragData::CDropDragData() : m_iDataCount(0), m_iIterIndex(0)
{
	buffset8(m_mediums, sizeof(DropDragMedium) * MAX_DRAG_DROP_DATA_COUNT, 0);
}

CDropDragData::~CDropDragData()
{
	ReleaseMediums();
}

int_x CDropDragData::GetType() const
{
	if(m_iIterIndex == m_iDataCount)
		return ClipboardDataTypeUnknown;
	else
		return m_mediums[m_iIterIndex].m_iType;
}

int_x CDropDragData::GetText8Length() const
{
	if(m_iIterIndex == m_iDataCount)
		return 0;
	const DropDragMedium & medium = m_mediums[m_iIterIndex];
	if(medium.m_iType != ClipboardDataTypeText8)
		return 0;
	else
	{
		CGlobalText text(medium.hGlobal);
		return text.GetText8Length();
	}
}
int_x CDropDragData::GetText16Length() const
{
	if(m_iIterIndex == m_iDataCount)
		return 0;
	const DropDragMedium & medium = m_mediums[m_iIterIndex];
	if(medium.m_iType != ClipboardDataTypeText16)
		return 0;
	else
	{
		CGlobalText text(medium.hGlobal);
		return text.GetText16Length();
	}
}

int_x CDropDragData::GetText8(char_8 * szText, int_x iSize) const
{
	textempty(szText, iSize);

	if(m_iIterIndex == m_iDataCount)
		return 0;
	const DropDragMedium & medium = m_mediums[m_iIterIndex];
	if(medium.m_iType != ClipboardDataTypeText8)
		return 0;
	else
	{
		CGlobalText text(medium.hGlobal);
		return text.GetText8(szText, iSize);
	}
}
int_x CDropDragData::GetText16(char_16 * szText, int_x iSize) const
{
	textempty(szText, iSize);

	if(m_iIterIndex == m_iDataCount)
		return 0;
	const DropDragMedium & medium = m_mediums[m_iIterIndex];
	if(medium.m_iType != ClipboardDataTypeText16)
		return 0;
	else
	{
		CGlobalText text(medium.hGlobal);
		return text.GetText16(szText, iSize);
	}
}

int_x CDropDragData::GetFileNameCount() const
{
	if(m_iIterIndex == m_iDataCount)
		return 0;
	const DropDragMedium & medium = m_mediums[m_iIterIndex];
	HDROP hDrop = (HDROP)medium.hGlobal;
	return (int_x)::DragQueryFileW(hDrop, 0xFFFFFFFF, nullptr, 0);
}

int_x CDropDragData::GetFileName8(int_x iIndex, char_8 * szText, int_x iSize) const
{
	textempty(szText, iSize);
	if(iIndex <0 || iIndex > I32_MAX)
		return 0;

	if(m_iIterIndex == m_iDataCount)
		return 0;
	const DropDragMedium & medium = m_mediums[m_iIterIndex];
	HDROP hDrop = (HDROP)medium.hGlobal;
	return (int_x)::DragQueryFileA(hDrop, (UINT)iIndex, szText, (uint_32)iSize);
}

int_x CDropDragData::GetFileName16(int_x iIndex, char_16 * szText, int_x iSize) const
{
	textempty(szText, iSize);
	if(iIndex <0 || iIndex > I32_MAX)
		return 0;
	if(m_iIterIndex == m_iDataCount)
		return 0;
	const DropDragMedium & medium = m_mediums[m_iIterIndex];
	HDROP hDrop = (HDROP)medium.hGlobal;
	return (int_x)::DragQueryFileW(hDrop, (UINT)iIndex, szText, (uint_32)iSize);
}

IObject * CDropDragData::GetDataObject(int_x iIndex) const
{
	return nullptr;
}

void CDropDragData::IterReset()
{
	m_iIterIndex = 0;
}

bool CDropDragData::IterNext()
{
	if(m_iIterIndex == m_iDataCount)
		return false;
	else
	{
		++m_iIterIndex;
		return true;
	}
}

int_x CDropDragData::Generate(IDataObject * pDataObj)
{
	ReleaseMediums();
	IEnumFORMATETC * pEnumFmt = NULL;
	HRESULT hr = pDataObj->EnumFormatEtc(DATADIR_GET, &pEnumFmt);
	if(hr == S_OK)
	{
		FORMATETC cFmt;
		ULONG Fetched = 0;

		pEnumFmt->Reset();
		HRESULT hr = S_OK;
		while(hr == S_OK)
		{
			DropDragMedium & medium = m_mediums[m_iDataCount];
			hr = pEnumFmt->Next(1, &cFmt, &Fetched);
			if(hr == S_OK)
			{
				/*if(medium.tymed == TYMED_FILE)
				{
				hr = pDataObj->GetData(&cFmt, &medium);
				if(hr == S_OK)
				{
				medium.m_iType = DataTypeFile;
				++m_iDataCount;
				}
				}
				else */if(cFmt.cfFormat == CF_UNICODETEXT)
				{
					hr = pDataObj->GetData(&cFmt, &medium);
					if(hr == S_OK)
					{
						medium.m_iType = ClipboardDataTypeText16;
						++m_iDataCount;
					}
				}
				else if(cFmt.cfFormat == CF_HDROP)
				{
					hr = pDataObj->GetData(&cFmt, &medium);
					if(hr == S_OK)
					{
						medium.m_iType = ClipboardDataTypeFile;
						++m_iDataCount;
					}
				}
			}
		}
	}
	return m_iDataCount;
}

int_x CDropDragData::GetMediumCount() const
{
	return m_iDataCount;
}

void CDropDragData::ReleaseMediums()
{
	for(int_x cnt = 0; cnt < m_iDataCount; ++cnt)
		ReleaseStgMedium(&m_mediums[cnt]);
	buffset8(m_mediums, sizeof(DropDragMedium)* MAX_DRAG_DROP_DATA_COUNT, 0);

	m_iDataCount = 0;
	m_iIterIndex = 0;
}

VENUS_END
