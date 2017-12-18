#include "stdafx.h"
#include "CControl.h"

VENUS_BEG

static const int_x CONTROL_MAX_W = I32_MAX / 4;
static const int_x CONTROL_MAX_H = I32_MAX / 4;

CControl::CControl()
	: CControl(0, 0, 0, 0)
{
}

CControl::CControl(int_x iX, int_x iY, int_x iWidth, int_x iHeight)
	: m_rect(iX, iY, iWidth, iHeight), m_fWeight(0.0f), m_eWidthMode(WHModeAbs), m_eHeightMode(WHModeAbs),
	m_pParent(nullptr), m_iId(0), m_iZOrder(0), m_eBorderType(BorderTypeNone), m_eLayoutMode(LayoutModeAbsolute), m_iLayouting(0),
	//m_minSize(0, 0), m_maxSize(CONTROL_MAX_W, CONTROL_MAX_H),
	m_uiAutoRepaint(0), m_uiBaseAttr(0), m_uiMouseState(0), m_uiState(0),
	m_uiBackColor(SysColorAuto), m_uiForeColor(Colors::Black),
	m_eCursor(CursorNormal),
	m_pMouseControl(nullptr), m_pSelectedControl(nullptr), m_pDropControl(nullptr),
	m_pCaptureControl(nullptr), m_eMouseWheelMode(MouseWheelModeHoving),
	m_pScrollX(nullptr), m_pScrollY(nullptr), m_eImeMode(ImeModeDisable)
{
}

CControl::~CControl()
{
	for(IControl * pChild : m_DynamicChildren)
		SafeRelease(pChild);
	m_DynamicChildren.clear();
	m_children.clear();
}

const cid_t & CControl::GetOid() const
{
	return OID_Control;
}

void CControl::GetMeta(IMetaHelper * pHelper)
{
	pHelper->help_evt(L"VisibleChanged", VisibleChanged);
	pHelper->help_evt(L"EnableChanged", EnableChanged);
	pHelper->help_evt(L"SelectedChanged", SelectedChanged);
	pHelper->help_evt(L"FocusedChanged", FocusedChanged);
	pHelper->help_evt(L"CapturedChanged", CapturedChanged);

	pHelper->help_evt(L"TextChanged", TextChanged);
	pHelper->help_evt(L"FontChanged", FontChanged);

	pHelper->help_evt(L"PositionChanged", PositionChanged);
	pHelper->help_evt(L"SizeChanged", SizeChanged);

	pHelper->help_evt(L"MouseMove", MouseMove);
	pHelper->help_evt(L"MouseIn", MouseIn);
	pHelper->help_evt(L"MouseOut", MouseOut);
	pHelper->help_evt(L"MouseDownL", MouseDownL);
	pHelper->help_evt(L"MouseDownM", MouseDownM);
	pHelper->help_evt(L"MouseDownR", MouseDownR);
	pHelper->help_evt(L"MouseUpL", MouseUpL);
	pHelper->help_evt(L"MouseUpM", MouseUpM);
	pHelper->help_evt(L"MouseUpR", MouseUpR);
	pHelper->help_evt(L"MouseClickL", MouseClickL);
	pHelper->help_evt(L"MouseClickM", MouseClickM);
	pHelper->help_evt(L"MouseClickR", MouseClickR);
	pHelper->help_evt(L"MouseDBClickL", MouseDBClickL);

	pHelper->help_evt(L"MouseWheelX", MouseWheelX);
	pHelper->help_evt(L"MouseWheelY", MouseWheelY);

	pHelper->help_evt(L"KeyDown", KeyDown);
	pHelper->help_evt(L"KeyUp", KeyUp);

	pHelper->help_evt(L"DragEnter", DragEnter);
	pHelper->help_evt(L"DragMove", DragMove);
	pHelper->help_evt(L"DragDrop", DragDrop);
	pHelper->help_evt(L"DragLeave", DragLeave);
}

void CControl::SetParent(IControl * pParent)
{
	if(pParent != m_pParent)
	{
		m_pParent = pParent;
	}
}

IControl * CControl::GetParent() const
{
	return m_pParent;
}

IControl * CControl::GetAncestor() const
{
	if(ToIForm())
		return (IControl *)this;
	else if(m_pParent)
		return m_pParent->GetAncestor();
	else
		return (IControl *)this;
}

IForm * CControl::GetForm() const
{
	if(m_pParent)
		return m_pParent->GetForm();
	else
		return nullptr;
}

void CControl::SetMargin(edgeix margin)
{
	m_margin = margin;
}

edgeix CControl::GetMargin() const
{
	return m_margin;
}

void CControl::SetPadding(const edgeix & padding)
{
	m_padding = padding;
}

edgeix CControl::GetPadding() const
{
	return m_padding;
}

void CControl::SetWeight(float_32 fWeight)
{
	m_fWeight = fWeight;
}

float_32 CControl::GetWeight() const
{
	return m_fWeight;
}

void CControl::SetWidthMode(WHModeE eMode)
{
	m_eWidthMode = eMode;
	if((m_eWidthMode == WHModeFill || m_eWidthMode == WHModeAuto) && (m_anchor.type & AlignLR))
	{
		log0(L"CControl::SetWidthMode with WHModeFill or WHModeAuto, but m_eAnchor is AlignLR.");
		anchor_t anchor = m_anchor;
		anchor.type &= ~AlignLR;
		SetAnchor(anchor);
	}
}

WHModeE CControl::GetWidthMode() const
{
	return m_eWidthMode;
}

void CControl::SetHeightMode(WHModeE eMode)
{
	m_eHeightMode = eMode;
	if((m_eHeightMode == WHModeFill || m_eHeightMode == WHModeAuto) && (m_anchor.type & AlignTB))
	{
		log0(L"CControl::SetHeightMode with WHModeFill or WHModeAuto, but m_eAnchor is AlignTB.");
		anchor_t anchor = m_anchor;
		anchor.type &= ~AlignTB;
		SetAnchor(anchor);
	}
}

WHModeE CControl::GetHeightMode() const
{
	return m_eHeightMode;
}

rectix CControl::GetRect() const
{
	return m_rect;
}

void CControl::SetRect(const rectix & rect)
{
	SetPosition(rect.position);
	SetSize(rect.size);
}

void CControl::SetRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight)
{
	SetRect(rectix(iX, iY, iWidth, iHeight));
}

void CControl::SetPosition(pointix position)
{
	if(position != m_rect.position)
	{
		m_rect.position = position;
		OnPositionChanged();
	}
}

void CControl::SetPosition(int_x iX, int_x iY)
{
	SetPosition(pointix(iX, iY));
}

void CControl::SetSize(sizeix size)
{
	if(size != m_rect.size)
	{
		m_rect.size = size;
		OnSizeChanged();
	}
}

void CControl::SetSize(int_x iWidth, int_x iHeight)
{
	SetSize(sizeix(iWidth, iHeight));
}

pointix CControl::GetPosition() const
{
	return pointix(m_rect.x, m_rect.y);
}

sizeix CControl::GetSize() const
{
	return sizeix(m_rect.w, m_rect.h);
}

void CControl::SetX(int_x iX)
{
	SetPosition(pointix(iX, m_rect.y));
}

void CControl::SetY(int_x iY)
{
	SetPosition(pointix(m_rect.x, iY));
}

void CControl::SetWidth(int_x iWidth)
{
	if(iWidth != m_rect.w)
	{
		SetSize(sizeix(iWidth, m_rect.h));
	}
}
void CControl::SetHeight(int_x iHeight)
{
	if(iHeight != m_rect.h)
	{
		SetSize(sizeix(m_rect.w, iHeight));
	}
}

pointix CControl::GetRelativePosition(IControl * pAncestor) const
{
	pointix point = GetClient().position;
	if(pAncestor == this)
		return point;

	point += m_rect.position;
	if(m_pParent)
		point += m_pParent->GetRelativePosition(pAncestor);
	return point;
}

sizeix CControl::GetPreferedSize() const
{
	return sizeix(m_font.size, m_font.size);
}

sizeix CControl::GetContentSize() const
{
	rectix rcFull(pointix(), GetPreferedSize());
	switch(m_eLayoutMode)
	{
	case LayoutModeAbsolute:
		for(int_x cnt = 0, size = m_children.size(); cnt < size; ++cnt)
		{
			IControl * pChild = m_children[cnt];
			if(!pChild->IsVisible())
				continue;

			rectix rcChild(pChild->GetPosition(), pChild->GetContentSize());
			rcFull |= rcChild;
		}
		break;
	case LayoutModeHorizontal:
		for(int_x cnt = 0, size = m_children.size(); cnt < size; ++cnt)
		{
			IControl * pChild = m_children[cnt];
			if(!pChild->IsVisible())
				continue;

			rectix rcChild(pChild->GetPosition(), pChild->GetContentSize());
			rcChild.size += pChild->GetMargin().size();
			rcFull.w += rcChild.w;
			rcFull.h = maxof(rcFull.h, rcChild.h);
		}
		break;
	case LayoutModeVertical:
		for(int_x cnt = 0, size = m_children.size(); cnt < size; ++cnt)
		{
			IControl * pChild = m_children[cnt];
			if(!pChild->IsVisible())
				continue;

			rectix rcChild(pChild->GetPosition(), pChild->GetContentSize());
			rcChild.size += pChild->GetMargin().size();
			rcFull.h += rcChild.h;
			rcFull.w += maxof(rcFull.w, rcChild.w);
		}
		break;
	default:
		break;
	}
	return rcFull.size;
}

int_x CControl::GetX() const
{
	return m_rect.x;
}

int_x CControl::GetY() const
{
	return m_rect.y;
}

int_x CControl::GetWidth() const
{
	return m_rect.w;
}

int_x CControl::GetHeight() const
{
	return m_rect.h;
}

int_x CControl::GetRight() const
{
	return m_rect.right();
}
int_x CControl::GetBottom() const
{
	return m_rect.bottom();
}

edgeix CControl::GetBorder() const
{
	switch(m_eBorderType)
	{
	case BorderType1:
	case BorderType1Inactive:
		return edgeix(1, 1, 1, 1);
	case BorderType2:
	case BorderType2Inactive:
		return edgeix(2, 2, 2, 2);
	case BorderType3:
	case BorderType3Inactive:
		return edgeix(3, 3, 3, 3);
	case BorderType4:
	case BorderType4Inactive:
		return edgeix(4, 4, 4, 4);
	case BorderType5:
	case BorderType5Inactive:
		return edgeix(5, 5, 5, 5);
	default:
		return edgeix();
	}
}

rectix CControl::GetVisual() const
{
	edgeix egBorder = GetBorder();
	rectix rcVisual(pointix(), m_rect.size);
	rcVisual.x += egBorder.left;
	rcVisual.y += egBorder.top;
	rcVisual.w -= egBorder.width();
	rcVisual.h -= egBorder.height();
	return rcVisual;
}

rectix CControl::GetClient() const
{
	rectix rcClient = GetVisual();
	if(m_pScrollX && m_pScrollX->IsVisible())
		rcClient.h -= m_pScrollX->GetHeight();
	if(m_pScrollY && m_pScrollY->IsVisible())
		rcClient.w -= m_pScrollY->GetWidth();
	if(rcClient.w < 0)
		rcClient.w = 0;
	if(rcClient.h < 0)
		rcClient.h = 0;
	return rcClient;
}

pointix CControl::GetOffset() const
{
	return pointix();
}

void CControl::SetText(const char_16 * szText, int_x iLength)
{
	if(!szText)
		szText = L"";

	if(iLength < 0)
		iLength = textlen(szText);

	if(!textequalex(m_text.buffer(), m_text.length(), szText, iLength, true))
	{
		m_text.set(szText, iLength);
		OnTextChanged();
	}
}

void CControl::SetText(textw text)
{
	if(m_text != text)
	{
		m_text = text;
		OnTextChanged();
	}
}

const textw & CControl::GetText() const
{
	return m_text;
}

void CControl::SetName(const textw & name)
{
	m_name = name;
}

const textw & CControl::GetName() const
{
	return m_name;
}

void CControl::SetId(int_x iID)
{
	m_iId = iID;
}

int_x CControl::GetId() const
{
	return m_iId;
}

void CControl::SetZOrder(int_x iZOrder)
{
	m_iZOrder = iZOrder;
}

int_x CControl::GetZorder() const
{
	return m_iZOrder;
}

bool CControl::IsDynamic() const
{
	return m_bDynamic;
}

void CControl::SetDynamic(bool bDynamic)
{
	m_bDynamic = bDynamic;
}

void CControl::SetEnable(bool bEnable)
{
	if(!bEnable != m_bDisabled)
	{
		if(!bEnable)
		{
			if(m_bCaptured)
				Capture(false);
			if(m_bFocused)
				Focus(false);
			if(m_bSelected)
				Select(false);
			if(m_bMouseIn && m_pParent)
				m_pParent->NcUpdateMouse(GetMousePosition());
		}
		m_bDisabled = !bEnable;
		OnEnableChanged();
	}
}

bool CControl::IsEnabled() const
{
	return !m_bDisabled;
}

bool CControl::IsAviliable() const
{
	if(m_bDisabled)
		return false;
	else if(m_pParent)
		return m_pParent->IsAviliable();
	else
		return true;
}

void CControl::SetVisible(bool bVisible)
{
	if(bVisible == m_bInvisible)
	{
		m_bInvisible = !bVisible;
		NcRepaint();
		if(m_bInvisible && m_bShown)
			SetShown(false);
		else if(!m_bInvisible && !m_bShown && m_pParent && m_pParent->IsShown())
			SetShown(true);
		else {}
		OnVisibleChanged();
	}
}

bool CControl::IsVisible() const
{
	return !m_bInvisible;
}

void CControl::OnLoaded()
{
	//log1(L"CControl::OnLoaded %s\n", m_name.buffer());
}

void CControl::OnVisibleChanged()
{
	VisibleChanged(this, !m_bInvisible);
}

void CControl::OnEnableChanged()
{
	if(m_bDisabled)
	{
		SetMouseControl(nullptr, pointix(I32_MAX, I32_MAX));
		SetSelectedControl(nullptr);
		Focus(false);
		Select(false);
	}

	EnableChanged(this, !m_bDisabled);
	if(m_bRepaintEnabledChanged)
		NcRepaint();
}

void CControl::OnSelectedChanged()
{
	SelectedChanged(this, m_bSelected);
	if(m_bRepaintSelectedChanged)
		NcRepaint();
}

void CControl::OnFocusedChanged()
{
	FocusedChanged(this, m_bFocused);
	if(m_bRepaintFocusedChanged)
		NcRepaint();
}

void CControl::OnActiveChanged()
{
	for(IControl * pControl : m_children)
		pControl->OnActiveChanged();

	if(m_bRepaintActiveChanged)
		NcRepaint();
}

void CControl::OnCapturedChanged()
{
	CapturedChanged(this, m_bCaptured);
	if(m_bRepaintCaptureChanged)
		NcRepaint();
}

void CControl::OnTextChanged()
{
	TextChanged(this);
	if(m_bRepaintTextChanged)
		NcRepaint();
}

void CControl::OnFontChanged()
{
	FontChanged(this);
	if(m_bRepaintFontChanged)
		NcRepaint();
}

void CControl::OnForeColorChanged()
{
	NcRepaint();
}

void CControl::OnBackColorChanged()
{
	NcRepaint();
}

void CControl::OnPositionChanged()
{
	if(m_bShown)
		PositionChanged(this, m_rect.position);
}

void CControl::OnSizeChanged()
{
	OnVisualSizeChanged();
	NcRepaint();

	if(m_bShown)
		SizeChanged(this, m_rect.size);
}

void CControl::OnVisualSizeChanged()
{
	bool bScrollX = m_pScrollX && m_pScrollX->IsVisible() && m_pScrollX->IsEnabled();
	bool bScrollY = m_pScrollY && m_pScrollY->IsVisible() && m_pScrollY->IsEnabled();
	_LayoutScrollBars(bScrollX, bScrollY);
	Layout();
}

void CControl::OnCursorChanged()
{

}

bool CControl::IsShown() const
{
	return m_bShown;
}

bool CControl::IsHidden() const
{
	return !m_bShown;
}

void CControl::SetShown(bool bShown)
{
	if(bShown != m_bShown)
	{
		if(bShown)
		{
			m_bShown = true;
			OnShow();
			if(!m_bDisAutoShowChild)
			{
				for_each(m_children,
					[&](IControl * pControl)
				{
					if(!(pControl->ToIForm()) && pControl->IsVisible())
						pControl->SetShown(true);
				});
			}
		}
		else
		{
			if(!m_bDisAutoShowChild)
			{
				for_each(m_children,
					[&](IControl * pControl)
				{
					if(pControl->IsShown())
						pControl->SetShown(false);
				});
			}
			m_bShown = false;
			OnHide();
		}
	}
}

void CControl::OnShow()
{
	if(m_bBuffered)
		ConfirmBuffer(m_rect.w, m_rect.h);

	NcRepaint();
}

void CControl::OnHide()
{
	SetDropControl(nullptr);
	SetCaptureControl(nullptr);
	SetSelectedControl(nullptr);
	SetMouseControl(nullptr, pointix(IX_MAX, IX_MAX));

	if(m_pParent)
	{
		if(m_pParent->GetCaptureControl() == this)
			m_pParent->SetCaptureControl(nullptr);

		if(m_pParent->GetDropControl() == this)
			m_pParent->SetDropControl(nullptr);

		if(m_pParent->GetSelectedControl() == this)
			m_pParent->SetSelectedControl(nullptr);

		if(m_pParent->GetMouseControl() == this)
			m_pParent->SetMouseControl(nullptr, pointix(IX_MAX, IX_MAX));
	}

	pointix point = GetMousePosition();
	if(m_bMouseIn)
		PreOnMouseOut(point);
	if(m_bMouseNcIn)
		PreOnNcMouseOut(point);
	m_uiMouseState = 0;
	m_pMouseControl = nullptr;
	m_pSelectedControl = nullptr;
	m_pCaptureControl = nullptr;
}

void CControl::SetNcControl(bool bNcControl)
{
	m_bNcControl = bNcControl;
}

bool CControl::IsNcControl() const
{
	return m_bNcControl;
}

void CControl::OnAdded(IControl * pContainer)
{
	m_pParent = pContainer;
}

void CControl::OnRemoved(IControl * pContainer)
{
	verify(m_pParent && (m_pParent == pContainer));
	m_pParent = nullptr;
}

void CControl::Capture(bool bCapture)
{
	if(m_bTransparent)
		return;

	if(m_bCaptured != bCapture)
	{
		if(m_pParent && IsAviliable())
		{
			if(!bCapture)
				SetCaptured(false);
			m_pParent->SetCaptureControl(bCapture ? this : nullptr);
			if(bCapture)
				SetCaptured(true);
		}
	}
}

void CControl::SetCaptured(bool bCaptured)
{
	if(bCaptured != m_bCaptured)
	{
		m_bCaptured = bCaptured;
		OnCapturedChanged();
	}
}

bool CControl::IsCaptured() const
{
	return m_bCaptured;
}

void CControl::SetImeMode(ImeModeE eImeMode)
{
	if(eImeMode != m_eImeMode)
	{
		m_eImeMode = eImeMode;
		UpdateIme();
	}
}

ImeModeE CControl::GetImeMode() const
{
	return m_eImeMode;
}

void CControl::UpdateIme()
{
	if(m_pParent)
		m_pParent->UpdateIme();
}

bool CControl::GetImeInfo(ImeInfoT & imeInfo) const
{
	imeInfo.eImeMode = ImeModeDisable;
	return false;
}

bool CControl::ClipboardCheckData(ClipboardDataTypeE eDataType) const
{
	if(m_pParent)
		return m_pParent->ClipboardCheckData(eDataType);
	else
		return false;
}

textw CControl::ClipboardGetText16() const
{
	if(m_pParent)
		return m_pParent->ClipboardGetText16();
	else
		return textw();
}

void CControl::ClipboardSetText16(const char_16 * szText, int_x iLength) const
{
	if(m_pParent)
		m_pParent->ClipboardSetText16(szText, iLength);
	else {}
}

void CControl::SetAcceptDrop(bool bAcceptDrop)
{
	m_bAcceptDrop = bAcceptDrop;
}

bool CControl::IsAcceptDrop() const
{
	return m_bAcceptDrop;
}

bool CControl::CanAcceptDrop() const
{
	return m_bAcceptDrop && IsAviliable();
}

void CControl::SetFont(const font_t & font)
{
	if(font != m_font)
	{
		m_font = font;
		OnFontChanged();
	}
}

void CControl::SetFontName(const char_16 * szFontName)
{
	if(m_font.name != szFontName)
	{
		m_font.name = szFontName;
		OnFontChanged();
	}
}
void CControl::SetFontSize(int_x iFontSize)
{
	if(iFontSize != m_font.size)
	{
		m_font.size = iFontSize;
		OnFontChanged();
	}
}

void CControl::SetFontRenderLevel(FontRenderlevelE eRenderLevel)
{
	if(eRenderLevel != m_font.renderlevel)
	{
		m_font.renderlevel = eRenderLevel;
		OnFontChanged();
	}
}

void CControl::SetFontFlag(FontFlagE eFontFlag, bool bSet)
{
	switch(eFontFlag)
	{
	case FontFlagBold:
		if(bSet != (m_font.weight == FONT_WEIGHT_BOLD))
		{
			m_font.weight = bSet ? FONT_WEIGHT_BOLD : FONT_WEIGHT_NORMAL;
			OnFontChanged();
		}
		break;
	case FontFlagItalic:
		if(bSet == !m_font.italic)
		{
			m_font.italic = bSet;
			OnFontChanged();
		}
		break;
	case FontFlagUnderLine:
		if(bSet == !m_font.underline)
		{
			m_font.underline = bSet;
			OnFontChanged();
		}
		break;
	case FontFlagStrikeOut:
		if(bSet == !m_font.strikeout)
		{
			m_font.strikeout = bSet;
			OnFontChanged();
		}
		break;
	default:
		throw exp_not_supported();
	}
}

const font_t & CControl::GetFont() const
{
	return m_font;
}

int_x CControl::GetFontSize() const
{
	return m_font.size;
}

void CControl::SetBackColor(uint_32 uiBackColor)
{
	if(uiBackColor != m_uiBackColor)
	{
		m_uiBackColor = uiBackColor;
		OnBackColorChanged();
	}
}

void CControl::SetForeColor(uint_32 uiForeColor)
{
	if(uiForeColor != m_uiForeColor)
	{
		m_uiForeColor = uiForeColor;
		OnForeColorChanged();
	}
}

uint_32 CControl::GetBackColor() const
{
	return m_uiBackColor;
}

uint_32 CControl::GetForeColor() const
{
	return m_uiForeColor;
}

void CControl::SetCursor(CursorE eCursor)
{
	if(eCursor != m_eCursor)
	{
		m_eCursor = eCursor;
		OnCursorChanged();
	}
}

CursorE CControl::GetCursor() const
{
	return m_eCursor;
}

void CControl::NcRequestCursor(pointix point, CursorE & eCursor) const
{
	pointix ptClient = ToClient(point);
	if(m_pMouseControl)
	{
		m_pMouseControl->NcRequestCursor(m_pMouseControl->ToLocal(ptClient), eCursor);
	}
	else
	{
		if(m_bPopuping && !m_bPupupResponse)
			eCursor = CursorNormal;
		else
		{
			if(NcHitTest(point) == HitTestClient)
			{
				RequestCursor(ptClient, eCursor);
			}
			else
				eCursor = CursorNormal;
		}
	}
}

void CControl::RequestCursor(pointix point, CursorE & eCursor) const
{
	if(IsEnabled())
		eCursor = m_eCursor;
	else
		eCursor = CursorNormal;
}

void CControl::SetFocusAble(bool bFocusAble)
{
	if(bFocusAble != m_bFocusAble)
	{
		m_bFocusAble = bFocusAble;
	}
}
bool CControl::IsFocusAble() const
{
	return m_bFocusAble;
}

void CControl::SetSelectAble(bool bSelectAble)
{
	if(bSelectAble == m_bDisSelectAble)
	{
		if(!bSelectAble)
			Select(false);
		m_bDisSelectAble = !bSelectAble;
	}
}
bool CControl::IsSelectAble() const
{
	return !m_bDisSelectAble;
}

bool CControl::CanSelect() const
{
	return !m_bDisSelectAble && IsAviliable();
}

bool CControl::CanFocus() const
{
	if(m_bShown && IsAviliable())
	{
		if(m_pSelectedControl && m_pSelectedControl->CanFocus())
			return true;
		else
			return m_bFocusAble;
	}
	else
		return false;
}


bool CControl::Focus(bool bFocus)
{
	if(m_pSelectedControl)
		return m_pSelectedControl->Focus(bFocus);
	else if(IsFocusAble())
	{
		SetFocused(bFocus);
		return true;
	}
	else
		return false;
}

void CControl::SetFocused(bool bFocused)
{
	if(bFocused && !m_bShown)
	{
		log0(L"Try SetFocused by true, but m_bShown is false now!");
		return;
	}

	if(m_bFocusAble && !m_bDisSelectAble)
	{
		if(bFocused != m_bFocused)
		{
			m_bFocused = bFocused;
			OnFocusedChanged();
			if(m_bFocused && m_pParent)
				m_pParent->OnChildGetFocus(this);
		}
	}
}

bool CControl::IsFocused() const
{
	return m_bFocused;
}

void CControl::Select(bool bSelect)
{
	if(m_pParent && !m_bDisabled && !m_bDisSelectAble)
	{
		if(bSelect != m_bSelected)
		{
			if(bSelect)
				m_pParent->SetSelectedControl(this);
			else if(m_pParent->GetSelectedControl() == this)
				m_pParent->SetSelectedControl(nullptr);
			else
				verify(false);
		}
	}
}

void CControl::SetSelected(bool bSelected)
{
	if(bSelected != m_bSelected)
	{
		if(!bSelected && m_bFocused)
			SetFocused(false);
		m_bSelected = bSelected;
		OnSelectedChanged();
		if(bSelected && m_bFocusAble && m_bShown)
			SetFocused(true);
	}
}

bool CControl::IsSelected() const
{
	return m_bSelected;
}

bool CControl::SelectLast(bool bWrap)
{
	if(!m_pSelectedControl)
	{
		for(int_x cnt = m_children.size() - 1; cnt >= 0; --cnt)
		{
			IControl * pControl = m_children[cnt];
			if(pControl->CanFocus())
			{
				pControl->Select(true);
				return true;
			}
			else if(pControl->SelectLast(false))
				return true;
			else {}
		}
	}
	else
	{
		if(m_pSelectedControl &&
			m_pSelectedControl->GetSelectedControl() &&
			m_pSelectedControl->SelectLast(false))
			return true;

		int_x iSelected = 0;
		for(int_x cnt = 0; cnt < m_children.size(); ++cnt)
		{
			if(m_pSelectedControl == m_children[cnt])
			{
				iSelected = cnt;
				break;
			}
		}

		if(bWrap)
		{
			for(int_x cnt = 0, size = m_children.size(); cnt < size - 1; ++cnt)
			{
				IControl * pControl = m_children[(iSelected - cnt - 1 + size) % size];
				if(pControl->CanFocus())
				{
					pControl->Select(true);
					return true;
				}
				else if(pControl->SelectLast(false))
					return true;
				else {}
			}
		}
		else if(iSelected >= 0)
		{
			for(int_x cnt = iSelected - 1; cnt >= 0; --cnt)
			{
				IControl * pControl = m_children[cnt];

				if(pControl->CanFocus())
				{
					pControl->Select(true);
					return true;
				}
				else if(pControl->SelectLast(false))
					return true;
				else {}
			}

		}
	}
	return false;
}

bool CControl::SelectNext(bool bWrap)
{
	if(!m_pSelectedControl)
	{
		for(int_x cnt = 0; cnt < m_children.size(); ++cnt)
		{
			IControl * pControl = m_children[cnt];
			if(pControl->CanFocus())
			{
				pControl->Select(true);
				return true;
			}
			else if(pControl->SelectNext(false))
				return true;
			else {}
		}
	}
	else
	{
		if(m_pSelectedControl &&
			m_pSelectedControl->GetSelectedControl() &&
			m_pSelectedControl->SelectNext(false))
			return true;

		int_x iSelected = -1;
		for(int_x cnt = 0; cnt < m_children.size(); ++cnt)
		{
			IControl * pControl = m_children[cnt];
			if(m_pSelectedControl == pControl)
			{
				iSelected = cnt;
				break;
			}
		}

		if(bWrap)
		{
			for(int_x cnt = 0, size = m_children.size(); cnt < size - 1; ++cnt)
			{
				IControl * pControl = m_children[(iSelected + cnt + 1) % size];

				if(pControl->CanFocus())
				{
					pControl->Select(true);
					return true;
				}
				else if(pControl->SelectNext(false))
					return true;
				else {}
			}
		}
		else if(iSelected >= 0)
		{
			for(int_x cnt = iSelected + 1, size = m_children.size(); cnt < size; ++cnt)
			{
				IControl * pControl = m_children[cnt];

				if(pControl->CanFocus())
				{
					pControl->Select(true);
					return true;
				}
				else if(pControl->SelectNext(false))
					return true;
				else {}
			}
		}
		else {}
	}
	return false;
}

void CControl::SetStatic(bool bStatic)
{
	m_bStatic = bStatic;
}

bool CControl::IsStatic() const
{
	return m_bStatic;
}

void CControl::SetTransparent(bool bTransparent)
{
	m_bTransparent = bTransparent;
}

bool CControl::IsTransparent() const
{
	return m_bTransparent;
}

void CControl::SetClipChild(bool bClipChild)
{
	m_bClipChild = bClipChild;
}

bool CControl::IsClipChild() const
{
	return m_bClipChild;
}

void CControl::SetAcceptEnter(bool bAcceptEnter)
{
	m_bAcceptEnter = bAcceptEnter;
}

void CControl::SetAcceptTab(bool bAcceptTab)
{
	m_bAcceptTab = bAcceptTab;
}

bool CControl::IsAcceptEnter() const
{
	return m_bAcceptEnter;
}

bool CControl::CanAcceptEnter() const
{
	if(!m_bShown)
		return false;
	else if(m_pSelectedControl && m_pSelectedControl->CanAcceptEnter())
		return true;
	else
		return m_bAcceptTab && !m_bDisabled;
}

bool CControl::IsAcceptTab() const
{
	return m_bAcceptTab;
}

bool CControl::CanAcceptTab() const
{
	if(!m_bShown)
		return false;
	else if(m_pSelectedControl && m_pSelectedControl->CanAcceptTab())
		return true;
	else
		return m_bAcceptTab && !m_bDisabled;
}

void CControl::SetAcceptScroll(bool bAcceptScroll)
{
	m_bNotAcceptScroll = !bAcceptScroll;
}

bool CControl::IsAcceptScroll() const
{
	return !m_bNotAcceptScroll;
}

bool CControl::CanAcceptScroll() const
{
	if(!m_bShown)
		return false;
	else
	{
		switch(m_eMouseWheelMode)
		{
		case MouseWheelModeDisabled:
			return false;
		case MouseWheelModeNoDispatch:
			return !m_bDisabled && !m_bNotAcceptScroll;
		case MouseWheelModeHoving:
			if(m_pMouseControl && m_pMouseControl->CanAcceptScroll())
				return true;
			else
				return !m_bDisabled && !m_bNotAcceptScroll;
		case MouseWheelModeFocus:
			if(m_pSelectedControl && m_pSelectedControl->CanAcceptScroll())
				return true;
			else
				return !m_bDisabled && !m_bNotAcceptScroll;
		default:
			return !m_bDisabled && !m_bNotAcceptScroll;
		}
	}
}

err_t CControl::SetParam(const char_16 * szName, const char_16 * szValue)
{
	return err_no_impl;
}

void CControl::SetAnchor(anchor_t anchor)
{
	if(anchor != m_anchor)
	{
		m_anchor = anchor;
		if((anchor.type & AlignLR) && (m_eWidthMode == WHModeFill || m_eWidthMode == WHModeAuto))
		{
			log0(L"CControl::SetAnchor with AlignLR, but m_eWidthMode is WHModeFill or WHModeAuto.");
			SetWidthMode(WHModeAbs);
		}
		if((anchor.type & AlignTB) && (m_eHeightMode == WHModeFill || m_eHeightMode == WHModeAuto))
		{
			log0(L"CControl::SetAnchor with AlignTB, but m_eHeightMode is WHModeFill or WHModeAuto.");
			SetHeightMode(WHModeAbs);
		}

		InitLayout();
	}
}

anchor_t CControl::GetAnchor() const
{
	return m_anchor;
}

void CControl::Anchor()
{
	if(m_pParent && m_anchor.type)
	{
		pointix position = m_rect.position;
		sizeix size = m_rect.size;
		if(m_eWidthMode == WHModeAbs)
		{
			if(m_anchor.type & AlignRight)
			{
				if(m_anchor.type & AlignLeft)
					size.w = m_pParent->GetWidth() - (m_anchor.edge.left + m_anchor.edge.right);
				else
					position.x = m_pParent->GetWidth() - m_anchor.edge.right - size.w;
			}
			else {}
		}
		else {}

		if(m_eHeightMode == WHModeAbs)
		{
			if(m_anchor.type & AlignBottom)
			{
				if(m_anchor.type & AlignTop)
					size.h = m_pParent->GetHeight() - (m_anchor.edge.top + m_anchor.edge.bottom);
				else
					position.y = m_pParent->GetHeight() - m_anchor.edge.bottom - size.h;
			}
			else {}
		}
		else {}

		if(position != m_rect.position)
		{
			m_rect.position = position;
			OnPositionChanged();
		}
		if(size != m_rect.size)
		{
			m_rect.size = size;
			OnSizeChanged();
		}
	}
}

void CControl::InitLayout()
{
	if(m_pParent)
	{
		rectix rcClient = m_pParent->GetClient();
		edgeix edge;
		edge.left = m_rect.x;
		edge.top = m_rect.y;
		edge.right = rcClient.w - m_rect.right();
		edge.bottom = rcClient.h - m_rect.bottom();
		m_anchor.edge = edge;
	}
}

pointix CControl::ToScreen(const pointix & point) const
{
	if(m_pParent)
		return m_pParent->ToScreen(ToGlobal(point));
	else
		return point;
}

pointix CControl::ToLocal(const pointix & point) const
{
	return point - m_rect.position;
}

pointix CControl::ToGlobal(const pointix & point) const
{
	return point + m_rect.position;
}

pointix CControl::ToClient(const pointix & point) const
{
	rectix rcClient = GetClient();
	return point - rcClient.position;
}

pointix CControl::ToNoneCl(const pointix & point) const
{
	rectix rcClient = GetClient();
	return point + rcClient.position;
}

void CControl::CenterToParent()
{
	if(m_pParent)
	{
		sizeix siParent = m_pParent->GetSize();
		int_x iX = (siParent.w - m_rect.w) / 2;
		int_x iY = (siParent.h - m_rect.h) / 2;
		SetPosition(pointix(iX, iY));
	}
}

void CControl::ShowTips(pointix point, const char_16 * szTips)
{
	if(m_pParent)
		m_pParent->ShowTips(ToGlobal(point), szTips);
}

void CControl::HideTips()
{
	if(m_pParent)
		m_pParent->HideTips();
}

void CControl::UpdateTips(pointix point)
{
	if(m_pParent)
		m_pParent->UpdateTips(ToGlobal(point));
}

int_x CControl::PopupMenu(pointix point, IMenu * pMenu)
{
	if(m_pParent)
		return m_pParent->PopupMenu(ToGlobal(point), pMenu);
	else
		return 0;
}

int_x CControl::PopupControl(pointix point, IControl * pControl)
{
	if(m_pParent)
		return m_pParent->PopupControl(ToGlobal(point), pControl);
	else
		return 0;
}

void CControl::SetAutoRepaint(uint_32 uiAutoRepaint)
{
	m_uiAutoRepaint = uiAutoRepaint;
}
uint_32 CControl::GetAutoRepaint() const
{
	return m_uiAutoRepaint;
}

void CControl::Repaint()
{
	if(m_bShown && m_bValidate)
	{
		//m_bValidate = false;
		NcRepaint(GetClient());
	}
}

void CControl::Repaint(const rectix & rect)
{
	if(m_bShown && m_bValidate)
	{
		//m_bValidate = false;
		rectix rcUpdate(rect);
		rcUpdate.position = ToNoneCl(rcUpdate.position);
		NcRepaint(rcUpdate);
	}
}

void CControl::NcRepaint()
{
	NcRepaint(rectix(pointix(), m_rect.size));
}

void CControl::NcRepaint(const rectix & rect)
{
	if(m_bShown && m_pParent)
	{
		rectix rcUpdate = rectix(pointix(), m_rect.size).intersect(rect);
		if(rcUpdate.is_valid())
		{
			rcUpdate.position += m_rect.position;
			rcUpdate.position = m_pParent->ToNoneCl(rcUpdate.position);
			m_pParent->NcRepaint(rcUpdate);
		}
		else {}
	}
}

void CControl::PreOnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme)
{
	if(m_rect.is_empty())
		return;

	/**
	 * 一旦 PreOnPaint 被调用，就说明控件的 rcClip区域是无效的，且 rcClip 区域必定
	 * 是 m_rect 的子集。
	 * 原点已经被设置为控件的 (0,0) 点，裁剪矩形也被设置为 rcClip。
	 * rcClip 使用的是控件的坐标系。
	 */

	rectix rcVisual = GetVisual();
	rectix rcClient = GetClient();
	rectix rcPaint;

	OnNcPaint(pPaint, rcClip, pTheme);
	pointix i2Offset = GetOffset();

	//------非客户区客户区绘图，在 Visual 区域中进行。
	rcPaint = rcVisual.intersect(rcClip);
	if(rcPaint.is_valid())
	{
		pPaint->PushClip(rcPaint);
		pPaint->PushOrign(rcClient.position);
		rcPaint.position -= rcClient.position;

		// -------------------------------- 绘制非客户区子控件。
		for(int_x cnt = 0, size = m_children.size(); cnt < size; ++cnt)
		{
			IControl * pChild = m_children[cnt];
			if(!pChild || !pChild->IsNcControl())
				continue;

			if(pChild->IsHidden() || pChild->ToIForm())
				continue;

			rectix rcChild = pChild->GetRect();
			rectix rcChildPaint = rcPaint.intersect(rcChild);
			if(rcChildPaint.is_valid())
			{
				pPaint->PushClip(rcChildPaint);
				pPaint->PushOrign(rcChild.position);
				rcChildPaint.position -= rcChild.position;

				pChild->PreOnNcPaint(pPaint, rcChildPaint, pTheme);
				pPaint->PopOrign();
				pPaint->PopClip();
			}
		}

		pPaint->PopOrign();
		pPaint->PopClip();
	}

	//------客户区绘图，在 Client 区域中进行。
	rcPaint = rcClient.intersect(rcClip);
	if(rcPaint.is_valid())
	{
		pPaint->PushClip(rcPaint);
		pPaint->PushOrign(rcClient.position);
		rcPaint.position -= rcClient.position;

		// -------------------------------- 绘制自己的客户区。
		PreOnPaint(pPaint, rcPaint, pTheme);

		// -------------------------------- 绘制子控件。
		for(int_x cnt = 0, size = m_children.size(); cnt < size; ++cnt)
		{
			IControl * pChild = m_children[cnt];
			if(!pChild || pChild->IsNcControl())
				continue;

			if(pChild->IsHidden() || pChild->ToIForm())
				continue;

			rectix rcChild = pChild->GetRect();
			rcChild.position += i2Offset;
			rectix rcChildPaint = rcPaint.intersect(rcChild);
			if(rcChildPaint.is_valid())
			{
				pPaint->PushClip(rcChildPaint);
				pPaint->PushOrign(rcChild.position);
				rcChildPaint.position -= rcChild.position;

				pChild->PreOnNcPaint(pPaint, rcChildPaint, pTheme);
				pPaint->PopOrign();
				pPaint->PopClip();
			}
		}

		AftOnPaint(pPaint, rcPaint, pTheme);
		pPaint->PopOrign();
		pPaint->PopClip();
	}

	m_bValidate = true;
}

void CControl::OnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	if(m_uiBackColor != SysColorAuto)
		pPaint->FillRect(rcClip, m_uiBackColor);

	if(m_eBorderType != BorderTypeNone)
	{
		uint_32 color = SysColorAuto;
		if(!(m_eBorderType & BorderTypeActive))
			color = SysColorFrameNormal;
		else if(!IsAviliable())
			color = SysColorFrameDisable;
		else if(IsFocused())
			color = SysColorFrameFocused;
		else if(IsSelected())
			color = SysColorFrameSelected;
		else if(IsMouseNcIn())
			color = SysColorFrameHoving;
		else
			color = SysColorFrameNormal;

		pPaint->DrawRect(0, 0, m_rect.w - 1, m_rect.h - 1, color);

		switch(m_eBorderType)
		{
		case BorderType2:
		case BorderType3:
		case BorderType2Inactive:
		case BorderType3Inactive:
			pPaint->DrawRect(1, 1, m_rect.w - 3, m_rect.h - 3, Colors::White);
			break;
		default:
			break;
		}
	}
	verify(m_bShown);
}

void CControl::PreOnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	verify(m_bShown);

	if(m_bBuffered)
		OnPaint(pPaint, rcClip, pTheme);
	else
		OnPaint(pPaint, rcClip, pTheme);
}

void CControl::AftOnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{

}

void CControl::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	verify(m_bShown);
}

void CControl::SetHotKey(KeyCodeE eHotKey)
{
	m_eHotKey = eHotKey;
}

KeyCodeE CControl::GetHotKey() const
{
	return m_eHotKey;
}

bool CControl::OnHotKey()
{
	return true;
}

void CControl::SetPopuping(bool bPopuping)
{
	if(bPopuping != m_bPopuping)
	{
		m_bPopuping = bPopuping;
		for_each(m_children, [&](IControl * pControl) { pControl->SetPopuping(bPopuping); });
	}
}
bool CControl::IsPopuping() const
{
	return m_bPopuping;
}

void CControl::SetBorderType(BorderTypeE eBorderType)
{
	if(m_eBorderType != eBorderType)
	{
		m_eBorderType = eBorderType;
		OnVisualSizeChanged();
		NcRepaint();
	}
}

BorderTypeE CControl::GetBorderType() const
{
	return m_eBorderType;
}

HitTestE CControl::NcHitTest(pointix point) const
{
	// 被当前控件捕获，就不用做其他的了。
	if(m_bCaptured)
		return HitTest(point);

	if(!rectix(pointix(), m_rect.size).is_point_in(point))
		return HitTestNoWhere;

	HitTestE eResult = HitTest(point);
	if(eResult == HitTestClient ||
		eResult == HitTestStatic ||
		(eResult == HitTestTransparent && m_bClipChild))
	{
		pointix ptClient = ToClient(point);
		//IControl * pControl = FindChild(ptClient);
		IControl * pControl = nullptr;

		HitTestE eHitTestChild = HitTestNoWhere;
		for(int_x cnt = m_children.size() - 1; cnt >= 0; --cnt)
		{
			IControl * pChild = m_children[cnt];
			pointix ptChild = pChild->ToLocal(ptClient);
			if(!pChild->IsShown())
				continue;

			HitTestE eHitTest = pChild->NcHitTest(ptChild);
			if(eHitTest != HitTestNoWhere)
			{
				eHitTestChild = eHitTest;
				if(eHitTest != HitTestTransparent)
					break;
			}
			else {}
		}

		switch(eHitTestChild)
		{
		case HitTestNoWhere:
			break;
		case HitTestInner:
		case HitTestClient:
			eResult = HitTestClient;
			break;
		case HitTestStatic:
			eResult = HitTestStatic;
			break;
		case HitTestTransparent:
			if(m_bClipChild)
				eResult = HitTestTransparent;
			break;
		case HitTestCaption:
			eResult = HitTestCaption;
			break;
		default:
			break;
		}
	}
	else {}

	return eResult;
}

HitTestE CControl::HitTest(pointix point) const
{
#ifdef _DEBUG
	if(!IsKeyDown(KeyCodeF3))
	{
		if(m_bTransparent)
			return HitTestTransparent;
		else{}
	}

	if(IsKeyDown(KeyCodeF4))
	{
		rectix rcVisual = GetVisual();
		if(rcVisual.is_point_in(point))
			return HitTestClient;
		else
			return HitTestInner;
	}
#endif
	if(m_bTransparent)
		return HitTestTransparent;
	else {}

	rectix rcVisual = GetVisual();
	if(rcVisual.is_point_in(point))
		return m_bStatic ? HitTestStatic : HitTestClient;
	else if(m_bStatic)
		return HitTestStatic;
	else
		return HitTestInner;
}

HitTestE CControl::HitTestDeep(pointix point) const
{
	HitTestE eRes = NcHitTest(point);
	if(eRes == HitTestNoWhere)
	{
		return HitTestNoWhere;
	}
	else if(m_pCaptureControl)
		return m_pCaptureControl->HitTestDeep(m_pCaptureControl->ToLocal(ToClient(point)));
	else
	{
		IControl * pChild = NcFindChild(point);
		if(pChild)
		{
			HitTestE eResult = pChild->HitTestDeep(pChild->ToLocal(ToClient(point)));
			if(eResult == HitTestNoWhere)
				return HitTestClient;
			else
				return eResult;
		}
		else
			return eRes;
	}
}

void CControl::NcUpdateMouse(pointix point)
{
	if(m_pCaptureControl || m_bCaptured)
	{
		// 不处理。
	}
	else
	{
		IControl * pControl = nullptr;
		if(point.x != IX_MAX && point.y != IX_MAX)
			pControl = NcFindChild(point);

		pointix ptClient = ToClient(point);
		bool bMouseIn = !pControl && NcHitTest(point) == HitTestClient;

		if(m_bMouseIn && !bMouseIn)
			PreOnMouseOut(ptClient);

		SetMouseControl(pControl, point);

		if(!m_bMouseIn && bMouseIn)
			PreOnMouseIn(ptClient);
	}
}

void CControl::PreOnNcMouseMove(pointix point)
{
	NcUpdateMouse(point);
	OnNcMouseMove(point);

	pointix ptClient = ToClient(point);

	if(m_bCaptured)
		PreOnMouseMove(ptClient);
	else if(m_pCaptureControl)
		m_pCaptureControl->PreOnNcMouseMove(m_pCaptureControl->ToLocal(ptClient));
	else if(m_pMouseControl)
		m_pMouseControl->PreOnNcMouseMove(m_pMouseControl->ToLocal(ptClient));
	else if(m_bMouseIn)
		PreOnMouseMove(ptClient);
	else {}
}

void CControl::OnNcMouseMove(pointix point)
{

}

void CControl::PreOnMouseMove(pointix point)
{
	if(m_bPopuping && !m_bPupupResponse)
		return;
	if(IsAviliable())
		OnMouseMove(point);
}

void CControl::OnMouseMove(pointix point)
{
	MouseMove(this, point);
}

void CControl::PreOnNcMouseIn(pointix point)
{
	verify(!m_bMouseNcIn);
	//Warnf(L"CControl::PreOnNcMouseIn %s\n", GetOid().name);
	m_bMouseNcIn = true;
	OnNcMouseIn(point);
	NcUpdateMouse(point);
}

void CControl::OnNcMouseIn(pointix point)
{
	if(m_eBorderType & BorderTypeActive)
		NcRepaint();
}

void CControl::PreOnMouseIn(pointix point)
{
	verify(!m_bMouseIn);

	if(m_bPopuping && !m_bPupupResponse)
		return;

	if(m_bRepaintMousePreInOut)
		NcRepaint();

	m_bMouseIn = true;
	OnMouseIn(point);
}

void CControl::OnMouseIn(pointix point)
{
	if(m_bRepaintMouseInOut)
		NcRepaint();
	MouseIn(this, point);
}

void CControl::PreOnNcMouseOut(pointix point)
{
	verify(m_bMouseNcIn);
	verify(!m_bCaptured);

	NcUpdateMouse(pointix(IX_MAX, IX_MAX));
	pointix ptClient = ToClient(point);

	if(m_bMouseIn)
		PreOnMouseOut(ptClient);

	m_bMouseNcIn = false;
	m_uiMouseState = 0;
	OnNcMouseOut(point);
}

void CControl::OnNcMouseOut(pointix point)
{
	if(m_eBorderType & BorderTypeActive)
		NcRepaint();
}

void CControl::PreOnMouseOut(pointix point)
{
	if(m_bRepaintMousePreInOut)
		NcRepaint();

	verify(m_bMouseIn);
	m_bMouseIn = false;
	OnMouseOut(point);
}

void CControl::OnMouseOut(pointix point)
{
	if(m_bRepaintMouseInOut)
		NcRepaint();
	MouseOut(this, point);
}

void CControl::PreOnNcMouseDown(pointix point, MouseButtonE eButton)
{
	NcUpdateMouse(point);

	switch(eButton)
	{
	case MouseButtonL:
		m_bMouseNcDownL = true;
		break;
	case MouseButtonM:
		m_bMouseNcDownM = true;
		break;
	case MouseButtonR:
		m_bMouseNcDownR = true;
		break;
	default:
		throw exp_not_supported();
	}

	//if(!IsAviliable())
	//	return;

	OnNcMouseDown(point, eButton);

	pointix ptClient = ToClient(point);
	if(m_pCaptureControl)
		m_pCaptureControl->PreOnNcMouseDown(m_pCaptureControl->ToLocal(ptClient), eButton);
	else if(m_pMouseControl)
	{
		if(m_pMouseControl->CanSelect())
			SetSelectedControl(m_pMouseControl->CanSelect() ? m_pMouseControl : nullptr);
		else
			SetSelectedControl(nullptr);
		if(m_pMouseControl)
			m_pMouseControl->PreOnNcMouseDown(m_pMouseControl->ToLocal(ptClient), eButton);
	}
	else
	{
		SetSelectedControl(nullptr);
#ifdef _DEBUG
		if(eButton == MouseButtonR && IsKeyDown(KeyCodeF4))
		{
			MenuT menu;
			int_x iBase = 0;
			QueryDebugMenu(iBase, &menu);
			int_x iResult = PopupMenu(ToNoneCl(ptClient), &menu);
			OnDebugMenu(iBase, iResult);
			return;
		}
#endif
		if(m_bMouseIn)
		{
			//SetSelectedControl(nullptr);
			//PreOnMouseMove(ptClient);
			PreOnMouseDown(ptClient, eButton);
		}
		else {}
	}
}

void CControl::OnNcMouseDown(pointix point, MouseButtonE eButton)
{

}

void CControl::PreOnMouseDown(pointix point, MouseButtonE eButton)
{
	if(m_bPopuping && !m_bPupupResponse)
		return;

	if(IsAviliable())
		OnMouseDown(point, eButton);
}

void CControl::OnMouseDown(pointix point, MouseButtonE eButton)
{
	switch(eButton)
	{
	case MouseButtonL:
		m_bMouseDownL = true;
		OnMouseDownL(point);
		break;
	case MouseButtonM:
		m_bMouseDownM = true;
		OnMouseDownM(point);
		break;
	case MouseButtonR:
		m_bMouseDownR = true;
		OnMouseDownR(point);
		break;
	default:
		verify(false);
		break;
	}
}

void CControl::OnMouseDownL(pointix point)
{
	if(m_bRepaintMouseDownUpL)
		NcRepaint();
	MouseDownL(this, point);
}

void CControl::OnMouseDownM(pointix point)
{
	if(m_bRepaintMouseDownUpL)
		NcRepaint();
	MouseDownM(this, point);
}

void CControl::OnMouseDownR(pointix point)
{
	if(m_bRepaintMouseDownUpR)
		NcRepaint();
	MouseDownR(this, point);
}

void CControl::PreOnNcMouseUp(pointix point, MouseButtonE eButton)
{
	bool bMouseUp = false;
	switch(eButton)
	{
	case MouseButtonL:
		m_bMouseNcDownL = false;
		bMouseUp = m_bMouseDownL;
		break;
	case MouseButtonM:
		m_bMouseNcDownM = false;
		bMouseUp = m_bMouseDownM;
		break;
	case MouseButtonR:
		m_bMouseNcDownR = false;
		bMouseUp = m_bMouseDownR;
		break;
	default:
		throw exp_not_supported();
	}

	OnNcMouseUp(point, eButton);

	pointix ptClient = ToClient(point);

	if(m_pCaptureControl)
		m_pCaptureControl->PreOnNcMouseUp(m_pCaptureControl->ToLocal(ptClient), eButton);
	else if(m_pMouseControl)
		m_pMouseControl->PreOnNcMouseUp(m_pMouseControl->ToLocal(ptClient), eButton);
	else if(bMouseUp)
		PreOnMouseUp(ptClient, eButton);
	else {}

	NcUpdateMouse(point);
}

void CControl::OnNcMouseUp(pointix point, MouseButtonE eButton)
{

}

void CControl::PreOnMouseUp(pointix point, MouseButtonE eButton)
{
	if(IsAviliable())
		OnMouseUp(point, eButton);
}

void CControl::OnMouseUp(pointix point, MouseButtonE eButton)
{
	switch(eButton)
	{
	case MouseButtonL:
		m_bMouseDownL = false;
		OnMouseUpL(point);
		break;
	case MouseButtonM:
		m_bMouseDownM = false;
		OnMouseUpM(point);
		break;
	case MouseButtonR:
		m_bMouseDownR = false;
		OnMouseUpR(point);
		break;
	default:
		throw exp_not_supported();
	}
}

void CControl::OnMouseUpL(pointix point)
{
	if(m_bRepaintMouseDownUpL)
		NcRepaint();
	MouseUpL(this, point);
}

void CControl::OnMouseUpM(pointix point)
{
	if(m_bRepaintMouseDownUpM)
		NcRepaint();
	MouseUpM(this, point);
}

void CControl::OnMouseUpR(pointix point)
{
	if(m_bRepaintMouseDownUpR)
		NcRepaint();
	MouseUpR(this, point);
}

void CControl::PreOnNcMouseClick(pointix point, MouseButtonE eButton)
{
	OnNcMouseClick(point, eButton);
	pointix ptClient = ToClient(point);

	if(m_pCaptureControl)
		m_pCaptureControl->PreOnNcMouseClick(m_pCaptureControl->ToLocal(ptClient), eButton);
	else if(m_pMouseControl)
		m_pMouseControl->PreOnNcMouseClick(m_pMouseControl->ToLocal(ptClient), eButton);
	else if(m_bMouseIn)
		PreOnMouseClick(ptClient, eButton);
	else {}
}

void CControl::OnNcMouseClick(pointix point, MouseButtonE eButton)
{

}

void CControl::PreOnMouseClick(pointix point, MouseButtonE eButton)
{
	if(IsAviliable())
	{
		switch(eButton)
		{
		case MouseButtonL:
			OnMouseClickL(point);
			break;
		case MouseButtonM:
			OnMouseClickM(point);
			break;
		case MouseButtonR:
			OnMouseClickR(point);
			break;
		default:
			verify(false);
			break;
		}
	}
}

void CControl::OnMouseClickL(pointix point)
{
	MouseClickL(this, point);
}

void CControl::OnMouseClickM(pointix point)
{
	MouseClickM(this, point);
}

void CControl::OnMouseClickR(pointix point)
{
	MouseClickR(this, point);
}

void CControl::PreOnNcMouseDBClick(pointix point, MouseButtonE eButton)
{
	OnNcMouseDBClick(point, eButton);
	pointix ptClient = ToClient(point);

	if(m_pCaptureControl)
		m_pCaptureControl->PreOnNcMouseDBClick(m_pCaptureControl->ToLocal(ptClient), eButton);
	else if(m_pMouseControl)
		m_pMouseControl->PreOnNcMouseDBClick(m_pMouseControl->ToLocal(ptClient), eButton);
	else if(m_bMouseIn)
		PreOnMouseDBClick(ptClient, eButton);
	else {}
}

void CControl::OnNcMouseDBClick(pointix point, MouseButtonE eButton)
{

}

void CControl::PreOnMouseDBClick(pointix point, MouseButtonE eButton)
{
	if(IsAviliable())
		OnMouseDBClick(point, eButton);
}

void CControl::OnMouseDBClick(pointix point, MouseButtonE eButton)
{
	switch(eButton)
	{
	case MouseButtonL:
		OnMouseDBClickL(point);
		break;
	default:
		verify(false);
		break;
	}
}

void CControl::OnMouseDBClickL(pointix point)
{
	MouseDBClickL(this, point);
}

void CControl::PreOnNcMouseWheel(pointix point, int_x iWhell, AlignE eDirection)
{
	OnNcMouseWheel(point, iWhell, eDirection);
	pointix ptClient = ToClient(point);

	switch(m_eMouseWheelMode)
	{
	case MouseWheelModeHoving:
		NcUpdateMouse(point);
		if(m_pMouseControl &&
			m_pMouseControl != m_pScrollX && m_pMouseControl != m_pScrollY &&
			m_pMouseControl->CanAcceptScroll())
			m_pMouseControl->PreOnNcMouseWheel(m_pMouseControl->ToLocal(ptClient), iWhell, eDirection);
		else
			PreOnMouseWheel(ptClient, iWhell, eDirection);
		break;
	case MouseWheelModeFocus:
		if(m_pSelectedControl &&
			(m_pSelectedControl->IsFocused() || m_pSelectedControl->GetSelectedControl()) &&
			m_pSelectedControl->CanAcceptScroll())
			m_pSelectedControl->PreOnNcMouseWheel(m_pSelectedControl->ToLocal(ptClient), iWhell, eDirection);
		else
			PreOnMouseWheel(ptClient, iWhell, eDirection);
		break;
	case MouseWheelModeNoDispatch:
		PreOnMouseWheel(ptClient, iWhell, eDirection);
		break;
	case MouseWheelModeDisabled:
		break;
	default:
		break;
	}
}

void CControl::OnNcMouseWheel(pointix point, int_x iWhell, AlignE eDirection)
{

}

void CControl::PreOnMouseWheel(pointix point, int_x iWhell, AlignE eDirection)
{
	if(IsAviliable())
		OnMouseWheel(point, iWhell, eDirection);
}

void CControl::OnMouseWheel(pointix point, int_x iWhell, AlignE eDirection)
{
	switch(eDirection)
	{
	case AlignLeftRight:
		OnMouseWheelX(point, iWhell);
		break;
	case AlignTopBottom:
		OnMouseWheelY(point, iWhell);
		break;
	default:
		verify(false);
		break;
	}
}

void CControl::OnMouseWheelX(pointix point, int_x iWhell)
{
	MouseWheelX(this, point, iWhell);
}

void CControl::OnMouseWheelY(pointix point, int_x iWhell)
{
	MouseWheelY(this, point, iWhell);

	if(!IsKeyDown(KeyCodeControl))
	{
		if(IsScrollYAble())
		{
			IScroll * pScrollY = m_pScrollY->ToIScroll();
			pScrollY->SetValue(pScrollY->GetValue() - pScrollY->GetLineValue() * iWhell);
		}
	}
}

pointix CControl::GetMousePosition() const
{
	if(m_pParent)
	{
		pointix point = m_pParent->GetMousePosition();
		rectix rcClient = GetClient();
		return point - m_rect.position - rcClient.position;
	}
	else
		return pointix(I32_MIN, I32_MIN);
}

bool CControl::IsMouseDown(MouseButtonE eButton) const
{
	if(!m_bShown)
		return false;
	switch(eButton)
	{
	case MouseButtonL:		return m_bMouseDownL;
	case MouseButtonM:		return m_bMouseDownM;
	case MouseButtonR:		return m_bMouseDownR;
	default:				throw exp_not_supported();
	}
}

bool CControl::IsPreDragIn() const
{
	return m_bMousePreDragIn;
}

bool CControl::IsDragIn() const
{
	return m_bMouseDragIn;
}


void CControl::PreOnKeyDown(KeyCodeE eKeyCode)
{
	if(IsAviliable())
	{
		if(eKeyCode == KeyCodeTab && !CanAcceptTab())
		{
			IControl * pControl = m_pSelectedControl;

			if(IsKeyDown(KeyCodeShift))
				SelectLast(true);
			else
				SelectNext(true);
		}
		else
		{
			bool bHandled = false;
			for(IControl * pControl : m_children)
			{
				if(pControl->GetHotKey() == eKeyCode && pControl->OnHotKey())
				{
					bHandled = true;
					break;
				}
			}

			if(!bHandled)
			{
				if(m_pSelectedControl)
					m_pSelectedControl->PreOnKeyDown(eKeyCode);
				else
					OnKeyDown(eKeyCode);
			}
		}
	}
}

void CControl::OnKeyDown(KeyCodeE eKeyCode)
{
	KeyDown(this, eKeyCode);
}

void CControl::OnKeyRepeat(KeyCodeE eKeyCode)
{
	// 默认不处理KeyRepeat事件。
}

void CControl::PreOnKeyUp(KeyCodeE eKeyCode)
{
	if(IsAviliable())
	{
		if(m_pSelectedControl)
			m_pSelectedControl->PreOnKeyUp(eKeyCode);
		else
			OnKeyUp(eKeyCode);
	}
}

void CControl::OnKeyUp(KeyCodeE eKeyCode)
{
	KeyUp(this, eKeyCode);
}

void CControl::PreOnKeyInput(char_32 chInput)
{
	if(IsAviliable())
	{
		if(m_bFocused)
			OnKeyInput(chInput);
		else if(m_pSelectedControl && m_pSelectedControl->IsSelected())
			m_pSelectedControl->PreOnKeyInput(chInput);
		else {}
	}
}

void CControl::OnKeyInput(char_32 chInput)
{

}

bool CControl::IsKeyDown(KeyCodeE ekeyCode) const
{
	if(m_pParent)
		return m_pParent->IsKeyDown(ekeyCode);
	else
		return false;
}

void CControl::ConfirmBuffer(int_x iWidth, int_x iHeight)
{
	if(iWidth > 0 && iHeight > 0)
	{
		//if(!m_pBuffer)
		//	m_pBuffer = GetRender()->CreateBuffer(iWidth, iHeight);
		//else
		//	m_pBuffer->SetSize(iWidth, iHeight);
	}
}

void CControl::AddControl(IControl * pControl)
{
	verify(pControl);
	if(pControl->GetParent())
		throw exp_bad_state(); // - 控件已经在某一个容器中了。

	pControl->OnAdded(this);

	if(m_bShown && !m_bDisAutoShowChild && pControl->IsVisible())
		pControl->SetShown(true);

	int_x iIndex = -1;
	for(int_x cnt = 0, size = m_children.size(); cnt < size; ++cnt)
	{
		const IControl * pChild = m_children[cnt];
		if(pChild->GetZorder() > pControl->GetZorder())
		{
			iIndex = cnt;
			break;
		}
	}
	if(iIndex >= 0)
		m_children.insert_at(iIndex, pControl);
	else
		m_children.add(pControl);
	if(pControl->IsDynamic())
		m_DynamicChildren.add(pControl);

	anchor_t anchor = pControl->GetAnchor();
	// anchor edge
	rectix rcClient = GetClient();
	rectix rcChild = pControl->GetRect();
	anchor.edge.left = rcChild.x;
	anchor.edge.top = rcChild.y;
	anchor.edge.right = rcClient.w - rcChild.right();
	anchor.edge.bottom = rcClient.h - rcChild.bottom();

	pControl->SetAnchor(anchor);

	Layout();
}

void CControl::RmvControl(IControl * pControl)
{
	verify(pControl);
	if(pControl->GetParent() != this)
		throw exp_illegal_operation();

	if(pControl == m_pCaptureControl)
		SetCaptureControl(nullptr);
	if(pControl == m_pSelectedControl)
		SetSelectedControl(nullptr);
	if(pControl == m_pDropControl)
		SetDropControl(nullptr);
	if(pControl == m_pMouseControl)
		SetMouseControl(nullptr, pointix(I32_MAX, I32_MAX));
	if(pControl == m_pScrollX)
		SetScrollBarX(nullptr);
	if(pControl == m_pScrollY)
		SetScrollBarY(nullptr);

	if(m_bShown && !m_bDisAutoShowChild&& pControl->IsShown())
		pControl->SetShown(false);

	m_children.remove(pControl);
	pControl->OnRemoved(this);

	if(pControl->IsDynamic())
	{
		m_DynamicChildren.remove(pControl);
		SafeRelease(pControl);
	}

	if(m_eLayoutMode != LayoutModeAbsolute)
		Layout();
}

void CControl::RmvAllControl()
{
	while(m_children.valid())
	{
		IControl * pControl = m_children[0];
		RmvControl(pControl);
	}
}

bool CControl::HasControl(IControl * pControl) const
{
	return m_children.find(pControl) >= 0;
}

void CControl::SetLayoutMode(LayoutModeE eType)
{
	if(m_eLayoutMode != eType)
	{
		m_eLayoutMode = eType;
		Layout();
	}
}

LayoutModeE CControl::GetLayoutMode() const
{
	return m_eLayoutMode;
}

void CControl::BegLayout()
{
	++m_iLayouting;
}

void CControl::EndLayout()
{
	if(m_iLayouting > 0)
	{
		--m_iLayouting;
		if(m_iLayouting == 0)
			Layout();
	}
}

void CControl::Layout()
{
	if(m_iLayouting > 0)
	{
		++m_iLayouting;
		return;
	}

	switch(m_eLayoutMode)
	{
	case LayoutModeAbsolute:
		_LayoutAbsolute();
		break;
	case LayoutModeHorizontal:
		_LayoutHorizontal();
		break;
	case LayoutModeVertical:
		_LayoutVertical();
		break;
	case LayoutModeTable:
		_LayoutTable();
		break;
	default:
		break;
	}
}

void CControl::_LayoutAbsolute()
{
	rectix rcClient = GetClient();
	for(IControl * pChild : m_children)
	{
		if(!pChild->IsVisible() || pChild->IsNcControl())
			continue;

		edgeix margin = pChild->GetMargin();
		rectix rcChild = pChild->GetRect();
		switch(pChild->GetWidthMode())
		{
		case WHModeAbs:
			switch(pChild->GetAnchor().type & AlignLeftRight)
			{
			case AlignLeftRight:
				rcChild.w = rcClient.w - pChild->GetAnchor().edge.width();
				break;
			case AlignRight:
				rcChild.x = rcClient.w - pChild->GetAnchor().edge.right - pChild->GetWidth();
				break;
			default:
				break;
			}
			break;
		case WHModeFill:
			rcChild.x = margin.left;
			rcChild.w = rcClient.w - margin.width();
			break;
		case WHModeAuto:
			rcChild.w = pChild->GetContentSize().w;
			break;
		default:
			break;
		}

		switch(pChild->GetHeightMode())
		{
		case WHModeAbs:
			switch(pChild->GetAnchor().type & AlignTopBottom)
			{
			case AlignTopBottom:
				rcChild.h = rcClient.h - pChild->GetAnchor().edge.height();
				break;
			case AlignBottom:
				rcChild.y = rcClient.h - pChild->GetAnchor().edge.bottom - pChild->GetHeight();
				break;
			default:
				break;
			}
			break;
		case WHModeFill:
			rcChild.y = margin.top;
			rcChild.h = rcClient.h - margin.height();
			break;
		case WHModeAuto:
			rcChild.h = pChild->GetContentSize().h;
			break;
		default:
			break;
		}

		pChild->SetRect(rcChild);
	}
}

void CControl::_LayoutHorizontal()
{
	if(m_children.empty())
		return;

	rectix rcVisual = GetVisual();
	int_x iWidthFixed = 0;
	float1 fWeight = 0.0f;
	for(IControl * pChild : m_children)
	{
		if(!pChild->IsVisible() || pChild->IsNcControl())
			continue;

		edgeix margin = pChild->GetMargin();
		rectix rect = pChild->GetRect();

		iWidthFixed += margin.width();
		float_32 weight = pChild->GetWeight();
		if(weight > F32_EPSILON)
			fWeight += weight;
		else
		{
			if(pChild->GetWidthMode() == WHModeAuto)
				rect.w = pChild->GetContentSize().w;
			iWidthFixed += rect.w;
		}

		switch(pChild->GetHeightMode())
		{
		case WHModeAbs:
			switch(pChild->GetAnchor().type & AlignTopBottom)
			{
			case AlignTopBottom:
				rect.h = rcVisual.h - pChild->GetAnchor().edge.height();
				break;
			case AlignBottom:
				rect.y = rcVisual.h - pChild->GetAnchor().edge.bottom - pChild->GetHeight();
				break;
			default:
				break;
			}
			break;
		case WHModeFill:
			rect.y = margin.top;
			rect.h = rcVisual.h - margin.height();
			break;
		case WHModeAuto:
			rect.h = pChild->GetContentSize().h;
			break;
		default:
			break;
		}

		pChild->SetRect(rect);
	}

	int_x iWidth = rcVisual.w - iWidthFixed;
	if(iWidth < 0)
		iWidth = 0;

	int_x iX = 0, iY = 0;
	for(int_x cnt = 0; cnt < m_children.size(); ++cnt)
	{
		IControl * pChild = m_children[cnt];
		if(!pChild->IsVisible() || pChild->IsNcControl())
			continue;

		edgeix margin = pChild->GetMargin();
		rectix rect = pChild->GetRect();

		iX += margin.left;
		rect.x = iX;
		rect.y = (rcVisual.h - pChild->GetHeight()) / 2;
		float_32 weight = pChild->GetWeight();
		if(weight > F32_EPSILON)
		{
			rect.w = (int_x)(iWidth * weight / fWeight);
			iX += rect.w;
		}
		else
		{
			iX += pChild->GetWidth();
		}
		iX += margin.right;

		pChild->SetRect(rect);
	}
}

void CControl::_LayoutVertical()
{
	if(m_children.empty())
		return;

	rectix rcVisual = GetVisual();
	int_x iHeightFixed = 0;
	float1 fWeight = 0.0f;
	for(IControl * pChild : m_children)
	{
		if(!pChild->IsVisible() || pChild->IsNcControl())
			continue;

		edgeix margin = pChild->GetMargin();
		rectix rect = pChild->GetRect();

		iHeightFixed += margin.height();
		float_32 weight = pChild->GetWeight();
		if(weight > F32_EPSILON)
			fWeight += weight;
		else
		{
			if(pChild->GetHeightMode() == WHModeAuto)
				rect.h = pChild->GetContentSize().h;
			iHeightFixed += rect.h;
		}

		switch(pChild->GetWidthMode())
		{
		case WHModeAbs:
			switch(pChild->GetAnchor().type & AlignLeftRight)
			{
			case AlignLeftRight:
				rect.w = rcVisual.w - pChild->GetAnchor().edge.width();
				break;
			case AlignRight:
				rect.x = rcVisual.w - pChild->GetAnchor().edge.right - pChild->GetWidth();
				break;
			default:
				break;
			}
			break;
		case WHModeFill:
			rect.x = margin.left;
			rect.w = rcVisual.w - margin.width();
			break;
		case WHModeAuto:
			rect.w = pChild->GetContentSize().w;
			break;
		default:
			break;
		}

		pChild->SetRect(rect);
	}

	int_x iHeight = rcVisual.h - iHeightFixed;
	if(iHeight < 0)
		iHeight = 0;

	int_x iY = 0;
	for(int_x cnt = 0; cnt < m_children.size(); ++cnt)
	{
		IControl * pChild = m_children[cnt];
		if(!pChild->IsVisible() || pChild->IsNcControl())
			continue;

		edgeix margin = pChild->GetMargin();
		rectix rect = pChild->GetRect();

		iY += margin.top;
		rect.x = margin.left;
		rect.y = iY;

		float_32 weight = pChild->GetWeight();
		if(weight > F32_EPSILON)
		{
			rect.h = (int_x)(iHeight * weight / fWeight);
			iY += rect.h;
		}
		else
		{
			iY += pChild->GetHeight();
		}
		iY += margin.bottom;

		pChild->SetRect(rect);
	}
}

void CControl::_LayoutTable()
{

}

IControl * CControl::NcFindChild(pointix point) const
{
	return FindChild(ToClient(point));
}

IControl * CControl::FindChild(pointix point) const
{
	for(int_x cnt = m_children.size() - 1; cnt >= 0; --cnt)
	{
		IControl * pControl = m_children[cnt];
		pointix ptChild = pControl->ToLocal(point);
		if(!pControl->IsShown())
			continue;

		HitTestE eHitTest = pControl->NcHitTest(ptChild);
		if(eHitTest != HitTestNoWhere && eHitTest != HitTestTransparent &&
			eHitTest != HitTestCaption)
			return pControl;
	}
	return nullptr;
}

IControl * CControl::FindChild(const char_16 * szName, int_x iDepth) const
{
	for(int_x cnt = 0, size = m_children.size(); cnt < size; ++cnt)
	{
		const IControl * pControl = m_children[cnt];
		if(pControl->GetName() == szName)
			return (IControl *)pControl;
		else if(iDepth > 0)
		{
			IControl * pChild = pControl->FindChild(szName, iDepth - 1);
			if(pChild)
				return pChild;
		}
		else {}
	}
	return nullptr;
}

void CControl::SetMouseControl(IControl * pControl, pointix point)
{
	if(m_pCaptureControl)
	{
		verify(false);
		return;
	}

	if(pControl != m_pMouseControl)
	{
		pointix ptClient = ToClient(point);

		if(m_pMouseControl)
			m_pMouseControl->PreOnNcMouseOut(m_pMouseControl->ToLocal(ptClient));
		else if(m_bMouseIn && pControl)
			PreOnMouseOut(ptClient);
		else {}

		m_pMouseControl = pControl;
		if(m_pMouseControl)
		{
			m_pMouseControl->PreOnNcMouseIn(m_pMouseControl->ToLocal(ptClient));
			m_pMouseControl->PreOnNcMouseMove(m_pMouseControl->ToLocal(ptClient));
		}
		else if(!m_bMouseIn && HitTest(point) == HitTestClient)
			PreOnMouseIn(ptClient);
		else {}
	}
	else
	{
	}
}

IControl * CControl::GetMouseControl() const
{
	return m_pMouseControl;
}

void CControl::SetCaptureControl(IControl * pControl)
{
	if(pControl != m_pCaptureControl)
	{
		if(!pControl && m_pCaptureControl && m_pCaptureControl->IsCaptured())
			m_pCaptureControl->SetCaptured(false);
		if(m_pParent)
			m_pParent->SetCaptureControl(pControl ? this : nullptr);
		m_pCaptureControl = pControl;
	}
}

IControl * CControl::GetCaptureControl() const
{
	return m_pCaptureControl;
}

void CControl::SetSelectedControl(IControl * pControl)
{
	if(pControl && !m_bSelected)
		Select(true);

	if(pControl != m_pSelectedControl)
	{
		if(m_pSelectedControl)
		{
			verify(m_pSelectedControl->CanSelect());
			m_pSelectedControl->SetSelectedControl(nullptr);
			m_pSelectedControl->SetSelected(false);
		}

		m_pSelectedControl = nullptr;

		if(pControl)
		{
			SetFocused(false);
			m_pSelectedControl = pControl;
			m_pSelectedControl->SetSelected(true);
		}
		else
		{
			if(m_bShown)
				SetFocused(true);
		}
	}
}

IControl * CControl::GetSelectedControl() const
{
	return m_pSelectedControl;
}

IControl * CControl::GetFocusControl() const
{
	if(m_pSelectedControl)
		return m_pSelectedControl->GetFocusControl();
	else if(m_bSelected)
	{
		if(m_bFocused)
			return (IControl *)this;
		else
			return nullptr;
	}
	else
		return nullptr;
}

void CControl::OnChildGetFocus(IControl * pControl)
{
	if(m_pParent)
		m_pParent->OnChildGetFocus(pControl);
}

void CControl::OnChildLostFocus(IControl * pControl)
{
	if(m_pParent)
		m_pParent->OnChildLostFocus(pControl);
}

void CControl::SetMouseWheelMode(MouseWheelModeE eMouseWheelMode)
{
	m_eMouseWheelMode = eMouseWheelMode;
}
MouseWheelModeE CControl::GetMouseWheelMode() const
{
	return m_eMouseWheelMode;
}

IScroll * CControl::ToIScroll() const
{
	return nullptr;
}

IForm * CControl::ToIForm() const
{
	return nullptr;
}

void CControl::OnCheckGroup(int_x iGroup)
{
	verify(iGroup > 0);
}

DropResultE CControl::PreOnNcDragEnter(IData * pData, pointix point)
{
	log0(L"PreOnNcDragEnter[%s].", GetOid().name);
	verify(!m_bMousePreDragIn);
	m_bMousePreDragIn = true;

	bool bMouseIn = NcHitTest(point) == HitTestClient;
	if(bMouseIn)
	{
		pointix ptClient = ToClient(point);
		IControl * pControl = FindChild(ptClient);
		if(pControl)
		{
			SetDropControl(pControl);
			return m_pDropControl->PreOnNcDragEnter(pData, pControl->ToLocal(ptClient));
		}
		else
			return OnDragEnter(pData, ptClient);
	}
	else
		return DragRequestNone;
}

DropResultE CControl::PreOnNcDragMove(IData * pData, pointix point)
{
	DropResultE eRequest = DragRequestNone;
	verify(m_bMousePreDragIn);
	bool bMouseIn = NcHitTest(point) == HitTestClient;

	pointix ptClient = ToClient(point);
	if(!bMouseIn)
	{
		if(m_pDropControl)
		{
			m_pDropControl->PreOnNcDragLeave(pData, m_pDropControl->ToLocal(ptClient));
			SetDropControl(nullptr);
		}

		if(m_bMouseDragIn)
			OnDragLeave(pData, ptClient);

		eRequest = DragRequestNone;
	}
	else
	{
		IControl * pControl = FindChild(ptClient);

		if(pControl != m_pDropControl)
		{
			// 直接从一个子控件上移动到另一个子控件上。
			if(pControl && m_pDropControl)
			{
				m_pDropControl->PreOnNcDragLeave(pData, m_pDropControl->ToLocal(ptClient));
				if(pControl->CanAcceptDrop())
				{
					SetDropControl(pControl);
					eRequest = m_pDropControl->PreOnNcDragEnter(pData, m_pDropControl->ToLocal(ptClient));
				}
				else
					SetDropControl(nullptr);
			}
			// 从一个子控件上移动到 this。
			else if(m_pDropControl)
			{
				m_pDropControl->PreOnNcDragLeave(pData, m_pDropControl->ToLocal(ptClient));
				SetDropControl(nullptr);
				if(!m_bMouseDragIn)
					eRequest = OnDragEnter(pData, ptClient);
			}
			// 从 this 移动到子控件里。
			else/* if(pControl)*/
			{
				if(m_bMouseDragIn)
					OnDragLeave(pData, ptClient);
				if(pControl->CanAcceptDrop())
				{
					SetDropControl(pControl);
					eRequest = m_pDropControl->PreOnNcDragEnter(pData, m_pDropControl->ToLocal(ptClient));
				}
				else
					SetDropControl(nullptr);
			}
		}
		else
		{
			if(!m_pDropControl && !m_bMouseDragIn)
				eRequest = OnDragEnter(pData, ptClient);
		}

		if(m_pDropControl)
			eRequest = m_pDropControl->PreOnNcDragMove(pData, m_pDropControl->ToLocal(ptClient));
		else if(m_bMouseDragIn)
			eRequest = OnDragMove(pData, ptClient);
		else {}
	}

	return eRequest;
}

void CControl::PreOnNcDragLeave(IData * pData, pointix point)
{
	verify(m_bMousePreDragIn);
	m_bMousePreDragIn = false;

	pointix ptClient = ToClient(point);
	if(m_pDropControl)
	{
		m_pDropControl->PreOnNcDragLeave(pData, m_pDropControl->ToLocal(ptClient));
		SetDropControl(nullptr);
	}

	if(m_bMouseDragIn)
		OnDragLeave(pData, ptClient);
}

DropResultE CControl::PreOnNcDragDrop(IData * pData, pointix point)
{
	verify(m_bMousePreDragIn);
	pointix ptClient = ToClient(point);
	if(m_pDropControl)
		m_pDropControl->PreOnNcDragDrop(pData, m_pDropControl->ToLocal(ptClient));
	else if(m_bMouseDragIn)
		OnDragDrop(pData, ptClient);
	else {}
	return DragRequestNone;
}

DropResultE CControl::OnDragEnter(IData * pData, pointix point)
{
	verify(!m_bMouseDragIn);
	m_bMouseDragIn = true;
	if(m_bRepaintMouseDragInOut)
		NcRepaint();

	DropResultE eResult = DragRequestNone;
	DragEnter(this, pData, &eResult);
	return eResult;
}

DropResultE CControl::OnDragMove(IData * pData, pointix point)
{
	verify(m_bMouseDragIn);
	DropResultE eResult = DragRequestNone;
	DragMove(this, pData, &eResult);
	return eResult;
}

void CControl::OnDragLeave(IData * pData, pointix point)
{
	verify(m_bMouseDragIn);
	DragLeave(this, pData);
	m_bMouseDragIn = false;
	if(m_bRepaintMouseDragInOut)
		NcRepaint();
}

DropResultE CControl::OnDragDrop(IData * pData, pointix point)
{
	verify(m_bMouseDragIn);
	DragDrop(this, pData);
	return DragRequestNone;
}

bool CControl::SetDropControl(IControl * pControl)
{
	m_pDropControl = pControl;
	return true;
}

IControl * CControl::GetDropControl() const
{
	return m_pDropControl;
}

bool CControl::IsScrollXAble() const
{
	return m_pScrollX && m_pScrollX->IsVisible() && m_pScrollX->IsEnabled();
}

bool CControl::IsScrollYAble() const
{
	return m_pScrollY && m_pScrollY->IsVisible() && m_pScrollY->IsEnabled();
}

void CControl::PreOnScroll(IControl * pControl, int_x iOffset)
{
	ensure(pControl);
	IScroll * pScroll = pControl->ToIScroll();
	ensure(pScroll);

	int_x iValue = pScroll->GetValue();

	if(pControl == m_pScrollX)
	{
		OnScroll(intx2(iValue, 0), intx2(iOffset, 0));
		if(m_bRepaintScrollX)
			NcRepaint();
	}
	else if(pControl == m_pScrollY)
	{
		OnScroll(intx2(0, iValue), intx2(0, iOffset));
		if(m_bRepaintScrollY)
			NcRepaint();
	}
	else {}
}

void CControl::OnScroll(intx2 scroll, intx2 offset)
{

}

void CControl::SetScrollPosX(int_x iX)
{
	if(IsScrollXAble())
		m_pScrollX->ToIScroll()->SetValue(iX);
}

void CControl::SetScrollPosY(int_x iY)
{
	if(IsScrollYAble())
		m_pScrollY->ToIScroll()->SetValue(iY);
}

void CControl::SetScrollPos(int_x iX, int_x iY)
{
	if(IsScrollXAble())
		m_pScrollX->ToIScroll()->SetValue(iX);
	if(IsScrollYAble())
		m_pScrollY->ToIScroll()->SetValue(iY);
}

int_x CControl::GetScrollPosX() const
{
	if(IsScrollXAble())
		return m_pScrollX->ToIScroll()->GetValue();
	else
		return 0;
}

int_x CControl::GetScrollPosY() const
{
	if(IsScrollYAble())
		return m_pScrollY->ToIScroll()->GetValue();
	else
		return 0;
}

pointix CControl::GetScrollPos() const
{
	return pointix(GetScrollPosX(), GetScrollPosY());
}


void CControl::SetScrollBarX(IControl * pScrollBar)
{
	m_pScrollX = pScrollBar;
	if(m_pScrollX)
	{
		m_pScrollX->SetSize(0, App().SystemMetrics(IApp::SM_ScroolbarW));
		m_pScrollX->SetVisible(m_bAlwaysShowScrollX);
		m_pScrollX->SetNcControl(true);
		m_pScrollX->SetZOrder(IX_MAX);
		m_pScrollX->ToIScroll()->SetDirection(AxisX);
		m_pScrollX->ToIScroll()->SetScrollTarget(this);
	}
}

void CControl::SetScrollBarY(IControl * pScrollBar)
{
	m_pScrollY = pScrollBar;
	if(m_pScrollY)
	{
		m_pScrollY->SetSize(App().SystemMetrics(IApp::SM_ScroolbarH), 0);
		m_pScrollY->SetVisible(m_bAlwaysShowScrollY);
		m_pScrollY->SetNcControl(true);
		m_pScrollY->SetZOrder(IX_MAX);
		m_pScrollY->ToIScroll()->SetDirection(AxisY);
		m_pScrollY->ToIScroll()->SetScrollTarget(this);
	}
}

IControl * CControl::GetScrollBarX() const
{
	return nullptr;
}

IControl * CControl::GetScrollBarY() const
{
	return nullptr;
}

void CControl::SetAutoHideScrollBarX(bool bAutoHide)
{
	if(bAutoHide == m_bAlwaysShowScrollX)
	{
		m_bAlwaysShowScrollX = !bAutoHide;
		if(m_pScrollX)
		{
			if(!bAutoHide && !m_pScrollX->IsVisible())
			{
				m_pScrollX->SetVisible(true);
				OnVisualSizeChanged();
			}

			if(bAutoHide && !m_pScrollX->IsEnabled() && m_pScrollX->IsVisible())
			{
				m_pScrollX->SetVisible(false);
				OnVisualSizeChanged();
			}
		}
	}
}
void CControl::SetAutoHideScrollBarY(bool bAutoHide)
{
	if(bAutoHide == m_bAlwaysShowScrollY)
	{
		m_bAlwaysShowScrollY = !bAutoHide;
		if(m_pScrollY)
		{
			if(!bAutoHide && !m_pScrollY->IsVisible())
			{
				m_pScrollY->SetVisible(true);
				OnVisualSizeChanged();
			}

			if(bAutoHide && !m_pScrollY->IsEnabled() && m_pScrollY->IsVisible())
			{
				m_pScrollY->SetVisible(false);
				OnVisualSizeChanged();
			}
		}
	}
}

bool CControl::IsAutoHideScrollBarX() const
{
	return !m_bAlwaysShowScrollX;
}

bool CControl::IsAutoHideScrollBarY() const
{
	return !m_bAlwaysShowScrollY;
}

void CControl::UpdateScroll()
{
	if(!m_pScrollX && !m_pScrollY)
		return;
	SetScroll(0, 0);
}

void CControl::SetScroll(intx2 i2Scroll)
{
	SetScroll(i2Scroll.cx, i2Scroll.cy);
}

void CControl::SetScroll(int_x icxScroll, int_x icyScroll)
{
	if(!m_pScrollX && !m_pScrollY)
		return;

	rectix rcVisual = GetVisual();
	bool bScrollX = false;
	bool bScrollY = false;
	IScroll * pScrollX = nullptr;
	IScroll * pScrollY = nullptr;
	if(rcVisual.is_valid())
	{
		if(m_pScrollY)
		{
			pScrollY = m_pScrollY->ToIScroll();
			if(icyScroll > rcVisual.h)
			{
				icxScroll += SCROLL_DEF_SIZE;
				bScrollY = true;
			}
			else if(m_bAlwaysShowScrollY)
				icxScroll += SCROLL_DEF_SIZE;
			else {}
		}
		else {}

		if(m_pScrollX)
		{
			pScrollX = m_pScrollX->ToIScroll();
			if(icxScroll > rcVisual.w)
			{
				bScrollX = true;
			}
			else {}

			if(bScrollX && m_pScrollY && icyScroll + SCROLL_DEF_SIZE > rcVisual.h)
			{
				icyScroll += SCROLL_DEF_SIZE;
				bScrollY = true;
			}
		}
	}

	if(bScrollX)
	{
		int_x iMaxValue = icxScroll - rcVisual.w;
		int_x iValue = saturate(pScrollX->GetValue(), (int_x)0, iMaxValue);
		pScrollX->SetScrollInfo(0, iMaxValue, iValue, rcVisual.w / LINE_RATE, rcVisual.w / PAGE_RATE);
	}
	else if(pScrollX)
		pScrollX->SetScrollInfo(0, 0, 0, 0, 0);
	else {}

	if(bScrollY)
	{
		int_x iMaxValue = icyScroll - rcVisual.h;
		int_x iValue = saturate(pScrollY->GetValue(), (int_x)0, iMaxValue);
		pScrollY->SetScrollInfo(0, iMaxValue, iValue, rcVisual.h / LINE_RATE, rcVisual.h / PAGE_RATE);
	}
	else if(pScrollY)
		pScrollY->SetScrollInfo(0, 0, 0, 0, 0);
	else {}

	_LayoutScrollBars(bScrollX, bScrollY);
}

intx2 CControl::GetScroll() const
{
	intx2 i2Scroll;
	if(IsScrollXAble())
		i2Scroll.cx = m_pScrollX->ToIScroll()->GetMaxValue();
	if(IsScrollYAble())
		i2Scroll.cy = m_pScrollY->ToIScroll()->GetMaxValue();
	return i2Scroll;
}

void CControl::_LayoutScrollBars(bool bScrollX, bool bScrollY)
{
	rectix rcVisual = GetVisual();
	rectix rcBarX(0, rcVisual.h - App().SystemMetrics(IApp::SM_ScroolbarH), rcVisual.w, App().SystemMetrics(IApp::SM_ScroolbarH));
	rectix rcBarY(rcVisual.w - App().SystemMetrics(IApp::SM_ScroolbarW), 0, App().SystemMetrics(IApp::SM_ScroolbarW), rcVisual.h);
	if(bScrollY || m_bAlwaysShowScrollY)
	{
		rcBarX.w -= SCROLL_DEF_SIZE;
		m_pScrollY->SetVisible(bScrollY || m_bAlwaysShowScrollY);
		m_pScrollY->SetEnable(bScrollY);
		m_pScrollY->SetRect(rcBarY);
	}
	else if(m_pScrollY)
	{
		m_pScrollY->SetVisible(false);
		m_pScrollY->SetEnable(false);
		m_pScrollY->SetRect(rcBarY);
	}
	else {}

	if(bScrollX || m_bAlwaysShowScrollX)
	{
		m_pScrollX->SetVisible(bScrollX || m_bAlwaysShowScrollX);
		m_pScrollX->SetEnable(bScrollX);
		m_pScrollX->SetRect(rcBarX);
	}
	else if(m_pScrollX)
	{
		m_pScrollX->SetVisible(false);
		m_pScrollX->SetEnable(false);
		m_pScrollX->SetRect(rcBarX);
	}
	else {}
}

void CControl::DoAttributeExchange(IAttributeServer * pServer, bool bPush)
{
	if(bPush)
	{
		pServer->Push(m_bDisabled, L"Disabled");
		pServer->Push(m_rect, L"Rect");
		pServer->Push(m_rect.x, L"X");
		pServer->Push(m_rect.y, L"X");
		pServer->Push(m_rect.w, L"Width");
		pServer->Push(m_rect.h, L"Height");
	}
	else
	{
		bool bValue = false;
		if(pServer->Pull(bValue, L"Disabled"))
			m_bDisabled = bValue;
		pServer->Pull(m_rect.x, L"X");
		pServer->Pull(m_rect.y, L"X");
		pServer->Pull(m_rect.w, L"Width");
		pServer->Pull(m_rect.h, L"Height");
		pServer->Pull(m_rect, L"Rect");
	}
}

void CControl::OutputInfo(int_x iDepth) const
{
	char_16 szBuffer[512] = {0};
	char_16 szTemp[64] = {0};
	int_x iIndex = 0;
	for(int_x cnt = 0; cnt < iDepth; ++cnt)
	{
		szTemp[iIndex++] = L' ';
		szTemp[iIndex++] = L'┊';
	}

	const cid_t & oid = GetOid();

	textcat_format(szBuffer, 512, L"%s%s \"%s\" [%d,%d,%d,%d](enable[%d]|visible[%d]|shown[%d])",
		szTemp, oid.name, m_name.buffer(),
		m_rect.x, m_rect.y, m_rect.w, m_rect.h,
		!m_bDisabled, !m_bInvisible, m_bShown);
	log0(szBuffer);
	++iDepth;
	for_cis(m_children, [&](const IControl * pControl) { pControl->OutputInfo(iDepth); });
}

void CControl::QueryDebugMenu(int_x & iBase, IMenu * pMenu) const
{
	MenuT * pMenuBaseAttr = new MenuT();

	pMenuBaseAttr->AddItem(iBase + 1, L"Enable", !m_bDisabled);
	pMenuBaseAttr->AddItem(iBase + 0, L"Visible", MenuFlagDisable | MenuFlagSelected);
	pMenuBaseAttr->AddItem(iBase + 8, L"Transparent", m_bTransparent);
	pMenuBaseAttr->AddItem(iBase + 9, L"ClipChild", m_bClipChild);
	pMenuBaseAttr->AddItem(iBase + 10, L"Static", m_bStatic);
	pMenuBaseAttr->AddSplit();
	pMenuBaseAttr->AddItem(iBase + 3, L"SelectAble", !m_bDisSelectAble);
	pMenuBaseAttr->AddItem(iBase + 4, L"FocusAble", m_bFocusAble);
	pMenuBaseAttr->AddSplit();
	pMenuBaseAttr->AddItem(iBase + 5, L"AcceptDrop", m_bAcceptDrop);
	pMenuBaseAttr->AddItem(iBase + 6, L"AcceptTab", m_bAcceptTab);
	pMenuBaseAttr->AddItem(iBase + 7, L"AcceptEnter", m_bAcceptEnter);
	pMenuBaseAttr->AddItem(iBase + 11, L"AcceptScroll", !m_bNotAcceptScroll);
	pMenuBaseAttr->AddSplit();
	pMenuBaseAttr->AddItem(iBase + 12, L"AutoHideScrollX", !m_bAlwaysShowScrollX);
	pMenuBaseAttr->AddItem(iBase + 13, L"AutoHideScrollY", !m_bAlwaysShowScrollY);
	pMenuBaseAttr->AddSplit();
	pMenuBaseAttr->AddItem(0, L"Aviliable", IsAviliable() ? (MenuFlagSelected | MenuFlagDisable) : MenuFlagDisable);
	pMenuBaseAttr->AddItem(0, L"Shown", m_bShown ? (MenuFlagSelected | MenuFlagDisable) : MenuFlagDisable);
	pMenuBaseAttr->AddItem(0, L"Selected", m_bSelected ? (MenuFlagSelected | MenuFlagDisable) : MenuFlagDisable);
	pMenuBaseAttr->AddItem(0, L"Focused", m_bFocused ? (MenuFlagSelected | MenuFlagDisable) : MenuFlagDisable);

	MenuT * pMenuImeMode = new MenuT();
	pMenuImeMode->AddItem(iBase + 100, L"ImeModeDefault", MenuGetFlags(m_eImeMode == ImeModeDefault));
	pMenuImeMode->AddItem(iBase + 101, L"ImeModeDisable", MenuGetFlags(m_eImeMode == ImeModeDisable));
	pMenuImeMode->AddItem(iBase + 102, L"ImeModeOn", MenuGetFlags(m_eImeMode == ImeModeOn));
	pMenuImeMode->AddItem(iBase + 103, L"ImeModeOff", MenuGetFlags(m_eImeMode == ImeModeOff));

	MenuT * pMenuCursor = new MenuT();
	pMenuCursor->AddItem(iBase + 200 + CursorUnknown, L"CursorUnknown", MenuGetFlags(m_eCursor == CursorUnknown));
	pMenuCursor->AddItem(iBase + 200 + CursorNormal, L"CursorNormal", MenuGetFlags(m_eCursor == CursorNormal));
	pMenuCursor->AddItem(iBase + 200 + CursorCross, L"CursorCross", MenuGetFlags(m_eCursor == CursorCross));
	pMenuCursor->AddItem(iBase + 200 + CursorHand, L"CursorHand", MenuGetFlags(m_eCursor == CursorHand));
	pMenuCursor->AddItem(iBase + 200 + CursorLink, L"CursorLink", MenuGetFlags(m_eCursor == CursorLink));
	pMenuCursor->AddItem(iBase + 200 + CursorHelp, L"CursorHelp", MenuGetFlags(m_eCursor == CursorHelp));
	pMenuCursor->AddItem(iBase + 200 + CursorWait, L"CursorWait", MenuGetFlags(m_eCursor == CursorWait));
	pMenuCursor->AddItem(iBase + 200 + CursorIBeam, L"CursorIBeam", MenuGetFlags(m_eCursor == CursorIBeam));
	pMenuCursor->AddItem(iBase + 200 + CursorNotAviliable, L"CursorNotAviliable", MenuGetFlags(m_eCursor == CursorNotAviliable));
	pMenuCursor->AddItem(iBase + 200 + CursorSizeLR, L"CursorSizeLR", MenuGetFlags(m_eCursor == CursorSizeLR));
	pMenuCursor->AddItem(iBase + 200 + CursorSizeTB, L"CursorSizeTB", MenuGetFlags(m_eCursor == CursorSizeTB));
	pMenuCursor->AddItem(iBase + 200 + CursorSizeLTRB, L"CursorSizeLTRB", MenuGetFlags(m_eCursor == CursorSizeLTRB));
	pMenuCursor->AddItem(iBase + 200 + CursorSizeLBRT, L"CursorSizeLBRT", MenuGetFlags(m_eCursor == CursorSizeLBRT));
	pMenuCursor->AddItem(iBase + 200 + CursorSizeAll, L"CursorSizeAll", MenuGetFlags(m_eCursor == CursorSizeAll));

	MenuT * pMenuFont = new MenuT();
	pMenuFont->AddItem(iBase + 301, L"Bold", m_font.weight == FONT_WEIGHT_BOLD);
	pMenuFont->AddItem(iBase + 302, L"Italic", m_font.italic);
	pMenuFont->AddItem(iBase + 303, L"UnderLine", m_font.underline);
	pMenuFont->AddItem(iBase + 304, L"StrkeOut", m_font.strikeout);

	MenuT * pMenuForeColor = new MenuT();
	pMenuForeColor->AddItem(iBase + 401, L"Black", MenuGetFlags(m_uiForeColor == Colors::Black));
	pMenuForeColor->AddItem(iBase + 402, L"Red", MenuGetFlags(m_uiForeColor == Colors::Red));
	pMenuForeColor->AddItem(iBase + 403, L"Green", MenuGetFlags(m_uiForeColor == Colors::Green));
	pMenuForeColor->AddItem(iBase + 404, L"Blue", MenuGetFlags(m_uiForeColor == Colors::Blue));
	pMenuForeColor->AddItem(iBase + 405, L"White", MenuGetFlags(m_uiForeColor == Colors::White));

	MenuT * pMenuBackColor = new MenuT();
	pMenuBackColor->AddItem(iBase + 501, L"Auto", MenuGetFlags(m_uiBackColor == Colors::Auto));
	pMenuBackColor->AddItem(iBase + 502, L"Black", MenuGetFlags(m_uiBackColor == Colors::Black));
	pMenuBackColor->AddItem(iBase + 503, L"Red", MenuGetFlags(m_uiBackColor == Colors::Red));
	pMenuBackColor->AddItem(iBase + 504, L"Green", MenuGetFlags(m_uiBackColor == Colors::Green));
	pMenuBackColor->AddItem(iBase + 505, L"Blue", MenuGetFlags(m_uiBackColor == Colors::Blue));
	pMenuBackColor->AddItem(iBase + 506, L"White", MenuGetFlags(m_uiBackColor == Colors::White));
	pMenuBackColor->AddItem(iBase + 507, L"Tomato", MenuGetFlags(m_uiBackColor == Colors::Tomato));
	pMenuBackColor->AddItem(iBase + 508, L"Gray", MenuGetFlags(m_uiBackColor == Colors::Gray));

	MenuT * pMenuBorder = new MenuT();
	pMenuBorder->AddItem(iBase + 601, L"None", MenuGetFlags(m_eBorderType == BorderTypeNone));
	pMenuBorder->AddItem(iBase + 602, L"1D", MenuGetFlags(m_eBorderType == BorderType1));
	pMenuBorder->AddItem(iBase + 603, L"2D", MenuGetFlags(m_eBorderType == BorderType2));
	pMenuBorder->AddItem(iBase + 604, L"3D", MenuGetFlags(m_eBorderType == BorderType3));
	pMenuBorder->AddItem(iBase + 605, L"4D", MenuGetFlags(m_eBorderType == BorderType4));
	pMenuBorder->AddItem(iBase + 606, L"5D", MenuGetFlags(m_eBorderType == BorderType5));
	pMenuBorder->AddItem(iBase + 607, L"1D Inactive", MenuGetFlags(m_eBorderType == BorderType1Inactive));
	pMenuBorder->AddItem(iBase + 608, L"2D Inactive", MenuGetFlags(m_eBorderType == BorderType2Inactive));
	pMenuBorder->AddItem(iBase + 609, L"3D Inactive", MenuGetFlags(m_eBorderType == BorderType3Inactive));
	pMenuBorder->AddItem(iBase + 610, L"4D Inactive", MenuGetFlags(m_eBorderType == BorderType4Inactive));
	pMenuBorder->AddItem(iBase + 611, L"5D Inactive", MenuGetFlags(m_eBorderType == BorderType5Inactive));

	MenuT * pMenuResult = new MenuT();
	pMenuResult->AddItem(iBase + 701, L"Cancel");
	pMenuResult->AddItem(iBase + 702, L"Ok");

	pMenu->AddItem(0, GetOid().name, MenuFlagDisable);
	pMenu->AddSplit();
	pMenu->AddItem(0, L"BaseAttr", pMenuBaseAttr);
	pMenu->AddItem(0, L"ImeMode", pMenuImeMode);
	pMenu->AddItem(0, L"Cursor", pMenuCursor);
	pMenu->AddItem(0, L"Font", pMenuFont);
	pMenu->AddItem(0, L"ForeColor", pMenuForeColor);
	pMenu->AddItem(0, L"BackColor", pMenuBackColor);
	pMenu->AddItem(0, L"BorderType", pMenuBorder);
	pMenu->AddItem(0, L"DialogResult", pMenuResult);
	pMenu->AddItem(-1, L"Output Info");

	SafeRelease(pMenuBaseAttr);
	SafeRelease(pMenuImeMode);
	SafeRelease(pMenuCursor);
	SafeRelease(pMenuFont);
	SafeRelease(pMenuForeColor);
	SafeRelease(pMenuBackColor);
	SafeRelease(pMenuBorder);
	SafeRelease(pMenuResult);

	iBase += 800;
}

void CControl::OnDebugMenu(int_x iBase, int_x iResult)
{
	if(200 + CursorUnknown <= iResult && iResult <= 200 + CursorSizeAll)
		SetCursor((CursorE)(iResult - 200));
	switch(iResult)
	{
	case -1:
		OutputInfo(0);
		break;
	case 1:
		SetEnable(m_bDisabled);
		break;
	case 3:
		SetSelectAble(m_bDisSelectAble);
		break;
	case 4:
		SetFocusAble(!m_bFocusAble);
		break;
	case 5:
		m_bAcceptDrop = !m_bAcceptDrop;
		break;
	case 6:
		m_bAcceptTab = !m_bAcceptTab;
		break;
	case 7:
		m_bAcceptEnter = !m_bAcceptEnter;
		break;
	case 8:
		m_bTransparent = !m_bTransparent;
		break;
	case 9:
		m_bClipChild = !m_bClipChild;
		break;
	case 10:
		m_bStatic = !m_bStatic;
		break;
	case 11:
		SetAcceptScroll(m_bNotAcceptScroll);
		break;
	case 12:
		SetAutoHideScrollBarX(m_bAlwaysShowScrollX);
		break;
	case 13:
		SetAutoHideScrollBarY(m_bAlwaysShowScrollY);
		break;

	case 100:
		SetImeMode(ImeModeDefault);
		break;
	case 101:
		SetImeMode(ImeModeDisable);
		break;
	case 102:
		SetImeMode(ImeModeOn);
		break;
	case 103:
		SetImeMode(ImeModeOff);
		break;
	case 301:
		SetFontFlag(FontFlagBold, m_font.weight != FONT_WEIGHT_BOLD);
		break;
	case 302:
		SetFontFlag(FontFlagItalic, !m_font.italic);
		break;
	case 303:
		SetFontFlag(FontFlagUnderLine, !m_font.underline);
		break;
	case 304:
		SetFontFlag(FontFlagStrikeOut, !m_font.strikeout);
		break;
	case 401:
		SetForeColor(Colors::Black);
		break;
	case 402:
		SetForeColor(Colors::Red);
		break;
	case 403:
		SetForeColor(Colors::Green);
		break;
	case 404:
		SetForeColor(Colors::Blue);
		break;
	case 405:
		SetForeColor(Colors::White);
		break;
	case 501:
		SetBackColor(Colors::Auto);
		break;
	case 502:
		SetBackColor(Colors::Black);
		break;
	case 503:
		SetBackColor(Colors::Red);
		break;
	case 504:
		SetBackColor(Colors::Green);
		break;
	case 505:
		SetBackColor(Colors::Blue);
		break;
	case 506:
		SetBackColor(Colors::White);
		break;
	case 507:
		SetBackColor(Colors::Tomato);
		break;
	case 508:
		SetBackColor(Colors::Gray);
		break;

	case 601: SetBorderType(BorderTypeNone); break;
	case 602: SetBorderType(BorderType1); break;
	case 603: SetBorderType(BorderType2); break;
	case 604: SetBorderType(BorderType3); break;
	case 605: SetBorderType(BorderType4); break;
	case 606: SetBorderType(BorderType5); break;
	case 607: SetBorderType(BorderType1Inactive); break;
	case 608: SetBorderType(BorderType2Inactive); break;
	case 609: SetBorderType(BorderType3Inactive); break;
	case 610: SetBorderType(BorderType4Inactive); break;
	case 611: SetBorderType(BorderType5Inactive); break;

	case 701:
		if(IForm * pForm = GetAncestor()->ToIForm())
			GetApp()->EndRun(pForm->GetFormId(), DialogResultCancel);
		break;
	case 702:
		if(IForm * pForm = GetAncestor()->ToIForm())
			GetApp()->EndRun(pForm->GetFormId(), DialogResultOk);
		break;
	default:
		break;
	}
}

VENUS_END

