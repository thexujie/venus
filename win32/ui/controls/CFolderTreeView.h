#pragma once

VENUS_BEG

const cid_t OID_CFolderTreeView = {L"foldertreeview"};

class WIN32_API CFolderTreeView : public CTreeView
{
public:
	CFolderTreeView();
	~CFolderTreeView();
	const cid_t & GetOid() const;

	void SetRoot(const char_16 * szPath);
	void SelectPath(const char_x * szFolder);
	void SetAttrFilter(uint_32 uiAttrCare, uint_32 uiAttrValue);

	bool BeforeExpand(TreeItemT * pItem);

	int_x GetCurrPath(char_x * szPath, int_x iSize);

protected:
	void _PaintImage(IPaint * pPaint, IImage * pImage, rectix rcImage) const;
protected:
	uint_32 m_uiAttrCare;
	uint_32 m_uiAttrValue;
	chbufferw<MAX_FILE_PATH> m_szRoot;
public:
	static textw GetItemPath(const TreeItemT * pItem);
};

VENUS_END
