#pragma once

#include "CControl.h"

VENUS_BEG

class V2D_API txb_paragraph_t
{
public:
	// ILayout 对象
	ITextLayout * pLayout;

	int_x para;
	// 文本范围
	trange_t trange;
	// 文字范围
	crange_t crange;
	// 行范围
	lrange_t lrange;
	// 显示范围
	tlrect_t tlrect;
	// 段落标记
	paragraph_tag_e tag;
};

class V2D_API CTextBox : public CControl, public IDocSource
{
public:
	CTextBox();
	~CTextBox();

public:
	const cid_t & GetOid() const override;
	void GetMeta(IMetaHelper * pHelper) override;

	void SetText(const char_16 * szText, int_x iLength = -1) override;
	void SetText(textw text) override;

	bool GetImeInfo(ImeInfoT & imeInfo) const override;

	void OnShow() override;
	void OnFontChanged() override;
	void OnFocusedChanged() override;
	void OnVisualSizeChanged() override;
	void OnEnableChanged() override;

	void OnKeyInput(char_32 chInput) override;
	void OnKeyDown(KeyCodeE eKeyCode) override;
	void OnMouseDownL(pointix point) override;
	void OnMouseUpL(pointix point) override;
	void OnMouseDownR(pointix point) override;
	void OnMouseUpR(pointix point) override;
	void OnMouseMove(pointix point) override;
	void OnScroll(intx2 scroll, intx2 offset) override;

	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;

public:
	doc_source_t GetDocSource() const override;
	textformat_t GetDefFormat() const override;

public:
	void UpdateScroll() override;

	void SetTextWrap(TextWrapE eWrapMode);
	TextWrapE GetTextWrap() const;
	void SetReadOnly(bool bReadOnly);
	bool IsReadOnly() const;

	void SetIndex(int_x iIndex);
	int_x GetIndex() const;

	void SetSelect(int_x iIndex, int_x iLength);
	bool GetSelect(int_x & iIndex, int_x & iLength) const;

	void ForceBlink(bool bBlink);
	void BeginBlink();
	void EndBlink();
	int_x OnBlink(int_x iTime);

	void SelectNone();
	void SelectRange(int_x iIndex1, int_x iIndex2);
	void SelectAll();

	bool IsSelectAll() const;
	bool HasTextSelected() const;
	int_x GetSelectedText(char_16 * pText, int_x iSize) const;

	void DeleteSelected();
	void DoBackSpace();
	void DoClear();
	void DoCopy() const;
	void DoPaste();
	void DoCut();
	void DoDelete();
	void AddText(const char_16 * szText, int_x iLength = -1);
	void InsertText(int_x iIndex, const char_16 * szText, int_x iLength = -1);

	// 光标控制。
	void TlCaretTo(int_x iIndex);
	void CaretLeft();
	void CaretRight();
	void CaretHome();
	void CaretEnd();
	void CaretTop();
	void CaretBottom();
	void CaretUp();
	void CaretDown();

	void OnSelectedTextChanged();

protected:
	void ScrollToCaret();
	rectix _GetCaretRect(int_x iIndex) const;
public:
	void QueryDebugMenu(int_x & iBase, IMenu * pMenu) const override;
	void OnDebugMenu(int_x iBase, int_x iResult) override;
private:
	int_x m_iIndex;
	int_x m_iLastIndex;
	int_x m_iSelectIndex;
	int_x m_iSelectLength;
	int_x m_iLastX;

	// 对于 ucs4，一个字符要分两次输入.
	char_16 m_chInputPre;
	bool m_bBlinking;
	bool m_bForceBlink;
	bool m_bReadOnly;

	TextWrapE m_eTextWrap;
public:
	event<IControl *, pointix> ShowingMenu;
	event<IControl *> SelectedTextChanged;

public:
	err_t TlLayout();
	err_t TlBranch();
	err_t TlClear();

	err_t TlInsertText(int_x iIndex, const char_16 * szText, int_x iLength = -1);
	err_t TlDeleteText(int_x iIndex, int_x iLength);

	int_x TlGetClusterCount() const;
	tl_cluster_t TlFindCluster(int_x iIndex) const;
	tl_cluster_t TlGetCluster(int_x iIndex) const;
	tl_line_t TlGetLine(int_x iIndex) const;
	tl_para_t TlGetPara(int_x iIndex) const;

	tl_range_t TlGetRange(pointix point) const;
	int_x TlGetRanges(int_x iCluster, int_x iLength, tl_range_t * ranges, int_x iCount) const;
	int_x TlGetRanges(int_x iIndex, int_x iLength, tl_range_t * ranges, int_x iCount, int_x iY, int_x iHeight) const;
	tl_metrics_t TlGetMetrics() const;

	void TlSetIndex(int_x iIndex);
	void TlSetLastIndex(int_x iIndex);
	void TlSelect(int_x iIndex, int_x iLength);
	void TlSelectRange(int_x iIndex1, int_x iIndex2);
protected:
	vector<txb_paragraph_t> m_paras;
};

VENUS_END
