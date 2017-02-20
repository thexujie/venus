#pragma once

#include "IControl.h"

VENUS_BEG


/**
 * @brief ���пؼ���ԭʼ���ࡣ
 * @details CControl �ṩ�˶� IControl �������ʵ�֣�ʹ�ÿؼ�����
 *  һЩ�����Ĺ��ܺ����ԣ��� λ�á���С���߽硢�ı������塢ͼ��ͼ��Ļ���ģʽ���ɼ��ԡ�
 *  �ɽ����ԡ��Ƿ�����ʾ����ݼ��ȵȡ�
 *  CControl ���ṩһЩ�¼����ܣ������������¼���һЩ���Ը����¼���
 *  ���⣬CControl ��ά����һ�� CControl ��˫������(�� IControl �̳еĽӿ�)�������ṩ
 *  �������ܸ������ؼ�����������Ҫ�����ؼ���ʹ���ߡ�
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

	// ----------------------- ������Ϣ
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
	// Visual ����ȥ���߽��Ŀɼ�������Ρ�(border.Left, border.Right, Width - border.Width, Height - border.Height)
	rectix GetVisual() const override;
	// Client ���򣬿ͻ�����������ȥ���߽硢������������
	rectix GetClient() const override;
	// Offset ��С��ͨ����ָ�ͻ����ӿؼ�������ƫ�ơ�
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

	// �ؼ���ꡣ
	void SetCursor(CursorE eCursor) override;
	CursorE GetCursor() const override;
	void NcRequestCursor(pointix point, CursorE & eCursor) const override;
	void RequestCursor(pointix point, CursorE & eCursor) const override;

	// �Ƿ���Խ��ս��㡣
	void SetFocusAble(bool bFocusAble) override;
	bool IsFocusAble() const override;
	// �ܷ�ѡ�С�
	void SetSelectAble(bool bSelectAble) override;
	bool IsSelectAble() const override;

	// ��ǰ�Ƿ���Խ��ս��㡣
	bool CanSelect() const override;
	// ��ǰ�ܷ�ѡ�С�
	bool CanFocus() const override;

	// �����ÿؼ���ý��㡣
	bool Focus(bool bFocus) override;
	// �ؼ��Ƿ��Ѿ�����˽��㡣
	void SetFocused(bool bFocused) override;
	bool IsFocused() const override;

	// ѡ��ؼ��������ÿؼ���ý��㡣
	void Select(bool bSelect) override;
	// �ؼ��Ƿ��Ѿ���ѡ���ˡ�
	void SetSelected(bool bSelected) override;
	bool IsSelected() const override;

	// ѡ��Tab˳���е���һ���ؼ���
	bool SelectNext(bool bWrap) override;
	// ѡ��Tab˳���е���һ���ؼ���
	bool SelectLast(bool bWrap) override;

	void SetStatic(bool bStatic) override;
	bool IsStatic() const override;
	void SetTransparent(bool bTransparent) override;
	bool IsTransparent() const override;
	void SetClipChild(bool bClipChild) override;
	bool IsClipChild() const override;

	// �Ƿ���� Enter �� Tab �ַ���Ϣ(��������ַ���Ϣ��Tab ����ѡ����һ��/��һ��)��
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

	// �ؼ���ê������(��������)��
	void SetAnchor(anchor_t anchor) override;
	anchor_t GetAnchor() const override;
	// ����ê�����������ÿؼ�λ��/��С��
	void Anchor() override;

	// ��ʼ�����ֲ�����
	void InitLayout();

	// ------------- ����ת��
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
	 * @brief �ػ������ؼ������¿ؼ���ռ�õ���ͼ����(����Update())��
	 */
	void Repaint() override;
	/**
	 * @brief �ػ�ָ������ؼ���������ռ�õ���ͼ����(����Update())��
	 * @param rect ָ��������
	 */
	void Repaint(const rectix & rect) override;
	
	/**
	 * @brief ���¿ؼ���ռ�õ���ͼ����Update ��Ҫ��ؼ������ػ档
	 */
	void NcRepaint() override;
	/**
	 * @brief ���¿ؼ�ָ������ͼ����Update ��Ҫ��ؼ������ػ档
	 * @param rect ָ���ľ�������
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
	 * @brief ʹ�ÿؼ����ꡣ   
	 */
	HitTestE NcHitTest(pointix point) const override;
	HitTestE HitTest(pointix point) const override;
	HitTestE HitTestDeep(pointix point) const override;
public:
	void NcUpdateMouse(pointix point) override;
	// ��꽻��
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
	// ���̽���
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
	//! ���ؼ�ָ�롣
	IControl * m_pParent;
	//! �ؼ�����
	textw m_name;
	//! �ؼ���ID��
	int_x m_iId;
	//! �ؼ��� Z ˳��
	int_x m_iZOrder;
	//! �ؼ���ȵ�ģʽ
	WHModeE m_eWidthMode;
	//! �ؼ��߶ȵ�ģʽ
	WHModeE m_eHeightMode;

	//! �ؼ��ķ�Χ��
	rectix m_rect;
	//! �ؼ��ⲿ��������ࡣ
	edgeix m_margin;
	//! �ؼ��ڲ���������ࡣ
	edgeix m_padding;
	//! �ؼ������еı߿�����
	BorderTypeE m_eBorderType;
	//! �ؼ����ֵ�Ȩ��
	float_32 m_fWeight;

	//! �ؼ����ı����ؼ��ڲ�����֧�ֶ��ֽ��ı�����
	textw m_text;
	//! �ı���ʽ��
	font_t m_font;
	//! ��ݼ�
	KeyCodeE m_eHotKey;
	//! ����ɫ��
	uint_32 m_uiBackColor;
	//! ǰ��ɫ��
	uint_32 m_uiForeColor;
	//! ͼƬ�Ļ���ģʽ��
	ImageModeE m_eImageMode;
	//! �����ͣ�Ĺ�ꡣ
	CursorE m_eCursor;
	//! �ؼ��İ����͡�
	anchor_t m_anchor;
	//! �ؼ���ѡ��ʱ�����뷨״̬��
	ImeModeE m_eImeMode;
	union
	{
		//! �ػ��ǡ�
		uint_32 m_uiAutoRepaint;
		struct
		{
			//! Ԥ���������롢�Ƴ�ʱ�Ƿ��Զ��ػ档
			bool m_bRepaintMousePreInOut : 1;
			//! �����롢�Ƴ�ʱ�Ƿ��Զ��ػ档
			bool m_bRepaintMouseInOut : 1;
			//! ������¡��ɿ�ʱ�Ƿ��Զ��ػ档
			bool m_bRepaintMouseDownUpL : 1;
			//! �м����¡��ɿ�ʱ�Ƿ��Զ��ػ档
			bool m_bRepaintMouseDownUpM : 1;
			//! �Ҽ����¡��ɿ�ʱ�Ƿ��Զ��ػ档
			bool m_bRepaintMouseDownUpR : 1;

			//! �ı��ı�ʱ�Ƿ��ػ档
			bool m_bRepaintTextChanged : 1;	
			//! �ı��ı�ʱ�Ƿ��ػ档
			bool m_bRepaintFontChanged : 1;
			//! ѡ�иı�ʱ�Ƿ��ػ档
			bool m_bRepaintSelectedChanged: 1;
			//! ����ı�ʱ�Ƿ��ػ档
			bool m_bRepaintFocusedChanged: 1;
			//! ����ı�ʱ�Ƿ��ػ档
			bool m_bRepaintActiveChanged: 1;
			//! ��겶��״̬�ı�ʱ�Ƿ��ػ档
			bool m_bRepaintCaptureChanged : 1;
			//! �ɽ����Ըı�ʱ�Ƿ��ػ档
			bool m_bRepaintEnabledChanged : 1;

			//! �����ק���롢�Ƴ�ʱ�Ƿ��Զ��ػ档
			bool m_bRepaintMouseDragInOut : 1;

			//! ˮƽ����ʱ�Ƿ��Զ��ػ档
			bool m_bRepaintScrollX : 1;
			//! ��ֱ����ʱ�Ƿ��Զ��ػ档
			bool m_bRepaintScrollY : 1;
		};
	};

	union
	{
		//! һЩ���������ԡ�
		uint_32 m_uiBaseAttr;
		struct
		{
			//! �Ƿ�̬��������̬������Ҫ AddRef �� Release��
			bool m_bDynamic : 1;
			//! �Ƿ񲻿��á�
			bool m_bDisabled : 1;
			//! �Ƿ񲻿ɼ���
			bool m_bInvisible : 1;
			//! �Ƿ��Ѿ���ʾ��
			bool m_bShown : 1;
			//! �Ƿ�����Զ���ʾ�ӿؼ���Ϊ false ʱ��������Ҫ��ʱ���Զ����� child �� SetShown��
			//! һ����˵����ֵ��Ӧ��Ϊ false������Ӧ���ֶ����� child �� SetShown��
			//! ��Ϊ true ʱ��ͨ��������Ҫ���� SetShown ��������ʱ�����е� child �������������� show ״̬��
			bool m_bDisAutoShowChild : 1;
			//! �Ƿ�λ�ڸ��ؼ��ķǿͻ�����ͨ����ScrollBar λ�ڷǿͻ�����
			bool m_bNcControl;
			//! ��ͼ�����Ƿ���Ч��
			bool m_bValidate : 1;
			//! �Ƿ�����˫�����ͼ�����������Ҫ�ؼ��Լ��������ʹ�á�
			bool m_bBuffered : 1;
			//! �Ƿ��ռ��ꡣ
			bool m_bCaptured : 1;

			/**
			 * ѡ��;۽�
			 * ѡ���Ǿ۽���ǰ�ᡣ
			 * ѡ������Ǹÿؼ���ѡ���˶��ѣ�ͨ����˵���ؼ����ǿ�ѡ��ģ����ֿؼ��ǲ���ѡ��ģ��� Static��
			 * �۽��ǽ������뽹�㣬�ؼ���ѡ����֮�󣬿��Խ������뽹�㣬Ȼ��������뷨��������������֡���������Ϣ�ȡ�
			 *   ���İ��š�������˫����Ϣ���ܽ�����Ƶģ���������Ϣ�� MouseWheelModeFocus ģʽ�»ᰴ�ս��㴫�ݡ�
			 * Ĭ������£��ؼ����ǿ�ѡ��ģ����ǲ����ܽ��㡣
			 */
			//! �Ƿ����ѡ��
			bool m_bDisSelectAble : 1;
			//! �Ƿ�ѡ�С�
			bool m_bSelected : 1;
			//! �Ƿ�������뽹�㡣
			bool m_bFocusAble : 1;
			//! �Ƿ���н��㡣
			bool m_bFocused : 1;

			//! �Ƿ�����ϷŲ�����
			bool m_bAcceptDrop : 1;
			//! �Ƿ���Ӧ��Tab������
			bool m_bAcceptTab : 1;
			//! �Ƿ���Ӧ��Enter������
			bool m_bAcceptEnter : 1;
			//! �Ƿ�ܾ�������Ϣ
			bool m_bNotAcceptScroll : 1;
			//! �Ƿ��Ǿ�̬�ؼ���
			bool m_bStatic : 1;
			//! �Ƿ���͸���ؼ���͸���ؼ�����Ӧ�κ�����⡣
			bool m_bTransparent : 1;
			//! �Ƿ�ü��ӿؼ���ֻ��Ӱ�� NcHitTest���� HitTest �Ľ�����ϲ㴫�ݡ�
			bool m_bClipChild : 1;
			//! �Ƿ����ڽ��е���������
			bool m_bPopuping : 1;
			//! �Ƿ���յ���ʱ�������Ϣ��
			bool m_bPupupResponse : 1;
			//! �Ƿ�Ա߽�(edge)���� hittest
			bool m_bHitTestEdge : 1;
		};
	};

	/// ���״̬
	union
	{
		//! ���״̬��
		uint_32 m_uiMouseState;
		struct
		{
			//! �Ƿ���ͣ��
			bool m_bMouseIn : 1;
			//! ����Ƿ��¡�
			bool m_bMouseDownL: 1;
			//! �м��Ƿ��¡�
			bool m_bMouseDownM: 1;
			//! �Ҽ��Ƿ��¡�
			bool m_bMouseDownR: 1;
			//! �Ƿ���ͣ��
			bool m_bMouseNcIn : 1;
			//! ����Ƿ��¡�
			bool m_bMouseNcDownL: 1;
			//! �м��Ƿ��¡�
			bool m_bMouseNcDownM: 1;
			//! �Ҽ��Ƿ��¡�
			bool m_bMouseNcDownR: 1;

			bool m_bMousePreDragIn : 1;
			bool m_bMouseDragIn : 1;
		};
	};

	IControl * m_pScrollX;
	IControl * m_pScrollY;

	union
	{
		//! ����״̬
		uint_32 m_uiState;
		struct
		{
			//! ������ʾˮƽ������(�������)��
			bool m_bAlwaysShowScrollX : 1;
			//! ������ʾ��ֱ������(�������)��
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

	//--------------------------------- �Զ��Ű�
	void SetLayoutMode(LayoutModeE eType) override;
	LayoutModeE GetLayoutMode() const override;

	// ���� child �� widemode��heightmode��anchor ��ȷ���ؼ���λ�úͣ��򣩳ߴ硣
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
	 * @param point �������ꡣ
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
