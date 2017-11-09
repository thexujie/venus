#pragma once

VENUS_BEG

static const cid_t OID_Control = {L"control"};
static const cid_t OID_Space = {L"space"};
static const cid_t OID_Form = {L"form"};
static const cid_t OID_Static = {L"static"};
static const cid_t OID_Lable = {L"label"};
static const cid_t OID_Button = {L"button"};
static const cid_t OID_Radio = {L"radio"};
static const cid_t OID_CheckBox = {L"checkbox"};
static const cid_t OID_ScrollBar = {L"scrollbar"};
static const cid_t OID_ProgressBar = {L"progressbar"};
static const cid_t OID_TextLine = {L"textline"};
static const cid_t OID_Layout = {L"layout"};

static const cid_t OID_ListBox = {L"listbox"};
static const cid_t OID_TextBox = {L"textbox"};
static const cid_t OID_GroupBox = {L"groupbox"};
static const cid_t OID_TabControl = {L"tabcontrol"};

static const cid_t OID_ListView = {L"listview"};
static const cid_t OID_TreeView = {L"treeview"};
static const cid_t OID_FoldingView = {L"foldingview"};

static const cid_t OID_MenuBar = {L"menubar"};

static const cid_t OID_Pane = {L"pane"};

static const cid_t OID_IHost = {L"IHost"};

/**
* 表示范围信息，最小值、最大值和当前值。
*/
class V2D_API RangeInfoT
{
public:
	RangeInfoT() : MinValue(0), MaxValue(100), Value(50) {}
	RangeInfoT(int_x iMin, int_x iMax, int_x iValue)
		: MinValue(iMin), MaxValue(iMax), Value(iValue)
	{
	}
	~RangeInfoT() {}


	int_x range() const { return MaxValue - MinValue; }
	int_x MinValue;
	int_x MaxValue;
	int_x Value;
};

/**
* 表示滚动信息，最小值、最大值、当前值、行值和页值。
*/
class V2D_API ScrollInfoT : public RangeInfoT
{
public:
	ScrollInfoT() : LineValue(1), PageValue(10) {}
	ScrollInfoT(int_x iMin, int_x iMax, int_x iValue, int_x iLine, int_x iPage)
		: RangeInfoT(iMin, iMax, iValue), LineValue(iLine), PageValue(iPage)
	{
	}
	~ScrollInfoT() {}

	int_x LineValue;
	int_x PageValue;
};

class V2D_API ImeInfoT
{
public:
	ImeModeE eImeMode;
	rectix rcCompose;
	font_t font;
};

class V2D_API anchor_t
{
public:
	AlignE type;
	edgeix edge;

	anchor_t():type(AlignNone)
	{

	}

	anchor_t(AlignE _type, int_x _left, int_x _top, int_x _right, int_x _bottom) :
	type(_type), edge(_left, _top, _right, _bottom)
	{

	}

	anchor_t(const anchor_t & another) = default;
	anchor_t & operator = (const anchor_t & another) = default;
	bool operator==(const anchor_t & another) const
	{
		return type == another.type && 
			edge == another.edge;
	}
	bool operator!=(const anchor_t & another) const { return !operator==(another); }
	int_x width() const { return edge.width(); }
	int_x height() const { return edge.height(); }
};

class IControl;
class IHost;
class IForm;
class IGroup;
class IPopupFrame;
class ITipsFrame;
class IPopupFrame;
class IPopupHost;
class IScroll;
class IFramework;
class IUITheme;
class IMenu;
class IAttributeServer;

typedef vector<IControl *> IControlVector;

class V2D_API IControl : public IObject
{
public:
	virtual ~IControl() {}

	const cid_t & GetOid() const override = 0;
	virtual void GetMeta(IMetaHelper * pHelper) = 0;

	virtual void SetParent(IControl * pParent) = 0;

	virtual IControl * GetParent() const = 0;
	virtual IControl * GetAncestor() const = 0;
	virtual IForm * GetForm() const = 0;


	virtual IScroll * ToIScroll() const = 0;
	virtual IForm * ToIForm() const = 0;

	virtual void SetWidthMode(WHModeE eMode) = 0;
	virtual WHModeE GetWidthMode() const = 0;
	virtual void SetHeightMode(WHModeE eMode) = 0;
	virtual WHModeE GetHeightMode() const = 0;

	virtual rectix GetRect() const = 0;
	virtual void SetRect(const rectix & rect) = 0;
	virtual void SetRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight) = 0;

	virtual pointix GetPosition() const = 0;
	virtual void SetPosition(pointix position) = 0;
	virtual void SetPosition(int_x iX, int_x iY) = 0;

	virtual sizeix GetSize() const = 0;
	virtual void SetSize(sizeix size) = 0;
	virtual void SetSize(int_x iWidth, int_x iHeight) = 0;

	virtual int_x GetX() const = 0;
	virtual int_x GetY() const = 0;
	virtual int_x GetWidth() const = 0;
	virtual int_x GetHeight() const = 0;

	virtual void SetX(int_x iWidth) = 0;
	virtual void SetY(int_x iWidth) = 0;
	virtual void SetWidth(int_x iWidth) = 0;
	virtual void SetHeight(int_x iHeight) = 0;

	virtual pointix GetRelativePosition(IControl * pAncestor) const = 0;

	// 内容大小
	virtual sizeix GetContentSize() const = 0;
	// 期望大小，包括子控件延伸开来的
	virtual sizeix GetPreferedSize() const = 0;

	virtual int_x GetRight() const = 0;
	virtual int_x GetBottom() const = 0;

	virtual void SetMargin(edgeix margin) = 0;
	virtual edgeix GetMargin() const = 0;
	virtual void SetPadding(const edgeix & padding) = 0;
	virtual edgeix GetPadding() const = 0;

	virtual edgeix GetBorder() const = 0;
	virtual rectix GetVisual() const = 0;
	virtual rectix GetClient() const = 0;
	virtual pointix GetOffset() const = 0;
	
	virtual void SetWeight(float_32 weight) = 0;
	virtual float_32 GetWeight() const = 0;

	virtual void SetText(const char_16 * szText, int_x iLength = -1) = 0;
	virtual void SetText(textw text) = 0;
	virtual const textw & GetText() const = 0;

	virtual void SetName(const textw & name) = 0;
	virtual const textw & GetName() const = 0;

	virtual void SetId(int_x iID) = 0;
	virtual int_x GetId() const = 0;

	virtual void SetZOrder(int_x iZOrder) = 0;
	virtual int_x GetZorder() const = 0;

	virtual void SetDynamic(bool bDynamic) = 0;
	virtual bool IsDynamic() const = 0;

	virtual void SetEnable(bool bEnable) = 0;
	virtual bool IsEnabled() const = 0;
	virtual bool IsAviliable() const = 0;
	virtual bool IsVisible() const = 0;
	virtual void SetVisible(bool bVisible) = 0;
	virtual bool IsShown() const = 0;
	virtual bool IsHidden() const = 0;
	virtual void SetShown(bool bShow) = 0;
	virtual void OnShow() = 0;
	virtual	void OnHide() = 0;

	virtual void SetNcControl(bool bNcControl) = 0;
	virtual bool IsNcControl() const = 0;

	virtual void OnAdded(IControl * pContainer) = 0;
	virtual void OnRemoved(IControl * pContainer) = 0;

	//! PreOnNcPaint 不可以是 const 的。
	virtual void PreOnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) = 0;
	virtual void OnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const = 0;
	virtual void PreOnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const = 0;
	virtual void AftOnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const = 0;
	virtual void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const = 0;

	virtual void SetAutoRepaint(uint_32 uiAutoRepaint) = 0;
	virtual uint_32 GetAutoRepaint() const = 0;
	
	virtual void Repaint() = 0;
	virtual void Repaint(const rectix & rect) = 0;
	virtual void NcRepaint() = 0;
	virtual void NcRepaint(const rectix & rect) = 0;

	virtual void Capture(bool bCapture) = 0;
	virtual void SetCaptured(bool bCaptured) = 0;
	virtual bool IsCaptured() const = 0;

	virtual void SetImeMode(ImeModeE eImeMode) = 0;
	virtual ImeModeE GetImeMode() const = 0;
	virtual void UpdateIme() = 0;
	virtual bool GetImeInfo(ImeInfoT & imeInfo) const = 0;

	virtual void SetAcceptDrop(bool bAcceptDrop) = 0;
	virtual bool IsAcceptDrop() const = 0;
	virtual bool CanAcceptDrop() const = 0;

	virtual void SetHotKey(KeyCodeE eHotKey) = 0;
	virtual KeyCodeE GetHotKey() const = 0;
	virtual bool OnHotKey() = 0;

	virtual void SetPopuping(bool bPopuping) = 0;
	virtual bool IsPopuping() const = 0;

	virtual void SetBorderType(BorderTypeE eBorderType) = 0;
	virtual BorderTypeE GetBorderType() const = 0;

	virtual void SetFont(const font_t & font) = 0;
	virtual void SetFontName(const char_16 * szFontName) = 0;
	virtual void SetFontSize(int_x iFontSize) = 0;
	virtual void SetFontRenderLevel(FontRenderlevelE eRenderLevel) = 0;
	virtual const font_t & GetFont() const = 0;

	virtual void SetBackColor(uint_32 uiBackColor) = 0;
	virtual void SetForeColor(uint_32 uiForeColor) = 0;
	virtual uint_32 GetBackColor() const = 0;
	virtual uint_32 GetForeColor() const = 0;

	virtual void SetCursor(CursorE eCursor) = 0;
	virtual CursorE GetCursor() const = 0;
	virtual void NcRequestCursor(pointix point, CursorE & eCursor) const = 0;
	virtual void RequestCursor(pointix point, CursorE & eCursor) const = 0;

	// 焦点相关。
	virtual void SetFocusAble(bool bCanFocus) = 0;
	virtual void SetSelectAble(bool bSelectAble) = 0;

	virtual bool IsFocusAble() const = 0;
	virtual bool IsSelectAble() const = 0;

	virtual bool CanSelect() const = 0;
	virtual bool CanFocus() const = 0;
	virtual bool Focus(bool bFocus) = 0;

	virtual void SetFocused(bool bFocused) = 0;
	virtual bool IsFocused() const = 0;

	virtual void Select(bool bSelect) = 0;
	virtual void SetSelected(bool bSelected) = 0;
	virtual bool IsSelected() const = 0;

	virtual void SetStatic(bool bStatic) = 0;
	virtual bool IsStatic() const = 0;
	virtual void SetTransparent(bool bTransparent) = 0;
	virtual bool IsTransparent() const = 0;
	virtual void SetClipChild(bool bClipChild) = 0;
	virtual bool IsClipChild() const = 0;

	virtual void SetAcceptTab(bool bAcceptTab) = 0;
	virtual bool IsAcceptTab() const = 0;
	virtual bool CanAcceptTab() const = 0;

	virtual void SetAcceptEnter(bool bAcceptEnter) = 0;
	virtual bool IsAcceptEnter() const = 0;
	virtual bool CanAcceptEnter() const = 0;

	virtual void SetAcceptScroll(bool bAcceptScroll) = 0;
	virtual bool IsAcceptScroll() const = 0;
	virtual bool CanAcceptScroll() const = 0;

	virtual err_t SetParam(const char_16 * szName, const char_16 * szValue) = 0;

	// Clipboard
	virtual bool ClipboardCheckData(ClipboardDataTypeE eDataType) const = 0;
	virtual textw ClipboardGetText16() const = 0;
	virtual void ClipboardSetText16(const char_16 * szText, int_x iLength = -1) const = 0;
	// 锚定父控件。
	virtual void SetAnchor(anchor_t anchor) = 0;
	virtual anchor_t GetAnchor() const = 0;
	virtual void Anchor() = 0;

	// ----------------------坐标转换。
	//! 父控件 客户区 -> 非客户区
	virtual pointix ToLocal(const pointix & point) const = 0;
	//! 非客户区 -> 父控件 客户区
	virtual pointix ToGlobal(const pointix & point) const = 0;

	//! 非客户区 -> 客户区
	virtual pointix ToClient(const pointix & point) const = 0;
	//! 客户区 ->非客户区
	virtual pointix ToNoneCl(const pointix & point) const = 0;
	//! 从客户区坐标转换到全局坐标。
	virtual pointix ToScreen(const pointix & point) const = 0;

	virtual HitTestE NcHitTest(pointix point) const = 0;
	virtual HitTestE HitTest(pointix point) const = 0;
	virtual HitTestE HitTestDeep(pointix point) const = 0;
	
	virtual void ShowTips(pointix point, const char_16 * szTips) = 0;
	virtual void HideTips() = 0;
	virtual void UpdateTips(pointix point) = 0;

	virtual int_x PopupMenu(pointix point, IMenu * pMenu) = 0;
	virtual int_x PopupControl(pointix point, IControl * pControl) = 0;
public:
	virtual void NcUpdateMouse(pointix point) = 0;
	// 鼠标交互
	virtual void PreOnNcMouseMove(pointix point) = 0;
	virtual void OnNcMouseMove(pointix point) = 0;
	virtual void PreOnMouseMove(pointix point) = 0;
	virtual void OnMouseMove(pointix point) = 0;

	virtual void PreOnNcMouseIn(pointix point) = 0;
	virtual void OnNcMouseIn(pointix point) = 0;
	virtual void PreOnMouseIn(pointix point) = 0;
	virtual void OnMouseIn(pointix point) = 0;

	virtual void PreOnNcMouseOut(pointix point) = 0;
	virtual void OnNcMouseOut(pointix point) = 0;
	virtual void PreOnMouseOut(pointix point) = 0;
	virtual void OnMouseOut(pointix point) = 0;

	virtual void PreOnNcMouseDown(pointix point, MouseButtonE eButton) = 0;
	virtual void OnNcMouseDown(pointix point, MouseButtonE eButton) = 0;
	virtual void PreOnMouseDown(pointix point, MouseButtonE eButton) = 0;
	virtual void OnMouseDown(pointix point, MouseButtonE eButton) = 0;
	virtual void OnMouseDownL(pointix point) = 0;
	virtual void OnMouseDownM(pointix point) = 0;
	virtual void OnMouseDownR(pointix point) = 0;

	virtual void PreOnNcMouseUp(pointix point, MouseButtonE eButton) = 0;
	virtual void OnNcMouseUp(pointix point, MouseButtonE eButton) = 0;
	virtual void PreOnMouseUp(pointix point, MouseButtonE eButton) = 0;
	virtual void OnMouseUp(pointix point, MouseButtonE eButton) = 0;
	virtual void OnMouseUpL(pointix point) = 0;
	virtual void OnMouseUpM(pointix point) = 0;
	virtual void OnMouseUpR(pointix point) = 0;

	virtual void PreOnNcMouseClick(pointix point, MouseButtonE eButton) = 0;
	virtual void OnNcMouseClick(pointix point, MouseButtonE eButton) = 0;
	virtual void PreOnMouseClick(pointix point, MouseButtonE eButton) = 0;
	virtual void OnMouseClickL(pointix point) = 0;
	virtual void OnMouseClickM(pointix point) = 0;
	virtual void OnMouseClickR(pointix point) = 0;

	virtual void PreOnNcMouseDBClick(pointix point, MouseButtonE eButton) = 0;
	virtual void OnNcMouseDBClick(pointix point, MouseButtonE eButton) = 0;
	virtual void PreOnMouseDBClick(pointix point, MouseButtonE eButton) = 0;
	virtual void OnMouseDBClick(pointix point, MouseButtonE eButton) = 0;
	virtual void OnMouseDBClickL(pointix point) = 0;

	virtual void PreOnNcMouseWheel(pointix point, int_x iWhell, AlignE eDirection) = 0;
	virtual void OnNcMouseWheel(pointix point, int_x iWhell, AlignE eDirection) = 0;
	virtual void PreOnMouseWheel(pointix point, int_x iWhell, AlignE eDirection) = 0;
	virtual void OnMouseWheel(pointix point, int_x iWhell, AlignE eDirection) = 0;
	virtual void OnMouseWheelX(pointix point, int_x iWhell) = 0;
	virtual void OnMouseWheelY(pointix point, int_x iWhell) = 0;

	virtual pointix GetMousePosition() const = 0;

	virtual bool IsMouseIn() const = 0;
	virtual bool IsMouseNcIn() const = 0;
	virtual bool IsMouseDownL() const = 0;
	virtual bool IsMouseDownM() const = 0;
	virtual bool IsMouseDownR() const = 0;
	virtual bool IsAnyButtonDown() const = 0;
	virtual bool IsAnyButtonNcDown() const = 0;
	virtual bool IsMouseDown(MouseButtonE eButton) const = 0;
	virtual bool IsPreDragIn() const = 0;
	virtual bool IsDragIn() const = 0;
	virtual bool IsKeyDown(KeyCodeE keyCode) const = 0;

	virtual void OnLoaded() = 0;
	// 属性改变。
	virtual void OnVisibleChanged() = 0;
	virtual void OnEnableChanged() = 0;
	virtual void OnCapturedChanged() = 0;
	virtual void OnSelectedChanged() = 0;
	virtual void OnFocusedChanged() = 0;
	virtual void OnActiveChanged() = 0;
	virtual void OnTextChanged() = 0;
	virtual void OnFontChanged() = 0;
	virtual void OnForeColorChanged() = 0;
	virtual void OnBackColorChanged() = 0;
	virtual void OnPositionChanged() = 0;
	virtual void OnSizeChanged() = 0;
	virtual void OnCursorChanged() = 0;

	// 键盘交互
	virtual void PreOnKeyDown(KeyCodeE eKeyCode) = 0;
	virtual void OnKeyDown(KeyCodeE eKeyCode) = 0;
	virtual void OnKeyRepeat(KeyCodeE eKeyCode) = 0;
	virtual void PreOnKeyUp(KeyCodeE eKeyCode) = 0;
	virtual void OnKeyUp(KeyCodeE eKeyCode) = 0;
	virtual void PreOnKeyInput(char_32 chInput) = 0;
	virtual void OnKeyInput(char_32 chInput) = 0;

	virtual void AddControl(IControl * pControl) = 0;
	virtual void RmvControl(IControl * pControl) = 0;
	virtual IControlVector & GetChildren() = 0;
	virtual const IControlVector & GetChildren() const = 0;

	virtual void SetLayoutMode(LayoutModeE eType) = 0;
	virtual LayoutModeE GetLayoutMode() const = 0;
	virtual void OnVisualSizeChanged() = 0;

	virtual void BegLayout() = 0;
	virtual void EndLayout() = 0;
	virtual void Layout() = 0;
	
	virtual IControl * NcFindChild(pointix point) const = 0;
	virtual IControl * FindChild(pointix point) const = 0;
	virtual IControl * FindChild(const char_16 * szName, int_x iDepth = 0) const = 0;

	//------------------- 鼠标正下方的控件
	virtual void SetMouseControl(IControl * pControl, pointix point) = 0;
	virtual IControl * GetMouseControl() const  = 0;

	//------------------- 捕获鼠标的控件
	virtual void SetCaptureControl(IControl * pControl) = 0;
	virtual IControl * GetCaptureControl() const = 0;

	//------------------- 选中的控件
	virtual bool SelectNext(bool bWrap) = 0;
	virtual bool SelectLast(bool bWrap) = 0;
	virtual void SetSelectedControl(IControl * pControl) = 0;
	virtual IControl * GetSelectedControl() const = 0;
	virtual IControl * GetFocusControl() const = 0;

	virtual void OnChildGetFocus(IControl * pControl) = 0;
	virtual void OnChildLostFocus(IControl * pControl) = 0;

	virtual void SetMouseWheelMode(MouseWheelModeE eMouseWheelMode) = 0;
	virtual MouseWheelModeE GetMouseWheelMode() const = 0;

	virtual void OnCheckGroup(int_x iGroup) = 0;

	// 拖拽
	virtual DropResultE PreOnNcDragEnter(IData * pData, pointix point) = 0;
	virtual DropResultE PreOnNcDragMove(IData * pData, pointix point) = 0;
	virtual void PreOnNcDragLeave(IData * pData, pointix point) = 0;
	virtual DropResultE PreOnNcDragDrop(IData * pData, pointix point) = 0;

	virtual DropResultE OnDragEnter(IData * pData, pointix point) = 0;
	virtual DropResultE OnDragMove(IData * pData, pointix point) = 0;
	virtual void OnDragLeave(IData * pData, pointix point) = 0;
	virtual DropResultE OnDragDrop(IData * pData, pointix point) = 0;

	virtual bool SetDropControl(IControl * pControl) = 0;
	virtual IControl * GetDropControl() const = 0;

	// debug
	virtual void OutputInfo(int_x iDepth) const = 0;
	virtual void QueryDebugMenu(int_x & iBase, IMenu * pMenu) const = 0;
	virtual void OnDebugMenu(int_x iBase, int_x iResult) = 0;

	virtual bool IsScrollXAble() const = 0;
	virtual bool IsScrollYAble() const = 0;
	virtual void SetAutoHideScrollBarX(bool bAutoHide) = 0;
	virtual void SetAutoHideScrollBarY(bool bAutoHide) = 0;
	virtual bool IsAutoHideScrollBarX() const = 0;
	virtual bool IsAutoHideScrollBarY() const = 0;
	virtual void SetScrollPosX(int_x iX) = 0;
	virtual void SetScrollPosY(int_x iY) = 0;
	virtual void SetScrollPos(int_x iX, int_x iY) = 0;
	virtual int_x GetScrollPosX() const = 0;
	virtual int_x GetScrollPosY() const = 0;
	virtual pointix GetScrollPos() const = 0;
	virtual IControl * GetScrollBarX() const = 0;
	virtual IControl * GetScrollBarY() const = 0;
	virtual void PreOnScroll(IControl * pScroll, int_x iOffset) = 0;
	virtual void OnScroll(intx2 scroll, intx2 offset) = 0;
	virtual void UpdateScroll() = 0;
	virtual void SetScroll(intx2 i2Scroll) = 0;
	virtual void SetScroll(int_x icxScroll, int_x icyScroll) = 0;

	virtual void DoAttributeExchange(IAttributeServer * pServer, bool bPush) = 0;
};

class V2D_API IForm
{
public:
	virtual ~IForm() {}

	virtual void SetOwner(IForm * pForm) = 0;
	virtual IForm * GetOwner() const = 0;

	virtual void SetFormType(FormTypeE eHostType) = 0;
	virtual FormTypeE GetFormType() const = 0;
	virtual void CreateHost() = 0;
	virtual void AttachHost(int_x iFormId) = 0;
	virtual IHost * GetHost() const = 0;

	virtual void SetResizeBorder(edgeix border) = 0;
	virtual edgeix GetResizeBorder() const = 0;

	virtual int_x GetFormId() const = 0;

	virtual void Show(ShowModeE eShow = ShowModeNormal, HostInitPosE eInitPos = HostInitPosDefault) = 0;
	virtual void Hide() = 0;

	virtual ShowModeE GetShowMode() const = 0;
	virtual bool IsMaxSize() const = 0;
	virtual bool IsMinSize() const = 0;

	virtual void OnRefresh(rectix rect) = 0;
	virtual void OnCreate() = 0;
	virtual void OnClose() = 0;
	virtual void OnDestroy() = 0;
	virtual void OnMaxSize() = 0;
	virtual void OnMinSize() = 0;
	virtual void OnRestore() = 0;

	virtual int_x RunDialog() = 0;
	virtual void EndRun(int_x iResult) = 0;

	virtual bool MouseActiving(pointix point) const = 0;
	virtual bool IsActived() const = 0;

	virtual void NotifyPositionChanged(const pointix & position) = 0;
	virtual void NotifySizeChanged(const sizeix & size, ShowModeE eShowMode) = 0;
	virtual void NotifyShownChanged(bool bShown) = 0;
	virtual void NotifyFocusChanged(bool bFocus) = 0;
	virtual void NotifyTextChanged(const char_16 * szText, int_x iLength = -1) = 0;
	virtual void NotifyActiveChanged(bool bActive) = 0;
	virtual void NotifyCaptureChanged(bool bCaptured) = 0;

	virtual void SetMinSize(const sizeix & size) = 0;
	virtual void SetMaxSize(const sizeix & size) = 0;
	virtual const sizeix & GetMinSize() const = 0;
	virtual const sizeix & GetMaxSize() const = 0;
};

class V2D_API IHost : public IObject
{
public:
	virtual ~IHost() {}

	virtual int_x CreateHost(IControl * pControl, FormTypeE eHostType) = 0;
	virtual int_x GetFormId() const = 0;

	virtual bool Attach(int_x iFormId) = 0;
	virtual void Detach() = 0;

	virtual void ShowHost(ShowModeE eShow, HostInitPosE eInitPos) = 0;

	virtual void Update(rectix rect) = 0;
	virtual void Refresh(rectix rect) = 0;

	virtual pointix GetMousePosition() const = 0;
	virtual bool IsKeyDown(KeyCodeE ekeyCode) const = 0;

	virtual void SetPosition(const pointix & position) = 0;
	virtual void SetSize(const sizeix & size) = 0;
	virtual void Active(bool bActive) = 0;
	virtual void SetText(const char_16 * szText, int_x iLength = -1) = 0;
	virtual void SetBoxes(bool bMinBox, bool bMaxBox, bool bCloseBox) = 0;
	virtual bool Capture(bool bCapture) = 0;

	virtual pointix ClientToScreen(const pointix & point) const = 0;

	virtual void UpdateIme() = 0;
	virtual bool ClipboardCheckData(ClipboardDataTypeE eDataType) const = 0;
	virtual textw ClipboardGetText16() const = 0;
	virtual void ClipboardSetText16(const char_16 * szText, int_x iLength = -1) const = 0;
};

class V2D_API IGroup
{
public:
	virtual ~IGroup() {}

	virtual IControl * GetGroupControl() = 0;

	virtual void SetGroupChecked(bool bChecked) = 0;
	virtual bool IsGroupChecked() const = 0;

	virtual void SetGroupIndex(int_x iGroupIndex) = 0;
	virtual int_x GetGroupIndex() const = 0;
};

class V2D_API IPopupHost
{
public:
	virtual ~IPopupHost() {}
	virtual void SetResult(int_x iId) = 0;
};

class V2D_API IRange
{
public:
	virtual ~IRange() {}

	virtual void SetMinValue(int_x iMin) = 0;
	virtual int_x GetMinValue() const = 0;
	virtual int_x GetRangeValue() const = 0;
	virtual void SetMaxValue(int_x iMax) = 0;
	virtual int_x GetMaxValue() const = 0;
	virtual void SetValue(int_x iValue) = 0;
	virtual int_x GetValue() const = 0;
	virtual void SetRangeInfo(int_x iMin, int_x iMax, int_x iValue) = 0;
	virtual void SetRangeInfo(const RangeInfoT & range) = 0;
	virtual void GetRangeInfo(int_x * piMin, int_x * piMax, int_x * piValue) const = 0;
	virtual float_32 GetRate() const = 0;

	virtual void MinValueChanged() = 0;
	virtual void MaxValueChanged() = 0;
	virtual void ValueChanged(int_x iOffset) = 0;
};

class V2D_API IScroll : public IRange
{
public:
	virtual ~IScroll() {}

	virtual void SetDirection(AxisE eDirection) = 0;
	virtual AxisE GetDirection() const = 0;
	virtual void SetLineValue(int_x iLine) = 0;
	virtual int_x GetLineValue() const = 0;
	virtual void SetPageValue(int_x iPage) = 0;
	virtual int_x GetPageValue() const = 0;
	virtual void SetScrollInfo(int_x iMin, int_x iMax, int_x iValue, int_x iLine, int_x iPage) = 0;
	virtual void SetScrollInfo(const ScrollInfoT & scroll) = 0;
	virtual ScrollInfoT GetScrollInfo() const = 0;
	virtual void LineUp() = 0;
	virtual void PageUp() = 0;
	virtual void LineDown() = 0;
	virtual void PageDown() = 0;

	virtual void LineValueChanged() = 0;
	virtual void PageValueChanged() = 0;

	virtual void SetScrollTarget(IControl * pScrollTarget) = 0;
	virtual IControl * GetScrollTarget() const = 0;
};

/*
 * 支持属性交换，用于存档或者初始化控件状态。
 * Push 将属性移交给 Server 处理。
 * Pull 从 Server 里读取出属性。
 */
class V2D_API IAttributeServer
{
public:
	virtual bool Push(int_x iValue, const char_16 * szName) = 0;
	virtual bool Push(rectix rect, const char_16 * szName) = 0;
	virtual bool Push(bool bValue, const char_16 * szName) = 0;
	virtual bool Push(const char_16 * szValue, int_x iLength, const char_16 * szName) = 0;

	virtual bool Pull(int_x & iValue, const char_16 * szName) = 0;
	virtual bool Pull(rectix & rect, const char_16 * szName) = 0;
	virtual bool Pull(bool & bValue, const char_16 * szName) = 0;
	virtual bool Pull(char_16 * szValue, int_x iSize, const char_16 * szName) = 0;
};

class V2D_API IUITheme
{
public:
	virtual ~IUITheme() {}
};

class V2D_API IMonitor
{
public:
	virtual int_x GetWidth() const = 0;
	virtual int_x GetHeight() const = 0;
	virtual sizeix GetSize() const = 0;
	virtual rectix GetRect() const = 0;
	virtual rectix GetWorkRect() const = 0;
};

class V2D_API IMonitorManager : public IService
{
public:
	virtual IMonitor * GetMonitorFromWindowId(int_x iId) const = 0;
	virtual IMonitor * GetMonitor(int_x iIndex) const = 0;
	virtual IMonitor * GetMonitor(int_x iX, int_x iY) const = 0;

public:
	static IMonitorManager * GetInstance();
};
const cid_t OID_IMonitorManager = {L"IMonitorManager"};

class V2D_API IUILoader : public IService
{
public:
	virtual ~IUILoader() {}
	virtual void Load(const char_x * szFile, IControl * pControl) = 0;
	virtual void Load(const byte_t * pData, int_x iSize, IControl * pControl) = 0;
	virtual void Load(const xml_node & node, IControl * pControl) = 0;

public:
	static IUILoader * GetInstance();
};
const cid_t OID_IUILoader = {L"IUILoader"};

class V2D_API IViewItem : public IObject
{
public:
	virtual ~IViewItem() {}
};

VENUS_END
