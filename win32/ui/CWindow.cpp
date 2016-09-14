#include "stdafx.h"
#include "CWindow.h"

VENUS_BEG

CWindow::CWindow()
	: m_pControl(nullptr),
	m_hWnd(NULL), m_hWndParent(NULL), m_pfnDefault(nullptr), m_eInitPos(HostInitPosDefault),
	m_uiStyle(0), m_uiStyleEx(0), m_iMouseDownLTime(0), m_hIcon(NULL), m_hIconBig(NULL), m_iCaptureRef(0), m_bTrackingMouse(false),
	m_pBuffer(nullptr), m_pPaint(nullptr), m_iRefreshing(0)
{
}

CWindow::~CWindow()
{
	if(m_hWnd)
	{
		// 马上就销毁了，等不及消息循环来处理 wm_destroy 了。
		Destroy();
		m_hWnd = NULL;
	}
}

IObject * CWindow::GetInterface(const oid_t & oid)
{
	if(oid == OID_IWindow)
		return (IWindow *)this;
	else
		return nullptr;
}

int_x CWindow::CreateHost(IControl * pControl, FormTypeE eHostType)
{
	if(!pControl || !pControl->ToIForm())
		throw exp_illegal_argument();
	m_pControl = pControl;

	if(!m_hWnd)
	{
		switch(eHostType)
		{
		case FormTypeDefault:
			m_uiStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
			m_uiStyleEx = 0;
			break;
		case FormTypePopup:
			m_uiStyle = WS_POPUP | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_THICKFRAME;
			m_uiStyleEx = 0;
			break;
		case FormTypeLayered:
			m_uiStyle = WS_POPUP | WS_SYSMENU | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_THICKFRAME;
			m_uiStyleEx = WS_EX_LAYERED;
			break;
		case FormTypeMenu:
			m_uiStyle = WS_POPUP;
			m_uiStyleEx = 0;
			break;
		default:
			throw exp_illegal_argument();
		}
	}
	return (int_x)m_hWnd;
}

bool CWindow::Attach(int_x iHostId)
{
	m_hWnd = (HWND)iHostId;
	Window::Attatch((HWND)iHostId, this);
	m_pfnDefault = Window::GetDefaultWndProc(m_hWnd);
	return true;
}

void CWindow::Detach()
{
	Window::Detatch(m_hWnd);
}

int_x CWindow::GetFormId() const
{
	return (int_x)m_hWnd;
}

void CWindow::SetHandle(HWND hWnd)
{
	Attach((int_x)hWnd);
}

HWND CWindow::GetHandle() const
{
	return m_hWnd;
}

void CWindow::ShowHost(ShowModeE eShow, HostInitPosE eInitPos)
{
	if(eShow == ShowModeHide && !m_hWnd)
		return;

	if(!m_hWnd)
		Create();

	switch(eInitPos)
	{
	case HostInitPosDefault:
		break;
	case HostInitPosCenterParent:
		if(HWND hWndParent = Window::GetOwner(m_hWnd))
		{
			rectix rcParent = Window::GetRect(hWndParent);
			rectix rcThis = Window::GetRect(m_hWnd);
			int_x iX = rcParent.x + (rcParent.w - rcThis.w) / 2;
			int_x iY = rcParent.y + (rcParent.h - rcThis.h) / 2;
			SetPosition(pointix(iX, iY));
		}
		else
		{
			rectix rcThis = Window::GetRect(m_hWnd);
			int_x iScreenW = GetSystemMetrics(SM_CXSCREEN);
			int_x iScreenH = GetSystemMetrics(SM_CYSCREEN);
			int_x iX = (iScreenW - rcThis.w) / 2;
			int_x iY = (iScreenH - rcThis.h) / 2;
			SetPosition(pointix(iX, iY));
		}
		break;
	case HostInitPosCenterScreen:
		if(true)
		{
			rectix rcThis = Window::GetRect(m_hWnd);
			int_x iScreenW = GetSystemMetrics(SM_CXSCREEN);
			int_x iScreenH = GetSystemMetrics(SM_CYSCREEN);
			int_x iX = (iScreenW - rcThis.w) / 2;
			int_x iY = (iScreenH - rcThis.h) / 2;
			SetPosition(pointix(iX, iY));
		}
		break;
	default:
		break;
	}

	switch(eShow)
	{
	case ShowModeNormal:
		::ShowWindow(m_hWnd, SW_SHOW);
		break;
	case ShowModeNormalNoActive:
		::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
		break;
	case ShowModeMinisize:
		::ShowWindow(m_hWnd, SW_MINIMIZE);
		break;
	case ShowModeMaxisize:
		::ShowWindow(m_hWnd, SW_MAXIMIZE);
		break;
	case ShowModeRestore:
		::ShowWindow(m_hWnd, SW_RESTORE);
		break;
	case ShowModeHide:
		::ShowWindow(m_hWnd, SW_HIDE);
		break;
	default:
		throw exp_illegal_argument();
	}
}

void CWindow::Update(rectix rect)
{
	if(m_pControl && m_pControl->IsShown())
	{
		//if(m_uiStyleEx & WS_EX_LAYERED)
		//{
		//	throw exp_not_implement();
		//}
		//else
		{
			if(!m_iRefreshing)
			{
				PostMSG(WM_REFRESH, 0, 0);
				m_iRefreshing = 1;
			}
			//RECT rc = {(int_32)rect.X, (int_32)rect.Y, (int_32)rect.GetRight(), (int_32)rect.GetBottom()};
			//::InvalidateRect(m_hWnd, &rc, FALSE);
		}
		m_rcRefresh = m_rcRefresh.join(rect);
	}
}

void CWindow::Refresh(rectix rect)
{
	if(rect.is_empty() || !m_pControl)
		return;

	rectix rcControl(pointix(), m_pControl->GetSize());
	if(rcControl.size.is_empty())
		return;

	if(!m_pBuffer)
		m_pBuffer = new CBmpBuffer();

	m_pBuffer->ReserveSize(rcControl.w, rcControl.h);

	if(!m_pPaint)
		m_pPaint = Get2DDevice()->CreatePaint(m_pBuffer);

	if(!m_pPaint)
		return;

	if(m_uiStyleEx & WS_EX_LAYERED)
	{
		//int_64 iTimeBeg = TimeGetInMS();

		if(rect.is_valid() && m_pPaint->BeginPaint())
		{
			m_pPaint->PushClip(rect);
			m_pPaint->Clear(rect, 0);
			m_pControl->PreOnNcPaint(m_pPaint, rect, &m_theme);
			m_pPaint->PopClip();
			m_pPaint->EndPaint();
		}
		//log1(L"绘图结束，耗时 %lld 毫秒。\n", TimeGetInMS() - iTimeBeg);

		rectix rcWindow = m_pControl->GetRect();
		rectix rcWindow2 = Window::GetRect(m_hWnd);
		//log(1, "(%d,%d)->(%d,%d)", rcWindow.x, rcWindow.y, rcWindow2.x, rcWindow2.y);

		POINT ptDst = {(int_32)rcWindow.x, (int_32)rcWindow.y};
		SIZE szDst = {(int_32)rcWindow.w, (int_32)rcWindow.h};
		POINT ptSrc = {0, 0};
		BLENDFUNCTION bf = {0};
		bf.AlphaFormat = AC_SRC_ALPHA;
		bf.BlendFlags = 0;
		bf.BlendOp = AC_SRC_OVER;
		bf.SourceConstantAlpha = 0xFF;
		UpdateLayeredWindow(m_hWnd, NULL, &ptDst, &szDst, m_pBuffer->GetBufferDC(), &ptSrc, 0, &bf, ULW_ALPHA);
	}
	else
	{
		// 有时候 rcUpdate 的尺寸会超过大小。
		//int_64 iTimeBeg = TimeGetInMS();
		if(m_pPaint->BeginPaint())
		{
			m_pPaint->PushClip(rect);
			m_pPaint->Clear(rect, 0);
			m_pControl->PreOnNcPaint(m_pPaint, rect, &m_theme);
			m_pPaint->PopClip();
			m_pPaint->EndPaint();
		}
		//log1(L"绘图结束，耗时 %lld 毫秒。\n", TimeGetInMS() - iTimeBeg);

		HDC hdcWindow = GetDC(m_hWnd);
		m_pBuffer->BitBltTo(hdcWindow, rect.x, rect.y, rect.w, rect.h);
		ReleaseDC(m_hWnd, hdcWindow);
	}
}

pointix CWindow::GetMousePosition() const
{
	if(m_pControl && m_hWnd)
	{
		pointix point = Window::GetMousePos(m_hWnd);
		return m_pControl->ToClient(point);
	}
	else
		return pointix();
}

bool CWindow::IsKeyDown(KeyCodeE ekeyCode) const
{
	return (::GetKeyState((int_32)Window::KeyCodeToVirtualKey(ekeyCode)) & 0x80) != 0;
}

int_x CWindow::SendMSG(uint_32 uiMessage, uint_x uiParam, int_x iParam) const
{
	ConfirmHandle();
	return ::SendMessageW(m_hWnd, uiMessage, uiParam, iParam);
}

bool CWindow::PostMSG(uint_32 uiMessage, uint_x uiParam, int_x iParam) const
{
	ConfirmHandle();
	return ::PostMessageW(m_hWnd, uiMessage, uiParam, iParam) != 0;
}

event<uint_32, uint_x, int_x> & CWindow::WmMessageEvent()
{
	return Message;
}

void CWindow::SetIcon(HICON hIcon)
{
	if(m_hWnd)
		SendMSG(WM_SETICON, ICON_SMALL, (uint_x)hIcon);
	else
		m_hIcon = hIcon;
}

void CWindow::SetIconBig(HICON hIcon)
{
	if(m_hWnd)
		SendMSG(WM_SETICON, ICON_BIG, (uint_x)hIcon);
	else
		m_hIconBig = hIcon;
}

void CWindow::SetParentHandle(HWND hWndParent)
{
	if(m_hWnd)
		SetWindowLongPtrW(m_hWnd, GWLP_HWNDPARENT, (LONG_PTR)hWndParent);
	m_hWndParent = hWndParent;
}

int_x CWindow::OnDefault(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	if(m_pfnDefault)
		return (int_x)CallWindowProc(m_pfnDefault, m_hWnd, (UINT)uiMessage, (WPARAM)uiParam, (LPARAM)iParam);
	else
		return (int_x)CallWindowProc(DefWindowProcW, m_hWnd, (UINT)uiMessage, (WPARAM)uiParam, (LPARAM)iParam);
}

void CWindow::SetStyle(uint_32 uiStyle)
{
	if(m_hWnd)
		SetWindowLongW(m_hWnd, GWL_STYLE, (LONG)uiStyle);
	m_uiStyle = uiStyle;
}

void CWindow::ModifyStyle(uint_32 uiStyleAffect, bool bKeep)
{
	if(m_hWnd)
	{
		LONG lStylte = GetWindowLongW(m_hWnd, GWL_STYLE);
		uint_32 uiStyle = (uint_32)lStylte;
		if(bKeep)
			uiStyle |= uiStyleAffect;
		else
			uiStyle &= ~uiStyleAffect;
		SetWindowLongW(m_hWnd, GWL_STYLE, (LONG)uiStyle);
		m_uiStyle = uiStyle;
	}
	else
	{
		uint_32 uiStyle = m_uiStyle;
		if(bKeep)
			uiStyle |= uiStyleAffect;
		else
			uiStyle &= ~uiStyleAffect;
		m_uiStyle = uiStyle;
	}
}

uint_32 CWindow::GetStyle() const
{
	return m_uiStyle;
}

void CWindow::SetStyleEx(uint_32 uiStyleEx)
{
	if(m_hWnd)
		SetWindowLongW(m_hWnd, GWL_EXSTYLE, (LONG)uiStyleEx);
	m_uiStyleEx = uiStyleEx;
}

void CWindow::ModifyStyleEx(uint_32 uiStyleAffect, bool bKeep)
{
	uint_32 uiStyleEx = m_uiStyleEx;
	if(bKeep)
		uiStyleEx |= uiStyleAffect;
	else
		uiStyleEx &= ~uiStyleAffect;

	if(m_hWnd)
		SetWindowLongW(m_hWnd, GWL_EXSTYLE, (LONG)uiStyleEx);
	m_uiStyleEx = uiStyleEx;
}

uint_32 CWindow::GetStyleEx() const
{
	return m_uiStyleEx;
}

PreMessageResultE CWindow::PreHandleMSG(uint_32 uiMessage, uint_x & uiParam, int_x & iParam)
{
	if(uiMessage == WM_KEYDOWN &&
		Window::VirtualKeyToKeyCode(uiParam) == KeyCodeTab &&
		(m_pControl && !(m_pControl->CanAcceptTab())))
		return PreMessageResultNotTranslate;
	else
		return PreMessageResultOk;
}

void CWindow::AfterHandleMSG(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{

}

#define CASE_MSG(M, F) case M: return F(uiMessage, uiParam, iParam)
int_x CWindow::HandleMSG(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	ConfirmHandle();
	int_x iResult = Message(uiMessage, uiParam, iParam);
	if(iResult)
		return 0;

	switch(uiMessage)
	{
		CASE_MSG(WM_NCHITTEST, OnWmHitTest);
		CASE_MSG(WM_NCCALCSIZE, OnWmNcCalcSize);
		CASE_MSG(WM_SHOWWINDOW, OnWmShow);
		CASE_MSG(WM_ERASEBKGND, OnWmEraseBack);
		CASE_MSG(WM_PAINT, OnWmPaint);
		CASE_MSG(WM_NCPAINT, OnWmNcPaint);
		CASE_MSG(WM_NCACTIVATE, OnWmNcActivate);

		CASE_MSG(WM_MOUSEMOVE, OnWmMouseMove);
		CASE_MSG(WM_MOUSELEAVE, OnWmMouseLeave);

		CASE_MSG(WM_LBUTTONDOWN, OnWmMouseDownL);
		CASE_MSG(WM_LBUTTONUP, OnWmMouseUpL);

		CASE_MSG(WM_RBUTTONDOWN, OnWmMouseDownR);
		CASE_MSG(WM_RBUTTONUP, OnWmMouseUpR);

		CASE_MSG(WM_MBUTTONDOWN, OnWmMouseDownM);
		CASE_MSG(WM_MBUTTONUP, OnWmMouseUpM);

		CASE_MSG(WM_NCLBUTTONDOWN, OnWmNcMouseDownL);

		CASE_MSG(WM_LBUTTONDBLCLK, OnWmMouseDBClick);
		CASE_MSG(WM_MOUSEWHEEL, OnWmMouseWheelV);

		CASE_MSG(WM_MOVE, OnWmMove);
		CASE_MSG(WM_SIZE, OnWmSize);
		CASE_MSG(WM_SETTEXT, OnWmSetText);
		CASE_MSG(WM_ACTIVATE, OnWmActive);
		CASE_MSG(WM_SETFOCUS, OnWmSetFocus);
		CASE_MSG(WM_KILLFOCUS, OnWmKillFocus);
		CASE_MSG(WM_CHAR, OnWmChar);
		CASE_MSG(WM_UNICHAR, OnWmChar);

		CASE_MSG(WM_SYSKEYDOWN, OnWmKeyDown);
		CASE_MSG(WM_KEYDOWN, OnWmKeyDown);
		CASE_MSG(WM_SYSKEYUP, OnWmKeyUp);
		CASE_MSG(WM_KEYUP, OnWmKeyUp);

		CASE_MSG(WM_SETCURSOR, OnWmSetCursor);
		CASE_MSG(WM_QUERYDRAGICON, OnWmQueryDrag);
		CASE_MSG(WM_DROPFILES, OnWmDropFiles);
		CASE_MSG(WM_NOTIFY, OnWmNotify);
		CASE_MSG(WM_CAPTURECHANGED, OnWmCaptureChanged);
		CASE_MSG(WM_MOUSEACTIVATE, OnWmMouseActive);
		CASE_MSG(WM_NCMOUSEMOVE, OnWmNcMouseMove);

		CASE_MSG(WM_CLOSE, OnWmClose);
		CASE_MSG(WM_DESTROY, OnWmDestroy);

		CASE_MSG(WM_GETMINMAXINFO, OnWmGetMinMaxInfo);
		CASE_MSG(WM_SYSCOMMAND, OnWmSysCommand);

		CASE_MSG(WM_REFRESH, OnWmRefresh);
	default:
		return OnDefault(uiMessage, uiParam, iParam);
	}
	return 0;
}

int_x CWindow::OnWmShow(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	//Printf(L"[%s]OnWmShow (%d, %d)\n", GetOid().Name, uiParam, iParam);
	if(!iParam)
		m_pControl->ToIForm()->NotifyShownChanged(uiParam != 0);
	return 0;
}

int_x CWindow::OnWmPaint(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	PAINTSTRUCT ps;
	HDC hdc = ::BeginPaint(m_hWnd, &ps);
	if(m_pBuffer)
	{
		m_pBuffer->BitBltTo(hdc, ps.rcPaint.left, ps.rcPaint.top,
			ps.rcPaint.right - ps.rcPaint.left,
			ps.rcPaint.bottom - ps.rcPaint.top);
	}
	::EndPaint(m_hWnd, &ps);
	return 0;
}

int_x CWindow::OnWmNcPaint(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	if(m_uiStyle & WS_POPUP)
		return 0;
	return OnDefault(uiMessage, uiParam, iParam);
}

int_x CWindow::OnWmNcActivate(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	if(::IsIconic(m_hWnd))
		return 1;

	if(m_uiStyle & WS_POPUP)
		return 1;
	else
		return OnDefault(uiMessage, uiParam, iParam);

}

int_x CWindow::OnWmEraseBack(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	return 0;
}

int_x CWindow::OnWmMouseMove(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	if(m_pControl)
	{
		if(!m_bTrackingMouse)
		{
			OnWmMouseEnter(uiMessage, uiParam, iParam);
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.dwHoverTime = 1;
			tme.hwndTrack = m_hWnd;
			TrackMouseEvent(&tme);
			m_bTrackingMouse = true;
		}
		pointix point = MakePointFromInt(iParam);
		m_pControl->PreOnNcMouseMove(point);
	}
	return 0;
}

int_x CWindow::OnWmMouseEnter(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	//Printf(L"[OnWmMouseEnter]%s\n", GetText());
	pointix point = MakePointFromInt(iParam);
	m_pControl->PreOnNcMouseIn(point);
	return 0;
}

int_x CWindow::OnWmMouseLeave(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	//log1(L"[OnWmMouseLeave]%s\n", m_pControl->GetText());
	if(m_pControl->IsShown() && m_pControl->IsMouseNcIn())
	{
		pointix point = MakePointFromInt(iParam);
		m_pControl->PreOnNcMouseOut(point);
	}
	m_bTrackingMouse = false;
	return 0;
}

int_x CWindow::OnWmMouseDown(uint_32 uiMessage, uint_x uiParam, int_x iParam, MouseButtonE eButton)
{
	pointix point = MakePointFromInt(iParam);
	switch(eButton)
	{
	case MouseButtonL:
		m_ptMouseDownL = point;
		++m_iCaptureRef;
		break;
	case MouseButtonM:
		m_ptMouseDownM = point;
		++m_iCaptureRef;
		break;
	case MouseButtonR:
		m_ptMouseDownR = point;
		++m_iCaptureRef;
		break;
	default:
		Assert(false);
		break;
	}

	//if(m_iCaptureRef == 1)
		//::SetCapture(m_hWnd);

	OnWmMouseMove(WM_MOUSEMOVE, 0, iParam);
	m_pControl->PreOnNcMouseDown(point, eButton);
	// 有时候从其他窗口切换到此窗口的时候，光标形状没有及时更新，需要重置一次。
	CursorE eCursor = CursorNormal;
	m_pControl->NcRequestCursor(point, eCursor);
	m_cursors.SetCursor(eCursor);

	return 0;
}

int_x CWindow::OnWmMouseUp(uint_32 uiMessage, uint_x uiParam, int_x iParam, MouseButtonE eButton)
{
	pointix point = MakePointFromInt(iParam);
	pointix * pCheckPoint = nullptr;
	bool bReleaseCapture = false;
	switch(eButton)
	{
	case MouseButtonL:
		pCheckPoint = &m_ptMouseDownL;
		m_ptMouseDownLD = m_ptMouseDownL;
		--m_iCaptureRef;
		break;
	case MouseButtonR:
		pCheckPoint = &m_ptMouseDownM;
		--m_iCaptureRef;
		break;
	case MouseButtonM:
		pCheckPoint = &m_ptMouseDownR;
		--m_iCaptureRef;
		break;
	default:
		Assert(false);
		break;
	}

	if(IsClickPoint(point, *pCheckPoint))
	{
		m_pControl->PreOnNcMouseClick(point, eButton);
		m_pControl->PreOnNcMouseUp(point, eButton);
	}
	else
	{
		m_pControl->PreOnNcMouseUp(point, eButton);
	}

	pCheckPoint->set(0, 0);

	//if(m_iCaptureRef == 0)
		//::ReleaseCapture();

	if(m_pControl->IsMouseNcIn())
	{
		OnWmMouseMove(WM_MOUSEMOVE, 0, iParam);

		CursorE eCursor = CursorNormal;
		m_pControl->NcRequestCursor(point, eCursor);
		m_cursors.SetCursor(eCursor);
	}
	return 0;
}

int_x CWindow::OnWmMouseClick(uint_32 uiMessage, uint_x uiParam, int_x iParam, MouseButtonE eButton)
{
	pointix point = MakePointFromInt(iParam);
	m_pControl->PreOnNcMouseClick(point, eButton);
	return 0;
}

int_x CWindow::OnWmMouseDownL(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	int_64 iTime = time_ms();
	int_64 iPeroid = (int_64)::GetDoubleClickTime();
	pointix point = MakePointFromInt(iParam);
	bool bDoubleClick = false;
	if(iTime - m_iMouseDownLTime <= iPeroid && IsClickPoint(point, m_ptMouseDownLD))
		bDoubleClick = true;
	int_x iRet = OnWmMouseDown(uiMessage, uiParam, iParam, MouseButtonL);
	if(bDoubleClick)
	{
		OnWmMouseDBClick(WM_LBUTTONDBLCLK, uiParam, iParam);
		m_iMouseDownLTime = 0;
	}
	else
		m_iMouseDownLTime = iTime;
	return iRet;
}

int_x CWindow::OnWmNcMouseDownL(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	return OnDefault(uiMessage, uiParam, iParam);
}

int_x CWindow::OnWmMouseUpL(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	return OnWmMouseUp(uiMessage, uiParam, iParam, MouseButtonL);
}

int_x CWindow::OnWmMouseDownR(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	return OnWmMouseDown(uiMessage, uiParam, iParam, MouseButtonR);
}

int_x CWindow::OnWmMouseUpR(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	return OnWmMouseUp(uiMessage, uiParam, iParam, MouseButtonR);
}
int_x CWindow::OnWmMouseDownM(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	return OnWmMouseDown(uiMessage, uiParam, iParam, MouseButtonM);
}

int_x CWindow::OnWmMouseUpM(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	return OnWmMouseUp(uiMessage, uiParam, iParam, MouseButtonM);
}

int_x CWindow::OnWmMouseDBClick(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	pointix point = MakePointFromInt(iParam);
	m_pControl->PreOnNcMouseDBClick(point, MouseButtonL);
	return 0;
}

int_x CWindow::OnWmSetCursor(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	HWND hWnd = (HWND)uiParam;
	int_x iHitResult = iParam & 0xFFFF;;
	int_x iMessage = (iParam >> 16) & 0xFFFF;

	if(iHitResult == HTCLIENT)
	{
		pointix ptMouse = Window::GetMousePos(m_hWnd);
		CursorE eCursor = CursorNormal;
		m_pControl->NcRequestCursor(ptMouse, eCursor);
		m_cursors.SetCursor(eCursor);
		return 0;
	}
	else
		return OnDefault(uiMessage, uiParam, iParam);
}

int_x CWindow::OnWmMouseWheelV(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	pointix point = MakePointFromInt(iParam);
	pointix wheel = MakePointFromUInt(uiParam);
	point = ScreenToClient(m_hWnd, point);
	m_pControl->PreOnNcMouseWheel(point, wheel.y / WHEEL_DELTA, AlignTopBottom);
	PostMSG(WM_MOUSEMOVE, 0, i32fromi16((int_16)point.x, (int_16)point.y));
	return 0;
}

int_x CWindow::OnWmMove(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	pointix position = MakePointFromInt(iParam);
	//log1(L"CWindow::OnWmMove (%d,%d)", position.x, position.y);
	m_pControl->ToIForm()->NotifyPositionChanged(position);
	return 0;
}

int_x CWindow::OnWmSize(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	sizeix size = MakeSizeFromInt(iParam);
	IForm * pForm = m_pControl->ToIForm();
	switch(uiParam)
	{
	case SIZE_RESTORED:
		m_pControl->ToIForm()->NotifySizeChanged(size, ShowModeRestore);
		break;
	case SIZE_MINIMIZED:
		m_pControl->ToIForm()->NotifySizeChanged(size, ShowModeMinisize);
		break;
	case SIZE_MAXIMIZED:
	{
		// 必须在当前显示器的范围内，检测是否被真的全屏了。
		IMonitorManager * pmm = IMonitorManager::GetInstance();
		IMonitor * pm = pmm->GetMonitorFromWindowId((int_x)m_hWnd);
		rectix rcMonitor = pm->GetRect();
		rectix rcWork = pm->GetWorkRect();
		rcWork.position -= rcMonitor.position;
		RECT rect;
		::GetWindowRect(m_hWnd, &rect);
		if(rect.right - rect.left == rcWork.w &&
			rect.bottom - rect.top == rcWork.h)
			m_pControl->ToIForm()->NotifySizeChanged(size, ShowModeMaxisize);
		else
			m_pControl->ToIForm()->NotifySizeChanged(size, ShowModeMaxisizeFake);
	}
	break;
	default:
		m_pControl->ToIForm()->NotifySizeChanged(size, ShowModeNormal);
		break;
	}
	return 0;
}

int_x CWindow::OnWmSetText(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	const char_16 * szText = (const char_16 *)iParam;
	if(IsWindowUnicode(m_hWnd))
		m_pControl->ToIForm()->NotifyTextChanged(szText, textlen(szText));
	return OnDefault(uiMessage, uiParam, iParam);
}

int_x CWindow::OnWmActive(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	uint_x uiReason = uxluh(uiParam);
	uint_x uiIsMinisize = uxhuh(uiParam);

	pointix point = Window::GetMousePos(m_hWnd);
	CursorE eCursor = CursorNormal;
	m_pControl->NcRequestCursor(point, eCursor);
	m_cursors.SetCursor(eCursor);

	m_pControl->ToIForm()->NotifyActiveChanged(uiReason != WA_INACTIVE);
	return 0;
}

int_x CWindow::OnWmSetFocus(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	if(m_pControl)
	{
		//Printf(L"OnWmSetFocus %s\n", m_pControl->GetOid().name);
		m_pControl->Focus(true);
	}
	return OnDefault(uiMessage, uiParam, iParam);
}

int_x CWindow::OnWmKillFocus(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	m_inputContext.SetImeMode(ImeModeDisable);
	m_pControl->Focus(false);
	return OnDefault(uiMessage, uiParam, iParam);
}

int_x CWindow::OnWmChar(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	if(uiParam)
		m_pControl->PreOnKeyInput((char_32)uiParam);
	return 0;
}

int_x CWindow::OnWmUnicodeChar(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	//if(uiParam != UNICODE_NOCHAR)
	//	m_pControl->PreOnKeyInput((char_32)uiParam);

	// unicode 窗口无法收到这个消息 
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646288(v=vs.85).aspx
	// WM_UNICHAR 返回 true 可以拒绝 WM_CHAR.
	return 0;
}

int_x CWindow::OnWmKeyDown(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	KeyCodeE eKey = Window::VirtualKeyToKeyCode(uiParam);
	if(eKey != KeyCodeInvalid)
	{
		if(eKey == KeyCodeControl)
			eKey = (iParam & intx_bits(24)) ? KeyCodeControlR : KeyCodeControlL;
		else if(eKey == KeyCodeAlt)
			eKey = (iParam & intx_bits(24)) ? KeyCodeAltR : KeyCodeAltL;
		else {}
		m_pControl->PreOnKeyDown(eKey);
	}
	return 0;
}

int_x CWindow::OnWmKeyUp(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	KeyCodeE eKey = Window::VirtualKeyToKeyCode(uiParam);
	if(eKey != KeyCodeInvalid)
	{
		if(eKey == KeyCodeControl)
			eKey = (iParam & intx_bits(24)) ? KeyCodeControlR : KeyCodeControlL;
		else if(eKey == KeyCodeAlt)
			eKey = (iParam & intx_bits(24)) ? KeyCodeAltR : KeyCodeAltL;
		else {}
		m_pControl->PreOnKeyUp(eKey);
	}
	return 0;
}

int_x CWindow::OnWmDropFiles(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	//HDROP hDrop = (HDROP)uiParam;
	//TCHAR fileName[MAX_PATH] = {0};
	//uint_32 cnt = DragQueryFile(hDrop, 0xFFFFFFFF, fileName, MAX_PATH);
	////if(cnt > 0)
	////{
	////	DragQueryFile(hDrop, 0, fileName, MAX_PATH);
	////	MessageBox(fileName);
	////}
	////DragFinish(hDrop);  
	return 0;
}
int_x CWindow::OnWmQueryDrag(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	return 0;
}

int_x CWindow::OnWmHitTest(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	int_x iDefault = OnDefault(uiMessage, uiParam, iParam);
	if(iDefault != HTCLIENT)
	{
		if(!::IsIconic(m_hWnd))
			return iDefault;
	}
	else if(IsKeyDown(KeyCodeF4))
		return HTCLIENT;
	else {}

	pointix point = MakePointFromInt(iParam);
	point = ScreenToClient(m_hWnd, point);
	HitTestE eResult = m_pControl->NcHitTest(point);
	switch(eResult)
	{
	case HitTestNoWhere:
		return HTNOWHERE;

	case HitTestInner:
	case HitTestClient:
		return HTCLIENT;

	case HitTestStatic:
		if(m_pControl->IsStatic())
			return HTCAPTION;
		else
			return HTCLIENT;

	case HitTestTransparent:
		if(m_pControl->IsStatic())
			return HTCAPTION;
		else
			return HTCLIENT;
		break;

	case HitTestL:
	case HitTestT:
	case HitTestR:
	case HitTestB:
	case HitTestLT:
	case HitTestTR:
	case HitTestRB:
	case HitTestBL:
		return HTCLIENT;

	case HitTestCaption: return HTCAPTION;
	case HitTestBorder: return HTCLIENT;
	case HitTestBorderL: return HTLEFT;
	case HitTestBorderT: return HTTOP;
	case HitTestBorderR: return HTRIGHT;
	case HitTestBorderB: return HTBOTTOM;
	case HitTestBorderLT: return HTTOPLEFT;
	case HitTestBorderTR: return HTTOPRIGHT;
	case HitTestBorderRB: return HTBOTTOMRIGHT;
	case HitTestBorderBL: return HTBOTTOMLEFT;
	case HitTestMinBox: return HTMINBUTTON;
	case HitTestClose: return HTCLOSE;

	default:
		return HTNOWHERE;
	}
}

int_x CWindow::OnWmClose(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	OnClose();
	return 0;
}

int_x CWindow::OnWmDestroy(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	return 0;
}

int_x CWindow::OnWmCaptureChanged(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	m_pControl->ToIForm()->NotifyCaptureChanged(m_hWnd == GetCapture());
	return 0;
}

int_x CWindow::OnWmMouseActive(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	pointix point = GetMousePosition();
	if(m_pControl->ToIForm()->MouseActiving(point))
		return MA_ACTIVATE;
	else
		return MA_NOACTIVATE;
}

int_x CWindow::OnWmNcCalcSize(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	if((m_uiStyle & WS_POPUP) && (uiParam || (m_uiStyleEx & WS_EX_LAYERED)))
	{
		NCCALCSIZE_PARAMS * pInfo = (NCCALCSIZE_PARAMS *)iParam;
		return 0;
	}
	else
		return OnDefault(uiMessage, uiParam, iParam);
}

int_x CWindow::OnWmNcMouseMove(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	return OnDefault(uiMessage, uiParam, iParam);
}

int_x CWindow::OnWmGetMinMaxInfo(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	MINMAXINFO * pInfo = (MINMAXINFO *)iParam;
	if(m_uiStyle & WS_POPUP)
	{
		// 必须在当前显示器的范围内。
		IMonitorManager * pmm = IMonitorManager::GetInstance();
		IMonitor * pm = pmm->GetMonitorFromWindowId((int_x)m_hWnd);
		rectix rcMonitor = pm->GetRect();
		rectix rcWork = pm->GetWorkRect();

		pInfo->ptMaxPosition = {(int_32)(rcWork.x - rcMonitor.x), (int_32)(rcWork.y - rcMonitor.y)};
		pInfo->ptMaxSize = {(int_32)rcWork.w, (int_32)rcWork.h};
	}

	edgeix margin = Window::GetMargin(m_hWnd);
	sizeix minSize = m_pControl->ToIForm()->GetMinSize();
	sizeix maxSize = m_pControl->ToIForm()->GetMaxSize();
	if(minSize.w > 0)
		pInfo->ptMinTrackSize.x = (int_32)(minSize.w + margin.width());
	if(minSize.h >= 0)
		pInfo->ptMinTrackSize.y = (int_32)(minSize.h + margin.height());
	if(maxSize.w > 0)
		pInfo->ptMaxTrackSize.x = (int_32)(maxSize.w + margin.width());
	if(maxSize.h > 0)
		pInfo->ptMaxTrackSize.y = (int_32)(maxSize.h + margin.height());
	return OnDefault(uiMessage, uiParam, iParam);
}

int_x CWindow::OnWmRefresh(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	if(m_iRefreshing)
	{
		if(m_rcRefresh.is_empty() || !IsWindowVisible(m_hWnd))
		{
			m_iRefreshing = 0;
			return 0;
		}

		m_pControl->ToIForm()->OnRefresh(m_rcRefresh);
		m_rcRefresh.set_empty();
		m_iRefreshing = 0;
		return 0;
	}
	else
		return OnDefault(uiMessage, uiParam, iParam);
}

int_x CWindow::OnWmSysCommand(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	return OnDefault(uiMessage, uiParam, iParam);
}

int_x CWindow::OnWmNotify(uint_32 uiMessage, uint_x uiParam, int_x iParam)
{
	int_x iID = (int_x)uiParam;
	NMHDR * pNMHDR = (NMHDR *)iParam;
	//if(pNMHDR)
	//{
	//	IWindow * pWndHandle = SearchWindow(pNMHDR->hwndFrom);
	//	int_x iReturn = 0;
	//	if(pWndHandle && pWndHandle->DoNotify(pNMHDR->code, (void *)pNMHDR, iReturn))
	//		return iReturn;
	//	else {}
	//}
	//else {}
	return OnDefault(uiMessage, uiParam, iParam);
}

void CWindow::UpdateIme()
{
	IControl * pControl = m_pControl->GetFocusControl();
	if(pControl)
	{
		rectix rcClient = m_pControl->GetClient();
		pointix point = pControl->GetRelativePosition(m_pControl);
		ImeInfoT info;
		if(pControl->GetImeInfo(info))
		{
			m_inputContext.SetImeMode(info.eImeMode);
			m_inputContext.SetCompositionWindow(point.x + info.rcCompose.x, point.y + info.rcCompose.y);
		}
		else
		{
			m_inputContext.SetImeMode(ImeModeDisable);
		}
	}
	else
	{
		m_inputContext.SetImeMode(ImeModeDisable);
	}
}

bool CWindow::ClipboardCheckData(ClipboardDataTypeE eDataType) const
{
	DWORD uiFormat = 0;
	switch(eDataType)
	{
	case ClipboardDataTypeText8:
		uiFormat = CF_TEXT;
		break;
	case ClipboardDataTypeText16:
		uiFormat = CF_UNICODETEXT;
		break;
	default:
		break;
	}
	if(!uiFormat)
		return false;
	else
	{
		return IsClipboardFormatAvailable(uiFormat) != FALSE;
	}
}

textw CWindow::ClipboardGetText16() const
{
	m_txtClipboard.resize(0);
	if(OpenClipboard(NULL))
	{
		HGLOBAL hMemory = GetClipboardData(CF_UNICODETEXT);
		if(hMemory)
		{
			m_txtClipboard.set((const char_16 *)GlobalLock(hMemory), -1);
			GlobalUnlock(hMemory);
		}

		CloseClipboard();
	}
	return m_txtClipboard;
}

void CWindow::ClipboardSetText16(const char_16 * szText, int_x iLength) const
{
	if(!m_hWnd)
		return;

	if(iLength < 0)
		iLength = textlen(szText);

	int_x iSize = (iLength + 1) << 1;
	HGLOBAL hMemory = GlobalAlloc(0, iSize);
	char_16 * pCpText = (char_16 *)GlobalLock(hMemory);
	textcpy(pCpText, iLength + 1, szText, iLength);
	GlobalUnlock(hMemory);

	if(OpenClipboard(m_hWnd) && EmptyClipboard())
	{
		SetClipboardData(CF_UNICODETEXT, hMemory);
		CloseClipboard();
	}
}

const char_16 * CWindow::GetClass() const
{
	return WINDOW_CLASS_NAME;
}

void CWindow::SetPosition(const pointix & position)
{
	if(m_hWnd && position != m_pControl->GetPosition())
	{
		SetWindowPos(m_hWnd, NULL, (int_32)position.x, (int_32)position.y, 0, 0,
			SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
	}
	else
		m_pControl->ToIForm()->NotifyPositionChanged(position);
}

void CWindow::SetSize(const sizeix & size)
{
	if(m_hWnd && size != m_pControl->GetSize())
	{
		SetWindowPos(m_hWnd, NULL, 0, 0, (int_32)size.w, (int_32)size.h,
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
	}
	else
		m_pControl->ToIForm()->NotifySizeChanged(size, ShowModeNormal);
}

void CWindow::Active(bool bActive)
{
	if(m_hWnd)
	{
		if(bActive)
			::SetForegroundWindow(m_hWnd);
		else if(GetActiveWindow() == m_hWnd)
			::SetForegroundWindow(NULL);
	}
}

void CWindow::SetText(const char_16 * szText, int_x iLength)
{
	if(m_hWnd)
		::SetWindowTextW(m_hWnd, szText);
	//else
}

void CWindow::SetBoxes(bool bMinBox, bool bMaxBox, bool bCloseBox)
{
	ModifyStyle(WS_MINIMIZEBOX, bMinBox);
	ModifyStyle(WS_MAXIMIZEBOX, bMaxBox);
}

bool CWindow::Capture(bool bCapture)
{
	if(!m_hWnd)
		return false;
	if(bCapture)
		::SetCapture(m_hWnd);
	else
		::ReleaseCapture();
	return true;
}

void CWindow::ConfirmHandle() const
{
	if(!m_hWnd)
		throw exp_bad_state();
}

static
LRESULT CALLBACK WindowWndProc(HWND hWnd, UINT uiMessage, WPARAM uiParam, LPARAM iParam)
{
	if(uiMessage == WM_NCCREATE)
	{
		CREATESTRUCT * pcs = (CREATESTRUCT *)iParam;
		if(pcs->lpCreateParams)
		{
			((IWindow *)pcs->lpCreateParams)->SetHandle(hWnd);
		}
	}

	return DefWindowProcW(hWnd, uiMessage, uiParam, iParam);
}

void CWindow::CreateHandle(const char_16 * szClassName, const char_16 * szText,
	uint_32 uiStyle, uint_32 uiStyleEx,
	int_x iX, int_x iY, int_x iWidth, int_x iHeight,
	HWND hWndParent, HMENU hMenu)
{
	if(m_pControl->ToIForm()->GetFormType() == FormTypeDefault)
	{
		// win 10...
		int_x icxBorder = ::GetSystemMetrics(SM_CXFRAME) + ::GetSystemMetrics(SM_CXFRAME);
		int_x icyBorder = ::GetSystemMetrics(SM_CYFRAME) + ::GetSystemMetrics(SM_CYFRAME);
		int_x icyCaption = ::GetSystemMetrics(SM_CYCAPTION);
		iX -= icxBorder;
		iY -= icyBorder;
		iWidth += icxBorder * 2;
		iHeight += icyBorder * 2;
		iY -= icyCaption;
		iHeight += icyCaption;
	}

	HINSTANCE hInstance = Win32::GetCurrentInstance();
	WNDCLASSEXW wcex = {sizeof(WNDCLASSEXW)};
	if(!GetClassInfoExW(hInstance, szClassName, &wcex))
	{
		wcex.cbSize = sizeof(WNDCLASSEXW);
		wcex.style = 0;
		wcex.lpfnWndProc = WindowWndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = NULL;
		wcex.hCursor = /*::LoadCursor(NULL, IDC_ARROW)*/NULL;
		wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szClassName;
		wcex.hIconSm = NULL;

		RegisterClassExW(&wcex);
	}
	else
	{
	}

	CreateWindowExW(
		uiStyleEx, szClassName, szText, uiStyle,
		(int_32)iX, (int_32)iY, (int_32)iWidth, (int_32)iHeight,
		hWndParent, hMenu, hInstance, (void *)(IWindow *)this);

	if(!m_hWnd)
	{
		log0(L"Create window failed! %s", Win32::FormatWinError());
		throw exp_base(_T("无法创建窗口"));
	}

	SetIcon(m_hIcon);
	SetIconBig(m_hIconBig);

	OnCreate();
}

void CWindow::DestroyHandle()
{
	if(m_hWnd)
	{
		::DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}
}

HWND CWindow::GetOwnerHandle()
{
	if(m_hWndParent)
		return m_hWndParent;
	else
	{
		IForm * pForm = m_pControl->ToIForm();
		IForm * pOwner = pForm ? pForm->GetOwner() : nullptr;
		if(pOwner)
			return (HWND)pOwner->GetFormId();
		else
			return NULL;
	}
}

void CWindow::UpdateLayered(rectix rect)
{

}

void CWindow::Create()
{
	rectix rect = m_pControl->GetRect();
	CreateHandle(GetClass(), m_pControl->GetText(), m_uiStyle, m_uiStyleEx,
		rect.x, rect.y, rect.w, rect.h,
		GetOwnerHandle(), NULL);
}

void CWindow::OnCreate()
{
	m_inputContext.SetHWND(m_hWnd);
	m_inputContext.SetImeMode(m_pControl->GetImeMode());
	if(m_pControl)
		m_pControl->ToIForm()->OnCreate();
}

void CWindow::Close()
{
	PostMSG(WM_CLOSE, 0, 0);
}

int_x CWindow::OnClose()
{
	m_pControl->ToIForm()->OnClose();
	return 0;
}

void CWindow::Destroy()
{
	OnDestroy();
	DestroyHandle();
}

int_x CWindow::OnDestroy()
{
	m_inputContext.SetHWND(NULL);
	m_pControl->ToIForm()->OnDestroy();
	Window::Detatch(m_hWnd);
	SafeRelease(m_pBuffer);
	SafeRelease(m_pPaint);
	return 0;
}

void CWindow::SetHostInitPos(HostInitPosE eInitPos)
{
	m_eInitPos = eInitPos;
}

HostInitPosE CWindow::GetHostInitPos() const
{
	return m_eInitPos;
}

pointix CWindow::ClientToScreen(const pointix & point) const
{
	if(m_hWnd)
		return ClientToScreen(m_hWnd, point);
	else
		return point;
}

sizeix CWindow::MakeSizeFromInt(int_x iParam)
{
#ifdef BIT64
	int_32 low = i64li32(iParam);
	return sizeix(i32li16(low), i32hi16(low));
#else
	return sizeix(i32li16(iParam), i32hi16(iParam));
#endif
}

pointix CWindow::MakePointFromInt(int_x iParam)
{
#ifdef BIT64
	int_32 low = i64li32(iParam);
	return pointix(i32li16(low), i32hi16(low));
#else
	return pointix(i32li16(iParam), i32hi16(iParam));
#endif
}

sizeix CWindow::MakeSizeFromUInt(uint_x uiParam)
{
	return pointix(uxlih(uiParam), uxhih(uiParam));
}

pointix CWindow::MakePointFromUInt(int_x uiParam)
{
#ifdef BIT64
	uint_32 low = i64lu32(uiParam);
	return sizeix(u32li16(low), u32hi16(low));
#else
	return sizeix(u32li16(uiParam), u32hi16(uiParam));
#endif
}

pointix CWindow::ScreenToClient(HWND hWnd, const pointix & point)
{
	POINT pt = {(int_32)point.x, (int_32)point.y};
	::ScreenToClient(hWnd, &pt);
	return pointix(pt.x, pt.y);
}

pointix CWindow::ClientToScreen(HWND hWnd, const pointix & point)
{
	POINT pt = {(int_32)point.x, (int_32)point.y};
	::ClientToScreen(hWnd, &pt);
	return pointix(pt.x, pt.y);
}

bool CWindow::IsClickPoint(const pointix & pointA, const pointix & pointB)
{
	int_x iOffX = (int_x)::GetSystemMetrics(SM_CXDOUBLECLK);
	int_x iOffY = (int_x)::GetSystemMetrics(SM_CYDOUBLECLK);
	return abs(pointA.x - pointB.x) < iOffX && abs(pointA.y - pointB.y) < iOffY;
}

bool CWindow::IsChildOf(HWND hWnd, HWND hWndParent)
{
	if(!hWndParent || !hWnd)
		return false;

	hWnd = ::GetParent(hWnd);
	while(hWnd)
	{
		if(hWnd == hWndParent)
			return true;
		else
			hWnd = ::GetParent(hWnd);
	}
	return false;
}

VENUS_END
