#pragma once

VENUS_BEG

/**
* 鼠标按钮。
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
* 键盘按键。
*/
enum KeyCodeE
{
	//! 不可用的按键码。
	KeyCodeInvalid = 0x0,

	// 数字键
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
	// 字符键
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

	// 小键盘
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

	// 功能键
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
	KeyCodeSub, // 减号 -_
	KeyCodeEqual, // 等于 =+
	KeyCodeGrave, // 抑音符 `~
	KeyCodeComma, // 逗号 ，<
	KeyCodePeriod, // 句号 .>
	KeyCodeSlash, // 斜杠 /?
	KeyCodeSemicolon, // 分号 ;:
	KeyCodeColon, // 冒号 : (某些键盘有单独的冒号)
	KeyCodeApostrophe, // 单引号、省字符 '
	KeyCodeBracketL, // 左方括号 [{
	KeyCodeBracketR, // 右方括号 ]}
	KeyCodeBackslash, // 反斜杠 \|

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

	// 特殊键
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
	//! 静音
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

	// -- 其他
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
* 窗口的显示方式。
*/
enum ShowModeE
{
	ShowModeNormal,
	ShowModeMinisize,
	ShowModeMaxisize,
	ShowModeRestore,
	ShowModeNormalNoActive,
	//窗口虽然被最大化了，但由于窗口尺寸限制，并未真正覆盖整个屏幕。
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
 * 框架类型
 */
enum LayoutModeE
{
	//! 绝对布局，直接按照控件自身的 rect 布局。
	LayoutModeAbsolute = 0,
	//! 水平布局，在水平方向上按照控件的 weight 或者 fixed width 布局。
	LayoutModeHorizontal,
	//! 垂直布局，在垂直方向上按照控件的 weight 或者 fixed height 布局。
	LayoutModeVertical,
	//! 列表布局，......
	LayoutModeTable,
};

/**
 * 框架内控件的宽/高模式
 */
enum WHModeE
{
	//! 绝对模式，使用 rect.w 和 rect.h 指定。
	WHModeAbs = 0,
	//! 填满父窗口
	WHModeFill,
	//! 自适应内容
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
* 图像格式。
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
 * 边框类型
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
* 光标形状。
*/
enum CursorE
{
	CursorUnknown = -1, // 未知
	CursorNormal = 0, // 箭头
	CursorCross = 1, // 十字架
	CursorHand = 2, // 手型
	CursorLink = 3, // 链接
	CursorHelp = 4, // 帮助
	CursorWait = 5, // 等待
	CursorIBeam = 6, // 工字型
	CursorNotAviliable = 7, // 无效

	CursorSizeLR = 8, // 左右调整大小
	CursorSizeTB = 9, // 上下调整大小
	CursorSizeLTRB = 10, // 斜向下调整大小
	CursorSizeLBRT = 11, // 斜向上调整大小
	CursorSizeAll = 12, // 全方向调整大小

	CursorUser = 0xFFFF, // 用户自定义
};


/**
* 滚动条上的位置代号。
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
* MouseWheel消息的模式。
*/
enum MouseWheelModeE
{
	//! 鼠标正下方的控件接收滚轮消息。
	MouseWheelModeHoving,
	//! 焦点控件接收滚轮消息。
	MouseWheelModeFocus,
	//! 不再向子控件派送滚动消息。
	MouseWheelModeNoDispatch,
	//！ 禁用滚轮消息。
	MouseWheelModeDisabled,
};

/**
* 对话框结果。
*/
enum DialogResultE
{
	// 无效的对话框结果。
	DialogResultInvalid = I32_MIN,
	DialogResultNone = 0,

	DialogResultCancel,
	DialogResultOk,

	DialogResultNo,
	DialogResultYes,
};

/**
* 鼠标击中测试的结果。
* 静态区域指示了鼠标在控件内，但控件不关心鼠标消息。
*/
enum HitTestE
{
	//! 不在控件内。
	HitTestNoWhere,
	//! 控件内，非客户区。
	HitTestInner,
	//! 控件内，客户区。
	HitTestClient,
	//! 控件内，静态区域。
	HitTestStatic,
	//! 控件内，被鼠标穿透。
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
* 拖放行为。
*/
enum DropResultE
{
	//! 不接受拖放。
	DragRequestNone = 0,
	//! 拖放后复制。
	DragRequestCopy,
	//! 拖放后移动。
	DragRequestMove,
	//! 拖放后打开连接。
	DragRequestLink,
	//! 拖放后滚动。
	DragRequestScroll,
};

/**
* 菜单标记。
*/
enum MenuFlag
{
	//! 无标记。
	MenuFlagNone,
	//! 菜单项是被选中的(Select)。
	MenuFlagSelected = 0x0001,
	//! 菜单项是被选中的(Check)。
	MenuFlagCheck = 0x0002,
	//! 菜单项是被禁用的，被禁用的菜单项也不会打开菜单。
	MenuFlagDisable = 0x0004,
	//! 菜单项是一个分隔标记。
	MenuFlagSplit = 0x00010000,
};

/**
* 列表项的 Item 状态。
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
* 控件的 Ime 状态。
*/
enum ImeModeE
{
	//！ 使用默认的 Ime 状态。
	ImeModeDefault,
	//! 激活控件时，Ime 处于打开状态。
	ImeModeOn,
	//! 激活控件时，Ime 处于关闭状态。
	ImeModeOff,
	//! 激活控件时，Ime 处于禁用状态。
	ImeModeDisable,
};

enum FormTypeE
{
	//! 普通的窗口，一般是 Overlaped。
	FormTypeDefault,
	//! 弹出式窗口
	FormTypePopup,
	//! 分层窗口
	FormTypeLayered,
	//! 弹出式菜单窗口
	FormTypeMenu,
	//! 子窗口
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
