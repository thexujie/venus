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
 * @brief 树状控件   
 */
class V2D_API CTreeView : public CControl
{
public:
	CTreeView();
	~CTreeView();
	const oid_t & GetOid() const override;
	void GetMeta(IMetaHelper * pHelper) override;

	/**增加/移除项*/
	TreeItemT * AddItem(const char_16 * szText, TreeItemT * pParent = nullptr, IImage * pImage = nullptr, int_x iUserData = 0);
	TreeItemT * AddItem(TreeItemT * pItem);
	void Remove(TreeItemT * pItem);
	void Clear(TreeItemT * pItem);
	void InvalidItem(TreeItemT * pItem, bool bContent = false);
	void BegUpdate();
	void EndUpdate();
	void Update();

	bool HasItem(TreeItemT * pItem) const;

	/**查找项*/
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

	/**选中项目*/
	void SelectItem(TreeItemT * pItem);
	void SelectNextItem(TreeItemT * pItem);
	void SelectLastItem(TreeItemT * pItem);

	void SetRowSpace(int_x iRowSpace);
	int_x GetRowSpace() const;
	void SetColSpace(int_x iColSpace);
	int_x GetColSpace() const;

	/**展开/折叠项目，如果项目是可展开的（无论是否有子项）*/
	void SetItemExpand(TreeItemT * pItem, bool bExpand);
	void EnsureVisible(TreeItemT * pItem);
public:
	void SetCanSelectNull(bool bCanSelectNull);
	bool CanSelectNull();

	/**是否显示展开/折叠标记*/
	void SetShowExpand(bool bShowExpand);
	bool IsShowExpand() const;
	/**设置项的展开/折叠方式*/
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

	// 行间距
	int_x m_iRowSpace;
	// 图标和文本的间距
	int_x m_iColSpace;
	// 标记的大小
	int_x m_iTagSize;
	//! 是否显示折叠标记
	bool m_bShowExpand;
	//! 正在 Update
	int_x m_iUpdating;
	//! 是否禁止手动选择空项
	bool m_bDisableSelectNull;
	//! 展开/折叠 的操作方式
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
	// 选中项改变
	event<IControl *, TreeItemT *> SelectedItemChanged;
	// 项展开/折叠前
	event<IControl *, TreeItemT *> ItemExpanding;
	// 项展开/折叠前
	event<IControl *, TreeItemT *> ItemExpanded;
};

VENUS_END
