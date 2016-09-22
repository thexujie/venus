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

	const oid_t & GetOid() const;
	void GetMeta(IMetaHelper * pHelper);

	void SetParent(IControl * pParent);

	IControl * GetParent() const;
	IControl * GetAncestor() const;
	IForm * GetForm() const;

	IScroll * ToIScroll() const;
	IForm * ToIForm() const;

	// ----------------------- ������Ϣ
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
	// Visual ����ȥ���߽��Ŀɼ�������Ρ�(border.Left, border.Right, Width - border.Width, Height - border.Height)
	rectix GetVisual() const;
	// Client ���򣬿ͻ�����������ȥ���߽硢������������
	rectix GetClient() const;
	// Offset ��С��ͨ����ָ�ͻ����ӿؼ�������ƫ�ơ�
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

	// �ؼ���ꡣ
	void SetCursor(CursorE eCursor);
	CursorE GetCursor() const;
	void NcRequestCursor(pointix point, CursorE & eCursor) const;
	void RequestCursor(pointix point, CursorE & eCursor) const;

	// �Ƿ���Խ��ս��㡣
	void SetFocusAble(bool bFocusAble);
	bool IsFocusAble() const;
	// �ܷ�ѡ�С�
	void SetSelectAble(bool bSelectAble);
	bool IsSelectAble() const;

	// ��ǰ�Ƿ���Խ��ս��㡣
	bool CanSelect() const;
	// ��ǰ�ܷ�ѡ�С�
	bool CanFocus() const;

	// �����ÿؼ���ý��㡣
	bool Focus(bool bFocus);
	// �ؼ��Ƿ��Ѿ�����˽��㡣
	void SetFocused(bool bFocused);
	bool IsFocused() const;

	// ѡ��ؼ��������ÿؼ���ý��㡣
	void Select(bool bSelect);
	// �ؼ��Ƿ��Ѿ���ѡ���ˡ�
	void SetSelected(bool bSelected);
	bool IsSelected() const;

	// ѡ��Tab˳���е���һ���ؼ���
	bool SelectNext(bool bWrap);
	// ѡ��Tab˳���е���һ���ؼ���
	bool SelectLast(bool bWrap);

	void SetStatic(bool bStatic);
	bool IsStatic() const;
	void SetTransparent(bool bTransparent);
	bool IsTransparent() const;
	void SetClipChild(bool bClipChild);
	bool IsClipChild() const;

	// �Ƿ���� Enter �� Tab �ַ���Ϣ(��������ַ���Ϣ��Tab ����ѡ����һ��/��һ��)��
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

	// �ؼ���ê������(��������)��
	void SetAnchor(AlignE eAnchor);
	AlignE GetAnchor() const;
	// ê��������
	void SetAnchorEdge(const edgeix & anchor);
	const edgeix & GetAnchorEdge() const;
	// ����ê�����������ÿؼ�λ��/��С��
	void Anchor();

	// ��ʼ�����ֲ�����
	void InitLayout();

	// ------------- ����ת��
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
	 * @brief �ػ������ؼ������¿ؼ���ռ�õ���ͼ����(����Update())��
	 */
	void Repaint();
	/**
	 * @brief �ػ�ָ������ؼ���������ռ�õ���ͼ����(����Update())��
	 * @param rect ָ��������
	 */
	void Repaint(const rectix & rect);
	
	/**
	 * @brief ���¿ؼ���ռ�õ���ͼ����Update ��Ҫ��ؼ������ػ档
	 */
	void NcRepaint();
	/**
	 * @brief ���¿ؼ�ָ������ͼ����Update ��Ҫ��ؼ������ػ档
	 * @param rect ָ���ľ�������
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
	 * @brief ʹ�ÿؼ����ꡣ   
	 */
	HitTestE NcHitTest(pointix point) const;
	HitTestE HitTest(pointix point) const;
	HitTestE HitTestDeep(pointix point) const;
public:
	void NcUpdateMouse(pointix point);
	// ��꽻��
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
	// ���̽���
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
	//! �ؼ���ê����Ϣ��
	edgeix m_anchor;
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
	AlignE m_eAnchor;
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
	IControlVector & GetChildren() { return m_children;}
	const IControlVector & GetChildren() const { return m_children; }

public:
	void AddControl(IControl * pControl);
	void RmvControl(IControl * pControl);
	void RmvAllControl();
	bool HasControl(IControl * pControl) const;

	//--------------------------------- �Զ��Ű�
	void SetLayoutMode(LayoutModeE eType);
	LayoutModeE GetLayoutMode() const;

	// ���� child �� widemode��heightmode��anchor ��ȷ���ؼ���λ�úͣ��򣩳ߴ硣
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
	 * @param point �������ꡣ
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
