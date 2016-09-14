#pragma once

#include "Window.h"
#include "CUITheme.h"

VENUS_BEG

class WIN32_API Cursors
{
public:
	Cursors()
	{
		m_hCursors[CursorNormal] = ::LoadCursor(NULL, IDC_ARROW);
		m_hCursors[CursorCross] = ::LoadCursor(NULL, IDC_CROSS);
		m_hCursors[CursorHand] = ::LoadCursor(NULL, IDC_HAND);
		m_hCursors[CursorLink] = m_hCursors[CursorHand];
		m_hCursors[CursorHelp] = ::LoadCursor(NULL, IDC_HELP);
		m_hCursors[CursorWait] = ::LoadCursor(NULL, IDC_WAIT);
		m_hCursors[CursorIBeam] = ::LoadCursor(NULL, IDC_IBEAM);
		m_hCursors[CursorNotAviliable] = ::LoadCursor(NULL, IDC_NO);
		m_hCursors[CursorSizeLR] = ::LoadCursor(NULL, IDC_SIZEWE);
		m_hCursors[CursorSizeTB] = ::LoadCursor(NULL, IDC_SIZENS);
		m_hCursors[CursorSizeLTRB] = ::LoadCursor(NULL, IDC_SIZENESW);
		m_hCursors[CursorSizeLBRT] = ::LoadCursor(NULL, IDC_SIZENWSE);
		m_hCursors[CursorSizeAll] = ::LoadCursor(NULL, IDC_SIZEALL);
	}
	~Cursors()
	{

	}

	void SetCursor(CursorE eType)
	{
		if(eType >= 0 && eType <= CursorSizeAll)
			::SetCursor(m_hCursors[eType]);
	}

	void SetLastCursor()
	{
		SetCursor(m_cursor);
	}
	HCURSOR m_hCursors[CursorSizeAll + 1];
	CursorE m_cursor;
};

class WIN32_API CWindow : public ObjectT<IHost, IWindow>
{
public:
	CWindow();
	~CWindow();

	IObject * GetInterface(const oid_t & oid);
	int_x CreateHost(IControl * pControl, FormTypeE eHostType);
	bool Attach(int_x iHostId);
	void Detach();
	int_x GetFormId() const;
public: // IHost
	void SetPosition(const pointix & position);
	void SetSize(const sizeix & size);
	void SetText(const char_16 * szText, int_x iLength = -1);
	void SetBoxes(bool bMinBox, bool bMaxBox, bool bCloseBox);
	bool Capture(bool bCapture);
	void Update(rectix rect);
	void Refresh(rectix rect);

	pointix GetMousePosition() const;
	bool IsKeyDown(KeyCodeE ekeyCode) const;

	void SetHostInitPos(HostInitPosE eInitPos);
	HostInitPosE GetHostInitPos() const;

	pointix ClientToScreen(const pointix & point) const;

	void Active(bool bActive);
	void UpdateIme();
	bool ClipboardCheckData(ClipboardDataTypeE eDataType) const;
	textw ClipboardGetText16() const;
	void ClipboardSetText16(const char_16 * szText, int_x iLength = -1) const;

	void ShowHost(ShowModeE eShow, HostInitPosE eInitPos);

public: // IWindow
	void SetHandle(HWND hWnd);
	HWND GetHandle() const;

	PreMessageResultE PreHandleMSG(uint_32 uiMessage, uint_x & uiParam, int_x & iParam);
	void AfterHandleMSG(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	int_x HandleMSG(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	int_x SendMSG(uint_32 uiMessage, uint_x uiParam, int_x iParam) const;
	bool PostMSG(uint_32 uiMessage, uint_x uiParam, int_x iParam) const;
	event<uint_32, uint_x, int_x> & WmMessageEvent();

	void SetIcon(HICON hIcon);
	void SetIconBig(HICON hIcon);
	void SetParentHandle(HWND hWndParent);

	int_x OnDefault(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	const char_16 * GetClass() const;
public: // IWindow
	void SetStyle(uint_32 uiStyle);
	void ModifyStyle(uint_32 uiStyleAffect, bool bKeep);
	uint_32 GetStyle() const;

	void SetStyleEx(uint_32 uiStyleEx);
	void ModifyStyleEx(uint_32 uiStyleAffect, bool bKeep);
	uint_32 GetStyleEx() const;

	virtual int_x OnWmMove(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmSize(uint_32 uiMessage, uint_x uiParam, int_x iParam);

	virtual int_x OnWmShow(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmPaint(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmNcPaint(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmNcActivate(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmEraseBack(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmMouseMove(uint_32 uiMessage, uint_x uiParam, int_x iParam);

	virtual int_x OnWmMouseEnter(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmMouseLeave(uint_32 uiMessage, uint_x uiParam, int_x iParam);

	virtual int_x OnWmMouseDown(uint_32 uiMessage, uint_x uiParam, int_x iParam, MouseButtonE eButton);
	virtual int_x OnWmMouseUp(uint_32 uiMessage, uint_x uiParam, int_x iParam, MouseButtonE eButton);
	virtual int_x OnWmMouseClick(uint_32 uiMessage, uint_x uiParam, int_x iParam, MouseButtonE eButton);

	virtual int_x OnWmMouseDownL(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmNcMouseDownL(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmMouseUpL(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmMouseDownR(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmMouseUpR(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmMouseDownM(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmMouseUpM(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmMouseWheelV(uint_32 uiMessage, uint_x uiParam, int_x iParam);

	virtual int_x OnWmSetText(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmActive(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmSetFocus(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmKillFocus(uint_32 uiMessage, uint_x uiParam, int_x iParam);

	virtual int_x OnWmChar(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmUnicodeChar(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmKeyDown(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmKeyUp(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmMouseDBClick(uint_32 uiMessage, uint_x uiParam, int_x iParam);

	virtual int_x OnWmSetCursor(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmDropFiles(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmQueryDrag(uint_32 uiMessage, uint_x uiParam, int_x iParam);

	virtual int_x OnWmHitTest(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmNotify(uint_32 uiMessage, uint_x uiParam, int_x iParam);

	virtual int_x OnWmClose(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmDestroy(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmCaptureChanged(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmMouseActive(uint_32 uiMessage, uint_x uiParam, int_x iParam);

	virtual int_x OnWmNcCalcSize(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmNcMouseMove(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmGetMinMaxInfo(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmRefresh(uint_32 uiMessage, uint_x uiParam, int_x iParam);
	virtual int_x OnWmSysCommand(uint_32 uiMessage, uint_x uiParam, int_x iParam);

	virtual void Create();
	virtual void OnCreate();
	virtual void Close();
	virtual int_x OnClose();
	virtual void Destroy();
	virtual int_x OnDestroy();
protected:
	void ConfirmHandle() const;
	void CreateHandle(const char_16 * szClassName, const char_16 * szText, 
		uint_32 uiStyle, uint_32 uiStyleEx,
		int_x iX, int_x iY, int_x iWidth, int_x iHeight,
		HWND hWndParent, HMENU hMenu);
	void DestroyHandle();
	HWND GetOwnerHandle();
	void UpdateLayered(rectix rect);
protected:
	IControl * m_pControl;

	HostInitPosE m_eInitPos;
	HWND m_hWnd, m_hWndParent;
	WNDPROC m_pfnDefault;
	HICON m_hIcon, m_hIconBig;

	uint_32 m_uiStyle;
	uint_32 m_uiStyleEx;

	bool m_bTrackingMouse;
	int_64 m_iMouseDownLTime;
	pointix m_ptMouseDownL;
	pointix m_ptMouseDownM;
	pointix m_ptMouseDownR;
	pointix m_ptMouseDownLD;
	int_x m_iCaptureRef;
	Cursors m_cursors;
	CInputContext m_inputContext;
	CUITheme m_theme;
	mutable textw m_txtClipboard;

	rectix m_rcRefresh;
	// 防止 WM_REFRESH 被发送多次。0 正常 refresh，1 强制 refresh。
	int_x m_iRefreshing;
	CBmpBuffer * m_pBuffer;
	IPaint * m_pPaint;

public:
	event<uint_32, uint_x, int_x> Message;
public:
	static sizeix MakeSizeFromInt(int_x iParam);
	static pointix MakePointFromInt(int_x iParam);
	static sizeix MakeSizeFromUInt(uint_x uiParam);
	static pointix MakePointFromUInt(int_x uiParam);
	static pointix ScreenToClient(HWND hWnd, const pointix & point);
	static pointix ClientToScreen(HWND hWnd, const pointix & point);
	static bool IsClickPoint(const pointix & pointA, const pointix & pointB);
	static bool IsChildOf(HWND hWnd, HWND hWndParent);
};

VENUS_END
