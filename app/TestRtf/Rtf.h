#pragma once

#include "BaseInc.h"

#ifdef _UNICODE
typedef textw TextX;
#else
typedef texta TextX;
#endif // _UNICODE

const int_x RTF_FACE_SIZE = 32;

class RichTextFontT
{
public:
	RichTextFontT();
	RichTextFontT & operator = (const RichTextFontT & another);
	bool operator == (const RichTextFontT & another) const;
	bool operator != (const RichTextFontT & another) const;

	chbuffer<char_x, RTF_FACE_SIZE> Face;
	int_x CharSet;

	int_x Size;
	int_x Weight;
	union
	{
		int_x Flags;
		struct
		{
			bool Italic : 1;
			bool UnderLine : 1;
			bool StruckOut : 1;
		};
	};
};

class RichTextFormatT
{
public:
	RichTextFormatT();
	RichTextFormatT(const char_x * szFace, int_x iSize);
	bool operator == (const RichTextFormatT & another) const;
	bool operator != (const RichTextFormatT & another) const;
	RichTextFormatT & operator = (const RichTextFormatT & another);

	RichTextFontT rtFont;

	uint_32 uiColor;
	uint_32 uiUnderLineColor;
	uint_32 uiStruckOutColor;
};

class RichTextMetricT
{
public:
	RichTextMetricT();

	int_x Height;
	int_x Ascent;
	int_x Descent;

	int_x InternalLeading;
	int_x ExternalLeading;

	int_x AveCharWidth;
	int_x MaxCharWidth;

	int_x Weight;
	int_x Overhang;

	int_x DigitizedAspectX;
	int_x DigitizedAspectY;

	int_x FirstChar;
	int_x LastChar;
	int_x DefaultChar;
	int_x BreakChar;

	bool Italic;
	bool UnderLine;
	bool StruckOut;

	int_x PitchAndFamily;
	int_x CharSet;
};

class RichTextT
{
public:
	RichTextFormatT rtFormat;
	TextX text;
};

class IRtfParagraph
{
public:
	virtual ~IRtfParagraph() {}
	virtual void AddRichText(const char_8 * szText, int_x iLength = -1) = 0;
	virtual void AddRichText(const char_16 * szText, int_x iLength = -1) = 0;
	virtual void AddRichText(const char_8 * szText, int_x iLength, const RichTextFormatT & rtFormat) = 0;
	virtual void AddRichText(const char_16 * szText, int_x iLength, const RichTextFormatT & rtFormat) = 0;
};

class IRtfDocument
{
public:
	virtual ~IRtfDocument() {}

	virtual IRtfParagraph * AddParagraph(const RichTextFormatT & rtFormat) = 0;
};
