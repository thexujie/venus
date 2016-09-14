#include "stdafx.h"
#include "CSHFileInfoBuffer.h"
#include "CShellItemImage.h"
#include <shellapi.h>
#include <shlobj.h>
#include <ShObjIdl.h>

VENUS_BEG

const char_16 CSHFileInfoBuffer_ShellFolder[] = L"{6D2D99B1-0DBB-4E14-B250-71655E4927F2}";

#if _WIN32_WINNT < WINVER_VISTA
HRESULT SHCreateItemFromParsingName(__in PCWSTR pszPath, __in_opt IBindCtx *pbc, __in REFIID riid, __deref_out void **ppv)
{
	IShellItem * psi = nullptr;
	LPITEMIDLIST pidl = SHSimpleIDListFromPath(pszPath);
	HRESULT hResult = SHCreateShellItem(nullptr, 0, pidl, &psi);
	if(SUCCEEDED(hResult))
		hResult = psi->QueryInterface(riid, ppv);
	SafeRelease(psi);
	return hResult;
}
#endif

CSHFileInfoBuffer::CSHFileInfoBuffer():m_pbctx(nullptr), m_pbctx_folder(nullptr)
{

}

CSHFileInfoBuffer::~CSHFileInfoBuffer()
{
	for(int_x cnt = 0, len = m_images.size(); cnt < len; ++cnt)
		SafeRelease<IWin32ShellItemImage>(m_images.at(cnt).value);
	m_images.clear();

	IBindCtx * pbctx = (IBindCtx *)m_pbctx;
	SafeRelease(pbctx); 
	pbctx = (IBindCtx *)m_pbctx_folder;
	SafeRelease(pbctx);
	m_pbctx = nullptr;
}

static const CLSID CLSID_Junction_Folder = {0xfc0a77e6, 0x9d70, 0x4258, {0x97, 0x83, 0x6d, 0xab, 0x1d, 0x0f, 0xe3, 0x1e}};
class CFileSysBindData : public IFileSystemBindData2
{
public:
	CFileSysBindData() : m_lRef(1), m_clsid(CLSID_Junction_Folder)
	{
		buffclr(m_fd);
		buffclr(m_liFileId);
	}

	// IUnknown
	IFACEMETHODIMP QueryInterface(REFIID riid, void ** ppv)
	{
		HRESULT hResult = S_OK;

		if(riid == IID_IFileSystemBindData)
			*ppv = static_cast<IFileSystemBindData *>(this);
		else if(riid == IID_IFileSystemBindData2)
			*ppv = static_cast<IFileSystemBindData2 *>(this);
		else if(riid == IID_IUnknown)
			*ppv = static_cast<IUnknown *>(this);
		else
		{
			*ppv = nullptr;
			hResult = E_NOINTERFACE;
		}
		if(hResult == S_OK)
			AddRef();
		return hResult;
	}

	IFACEMETHODIMP_(ULONG) AddRef()
	{
		return InterlockedIncrement(&m_lRef);
	}

	IFACEMETHODIMP_(ULONG) Release()
	{
		long cRef = InterlockedDecrement(&m_lRef);
		if(!cRef)
			delete this;
		return cRef;
	}

	// IFileSystemBindData
	IFACEMETHODIMP SetFindData(const WIN32_FIND_DATAW *pfd)
	{
		m_fd = *pfd;
		return S_OK;
	}

	IFACEMETHODIMP GetFindData(WIN32_FIND_DATAW *pfd)
	{
		*pfd = m_fd;
		return S_OK;
	}

	// IFileSystemBindData2
	IFACEMETHODIMP SetFileID(LARGE_INTEGER liFileID)
	{
		m_liFileId = liFileID;
		return S_OK;
	}

	IFACEMETHODIMP GetFileID(LARGE_INTEGER *pliFileID)
	{
		*pliFileID = m_liFileId;
		return S_OK;
	}

	IFACEMETHODIMP SetJunctionCLSID(REFCLSID clsid)
	{
		m_clsid = clsid;
		return S_OK;
	}

	IFACEMETHODIMP GetJunctionCLSID(CLSID *pclsid)
	{
		HRESULT hr;
		if(CLSID_Junction_Folder == m_clsid)
		{
			*pclsid = CLSID_NULL;
			hr = E_FAIL;
		}
		else
		{
			*pclsid = m_clsid;   // may be CLSID_NULL (no junction handler case)
			hr = S_OK;
		}
		return hr;
	}

private:
	long m_lRef;
	WIN32_FIND_DATAW m_fd;
	LARGE_INTEGER m_liFileId;
	CLSID m_clsid;
};

IWin32ShellItemImage * CSHFileInfoBuffer::GetShellIcon(const char_x * szPathOrExt)
{
	char_x szKeyPath[MAX_FILE_PATH] = {};
	enum CreateE
	{
		CreateNone,
		CreateExtension,
		CreateFolder,
	};

	CreateE eCreate = CreateNone;
	if(!szPathOrExt || !szPathOrExt[0])
	{
		textcpy(szKeyPath, MAX_FILE_PATH, L"file", 4);
		eCreate = CreateExtension;
	}
	else if(szPathOrExt[0] == L'/' || szPathOrExt[0] == L'\\')
	{
		textcpy(szKeyPath, MAX_FILE_PATH, CSHFileInfoBuffer_ShellFolder, -1);
		eCreate = CreateFolder;
	}
	else if(szPathOrExt[0] == L'.')
	{
		textcpy(szKeyPath, MAX_FILE_PATH, szPathOrExt, -1);
		eCreate = CreateExtension;
	}
	else
	{
		pathinfo_t info(szPathOrExt);
		textcpy(szKeyPath, MAX_FILE_PATH, szPathOrExt, -1);
		textrplch(szKeyPath, MAX_FILE_PATH, L'/', L'\\');
		if(!info.exists)
			eCreate = CreateExtension;
	}

	int_x iHash = hashtextx(szKeyPath, -1);
	IWin32ShellItemImage * pImage = nullptr;
	if(m_images.get_value(iHash, pImage))
		return pImage;

	IBindCtx * pbctx = nullptr;
	HRESULT hResult = S_OK;
	if(eCreate == CreateExtension)
	{
		if(!m_pbctx)
		{
			IBindCtx * pTemp = nullptr;
			BIND_OPTS bopt = {sizeof(BIND_OPTS)};
			bopt.dwTickCountDeadline = 0;
			bopt.grfFlags = 0;
			bopt.grfMode = STGM_CREATE;
			hResult = CreateBindCtx(0, &pTemp);
			hResult = pTemp->SetBindOptions(&bopt);
			m_pbctx = (void *)pTemp;
		}
		else {}
		pbctx = (IBindCtx *)m_pbctx;
	}
	else if(eCreate == CreateFolder)
	{
		if(!m_pbctx_folder)
		{
			IBindCtx * pTemp = nullptr;
			BIND_OPTS bopt = {sizeof(BIND_OPTS)};
			bopt.dwTickCountDeadline = 0;
			bopt.grfFlags = 0;
			bopt.grfMode = STGM_CREATE;
			hResult = CreateBindCtx(0, &pTemp);
			hResult = pTemp->SetBindOptions(&bopt);

			IFileSystemBindData2 * pfsbd = new CFileSysBindData();
			hResult = pTemp->RegisterObjectParam(STR_FILE_SYS_BIND_DATA, pfsbd);
			if(SUCCEEDED(hResult))
			{
				WIN32_FIND_DATAW fd = {};
				fd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
				hResult = pfsbd->SetFindData(&fd);
			}
			SafeRelease(pfsbd);

			m_pbctx_folder = (void *)pTemp;
		}
		else {}
		pbctx = (IBindCtx *)m_pbctx_folder;
	}
	else {}

	IShellItemImageFactory * pshliif = nullptr;
	hResult = SHCreateItemFromParsingName(szKeyPath, pbctx, IID_IShellItemImageFactory, (void **)&pshliif);

	if(hResult == S_OK)
	{
		pImage = new CShellItemImage(pshliif);
		SafeRelease(pshliif);
		m_images[iHash] = pImage;

		return pImage;
	}
	else
		return nullptr;
}
#include <Shlguid.h>
#include <NTQuery.h>
#include <propkey.h>

static int_64 FileTimeToUTC(const FILETIME & fTime)
{
	return (((int_64)fTime.dwHighDateTime << 32i64) | fTime.dwLowDateTime) - 116444736000000000i64;
}
SHFileInfoT CSHFileInfoBuffer::GetInfo(const char_x * szFile)
{
	SHFileInfoT info = {};
	//HANDLE hFile = CreateFileW(szFile, GENERIC_READ,
	//	FILE_SHARE_READ,
	//	NULL,
	//	OPEN_EXISTING,
	//	FILE_FLAG_BACKUP_SEMANTICS,
	//	NULL);
	//if(hFile != INVALID_HANDLE_VALUE)
	//{
	//	FILETIME ftCreate, ftRead, ftWrite;
	//	if(GetFileTime(hFile, &ftCreate, &ftRead, &ftWrite))
	//	{
	//		info.tCreate = ((int_64)ftCreate.dwHighDateTime << 32i64) | ftCreate.dwLowDateTime;
	//		info.tRead = ((int_64)ftRead.dwHighDateTime << 32i64) | ftRead.dwLowDateTime;
	//		info.tWrite = ((int_64)ftWrite.dwHighDateTime << 32i64) | ftWrite.dwLowDateTime;

	//		info.tCreate -= 116444736000000000i64;
	//		info.tRead -= 116444736000000000i64;
	//		info.tWrite -= 116444736000000000i64;
	//	}
	//	CloseHandle(hFile);
	//}

	char_16 szTemp[MAX_FILE_PATH];
	textcpy(szTemp, MAX_FILE_PATH, szFile, -1);
	textrplch(szTemp, -1, L'/', L'\\');
	IShellItem2 * pshli = nullptr;
	HRESULT hResult = SHCreateItemFromParsingName(szTemp, nullptr, IID_IShellItem, (void **)&pshli);
	if(pshli)
	{
		char_16 * ppszValue = nullptr;
		ulong_64 ulValue = 0;
		FILETIME ftValue = {};

		hResult = pshli->GetString(PKEY_ItemTypeText, &ppszValue);
		if(hResult == S_OK)
			info.chbType = ppszValue;
		hResult = pshli->GetFileTime(PKEY_DateCreated, &ftValue);
		if(hResult == S_OK)
			info.tCreate = FileTimeToUTC(ftValue);
		hResult = pshli->GetFileTime(PKEY_DateAccessed, &ftValue);
		if(hResult == S_OK)
			info.tRead= FileTimeToUTC(ftValue);
		hResult = pshli->GetFileTime(PKEY_DateModified, &ftValue);
		if(hResult == S_OK)
			info.tWrite = FileTimeToUTC(ftValue);

		hResult = pshli->GetUInt64(PKEY_Size, &ulValue);
		if(hResult == S_OK)
			info.size = (int_64)ulValue;

		CoTaskMemFree(ppszValue);
		SafeRelease(pshli);
	}
	return info;
}

VENUS_END
