#include "stdafx.h"
#include "Rtf.h"

// --------------------------------------------------------- RichTextFontT
RichTextFontT::RichTextFontT()
{
	buffclr(*this);
}

RichTextFontT & RichTextFontT::operator = (const RichTextFontT & another)
{
	buffcpy(*this, another);
	return *this;
}

bool RichTextFontT::operator == (const RichTextFontT & another) const
{
	if(textcmp(Face.buffer, -1, another.Face.buffer, -1))
		return false;

	return CharSet == another.CharSet &&
		Size == another.Size &&
		Weight == another.Weight &&

		Flags == another.Flags;
}

bool RichTextFontT::operator != (const RichTextFontT & another) const
{
	return !operator==(another);
}

// --------------------------------------------------------- RichTextFormatT
RichTextFormatT::RichTextFormatT()
{
	buffclr(*this);
}

RichTextFormatT::RichTextFormatT(const char_x * szFace, int_x iSize)
{
	buffclr(*this);
	rtFont.Face = szFace;
	rtFont.Size = iSize;
}

bool RichTextFormatT::operator == (const RichTextFormatT & another) const
{
	if(rtFont != another.rtFont || uiColor != another.uiColor)
		return false;

	if(rtFont.UnderLine && uiUnderLineColor != another.uiUnderLineColor)
	   return false;

	if(rtFont.StruckOut && uiStruckOutColor != another.uiStruckOutColor)
		return false;

	return true;
}

bool RichTextFormatT::operator != (const RichTextFormatT & another) const
{
	return !operator==(another);
}

RichTextFormatT & RichTextFormatT::operator = (const RichTextFormatT & another)
{
	buffcpy(*this, another);
	return *this;
}

RichTextMetricT::RichTextMetricT()
{
	buffclr(*this);
}
