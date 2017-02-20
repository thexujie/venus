#pragma once

#include "../controls/CControl.h"

VENUS_BEG

class V2D_API CForm : public CControl, public IForm
{
public:
	CForm();
	~CForm();
	const oid_t & GetOid() const override;
	void GetMeta(IMetaHelper * pHelper) override;

	void SetParent(IControl * pParent) override;

	IForm * GetForm() const override;
	IForm * ToIForm() const override;

	void SetPosition(pointix position) override;
	void SetSize(sizeix size) override;
	edgeix GetBorder() const override;
	rectix GetVisual() const override;

	HitTestE NcHitTest(pointix point) const override;
	void SetText(const char_16 * szText, int_x iLength = -1) override;

	sizeix GetPreferedSize() const override;
	void UpdateIme() override;
	void Capture(bool bCapture) override;
	void SetCaptureControl(IControl * pControl) override;

	bool ClipboardCheckData(ClipboardDataTypeE eDataType) const override;
	textw ClipboardGetText16() const override;
	void ClipboardSetText16(const char_16 * szText, int_x iLength = -1) const override;

	void Active(bool bActive);
	using CControl::NcRepaint;
	void NcRepaint(const rectix & rect) override;
	pointix GetMousePosition() const override;
	bool IsKeyDown(KeyCodeE ekeyCode) const override;
	pointix ToScreen(const pointix & point) const override;

	void OnNcPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const override;

	void OnNcMouseMove(pointix point) override;
	void OnNcMouseOut(pointix point) override;
	void OnNcMouseDown(pointix point, MouseButtonE eButton) override;
	void OnNcMouseUp(pointix point, MouseButtonE eButton) override;
	void OnNcMouseClick(pointix point, MouseButtonE eButton) override;

	void OnSizeChanged() override;

	int_x PopupMenu(pointix point, IMenu * pMenu) override;
	int_x PopupControl(pointix point, IControl * pControl) override;

	void OnActiveChanged() override;

	void QueryDebugMenu(int_x & iBase, IMenu * pMenu) const override;
	void OnDebugMenu(int_x iBase, int_x iResult) override;
public:
	void SetOwner(IForm * pForm) override;
	IForm * GetOwner() const override;

	void SetFormType(FormTypeE eHostType) override;
	FormTypeE GetFormType() const override;
	void CreateHost() override;
	void AttachHost(int_x iFormId) override;
	IHost * GetHost() const override;

	void SetResizeBorder(edgeix border) override;
	edgeix GetResizeBorder() const override;
	int_x GetFormId() const override;

	void Show(ShowModeE eShow = ShowModeNormal, HostInitPosE eInitPos = HostInitPosDefault) override;
	void Hide() override;

	ShowModeE GetShowMode() const override;
	bool IsMaxSize() const override;
	bool IsMinSize() const override;

	void OnRefresh(rectix rect) override;
	void OnCreate() override;
	void OnClose() override;
	void OnDestroy() override;
	void OnMaxSize() override;
	void OnMinSize() override;
	void OnRestore() override;

	int_x RunDialog() override;
	void EndRun(int_x iResult) override;

	bool MouseActiving(pointix point) const override;
	bool IsActived() const override;

	void NotifyPositionChanged(const pointix & position) override;
	void NotifySizeChanged(const sizeix & size, ShowModeE eShowMode) override;
	void NotifyShownChanged(bool bShown) override;
	void NotifyFocusChanged(bool bFocus) override;
	void NotifyActiveChanged(bool bActive) override;
	void NotifyCaptureChanged(bool bCaptured) override;
	void NotifyTextChanged(const char_16 * szText, int_x iLength = -1) override;

	void SetMinSize(const sizeix & size) override;
	void SetMaxSize(const sizeix & size) override;
	const sizeix & GetMinSize() const override;
	const sizeix & GetMaxSize() const override;

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
