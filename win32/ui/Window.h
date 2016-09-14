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

	/*��ȡ�������ڵ���Ļ����*/
	static rectix GetRect(HWND hWnd);
	/*��ȡ�ͻ������ڵ���Ļ����*/
	static rectix GetClient(HWND hWnd);
	/*��ȡ���ڵ�һЩ��Ϣ*/
	static winfo_t GetInfo(HWND hWnd);
	/*��ȡ���ڵķǿͻ�����С*/
	static edgeix GetMargin(HWND hWnd);
	/**
	* ��ȡ hWnd ���ڵĶ��㴰�ڣ���� hWnd ����һ�����㴰�ڣ��򷵻� hWnd ����
	*/
	static HWND GetTop(HWND hWnd);
	/**
	* ��ȡ hWnd ���ڶ��㴰�ڵ��������ڡ�
	*/
	static HWND GetOwner(HWND hWnd);

	static bool IsChildOf(HWND hWnd, HWND hWndParent);


	/**
	* �� dll �У����� RegisterClassExW ֮����� dll ж�أ��ᵼ�� WndProc ͣ����
	* ��ж�ص�ģ���У���һ�� CraeteWindow �����
	*/
	static void PreRegisterClass();
	static void PreUnRegisterClass();
	/**
	* ��ȡ hWnd ��Ĭ�ϴ��ڹ��̣���ȡ��Ĭ�ϴ��ڹ����� AttatchWindow �滻֮ǰ�Ĵ��ڹ��̣�
	* ���δ�Դ��ڵ��� AttatchWindow����ֱ�ӷ��� DefWndProc��
	*/
	static WNDPROC GetDefaultWndProc(HWND hWnd);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM uiParam, LPARAM iParam);

	static bool Attatch(HWND hWnd, IWindow * pWnd);
	/**
	* @brief ȡ��ָ���Ĵ��� hWnd �� IWindow * �Ĺ�����
	* ��������ù�����ص�һЩ�ڲ����ԣ����� thisָ�롢DialogResult���ԡ�
	* ����ȡ��������(Detatch)�ľɵĴ��ں����ȡ�
	* �ú����Ὣ���ں����滻Ϊ���� AttatchWindow ֮ǰ�Ĵ��ں�����
	*/
	static void Detatch(HWND hWnd);
	/**
	 * ������ hWnd ������ IWindow��
	 */
	static IWindow * Search(HWND hWnd);

	static int_x SendMSG(HWND hWnd, uint_32 uiMessage, uint_x uiParam, int_x iParam);
	static bool PostMSG(HWND hWnd, uint_32 uiMessage, uint_x uiParam, int_x iParam);
	/**
	 * ���е���Ϣ�����ն�Ӧ�ý��� HandleMSG ���������������е��� DispatchMessage��
	 */
	static void HandleMSG(MSG & msg);

	/**!@brief �����Ϣ���С���*/
	static void ClearMSG();

	static bool SetDialogResult(HWND hWnd, int_x iResult);
	static int_x GetDialogResult(HWND hWnd);

	static pointix GetMousePos(HWND hWnd);

	static const char_x * GetWmMessageText(uint_32 uiMessage);
	static KeyCodeE VirtualKeyToKeyCode(int_x iVirtualKey);
	static int_x KeyCodeToVirtualKey(KeyCodeE eKeyCode);
};

VENUS_END
