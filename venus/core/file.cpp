#include "stdafx.h"
#include "file.h"
#include <io.h>
#include <windows.h>

VENUS_BEG

static int_64 FileTimeToInt64(const FILETIME & ftime)
{
	return ((int_64)ftime.dwHighDateTime << 32i64) | ftime.dwLowDateTime;
}

pathinfo_t::pathinfo_t()
{
	textempty(path, MAX_FILE_PATH);
	_attributes = 0;
}

pathinfo_t::pathinfo_t(const char_x * szFile)
{
	generate(szFile);
}
pathinfo_t::pathinfo_t(const char_x * szDirectory, const char_x * szFile)
{
	generate(szDirectory, szFile);
}

void pathinfo_t::generate(const char_x * szFile, int_x iLength)
{
	if(iLength < 0)
		iLength = textlen(szFile);

	if(!iLength)
	{
		char_x szDirectory[MAX_FILE_PATH];
		int_x iLen = (int_x)::GetCurrentDirectory(MAX_FILE_PATH, szDirectory);
		iLength = textcpy(path, MAX_FILE_PATH, szDirectory, iLen);
		textrplch(path, iLen, _T('\\'), _T('/'));
	}
	else if(textch(szFile, iLength, (char_x)':') >= 0)
	{
		textcpy(path, MAX_FILE_PATH, szFile, iLength);
		textrplch(path, iLength, _T('\\'), _T('/'));
	}
	else
	{
		char_x szDirectory[MAX_FILE_PATH];
		int_x iLen = (int_x)::GetCurrentDirectory(MAX_FILE_PATH, szDirectory);
		textmakepath(path, MAX_FILE_PATH, szDirectory, iLen, szFile, -1);
	}

	WIN32_FILE_ATTRIBUTE_DATA fad = {};
	BOOL bOk = GetFileAttributesEx(path, GetFileExInfoStandard, (void *)&fad);
	if(bOk && fad.dwFileAttributes != INVALID_FILE_ATTRIBUTES)
	{
		_attributes = fad.dwFileAttributes & FILE_ATTR_MASK;
		exists = true;
		int_x iColon = textch(path, iLength, (char_x)':');
		if(iColon >= 0 && iColon  + 1 == iLength)
			disk = true;
		else
			disk = false;
	}
	else
		_attributes = 0;
}

void pathinfo_t::generate(const char_x * szFolder, const char_x * szFile, int_x iFolderLength, int_x iFileLength)
{
	if(iFileLength < 0)
		iFileLength = textlen(szFile);
	if(iFolderLength < 0)
		iFolderLength = textlen(szFolder);

	int_x iLength = 0;
	if(!iFolderLength)
	{
		char_x szCurrDirectory[MAX_FILE_PATH] = {0};
		int_x iCurrLength = (int_x)::GetCurrentDirectory(MAX_FILE_PATH, szCurrDirectory);
		iLength = textmakepath(path, MAX_FILE_PATH, szCurrDirectory, iCurrLength, szFile, iFileLength);
	}
	else if(textch(szFolder, iFolderLength, (char_x)':') < 0)
	{
		char_x szCurrDirectory[MAX_FILE_PATH] = {0};
		char_x szFullDirectory[MAX_FILE_PATH] = {0};
		int_x iCurrLength = (int_x)::GetCurrentDirectory(MAX_FILE_PATH, szCurrDirectory);
		iFolderLength = textmakepath(szFullDirectory, MAX_FILE_PATH,
			szCurrDirectory, iCurrLength, szFolder, iFolderLength);
		iLength = textmakepath(path, MAX_FILE_PATH, szFullDirectory, iFolderLength, szFile, iFileLength);
	}
	else
	{
		iLength = textmakepath(path, MAX_FILE_PATH, szFolder, iFolderLength, szFile, iFileLength);
	}

	uint_32 uiAttributes = GetFileAttributes(path);
	if(uiAttributes != INVALID_FILE_ATTRIBUTES)
	{
		_attributes = uiAttributes & FILE_ATTR_MASK;
		exists = true;
		int_x iColon = textch(path, -1, (char_x)':');
		disk = iColon >= 0 && iLength - iColon <= 1;
	}
	else
		_attributes = 0;
}
void pathinfo_t::refresh()
{
	char_x szFullName[MAX_FILE_PATH] = {0};
	textcpy(szFullName, MAX_FILE_PATH, path, MAX_FILE_PATH - 1);
	generate(szFullName);
}

int_x pathinfo_t::driver(char_x * szDriver, int_x iSize) const
{
	return textsplitpath_drive(path, -1, szDriver, iSize);
}

int_x pathinfo_t::folder(char_x * szDir, int_x iSize) const
{
	return textsplitpath_folder(path, -1, szDir, iSize);
}

int_x pathinfo_t::name(char_x * szName, int_x iSize) const
{
	return textsplitpath_name(path, -1, szName, iSize);
}

int_x pathinfo_t::extention(char_x * szExt, int_x iSize) const
{
	return textsplitpath_ext(path, -1, szExt, iSize);
}

textw pathinfo_t::driver() const
{
	char_x szTemp[MAX_FILE_PATH] = {};
	int_x iLength = textsplitpath_drive(path, -1, szTemp, MAX_FILE_PATH);
	return textw(szTemp, iLength);
}

textw pathinfo_t::folder() const
{
	char_x szFolder[MAX_FILE_PATH] = {};
	int_x iLength = textsplitpath_folder(path, -1, szFolder, MAX_FILE_PATH);
	return textw(szFolder, iLength);
}

textw pathinfo_t::name() const
{
	char_x szTemp[MAX_FILE_PATH] = {};
	int_x iLength = textsplitpath_name(path, -1, szTemp, MAX_FILE_PATH);
	return textw(szTemp, iLength);
}

textw pathinfo_t::extention() const
{
	char_x szTemp[MAX_FILE_PATH] = {};
	int_x iLength = textsplitpath_ext(path, -1, szTemp, MAX_FILE_PATH);
	return textw(szTemp, iLength);
}

int_x pathinfo_t::get_paths(vector<pathinfo_t> & arrPaths, uint_32 uiAttrCare, uint_32 uiAttr, const char_x * szFilter) const
{
	int_x iCount = 0;
	char_x szFind[MAX_FILE_PATH] = {0};
	textcpy(szFind, MAX_FILE_PATH, path, MAX_FILE_PATH - 1);
	if(szFilter)
	{
		textcat(szFind, MAX_FILE_PATH, _T("\\"), 1);
		textcat(szFind, MAX_FILE_PATH, szFilter, -1);
	}
	else
		textcat(szFind, MAX_FILE_PATH, _T("\\*"), 2);

	WIN32_FIND_DATA fd;
	//HANDLE hFind = FindFirstFileEx(szFind, FindExInfoStandard, &fd, FindExSearchNameMatch, NULL, 0);
	HANDLE hFind = FindFirstFile(szFind, &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(fd.cFileName[0] == '.')
			{
				if(fd.cFileName[1] == 0 ||
					(fd.cFileName[1] == '.' && fd.cFileName[2] == 0))
					continue;
			}
			if((fd.dwFileAttributes & uiAttrCare) == uiAttr)
			{
				textcpy(szFind, MAX_FILE_PATH, path, MAX_FILE_PATH - 1);
				textcat(szFind, MAX_FILE_PATH, _T("\\"), 1);
				textcat(szFind, MAX_FILE_PATH, fd.cFileName, MAX_FILE_PATH - 1);
				arrPaths.add(pathinfo_t(szFind));
				++iCount;
			}
		}
		while(FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
	else {}
	return iCount;
}

vector<pathinfo_t> pathinfo_t::get_paths(uint_32 uiAttrCare, uint_32 uiAttr, const char_x * szFilter) const
{
	vector<pathinfo_t> paths;
	get_paths(paths, uiAttrCare, uiAttr, szFilter);
	return paths;
}

bool pathinfo_t::operator < (const pathinfo_t & another) const
{
	if(directory && !another.directory)
		return true;
	else if(!directory && another.directory)
		return false;
	else
		return textcmp(path, -1, another.path, -1) < 0;
}

fileinfo_t::fileinfo_t()
{
}

fileinfo_t::fileinfo_t(const char_x * szFile) : pathinfo_t(szFile)
{
}
fileinfo_t::fileinfo_t(const char_x * szDirectory, const char_x * szFile)
	: pathinfo_t(szDirectory, szFile)
{
}

int_x fileinfo_t::filename(char_x * szName, int_x iSize) const
{
	const char_x * pDot = textprch(path, -1, _T('.'));
	const char_x * pSlash = textprch(path, -1, _T('/'));
	if(pDot && pSlash && pSlash < pDot)
	{
		return textcpy(szName, iSize, pSlash + 1, pDot - (pSlash + 1));
	}
	else
	{
		textempty(szName, iSize);
		return 0;
	}
}

textx fileinfo_t::filename() const
{
	char_x szFileName[MAX_FILE_PATH] = {};
	int_x iLength = textsplitpath_filename(path, -1, szFileName, MAX_FILE_PATH);
	return textw(szFileName, iLength);
}

bool fileinfo_t::create(bool bForce)
{
	if(!bForce && File::Exists(path))
		return false;
	return File::Create(path);
}

bool fileinfo_t::remove()
{
	return ::DeleteFile(path) != FALSE;
}

bool fileinfo_t::rename(const char_x * szFileName, int_x iLength, bool bForce)
{
	char_x szFolder[MAX_FILE_PATH] = {0};
	char_x szNewFile[MAX_FILE_PATH] = {0};
	folder(szFolder, MAX_FILE_PATH);
	textmakepath(szNewFile, MAX_FILE_PATH, szFolder, -1, szFileName, -1);

	if(File::Copy(path, szNewFile, bForce) && File::Delete(path))
	{
		generate(szNewFile);
		return true;
	}
	else
		return false;
}

int_x fileinfo_t::size() const
{
	return static_cast<int_x>(File::GetSize(path));
}

folderinfo_t::folderinfo_t()
{

}

folderinfo_t::folderinfo_t(const char_x * szFile) : pathinfo_t(szFile)
{

}

folderinfo_t::folderinfo_t(const char_x * szDirectory, const char_x * szFile) : pathinfo_t(szDirectory, szFile)
{

}

bool folderinfo_t::has_paths(uint_32 uiAttrCare, uint_32 uiAttr, const char_x * szFilter) const
{
	int_x iCount = 0;
	char_x szFind[MAX_FILE_PATH] = {0};
	textcpy(szFind, MAX_FILE_PATH, path, MAX_FILE_PATH - 1);
	if(szFilter)
	{
		textcat(szFind, MAX_FILE_PATH, _T("\\"), 1);
		textcat(szFind, MAX_FILE_PATH, szFilter, -1);
	}
	else
		textcat(szFind, MAX_FILE_PATH, _T("\\*"), 2);

	WIN32_FIND_DATA fd = {};
	HANDLE hFind = FindFirstFile(szFind, &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		uint_32 uiCare = uiAttrCare;
		uint_32 uiValue = uiAttr;
		do
		{
			if((fd.dwFileAttributes & uiCare) == uiValue)
			{
				if(fd.cFileName[0] == '.')
				{
					if(fd.cFileName[1] == 0 ||
						(fd.cFileName[1] == '.' && fd.cFileName[2] == 0))
						continue;
				}
				else {}
				return true;
			}
		}
		while(FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
	else {}
	return false;
}

bool folderinfo_t::has_folders(uint_32 uiAttrCare, uint_32 uiAttr, const char_x * szFilter) const
{
	int_x iCount = 0;
	char_x szFind[MAX_FILE_PATH] = {0};
	textcpy(szFind, MAX_FILE_PATH, path, MAX_FILE_PATH - 1);
	if(szFilter)
	{
		textcat(szFind, MAX_FILE_PATH, _T("\\"), 1);
		textcat(szFind, MAX_FILE_PATH, szFilter, -1);
	}
	else
		textcat(szFind, MAX_FILE_PATH, _T("\\*"), 2);

	WIN32_FIND_DATA fd = {};
	bool bFound = false;
	HANDLE hFind = FindFirstFile(szFind, &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		uint_32 uiCare = FILE_ATTRIBUTE_DIRECTORY | uiAttrCare;
		uint_32 uiValue = FILE_ATTRIBUTE_DIRECTORY | uiAttr;
		do
		{
			if((fd.dwFileAttributes & uiCare) == uiValue)
			{
				if(fd.cFileName[0] == '.')
				{
					if(fd.cFileName[1] == 0 ||
						(fd.cFileName[1] == '.' && fd.cFileName[2] == 0))
						continue;
				}
				else {}
				bFound = true;
				break;
			}
		}
		while(FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
	else {}
	return bFound;
}

bool folderinfo_t::has_files(uint_32 uiAttrCare, uint_32 uiAttr, const char_x * szFilter) const
{
	int_x iCount = 0;
	char_x szFind[MAX_FILE_PATH] = {0};
	textcpy(szFind, MAX_FILE_PATH, path, MAX_FILE_PATH - 1);
	if(szFilter)
	{
		textcat(szFind, MAX_FILE_PATH, _T("\\"), 1);
		textcat(szFind, MAX_FILE_PATH, szFilter, -1);
	}
	else
		textcat(szFind, MAX_FILE_PATH, _T("\\*"), 2);

	WIN32_FIND_DATA fd = {};
	bool bFound = false;
	HANDLE hFind = FindFirstFile(szFind, &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		uint_32 uiCare = FILE_ATTRIBUTE_DIRECTORY | uiAttrCare;
		uint_32 uiValue = uiAttr & (~FILE_ATTRIBUTE_DIRECTORY);
		do
		{
			if((fd.dwFileAttributes & uiCare) == uiValue)
			{
				bFound = true;
				break;
			}
		}
		while(FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
	else {}
	return bFound;
}

int_x folderinfo_t::get_folders(vector<folderinfo_t> & arrFolders, uint_32 uiAttrCare, uint_32 uiAttr, const char_x * szFilter) const
{
	int_x iCount = 0;
	char_x szFind[MAX_FILE_PATH] = {0};
	textcpy(szFind, MAX_FILE_PATH, path, MAX_FILE_PATH - 1);
	if(szFilter)
	{
		textcat(szFind, MAX_FILE_PATH, _T("\\"), 1);
		textcat(szFind, MAX_FILE_PATH, szFilter, -1);
	}
	else
		textcat(szFind, MAX_FILE_PATH, _T("\\*"), 2);

	WIN32_FIND_DATA fd;
	//HANDLE hFind = FindFirstFileEx(szFind, FindExInfoStandard, &fd, FindExSearchNameMatch, NULL, 0);
	HANDLE hFind = FindFirstFile(szFind, &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		uint_32 uiCare = FILE_ATTRIBUTE_DIRECTORY | uiAttrCare;
		uint_32 uiValue = FILE_ATTRIBUTE_DIRECTORY | uiAttr;
		do
		{
			if((fd.dwFileAttributes & uiCare) == uiValue)
			{
				if(fd.cFileName[0] == '.')
				{
					if(fd.cFileName[1] == 0 ||
						(fd.cFileName[1] == '.' && fd.cFileName[2] == 0))
						continue;
				}

				textcpy(szFind, MAX_FILE_PATH, path, MAX_FILE_PATH - 1);
				textcat(szFind, MAX_FILE_PATH, _T("/"), 1);
				textcat(szFind, MAX_FILE_PATH, fd.cFileName, MAX_FILE_PATH - 1);
				arrFolders.add(folderinfo_t(szFind));
				++iCount;
			}
		}
		while(FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
	else {}
	return iCount;
}

vector<folderinfo_t> folderinfo_t::get_folders(uint_32 uiAttrCare, uint_32 uiAttr, const char_x * szFilter) const
{
	vector<folderinfo_t> folders;
	get_folders(folders, uiAttrCare, uiAttr, szFilter);
	return folders;
}

int_x folderinfo_t::get_files(vector<fileinfo_t> & arrFiles, uint_32 uiAttrCare, uint_32 uiAttr, const char_x * szFilter) const
{
	int_x iCount = 0;
	char_x szFind[MAX_FILE_PATH] = {0};
	textcpy(szFind, MAX_FILE_PATH, path, MAX_FILE_PATH - 1);
	if(szFilter)
	{
		textcat(szFind, MAX_FILE_PATH, _T("\\"), 1);
		textcat(szFind, MAX_FILE_PATH, szFilter, -1);
	}
	else
		textcat(szFind, MAX_FILE_PATH, _T("\\*"), 2);

	WIN32_FIND_DATA fd;
	//HANDLE hFind = FindFirstFileEx(szFind, FindExInfoStandard, &fd, FindExSearchNameMatch, NULL, 0);
	HANDLE hFind = FindFirstFile(szFind, &fd);
	if(hFind != INVALID_HANDLE_VALUE)
	{
		uint_32 uiCare = FILE_ATTRIBUTE_DIRECTORY | uiAttrCare;
		uint_32 uiValue = uiAttr & (~FILE_ATTRIBUTE_DIRECTORY);
		do
		{
			if((fd.dwFileAttributes & uiCare) == uiValue)
			{
				textcpy(szFind, MAX_FILE_PATH, path, MAX_FILE_PATH - 1);
				textcat(szFind, MAX_FILE_PATH, _T("\\"), 1);
				textcat(szFind, MAX_FILE_PATH, fd.cFileName, MAX_FILE_PATH - 1);
				arrFiles.add(fileinfo_t(szFind));
				++iCount;
			}
		}
		while(FindNextFile(hFind, &fd));
		FindClose(hFind);
	}
	else {}
	return iCount;
}

vector<fileinfo_t> folderinfo_t::get_files(uint_32 uiAttrCare, uint_32 uiAttr, const char_x * szFilter) const
{
	vector<fileinfo_t> files;
	get_files(files, uiAttrCare, uiAttr, szFilter);
	return files;
}

bool folderinfo_t::create()
{
	if(::CreateDirectory(path, NULL) != FALSE)
	{
		refresh();
		return true;
	}
	else
		return false;
}

bool folderinfo_t::remove()
{
	if(::RemoveDirectory(path) != FALSE)
	{
		refresh();
		return true;
	}
	else
		return false;
}

bool folderinfo_t::remove_all()
{
	if(!exists || readonly)
		return false;

	vector<folderinfo_t> folders = get_folders();
	for(int_x cnt = 0, len = folders.size(); cnt < len; ++cnt)
	{
		folderinfo_t & folder = folders[cnt];
		if(!folder.remove_all())
			return false;

		if(!folder.remove())
			return false;
	}
	vector<fileinfo_t> files = get_files();
	for(int_x cnt = 0, len = files.size(); cnt < len; ++cnt)
	{
		if(!files[cnt].remove())
			return false;
	}
	return true;
}

driverinfo_t::driverinfo_t()
{
	type = driver_unknown;
}

driverinfo_t::driverinfo_t(const char_x * szDriver, int_x iLength)
{
	generate(szDriver, iLength);
	UINT uiType = ::GetDriveType(szDriver);
	switch(uiType)
	{
	case DRIVE_UNKNOWN:
		type = driver_unknown;
		break;
	case DRIVE_NO_ROOT_DIR:
		type = driver_invalid;
		break;
	case DRIVE_REMOVABLE:
		type = driver_removealbe;
		break;
	case DRIVE_FIXED:
		type = driver_fixed;
		break;
	case DRIVE_REMOTE:
		type = driver_remote;
		break;
	case DRIVE_CDROM:
		type = driver_CDROM;
		break;
	case DRIVE_RAMDISK:
		type = driver_RAM;
		break;
	default:
		type = driver_invalid;
		break;
	}

	char_x szTemp[MAX_FILE_PATH] = {path[0], L':', L'\\', L'\0'};
	GetVolumeInformation(szTemp, lable, MAX_FILE_PATH, nullptr, nullptr, nullptr, nullptr, 0);
}

int_x get_drivers(vector<driverinfo_t> & arrDrivers)
{
	char_x szDriveBuf[MAX_FILE_PATH];
	GetLogicalDriveStrings(MAX_FILE_PATH, szDriveBuf);
	char_x * pText = szDriveBuf;
	int_x iCount = 0;
	while(*pText)
	{
		char_x chDrive = *pText;
		if(chDrive != L'A' && chDrive != L'B')
		{
			UINT uiType = ::GetDriveType(pText);
			if(uiType != DRIVE_UNKNOWN && uiType != DRIVE_NO_ROOT_DIR)
			{
				char_x szDir[3] = {chDrive, ':', '\0'};
				arrDrivers.add(driverinfo_t(szDir));
				++iCount;
			}
		}
		pText += textlen(pText) + 1;
	}
	return iCount;
}

vector<driverinfo_t> get_drivers()
{
	vector<driverinfo_t> drivers;
	get_drivers(drivers);
	return drivers;
}

bool File::Exists(textw filePath)
{
	return _waccess(filePath, 0) != -1;
}

bool File::Exists(texta filePath)
{
	return _access(filePath, 0) != -1;
}

bool File::Copy(textw srcFilePath, textx dstFilePath, bool overwrite)
{
	return !!::CopyFileW(srcFilePath, dstFilePath, overwrite);
}

bool File::Copy(texta srcFilePath, texta dstFilePath, bool overwrite)
{
	return !!::CopyFileA(srcFilePath, dstFilePath, overwrite);
}

bool File::Move(textw srcFilePath, textx dstFilePath)
{
	return !!::MoveFileW(srcFilePath, dstFilePath);
}

bool File::Move(texta srcFilePath, texta dstFilePath)
{
	return !!::MoveFileA(srcFilePath, dstFilePath);
}

bool File::Delete(textw filePath)
{
	return !!::DeleteFileW(filePath);
}

bool File::Delete(texta filePath)
{
	return !!::DeleteFileA(filePath);
}

bool File::Create(textw filePath)
{
	HANDLE hFile = ::CreateFileW(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile);
		return true;
	}
	else
		return false;
}

bool File::Create(texta filePath)
{
	HANDLE hFile = ::CreateFileA(filePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile);
		return true;
	}
	else
		return false;
}

int_64 File::GetSize(textw filePath)
{
	FILETIME ftime = {};
	struct _stat64 state;
	_wstat64(filePath, &state);
	return state.st_size;
}

int_64 File::GetSize(texta filePath)
{
	FILETIME ftime = {};
	struct _stat64 state;
	_stat64(filePath, &state);
	return state.st_size;
}

date_t File::GetCreationTime(textw filePath)
{
	FILETIME ftime = {};
	struct _stat64 state;
	_wstat64(filePath, &state);
	return date_t(state.st_ctime * 1000);
}

date_t File::GetCreationTime(texta filePath)
{
	FILETIME ftime = {};
	struct _stat64 state;
	_stat64(filePath, &state);
	return date_t(state.st_ctime * 1000);
}

date_t File::GetAccessTime(textw filePath)
{
	FILETIME ftime = {};
	struct _stat64 state;
	_wstat64(filePath, &state);
	return date_t(state.st_atime * 1000);
}

date_t File::GetAccessTime(texta filePath)
{
	FILETIME ftime = {};
	struct _stat64 state;
	_stat64(filePath, &state);
	return date_t(state.st_atime * 1000);
}

date_t File::GetWriteTime(textw filePath)
{
	FILETIME ftime = {};
	struct _stat64 state;
	_wstat64(filePath, &state);
	return date_t(state.st_mtime * 1000);
}

date_t File::GetWriteTime(texta filePath)
{
	FILETIME ftime = {};
	struct _stat64 state;
	_stat64(filePath, &state);
	return date_t(state.st_mtime * 1000);
}


VENUS_END
