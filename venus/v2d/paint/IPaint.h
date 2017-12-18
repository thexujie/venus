#pragma once

VENUS_BEG

/**
* 路径命令。
*/
enum PathCommandE
{
	PathCommandInvalid = 0,
	PathCommandMoveTo,
	PathCommandLineTo,
	PathCommandBezierTo3,
	PathCommandBezierTo4,
	PathCommandCurveTo,
	PathCommandArcTo4,
};

/**
* 画刷的采样器类型。
*/
enum SamplerTyleE
{
	//! 纯色采样器。
	SamplerTyleSolid,
	//! 线性渐变采样器。
	SamplerTyleLinerGradient,
};

/**
* 画刷的重复/回绕模式。
*/
enum BrushWrapModeE
{
	//! 分块重复。
	BrushWrapModeTile = 0,
	//! 横向翻转。
	BrushWrapModeTileFlipX,
	//! 纵向翻转。
	BrushWrapModeTileFlipY,
	//! 横向、纵向翻转。
	BrushWrapModeTileFlipXY,
	//! 嵌位。
	BrushWrapModeClamp
};

/**
* 图形渲染模式。
*/
enum ShapeRenderLevelE
{
	//! 默认，不抗锯齿。
	ShapeRenderLevelDefault,
	//! 抗锯齿。
	ShapeRenderLevelAnti,
};

/**
* @brief 图片模式，主要用于指定当图片需要绘制在更大(或者更小)矩形内的时候如何对图片进行裁剪、拉伸以获得
*  更好的显示效果。
*/
enum ImageModeE
{
	//! 将原图缩放后绘制到目标区域。
	ImageModeScale = 0,
	//! 垂直方向保持拉伸，水平方向切分为3部分绘制到目标区域，中间部分进行拉伸，左右宽度不变。
	ImageModeGrid3x1H,
	//! 垂直方向保持拉伸，水平方向切分为3部分绘制到目标区域，左右部分进行拉伸，中间宽度不变。
	ImageModeGrid3x1LR,

	ImageModeGrid1x3,
	ImageModeGrid3x3,

	ImageModeMirrorH, // 水平翻转
	ImageModeMirrorV, // 垂直翻转
	ImageModeMirrorHV, // 水平、垂直翻转

	ImageModeClamp, // 嵌位
	ImageModeTileRepeat, // 重复平铺
	ImageModeTileMirror, // 镜像平铺
};

enum SysColorE
{
	SysColorAuto = 0,

	SysColor,
	SysColorForm,
	SysColorBarBackNormal,
	SysColorBarNormal,
	SysColorBarHighlight,
	SysColorBarPressed,
	SysColorBarDisabled,

	SysColorCaptionActive,
	SysColorCaptionNoneActive,

	SysColorBorder,

	//! 控件被禁用时的背景色
	SysColorBackNormal,
	SysColorBackHoving,
	SysColorBackPressed,
	SysColorBackSelected,
	SysColorBackFocused,
	SysColorBackDisable,

	SysColorForeNormal,
	SysColorForeDisable,

	//! 正常的边框
	SysColorFrameNormal,
	//! 高亮的边框
	SysColorFrameHoving,
	//! 按下的边框
	SysColorFramePressed,
	//! 选中的边框
	SysColorFrameSelected,
	//! 焦点的边框
	SysColorFrameFocused,
	//! 禁用的边框
	SysColorFrameDisable,
	//! 控件内部的分隔线(正常)
	SysColorLineNormal,
	//! 控件内部的分隔线(高亮)
	SysColorLineHighlight,
	//! 控件内部的分隔线(禁用)
	SysColorLineDisable,
	//ForeDisable = 0xFF6D6D6D;

	//TextBackSel = 0xFFA7BDE2;
	//TextBackSelDa = 0xFFE1E1E1;
	//! 选中的文本背景
	SysColorTextSelectNormal,
	SysColorTextSelectDisable,

	SysColorPaneCaptionBackNormal,
	SysColorPaneCaptionBackSelected,
	SysColorPaneCaptionTextNormal,
	SysColorPaneCaptionTextSelected,

	SysColorCount,
};

class V2D_API IShape
{
public:
	virtual ~IShape() {}
	virtual void BeginIter(int_x & iIndex) const = 0;
	virtual bool DoIter(int_x & iIndex, PathCommandE & command, float_32 & fX, float_32 & fY) const = 0;
	virtual void EndIter(int_x & iIndex) const = 0;
};

class V2D_API IPath : public IShape
{
public:
	virtual void MoveTo(float_32 fX, float_32 fY) = 0;
	virtual void LineTo(float_32 fX, float_32 fY) = 0;
	virtual void BezierTo3(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2) = 0;
	virtual void BezierTo4(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, float_32 fX3, float_32 fY3) = 0;

	virtual void CurveToX(float_32 * pfX, float_32 * pfY, int_x iCount) = 0;
};

class V2D_API ISampler
{
public:
	virtual ~ISampler() {}
	virtual SamplerTyleE GetSampleType() const = 0;
	virtual uint_32 DoSample() = 0;
	virtual uint_32 DoSample(int_x iX, int_x iY) = 0;
};

class V2D_API IPen : public ISampler
{
public:
	virtual ~IPen() {}
	virtual SamplerTyleE GetSampleType() const = 0;
	virtual uint_32 DoSample() = 0;
	virtual uint_32 DoSample(int_x iX, int_x iY) = 0;
	virtual float_32 GetWidth() const = 0;
};


class V2D_API IBrush : public ISampler
{
public:
	virtual ~IBrush() {}
	virtual SamplerTyleE GetSampleType() const = 0;
	virtual uint_32 DoSample() = 0;
	virtual uint_32 DoSample(int_x iX, int_x iY) = 0;
};

class V2D_API ISolidBrush : public IBrush
{
public:
	virtual ~ISolidBrush() {}
	virtual SamplerTyleE GetSampleType() const = 0;
	virtual uint_32 DoSample() = 0;
	virtual uint_32 DoSample(int_x iX, int_x iY) = 0;
};

class V2D_API ILinerGradientBrush : public IBrush
{
public:
	virtual ~ILinerGradientBrush() {}
	virtual SamplerTyleE GetSampleType() const = 0;
	virtual uint_32 DoSample() = 0;
	virtual uint_32 DoSample(int_x iX, int_x iY) = 0;

	virtual BrushWrapModeE GetWrapMode() const = 0;
	virtual uint_32 GetColorA() const = 0;
	virtual uint_32 GetColorB() const = 0;
	virtual pointix GetPointA() const = 0;
	virtual pointix GetPointB() const = 0;
};

/**
* @brief 绘图工具接口，控件使用 IPaint 进行各种绘图操作，而不用理会绘图工具的具体实现
*  和平台特性。
*/
class V2D_API IPaint : public IObject
{
public:
	virtual ~IPaint() {}

	virtual void ResetTarget(I2DRTarget * pTarget) = 0;
	virtual bool BeginPaint() = 0;
	virtual void EndPaint() = 0;

	virtual void SetShapeRenderLevel(ShapeRenderLevelE eShapeRenderLevel) = 0;
	virtual ShapeRenderLevelE GetShapeRenderLevel() const = 0;

	virtual void PushOrign(int_x iX, int_x iY) = 0;
	virtual void PushOrign(const pointix & point) = 0;
	virtual pointix GetOrign() const = 0;
	virtual void PopOrign() = 0;

	virtual void PushClip(int_x iX, int_x iY, int_x iWidth, int_x iHeight) = 0;
	virtual void PushClip(const rectix & rect) = 0;
	virtual rectix GetClip() const = 0;
	virtual void PopClip() = 0;

	virtual void SetTransform(const matrix3f & matrix) = 0;
	virtual void GetTransform(matrix3f & matrix) const = 0;
	virtual void ResetTransform() = 0;

	virtual void SetImageColorMatrix(const matrix5f & matrix) = 0;
	virtual void GetImageColorMatrix(matrix5f & matrix) const = 0;
	virtual void ResetImageColorMatrix() = 0;

	virtual void Clear(uint_32 uiColor) = 0;
	virtual void Clear(rectix rcClear, uint_32 uiColor) = 0;
	virtual void SetPixel(float_32 fX, float_32 fY, uint_32 uiColor) = 0;
	virtual uint_32 GetPixel(float_32 fX, float_32 fY) = 0;

	virtual void DrawLine(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, IPen * pPen) = 0;
	virtual void DrawPolygon(const pointf32 * pPoints, int_x iCount, uint_32 uiColor, float_32 fLineWidth = 1.0f) = 0;
	virtual void DrawPolygon(const pointix * pPoints, int_x iCount, uint_32 uiColor, float_32 fLineWidth = 1.0f) = 0;
	virtual void FillPolygon(const pointf32 * pPoints, int_x iCount, IBrush * pBrush) = 0;
	virtual void FillPolygon(const pointix * pPoints, int_x iCount, IBrush * pBrush) = 0;
	virtual void FillPolygon(const pointix * pPoints, int_x iCount, uint_32 uiColor) = 0;
	virtual void DrawRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IPen * pPen) = 0;
	virtual void FillRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IBrush * pBrush) = 0;

	virtual void DrawBezier3(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, float_32 fX3, float_32 fY3, IPen * pPen) = 0;

	virtual void DrawArc(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, float_32 fStartAngle, float_32 fAngle, IPen * pPen) = 0;
	virtual void DrawCircle(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IPen * pPen) = 0;
	virtual void FillCircle(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, IBrush * pBrush) = 0;
	virtual void FillCircle(float_32 fX, float_32 fY, float_32 fRadius, IBrush * pBrush) = 0;
	virtual void FillTriangle(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, float_32 fX3, float_32 fY3, IBrush * pBrush) = 0;

	virtual void DrawRoundRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight,
		float_32 fConorW, float_32 fConorH, IPen * pPen) = 0;
	virtual void FillRoundRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight,
		float_32 fConorW, float_32 fConorH, IBrush * pBrush) = 0;

	virtual void DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, pointix point, IBrush * pBrush) = 0;
	virtual void DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, rectix rect, IBrush * pBrush) = 0;

	virtual void DrawShape(const IShape * pShape, IPen * pPen) = 0;
	virtual void FillShape(const IShape * pShape, IBrush * pBrush) = 0;


	// CPaintBase 实现
	virtual void SetPixel(int_x iX, int_x iY, uint_32 uiColor) = 0;
	virtual uint_32 GetPixel(int_x iX, int_x iY) = 0;

	virtual void DrawLine(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, uint_32 uiColor, float_32 fLineWidth = 1.0f) = 0;
	virtual void DrawLine(int_x iX1, int_x iY1, int_x iX2, int_x iY2, IPen * pPen) = 0;
	virtual void DrawLine(int_x iX1, int_x iY1, int_x iX2, int_x iY2, uint_32 uiColor, float_32 fLineWidth = 1.0f) = 0;
	virtual void DrawLine(const pointf32 & point1, const pointf32 & point2, uint_32 uiColor, float_32 fLineWidth = 1.0f) = 0;
	virtual void DrawLine(const pointf32 & point1, const pointf32 & point2, IPen * pPen) = 0;
	virtual void DrawLine(const pointix & point1, const pointix & point2, uint_32 uiColor, float_32 fLineWidth = 1.0f) = 0;
	virtual void DrawLine(const pointix & point1, const pointix & point2, IPen * pPen) = 0;

	// --- TODO
	virtual void DrawRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, uint_32 uiColor, float_32 fLineWidth = 1.0f) = 0;
	virtual void DrawRect(const rectf32 & rect, IPen * pPen) = 0;
	virtual void DrawRect(const rectf32 & rect, uint_32 uiColor, float_32 fLineWidth = 1.0f) = 0;

	virtual void DrawRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight, IPen * pPen) = 0;
	virtual void DrawRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight, uint_32 uiColor, float_32 fLineWidth = 1.0f) = 0;

	virtual void DrawRect(const rectix & rect, IPen * pPen) = 0;
	virtual void DrawRect(const rectix & rect, uint_32 uiColor, float_32 fLineWidth = 1.0f) = 0;

	virtual void FillRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, uint_32 uiColor) = 0;
	virtual void FillRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight, IBrush * pBrush) = 0;
	virtual void FillRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight, uint_32 uiColor) = 0;
	virtual void FillRect(const rectix & rect, IBrush * pBrush) = 0;
	virtual void FillRect(const rectix & rect, uint_32 uiColor) = 0;

	virtual void DrawCircle(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, uint_32 uiColor, float_32 fLineWidth = 1.0f) = 0;
	virtual void DrawCircle(int_x iX, int_x iY, int_x iWidth, int_x iHeight, IPen * pPen) = 0;
	virtual void DrawCircle(const rectf32 & rect, uint_32 uiColor, float_32 fLineWidth = 1.0f) = 0;
	virtual void DrawCircle(const rectf32 & rect, IPen * pPen) = 0;
	virtual void DrawCircle(const rectix & rect, uint_32 uiColor, float_32 fLineWidth = 1.0f) = 0;
	virtual void DrawCircle(const rectix & rect, IPen * pPen) = 0;

	virtual void FillCircle(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight, uint_32 uiColor) = 0;
	virtual void FillCircle(int_x iX, int_x iY, int_x iWidth, int_x iHeight, IBrush * pBrush) = 0;
	virtual void FillCircle(const rectf32 & rect, uint_32 uiColor) = 0;
	virtual void FillCircle(const rectf32 & rect, IBrush * pBrush) = 0;
	virtual void FillCircle(const rectix & rect, uint_32 uiColor) = 0;
	virtual void FillCircle(const rectix & rect, IBrush * pBrush) = 0;

	virtual void FillCircle(float_32 fX, float_32 fY, float_32 fRadius, uint_32 uiColor) = 0;
	virtual void FillCircle(int_x iX, int_x iY, int_x iRadius, IBrush * pBrush) = 0;
	virtual void FillCircle(int_x iX, int_x iY, int_x iRadius, uint_32 uiColor) = 0;

	virtual void FillTriangle(float_32 fX1, float_32 fY1, float_32 fX2, float_32 fY2, float_32 fX3, float_32 fY3, uint_32 uiColor) = 0;
	virtual void FillTriangle(int_x iX1, int_x iY1, int_x iX2, int_x iY2, int_x iX3, int_x iY3, uint_32 uiColor) = 0;

	virtual void DrawRoundRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight,
		float_32 fConorW, float_32 fConorH, uint_32 uiColor, float_32 fLineWidth = 1.0f) = 0;
	virtual void FillRoundRect(float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight,
		float_32 fConorW, float_32 fConorH, uint_32 uiColor) = 0;
	virtual void DrawRoundRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight,
		int_x iConorW, int_x iConorH, uint_32 uiColor, float_32 fLineWidth = 1.0f) = 0;
	virtual void FillRoundRect(int_x iX, int_x iY, int_x iWidth, int_x iHeight,
		int_x iConorW, int_x iConorH, uint_32 uiColor) = 0;
	virtual void DrawRoundRect(const rectix & rect, int_x iConorW, int_x iConorH, uint_32 uiColor, float_32 fLineWidth = 1.0f) = 0;
	virtual void FillRoundRect(const rectf32 & rect, float_32 fConorW, float_32 fConorH, uint_32 uiColor) = 0;

	virtual void DrawBezier3(const pointf32 & point1, const pointf32 & point2, const pointf32 & point3, IPen * pPen) = 0;

	virtual void DrawShape(const IShape * pShape, uint_32 uiColor, float_32 fWidth) = 0;
	virtual void FillShape(const IShape * pShape, uint_32 uiColor) = 0;

public: // ---------------------------------------------- DrawString
	virtual void DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, pointix point, uint_32 uiColor) = 0;
	virtual void DrawString(const char_16 * szText, int_x iLength, const textformat_t & format, rectix rect, uint_32 uiColor) = 0;

	virtual void DrawString(const textw & text, const textformat_t & format, pointix point, IBrush * pBrush) = 0;
	virtual void DrawString(const textw & text, const textformat_t & format, rectix rect, IBrush * pBrush) = 0;
	virtual void DrawString(const textw & text, const textformat_t & format, pointix point, uint_32 uiColor) = 0;
	virtual void DrawString(const textw & text, const textformat_t & format, rectix rect, uint_32 uiColor) = 0;

	virtual void DrawTextLayout(const ITextLayout * pTextLayout,
		const pointix & ptDraw, const rectix & rcClip,
		uint_32 uiColor) = 0;

public: // ------------------------------- DrawImage
	virtual void DrawImage(IImage * pImage, float_32 fX, float_32 fY) = 0;
	virtual void DrawImage(IImage * pImage, float_32 fX, float_32 fY, AlignE align) = 0;

	virtual void DrawImage(IImage * pImage, float_32 fX, float_32 fY, float_32 fWidth, float_32 fHeight) = 0;
	virtual void DrawImage(IImage * pImage, rectf32 rect) = 0;

	virtual void DrawImage(IImage * pImage,
		float_32 fDstX, float_32 fDstY,
		float_32 fSrcX, float_32 fSrcY, float_32 fSrcWidth, float_32 fSrcHeight) = 0;
	virtual void DrawImage(IImage * pImage,
		float_32 fDstX, float_32 fDstY, float_32 fDstWidth, float_32 fDstHeight,
		float_32 fSrcX, float_32 fSrcY, float_32 fSrcWidth, float_32 fSrcHeight) = 0;
	virtual void DrawImage(IImage * pImage, const pointf32 & ptDst, const rectf32 & rcSrc) = 0;
	virtual void DrawImage(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc) = 0;

	virtual void DrawImageMirrorH(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc) = 0;
	virtual void DrawImageMirrorV(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc) = 0;
	virtual void DrawImageMirrorHV(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc) = 0;

	virtual void DrawImageGrid(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc, const rectf32 & rcGrid) = 0;
	virtual void DrawImageGrid3x1(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc, float_32 fGridX, float_32 fGridW) = 0;
	virtual void DrawImageGrid1x3(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc, float_32 fGridY, float_32 fGridH) = 0;
	virtual void DrawImageGridH3V3(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc) = 0;

	virtual void DrawImageGridH3H(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc) = 0;
	virtual void DrawImageGridH3LR(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc) = 0;
	virtual void DrawImageGridV3V(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc) = 0;

	virtual void DrawImageTileRepeat(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc) = 0;
	virtual void DrawImageTileMirror(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc) = 0;

	virtual void DrawImageRotate(IImage * pImage, float_32 fX, float_32 fY,
		float_32 fRotation, float_32 fOrignX = 0, float_32 fOrignY = 0) = 0;
	virtual void DrawImageClamp(IImage * pImage, const rectf32 & rcDst, const rectf32 & rcSrc) = 0;

	virtual void DrawImageWithMode(IImage * pImage, const rectix & rcDst, const rectix & rcSrc, ImageModeE eMode) = 0;

	// ----------------- int_x 类型。
	virtual void DrawImage(IImage * pImage, int_x iX, int_x iY, AlignE align = AlignLeftTop) = 0;
	virtual void DrawImage(IImage * pImage, int_x iX, int_x iY, int_x iWidth, int_x iHeight) = 0;
	virtual void DrawImage(IImage * pImage, rectix rect) = 0;

	virtual void DrawImage(IImage * pImage,
		int_x iDstX, int_x iDstY,
		int_x iSrcX, int_x iSrcY, int_x iSrcWidth, int_x iSrcHeight) = 0;
	virtual void DrawImage(IImage * pImage,
		int_x iDstX, int_x iDstY, int_x iDstWidth, int_x iDstHeight,
		int_x iSrcX, int_x iSrcY, int_x iSrcWidth, int_x iSrcHeight) = 0;

	virtual uint_32 GetSystemColor(SysColorE eColor) const = 0;
};

class V2D_API IDevice2D : public IService
{
public:
	virtual ~IDevice2D() {}

	virtual Device2DTypeE GetType() const = 0;
	virtual ITextLayout * CreateTextLayout() = 0;
	virtual textsize_t GetTextSize(const char_16 * szText, int_x iLength, const font_t & font) = 0;

	virtual image_convert_rule_fun_t GetImageConvertRuleFun() const = 0;
	virtual IPaint * CreatePaint(I2DRTarget * pRenderTarget) = 0;
};

static const cid_t CID_IDevice2D = {L"IDevice2D"};

V2D_API void Set2DDevice(IDevice2D * pDevice);
V2D_API IDevice2D * Device2D();

VENUS_END
