#pragma once

#include "IControl.h"

VENUS_BEG


/**
 * @brief 所有控件的原始基类。
 * @details CControl 提供了对 IControl 最基本的实现，使得控件具有
 *  一些基本的功能和属性，如 位置、大小、边界、文本、字体、图像、图像的绘制模式、可见性、
 *  可交互性、是否已显示、快捷键等等。
 *  CControl 还提供一些事件功能，包括鼠标键盘事件和一些属性更改事件。
 *  另外，CControl 还维护了一个 CControl 的双向链表(从 IControl 继承的接口)，用于提供
 *  遍历功能给容器控件或者其他需要遍历控件的使用者。
 */
class V2D_API CControl : public ObjectT<IControl>
{
public:
	CControl();
	CControl(int_x iX, int_x iY, int_x iWidth, int_x iHeight);
	~CControl();

	const oid_t & GetOid() const override;
	void GetMeta(IMetaHelper * pHelper) override;

	void SetParent(IControl * pParent) override;

	IControl * GetParent() const override;
	IControl * GetAncestor() const override;
	IForm * GetForm() const override;

	IScroll * ToIScroll() const override;
	IForm * ToIForm() const override;

	// ----------------------- 几何信息
	void SetWidthMode(WHModeE eMode) override;
	WHModeE GetWidthMode() const override;
	void SetHeightMode(WHModeE eMode) override;
	WHModeE GetHeightMode() const override;

	rectix GetRect() const override;
	void SetRect(const rectix & rect) override;
	void SetRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight) override;

	void SetPosition(pointix position) override;
	void SetPosition(int_x iX, int_x iY) override;
	void SetSize(sizeix size) override;
	void SetSize(int_x iWidth, int_x iHeight) override;
	pointix GetPosition() const override;
	sizeix GetSize() const override;

	void SetX(int_x iX) override;
	void SetY(int_x iY) override;
	void SetWidth(int_x iWidth) override;
	void SetHeight(int_x iHeight) override;

	pointix GetRelativePosition(IControl * pAncestor) const override;
	sizeix GetContentSize() const override;
	sizeix GetPreferedSize() const override;

	int_x GetX() const override;
	int_x GetY() const override;
	int_x GetWidth() const override;
	int_x GetHeight() const override;

	int_x GetRight() const override;
	int_x GetBottom() const override;

	void SetMargin(edgeix margin) override;
	edgeix GetMargin() const override;
	void SetPadding(const edgeix & padding) override;
	edgeix GetPadding() const override;

	void SetWeight(float_32 fWeight) override;
	float_32 GetWeight() const override;

	edgeix GetBorder() const override;
	// Visual 区域，去除边界后的可见区域矩形。(border.Left, border.Right, Width - border.Width, Height - border.Height)
	rectix GetVisual() const override;
	// Client 区域，客户区区域，这是去除边界、滚动条的区域。
	rectix GetClient() const override;
	// Offset 大小，通常是指客户区子控件的整体偏移。
	pointix GetOffset() const override;

	void SetText(const char_16 * szText, int_x iLength = -1) override;
	void SetText(textw text) override;
	const textw & GetText() const override;

	void SetName(const textw & name) override;
	const textw & GetName() const override;

	void SetId(int_x iID) override;
	int_x GetId() const override;

	void SetZOrder(int_x iZOrder) override;
	int_x GetZorder() const override;

	void SetDynamic(bool bDynamic) override;
	bool IsDynamic() const override;

	void SetEnable(bool bEnable) override;
	bool IsEnabled() const override;
	bool IsAviliable() const override;

	void SetVisible(bool bVisible) override;
	bool IsVisible() const override;

	bool IsShown() const override;
	bool IsHidden() const override;
	void SetShown(bool bShown) override;

	void OnShow() override;
	void OnHide() override;

	void SetNcControl(bool bNcControl) override;
	bool IsNcControl() const override;

	void OnAdded(IControl * pContainer) override;
	void OnRemoved(IControl * pContainer) override;

	void Capture(bool bCapture) override;
	void SetCaptured(bool bCaptured) override;
	bool IsCaptured() const override;

	void SetImeMode(ImeModeE eImeMode) override;
	ImeModeE GetImeMode() const override;
	void UpdateIme() override;
	bool GetImeInfo(ImeInfoT & imeInfo) const override;

	bool ClipboardCheckData(ClipboardDataTypeE eDataType) const override;
	textw ClipboardGetText16() const override;
	void ClipboardSetText16(const char_16 * szText, int_x iLength = -1) const override;

	void SetAcceptDrop(bool bAcceptDrop) override;
	bool IsAcceptDrop() const override;
	bool CanAcceptDrop() const override;

	void SetFont(const font_t & font) override;
	void SetFontName(const char_16 * szFontName) override;
	void SetFontSize(int_x iFontSize) override;
	void SetFontRenderLevel(FontRenderlevelE eRenderLevel) override;
	void SetFontFlag(FontFlagE eFontFlag, bool bSet);
	const font_t & GetFont() const override;

	int_x GetFontSize() const;

	void SetBackColor(uint_32 uiBackColor) override;
	void SetForeColor(uint_32 uiForeColor) override;
	uint_32 GetBackColor() const override;
	uint_32 GetForeColor() const override;

	// 控件光标。
	void SetCursor(CursorE eCursor) override;
	CursorE GetCursor() const override;
	void NcRequestCursor(pointix point, CursorE & eCursor) const override;
	void RequestCursor(pointix point, CursorE & eCursor) const override;

	// 是否可以接收焦点。
	void SetFocusAble(bool bFocusAble) override;
	bool IsFocusAble() const override;
	// 能否被选中。
	void SetSelectAble(bool bSelectAble) override;
	bool IsSelectAble() const override;

	// 当前是否可以接收焦点。
	bool CanSelect() const override;
	// 当前能否被选中。
	bool CanFocus() const override;

	// 尝试让控件获得焦点。
	bool Focus(bool bFocus) override;
	// 控件是否已经获得了焦点。
	void SetFocused(bool bFocused) override;
	bool IsFocused() const override;

	// 选择控件，尝试让控件获得焦点。
	void Select(bool bSelect) override;
	// 控件是否已经被选择了。
	void SetSelected(bool bSelected) override;
	bool IsSelected() const override;

	// 选择Tab顺序中的下一个控件。
	bool SelectNext(bool bWrap) override;
	// 选择Tab顺序中的上一个控件。
	bool SelectLast(bool bWrap) override;

	void SetStatic(bool bStatic) override;
	bool IsStatic() const override;
	void SetTransparent(bool bTransparent) override;
	bool IsTransparent() const override;
	void SetClipChild(bool bClipChild) override;
	bool IsClipChild() const override;

	// 是否接受 Enter 和 Tab 字符信息(如果接受字符信息，Tab 不会选择上一个/下一个)。
	void SetAcceptEnter(bool bAcceptEnter) override;
	void SetAcceptTab(bool bAcceptTab) override;
	bool IsAcceptEnter() const override;
	bool CanAcceptEnter() const override;
	bool IsAcceptTab() const override;
	bool CanAcceptTab() const override;
	void SetAcceptScroll(bool bAcceptScroll) override;
	bool IsAcceptScroll() const override;
	bool CanAcceptScroll() const override;

	err_t SetParam(const char_16 * szName, const char_16 * szValue) override;

	// 控件的锚定规则(上下左右)。
	void SetAnchor(anchor_t anchor) override;
	anchor_t GetAnchor() const override;
	// 根据锚定参数，重置控件位置/大小。
	void Anchor() override;

	// 初始化布局参数。
	void InitLayout();

	// ------------- 坐标转换
	//! parent -> none client
	pointix ToLocal(const pointix & point) const override;
	//! client -> parent client
	pointix ToGlobal(const pointix & point) const override;

	//! none client -> client
	pointix ToClient(const pointix & point) const override;
	//! client -> none client
	pointix ToNoneCl(const pointix & point) const override;
	//! client -> screen
	pointix ToScreen(const pointix & point) const override;

	void CenterToParent();
	void ShowTips(pointix point, const char_16 * szTips) override;
	void HideTips() override;
	void UpdateTips(pointix point) override;

	int_x PopupMenu(pointix point, IMenu * pMenu) override;
	int_x PopupControl(pointix point, IControl * pControl) override;
public:
	void OnLoaded() override;
	void OnVisibleChanged() override;
	void OnEnableChanged() override;
	void OnFocusedChanged() override;
	void OnActiveChanged() override;
	void OnSelectedChanged() override;
	void OnCapturedChanged() override;
	void OnTextChanged() override;
	void OnFontChanged() override;
	void OnForeColorChanged() override;
	void OnBackColorChanged() override;
	void OnPositionChanged() override;
	void OnSizeChanged() override;
	void OnVisualSizeChanged() override;
	void OnCursorChanged() override;

	void SetAutoRepaint(uint_32 uiAutoRepaint) override;
	uint_32 GetAutoRepaint() const override;

	/**
	 * @brief 重绘整个控件，更新控件所占用的视图区域(调用Update())。
	 */
	void Repaint() override;
	/**
	 * @brief 重绘指定区域控件，更新所占用的视图区域(调用Update())。
	 * @param rect 指定的区域。
	 */
	void Repaint(const rectix & rect) override;
	
	/**
	 * @brief 更新控件所占用的视图区域，Update 不要求控件本身重绘。
	 */
	void NcRepaint() override;
	/**
	 * @brief 更新控件指定的视图区域，Update 不要求控件本身重绘。
	 * @param rect 指定的矩形区域。
	 */
	void NcRepaint(const rectix & rect) override;

	void PreOnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) override;
	void OnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;
	void PreOnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;
	void AftOnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;

	void SetHotKey(KeyCodeE eHotKey) override;
	KeyCodeE GetHotKey() const override;
	bool OnHotKey() override;

	void SetPopuping(bool bPopuping) override;
	bool IsPopuping() const override;

	void SetBorderType(BorderTypeE eBorderType) override;
	BorderTypeE GetBorderType() const override;

	/**
	 * @brief 使用控件坐标。   
	 */
	HitTestE NcHitTest(pointix point) const override;
	HitTestE HitTest(pointix point) const override;
	HitTestE HitTestDeep(pointix point) const override;
public:
	void NcUpdateMouse(pointix point) override;
	// 鼠标交互
	void PreOnNcMouseMove(pointix point) override;
	void OnNcMouseMove(pointix point) override;
	void PreOnMouseMove(pointix point) override;
	void OnMouseMove(pointix point) override;

	void PreOnNcMouseIn(pointix point) override;
	void OnNcMouseIn(pointix point) override;
	void PreOnMouseIn(pointix point) override;
	void OnMouseIn(pointix point) override;

	void PreOnNcMouseOut(pointix point) override;
	void OnNcMouseOut(pointix point) override;
	void PreOnMouseOut(pointix point) override;
	void OnMouseOut(pointix point) override;

	void PreOnNcMouseDown(pointix point, MouseButtonE eButton) override;
	void OnNcMouseDown(pointix point, MouseButtonE eButton) override;
	void PreOnMouseDown(pointix point, MouseButtonE eButton) override;
	void OnMouseDown(pointix point, MouseButtonE eButton) override;
	void OnMouseDownL(pointix point) override;
	void OnMouseDownM(pointix point) override;
	void OnMouseDownR(pointix point) override;

	void PreOnNcMouseUp(pointix point, MouseButtonE eButton) override;
	void OnNcMouseUp(pointix point, MouseButtonE eButton) override;
	void PreOnMouseUp(pointix point, MouseButtonE eButton) override;
	void OnMouseUp(pointix point, MouseButtonE eButton) override;
	void OnMouseUpL(pointix point) override;
	void OnMouseUpM(pointix point) override;
	void OnMouseUpR(pointix point) override;

	void PreOnNcMouseClick(pointix point, MouseButtonE eButton) override;
	void OnNcMouseClick(pointix point, MouseButtonE eButton) override;
	void PreOnMouseClick(pointix point, MouseButtonE eButton) override;
	void OnMouseClickL(pointix point) override;
	void OnMouseClickM(pointix point) override;
	void OnMouseClickR(pointix point) override;

	void PreOnNcMouseDBClick(pointix point, MouseButtonE eButton) override;
	void OnNcMouseDBClick(pointix point, MouseButtonE eButton) override;
	void PreOnMouseDBClick(pointix point, MouseButtonE eButton) override;
	void OnMouseDBClick(pointix point, MouseButtonE eButton) override;
	void OnMouseDBClickL(pointix point) override;

	void PreOnNcMouseWheel(pointix point, int_x iWhell, AlignE eDirection) override;
	void OnNcMouseWheel(pointix point, int_x iWhell, AlignE eDirection) override;
	void PreOnMouseWheel(pointix point, int_x iWhell, AlignE eDirection) override;
	void OnMouseWheel(pointix point, int_x iWhell, AlignE eDirection) override;
	void OnMouseWheelX(pointix point, int_x iWhell) override;
	void OnMouseWheelY(pointix point, int_x iWhell) override;

	pointix GetMousePosition() const override;

	bool IsMouseIn() const override { return m_bMouseIn;}
	bool IsMouseNcIn() const override { return m_bMouseNcIn;}
	bool IsMouseDownL() const override { return m_bMouseDownL;}
	bool IsMouseDownM() const override { return m_bMouseDownM;}
	bool IsMouseDownR() const override { return m_bMouseDownR;}
	bool IsAnyButtonDown() const override
	{ return m_bMouseDownL | m_bMouseDownM | m_bMouseDownR;}
	bool IsAnyButtonNcDown() const override
	{ return m_bMouseNcDownL | m_bMouseNcDownM | m_bMouseNcDownR;}

	bool IsMouseDown(MouseButtonE eButton) const override;
	bool IsPreDragIn() const override;
	bool IsDragIn() const override;
	// 键盘交互
	void PreOnKeyDown(KeyCodeE eKeyCode) override;
	void OnKeyDown(KeyCodeE eKeyCode) override;

	void OnKeyRepeat(KeyCodeE eKeyCode) override;

	void PreOnKeyUp(KeyCodeE eKeyCode) override;
	void OnKeyUp(KeyCodeE eKeyCode) override;

	void PreOnKeyInput(char_32 chInput) override;
	void OnKeyInput(char_32 chInput) override;

	bool IsKeyDown(KeyCodeE ekeyCode) const override;

	bool IsScrollXAble() const override;
	bool IsScrollYAble() const override;
	void SetAutoHideScrollBarX(bool bAutoHide) override;
	void SetAutoHideScrollBarY(bool bAutoHide) override;
	bool IsAutoHideScrollBarX() const override;
	bool IsAutoHideScrollBarY() const override;
	void SetScrollPosX(int_x iX) override;
	void SetScrollPosY(int_x iY) override;
	void SetScrollPos(int_x iX, int_x iY) override;
	int_x GetScrollPosX() const override;
	int_x GetScrollPosY() const override;
	pointix GetScrollPos() const override;

	void SetScrollBarX(IControl * pScrollBar);
	void SetScrollBarY(IControl * pScrollBar);
	IControl * GetScrollBarX() const override;
	IControl * GetScrollBarY() const override;
	void PreOnScroll(IControl * pScroll, int_x iOffset) override;
	void OnScroll(intx2 scroll, intx2 offset) override;
	void UpdateScroll() override;
	void SetScroll(intx2 i2Scroll) override;
	void SetScroll(int_x icxScroll, int_x icyScroll) override;
	intx2 GetScroll() const;
	void _LayoutScrollBars(bool bScrollX, bool bScrollY);

	void DoAttributeExchange(IAttributeServer * pServer, bool bPush) override;
protected:
	void ConfirmBuffer(int_x iWidth, int_x iHeight);
protected:
	//! 父控件指针。
	IControl * m_pParent;
	//! 控件名称
	textw m_name;
	//! 控件的ID。
	int_x m_iId;
	//! 控件的 Z 顺序
	int_x m_iZOrder;
	//! 控件宽度的模式
	WHModeE m_eWidthMode;
	//! 控件高度的模式
	WHModeE m_eHeightMode;

	//! 控件的范围。
	rectix m_rect;
	//! 控件外部的期望间距。
	edgeix m_margin;
	//! 控件内部的期望间距。
	edgeix m_padding;
	//! 控件所具有的边框类型
	BorderTypeE m_eBorderType;
	//! 控件布局的权重
	float_32 m_fWeight;

	//! 控件的文本（控件内部不再支持多字节文本）。
	textw m_text;
	//! 文本格式。
	font_t m_font;
	//! 快捷键
	KeyCodeE m_eHotKey;
	//! 背景色。
	uint_32 m_uiBackColor;
	//! 前景色。
	uint_32 m_uiForeColor;
	//! 图片的绘制模式。
	ImageModeE m_eImageMode;
	//! 鼠标悬停的光标。
	CursorE m_eCursor;
	//! 控件的绑定类型。
	anchor_t m_anchor;
	//! 控件被选中时的输入法状态。
	ImeModeE m_eImeMode;
	union
	{
		//! 重绘标记。
		uint_32 m_uiAutoRepaint;
		struct
		{
			//! 预处理鼠标进入、移出时是否自动重绘。
			bool m_bRepaintMousePreInOut : 1;
			//! 鼠标进入、移出时是否自动重绘。
			bool m_bRepaintMouseInOut : 1;
			//! 左键按下、松开时是否自动重绘。
			bool m_bRepaintMouseDownUpL : 1;
			//! 中键按下、松开时是否自动重绘。
			bool m_bRepaintMouseDownUpM : 1;
			//! 右键按下、松开时是否自动重绘。
			bool m_bRepaintMouseDownUpR : 1;

			//! 文本改变时是否重绘。
			bool m_bRepaintTextChanged : 1;	
			//! 文本改变时是否重绘。
			bool m_bRepaintFontChanged : 1;
			//! 选中改变时是否重绘。
			bool m_bRepaintSelectedChanged: 1;
			//! 焦点改变时是否重绘。
			bool m_bRepaintFocusedChanged: 1;
			//! 焦点改变时是否重绘。
			bool m_bRepaintActiveChanged: 1;
			//! 鼠标捕获状态改变时是否重绘。
			bool m_bRepaintCaptureChanged : 1;
			//! 可交互性改变时是否重绘。
			bool m_bRepaintEnabledChanged : 1;

			//! 鼠标拖拽进入、移出时是否自动重绘。
			bool m_bRepaintMouseDragInOut : 1;

			//! 水平滚动时是否自动重绘。
			bool m_bRepaintScrollX : 1;
			//! 垂直滚动时是否自动重绘。
			bool m_bRepaintScrollY : 1;
		};
	};

	union
	{
		//! 一些基本的属性。
		uint_32 m_uiBaseAttr;
		struct
		{
			//! 是否动态创建，动态创建需要 AddRef 和 Release。
			bool m_bDynamic : 1;
			//! 是否不可用。
			bool m_bDisabled : 1;
			//! 是否不可见。
			bool m_bInvisible : 1;
			//! 是否已经显示。
			bool m_bShown : 1;
			//! 是否禁用自动显示子控件，为 false 时，会在需要的时候自动调用 child 的 SetShown。
			//! 一般来说，该值都应该为 false，否则，应该手动调用 child 的 SetShown。
			//! 设为 true 时，通常子类需要重载 SetShown 来处理，此时，所有的 child 都由子类来管理 show 状态。
			bool m_bDisAutoShowChild : 1;
			//! 是否位于父控件的非客户区，通常，ScrollBar 位于非客户区。
			bool m_bNcControl;
			//! 绘图区域是否有效。
			bool m_bValidate : 1;
			//! 是否启用双缓冲绘图，这个参数需要控件自己决定如何使用。
			bool m_bBuffered : 1;
			//! 是否独占鼠标。
			bool m_bCaptured : 1;

			/**
			 * 选择和聚焦
			 * 选择是聚焦的前提。
			 * 选择仅仅是该控件被选择了而已，通常来说，控件都是可选择的，部分控件是不可选择的，如 Static。
			 * 聚焦是接受输入焦点，控件被选择了之后，可以接受输入焦点，然后控制输入法、处理输入的文字、处理按键消息等。
			 *   鼠标的按放、单击、双击消息不受焦点控制的，但滚轮消息在 MouseWheelModeFocus 模式下会按照焦点传递。
			 * 默认情况下，控件都是可选择的，但是不接受焦点。
			 */
			//! 是否禁用选择。
			bool m_bDisSelectAble : 1;
			//! 是否被选中。
			bool m_bSelected : 1;
			//! 是否接受输入焦点。
			bool m_bFocusAble : 1;
			//! 是否具有焦点。
			bool m_bFocused : 1;

			//! 是否接受拖放操作。
			bool m_bAcceptDrop : 1;
			//! 是否响应‘Tab’键。
			bool m_bAcceptTab : 1;
			//! 是否响应‘Enter’键。
			bool m_bAcceptEnter : 1;
			//! 是否拒绝滚轮消息
			bool m_bNotAcceptScroll : 1;
			//! 是否是静态控件。
			bool m_bStatic : 1;
			//! 是否是透明控件，透明控件不响应任何鼠标检测。
			bool m_bTransparent : 1;
			//! 是否裁剪子控件，只会影响 NcHitTest，将 HitTest 的结果向上层传递。
			bool m_bClipChild : 1;
			//! 是否正在进行弹出操作。
			bool m_bPopuping : 1;
			//! 是否接收弹出时的鼠标消息。
			bool m_bPupupResponse : 1;
			//! 是否对边界(edge)进行 hittest
			bool m_bHitTestEdge : 1;
		};
	};

	/// 鼠标状态
	union
	{
		//! 鼠标状态。
		uint_32 m_uiMouseState;
		struct
		{
			//! 是否悬停。
			bool m_bMouseIn : 1;
			//! 左键是否按下。
			bool m_bMouseDownL: 1;
			//! 中键是否按下。
			bool m_bMouseDownM: 1;
			//! 右键是否按下。
			bool m_bMouseDownR: 1;
			//! 是否悬停。
			bool m_bMouseNcIn : 1;
			//! 左键是否按下。
			bool m_bMouseNcDownL: 1;
			//! 中键是否按下。
			bool m_bMouseNcDownM: 1;
			//! 右键是否按下。
			bool m_bMouseNcDownR: 1;

			bool m_bMousePreDragIn : 1;
			bool m_bMouseDragIn : 1;
		};
	};

	IControl * m_pScrollX;
	IControl * m_pScrollY;

	union
	{
		//! 其他状态
		uint_32 m_uiState;
		struct
		{
			//! 总是显示水平滚动条(如果存在)。
			bool m_bAlwaysShowScrollX : 1;
			//! 总是显示垂直滚动条(如果存在)。
			bool m_bAlwaysShowScrollY : 1;
		};
	};
protected:
	IControlVector m_children;
	IControlVector m_DynamicChildren;
	LayoutModeE m_eLayoutMode;
	bool m_bLayouted;
	int_x m_iLayouting;

public:
	IControlVector & GetChildren() override { return m_children;}
	const IControlVector & GetChildren() const override { return m_children; }

public:
	void AddControl(IControl * pControl) override;
	void RmvControl(IControl * pControl) override;
	void RmvAllControl();
	bool HasControl(IControl * pControl) const;

	//--------------------------------- 自动排版
	void SetLayoutMode(LayoutModeE eType) override;
	LayoutModeE GetLayoutMode() const override;

	// 根据 child 的 widemode、heightmode、anchor 来确定控件的位置和（或）尺寸。
	void BegLayout() override;
	void EndLayout() override;
	void Layout() override;
	void _LayoutAbsolute();
	void _LayoutHorizontal();
	void _LayoutVertical();
	void _LayoutTable();

	IControl * NcFindChild(pointix point) const override;
	IControl * FindChild(pointix point) const override;
	IControl * FindChild(const char_16 * szName, int_x iDepth = 0) const override;
	/**
	 * @param point 本地坐标。
	 */
	void SetMouseControl(IControl * pControl, pointix point) override;
	IControl * GetMouseControl() const override;

	void SetCaptureControl(IControl * pControl) override;
	IControl * GetCaptureControl() const override;

	void SetSelectedControl(IControl * pControl) override;
	IControl * GetSelectedControl() const override;
	IControl * GetFocusControl() const override;

	void OnChildGetFocus(IControl * pControl) override;
	void OnChildLostFocus(IControl * pControl) override;

	void SetMouseWheelMode(MouseWheelModeE eMouseWheelMode) override;
	MouseWheelModeE GetMouseWheelMode() const override;

	void OnCheckGroup(int_x iGroup) override;

	DropResultE PreOnNcDragEnter(IData * pData, pointix point) override;
	DropResultE PreOnNcDragMove(IData * pData, pointix point) override;
	void PreOnNcDragLeave(IData * pData, pointix point) override;
	DropResultE PreOnNcDragDrop(IData * pData, pointix point) override;

	DropResultE OnDragEnter(IData * pData, pointix point) override;
	DropResultE OnDragMove(IData * pData, pointix point) override;
	void OnDragLeave(IData * pData, pointix point) override;
	DropResultE OnDragDrop(IData * pData, pointix point) override;

	bool SetDropControl(IControl * pControl) override;
	IControl * GetDropControl() const override;
	// debug
public:
	void OutputInfo(int_x iDepth) const override;
	void QueryDebugMenu(int_x & iBase, IMenu * pMenu) const override;
	void OnDebugMenu(int_x iBase, int_x iResult) override;
protected:
	IControl * m_pMouseControl;
	IControl * m_pSelectedControl;
	IControl * m_pDropControl;

	IControl * m_pCaptureControl;

	MouseWheelModeE m_eMouseWheelMode;

public:
	event<IControl *, bool> VisibleChanged;
	event<IControl *, bool> EnableChanged;
	event<IControl *, bool> SelectedChanged;
	event<IControl *, bool> FocusedChanged;
	event<IControl *, bool> CapturedChanged;

	event<IControl *> TextChanged;
	event<IControl *> FontChanged;
	event<IControl *, pointix> PositionChanged;
	event<IControl *, sizeix> SizeChanged;

	event<IControl *, pointix> MouseMove;
	event<IControl *, pointix> MouseIn;
	event<IControl *, pointix> MouseOut;
	event<IControl *, pointix> MouseDownL;
	event<IControl *, pointix> MouseDownM;
	event<IControl *, pointix> MouseDownR;
	event<IControl *, pointix> MouseUpL;
	event<IControl *, pointix> MouseUpM;
	event<IControl *, pointix> MouseUpR;
	event<IControl *, pointix> MouseClickL;
	event<IControl *, pointix> MouseClickM;
	event<IControl *, pointix> MouseClickR;
	event<IControl *, pointix> MouseDBClickL;
	event<IControl *, pointix, int_x> MouseWheelX;
	event<IControl *, pointix, int_x> MouseWheelY;

	event<IControl *, KeyCodeE> KeyDown;
	event<IControl *, KeyCodeE> KeyUp;

	event<IControl *, IData *, DropResultE *> DragEnter;
	event<IControl *, IData *, DropResultE *> DragMove;
	event<IControl *, IData *> DragDrop;
	event<IControl *, IData *> DragLeave;

public:
	static const int_x DEF_EDGE = 0;
	static const int_x SCROLL_DEF_SIZE = 14;
	static const int_x LINE_RATE = 10;
	static const int_x PAGE_RATE = 2;
	static const int_x DEF_SPACE = 3;
};

VENUS_END
