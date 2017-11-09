#pragma once

#include "CControl.h"

VENUS_BEG

enum ListViewModeE
{
	//! 详细信息
	ListViewModeDetails = 0,
	//! 平铺
	ListViewModeTile,
	//! 图标
	ListViewModeIcon,
	//! 仅显示图标
	ListViewModeIconOnly,
};

class V2D_API ListViewItemT : public ObjectT<IObject>
{
public:
	ListViewItemT() : Image(nullptr), Row(0), Col(0), UserData(0) {}
	ListViewItemT(const char_16 * szText, IImage * pImage = nullptr, int_x iUserData = 0) 
		: Text(szText), Image(pImage), Row(0), UserData(iUserData) 
	{
		SafeAddRef(Image);
	}
	ListViewItemT(const ListViewItemT & another)
	{
		Text = another.Text;
		ContentSize = another.ContentSize;
		Image = another.Image;
		Row = another.Row;
		UserData = another.UserData;
		SafeAddRef(Image);
	}
	~ListViewItemT()
	{
		SafeRelease(Image);
	}

	textw Text;
	sizeix ContentSize;
	IImage * Image;
	int_x Row;
	int_x Col;
	int_x UserData;
};

class V2D_API ListViewColT : public ObjectT<IObject>
{
public:
	ListViewColT() : Id(0), Col(0), Width(0), UserData(0), TextAlign(AlignLeft) {}
	ListViewColT(const char_16 * szText, int_x iWidth, int_x iUserData = 0): Id(0), Text(szText), Col(0), Width(iWidth), UserData(iUserData) {}

	textw Text;
	int_x Width;
	AlignE TextAlign;

	int_x Id;
	int_x Col;
	int_x UserData;
};

class V2D_API ListViewRowT : public ObjectT<IObject>
{
public:
	ListViewRowT() : Row(0), Height(0), UserData(0) {}
	ListViewRowT(int_x iRow, int_x iHeight, int_x iUserData) : Row(iRow), Height(iHeight), UserData(iUserData){}
	~ListViewRowT()
	{
		for(int_x cnt = 0, size = Items.size(); cnt < size; ++cnt)
			SafeRelease(Items[cnt]);
	}
	int_x Row;
	int_x Height;
	int_x UserData;

	vector<ListViewItemT *> Items;
};

// 如果 row 等于此值，则表示光标位于标题栏。
const int_x LISTVIEW_HEADER_ROW = IX_MIN;

class V2D_API CListView : public CControl
{
public:
	CListView();
	~CListView();
	const cid_t & GetOid() const override;
	void GetMeta(IMetaHelper * pHelper) override;

	int_x AddCol(const char_16 * szText, int_x iWidth = 0, AlignE eTextAlign = AlignLeftCenterY, int_x iUserData = 0);
	int_x AddRow(int_x iUserData = 0);
	int_x AddRow(ListViewRowT * pRow);
	int_x AddItem(int_x iRow, const char_16 * szText, IImage * pImage = nullptr, int_x iUserData = 0);
	// 不要再外部 release pItem.
	int_x AddItem(int_x iRow, ListViewItemT * pItem);

	void BegUpdate();
	void EndUpdate();
	void Update();

	const ListViewColT & GetCol(int_x iIndex) const;
	ListViewItemT * GetItem(int_x iRow, int_x iCol);
	const ListViewItemT * GetItem(int_x iRow, int_x iCol) const;

	int_x GetColCount() const;
	int_x GetRowCount() const;

	void SetMainCol(int_x iCol);
	int_x GetMainCol() const;
	
	void SetInterestCols(const int_x * pColIds, int_x iCount);
	template<int_x COUNT>
	void SetInterestCols(const int_x (&pColIds)[COUNT]) { SetInterestCols(pColIds, COUNT); }

	void SetHeaderHeight(int_x iHeight);
	int_x GetHeaderHeight() const;

	void SetViewMode(ListViewModeE eMode, int_x iIconSize = 0);
	ListViewModeE GetViewMode() const;
	
	void SetIconSize(int_x iIconSize);
	int_x GetIconSize() const;

	void SetColSpace(int_x iSpace);
	int_x GetColSpace() const;
	void SetRowSpace(int_x iSpace);
	int_x GetRowSpace() const;

	void SetColWidth(int_x iCol, int_x iWidth);
	void SetRowHeight(int_x iRow, int_x iHeight);
	int_x GetColWidth(int_x iCol) const;
	int_x GetRowHeight(int_x iRow) const;

	int_x GetColX(int_x iIndex) const;
	int_x GetRowY(int_x iRow) const;
	rectix GetRowRect(int_x iRow) const;
	rectix GetTileRect(int_x iRow, int_x iCol) const;
	intx2 GetTileNum() const;
	sizeix GetTileSize() const;
	intx2 GetRowTile(int_x iRow) const;

	void SetRowUserData(int_x iRow, int_x iUserData);
	int_x GetRowUserData(int_x iRow) const;
	void SetItemUserData(int_x iRow, int_x iCol, int_x iUserData);

	void RemoveAllCols();
	void RemoveAllRows();
	void RemoveAll();

	int_x GetAllWidth() const;
	int_x GetAllHeight() const;

	void SetHovingRow(int_x iRow);
	int_x GetHovingRow() const;
	void SetSelectedRow(int_x iRow);
	int_x GetSelectedRow() const;

	ListViewRowT * GetRowSelected() const;
	ListViewColT * GetColSelected() const;
	ListViewItemT * GetItemSelected() const;

	void SetHovingCol(int_x iCol);
	int_x GetHovingCol() const;
	void SetSelectedCol(int_x iCol);
	int_x GetSelectedCol() const;

	void SetSelectedItem(int_x iRow, int_x iCol);
	void GetSelectedItem(int_x & iRow, int_x & iCol);

	void SetSizingCol(int_x iCol, int_x iOffset);
	int_x GetSizingCol() const;

	void SetCanSelectNullRow(bool bCanSelectNullRow);
	bool CanSelectNullRow() const;

	void SetShowFrameX(bool bFrameX);
	void SetShowFrameY(bool bFrameY);
	void SetShowHeader(bool bHeader);
	bool IsShowHeader() const;
	bool IsShowFrameX() const;
	bool IsShowFrameY() const;

	void Sort(bool (*pfnCmp)(const ListViewRowT * pLeftItem, const ListViewRowT * pRightItem));
	void Sort(function<int_x(const ListViewRowT *, const ListViewRowT *)> fun);

	void UpdateScroll() override;
	void EnsureRowVisible(int_x iRow);
	bool GetItemAt(int_x iX, int_x iY, int_x & iRow, int_x & iCol, int_x & iColSizing);

public:
	void RequestCursor(pointix point, CursorE & eCursor) const override;
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;

public:
	void OnVisualSizeChanged() override;
	void OnMouseDownL(pointix point) override;
	void OnMouseDownR(pointix point) override;
	void OnMouseUpL(pointix point) override;
	void OnMouseDBClickL(pointix point) override;
	void OnMouseMove(pointix point) override;
	void OnMouseOut(pointix point) override;
	void OnKeyDown(KeyCodeE eKeyCode) override;
	void OnScroll(intx2 scroll, intx2 offset) override;
	void QueryDebugMenu(int_x & iBase, IMenu * pMenu) const override;
	void OnDebugMenu(int_x iBase, int_x iResult) override;
protected:
	void OnPaintDetails(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;
	void OnPaintTile(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;
	void OnPaintIcon(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;
	void PaintHeader(IPaint * pPaint, const IUITheme * pTheme) const;
	void PaintRow(IPaint * pPaint, const ListViewRowT * pRow, int_x iX, int_x iY, const IUITheme * pTheme) const;
	void PaintFrame(IPaint * pPaint, const IUITheme * pTheme) const;
	virtual void _PaintImage(IPaint * pPaint, IImage * pImage, rectix rcImage) const;

protected:
	// 纵向间距，内容起始、图片文字之间
	int_x m_iColSpace;
	// 横向间距，row 之间。
	int_x m_iRowSpace;
	// 标题栏高度
	int_x m_iHeaderHeight;
	// Adding 状态下，共计有 m_iItemAdding - 1 被 Add。
	int_x m_iUpdating;
	// 关键列，非 details 模式下显示的内容。
	int_x m_iColMain;
	// 感兴趣的列，title 模式下的额外信息。
	vector<int_x> m_colsOfInterest;
	// 显示模式。
	ListViewModeE m_eViewMode;
	// 图标大小，默认 details(16)，tile(48)
	int_x m_iIconSize;

	vector<ListViewColT> m_cols;
	vector<ListViewRowT *> m_rows;

	int_x m_iColSizing;
	int_x m_iSizingParam;

	int_x m_iRowHoving;
	int_x m_iRowSelected;

	int_x m_iColHoving;
	int_x m_iColSelected;

	bool m_bShowHeader;
	bool m_bShowFrameX;
	bool m_bShowFrameY;
	//! 是否禁止手动选择空项
	bool m_bDisableSelectNullRow;

public:
	event<IControl*, int_x> HeaderMouseDownL;
	event<IControl*, int_x> RowSelecting;
	event<IControl*, int_x> RowSelected;
	event<IControl*, int_x> ColSelecting;
	event<IControl*, int_x> ColSelected;
	event<IControl*, int_x, int_x> ItemSelecting;
	event<IControl*, int_x, int_x> ItemSelected;

public:
	static void ListViewItemSwap(ListViewRowT *& pLeft, ListViewRowT *& pRight);
	static const int_x HEADER_H_EX = 1;
	static const int_x ICON_DEF_SIZE_DETAILS = 16;
	static const int_x ICON_DEF_SIZE_TITLE = 48;
	static const int_x ICON_MIN_SIZE = 16;
	static const int_x ICON_MAX_SIZE = 512;
};

VENUS_END
