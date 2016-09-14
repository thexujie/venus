#include "stdafx.h"
#include "textformat.h"

VENUS_BEG

font_base_t::font_base_t()
	: name(FONT_DEF_FACE), charset(FONT_DEF_CHARSET)
{

}

font_base_t::font_base_t(const char_16 * _name, int_x _charset)
	: name(_name), charset(_charset)
{

}

font_base_t::font_base_t(const font_base_t & another)
	: name(another.name), charset(another.charset)
{

}

font_base_t & font_base_t::operator = (const font_base_t & another)
{
	name = another.name;
	charset = another.charset;
	return *this;
}

font_t::font_t()
	: size(FONT_DEF_SIZE), weight(FONT_DEF_WEIGHT), italic(0), underline(0), strikeout(0),
	renderlevel(FontRenderLevelClearTypeGrid)

{
}

font_t::font_t(const char_16 * _name, int_x _size, int_x _weight, FontRenderlevelE _renderlevel, int_x _charset)
	: font_base_t(_name, _charset), size(_size), weight(_weight), renderlevel(_renderlevel),
	italic(0), underline(0), strikeout(0)
{
}

font_t::font_t(const font_t & another)
	: font_base_t(another.name, another.charset), size(another.size), weight(another.weight),
	italic(another.italic), underline(another.underline), strikeout(another.strikeout),
	renderlevel(another.renderlevel)
{

}

font_t & font_t::operator = (const font_t & another)
{
	name = another.name;
	charset = another.charset;
	size = another.size;
	weight = another.weight;
	italic = another.italic;
	underline = another.underline;
	strikeout = another.strikeout;
	renderlevel = another.renderlevel;
	return *this;
}

bool font_t::operator == (const font_t & another) const
{
	return name == another.name && charset == another.charset &&
		size == another.size &&
		weight == another.weight &&
		italic == another.italic &&
		underline == another.underline &&
		strikeout == another.strikeout &&
		renderlevel == another.renderlevel;
}

bool font_t::operator != (const font_t & another) const
{
	return !operator==(another);
}

int_x font_t::Hash() const
{
	int_x iHash = hashtextx(name);
	iHash = hashx(charset, iHash);
	iHash = hashx(size, iHash);
	iHash = hashx(weight, iHash);
	iHash = hashx(italic, iHash);
	iHash = hashx(underline, iHash);
	iHash = hashx(strikeout, iHash);
	return iHash;
}

textformat_t::textformat_t()
	: align(AlignLeftTop), wrap(TextWrapNone), trimming(TextTrimmingNone), direction(AxisX)
{

}

textformat_t::textformat_t(const font_t & _font)
	: font(_font), align(AlignLeftTop), wrap(TextWrapNone), trimming(TextTrimmingNone), direction(AxisX)
{

}

textformat_t::textformat_t(const font_t & _font, TextWrapE _wrap)
	: font(_font), align(AlignLeftTop), wrap(_wrap), trimming(TextTrimmingNone), direction(AxisX)
{

}

textformat_t::textformat_t(const font_t & _font, AlignE _align, TextWrapE _wrap, TextTrimmingE _trmming, AxisE _direction)
	: font(_font), align(_align), wrap(_wrap), trimming(_trmming), direction(_direction)
{

}

textformat_t & textformat_t::operator = (const textformat_t & another)
{
	font = another.font;
	align = another.align;
	wrap = another.wrap;
	trimming = another.trimming;
	direction = another.direction;
	return *this;
}

int_x textformat_t::Hash() const
{
	int_x iHash = font.Hash();
	iHash = hashx(align, iHash);
	iHash = hashx(wrap, iHash);
	iHash = hashx(trimming, iHash);
	iHash = hashx(direction, iHash);
	return iHash;
}

VENUS_END

