#include "stdafx.h"
#include "CFolderTreeView.h"
#include <ShlObj.h>

VENUS_BEG

class FolderTreeItemT : public TreeItemT
{
public:
	FolderTreeItemT(const char_16 * szText, IImage * pImage,
		TreeItemT * pParent, const char_16 * szPath) :
		TreeItemT(szText, pImage, pParent), Path(szPath)
	{
	}


	chbufferw<MAX_FILE_PATH> Name;
	chbufferw<MAX_FILE_PATH> Path;
};

CFolderTreeView::CFolderTreeView()
	:m_uiAttrCare(0), m_uiAttrValue(0)
{
}

CFolderTreeView::~CFolderTreeView()
{

}

const oid_t & CFolderTreeView::GetOid() const
{
	return OID_CFolderTreeView;
}

void CFolderTreeView::SetRoot(const char_16 * szRoot)
{
	CSHFileInfoBuffer * pShellBuffer = GetService<CSHFileInfoBuffer>(OID_CSHFileInfoBuffer);
	fileinfo_t root(szRoot);
	m_szRoot = root.path;
	textpathnormalize(m_szRoot.buffer, m_szRoot.length());
	char_x szName[MAX_FILE_PATH] = {0};
	if(m_szRoot.length() == 0)
	{
		vector<driverinfo_t> drivers = get_drivers();

		char_x szDriver[MAX_FILE_PATH] = {0};
		char_x szText[MAX_FILE_PATH] = {0};
		for(int_x cnt = 0, len = drivers.size(); cnt < len; ++cnt)
		{
			const driverinfo_t & driver = drivers[cnt];
			driver.get_driver(szDriver, MAX_FILE_PATH);
			switch(driver.type)
			{
			case driver_invalid:
				textformat(szText, MAX_FILE_PATH, L"无法识别的设备 (%s)", driver.path);
				break;
			case driver_unknown:
				textformat(szText, MAX_FILE_PATH, L"无法识别的设备 (%s)", driver.path);
				break;
			case driver_removealbe:
				textformat(szText, MAX_FILE_PATH, L"可移动磁盘 (%s)", driver.path);
				break;
			case driver_fixed:
				textformat(szText, MAX_FILE_PATH, L"本地磁盘 (%s)", driver.path);
				break;
			case driver_remote:
				textformat(szText, MAX_FILE_PATH, L"网络磁盘 (%s)", driver.path);
				break;
			case driver_CDROM:
				textformat(szText, MAX_FILE_PATH, L"CD 驱动器 (%s)", driver.path);
				break;
			case driver_RAM:
				textformat(szText, MAX_FILE_PATH, L"虚拟磁盘 (%s)", driver.path);
				break;
			default:
				szText[0] = 0;
				break;
			}

			textformat(szText, MAX_FILE_PATH, L"%s (%s)", driver.lable, driver.path);
			IWin32ShellItemImage * pImage = pShellBuffer->GetShellIcon(driver.path);
			FolderTreeItemT * pItem = new FolderTreeItemT(szText, pImage, nullptr, driver.path);
			pItem->Name = driver.path;
			if(driver.has_folders())
				pItem->ForceExpandable = true;
			AddItem(pItem);
		}
	}
	else
	{
		folderinfo_t directory(szRoot);
		vector<folderinfo_t> folders = directory.get_folders();
		for(int_x cnt = 0, size = folders.size(); cnt < size; ++cnt)
		{
			const folderinfo_t & folderInfo = folders[cnt];
			folderInfo.get_name(szName, MAX_FILE_PATH);

			IWin32ShellItemImage * pImage = pShellBuffer->GetShellIcon(L"/");
			FolderTreeItemT * pItem = new FolderTreeItemT(szName, pImage, nullptr, folderInfo.path);
			pItem->Name = szName;
			if(folderInfo.has_folders())
				pItem->ForceExpandable = true;
			AddItem(pItem);
		}
	}
}

class Path
{
public:
	static bool IsFullPath(const char_x * szPath, int_x iLength = -1)
	{
		if(!szPath || !iLength)
			return false;

		const char_x * szTemp = szPath;
		while(*szTemp && iLength--)
		{
			if(*szTemp == ':')
				return true;
			++szTemp;
		}
		return false;
	}

	static bool IsParent(const char_x * szParent, const char_x * szPath, int_x iParentLength = -1, int_x iPathLength = -1)
	{
		if(!szParent || !szPath || !iParentLength || !iPathLength)
			return false;
		if(iParentLength < 0)
			iParentLength = textlen(szParent);
		if(iPathLength)
			iPathLength = textlen(szPath);

		if(iParentLength > iPathLength)
			return false;
		return textequalex(szParent, iParentLength, szPath, iParentLength, true);
	}
};
void CFolderTreeView::SelectPath(const char_x * szPath)
{
	if(!szPath || !szPath[0])
		return;

	chbufferw<MAX_FILE_PATH> folder = szPath;
	textpathnormalize(folder.buffer, folder.length());
	const char_x * szTemp = folder.buffer;
	if(Path::IsFullPath(szPath))
	{
		if(!Path::IsParent(m_szRoot, folder))
			return;
		szTemp += m_szRoot.length();
	}

	FolderTreeItemT * pScope = nullptr;
	int_x iLength = textlen(folder.buffer);
	const char_x * szEnd = folder.buffer + iLength;
	while(szTemp < szEnd)
	{
		int_x iSlash = textch(szTemp, -1, L'/');
		if(iSlash == 0)
		{
			++szTemp;
			continue;
		}
		if(iSlash < 0)
			iSlash = szEnd - szTemp;
	
		if(!pScope)
		{
			for(int_x cnt = 0; cnt < m_items.size(); ++cnt)
			{
				FolderTreeItemT * pTemp = (FolderTreeItemT *)m_items[cnt];
				if(pTemp->Name.equal(szTemp, iSlash, true))
				{
					this->SetItemExpand(pTemp, true);
					pScope = pTemp;
					break;
				}
			}
		}
		else
		{
			for(int_x cnt = 0; cnt < pScope->Items.size(); ++cnt)
			{
				FolderTreeItemT * pTemp = (FolderTreeItemT *)(TreeItemT *)(pScope->Items[cnt]);
				if(pTemp->Name.equal(szTemp, iSlash, true))
				{
					this->SetItemExpand(pTemp, true);
					pScope = pTemp;
					break;
				}
			}
		}
		szTemp += iSlash + 1;
	}
	szTemp = nullptr;
	if(pScope)
	{
		SetSelectedItem(pScope);
		EnsureVisible(pScope);
	}
}

void CFolderTreeView::SetAttrFilter(uint_32 uiAttrCare, uint_32 uiAttrValue)
{
	m_uiAttrCare = uiAttrCare;
	m_uiAttrValue = uiAttrValue;
}

bool CFolderTreeView::BeforeExpand(TreeItemT * pItem)
{
	if(pItem->Expanded)
		return true;

	FolderTreeItemT * pFolderItem = (FolderTreeItemT *)pItem;
	int_64 iBeg = time_ms();

	BegUpdate();
	Update();
	pItem->Items.clear();

	char_x szName[MAX_FILE_PATH] = {0};
	CSHFileInfoBuffer * pShellBuffer = GetService<CSHFileInfoBuffer>(OID_CSHFileInfoBuffer);

	folderinfo_t directory(pFolderItem->Path);
	vector<folderinfo_t> folders = directory.get_folders();
	folders.sort(small_then<folderinfo_t>);
	for(int_x cnt = 0, size = folders.size(); cnt < size; ++cnt)
	{
		const folderinfo_t & folder = folders[cnt];
		folder.get_name(szName, MAX_FILE_PATH);

		IWin32ShellItemImage * pImage = pShellBuffer->GetShellIcon(L"/");
		FolderTreeItemT * pSubItem = new FolderTreeItemT(szName, pImage, pItem, folder.path);
		pSubItem->Name = szName;
		if(folder.has_folders())
			pSubItem->ForceExpandable = true;
		AddItem(pSubItem);
	}
	EndUpdate();

	log1(L"共计用时 %d ms.", time_ms() - iBeg);
	return true;
}

int_x CFolderTreeView::GetCurrPath(char_x * szPath, int_x iSize)
{
	if(!m_pSelectedItem)
	{
		textempty(szPath, iSize);
		return 0;
	}
	else
	{
		textw path = GetItemPath(m_pSelectedItem);
		return textcpy(szPath, iSize, path.buffer(), path.length());
	}
}

void CFolderTreeView::_PaintImage(IPaint * pPaint, IImage * pImage, rectix rcImage) const
{
	if(!pPaint || !pImage || rcImage.is_empty())
		return;

	CShellItemImage * psiImage = dynamic_cast<CShellItemImage *>(pImage);
	if(psiImage->Prepare(rcImage.size, false))
	{
		sizeix size = psiImage->GetSize();
		rcImage.x = rcImage.x + (rcImage.w - size.w) / 2;
		rcImage.y = rcImage.y + (rcImage.h - size.h) / 2;
		pPaint->DrawImage(pImage, rcImage.x, rcImage.y);
	}
}

textw CFolderTreeView::GetItemPath(const TreeItemT * pItem)
{
	const FolderTreeItemT * pfItem = (const FolderTreeItemT *)pItem;
	return pfItem->Path;
}

VENUS_END
