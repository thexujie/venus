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

	const oid_t & GetOid() const;
	void GetMeta(IMetaHelper * pHelper);

	void SetParent(IControl * pParent);

	IControl * GetParent() const;
	IControl * GetAncestor() const;
	IForm * GetForm() const;

	IScroll * ToIScroll() const;
	IForm * ToIForm() const;

	// ----------------------- 几何信息
	void SetWidthMode(WHModeE eMode);
	WHModeE GetWidthMode() const;
	void SetHeightMode(WHModeE eMode);
	WHModeE GetHeightMode() const;

	rectix GetRect() const;
	void SetRect(const rectix & rect);
	void SetRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight);

	void SetPosition(pointix position);
	void SetPosition(int_x iX, int_x iY);
	void SetSize(sizeix size);
	void SetSize(int_x iWidth, int_x iHeight);
	pointix GetPosition() const;
	sizeix GetSize() const;

	void SetX(int_x iX);
	void SetY(int_x iY);
	void SetWidth(int_x iWidth);
	void SetHeight(int_x iHeight);

	pointix GetRelativePosition(IControl * pAncestor) const;
	sizeix GetPreferedSize() const;
	sizeix GetContentSize() const;

	int_x GetX() const;
	int_x GetY() const;
	int_x GetWidth() const;
	int_x GetHeight() const;

	int_x GetRight() const;
	int_x GetBottom() const;

	void SetMargin(edgeix margin);
	edgeix GetMargin() const;
	void SetPadding(const edgeix & padding);
	edgeix GetPadding() const;

	void SetWeight(float_32 fWeight);
	float_32 GetWeight() const;

	edgeix GetBorder() const;
	// Visual 区域，去除边界后的可见区域矩形。(border.Left, border.Right, Width - border.Width, Height - border.Height)
	rectix GetVisual() const;
	// Client 区域，客户区区域，这是去除边界、滚动条的区域。
	rectix GetClient() const;
	// Offset 大小，通常是指客户区子控件的整体偏移。
	pointix GetOffset() const;

	void SetText(const char_16 * szText, int_x iLength = -1);
	void SetText(textw text);
	const textw & GetText() const;

	void SetName(const textw & name);
	const textw & GetName() const;

	void SetId(int_x iID);
	int_x GetId() const;

	void SetZOrder(int_x iZOrder);
	int_x GetZorder() const;

	void SetDynamic(bool bDynamic);
	bool IsDynamic() const;

	void SetEnable(bool bEnable);
	bool IsEnabled() const;
	bool IsAviliable() const;

	void SetVisible(bool bVisible);
	bool IsVisible() const;

	bool IsShown() const;
	bool IsHidden() const;
	void SetShown(bool bShown);

	void OnShow();
	void OnHide();

	void SetNcControl(bool bNcControl);
	bool IsNcControl() const;

	void OnAdded(IControl * pContainer);
	void OnRemoved(IControl * pContainer);

	void Capture(bool bCapture);
	void SetCaptured(bool bCaptured);
	bool IsCaptured() const;

	void SetImeMode(ImeModeE eImeMode);
	ImeModeE GetImeMode() const;
	void UpdateIme();
	bool GetImeInfo(ImeInfoT & imeInfo) const;

	bool ClipboardCheckData(ClipboardDataTypeE eDataType) const;
	textw ClipboardGetText16() const;
	void ClipboardSetText16(const char_16 * szText, int_x iLength = -1) const;

	void SetAcceptDrop(bool bAcceptDrop);
	bool IsAcceptDrop() const;
	bool CanAcceptDrop() const;

	void SetFont(const font_t & font);
	void SetFontName(const char_16 * szFontName);
	void SetFontSize(int_x iFontSize);
	void SetFontRenderLevel(FontRenderlevelE eRenderLevel);
	void SetFontFlag(FontFlagE eFontFlag, bool bSet);
	const font_t & GetFont() const;

	int_x GetFontSize() const;

	void SetBackColor(uint_32 uiBackColor);
	void SetForeColor(uint_32 uiForeColor);
	uint_32 GetBackColor() const;
	uint_32 GetForeColor() const;

	// 控件光标。
	void SetCursor(CursorE eCursor);
	CursorE GetCursor() const;
	void NcRequestCursor(pointix point, CursorE & eCursor) const;
	void RequestCursor(pointix point, CursorE & eCursor) const;

	// 是否可以接收焦点。
	void SetFocusAble(bool bFocusAble);
	bool IsFocusAble() const;
	// 能否被选中。
	void SetSelectAble(bool bSelectAble);
	bool IsSelectAble() const;

	// 当前是否可以接收焦点。
	bool CanSelect() const;
	// 当前能否被选中。
	bool CanFocus() const;

	// 尝试让控件获得焦点。
	bool Focus(bool bFocus);
	// 控件是否已经获得了焦点。
	void SetFocused(bool bFocused);
	bool IsFocused() const;

	// 选择控件，尝试让控件获得焦点。
	void Select(bool bSelect);
	// 控件是否已经被选择了。
	void SetSelected(bool bSelected);
	bool IsSelected() const;

	// 选择Tab顺序中的下一个控件。
	bool SelectNext(bool bWrap);
	// 选择Tab顺序中的上一个控件。
	bool SelectLast(bool bWrap);

	void SetStatic(bool bStatic);
	bool IsStatic() const;
	void SetTransparent(bool bTransparent);
	bool IsTransparent() const;
	void SetClipChild(bool bClipChild);
	bool IsClipChild() const;

	// 是否接受 Enter 和 Tab 字符信息(如果接受字符信息，Tab 不会选择上一个/下一个)。
	void SetAcceptEnter(bool bAcceptEnter);
	void SetAcceptTab(bool bAcceptTab);
	bool IsAcceptEnter() const;
	bool CanAcceptEnter() const;
	bool IsAcceptTab() const;
	bool CanAcceptTab() const;
	void SetAcceptScroll(bool bAcceptScroll);
	bool IsAcceptScroll() const;
	bool CanAcceptScroll() const;

	err_t SetParam(const char_16 * szName, const char_16 * szValue);

	// 控件的锚定规则(上下左右)。
	void SetAnchor(AlignE eAnchor);
	AlignE GetAnchor() const;
	// 锚定参数。
	void SetAnchorEdge(const edgeix & anchor);
	const edgeix & GetAnchorEdge() const;
	// 根据锚定参数，重置控件位置/大小。
	void Anchor();

	// 初始化布局参数。
	void InitLayout();

	// ------------- 坐标转换
	//! parent -> none client
	pointix ToLocal(const pointix & point) const;
	//! none client -> client
	pointix ToClient(const pointix & point) const;
	//! client -> none client
	pointix ToNoneClient(const pointix & point) const;
	//! client -> parent client
	pointix ClToParentCl(const pointix & point) const;
	//! parent client -> client
	pointix ParentClToCl(const pointix & point) const;

	pointix ClientToScreen(const pointix & point) const;
	pointix ToAncestor(const pointix & point) const;

	void CenterToParent();
	void ShowTips(pointix point, const char_16 * szTips);
	void HideTips();
	void UpdateTips(pointix point);

	int_x PopupMenu(pointix point, IMenu * pMenu);
	int_x PopupControl(pointix point, IControl * pControl);
public:
	void OnLoaded();
	void OnVisibleChanged();
	void OnEnableChanged();
	void OnFocusedChanged();
	void OnActiveChanged();
	void OnSelectedChanged();
	void OnCapturedChanged();
	void OnTextChanged();
	void OnFontChanged();
	void OnForeColorChanged();
	void OnBackColorChanged();
	void OnPositionChanged();
	void OnSizeChanged();
	void OnVisualSizeChanged();
	void OnCursorChanged();

	void SetAutoRepaint(uint_32 uiAutoRepaint);
	uint_32 GetAutoRepaint() const;

	/**
	 * @brief 重绘整个控件，更新控件所占用的视图区域(调用Update())。
	 */
	void Repaint();
	/**
	 * @brief 重绘指定区域控件，更新所占用的视图区域(调用Update())。
	 * @param rect 指定的区域。
	 */
	void Repaint(const rectix & rect);
	
	/**
	 * @brief 更新控件所占用的视图区域，Update 不要求控件本身重绘。
	 */
	void NcRepaint();
	/**
	 * @brief 更新控件指定的视图区域，Update 不要求控件本身重绘。
	 * @param rect 指定的矩形区域。
	 */
	void NcRepaint(const rectix & rect);

	void PreOnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme);
	void OnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;
	void PreOnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;
	void AftOnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;

	void SetHotKey(KeyCodeE eHotKey);
	KeyCodeE GetHotKey() const;
	bool OnHotKey();

	void SetPopuping(bool bPopuping);
	bool IsPopuping() const;

	void SetBorderType(BorderTypeE eBorderType);
	BorderTypeE GetBorderType() const;

	/**
	 * @brief 使用控件坐标。   
	 */
	HitTestE NcHitTest(pointix point) const;
	HitTestE HitTest(pointix point) const;
	HitTestE HitTestDeep(pointix point) const;
public:
	void NcUpdateMouse(pointix point);
	// 鼠标交互
	void PreOnNcMouseMove(pointix point);
	void OnNcMouseMove(pointix point);
	void PreOnMouseMove(pointix point);
	void OnMouseMove(pointix point);

	void PreOnNcMouseIn(pointix point);
	void OnNcMouseIn(pointix point);
	void PreOnMouseIn(pointix point);
	void OnMouseIn(pointix point);

	void PreOnNcMouseOut(pointix point);
	void OnNcMouseOut(pointix point);
	void PreOnMouseOut(pointix point);
	void OnMouseOut(pointix point);

	void PreOnNcMouseDown(pointix point, MouseButtonE eButton);
	void OnNcMouseDown(pointix point, MouseButtonE eButton);
	void PreOnMouseDown(pointix point, MouseButtonE eButton);
	void OnMouseDown(pointix point, MouseButtonE eButton);
	void OnMouseDownL(pointix point);
	void OnMouseDownM(pointix point);
	void OnMouseDownR(pointix point);

	void PreOnNcMouseUp(pointix point, MouseButtonE eButton);
	void OnNcMouseUp(pointix point, MouseButtonE eButton);
	void PreOnMouseUp(pointix point, MouseButtonE eButton);
	void OnMouseUp(pointix point, MouseButtonE eButton);
	void OnMouseUpL(pointix point);
	void OnMouseUpM(pointix point);
	void OnMouseUpR(pointix point);

	void PreOnNcMouseClick(pointix point, MouseButtonE eButton);
	void OnNcMouseClick(pointix point, MouseButtonE eButton);
	void PreOnMouseClick(pointix point, MouseButtonE eButton);
	void OnMouseClickL(pointix point);
	void OnMouseClickM(pointix point);
	void OnMouseClickR(pointix point);

	void PreOnNcMouseDBClick(pointix point, MouseButtonE eButton);
	void OnNcMouseDBClick(pointix point, MouseButtonE eButton);
	void PreOnMouseDBClick(pointix point, MouseButtonE eButton);
	void OnMouseDBClick(pointix point, MouseButtonE eButton);
	void OnMouseDBClickL(pointix point);

	void PreOnNcMouseWheel(pointix point, int_x iWhell, AlignE eDirection);
	void OnNcMouseWheel(pointix point, int_x iWhell, AlignE eDirection);
	void PreOnMouseWheel(pointix point, int_x iWhell, AlignE eDirection);
	void OnMouseWheel(pointix point, int_x iWhell, AlignE eDirection);
	void OnMouseWheelX(pointix point, int_x iWhell);
	void OnMouseWheelY(pointix point, int_x iWhell);

	pointix GetMousePosition() const;

	bool IsMouseIn() const { return m_bMouseIn;}
	bool IsMouseNcIn() const { return m_bMouseNcIn;}
	bool IsMouseDownL() const { return m_bMouseDownL;}
	bool IsMouseDownM() const { return m_bMouseDownM;}
	bool IsMouseDownR() const { return m_bMouseDownR;}
	bool IsAnyButtonDown() const 
	{ return m_bMouseDownL | m_bMouseDownM | m_bMouseDownR;}
	bool IsAnyButtonNcDown() const 
	{ return m_bMouseNcDownL | m_bMouseNcDownM | m_bMouseNcDownR;}

	bool IsMouseDown(MouseButtonE eButton) const;
	bool IsPreDragIn() const;
	bool IsDragIn() const;
	// 键盘交互
	void PreOnKeyDown(KeyCodeE eKeyCode);
	void OnKeyDown(KeyCodeE eKeyCode);

	void OnKeyRepeat(KeyCodeE eKeyCode);

	void PreOnKeyUp(KeyCodeE eKeyCode);
	void OnKeyUp(KeyCodeE eKeyCode);

	void PreOnKeyInput(char_32 chInput);
	void OnKeyInput(char_32 chInput);

	bool IsKeyDown(KeyCodeE ekeyCode) const;

	bool IsScrollXAble() const;
	bool IsScrollYAble() const;
	void SetAutoHideScrollBarX(bool bAutoHide);
	void SetAutoHideScrollBarY(bool bAutoHide);
	bool IsAutoHideScrollBarX() const;
	bool IsAutoHideScrollBarY() const;
	void SetScrollPosX(int_x iX);
	void SetScrollPosY(int_x iY);
	void SetScrollPos(int_x iX, int_x iY);
	int_x GetScrollPosX() const;
	int_x GetScrollPosY() const;
	pointix GetScrollPos() const;

	void SetScrollBarX(IControl * pScrollBar);
	void SetScrollBarY(IControl * pScrollBar);
	IControl * GetScrollBarX() const;
	IControl * GetScrollBarY() const;
	void PreOnScroll(IControl * pScroll);
	void OnScrollX(int_x iValue);
	void OnScrollY(int_x iValue);
	void UpdateScroll();
	void SetScroll(intx2 i2Scroll);
	void SetScroll(int_x icxScroll, int_x icyScroll);
	intx2 GetScroll() const;
	void _LayoutScrollBars(bool bScrollX, bool bScrollY);

	void DoAttributeExchange(IAttributeServer * pServer, bool bPush);
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
	//! 控件的锚定信息。
	edgeix m_anchor;
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
	AlignE m_eAnchor;
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
	IControlVector & GetChildren() { return m_children;}
	const IControlVector & GetChildren() const { return m_children; }

public:
	void AddControl(IControl * pControl);
	void RmvControl(IControl * pControl);
	void RmvAllControl();
	bool HasControl(IControl * pControl) const;

	//--------------------------------- 自动排版
	void SetLayoutMode(LayoutModeE eType);
	LayoutModeE GetLayoutMode() const;

	// 根据 child 的 widemode、heightmode、anchor 来确定控件的位置和（或）尺寸。
	void BegLayout();
	void EndLayout();
	void Layout();
	void _LayoutAbsolute();
	void _LayoutHorizontal();
	void _LayoutVertical();
	void _LayoutTable();

	IControl * NcFindChild(pointix point) const;
	IControl * FindChild(pointix point) const;
	IControl * FindChild(const char_16 * szName, int_x iDepth = 0) const;
	/**
	 * @param point 本地坐标。
	 */
	void SetMouseControl(IControl * pControl, pointix point);
	IControl * GetMouseControl() const ;

	void SetCaptureControl(IControl * pControl);
	IControl * GetCaptureControl() const;

	void SetSelectedControl(IControl * pControl);
	IControl * GetSelectedControl() const;
	IControl * GetFocusControl() const;

	void OnChildGetFocus(IControl * pControl);
	void OnChildLostFocus(IControl * pControl);

	void SetMouseWheelMode(MouseWheelModeE eMouseWheelMode);
	MouseWheelModeE GetMouseWheelMode() const;

	void OnCheckGroup(int_x iGroup);

	DropResultE PreOnNcDragEnter(IData * pData, pointix point);
	DropResultE PreOnNcDragMove(IData * pData, pointix point);
	void PreOnNcDragLeave(IData * pData, pointix point);
	DropResultE PreOnNcDragDrop(IData * pData, pointix point);

	DropResultE OnDragEnter(IData * pData, pointix point);
	DropResultE OnDragMove(IData * pData, pointix point);
	void OnDragLeave(IData * pData, pointix point);
	DropResultE OnDragDrop(IData * pData, pointix point);

	bool SetDropControl(IControl * pControl);
	IControl * GetDropControl() const;
	// debug
public:
	void OutputInfo(int_x iDepth) const;
	void QueryDebugMenu(int_x & iBase, IMenu * pMenu) const;
	void OnDebugMenu(int_x iBase, int_x iResult);
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
