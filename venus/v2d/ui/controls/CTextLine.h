#pragma once

#include "CControl.h"

VENUS_BEG

class V2D_API CTextLine : public CControl, public IDocSource
{
public:
	CTextLine();
	~CTextLine();
public:
	const oid_t & GetOid() const override;
	void GetMeta(IMetaHelper * pHelper) override;

	bool CanFocus() const override;

	void SetText(const char_16 * szText, int_x iLength = -1) override;
	void SetText(textw text) override;

	void OnFocusedChanged() override;
	void OnVisualSizeChanged() override;
	void OnEnableChanged() override;
	void OnFontChanged() override;

	void OnKeyInput(char_32 chInput) override;
	void OnKeyDown(KeyCodeE eKeyCode) override;
	void OnMouseDownL(pointix point) override;
	void OnMouseUpL(pointix point) override;
	void OnMouseDownR(pointix point) override;
	void OnMouseMove(pointix point) override;

	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;

	bool GetImeInfo(ImeInfoT & imeInfo) const override;

public:
	doc_source_t GetDocSource() const override;
	textformat_t GetDefFormat() const override;
public:
	sizeix GetContentSize() const override;

	void SetIndex(int_x iIndex);
	int_x GetIndex() const;

	void SetTextSelectAble(bool bSelectAble);
	bool IsTextSelectAble() const;

	void SetTextReadOnly(bool bReadOnly);
	bool IsTextReadOnly() const;

	void SetAlwaysShowSelect(bool bAlwaysShowSelect);
	bool IsAlwaysShowSelect() const;

	void SetSelect(int_x iIndexBeg, int_x iIndexEnd);
	bool GetSelect(int_x & iIndex, int_x & iLength) const;

	void SelectNone();
	void SelectRange(int_x iIndex1, int_x iIndex2);
	void SelectAll();

	bool HasTextSelected() const;
	void DeleteSelected();
	int_x GetSelectedText(char_16 * pText, int_x iSize) const;

	void DoBackSpace();
	void DoDelete();

	void DoClear();
	void DoCopy() const;
	void DoPaste();
	void DoCut();
	void InsertText(int_x iIndex, const char_16 * szText, int_x iLength = -1);

	// ¹â±ê¿ØÖÆ¡£
	void DoCaretLeft();
	void DoCaretRight();
	void DoCaretHome();
	void DoCaretEnd();
	void CaretTop();
	void CaretBottom();
	void ForceBlink(bool bBlink);
	void BeginBlink();
	void EndBlink();
	int_x OnBlink(int_x iTime);

	void OnSelectedTextChanged();

	void SetDefaultText(const char_16 * szText, int_x iLength = -1);
	void SetDefaultText(textw text);
protected:
	void ScrollToCaret();

protected:
	void QueryDebugMenu(int_x & iBase, IMenu * pMenu) const override;
	void OnDebugMenu(int_x iBase, int_x iResult) override;

	err_t TlLayout();
	err_t TlClear();
	err_t TlInsertText(int_x iIndex, const char_16 * szText, int_x iLength = -1);
	err_t TlDeleteText(int_x iIndex, int_x iLength);

	int_x TlGetClusterCount() const;
	tl_cluster_t TlFindCluster(int_x iIndex) const;
	tl_cluster_t TlGetCluster(int_x iIndex) const;
	tl_range_t TlGetRange(pointix point) const;

	void TlSetLastIndex(int_x iCurrIndex);
	void TlSetIndex(int_x iIndex);
	void TlSelect(int_x iIndex, int_x iLength);
	void TlSelectRange(int_x iIndex1, int_x iIndex2);

	rectix _GetCaretRect(int_x iIndex) const;
protected:
	int_x m_iLastIndex;
	int_x m_iIndex;
	int_x m_iSelectIndex;
	int_x m_iSelectLength;

	ITextLayout * m_pLayout;
	union
	{
		int_x m_iFlags;
		struct
		{
			bool m_bShowCaret : 1;
			bool m_bForceShowCaret : 1;
			bool m_bTextNotSelectAble : 1;
			bool m_bTextReadOnly : 1;
			bool m_bAlwaysShowSelect : 1;
		};
	};

	int_x m_iTextOffsetX;
	textw m_textDefault;

public:
	event<IControl *, pointix> ShowingMenu;
	event<IControl *, int_x, int_x> SelectedTextChanged;

public:
	static const oid_t & Oid() { return OID_TextLine; }
};

VENUS_END
