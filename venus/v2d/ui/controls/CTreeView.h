#pragma once

#include "CControl.h"
#include "../base/TreeItem.h"

VENUS_BEG

enum TreeExpandModeE
{
	TreeExpandModeNone,
	TreeExpandModeClick,
	TreeExpandModeDBClick,
	TreeExpandModeStrick,
	TreeExpandModeStrickOrDBClick,
};

/**
 * @brief ��״�ؼ�   
 */
class V2D_API CTreeView : public CControl
{
public:
	CTreeView();
	~CTreeView();
	const oid_t & GetOid() const override;
	void GetMeta(IMetaHelper * pHelper) override;

	/**����/�Ƴ���*/
	TreeItemT * AddItem(const char_16 * szText, TreeItemT * pParent = nullptr, IImage * pImage = nullptr, int_x iUserData = 0);
	TreeItemT * AddItem(TreeItemT * pItem);
	void Remove(TreeItemT * pItem);
	void Clear(TreeItemT * pItem);
	void InvalidItem(TreeItemT * pItem, bool bContent = false);
	void BegUpdate();
	void EndUpdate();
	void Update();

	bool HasItem(TreeItemT * pItem) const;

	/**������*/
	TreeItemT * FindItem(pointix point) const;

	void OnVisualSizeChanged() override;
	void OnMouseMove(pointix point) override;
	void OnMouseOut(pointix point) override;
	void OnMouseDownL(pointix point) override;
	void OnMouseDBClickL(pointix point) override;
	void OnKeyDown(KeyCodeE eKeyCode) override;
	void RequestCursor(pointix point, CursorE & eCursor) const override;

	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;
	void PaintItem(IPaint * pPaint, int_x iX, int_x iY, const TreeItemT * pItem) const;

	void OnScrollX(int_x iValue) override;
	void OnScrollY(int_x iValue) override;
	void UpdateScroll() override;

	void QueryDebugMenu(int_x & iBase, IMenu * pMenu) const override;
	void OnDebugMenu(int_x iBase, int_x iResult) override;

	void SetHovingItem(TreeItemT * pItem);
	const TreeItemT * GetHovingItem() const;
	void SetSelectedItem(TreeItemT * pItem);
	TreeItemT * GetSelectedItem() const;

	/**ѡ����Ŀ*/
	void SelectItem(TreeItemT * pItem);
	void SelectNextItem(TreeItemT * pItem);
	void SelectLastItem(TreeItemT * pItem);

	void SetRowSpace(int_x iRowSpace);
	int_x GetRowSpace() const;
	void SetColSpace(int_x iColSpace);
	int_x GetColSpace() const;

	/**չ��/�۵���Ŀ�������Ŀ�ǿ�չ���ģ������Ƿ������*/
	void SetItemExpand(TreeItemT * pItem, bool bExpand);
	void EnsureVisible(TreeItemT * pItem);
public:
	void SetCanSelectNull(bool bCanSelectNull);
	bool CanSelectNull();

	/**�Ƿ���ʾչ��/�۵����*/
	void SetShowExpand(bool bShowExpand);
	bool IsShowExpand() const;
	/**�������չ��/�۵���ʽ*/
	void SetExpandMode(TreeExpandModeE eExpandMode);
	TreeExpandModeE GetExpandMode() const;

	int_x GetLineHeight() const;
	int_x GetItemTop(TreeItemT * pItem) const;

	virtual bool BeforeExpand(TreeItemT * pItem);
	virtual void OnSelectedItemChanged();
protected:
	void _InvalidAllItems(TreeItemT * pItem, bool bContent);
	void _UpdateItem(TreeItemT * pItem);
	virtual void _PaintImage(IPaint * pPaint, IImage * pImage, rectix rcImage) const;
protected:
	vector<TreeItemT *> m_items;
	TreeItemT * m_pHoveringItem;
	TreeItemT * m_pSelectedItem;

	// �м��
	int_x m_iRowSpace;
	// ͼ����ı��ļ��
	int_x m_iColSpace;
	// ��ǵĴ�С
	int_x m_iTagSize;
	//! �Ƿ���ʾ�۵����
	bool m_bShowExpand;
	//! ���� Update
	int_x m_iUpdating;
	//! �Ƿ��ֹ�ֶ�ѡ�����
	bool m_bDisableSelectNull;
	//! չ��/�۵� �Ĳ�����ʽ
	TreeExpandModeE m_eExpandMode;

public:
	static TreeItemT * HelperGetNextItem(TreeItemT * pItem);
	static TreeItemT * HelperGetPrevItem(TreeItemT * pItem);
	static int_x HelperGetItemCount(const TreeItemT::TreeItemCollection & items);
	static bool HelperIsParent(const TreeItemT * pParent, const TreeItemT * pChild);

public:
	static const int_x ROW_SPACE = 3;
	static const int_x COL_SPACE = 3;
	static const int_x TAG_SIZE = 12;
public:
	// ѡ����ı�
	event<IControl *, TreeItemT *> SelectedItemChanged;
	// ��չ��/�۵�ǰ
	event<IControl *, TreeItemT *> ItemExpanding;
	// ��չ��/�۵�ǰ
	event<IControl *, TreeItemT *> ItemExpanded;
};

VENUS_END
