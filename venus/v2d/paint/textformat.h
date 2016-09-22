#pragma once

VENUS_BEG

enum TextWrapE
{
	//! ���ֲ��Զ�����
	TextWrapNone,
	//! �������ַ��Զ�����
	TextWrapChar,
	//! �����������Զ����У����в�������һ�����ʣ��� TextWrapChar ��ֵ���
	TextWrapWord,
	//! �����������Զ����У����в�������һ������ʱ����Ȼ����ָõ��ʡ�=
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
	//! ʹ��ϵͳ����ֵ��
	FontRenderLevelSystem = -1,
	//! ʹ����ʾ��Ϣ��ʹ�� ClearType ��ɫͼ��Ⱦ��
	FontRenderLevelClearTypeGrid = 0,
	//! �Ҷ�ͼ��Ⱦ��
	FontRenderLevelGray,
	//! ʹ����ʾ��Ϣ���Ҷ�ͼ��Ⱦ��
	FontRenderLevelGrayGrid,
	//! �Ҷ�ͼ�������Ⱦ��
	FontRenderLevelAntiGray,
	//! ʹ����ʾ��Ϣ���Ҷ�ͼ�������Ⱦ��
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

	//! �������ơ�
	chbufferw<MAX_FONTNAME> name;
	//! �ַ���
	int_x charset;
};

/**
* ָʾ Paint ��������ʱ��ʹ�õ����壬
* ������������ơ��ֺš���Ⱦ��ʽ�����塢б�塢�»��ߡ�ɾ���ߵȡ�
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

	//! �����С��
	int_x size;
	//! �����ϸ
	int_x weight;
	//! б��
	int_x italic;
	//! �»���
	int_x underline;
	//! ɾ����
	int_x strikeout;
	//! ������Ⱦ�ȼ���
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
* ������Ϣ����ͬ�� font_t��fontmetrics_t ָ��������ľ������ػ���Ϣ����׼��ߡ��оࡢ�иߵȡ�
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
