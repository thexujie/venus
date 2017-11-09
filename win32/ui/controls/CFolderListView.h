#pragma once

VENUS_BEG

const cid_t OID_CFolderListView = {L"folderlistview"};

class WIN32_API FolderListViewRowT: public ListViewRowT
{
public:
	chbufferw<MAX_FILE_PATH> path;
};

class WIN32_API CFolderListView : public CListView
{
public:
	CFolderListView();
	~CFolderListView();
	void SetPath(const char_16 * szPath);
	void SetThumbnails(bool bThumbnails);
	void OnMouseWheelY(pointix point, int_x iWhell);
	void QueryDebugMenu(int_x & iBase, IMenu * pMenu) const;
	void OnDebugMenu(int_x iBase, int_x iResult);

protected:
	void _PaintImage(IPaint * pPaint, IImage * pImage, rectix rcImage) const;

protected:
	bool m_bThumbnails;
};

VENUS_END
