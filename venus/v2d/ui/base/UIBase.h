#pragma once

VENUS_BEG

/**
* ��갴ť��
*/
enum MouseButtonE
{
	MouseButtonNone = 0,
	MouseButtonL = 1,
	MouseButtonR = 2,
	MouseButtonM = 3,
	MouseButtonBack = 4,
	MouseButtonForward = 5,
};

/**
* ���̰�����
*/
enum KeyCodeE
{
	//! �����õİ����롣
	KeyCodeInvalid = 0x0,

	// ���ּ�
	KeyCode0,
	KeyCode1,
	KeyCode2,
	KeyCode3,
	KeyCode4,
	KeyCode5,
	KeyCode6,
	KeyCode7,
	KeyCode8,
	KeyCode9,
	// �ַ���
	KeyCodeCapsLock,
	KeyCodeA,
	KeyCodeB,
	KeyCodeC,
	KeyCodeD,
	KeyCodeEe,
	KeyCodeF,
	KeyCodeG,
	KeyCodeH,
	KeyCodeI,
	KeyCodeJ,
	KeyCodeK,
	KeyCodeL,
	KeyCodeM,
	KeyCodeN,
	KeyCodeO,
	KeyCodeP,
	KeyCodeQ,
	KeyCodeR,
	KeyCodeS,
	KeyCodeT,
	KeyCodeU,
	KeyCodeV,
	KeyCodeW,
	KeyCodeX,
	KeyCodeY,
	KeyCodeZ,

	KeyCodeBackspace,
	KeyCodeTab,
	KeyCodeEnter,

	KeyCodeEscape,
	KeyCodeSpace,

	// С����
	KeyCodeNumlock,
	KeyCodeNum0,
	KeyCodeNum1,
	KeyCodeNum2,
	KeyCodeNum3,
	KeyCodeNum4,
	KeyCodeNum5,
	KeyCodeNum6,
	KeyCodeNum7,
	KeyCodeNum8,
	KeyCodeNum9,
	KeyCodeNumAdd, // +
	KeyCodeNumSub, // -
	KeyCodeNumMul, // *
	KeyCodeNumDiv,  // /
	KeyCodeNumDecimal, // .
	KeyCodeNumEnter, // enter
	KeyCodeNumEqual, // equal

	// ���ܼ�
	KeyCodeF1,
	KeyCodeF2,
	KeyCodeF3,
	KeyCodeF4,
	KeyCodeF5,
	KeyCodeF6,
	KeyCodeF7,
	KeyCodeF8,
	KeyCodeF9,
	KeyCodeF10,
	KeyCodeF11,
	KeyCodeF12,
	KeyCodeF13,
	KeyCodeF14,
	KeyCodeF15,
	KeyCodeF16,
	KeyCodeF17,
	KeyCodeF18,
	KeyCodeF19,
	KeyCodeF20,
	KeyCodeF21,
	KeyCodeF22,
	KeyCodeF23,
	KeyCodeF24,

	// --  Sign
	KeyCodeSub, // ���� -_
	KeyCodeEqual, // ���� =+
	KeyCodeGrave, // ������ `~
	KeyCodeComma, // ���� ��<
	KeyCodePeriod, // ��� .>
	KeyCodeSlash, // б�� /?
	KeyCodeSemicolon, // �ֺ� ;:
	KeyCodeColon, // ð�� : (ĳЩ�����е�����ð��)
	KeyCodeApostrophe, // �����š�ʡ�ַ� '
	KeyCodeBracketL, // ������ [{
	KeyCodeBracketR, // �ҷ����� ]}
	KeyCodeBackslash, // ��б�� \|

	KeyCodeInsert,
	KeyCodeDelete,
	KeyCodeHome,
	KeyCodeEnd,
	KeyCodePageUp,
	KeyCodePageDown,

	KeyCodeLeft,
	KeyCodeUp,
	KeyCodeRight,
	KeyCodeDown,

	KeyCodePrintScreen,
	KeyCodeScrollLock,
	KeyCodePauseBreak,

	// -- Win
	KeyCodeWinL,
	KeyCodeWinR,
	// -- Ctrl
	KeyCodeControl,
	KeyCodeControlL,
	KeyCodeControlR,
	// -- Shift
	KeyCodeShift,
	KeyCodeShiftL,
	KeyCodeShiftR,
	// -- Alt
	KeyCodeAlt,
	KeyCodeAltL,
	KeyCodeAltR,
	// -- App
	KeyCodeApps,

	// �����
	KeyCodeSleep,

	KeyCodeClear,
	KeyCodeSelect,
	KeyCodePrint,
	KeyCodeExcute,
	KeyCodeHelp,

	// -- IME
	KeyCodeImeKana,
	KeyCodeImeHangul,
	KeyCodeImeJunja,
	KeyCodeImeFinal,
	KeyCodeImeHanja,
	KeyCodeImeKanji,
	KeyCodeImeConvert,
	KeyCodeImeNonConvert,
	KeyCodeImeAccept,
	KeyCodeImeModeChange,
	KeyCodeImeProcessKey,

	// -- Browser
	KeyCodeBrowserBack,
	KeyCodeBrowserForward,
	KeyCodeBrowserRefresh,
	KeyCodeBrowserStop,
	KeyCodeBrowserSearch,
	KeyCodeBrowserFavorites,
	KeyCodeBrowserHome,

	// -- Volume
	//! ����
	KeyCodeVolumeMute,
	KeyCodeVolumeDown,
	KeyCodeVolumeUp,

	// -- Media
	KeyCodeMediaNext,
	KeyCodeMediaPrev,
	KeyCodeMediaStop,
	KeyCodeMediaPlayPause,

	// -- Launch
	KeyCodeLaunchMail,
	KeyCodeLaunchMediaSelect,
	KeyCodeLaunchApp1,
	KeyCodeLaunchApp2,

	// -- ����
	KeyCodeElseAttn,
	KeyCodeElseArsel,
	KeyCodeElseExsel,
	KeyCodeElseEreof,
	KeyCodeElsePlay,
	KeyCodeElseZoom,
	KeyCodeElsePa1,
	KeyCodeElseClear,

	KeyCodeCount,
};

/**
* ���ڵ���ʾ��ʽ��
*/
enum ShowModeE
{
	ShowModeNormal,
	ShowModeMinisize,
	ShowModeMaxisize,
	ShowModeRestore,
	ShowModeNormalNoActive,
	//������Ȼ������ˣ������ڴ��ڳߴ����ƣ���δ��������������Ļ��
	ShowModeMaxisizeFake,
	ShowModeHide,
};

enum FormCommandE
{
	FormCommandNone,
	FormCommandMin,
	FormCommandMax,
	FormCommandClose,
};

/**
 * �������
 */
enum LayoutModeE
{
	//! ���Բ��֣�ֱ�Ӱ��տؼ������ rect ���֡�
	LayoutModeAbsolute = 0,
	//! ˮƽ���֣���ˮƽ�����ϰ��տؼ��� weight ���� fixed width ���֡�
	LayoutModeHorizontal,
	//! ��ֱ���֣��ڴ�ֱ�����ϰ��տؼ��� weight ���� fixed height ���֡�
	LayoutModeVertical,
	//! �б��֣�......
	LayoutModeTable,
};

/**
 * ����ڿؼ��Ŀ�/��ģʽ
 */
enum WHModeE
{
	//! ����ģʽ��ʹ�� rect.w �� rect.h ָ����
	WHModeAbs = 0,
	//! ����������
	WHModeFill,
	//! ����Ӧ����
	WHModeAuto,
};

enum/* TextFlagE */
{
	TextFlagRegular = 0x0000,
	TextFlagBold = 0x0001,
	TextFlagItalic = 0x0002,
	TextFlagUnderline = 0x0004,
	TextFlagStrikeout = 0x0008,

	TextFlagStyleMask = 0x000F,

	TextFlagWrap = 0x0010,
	TextFlagVertical = 0x0020,
	TextFlagFullLIne = 0x0040,
	TextFlagFlagMask = 0x00F0,
};

/**
* ͼ���ʽ��
*/
enum ImageFormatE
{
	ImageFormatBmp = 0,
	ImageFormatJpeg = 1,
	ImageFormatPng = 2,
	ImageFormatGif = 3,
	ImageFormatTiff = 4,
};


/*
 * �߿�����
 */
enum BorderTypeE
{
	BorderTypeNone = 0,
	BorderTypeActive = 0x10000,

	BorderType1Inactive = 1,
	BorderType2Inactive = 2,
	BorderType3Inactive = 3,
	BorderType4Inactive = 4,
	BorderType5Inactive = 5,

	BorderType1 = BorderType1Inactive | BorderTypeActive,
	BorderType2 = BorderType2Inactive | BorderTypeActive,
	BorderType3 = BorderType3Inactive | BorderTypeActive,
	BorderType4 = BorderType4Inactive | BorderTypeActive,
	BorderType5 = BorderType5Inactive | BorderTypeActive,

	BorderTypeUserInactive = 0x8000,
	BorderTypeUser = BorderTypeUserInactive | BorderTypeActive,
};

/**
* �����״��
*/
enum CursorE
{
	CursorUnknown = -1, // δ֪
	CursorNormal = 0, // ��ͷ
	CursorCross = 1, // ʮ�ּ�
	CursorHand = 2, // ����
	CursorLink = 3, // ����
	CursorHelp = 4, // ����
	CursorWait = 5, // �ȴ�
	CursorIBeam = 6, // ������
	CursorNotAviliable = 7, // ��Ч

	CursorSizeLR = 8, // ���ҵ�����С
	CursorSizeTB = 9, // ���µ�����С
	CursorSizeLTRB = 10, // б���µ�����С
	CursorSizeLBRT = 11, // б���ϵ�����С
	CursorSizeAll = 12, // ȫ���������С

	CursorUser = 0xFFFF, // �û��Զ���
};


/**
* �������ϵ�λ�ô��š�
*/
enum ScrollPointE
{
	ScrollPointInvalid = 0,
	ScrollPointHome,
	ScrollPointLineUp,
	ScrollPointPageUp,
	ScrollPointBar,
	ScrollPointPageDown,
	ScrollPointLineDown,
	ScrollPointEnd
};

/**
* MouseWheel��Ϣ��ģʽ��
*/
enum MouseWheelModeE
{
	//! ������·��Ŀؼ����չ�����Ϣ��
	MouseWheelModeHoving,
	//! ����ؼ����չ�����Ϣ��
	MouseWheelModeFocus,
	//! �������ӿؼ����͹�����Ϣ��
	MouseWheelModeNoDispatch,
	//�� ���ù�����Ϣ��
	MouseWheelModeDisabled,
};

/**
* �Ի�������
*/
enum DialogResultE
{
	// ��Ч�ĶԻ�������
	DialogResultInvalid = I32_MIN,
	DialogResultNone = 0,

	DialogResultCancel,
	DialogResultOk,

	DialogResultNo,
	DialogResultYes,
};

/**
* �����в��ԵĽ����
* ��̬����ָʾ������ڿؼ��ڣ����ؼ������������Ϣ��
*/
enum HitTestE
{
	//! ���ڿؼ��ڡ�
	HitTestNoWhere,
	//! �ؼ��ڣ��ǿͻ�����
	HitTestInner,
	//! �ؼ��ڣ��ͻ�����
	HitTestClient,
	//! �ؼ��ڣ���̬����
	HitTestStatic,
	//! �ؼ��ڣ�����괩͸��
	HitTestTransparent,
	HitTestL,
	HitTestT,
	HitTestR,
	HitTestB,
	HitTestLT,
	HitTestTR,
	HitTestRB,
	HitTestBL,

	HitTestCaption,
	HitTestMinBox,
	HitTestMaxBox,
	HitTestRestoreBox,
	HitTestClose,

	HitTestBorder,
	HitTestBorderL,
	HitTestBorderT,
	HitTestBorderR,
	HitTestBorderB,
	HitTestBorderLT,
	HitTestBorderTR,
	HitTestBorderRB,
	HitTestBorderBL,
};

/**
* �Ϸ���Ϊ��
*/
enum DropResultE
{
	//! �������Ϸš�
	DragRequestNone = 0,
	//! �Ϸź��ơ�
	DragRequestCopy,
	//! �Ϸź��ƶ���
	DragRequestMove,
	//! �Ϸź�����ӡ�
	DragRequestLink,
	//! �Ϸź������
	DragRequestScroll,
};

/**
* �˵���ǡ�
*/
enum MenuFlag
{
	//! �ޱ�ǡ�
	MenuFlagNone,
	//! �˵����Ǳ�ѡ�е�(Select)��
	MenuFlagSelected = 0x0001,
	//! �˵����Ǳ�ѡ�е�(Check)��
	MenuFlagCheck = 0x0002,
	//! �˵����Ǳ����õģ������õĲ˵���Ҳ����򿪲˵���
	MenuFlagDisable = 0x0004,
	//! �˵�����һ���ָ���ǡ�
	MenuFlagSplit = 0x00010000,
};

/**
* �б���� Item ״̬��
*/
enum ItemStateE
{
	ItemStateNormal = 0,
	ItemStateHoving = 0x0001,
	ItemStatePressed = 0x0002,
	ItemStateSelected = 0x0004,
	ItemStateInactive = 0x0008,
	ItemStateChecked = 0x0010,

	ItemStateSelectedHover = ItemStateSelected | ItemStateHoving,
	ItemStateSelectedInactive = ItemStateSelected | ItemStateInactive,
};

/**
* �ؼ��� Ime ״̬��
*/
enum ImeModeE
{
	//�� ʹ��Ĭ�ϵ� Ime ״̬��
	ImeModeDefault,
	//! ����ؼ�ʱ��Ime ���ڴ�״̬��
	ImeModeOn,
	//! ����ؼ�ʱ��Ime ���ڹر�״̬��
	ImeModeOff,
	//! ����ؼ�ʱ��Ime ���ڽ���״̬��
	ImeModeDisable,
};

enum FormTypeE
{
	//! ��ͨ�Ĵ��ڣ�һ���� Overlaped��
	FormTypeDefault,
	//! ����ʽ����
	FormTypePopup,
	//! �ֲ㴰��
	FormTypeLayered,
	//! ����ʽ�˵�����
	FormTypeMenu,
	//! �Ӵ���
	FormTypeChild,
};

enum HostInitPosE
{
	HostInitPosDefault = 0,
	HostInitPosCenterParent,
	HostInitPosCenterScreen
};

enum ClipboardDataTypeE
{
	ClipboardDataTypeUnknown,
	ClipboardDataTypeText8,
	ClipboardDataTypeText16,
	ClipboardDataTypeFile,
};

class IMenuItem;

class V2D_API IMenu : public IObject
{
public:
	virtual ~IMenu() {}

	virtual void AddItem(int_x iId, const char_16 * szText, int_x iFlags = 0) = 0;
	virtual void AddItem(int_x iId, const char_16 * szText, IMenu * pSubMenu, int_x iFlags = 0) = 0;
	virtual void AddSplit() = 0;

	virtual int_x GetItemCount() const = 0;
	virtual IMenuItem * GetItem(int_x iIndex) = 0;
};

class V2D_API IMenuItem
{
public:
	virtual ~IMenuItem() {}
	virtual bool IsSpilit() const = 0;
	virtual bool IsChecked() const = 0;
	virtual bool IsSelected() const = 0;
	virtual bool IsEnable() const = 0;
	virtual int_x GetId() const = 0;
	virtual const char_16 * GetText() const = 0;
	virtual IImage * GetImage() const = 0;
	virtual IMenu * GetSubMenu() const = 0;
	virtual int_x GetShortKey() const = 0;
};

class V2D_API formborder_t
{
public:
	formborder_t() : caption(0) {}
	formborder_t(edgeix _edge, int_x _caption) : edge(_edge), caption(_caption) {}
	edgeix edge;
	int_x caption;
	bool is_empty() const { return edge.left <= 0 && edge.top <= 0 && 
		edge.right <= 0 && edge.bottom <= 0 && caption <= 0; }
};

VENUS_END
