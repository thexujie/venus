#include "stdafx.h"
#include "CTextBox.h"
#include "CScrollBar.h"

/*
* 通过 TlFindCluster 无法获得 ling tag，但是可以获得 para tag.
* 通过 TlGetCluster 可以获得 line tag 和 para tag.
*/

VENUS_BEG

static const int_x MIN_SEL_BACK_W = 2;
static const int_x DEF_MAX_LENGTH = 65536;

CTextBox::CTextBox() :m_chInputPre(0)
{
	m_bRepaintMouseDragInOut = true;
	m_bRepaintFocusedChanged = true;
	m_bRepaintEnabledChanged = true;
	m_bAcceptTab = true;
	m_bAcceptEnter = true;
	m_bFocusAble = true;
	m_eCursor = CursorIBeam;
	m_padding.set(3, 4, 3, 4);
	m_margin.set(3, 3, 3, 3);

	SetAutoHideScrollBarX(true);
	SetAutoHideScrollBarY(false);

	CScrollBar * pScrollBarX = new CScrollBar();
	CScrollBar * pScrollBarY = new CScrollBar();
	pScrollBarX->SetDynamic(true);
	pScrollBarY->SetDynamic(true);
	CControl::SetScrollBarX(pScrollBarX);
	CControl::SetScrollBarY(pScrollBarY);
	AddControl(pScrollBarX);
	AddControl(pScrollBarY);

	m_bBlinking = false;
	m_bForceBlink = false;
	m_bReadOnly = false;
	m_iLastX = 0;
	m_iLastIndex = 0;
	m_iIndex = 0;
	m_iSelectIndex = 0;
	m_iSelectLength = 0;
	m_eTextWrap = TextWrapWord;
	m_eBorderType = BorderType3;
}

CTextBox::~CTextBox()
{
	TlClear();
	EndBlink();
}

const oid_t & CTextBox::GetOid() const
{
	return OID_TextBox;
}

void CTextBox::GetMeta(IMetaHelper * pHelper)
{
	CControl::GetMeta(pHelper);
	pHelper->help_evt(L"ShowingMenu", ShowingMenu);
	pHelper->help_evt(L"SelectedTextChanged", SelectedTextChanged);
}

void CTextBox::SetText(const char_16 * szText, int_x iLength)
{
	if(iLength < 0)
		iLength = textlen(szText);

	if(!m_text.equal(szText, iLength))
	{
		m_text.set(szText, iLength);
		TlLayout();
		SetIndex(m_text.length());
		UpdateScroll();
		OnTextChanged();
	}
}

void CTextBox::SetText(textw text)
{
	if(m_text != text)
	{
		m_text = text;
		TlLayout();
		SetIndex(m_text.length());
		OnTextChanged();
	}
}

bool CTextBox::GetImeInfo(ImeInfoT & imeInfo) const
{
	if(m_bReadOnly)
		return false;

	sizeix szClient = GetClient().size;
	pointix sclText = GetScrollPos();
	imeInfo.eImeMode = m_eImeMode;
	tl_cluster_t cluster = TlGetCluster(m_iIndex);
	imeInfo.rcCompose.set(cluster.tlrect.x - sclText.x, cluster.tlrect.y + cluster.tlrect.w - sclText.y, 1, cluster.tlrect.h);
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

sizeix CTextBox::GetContentSize() const
{
	return maxof(Get2DDevice()->GetTextSize(m_text, m_text.length(), m_font) + m_padding.size(), sizeix(m_font.size / 2, m_font.size));
}

void CTextBox::OnShow()
{
	CControl::OnShow();
}

void CTextBox::OnFontChanged()
{
	int_x iIndex = TlGetCluster(m_iIndex).trange.index;
	int_x iSelectIndex = 0, iSelectLength = 0;
	GetSelect(iSelectIndex, iSelectLength);
	TlLayout();
	m_iIndex = TlFindCluster(iIndex).cluster;
	if(iSelectLength)
	{
		m_iSelectIndex = TlFindCluster(iSelectIndex).cluster;
		m_iSelectLength = TlFindCluster(iSelectIndex + iSelectLength).cluster - m_iSelectIndex;
	}
	UpdateScroll();
	CControl::OnFontChanged();
}

void CTextBox::OnFocusedChanged()
{
	if(IsFocused())
	{
		BeginBlink();
		UpdateIme();
	}
	else
		EndBlink();
	ForceBlink(IsFocused());
	CControl::OnFocusedChanged();
}

void CTextBox::OnVisualSizeChanged()
{
	CControl::OnVisualSizeChanged();
	if(m_eTextWrap == TextWrapNone)
		return;

	int_x iIndex = TlGetCluster(m_iIndex).trange.index;
	int_x iSelectIndex = 0, iSelectLength = 0;
	GetSelect(iSelectIndex, iSelectLength);
	TlBranch();
	m_iIndex = TlFindCluster(iIndex).cluster;
	if(iSelectLength)
	{
		m_iSelectIndex = TlFindCluster(iSelectIndex).cluster;
		m_iSelectLength = TlFindCluster(iSelectIndex + iSelectLength).cluster - m_iSelectIndex;
	}
	UpdateScroll();
}

void CTextBox::OnEnableChanged()
{
	EndBlink();
	ForceBlink(false);
	Repaint();
	CControl::OnEnableChanged();
}

void CTextBox::OnKeyInput(char_32 chInput)
{
	if(!m_bReadOnly)
	{
		switch(chInput)
		{
		case '\r':
			m_chInputPre = 0;
			if(HasTextSelected())
				DeleteSelected();
			TlInsertText(m_iIndex, L"\r\n", 2);
			break;
		case '\n':
			m_chInputPre = 0;
			if(HasTextSelected())
				DeleteSelected();
			TlInsertText(m_iIndex, L"\n", 1);
			break;
		case '\b':
			m_chInputPre = 0;
			break;
		default:
			if(isprintchar(chInput))
			{
				char_16 szChar[2] = {0};
				int_x iLength = 0;
				const unicodeplane_t & plane = unicodeplane_find(chInput);
				if(plane.plane == unicodeplane_utf16_high)
					m_chInputPre = chInput;
				else
				{
					if(m_chInputPre)
					{
						if(plane.plane == unicodeplane_utf16_low)
						{
							szChar[0] = m_chInputPre;
							szChar[1] = chInput;
							iLength = 2;
						}
						else
						{
							m_chInputPre = 0;
							iLength = 0;
						}
					}
					else
					{
						szChar[0] = chInput;
						iLength = 1;
					}
				}

				if(iLength)
				{
					m_chInputPre = 0;
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

void CTextBox::OnKeyDown(KeyCodeE eKeyCode)
{
	switch(eKeyCode)
	{
	case KeyCodeBackspace:
		if(!m_bReadOnly)
			DoBackSpace();
		break;
	case KeyCodeDelete:
		if(!m_bReadOnly)
			DoDelete();
		break;
	case KeyCodeLeft:
		CaretLeft();
		break;
	case KeyCodeRight:
		CaretRight();
		break;
	case KeyCodeUp:
		CaretUp();
		break;
	case KeyCodeDown:
		CaretDown();
		break;
	case KeyCodeHome:
		if(IsKeyDown(KeyCodeControl))
			CaretTop();
		else
			CaretHome();
		break;
	case KeyCodeEnd:
		if(IsKeyDown(KeyCodeControl))
			CaretBottom();
		else
			CaretEnd();
		break;
	case KeyCodeV:
		if(!m_bReadOnly && IsKeyDown(KeyCodeControl))
			DoPaste();
		break;
	case KeyCodeC:
		if(IsKeyDown(KeyCodeControl))
			DoCopy();
		break;
	case KeyCodeX:
		if(!m_bReadOnly && IsKeyDown(KeyCodeControl))
			DoCut();
		break;
	case KeyCodeA:
		if(IsKeyDown(KeyCodeControl))
			SelectAll();
		break;
	default:
		break;
	}
	CControl::OnKeyDown(eKeyCode);
}

void CTextBox::OnMouseDownL(pointix point)
{
	Capture(true);

	pointix sclText = GetScrollPos();
	tl_range_t range = TlGetRange(point + sclText);
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
	}

	CControl::OnMouseDownL(point);
}

void CTextBox::OnMouseUpL(pointix point)
{
	Capture(false);
	CControl::OnMouseUpL(point);
}

void CTextBox::OnMouseDownR(pointix point)
{
	CControl::OnMouseDownR(point);
}

void CTextBox::OnMouseUpR(pointix point)
{
	CControl::OnMouseUpR(point);
	if(IsKeyDown(KeyCodeShift))
	{
	}
	else
	{
		int_x bShownMenu = ShowingMenu(this, point);
		if(!bShownMenu)
		{
			MenuT menu;
			int_x iFlag = HasTextSelected() ? 0 : MenuFlagDisable;

			bool bCanPaste = !m_bReadOnly && ClipboardCheckData(ClipboardDataTypeText16);
			bool bCanSelectAll = !IsSelectAll();
			menu.AddItem(1, L"剪切", HasTextSelected() && !IsReadOnly() ? MenuFlagNone : MenuFlagDisable);
			menu.AddItem(2, L"复制", HasTextSelected() ? MenuFlagNone : MenuFlagDisable);
			menu.AddItem(3, L"粘贴", bCanPaste ? 0 : MenuFlagDisable);
			menu.AddItem(4, L"删除", HasTextSelected() && !IsReadOnly() ? MenuFlagNone : MenuFlagDisable);
			menu.AddSplit();
			menu.AddItem(5, L"选择全部", bCanSelectAll ? 0 : MenuFlagDisable);
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
				DoDelete();
				break;
			case 5:
				SelectAll();
				break;
			default:
				break;
			}
		}
	}
}
void CTextBox::OnMouseMove(pointix point)
{
	if(IsMouseDownL())
	{
		pointix sclText = GetScrollPos();
		tl_range_t range = TlGetRange(sclText + point);
		if(range.crange.index != m_iLastIndex)
			TlSelectRange(m_iLastIndex, range.crange.index);
		else
			TlSelect(0, 0);
		TlSetIndex(range.crange.index);
	}
	CControl::OnMouseMove(point);
}

void CTextBox::OnScroll(intx2 scroll, intx2 offset)
{
	Repaint();
	CControl::OnScroll(scroll, offset);
}

void CTextBox::UpdateScroll()
{
	tl_metrics_t metrics = TlGetMetrics();
	if(m_eTextWrap == TextWrapWordNoBreaking || m_eTextWrap == TextWrapNone)
		++metrics.tl_width;
	SetScroll(metrics.tl_width, metrics.tl_height);
}

void CTextBox::SetTextWrap(TextWrapE eWrapMode)
{
	if(eWrapMode != m_eTextWrap)
	{
		m_eTextWrap = eWrapMode;

		int_x iIndex = TlGetCluster(m_iIndex).trange.index;
		int_x iSelectIndex = 0, iSelectLength = 0;
		GetSelect(iSelectIndex, iSelectLength);
		TlBranch();
		m_iIndex = TlFindCluster(iIndex).cluster;
		if(iSelectLength)
		{
			m_iSelectIndex = TlFindCluster(iSelectIndex).cluster;
			m_iSelectLength = TlFindCluster(iSelectIndex + iSelectLength).cluster - m_iSelectIndex;
		}
		UpdateScroll();
		ScrollToCaret();
		Repaint();
	}
}

TextWrapE CTextBox::GetTextWrap() const
{
	return m_eTextWrap;
}

void CTextBox::SetReadOnly(bool bReadOnly)
{
	if(bReadOnly != m_bReadOnly)
	{
		m_bReadOnly = bReadOnly;
		UpdateIme();
		Repaint();
	}
}

bool CTextBox::IsReadOnly() const
{
	return m_bReadOnly;
}

void CTextBox::SetIndex(int_x iIndex)
{
	tl_cluster_t cluster = TlFindCluster(iIndex);
	TlSetIndex(cluster.cluster);
}

int_x CTextBox::GetIndex() const
{
	tl_cluster_t cluster = TlGetCluster(m_iIndex);
	return cluster.trange.index;
}

void CTextBox::SetSelect(int_x iIndex, int_x iLength)
{
	SelectRange(iIndex, iIndex + iLength);
}

bool CTextBox::GetSelect(int_x & iIndex, int_x & iLength) const
{
	if(m_iSelectLength)
	{
		tl_cluster_t cluster_beg = TlGetCluster(m_iSelectIndex);
		tl_cluster_t cluster_end = TlGetCluster(m_iSelectIndex + m_iSelectLength - 1);

		iIndex = cluster_beg.trange.index;
		iLength = cluster_end.trange.index + cluster_end.trange.length - cluster_beg.trange.index;
		return true;
	}
	else
	{
		iIndex = iLength = 0;
		return false;
	}
}

void CTextBox::ForceBlink(bool bBlink)
{
	m_bBlinking = bBlink;
	m_bForceBlink = bBlink;
	Repaint();
}

void CTextBox::BeginBlink()
{
	IApp * pps = GetApp();
	if(pps)
		pps->SetTimer(bind(this, &CTextBox::OnBlink), 500, 0);
}

void CTextBox::EndBlink()
{
	IApp * pps = GetApp();
	if(pps)
		pps->KillTimer(bind(this, &CTextBox::OnBlink), 0);
}

int_x CTextBox::OnBlink(int_x iTime)
{
	m_bBlinking = !m_bBlinking;
	Repaint();
	return 0;
}

void CTextBox::SelectNone()
{
	TlSelect(0, 0);
}

void CTextBox::SelectRange(int_x iIndex1, int_x iIndex2)
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

void CTextBox::SelectAll()
{
	SetSelect(0, m_text.length());
}

bool CTextBox::IsSelectAll() const
{
	if(m_text.is_empty())
		return false;
	else if(!m_iSelectLength)
		return false;
	else
	{
		int_x iIndex = 0, iLength = 0;
		GetSelect(iIndex, iLength);
		return iIndex == 0 && iLength == m_text.length();
	}
}

bool CTextBox::HasTextSelected() const
{
	return m_iSelectLength != 0;
}

void CTextBox::DoDelete()
{
	if(m_iSelectLength)
		TlDeleteText(m_iSelectIndex, m_iSelectLength);
	else
	{
		int_x iIndex = m_iIndex;
		int_x iCount = TlGetClusterCount();
		while(iIndex < iCount)
		{
			tl_cluster_t cluster = TlGetCluster(iIndex);
			if(!cluster.trange.length)
				++iIndex;
			else
			{
				TlDeleteText(iIndex, 1);
				break;
			}
		}
	}
}

int_x CTextBox::GetSelectedText(char_16 * pText, int_x iSize) const
{
	if(!m_iSelectLength)
	{
		textempty(pText, iSize);
		return 0;
	}
	else
	{
		int_x iIndex = 0, iLength = 0;
		GetSelect(iIndex, iLength);
		return textcpy(pText, iSize, m_text.buffer() + iIndex, iLength);
	}
}

void CTextBox::DeleteSelected()
{
	if(m_iSelectLength)
		TlDeleteText(m_iSelectIndex, m_iSelectLength);
}

void CTextBox::DoBackSpace()
{
	if(m_iSelectLength)
	{
		TlDeleteText(m_iSelectIndex, m_iSelectLength);
	}
	else
	{
		int_x iIndex = m_iIndex - 1;
		while(iIndex >= 0)
		{
			tl_cluster_t cluster = TlGetCluster(iIndex);
			if(!cluster.trange.length)
				--iIndex;
			else
			{
				TlDeleteText(iIndex, 1);
				break;
			}
		}
	}
}

void CTextBox::DoClear()
{
	TlSelect(0, 0);
	TlSetIndex(0);
	TlSetLastIndex(0);

	TlClear();
	UpdateScroll();
	ScrollToCaret();
	Repaint();
}

void CTextBox::DoCopy() const
{
	if(m_iSelectLength)
	{
		tl_cluster_t cluster_beg = TlGetCluster(m_iSelectIndex);
		tl_cluster_t cluster_end = TlGetCluster(m_iSelectIndex + m_iSelectLength - 1);

		ClipboardSetText16(m_text.buffer() + cluster_beg.trange.index,
			cluster_end.trange.index + cluster_end.trange.length - cluster_beg.trange.index);
	}
}

void CTextBox::DoPaste()
{
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
			break;
		case '\n':
			text.append(L'\r');
			text.append(L'\n');
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

void CTextBox::DoCut()
{
	if(HasTextSelected())
	{
		DoCopy();
		DoDelete();
	}
}

void CTextBox::AddText(const char_16 * szText, int_x iLength)
{
	if(iLength < 0)
		iLength = textlen(szText);
	if(!iLength)
		return;

	InsertText(m_text.length(), szText, iLength);
}

void CTextBox::InsertText(int_x iIndex, const char_16 * szText, int_x iLength)
{
	if(iLength < 0)
		iLength = textlen(szText);
	if(!iLength)
		return;

	tl_cluster_t cluster = TlFindCluster(iIndex);
	TlInsertText(cluster.cluster, szText, iLength);
}

void CTextBox::TlCaretTo(int_x iIndex)
{
	if(IsKeyDown(KeyCodeShift))
	{
		if(iIndex >= 0)
		{
			TlSelectRange(m_iLastIndex, iIndex);
			TlSetIndex(iIndex);
		}
	}
	else
	{
		SelectNone();
		if(iIndex >= 0)
		{
			TlSetLastIndex(iIndex);
			TlSetIndex(iIndex);
		}
	}
}

void CTextBox::CaretLeft()
{
	int_x iIndex = m_iIndex - 1;
	while(iIndex >= 0)
	{
		tl_cluster_t cluster = TlGetCluster(iIndex);
		if(cluster.linetag && !cluster.paragraphtag)
			--iIndex;
		else
		{
			TlCaretTo(iIndex);
			break;
		}
	}
}

void CTextBox::CaretRight()
{
	int_x iCount = TlGetClusterCount();
	int_x iIndex = m_iIndex + 1;
	while(iIndex < iCount)
	{
		tl_cluster_t cluster = TlGetCluster(iIndex - 1);
		if(cluster.linetag && !cluster.paragraphtag)
			++iIndex;
		else
		{
			TlCaretTo(iIndex);
			break;
		}
	}
}

void CTextBox::CaretHome()
{
	tl_cluster_t cluster = TlGetCluster(m_iIndex);
	tl_line_t line = TlGetLine(cluster.line);

	TlCaretTo(line.crange.index);
}

void CTextBox::CaretEnd()
{
	tl_cluster_t cluster = TlGetCluster(m_iIndex);
	tl_line_t line = TlGetLine(cluster.line);

	TlCaretTo(line.crange.index + line.crange.length - 1);
}

void CTextBox::CaretTop()
{
	TlCaretTo(0);
}

void CTextBox::CaretBottom()
{
	TlCaretTo(TlGetClusterCount() - 1);
}

void CTextBox::CaretUp()
{
}

void CTextBox::CaretDown()
{
}

void CTextBox::OnSelectedTextChanged()
{
	Repaint();
	SelectedTextChanged(this);
}

void CTextBox::ScrollToCaret()
{
	sizeix siClient = GetClient().size;
	tl_cluster_t cluster = TlGetCluster(m_iIndex);
	pointix sclText = GetScrollPos();
	int_x iTop = cluster.tlrect.y - sclText.y;
	int_x iBottom = iTop + cluster.tlrect.h;
	int_x iX = cluster.tlrect.x - sclText.x;
	if(iX < 0)
		sclText.x = cluster.tlrect.x;
	else if(iX >= siClient.w)
		sclText.x = sclText.x + iX - siClient.w + 1;
	else {}

	if(iTop < 0)
		sclText.y = cluster.tlrect.y;
	else if(iBottom > siClient.h)
		sclText.y = cluster.tlrect.y + cluster.tlrect.h - siClient.h;
	else {}

	SetScrollPos(sclText.x, sclText.y);
}

rectix CTextBox::_GetCaretRect(int_x iIndex) const
{
	tl_cluster_t cluster = TlGetCluster(iIndex);
	rectix rect;
	rect.x = cluster.tlrect.x;
	rect.y = cluster.tlrect.y;
	rect.w = cluster.tlrect.w;
	rect.h = cluster.tlrect.h;
	return rect;
}

void CTextBox::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	rectix rcClient = GetClient();
	pointix sclText = GetScrollPos();
	if(m_iSelectLength)
	{
		color_t clrHl = IsFocused() ? SysColorTextSelectNormal : SysColorTextSelectDisable;
		int_x iCount = TlGetRanges(m_iSelectIndex, m_iSelectLength, nullptr, 0, sclText.y, rcClient.h);
		vector<tl_range_t> ranges(iCount, iCount);
		TlGetRanges(m_iSelectIndex, m_iSelectLength, ranges, iCount, sclText.y, rcClient.h);
		for(int_x cnt = 0; cnt < iCount; ++cnt)
		{
			const tl_range_t & range = ranges[cnt];
			pPaint->FillRect(-sclText.x + range.tlrect.x, -sclText.y + range.tlrect.y, range.tlrect.w, range.tlrect.h, clrHl);
		}
	}

	uint_32 clrHlText = m_uiForeColor;
	if(!IsAviliable())
		clrHlText = SysColorForeDisable;

	int_x iY = -sclText.y;
	for(int_x cnt = 0, len = m_paras.size(); cnt < len; ++cnt)
	{
		const txb_paragraph_t & para = m_paras[cnt];
		if(iY + para.tlrect.h < rcClip.y)
		{
			iY += para.tlrect.h;
			continue;;
		}
		else if(iY >= rcClip.bottom())
			break;

		pPaint->DrawTextLayout(para.pLayout, pointix(-sclText.x, iY), rcClip, m_uiForeColor);
		iY += para.tlrect.h;
	}

	if(m_bBlinking)
	{
		rectix rcCaret = _GetCaretRect(m_iIndex);
		pPaint->DrawLine(pointix(rcCaret.x - sclText.x, rcCaret.y - sclText.y),
			pointix(rcCaret.x - sclText.x, rcCaret.y - sclText.y + rcCaret.h - 1),
			/*clrHlText*/Colors::Red);
	}
}

doc_source_t CTextBox::GetDocSource() const
{
	doc_source_t source = {m_text.length(), m_text.buffer()};
	return source;
}

textformat_t CTextBox::GetDefFormat() const
{
	return textformat_t(m_font, m_eTextWrap);
}

void CTextBox::QueryDebugMenu(int_x & iBase, IMenu * pMenu) const
{
	CControl::QueryDebugMenu(iBase, pMenu);
	MenuT * pSubMenu = new MenuT();
	MenuT * pWrapMenu = new MenuT();
	pSubMenu->AddItem(iBase + 1, L"Clear");
	pSubMenu->AddItem(iBase + 2, L"TextReadOnly", m_bReadOnly);
	pSubMenu->AddItem(0, L"TextWrapMode", pWrapMenu);
	pMenu->AddSplit();
	pMenu->AddItem(0, L"TextBox", pSubMenu);

	pWrapMenu->AddItem(iBase + 3, L"TextWrapNone", m_eTextWrap == TextWrapNone);
	pWrapMenu->AddItem(iBase + 4, L"TextWrapChar", m_eTextWrap == TextWrapChar);
	pWrapMenu->AddItem(iBase + 5, L"TextWrapWord", m_eTextWrap == TextWrapWord);
	pWrapMenu->AddItem(iBase + 6, L"TextWrapWordNoBreaking", m_eTextWrap == TextWrapWordNoBreaking);
	SafeRelease(pSubMenu);
	SafeRelease(pWrapMenu);
	iBase += 10;
}

void CTextBox::OnDebugMenu(int_x iBase, int_x iResult)
{
	iBase -= 10;
	switch(iResult - iBase)
	{
	case 1:
		DoClear();
		break;
	case 2:
		SetReadOnly(!m_bReadOnly);
		break;
	case 3:
	case 4:
	case 5:
	case 6:
		SetTextWrap((TextWrapE)(iResult - iBase - 3));
		break;
	default:
		CControl::OnDebugMenu(iBase, iResult);
		break;
	}
}



//--------------------------------------------------------------------------------------------------
err_t CTextBox::TlLayout()
{
	int_x iWidth = maxof(GetClient().w - 1, (int_x)1);
	for(int_x cnt = 0, len = m_paras.size(); cnt < len; ++cnt)
		SafeRelease(m_paras[cnt].pLayout);
	m_paras.clear();

	if(m_text.is_empty())
	{
		txb_paragraph_t para;
		para.pLayout = nullptr;
		para.para = 0;
		para.trange = {0, 0};
		para.crange = {0, 1};
		para.lrange = {0, 1};
		para.tlrect = {0, 0, 0, m_font.size};
		para.tag = paragraph_tag_none;
		m_paras.add(para);
		return err_ok;
	}

	int_x cluster_count = 0;
	int_x iy = 0, itext = 0, icluster = 0, iline = 0;
	vector<doc_paragraph_t> paragraphs = TextParseParagraphs(m_text, m_text.length());
	if(paragraphs.is_empty())
	{
		txb_paragraph_t para;
		para.pLayout = nullptr;
		para.para = 0;
		para.trange = {0, 0};
		para.crange = {0, 1};
		para.lrange = {0, 1};
		para.tlrect = {0, 0, 0, m_font.size};
		para.tag = paragraph_tag_none;
		m_paras.add(para);
	}
	else
	{
		for(int_x ipara = 0; ipara < paragraphs.size(); ++ipara)
		{
			const doc_paragraph_t & docpara = paragraphs[ipara];
			ITextLayout * pLayout = Get2DDevice()->CreateTextLayout();
			pLayout->Initialize(this);
			err_t err = pLayout->Layout({docpara.trange.index, docpara.trange.length - GetParagraphTagLength(docpara.tag)}, iWidth, docpara.tag);
			if(err)
				return err;

			tl_metrics_t metric = pLayout->GetMetrics();

			txb_paragraph_t & para = m_paras.add();
			para.pLayout = pLayout;
			para.para = ipara;
			para.trange = {itext, docpara.trange.length};
			para.crange = {icluster, metric.cluster_length};
			para.lrange = {iline, metric.line_length};
			para.tlrect = {0, iy, metric.tl_width, metric.tl_height};
			para.tag = docpara.tag;

			itext += docpara.trange.length;
			icluster += metric.cluster_length;
			iline += metric.line_length;
			iy += metric.tl_height;
		}
	}
	return 0;
}

err_t CTextBox::TlBranch()
{
	int_x iWidth = GetClient().w - 1;
	int_x y = 0, icluster = 0;
	for(int_x cnt = 0, len = m_paras.size(); cnt < len; ++cnt)
	{
		txb_paragraph_t & para = m_paras[cnt];
		if(para.pLayout)
		{
			para.pLayout->Branch(iWidth);

			tl_metrics_t metric = para.pLayout->GetMetrics();
			para.crange = {icluster, metric.cluster_length};
			para.tlrect.y = y;
			para.tlrect.w = metric.tl_width;
			para.tlrect.h = metric.tl_height;
		}
		else
		{
			para.crange = {icluster, 1};
			para.tlrect.y = y;
			para.tlrect.w = 0;
			para.tlrect.h = m_font.size;
		}

		y += para.tlrect.h;
		icluster += para.crange.length;
	}
	return err_ok;
}

err_t CTextBox::TlClear()
{
	for(int_x cnt = 0, len = m_paras.size(); cnt < len; ++cnt)
		SafeRelease(m_paras[cnt].pLayout);
	m_paras.clear();
	return err_ok;
}

err_t CTextBox::TlInsertText(int_x iIndex, const char_16 * szText, int_x iLength)
{
	if(iLength < 0)
		iLength = textlen(szText);
	if(!iLength)
		return err_ok;

	int_x iWidth = GetClient().w - 1;

	tl_cluster_t cluster = TlGetCluster(iIndex);
	int_x iIndexNew = cluster.trange.index + iLength;

	m_text.insert(cluster.trange.index, szText, iLength);

	// 定位受影响的段落，该段落会被一分为二，前半部分与插入的 szText 中第一段落合并，
	// 后半部分与插入的 szText 最后一段落合并。

	// 受影响的 text point。
	int_x iY = 0, tIndex = 0, tLength = 0, cIndex = 0, pIndex = cluster.para, lIndex = 0;
	if(cluster.para > 0)
	{
		txb_paragraph_t & paraLast = m_paras[cluster.para - 1];
		tIndex = paraLast.trange.index + paraLast.trange.length;
		cIndex = paraLast.crange.index + paraLast.crange.length;
		lIndex = paraLast.lrange.index + paraLast.lrange.length;
		iY = paraLast.tlrect.y + paraLast.tlrect.h;
	}
	else
	{
		tIndex = 0;
		cIndex = 0;
		lIndex = 0;
		iY = 0;
	}

	bool bNewParagraph = true;
	if(cluster.para < m_paras.size() - 1)
	{
		txb_paragraph_t & paraNext = m_paras[cluster.para + 1];
		tLength = paraNext.trange.index + iLength - tIndex;
		bNewParagraph = false;
	}
	else
		tLength = m_text.length() - tIndex;

	if(cluster.para == 0 && m_paras.is_empty())
		;
	else
	{
		SafeRelease(m_paras[cluster.para].pLayout);
		m_paras.remove_at(cluster.para);
	}

	// 插入的段落，将位于 iIndexBeg 和 iIndexEnd 之间.

	vector<doc_paragraph_t> paragraphs = TextParseParagraphs(m_text.buffer() + tIndex, tLength);
	int_x iParaCount = bNewParagraph ? paragraphs.size() : paragraphs.size() - 1;
	for(int_x cnt = 0; cnt < iParaCount; ++cnt)
	{
		const doc_paragraph_t & docpara = paragraphs[cnt];
		ITextLayout * pLayout = Get2DDevice()->CreateTextLayout();
		pLayout->Initialize(this);
		pLayout->Layout({tIndex, docpara.trange.length - GetParagraphTagLength(docpara.tag)}, iWidth, docpara.tag);
		tl_metrics_t metric = pLayout->GetMetrics();

		txb_paragraph_t & para = m_paras.insert_at(cluster.para + cnt);

		para.pLayout = pLayout;
		para.para = pIndex++;
		para.trange = {tIndex, docpara.trange.length};
		para.crange = {cIndex, metric.cluster_length};
		para.lrange = {lIndex, metric.line_length};
		para.tlrect = {0, iY, metric.tl_width, metric.tl_height};

		tIndex += docpara.trange.length;
		cIndex += metric.cluster_length;
		lIndex += metric.line_length;
		iY += metric.tl_height;
	}

	for(int_x cnt = cluster.para + iParaCount; cnt < m_paras.size(); ++cnt)
	{
		txb_paragraph_t & para = m_paras[cnt];

		para.para = pIndex++;
		para.trange = {tIndex, para.trange.length};
		para.crange = {cIndex, para.crange.length};
		para.lrange = {lIndex, para.lrange.length};
		para.tlrect = {0, iY, para.tlrect.w, para.tlrect.h};

		tIndex += para.trange.length;
		cIndex += para.crange.length;
		lIndex += para.lrange.length;
		iY += para.tlrect.h;
	}

	TlSelect(0, 0);
	tl_cluster_t cluster_caret = TlFindCluster(iIndexNew);
	TlSetIndex(cluster_caret.cluster);
	TlSetLastIndex(cluster_caret.cluster);
	UpdateScroll();
	ScrollToCaret();
	Repaint();
	return err_ok;
}

err_t CTextBox::TlDeleteText(int_x iIndex, int_x iLength)
{
	if(!iLength)
		return err_ok;

	int_x iWidth = GetClient().w - 1;

	tl_metrics_t metrics = TlGetMetrics();
	tl_cluster_t cluster_beg = TlGetCluster(iIndex);
	tl_cluster_t cluster_end = TlGetCluster(iIndex + iLength - 1);
	tl_para_t para_beg = TlGetPara(cluster_beg.para);
	tl_para_t para_end;
	if(cluster_end.paragraphtag && cluster_end.para < m_paras.size() - 1)
		para_end = TlGetPara(cluster_end.para + 1);
	else
		para_end = TlGetPara(cluster_end.para);

	int_x iTextLength = cluster_end.trange.index + cluster_end.trange.length - cluster_beg.trange.index;
	bool bNewParagraph = para_end.para == m_paras.size() - 1;

	m_text.remove(cluster_beg.trange.index, iTextLength);
	for(int_x cnt = para_beg.para; cnt <= para_end.para; ++cnt)
		SafeRelease(m_paras[cnt].pLayout);
	m_paras.remove_at(para_beg.para, para_end.para - para_beg.para + 1);

	int_x pIndex = para_beg.para;
	int_x tIndex = para_beg.trange.index;
	int_x tLength = para_end.trange.index + para_end.trange.length - para_beg.trange.index - iTextLength;
	int_x cIndex = para_beg.crange.index;
	int_x lIndex = para_beg.lrange.index;
	int_x iY = para_beg.tlrect.y;

	vector<doc_paragraph_t> paragraphs = TextParseParagraphs(m_text.buffer() + tIndex, tLength);
	int_x iParaCount = bNewParagraph ? paragraphs.size() : paragraphs.size() - 1;
	for(int_x cnt = 0; cnt < iParaCount; ++cnt)
	{
		const doc_paragraph_t & docpara = paragraphs[cnt];

		ITextLayout * pLayout = Get2DDevice()->CreateTextLayout();
		pLayout->Initialize(this);
		pLayout->Layout({tIndex, docpara.trange.length - GetParagraphTagLength(docpara.tag)}, iWidth, docpara.tag);
		tl_metrics_t metric = pLayout->GetMetrics();

		txb_paragraph_t & para = m_paras.insert_at(pIndex);

		para.pLayout = pLayout;
		para.para = pIndex;

		para.trange = {tIndex, docpara.trange.length};
		para.crange = {cIndex, metric.cluster_length};
		para.lrange = {lIndex, metric.line_length};
		para.tlrect = {0, iY, metric.tl_width, metric.tl_height};
		para.tag = docpara.tag;


		pIndex += 1;
		tIndex += para.trange.length;
		cIndex += para.crange.length;
		lIndex += para.lrange.length;
		iY += para.tlrect.h;
	}

	for(int_x cnt = para_beg.para + iParaCount; cnt < m_paras.size(); ++cnt)
	{
		txb_paragraph_t & para = m_paras[cnt];

		para.para = pIndex;
		para.trange = {tIndex, para.trange.length};
		para.crange = {cIndex, para.crange.length};
		para.lrange = {lIndex, para.lrange.length};
		para.tlrect.y = iY;

		pIndex += 1;
		tIndex += para.trange.length;
		cIndex += para.crange.length;
		lIndex += para.lrange.length;
		iY += para.tlrect.h;
	}

	TlSelect(0, 0);
	TlSetIndex(iIndex);
	TlSetLastIndex(iIndex);
	UpdateScroll();
	ScrollToCaret();
	Repaint();
	return err_ok;
}

int_x CTextBox::TlGetClusterCount() const
{
	return TlGetMetrics().cluster_length;
}

tl_cluster_t CTextBox::TlFindCluster(int_x iIndex) const
{
	if(iIndex == 0 && m_paras.is_empty())
	{
		tl_cluster_t cluster = {};
		cluster.tlrect.h = m_font.size;
		return cluster;
	}

	for(int_x cnt = 0, size = m_paras.size(); cnt < size; ++cnt)
	{
		const txb_paragraph_t & para = m_paras[cnt];
		bool match = para.trange.index <= iIndex && iIndex < para.trange.index + para.trange.length;
		if(!match)
			match = cnt == size - 1 && iIndex == para.trange.index + para.trange.length;

		if(!match)
			continue;

		if(para.pLayout)
		{
			tl_cluster_t cluster = para.pLayout->FindCluster(iIndex - para.trange.index);
			cluster.cluster += para.crange.index;
			cluster.line += para.lrange.index;
			cluster.para = para.para;

			cluster.trange.index += para.trange.index;
			cluster.tlrect.y += para.tlrect.y;

			return cluster;
		}
		else
		{
			Assert(iIndex == para.crange.index && para.crange.length == 1);
			tl_cluster_t cluster = {};
			cluster.tlrect.h = m_font.size;

			cluster.cluster += para.crange.index;
			cluster.line += para.lrange.index;
			cluster.para = para.para;
			cluster.trange.index += para.trange.index;
			cluster.tlrect.y += para.tlrect.y;

			return cluster;
		}
	}
	throw exp_out_of_bound();
}

tl_cluster_t CTextBox::TlGetCluster(int_x iIndex) const
{
	if(iIndex == 0 && m_paras.is_empty())
	{
		tl_cluster_t cluster = {};
		cluster.tlrect.h = m_font.size;
		return cluster;
	}

	for(int_x ipara = 0, size = m_paras.size(); ipara < size; ++ipara)
	{
		const txb_paragraph_t & para = m_paras[ipara];
		if(para.crange.index <= iIndex && iIndex < para.crange.index + para.crange.length)
		{
			if(para.pLayout)
			{
				tl_cluster_t cluster = para.pLayout->GetCluster(iIndex - para.crange.index);

				cluster.cluster += para.crange.index;
				cluster.line += para.lrange.index;
				cluster.para = para.para;

				cluster.trange.index += para.trange.index;
				cluster.tlrect.y += para.tlrect.y;

				return cluster;
			}
			else
			{
				Assert(iIndex == para.crange.index && para.crange.length == 1);
				tl_cluster_t cluster = {};
				cluster.tlrect.h = m_font.size;

				cluster.cluster += para.crange.index;
				cluster.line += para.lrange.index;
				cluster.para = para.para;
				cluster.trange.index += para.trange.index;
				cluster.tlrect.y += para.tlrect.y;

				return cluster;
			}
		}
	}
	throw exp_out_of_bound();
}

tl_line_t CTextBox::TlGetLine(int_x iIndex) const
{
	if(!iIndex && m_paras.is_empty())
	{
		tl_line_t line;
		line.line = 0;
		line.para = 0;
		line.trange = {0, 0};
		line.crange = {0, 1};
		line.tlrect = {0, 0, 0, m_font.size};
		return line;
	}

	for(int_x ipara = 0, size = m_paras.size(); ipara < size; ++ipara)
	{
		const txb_paragraph_t & para = m_paras[ipara];
		if(para.lrange.index <= iIndex && iIndex < para.lrange.index + para.lrange.length)
		{
			if(!para.pLayout)
			{
				tl_line_t line;
				line.line = para.lrange.index;
				line.para = ipara;
				line.trange = para.trange;
				line.crange = para.crange;
				line.tlrect = para.tlrect;
				return line;
			}
			else
			{
				tl_line_t line = para.pLayout->GetLine(iIndex - para.lrange.index);
				line.line += para.lrange.index;
				line.para = ipara;
				line.trange.index += para.trange.index;
				line.crange.index += para.crange.index;
				line.tlrect.y += para.tlrect.y;
				return line;
			}
		}
	}
	throw exp_out_of_bound();
}

tl_para_t CTextBox::TlGetPara(int_x iIndex) const
{
	if(!iIndex && m_paras.is_empty())
	{
		tl_para_t para;
		para.para = 0;
		para.trange = {0, 0};
		para.crange = {0, 1};
		para.lrange = {0, 1};
		para.tlrect = {0, 0, 0, m_font.size};
		return para;
	}

	const txb_paragraph_t & tpara = m_paras[iIndex];
	tl_para_t para = {tpara.para, tpara.trange, tpara.crange, tpara.lrange, tpara.tlrect};
	return para;
}

tl_range_t CTextBox::TlGetRange(pointix point) const
{
	if(m_paras.is_empty())
	{
		tl_range_t range = {};
		return range;
	}

	int_x iParagraph = -1;
	int_x iIndexBase = 0;

	if(point.y <= 0)
		iParagraph = 0;
	else
	{
		// 二分查找
		int_x iTop = 0, iBottom = 0;
		int_x iLen = m_paras.size();
		int_x iBeg = 0;
		int_x iEnd = m_paras.size();
		int_x iIndex = 0;
		while(true)
		{
			iIndex = (iBeg + iEnd) / 2;
			const txb_paragraph_t & para = m_paras[iIndex];
			iTop = para.tlrect.y;
			iBottom = iTop + para.tlrect.h;
			if((iTop <= point.y && point.y < iBottom) || (iIndex == iLen - 1 && iEnd - iBeg <= 1))
			{
				iParagraph = iIndex;
				break;
			}
			else if(iBeg == iEnd)
				break;
			else
			{
				if(point.y < iTop)
					iEnd = iIndex;
				else
					iBeg = iIndex;
			}
		}

		if(iParagraph < 0)
			iParagraph = m_paras.size() - 1;
	}

	const txb_paragraph_t & para = m_paras[iParagraph];
	if(para.pLayout)
	{
		tl_range_t range = para.pLayout->HitTest(pointix(point.x, point.y - para.tlrect.y));
		range.line += para.lrange.length;
		range.para += para.para;
		range.trange.index += para.trange.index;
		range.crange.index += para.crange.index;
		range.tlrect.y += para.tlrect.y;
		return range;
	}
	else
	{
		tl_range_t range = {};
		range.crange = {0, 1};
		range.tlrect = {0, 0, 0, m_font.size};

		range.line += para.lrange.length;
		range.para += para.para;
		range.trange.index += para.trange.index;
		range.crange.index += para.crange.index;
		range.tlrect.y += para.tlrect.y;
		return range;
	}
}

int_x CTextBox::TlGetRanges(int_x iCluster, int_x iLength, tl_range_t * ranges, int_x iCount) const
{
	return TlGetRanges(iCluster, iLength, ranges, iCount, 0, IX_MAX);
}

int_x CTextBox::TlGetRanges(int_x iCluster, int_x iLength, tl_range_t * ranges, int_x iCount, int_x iY, int_x iHeight) const
{
	if(m_paras.is_empty())
		return 0;

	int_x iResult = 0;
	for(int_x cnt = 0, len = m_paras.size(); cnt < len; ++cnt)
	{
		const txb_paragraph_t & para = m_paras[cnt];
		if(para.tlrect.y + para.tlrect.h < iY)
			continue;
		if(para.tlrect.y >= iY + iHeight)
			break;

		int_x iBeg = maxof(iCluster, para.crange.index);
		int_x iEnd = minof(para.crange.index + para.crange.length, iCluster + iLength);

		if(iBeg < iEnd)
		{
			int_x iCountTemp = 0;
			if(!ranges)
				iCountTemp = para.pLayout->HitTestRange(iBeg - para.crange.index, iEnd - iBeg, nullptr, 0);
			else
			{
				iCountTemp = para.pLayout->HitTestRange(iBeg - para.crange.index, iEnd - iBeg, ranges + iResult, iCount - iResult);
				for(int_x irange = 0; irange < iCountTemp; ++irange)
				{
					tl_range_t & range = ranges[iResult + irange];
					range.tlrect.y += para.tlrect.y;
				}
			}

			if(iCountTemp < 0)
			{
				// iCount 不够了
			}
			else
				iResult += iCountTemp;
		}
	}

	return iResult;
}

tl_metrics_t CTextBox::TlGetMetrics() const
{
	tl_metrics_t metrics = {};
	for(int_x cnt = 0, len = m_paras.size(); cnt < len; ++cnt)
	{
		const txb_paragraph_t & para = m_paras[cnt];
		metrics.length += para.trange.length;
		if(para.tlrect.w > metrics.tl_width)
			metrics.tl_width = para.tlrect.w;
		metrics.tl_height += para.tlrect.h;
		metrics.cluster_length += para.crange.length;
		metrics.line_length += para.lrange.length;
	}
	return metrics;
}

void CTextBox::TlSetIndex(int_x iIndex)
{
	if(iIndex != m_iIndex)
	{
		m_iIndex = iIndex;
		ForceBlink(IsFocused());
		ScrollToCaret();
		UpdateIme();
	}
}

void CTextBox::TlSetLastIndex(int_x iIndex)
{
	m_iLastIndex = iIndex;
}

void CTextBox::TlSelect(int_x iIndex, int_x iLength)
{
	if(m_iSelectIndex != iIndex || m_iSelectLength != iLength)
	{
		m_iSelectIndex = iIndex;
		m_iSelectLength = iLength;
		OnSelectedTextChanged();
		Repaint();
	}
}

void CTextBox::TlSelectRange(int_x iIndex1, int_x iIndex2)
{
	if(iIndex1 < iIndex2)
		TlSelect(iIndex1, iIndex2 - iIndex1);
	else
		TlSelect(iIndex2, iIndex1 - iIndex2);
}

VENUS_END
