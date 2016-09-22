#pragma once

VENUS_BEG

enum TextWrapE
{
	//! 文字不自动换行
	TextWrapNone,
	//! 按单个字符自动换行
	TextWrapChar,
	//! 按单个单词自动换行，当行不能容下一个单词，则按 TextWrapChar 拆分单词
	TextWrapWord,
	//! 按单个单词自动换行，当行不能容下一个单词时，依然不拆分该单词。=
	TextWrapWordNoBreaking,
};

enum TextTrimmingE
{
	TextTrimmingNone,
	TextTrimmingChar,
	TextTrimmingCharEllipsis,
	TextTrimmingWord,
	TextTrimmingWordEllipsis,
	TextTrimmingPathEllipsis,
};

enum FontFlagE
{
	FontFlagBold,
	FontFlagItalic,
	FontFlagUnderLine,
	FontFlagStrikeOut,
};
enum FontWrapE
{
	FontWrapByWord,
	FontWrapByChar,
	FontWrapNoWrap,
};

enum FontRenderlevelE
{
	//! 使用系统设置值。
	FontRenderLevelSystem = -1,
	//! 使用提示信息，使用 ClearType 彩色图渲染。
	FontRenderLevelClearTypeGrid = 0,
	//! 灰度图渲染。
	FontRenderLevelGray,
	//! 使用提示信息，灰度图渲染。
	FontRenderLevelGrayGrid,
	//! 灰度图抗锯齿渲染。
	FontRenderLevelAntiGray,
	//! 使用提示信息，灰度图抗锯齿渲染。
	FontRenderLevelAntiGrayGrid,
};

const int_x MAX_FONTNAME = 32;
const char_16 FONT_DEF_FACE[] = L"";
const int_x FONT_WEIGHT_BOLD = 800;
const int_x FONT_WEIGHT_NORMAL = 400;

const int_x FONT_DEF_CHARSET = 134;
const int_x FONT_DEF_SIZE = 16;

const int_x FONT_DEF_WEIGHT = FONT_WEIGHT_NORMAL;
const int_x FONT_DEF_FLAG = 0;
const uint_32 FONT_DEF_COLOR = 0x00FFFFFF;

const FontRenderlevelE FONT_DEF_RENDER_LEVEL = FontRenderLevelClearTypeGrid;

class V2D_API font_base_t
{
public:
	font_base_t();
	font_base_t(const char_16 * _name, int_x _charset = FONT_DEF_CHARSET);
	font_base_t(const font_base_t & another);
	font_base_t & operator = (const font_base_t & another);

	//! 字体名称。
	chbufferw<MAX_FONTNAME> name;
	//! 字符集
	int_x charset;
};

/**
* 指示 Paint 绘制文字时所使用的字体，
* 包括字体簇名称、字号、渲染方式、粗体、斜体、下划线、删除线等。
*/
class V2D_API font_t : public font_base_t
{
public:
	font_t();
	font_t(const char_16 * _name,
		int_x _size = FONT_DEF_SIZE,
		int_x _weight = FONT_DEF_WEIGHT,
		FontRenderlevelE _renderlevel = FontRenderLevelClearTypeGrid,
		int_x _charset = FONT_DEF_CHARSET);
	font_t(const font_t & another);
	font_t & operator = (const font_t & another);
	bool operator == (const font_t & another) const;
	bool operator != (const font_t & another) const;

	int_x Hash() const;

	//! 字体大小。
	int_x size;
	//! 字体粗细
	int_x weight;
	//! 斜体
	int_x italic;
	//! 下划线
	int_x underline;
	//! 删除线
	int_x strikeout;
	//! 字体渲染等级。
	FontRenderlevelE renderlevel;
};

class V2D_API textformat_t
{
public:
	textformat_t();
	textformat_t(const font_t & _font);
	textformat_t(const font_t & _font, TextWrapE _wrap);
	textformat_t(const font_t & _font,
		AlignE _align,
		TextWrapE _wrap = TextWrapNone,
		TextTrimmingE _trmming = TextTrimmingNone,
		AxisE _direction = AxisX);
	textformat_t & operator = (const textformat_t & another);
	int_x Hash() const;

	font_t font;
	AlignE align;
	TextWrapE wrap;
	TextTrimmingE trimming;
	AxisE direction;
};

/**
* 字体信息，不同于 font_t，fontmetrics_t 指出了字体的具体像素化信息，标准宽高、行距、行高等。
*/
class V2D_API fontmetrics_t
{
public:
	fontmetrics_t() { buffclr(*this); }

	int_x size;
	int_x weight;

	int_x linespace;
	int_x baseline;

	bool italic;
	bool underLine;
	bool struckout;
};

VENUS_END
