#include "stdafx.h"
#include "CWin32App.h"

VENUS_BEG

CWin32App::CWin32App() :
m_iTimerId(0)
{
	SetApp(this);
	Initialize();
}

CWin32App::~CWin32App()
{
	UnInitialize();
	SetApp(nullptr);
}

err_t CWin32App::Initialize()
{
	IStdOut * pStdOut = new CSimpleStdOut();
	SetService(OID_IStdOut, pStdOut);
	SafeRelease(pStdOut);

	IMonitorManager * pMonitorManager = new CMonitorManager();
	SetService(OID_IMonitorManager, pMonitorManager);
	SafeRelease(pMonitorManager);

	IUILoader * pUILoader = new CWin32UILoaderXml();
	SetService(OID_IUILoader, pUILoader);
	SafeRelease(pUILoader);
	return err_ok;
}


IObject * CWin32App::CreateObject(const oid_t & oid, void * pParam)
{
	if(oid == OID_IHost)
	{
		CWindow * pWindow = new CWindow();
		return (IObject *)(IHost *)pWindow;
	}
	return nullptr;
}

bool CWin32App::RegisterObject(const oid_t & oid, function<IObject *(void *)> fnCreate, bool bReplace)
{
	for(int_x cnt = 0, size = m_objectInfos.size(); cnt < size; ++cnt)
	{
		ObjectInfoT & info = m_objectInfos[cnt];
		if(info.oid == oid)
		{
			if(bReplace)
			{
				info.fnCreate = fnCreate;
				return true;
			}
			else
				return false;
		}
	}
	ObjectInfoT newInfo = {oid, fnCreate};
	m_objectInfos.add(newInfo);
	return true;
}

bool CWin32App::UnregisterObject(const oid_t & oid)
{
	int_x iIndex = -1;
	for(int_x cnt = 0, size = m_objectInfos.size(); cnt < size; ++cnt)
	{
		ObjectInfoT & info = m_objectInfos[cnt];
		if(info.oid == oid)
		{
			iIndex = cnt;
			break;
		}
	}
	if(iIndex >= 0)
	{
		m_objectInfos.remove_at(iIndex);
		return true;
	}
	else
		return false;
}

void CWin32App::UnInitialize()
{
	if(m_iTimerId)
	{
		::KillTimer(NULL, m_iTimerId);
		m_iTimerId = 0;
	}
	m_timers.clear();

	for(int_x cnt = 0, len = m_services.size(); cnt < len; ++cnt)
		SafeRelease(m_services[cnt].pService);
	m_services.clear();
}

void CWin32App::SetService(const oid_t & oid, IService * pService)
{
	for(int_x cnt = 0, len = m_services.size(); cnt < len; ++cnt)
	{
		ServiceT & service = m_services[cnt];
		if(service.oid == oid)
		{
			SafeRelease(service.pService);
			service.pService = pService;
			SafeAddRef(pService);
			return;
		}
	}

	ServiceT service = {oid, pService};
	m_services.add(service);
	SafeAddRef(pService);
}

IService * CWin32App::GetService(const oid_t & oid) const
{
	for(int_x cnt = 0, len = m_services.size(); cnt < len; ++cnt)
	{
		const ServiceT & service = m_services[cnt];
		if(service.oid == oid)
			return service.pService;
	}
	if(oid == OID_CSHFileInfoBuffer)
	{
		CSHFileInfoBuffer * psh = new CSHFileInfoBuffer();
		((CWin32App *)this)->SetService(oid, psh);
		psh->Release();
		return psh;
	}
	return nullptr;
}

static void __stdcall UpdateViewTimerFunc(HWND hWnd, UINT/* = uiMessage*/, UINT_PTR uiTimerID, DWORD/* = dwWindowRunTime*/)
{
	CWin32App * pThis = (CWin32App *)GetApp();
	if(pThis)
		pThis->OnTimerProc();
}

void CWin32App::SetTimer(function<int_x(int_x)> fun, int_x iPeriod, int_x iId)
{
	if(!m_iTimerId)
		m_iTimerId = (int_x)::SetTimer(NULL, 0, 100, UpdateViewTimerFunc);

	iPeriod /= 100;
	iPeriod *= 100;

	TimerT timer = {fun, iPeriod, iId, 0};
	m_timers.add(timer);
}

void CWin32App::KillTimer(function<int_x(int_x)> fun, int_x iId)
{
	TimerT timer = {fun, 0, iId, 0};
	if(m_timers.remove(timer) && m_timers.is_empty() && m_iTimerId)
	{
		::KillTimer(NULL, m_iTimerId);
		m_iTimerId = 0;
	}
}

void CWin32App::Create2DDevice(Device2DTypeE eType)
{
	I2DDevice * pDevice = Win32::Create2DDevice(eType);
	SetService(OID_IDevice2D, pDevice);
	SafeRelease(pDevice);
}

int_x CWin32App::OnRun()
{
	return 0;
}

err_t CWin32App::Run(int_x iFormId, AppRunE eAppRun)
{
	switch(eAppRun)
	{
	case AppRunLoop:
		return RunLoop(iFormId);
	case AppRunDialog:
		return RunDialog(iFormId);
	case AppRunPopup:
		return RunPopup(iFormId);
	default:
		break;
	}
	return err_ok;
}

int_x CWin32App::EndRun(int_x iFormId, int_x iResult)
{
	if(iFormId)
	{
		Window::SetDialogResult((HWND)iFormId, iResult);
		return err_ok;
	}
	else
		return err_invalidarg;
}

int_x CWin32App::RunLoop(int_x iFormId)
{
	HWND hWnd = (HWND)iFormId;

	MSG msg = {0};
	bool bRunning = true;
	int_x iRet = 0;
	while(bRunning)
	{
		if(OnRun())
		{
			while(::PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if(msg.message == WM_QUIT)
				{
					iRet = (int_x)msg.wParam;
					bRunning = false;
					break;
				}
				else
					Window::HandleMSG(msg);
			}
		}
		else
		{
			int iGetResult = ::GetMessageW(&msg, NULL, 0, 0);
			switch(iGetResult)
			{
			case -1:
				iRet = -1;
				bRunning = false;
				break;
			case 0:
				iRet = (int_x)msg.wParam;
				bRunning = false;
				break;
			default:
				Window::HandleMSG(msg);
				break;
			}
		}
	}
	return iRet;
}

int_x CWin32App::RunDialog(int_x iFormId)
{
	HWND hWnd = (HWND)iFormId;
	if(!hWnd)
		return DialogResultNone;

	bool bRunning = true;
	MSG msg = {0};
	HWND hWndParent = Window::GetOwner(hWnd);
	bool bDisableParent = false;
	if(hWndParent && hWndParent != GetDesktopWindow() && IsWindowEnabled(hWndParent))
	{
		EnableWindow(hWndParent, FALSE);
		bDisableParent = true;
	}

	DialogResultE eResult = DialogResultNone;
	Window::SetDialogResult(hWnd, DialogResultInvalid);
	while(bRunning && IsWindowVisible(hWnd))
	{
		eResult = (DialogResultE)Window::GetDialogResult(hWnd);
		if(eResult != DialogResultInvalid)
			break;

		if(OnRun())
		{
			while(::PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if(msg.message == WM_QUIT)
				{
					bRunning = false;
					break;
				}
				else
					Window::HandleMSG(msg);
			}
		}
		else
		{
			int iGetResult = ::GetMessageW(&msg, NULL, 0, 0);
			switch(iGetResult)
			{
			case -1:
				bRunning = false;
				break;
			case 0:
				bRunning = false;
				break;
			default:
				Window::HandleMSG(msg);
				break;
			}
		}
	}

	/**
	 * 必须先启用 hWndParent，再 Hide hWnd。
	 */
	if(bDisableParent)
		EnableWindow(hWndParent, TRUE);

	::ShowWindow(hWnd, SW_HIDE);

	if(hWndParent)
		::SetActiveWindow(hWndParent);

	// clear message.
	Window::ClearMSG();

	return eResult;
}

int_x CWin32App::RunPopup(int_x iFormId)
{
	HWND hWnd = (HWND)iFormId;
	if(!hWnd)
		return DialogResultNone;

	MSG msg = {0};
	Window::ClearMSG();

	enum MsgDealE
	{
		// 忽略这个消息，但不从消息队列中移除，以便返回上一层消息循环时处理。
		MsgDealIgnore,
		// 移除这个消息。
		MsgDealRemove,
		// 按照固定的消息映射处理这个消息。
		MsgDealHandle,
		// 将消息映射到弹出的窗口。
		MsgDealReflact,
	};

	HWND hWndOwner = Window::GetOwner(hWnd);
	HWND hWndForeground = GetForegroundWindow();
	DialogResultE eResult = DialogResultNone;
	Window::SetDialogResult(hWnd, DialogResultInvalid);
	if(!hWndOwner || hWndOwner != hWndForeground)
	{
		EndRun(iFormId, DialogResultNone);
	}
	else
	{
		MsgDealE eDeal = MsgDealHandle;
		while(IsWindowVisible(hWnd))
		{
			eResult = (DialogResultE)Window::GetDialogResult(hWnd);
			if(eResult != DialogResultInvalid)
				break;

			//HWND hForeWindow = ::GetForegroundWindow();
			HWND hForeWindow = ::GetActiveWindow();
			if(hForeWindow != hWndOwner && hForeWindow != hWnd)
			{
				EndRun(iFormId, DialogResultNone);
			}
			else if(PeekMessageW(&msg, NULL, 0, 0, PM_NOREMOVE))
			{
				eDeal = MsgDealHandle;
				switch(msg.message)
				{
					//case WM_ENDMODAL:
					//	EndRun(iFormId, msg.lParam);
					//	eDeal = MsgDealRemove;
					//	break;
				case WM_NCLBUTTONDOWN:
				case WM_NCRBUTTONDOWN:
				case WM_NCMBUTTONDOWN:
					EndRun(iFormId, DialogResultNone);
					eDeal = MsgDealIgnore;
					break;
				case WM_LBUTTONDOWN:
				case WM_MBUTTONDOWN:
				case WM_RBUTTONDOWN:
				case WM_XBUTTONDOWN:
					if(msg.hwnd != hWnd && !Window::IsChildOf(msg.hwnd, hWnd))
					{
						EndRun(iFormId, DialogResultNone);
						Window::PostMSG(msg.hwnd, WM_MOUSEMOVE, (uint_x)msg.wParam, (int_x)msg.lParam);
						eDeal = MsgDealIgnore;
					}
					break;
				case WM_MOUSEWHEEL:
#if _WIN32_WINNT >= _WIN32_WINNT_WIN6
				case WM_MOUSEHWHEEL:
#endif
					eDeal = MsgDealReflact;
					break;
				case WM_SYSKEYDOWN:
				case WM_SYSKEYUP:
				case WM_KEYDOWN:
				case WM_KEYUP:
				case WM_CHAR:
					if(msg.hwnd == hWndOwner)
						eDeal = MsgDealReflact;
					break;
				case WM_MOUSEMOVE:
					eDeal = MsgDealHandle;
					break;
				default:
					break;
				}

				switch(eDeal)
				{
				case MsgDealIgnore:
					break;
				case MsgDealRemove:
					PeekMessageW(&msg, msg.hwnd, msg.message, msg.message, PM_REMOVE);
					break;
				case MsgDealHandle:
					PeekMessageW(&msg, msg.hwnd, msg.message, msg.message, PM_REMOVE);
					Window::HandleMSG(msg);
					break;
				case MsgDealReflact:
					PeekMessageW(&msg, msg.hwnd, msg.message, msg.message, PM_REMOVE);
					msg.hwnd = hWnd;
					Window::HandleMSG(msg);
					break;
				default:
					verify(false);
					break;
				}
			}
			else
			{
				if(!OnRun())
					MsgWaitForMultipleObjectsEx(0, NULL, 1, QS_ALLINPUT, 0);
				//Sleep(100);
			}
		}
	}

	::ShowWindow(hWnd, SW_HIDE);

	// 清空跟此窗口相关的消息。
	while(PeekMessageW(&msg, hWnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if(hWndOwner)
		BringWindowToTop(hWndOwner);

	return eResult;
}
void CWin32App::OnTimerProc()
{
	for(int_x cnt = 0, len = m_timers.size(); cnt < len; ++cnt)
	{
		TimerT & timer = m_timers[cnt];
		timer.iTime += 100;
		if(timer.iTime % timer.iPeriod == 0)
			timer.fun(timer.iTime);
	}
}

VENUS_END
