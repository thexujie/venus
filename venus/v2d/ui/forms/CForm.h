#pragma once

#include "../controls/CControl.h"

VENUS_BEG

class V2D_API CForm : public CControl, public IForm
{
public:
	CForm();
	~CForm();
	const oid_t & GetOid() const;
	void GetMeta(IMetaHelper * pHelper);

	void SetParent(IControl * pParent);

	IForm * GetForm() const;
	IForm * ToIForm() const;

	void SetPosition(pointix position);
	void SetSize(sizeix size);
	edgeix GetBorder() const;
	rectix GetVisual() const;

	HitTestE NcHitTest(pointix point) const;
	void SetText(const char_16 * szText, int_x iLength = -1);
	void UpdateIme();
	void Capture(bool bCapture);
	void SetCaptureControl(IControl * pControl);

	bool ClipboardCheckData(ClipboardDataTypeE eDataType) const;
	textw ClipboardGetText16() const;
	void ClipboardSetText16(const char_16 * szText, int_x iLength = -1) const;

	void Active(bool bActive);
	using CControl::NcRepaint;
	void NcRepaint(const rectix & rect);
	pointix GetMousePosition() const;
	bool IsKeyDown(KeyCodeE ekeyCode) const;
	pointix ClientToScreen(const pointix & point) const;

	void OnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;

	void OnNcMouseMove(pointix point);
	void OnNcMouseOut(pointix point);
	void OnNcMouseDown(pointix point, MouseButtonE eButton);
	void OnNcMouseUp(pointix point, MouseButtonE eButton);
	void OnNcMouseClick(pointix point, MouseButtonE eButton);

	void OnSizeChanged();

	int_x PopupMenu(pointix point, IMenu * pMenu);
	int_x PopupControl(pointix point, IControl * pControl);

	void OnActiveChanged();

	void QueryDebugMenu(int_x & iBase, IMenu * pMenu) const;
	void OnDebugMenu(int_x iBase, int_x iResult);
public:
	void SetOwner(IForm * pForm);
	IForm * GetOwner() const;

	void SetFormType(FormTypeE eHostType);
	FormTypeE GetFormType() const;
	void CreateHost();
	void AttachHost(int_x iFormId);
	IHost * GetHost() const;

	void SetResizeBorder(edgeix border);
	edgeix GetResizeBorder() const;
	int_x GetFormId() const;

	void Show(ShowModeE eShow = ShowModeNormal, HostInitPosE eInitPos = HostInitPosDefault);
	void Hide();

	ShowModeE GetShowMode() const;
	bool IsMaxSize() const;
	bool IsMinSize() const;

	void OnRefresh(rectix rect);
	void OnCreate();
	void OnClose();
	void OnDestroy();
	void OnMaxSize();
	void OnMinSize();
	void OnRestore();

	int_x RunDialog();
	void EndRun(int_x iResult);

	bool MouseActiving(pointix point) const;
	bool IsActived() const;

	void NotifyPositionChanged(const pointix & position);
	void NotifySizeChanged(const sizeix & size, ShowModeE eShowMode);
	void NotifyShownChanged(bool bShown);
	void NotifyFocusChanged(bool bFocus);
	void NotifyActiveChanged(bool bActive);
	void NotifyCaptureChanged(bool bCaptured);
	void NotifyTextChanged(const char_16 * szText, int_x iLength = -1);

	void SetMinSize(const sizeix & size);
	void SetMaxSize(const sizeix & size);
	const sizeix & GetMinSize() const;
	const sizeix & GetMaxSize() const;

	void SetCaptionHeight(int_x iCaptionHeight);
	int_x GetCaptionHeight() const;

	void SetMinBox(bool bMinSizeBox);
	bool HasMinBox() const;
	void SetMaxBox(bool bMaxSizeBox);
	bool HasMaxBox() const;
	void SetCloseBox(bool bCloseBox);
	bool HasCloseBox() const;
	void SetBoxes(bool bMinSizeBox, bool bMaxSizeBox, bool bCloseBox);

	FormCommandE NcFindCommand(pointix point) const;
protected:
	IForm * m_pOwner;
	IHost * m_pHost;
	FormTypeE m_eFormType;
	sizeix m_minSize;
	sizeix m_maxSize;
	edgeix m_bdResize;
	ShowModeE m_eShowMode;

	int_x m_iCaptionHeight;
	FormCommandE m_eCommand;
	union
	{
		uint_x m_uiFormAttribute;
		struct
		{
			bool m_bInModal : 1;
			bool m_bActived : 1;
			bool m_bMinBox : 1;
			bool m_bMaxBox : 1;
			bool m_bCloseBox : 1;
		};
	};

	class CPopMenu * m_pPopupMenu;
	class CPopForm * m_pPopupForm;

public:
	event<IControl *, bool> ActiveChanged;

public:
	static const int_x BORDER_SIZE_DEFAULT = 4;
	static const int_x CAPTION_HEIGHT_DEFAULT = 24;
};

VENUS_END
