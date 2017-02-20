#pragma  once

#include "CControl.h"

VENUS_BEG

class V2D_API ListBoxItemT
{
public:
	ListBoxItemT() : Index(0), Id(0), Image(nullptr), Selected(false), Hoving(false) {}
	ListBoxItemT(int_x iIndex, const char_16 * szText, int_x iId, IImage * pImage) 
		: Index(iIndex), Text(szText), Id(iId), Image(pImage), Selected(false), Hoving(false)
	{
		SafeAddRef(Image);
	}
	ListBoxItemT(const ListBoxItemT & another)
		: Index(another.Index), Text(another.Text), Id(another.Id), Image(another.Image), Selected(another.Selected), Hoving(another.Hoving)
	{
		SafeAddRef(Image);
	}
	~ListBoxItemT()
	{
		SafeRelease(Image);
	}

	void SetSelected(bool bSelected)
	{
		Selected = bSelected;
	}
	bool IsSelected() const { return Selected; }

	int_x Index;
	textw Text;
	int_x Id;
	IImage * Image;

	sizeix Size;
	sizeix ContentSize;

	bool Hoving;
	bool Selected;
};

class V2D_API CListBox : public CControl
{
public:
	CListBox();
	~CListBox();

	const oid_t & GetOid() const override;

	int_x AddItem(const char_16 * szText, IImage * pImage = nullptr, int_x iId = -1);
	void RemoveAll();
	int_x GetItemCount() const;
	const char_16 * GetItemText(int_x iIndex) const;
	
	void BegUpdate();
	void EndUpdate();
	void Update();

	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;

	sizeix GetPreferedSize() const override;
	void OnVisualSizeChanged() override;
	void OnMouseMove(pointix point) override;
	void OnMouseOut(pointix point) override;
	void OnMouseDownL(pointix point) override;
	void OnMouseClickL(pointix point) override;
	void OnKeyDown(KeyCodeE eKeyCode) override;

	void SelectItem(int_x iIndex, bool bSelect = true, bool bMultiSelect = false);
	void SelectItemNext(int_x iIndex);
	void SelectItemPrev(int_x iIndex);
	void SelectItemByText(const char_16 * szText);
	bool IsItemSelected(int_x iIndex) const;

	void SetHovingIndex(int_x iIndex);
	void SetSelectedIndex(int_x iIndex);
	int_x GetSelectedIndex() const;

	void SetCanSelectNull(bool bCanSelectNull);
	bool CanSelectNull() const;
	void SetMultiSelect(bool bMultiSelect);
	bool IsMultiSelect() const;
	void SetShowSelect(bool bShowSelect);
	bool IsShowSelect() const;
	void SetFullCheck(bool bFullCheck);
	bool IsFullCheck() const;
	void SetShowImage(bool bShowImage);
	bool IsShowImage() const;

	void SelectNone();
	void SelectAll();
	void SelectReverse();

	void UpdateScroll() override;

	int_x FindItem(pointix point) const;
	const ListBoxItemT & GetItem(int_x iIndex) const;
public:
	void QueryDebugMenu(int_x & iBase, IMenu * pMenu) const override;
	void OnDebugMenu(int_x iBase, int_x iResult) override;
private:
	vector<ListBoxItemT> m_items;
	int_x m_iUpdating;
	int_x m_iHovingIndex;

	int_x m_iLineSpace;
	int_x m_iIconSize;
	int_x m_iItemSpace;
	// 启用复选框
	bool m_bShowSelect;
	// 单击整行而不是复选框来选中。
	bool m_bFullCheck;
	// 是否允许选中多个
	bool m_bMultiCheck;
	// 是否隐藏图像
	bool m_bHideImage;
	//! 是否禁止手动选择空项
	bool m_bDisableSelectNull;

public:
	static const int_x LINE_SPACE = 3;
	static const int_x ICON_SIZE = 16;
	static const int_x ITEM_SPACE = 4;

public:
	// 返回 1 表示不允许选择/取消选择
	event<IControl *, ListBoxItemT *> ItemSelecting;
	event<IControl *, ListBoxItemT *> ItemSelected;
	event<IControl *, ListBoxItemT *> SelectedItemChanged;
};

VENUS_END
