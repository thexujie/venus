#pragma once

#include "CControl.h"

VENUS_BEG

class V2D_API IFoldingViewItem : public IViewItem
{

};

class V2D_API FoldingViewItemT : public ObjectT<IFoldingViewItem>
{
public:
	FoldingViewItemT() : index(-1), panel(nullptr), minwidth(0), userdata(0), hoving(false), collapsed(false){}
	~FoldingViewItemT() {}

	int_x index;
	textw title;
	IControl * panel;
	int_x minwidth;
	int_x userdata;

	sizeix siTitle;
	sizeix siPanel;
	sizeix siItem;
	bool hoving;
	bool collapsed;
};

class V2D_API CFoldingView : public CControl
{
public:
	CFoldingView();
	~CFoldingView();
	const oid_t & GetOid() const override;

	FoldingViewItemT * AddItem(const char_16 * szText, int_x iMinWidth = 0, IImage * pImage = nullptr, int_x iUserData = 0);
	FoldingViewItemT * AddItem(FoldingViewItemT * pItem);

	void BegUpdate();
	void EndUpdate();
	void Update();
	void UpdateView();
	void UpdateScroll() override;

	pointix GetOffset() const override;

	void OnMouseMove(pointix point) override;
	void OnMouseDownL(pointix point) override;
	void OnMouseUpL(pointix point) override;
	void OnMouseClickL(pointix point) override;
	void OnMouseOut(pointix point) override;
	void OnScroll(intx2 scroll, intx2 offset) override;

	void OnVisualSizeChanged() override;
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;
	void QueryDebugMenu(int_x & iBase, IMenu * pMenu) const override;
	void OnDebugMenu(int_x iBase, int_x iResult) override;

public:
	void SetItemExpanded(int_x iIndex, bool bExpanded = true);
	bool IsItemExpanded(int_x iIndex) const;
	void SetRowSpace(int_x iRowSpace);
	int_x GetRowSpace() const;
	void SetColSpace(int_x iColSpace);
	int_x GetColSpace() const;
protected:
	int_x _GetViewWidth() const;
	rectix _GetItemRect(int_x iIndex) const;
	rectix _GetItemTitleRect(int_x iIndex)const;
	rectix _GetTagRect(rectix rcItem)const;
	int_x _FindTagIndex(pointix point) const;
	void _SetHovingTagIndex(int_x iIndex);
protected:
	vector<FoldingViewItemT *> m_items;
	int_x m_iUpdating;
	int_x m_iRowSpace;
	int_x m_iColSpace;
	int_x m_iHovingTagIndex;
};

VENUS_END
