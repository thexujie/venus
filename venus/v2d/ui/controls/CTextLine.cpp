#include "stdafx.h"
#include "CTextLine.h"

VENUS_BEG

CTextLine::CTextLine() : m_pLayout(nullptr),
m_iIndex(0), m_iLastIndex(0),
m_iSelectIndex(0), m_iSelectLength(),
m_iFlags(0),
m_iTextOffsetX(0)
{
	m_bRepaintMouseInOut = true;
	m_bRepaintMouseDragInOut = true;
	m_bRepaintFocusedChanged = true;
	m_bRepaintEnabledChanged = true;
	m_eCursor = CursorIBeam;
	m_eImeMode = ImeModeOn;
	m_bFocusAble = true;
	m_eBorderType = BorderType2;
	m_padding.set(3, 4, 3, 4);
	m_margin.set(3, 3, 3, 3);
}

CTextLine::~CTextLine()
{
	TlClear();
	EndBlink();
}

const oid_t & CTextLine::GetOid() const
{
	return OID_TextLine;
}

void CTextLine::GetMeta(IMetaHelper * pHelper)
{
	CControl::GetMeta(pHelper);
	pHelper->help_evt(L"OnShowingMenu", ShowingMenu);
	pHelper->help_evt(L"OnSelectedTextChanged", SelectedTextChanged);
}

bool CTextLine::CanFocus() const
{
	return !m_bTextReadOnly && CControl::CanFocus();
}

void CTextLine::SetText(const char_16 * szText, int_x iLength)
{
	if(iLength < 0)
		iLength = textlen(szText);

	if(!m_text.equal(szText, iLength))
	{
		m_text.set(szText, iLength);
		TlLayout();
		SetIndex(m_text.length());
		OnTextChanged();
	}
}

void CTextLine::SetText(textw text)
{
	if(m_text != text)
	{
		m_text = text;
		TlLayout();
		SetIndex(m_text.length());
		OnTextChanged();
	}
}

void CTextLine::OnFocusedChanged()
{
	if(m_bFocused)
		BeginBlink();
	else
		EndBlink();
	UpdateIme();
	ForceBlink(m_bFocused);
	Repaint();
	CControl::OnFocusedChanged();
}

void CTextLine::OnVisualSizeChanged()
{
	ScrollToCaret();
	UpdateIme();
	Repaint();
	CControl::OnVisualSizeChanged();
}

void CTextLine::OnEnableChanged()
{
	if(m_bDisabled)
	{
		EndBlink();
		Repaint();
	}
	CControl::OnEnableChanged();
}

void CTextLine::OnFontChanged()
{
	if(m_text.is_valid())
	{
		TlLayout();
		ScrollToCaret();
		Repaint();
	}
	CControl::OnFontChanged();
}

void CTextLine::OnKeyInput(char_32 chInput)
{
	if(!m_bTextReadOnly && isprintchar(chInput))
	{
		switch(chInput)
		{
		case '\b':
			break;
		default:
		{
			char_16 szChar[2] = {0};
			int_x iLength = unicode_to_utf16(chInput, szChar, 2);
			if(iLength > 0)
			{
				if(HasTextSelected())
					DeleteSelected();
				TlInsertText(m_iIndex, szChar, iLength);
			}
		}
		break;
		}
	}
	CControl::OnKeyInput(chInput);
}

void CTextLine::OnKeyDown(KeyCodeE eKeyCode)
{
	CControl::OnKeyDown(eKeyCode);
	switch(eKeyCode)
	{
	case KeyCodeBackspace:
		DoBackSpace();
		break;
	case KeyCodeDelete:
		DoDelete();
		break;
	case KeyCodeLeft:
		DoCaretLeft();
		break;
	case KeyCodeRight:
		DoCaretRight();
		break;
	case KeyCodeHome:
		DoCaretHome();
		break;
	case KeyCodeEnd:
		DoCaretEnd();
		break;
	case KeyCodeV:
		if(IsKeyDown(KeyCodeControl) && !m_bTextReadOnly)
			DoPaste();
		break;
	case KeyCodeC:
		if(IsKeyDown(KeyCodeControl))
			DoCopy();
		break;
	case KeyCodeX:
		if(IsKeyDown(KeyCodeControl) && !m_bTextReadOnly)
			DoCut();
		break;
	case KeyCodeA:
		if(IsKeyDown(KeyCodeControl))
			SelectAll();
		break;
	case KeyCodeEnter:
		break;
	default:
		break;
	}
}

void CTextLine::OnMouseDownL(pointix point)
{
	if(m_pLayout)
	{
		tl_range_t range = TlGetRange(pointix(point.x + m_iTextOffsetX, point.y));
		if(IsKeyDown(KeyCodeShift))
		{
			TlSetIndex(range.crange.index);
			TlSelectRange(m_iLastIndex, range.crange.index);
		}
		else
		{
			TlSetIndex(range.crange.index);
			TlSetLastIndex(range.crange.index);
			TlSelect(0, 0);
			Capture(true);
		}

	}
	CControl::OnMouseDownL(point);
}

void CTextLine::OnMouseUpL(pointix point)
{
	Capture(false);
	CControl::OnMouseUpL(point);
}

void CTextLine::OnMouseDownR(pointix point)
{
	CControl::OnMouseDownR(point);

	if(IsKeyDown(KeyCodeShift))
	{

	}
	else
	{
		int_x bShownMenu = ShowingMenu(this, point);
		if(!bShownMenu)
		{
			bool bCanPaste = !m_bTextReadOnly && ClipboardCheckData(ClipboardDataTypeText16);
			MenuT menu;
			menu.AddItem(1, L"剪切", HasTextSelected() && !IsTextReadOnly() ? 0 : MenuFlagDisable);
			menu.AddItem(2, L"复制", HasTextSelected() ? 0 : MenuFlagDisable);
			menu.AddItem(3, L"粘贴", bCanPaste ? 0 : MenuFlagDisable);
			menu.AddItem(4, L"删除", HasTextSelected() && !IsTextReadOnly() ? 0 : MenuFlagDisable);
			menu.AddSplit();
			menu.AddItem(5, L"选择全部", m_text.is_valid() && IsTextSelectAble() ? 0 : MenuFlagDisable);
			//menu.AddItem(6, L"只读", IsTextReadOnly());
			//menu.AddItem(7, L"可选", IsTextSelectAble());
			switch(PopupMenu(point, &menu))
			{
			case 1:
				DoCut();
				break;
			case 2:
				DoCopy();
				break;
			case 3:
				DoPaste();
				break;
			case 4:
				DeleteSelected();
				break;
			case 5:
				SelectAll();
				break;
			case 6:
				SetTextReadOnly(!m_bTextReadOnly);
				break;
			case 7:
				SetTextSelectAble(m_bTextNotSelectAble);
				break;
			default:
				break;
			}
		}
	}
}

void CTextLine::OnMouseMove(pointix point)
{
	if(IsMouseDownL() && m_pLayout)
	{
		tl_range_t range = m_pLayout->HitTest(pointix(point.x + m_iTextOffsetX, point.y));
		TlSetIndex(range.trange.index);
		TlSelectRange(m_iLastIndex, m_iIndex);
	}
	CControl::OnMouseMove(point);
}

sizeix CTextLine::GetContentSize() const
{
	return maxof(MeasureMenuString(m_text, m_text.length(), m_font) + m_padding.size(), sizeix(m_font.size / 2, m_font.size));
}

void CTextLine::SetIndex(int_x iIndex)
{
	TlSetIndex(TlFindCluster(iIndex).cluster);
}

int_x CTextLine::GetIndex() const
{
	return TlGetCluster(m_iIndex).trange.index;
}

void CTextLine::SetTextSelectAble(bool bSelectAble)
{
	if(bSelectAble == m_bTextNotSelectAble)
	{
		if(!bSelectAble)
			SelectNone();
		m_bTextNotSelectAble = !bSelectAble;
	}
}

bool CTextLine::IsTextSelectAble() const
{
	return !m_bTextNotSelectAble;
}

void CTextLine::SetTextReadOnly(bool bReadOnly)
{
	if(bReadOnly != m_bTextReadOnly)
	{
		m_bTextReadOnly = bReadOnly;
		NcRepaint();
	}
}

bool CTextLine::IsTextReadOnly() const
{
	return m_bTextReadOnly;
}

void CTextLine::SetAlwaysShowSelect(bool bAlwaysShowSelect)
{
	if(m_bAlwaysShowSelect != bAlwaysShowSelect)
	{
		m_bAlwaysShowSelect = bAlwaysShowSelect;
		if(!IsFocused() && HasTextSelected())
			Repaint();
	}
}

bool CTextLine::IsAlwaysShowSelect() const
{
	return !m_bAlwaysShowSelect;
}

void CTextLine::SetSelect(int_x iIndex, int_x iLength)
{
	SelectRange(iIndex, iIndex + iLength);
}

bool CTextLine::GetSelect(int_x & iIndex, int_x & iLength) const
{
	if(iLength)
	{
		iIndex = m_iSelectIndex;
		iLength = m_iSelectLength;
		return true;
	}
	else
	{
		iIndex = iLength = 0;
		return false;
	}
}

void CTextLine::SelectNone()
{
	TlSelect(0, 0);
}

void CTextLine::SelectRange(int_x iIndex1, int_x iIndex2)
{
	if(iIndex1 == iIndex2)
	{
		if(m_iSelectLength)
		{
			m_iSelectIndex = m_iSelectLength = 0;
			OnSelectedTextChanged();
			Repaint();
		}
		return;
	}

	if(iIndex1 > iIndex2)
		swap(iIndex1, iIndex2);

	tl_cluster_t cluster_beg = TlFindCluster(iIndex1);
	tl_cluster_t cluster_end = TlFindCluster(iIndex2);
	TlSelectRange(cluster_beg.cluster, cluster_end.cluster);
}

void CTextLine::SelectAll()
{
	SetSelect(0, m_text.length());
}

bool CTextLine::HasTextSelected() const
{
	return m_iSelectLength != 0;
}

void CTextLine::DeleteSelected()
{
	if(m_iSelectLength)
		TlDeleteText(m_iSelectIndex, m_iSelectLength);
}

int_x CTextLine::GetSelectedText(char_16 * pText, int_x iSize) const
{
	if(!pText || !iSize)
		return m_iSelectLength;
	else if(HasTextSelected() && iSize > 0)
		return textcpy(pText, iSize, m_text.buffer() + m_iSelectIndex, m_iSelectLength);
	else
		return 0;
}

void CTextLine::DoBackSpace()
{
	if(HasTextSelected())
		DeleteSelected();
	else if(m_pLayout && m_iIndex > 0)
		TlDeleteText(m_iIndex - 1, 1);
	else {}
}

void CTextLine::DoDelete()
{
	if(HasTextSelected())
		DeleteSelected();
	else if(m_pLayout && m_iIndex < TlGetClusterCount() - 1)
		TlDeleteText(m_iIndex, 1);
	else {}
}

void CTextLine::DoClear()
{
	TlClear();
}

void CTextLine::DoCopy() const
{
	if(HasTextSelected())
		ClipboardSetText16(m_text.buffer() + m_iSelectIndex, m_iSelectLength);
}

void CTextLine::DoPaste()
{
	// 移除换行符
	const char_16 * pText = ClipboardGetText16();
	if(!pText || !pText[0])
		return;
	textw text;
	bool bRunning = true;
	char_16 ch = 0;
	while(bRunning)
	{
		ch = *pText;
		switch(ch)
		{
		case '\0':
			bRunning = false;
			break;
		case '\r':
		case '\n':
			break;
		default:
			text.append(ch);
			break;
		}
		++pText;
	}

	if(HasTextSelected())
		DeleteSelected();

	TlInsertText(m_iIndex, text, text.length());
}

void CTextLine::DoCut()
{
	if(HasTextSelected())
	{
		DoCopy();
		DoDelete();
	}
}

void CTextLine::InsertText(int_x iIndex, const char_16 * szText, int_x iLength)
{
	m_text.insert(iIndex, szText, iLength);
	TlLayout();

	SetSelect(0, 0);
	tl_cluster_t cluster = m_pLayout->FindCluster(iIndex + iLength);
	TlSetIndex(cluster.cluster);
	TlSetLastIndex(cluster.cluster);
}

void CTextLine::DoCaretLeft()
{
	int_x iIndex = m_iIndex - 1;
	if(IsKeyDown(KeyCodeShift))
	{
		if(iIndex >= 0)
		{
			TlSetIndex(iIndex);
			TlSelectRange(iIndex, m_iLastIndex);
		}
	}
	else if(iIndex >= 0)
	{
		TlSelect(0, 0);
		TlSetIndex(iIndex);
		TlSetIndex(iIndex);
	}
	else {}
}

void CTextLine::DoCaretRight()
{
	int_x iIndex = m_iIndex + 1;
	if(IsKeyDown(KeyCodeShift))
	{
		if(iIndex < TlGetClusterCount())
		{
			TlSetIndex(iIndex);
			TlSelectRange(iIndex, m_iLastIndex);
		}
	}
	else if(iIndex < TlGetClusterCount())
	{
		TlSelect(0, 0);
		TlSetIndex(iIndex);
		TlSetIndex(iIndex);
	}
	else {}
}

void CTextLine::DoCaretHome()
{
	if(TlGetClusterCount() == 0)
		return;

	int_x iIndex = 0;
	if(iIndex == m_iIndex)
		return;

	if(IsKeyDown(KeyCodeShift))
	{
		TlSetIndex(iIndex);
		TlSelectRange(iIndex, m_iLastIndex);
	}
	else
	{
		TlSelect(0, 0);
		TlSetIndex(iIndex);
	}
}

void CTextLine::DoCaretEnd()
{
	if(TlGetClusterCount() == 0)
		return;

	int_x iIndex = TlGetClusterCount() - 1;
	if(iIndex == m_iIndex)
		return;

	if(IsKeyDown(KeyCodeShift))
	{
		TlSetIndex(iIndex);
		TlSelectRange(iIndex, m_iLastIndex);
	}
	else
	{
		TlSelect(0, 0);
		TlSetIndex(iIndex);
	}
}

void CTextLine::CaretTop()
{
	DoCaretHome();
}

void CTextLine::CaretBottom()
{
	DoCaretEnd();
}

void CTextLine::ForceBlink(bool bBlink)
{
	m_bShowCaret = bBlink;
	m_bForceShowCaret = bBlink;
	Repaint();
}

void CTextLine::BeginBlink()
{
	IApp * pps = GetApp();
	if(pps)
		pps->SetTimer(bind(this, &CTextLine::OnBlink), 500, 0);
}

void CTextLine::EndBlink()
{
	IApp * pps = GetApp();
	if(pps)
		pps->KillTimer(bind(this, &CTextLine::OnBlink), 0);
}

int_x CTextLine::OnBlink(int_x iTime)
{
	m_bShowCaret = !m_bShowCaret;
	Repaint();
	return 0;
}

void CTextLine::OnSelectedTextChanged()
{
	SelectedTextChanged(this, m_iSelectIndex, m_iSelectLength);
}

void CTextLine::SetDefaultText(const char_16 * szText, int_x iLength)
{
	m_textDefault.set(szText, iLength);
	if(m_text.is_empty())
		Repaint();
}

void CTextLine::SetDefaultText(textw text)
{
	m_textDefault = text;
	if(m_text.is_empty())
		Repaint();
}

void CTextLine::ScrollToCaret()
{
	if(!m_pLayout)
		return;

	tl_cluster_t cluster = TlGetCluster(m_iIndex);
	int_x iCaretX = cluster.tlrect.x - m_iTextOffsetX;
	int_x iClientW = GetClient().w - m_padding.width();
	if(iClientW < 0)
		iClientW = 0;

	if(iCaretX < 0)
	{
		m_iTextOffsetX += iCaretX;
		Repaint();
	}
	else if(iCaretX >= iClientW)
	{
		m_iTextOffsetX += iCaretX - iClientW;
		Repaint();
	}
	else if(m_iTextOffsetX > 0)
	{
		// 如果最后一个字符右边还有空间，同时文本已经向左偏移了，则补齐这部分空间。
		tl_cluster_t cluster_end = m_pLayout->FindCluster(m_text.length());
		int_x iNeed = iClientW - (cluster_end.tlrect.x - m_iTextOffsetX);
		int_x iTextOffsetX = m_iTextOffsetX;
		if(iNeed > 0)
			iTextOffsetX = maxof(m_iTextOffsetX - iNeed, (int_x)0);

		if(iTextOffsetX != m_iTextOffsetX)
		{
			m_iTextOffsetX = iTextOffsetX;
			Repaint();
		}
	}
	else {}
}

void CTextLine::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	sizeix szClient = GetClient().size;
	rectix rcClipThis(m_padding.left, 0, szClient.w - m_padding.width(), szClient.h);
	pPaint->PushClip(rcClipThis);
	int_x iOffsetX = -m_iTextOffsetX + m_padding.left;
	int_x iOffsetY = (szClient.h - m_font.size) / 2;

	if(m_text.is_valid())
	{
		if(m_iSelectLength && (IsFocused() || m_bAlwaysShowSelect))
		{
			color_t color = IsFocused() ? SysColorTextSelectNormal : SysColorTextSelectDisable;
			tl_range_t range;
			m_pLayout->HitTestRange(m_iSelectIndex, m_iSelectLength, &range, 1);
			pPaint->FillRect(iOffsetX + range.tlrect.x, iOffsetY + range.tlrect.y, range.tlrect.w, range.tlrect.h, color);
		}

		pPaint->DrawTextLayout(m_pLayout, pointix(iOffsetX, iOffsetY), rcClip, m_uiForeColor);
	}
	else if(m_textDefault.is_valid() && !m_bSelected)
	{
		pPaint->DrawString(m_textDefault, m_font, pointix(iOffsetX, iOffsetY), SysColorForeDisable);
	}
	else {}

	pPaint->PopClip();
	// 输入指示
	if(m_bShowCaret && m_bFocused)
	{
		rectix rcCaret = _GetCaretRect(m_iIndex);
		pPaint->DrawLine(pointix(iOffsetX + rcCaret.x, iOffsetY + rcCaret.y),
			pointix(iOffsetX + rcCaret.x, iOffsetY + rcCaret.y + rcCaret.h),
			m_uiForeColor);
	}
}

bool CTextLine::GetImeInfo(ImeInfoT & imeInfo) const
{
	if(m_bTextReadOnly)
		return false;

	sizeix szClient = GetClient().size;
	imeInfo.eImeMode = m_eImeMode;
	tl_cluster_t cluster = TlFindCluster(m_iIndex);
	int_x iOffsetY = (szClient.h - m_font.size) / 2;
	imeInfo.rcCompose.x = cluster.tlrect.x - m_iTextOffsetX + m_padding.left;
	imeInfo.rcCompose.y = (szClient.h - m_font.size) / 2;
	imeInfo.rcCompose.w = szClient.w - imeInfo.rcCompose.x;
	imeInfo.rcCompose.h = szClient.h - imeInfo.rcCompose.y;
	if(imeInfo.rcCompose.y < 0)
		imeInfo.rcCompose.y = 0;
	else if(imeInfo.rcCompose.bottom() > szClient.h)
		imeInfo.rcCompose.y = szClient.h - imeInfo.rcCompose.h;
	else {}
	imeInfo.font = m_font;
	return true;
}

doc_source_t CTextLine::GetDocSource() const
{
	doc_source_t source = {m_text.length(), m_text.buffer()};
	return source;
}

textformat_t CTextLine::GetDefFormat() const
{
	return textformat_t(m_font, TextWrapNone);
}

void CTextLine::QueryDebugMenu(int_x & iBase, IMenu * pMenu) const
{
	CControl::QueryDebugMenu(iBase, pMenu);
	MenuT * pSubMenu = new MenuT();
	pSubMenu->AddItem(iBase + 1, L"TextSelect", !m_bTextNotSelectAble);
	pSubMenu->AddItem(iBase + 2, L"EditAble", !m_bTextReadOnly);
	pSubMenu->AddItem(iBase + 3, L"AlwaysShowSelect", m_bAlwaysShowSelect);
	pMenu->AddSplit();
	pMenu->AddItem(0, L"TextLine", pSubMenu);
	SafeRelease(pSubMenu);
	iBase += 10;
}

void CTextLine::OnDebugMenu(int_x iBase, int_x iResult)
{
	iBase -= 10;
	switch(iResult - iBase)
	{
	case 1:
		SetTextSelectAble(m_bTextNotSelectAble);
		break;
	case 2:
		SetTextReadOnly(!m_bTextReadOnly);
		break;
	case 3:
		SetAlwaysShowSelect(!m_bAlwaysShowSelect);
		break;
	default:
		CControl::OnDebugMenu(iBase, iResult);
		break;
	}
}

err_t CTextLine::TlLayout()
{
	if(!m_pLayout)
	{
		m_pLayout = Get2DDevice()->CreateTextLayout();
		m_pLayout->Initialize(this);
	}

	if(m_pLayout)
		m_pLayout->Layout({0, m_text.length()}, GetClient().w, paragraph_tag_none);
	return err_ok;
}

err_t CTextLine::TlClear()
{
	SafeRelease(m_pLayout);
	TlSelect(0, 0);
	TlSetIndex(0);
	TlSetLastIndex(0);
	UpdateScroll();
	ScrollToCaret();
	Repaint();
	return err_ok;
}

err_t CTextLine::TlInsertText(int_x iIndex, const char_16 * szText, int_x iLength)
{
	if(iLength < 0)
		iLength = textlen(szText);

	if(!iLength)
		return err_ok;

	tl_cluster_t cluster = TlGetCluster(iIndex);
	m_text.insert(cluster.trange.index, szText, iLength);
	TlLayout();
	TlSelect(0, 0);
	cluster = TlFindCluster(cluster.trange.index + iLength);
	TlSetIndex(cluster.cluster);
	TlSetLastIndex(cluster.cluster);
	UpdateScroll();
	ScrollToCaret();
	Repaint();
	return err_ok;
}

err_t CTextLine::TlDeleteText(int_x iIndex, int_x iLength)
{
	if(!iLength)
		return err_ok;

	tl_cluster_t cluster_beg = TlGetCluster(iIndex);
	tl_cluster_t cluster_end = TlGetCluster(iIndex + iLength - 1);
	m_text.remove(cluster_beg.trange.index, cluster_end.trange.index + cluster_end.trange.length - cluster_beg.trange.index);
	TlLayout();
	TlSelect(0, 0);
	TlSetIndex(iIndex);
	TlSetLastIndex(iIndex);
	UpdateScroll();
	ScrollToCaret();
	Repaint();
	return err_ok;
}

int_x CTextLine::TlGetClusterCount() const
{
	if(!m_pLayout)
		return 1;

	tl_metrics_t metrics = m_pLayout->GetMetrics();
	return metrics.cluster_length;
}

tl_cluster_t CTextLine::TlFindCluster(int_x iIndex) const
{
	if(iIndex == 0 && !m_pLayout)
	{
		tl_cluster_t cluster = {};
		cluster.tlrect.h = m_font.size;
		return cluster;
	}

	return m_pLayout->FindCluster(iIndex);
}

tl_cluster_t CTextLine::TlGetCluster(int_x iIndex) const
{
	if(iIndex == 0 && !m_pLayout)
	{
		tl_cluster_t cluster = {};
		cluster.tlrect.h = m_font.size;
		return cluster;
	}

	return m_pLayout->GetCluster(iIndex);
}

tl_range_t CTextLine::TlGetRange(pointix point) const
{
	if(!m_pLayout)
	{
		tl_range_t range = {};
		return range;
	}

	return m_pLayout->HitTest(point);
}

void CTextLine::TlSetLastIndex(int_x iCurrIndex)
{
	if(iCurrIndex != m_iLastIndex)
	{
		m_iLastIndex = iCurrIndex;
	}
}

void CTextLine::TlSetIndex(int_x iIndex)
{
	if(iIndex != m_iIndex)
	{
		m_iIndex = iIndex;
		ScrollToCaret();
		ForceBlink(true);
		UpdateIme();
	}
}

void CTextLine::TlSelect(int_x iIndex, int_x iLength)
{
	if(m_iSelectIndex != iIndex || m_iSelectLength != iLength)
	{
		m_iSelectIndex = iIndex;
		m_iSelectLength = iLength;
		OnSelectedTextChanged();
		Repaint();
	}
}

void CTextLine::TlSelectRange(int_x iIndex1, int_x iIndex2)
{
	if(iIndex1 < iIndex2)
		TlSelect(iIndex1, iIndex2 - iIndex1);
	else
		TlSelect(iIndex2, iIndex1 - iIndex2);
}


rectix CTextLine::_GetCaretRect(int_x iIndex) const
{
	tl_cluster_t cluster = TlGetCluster(iIndex);
	rectix rect;
	rect.x = cluster.tlrect.x;
	rect.y = cluster.tlrect.y;
	rect.w = cluster.tlrect.w;
	rect.h = cluster.tlrect.h;
	return rect;
}


VENUS_END
