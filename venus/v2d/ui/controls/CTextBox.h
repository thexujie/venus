#pragma once

#include "CControl.h"

VENUS_BEG

class V2D_API txb_paragraph_t
{
public:
	// ILayout ����
	ITextLayout * pLayout;

	int_x para;
	// �ı���Χ
	trange_t trange;
	// ���ַ�Χ
	crange_t crange;
	// �з�Χ
	lrange_t lrange;
	// ��ʾ��Χ
	tlrect_t tlrect;
	// ������
	paragraph_tag_e tag;
};

class V2D_API CTextBox : public CControl, public IDocSource
{
public:
	CTextBox();
	~CTextBox();

public:
	const oid_t & GetOid() const;
	void GetMeta(IMetaHelper * pHelper);

	void SetText(const char_16 * szText, int_x iLength = -1);
	void SetText(textw text);

	void OnShow();
	void OnFontChanged();
	void OnFocusedChanged();
	void OnVisualSizeChanged();
	void OnEnableChanged();
	void OnUpdate();

	void OnKeyInput(char_32 chInput);
	void OnKeyDown(KeyCodeE eKeyCode);
	void OnMouseDownL(pointix point);
	void OnMouseUpL(pointix point);
	void OnMouseDownR(pointix point);
	void OnMouseUpR(pointix point);
	void OnMouseMove(pointix point);

	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;

	void OnScrollX(int_x iValue);
	void OnScrollY(int_x iValue);
public:
	doc_source_t GetDocSource() const;
	textformat_t GetDefFormat() const;

public:
	void UpdateScroll();

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

	// �����ơ�
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
	bool GetImeInfo(ImeInfoT & imeInfo) const;
	rectix _GetCaretRect(int_x iIndex) const;
public:
	void QueryDebugMenu(int_x & iBase, IMenu * pMenu) const;
	void OnDebugMenu(int_x iBase, int_x iResult);
private:
	int_x m_iIndex;
	int_x m_iLastIndex;
	int_x m_iSelectIndex;
	int_x m_iSelectLength;
	int_x m_iLastX;

	// ���� ucs4��һ���ַ�Ҫ����������.
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
