#pragma once

VENUS_BEG

enum WindowMessage
{
	WM_REFRESH = WM_USER,
	//WM_ENDMODAL,
};

const char_16 WINDOW_CLASS_NAME[] = L"[Default:29a0f390-f5ab-4617-9ac0-c41fe276fb64]";
const char_16 WINDOW_PROP_THIS_PTR[] = L"C8C8BD2D-46A7-4DFB-BB5D-EE6A25E83368";
const char_16 WINDOW_PROP_DLG_RESULT[] = L"99358F8F-E19F-4337-8192-A088F1CA41D1";
const char_16 WINDOW_PROP_OLD_WNDPROC[] = L"FCAC0730-350D-4D79-B28E-D137C22648EA";

enum PreMessageResultE
{
	PreMessageResultOk,
	PreMessageResultIgnore,
	PreMessageResultNotTranslate,
};

const oid_t OID_IWindow = {L"IWindow"};
class WIN32_API IWindow : public IObject
{
public:
	virtual ~IWindow() {}
	virtual void SetHandle(HWND hWnd) = 0;
	virtual HWND GetHandle() const = 0;
	virtual PreMessageResultE PreHandleMSG(uint_32 uiMessage, uint_x & uiParam, int_x & iParam) = 0;
	virtual void AfterHandleMSG(uint_32 uiMessage, uint_x uiParam, int_x iParam) = 0;
	virtual int_x HandleMSG(uint_32 uiMessage, uint_x uiParam, int_x iParam) = 0;
	virtual int_x SendMSG(uint_32 uiMessage, uint_x uiParam, int_x iParam) const = 0;
	virtual bool PostMSG(uint_32 uiMessage, uint_x uiParam, int_x iParam) const = 0;
	virtual int_x OnDefault(uint_32 uiMessage, uint_x uiParam, int_x iParam) = 0;
	virtual event<uint_32, uint_x, int_x> & WmMessageEvent() = 0;

	virtual void SetIcon(HICON hIcon) = 0;
	virtual void SetIconBig(HICON hIcon) = 0;
	virtual void SetParentHandle(HWND hWndParent) = 0;
};


struct WIN32_API winfo_t
{
	rectix window;
	rectix client;
	sizeix border;
	edgeix margin;
};

class WIN32_API Window
{
public:
	Window() = delete;
	~Window() = delete;

	/*获取窗口所在的屏幕矩形*/
	static rectix GetRect(HWND hWnd);
	/*获取客户区所在的屏幕矩形*/
	static rectix GetClient(HWND hWnd);
	/*获取窗口的一些信息*/
	static winfo_t GetInfo(HWND hWnd);
	/*获取窗口的非客户区大小*/
	static edgeix GetMargin(HWND hWnd);
	/**
	* 获取 hWnd 所在的顶层窗口，如果 hWnd 就是一个顶层窗口，则返回 hWnd 本身。
	*/
	static HWND GetTop(HWND hWnd);
	/**
	* 获取 hWnd 所在顶层窗口的所属窗口。
	*/
	static HWND GetOwner(HWND hWnd);

	static bool IsChildOf(HWND hWnd, HWND hWndParent);


	/**
	* 在 dll 中，调用 RegisterClassExW 之后，如果 dll 卸载，会导致 WndProc 停留在
	* 被卸载的模块中，下一次 CraeteWindow 会出错。
	*/
	static void PreRegisterClass();
	static void PreUnRegisterClass();
	/**
	* 获取 hWnd 的默认窗口过程，获取的默认窗口过程是 AttatchWindow 替换之前的窗口过程，
	* 如果未对窗口调用 AttatchWindow，则直接返回 DefWndProc。
	*/
	static WNDPROC GetDefaultWndProc(HWND hWnd);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM uiParam, LPARAM iParam);

	static bool Attatch(HWND hWnd, IWindow * pWnd);
	/**
	* @brief 取消指定的窗口 hWnd 与 IWindow * 的关联。
	* 并销毁与该关联相关的一些内部属性，包括 this指针、DialogResult属性、
	* 用于取消关联的(Detatch)的旧的窗口函数等。
	* 该函数会将窗口函数替换为调用 AttatchWindow 之前的窗口函数。
	*/
	static void Detatch(HWND hWnd);
	/**
	 * 查找与 hWnd 关联的 IWindow。
	 */
	static IWindow * Search(HWND hWnd);

	static int_x SendMSG(HWND hWnd, uint_32 uiMessage, uint_x uiParam, int_x iParam);
	static bool PostMSG(HWND hWnd, uint_32 uiMessage, uint_x uiParam, int_x iParam);
	/**
	 * 所有的消息，最终都应该交由 HandleMSG 来处理，而不是自行调用 DispatchMessage。
	 */
	static void HandleMSG(MSG & msg);

	/**!@brief 清空消息队列。。*/
	static void ClearMSG();

	static bool SetDialogResult(HWND hWnd, int_x iResult);
	static int_x GetDialogResult(HWND hWnd);

	static pointix GetMousePos(HWND hWnd);

	static const char_x * GetWmMessageText(uint_32 uiMessage);
	static KeyCodeE VirtualKeyToKeyCode(int_x iVirtualKey);
	static int_x KeyCodeToVirtualKey(KeyCodeE eKeyCode);
};

VENUS_END
