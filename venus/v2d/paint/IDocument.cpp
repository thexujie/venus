#include "stdafx.h"
#include "IDocument.h"

VENUS_BEG

int_x GetParagraphTagLength(paragraph_tag_e tag)
{
	switch(tag)
	{
	case paragraph_tag_none: return 0;
	case paragraph_tag_cr: return 1;
	case paragraph_tag_lf: return 1;
	case paragraph_tag_crlf: return 2;
	case paragraph_tag_ls: return 1;
	case paragraph_tag_ps: return 1;
	default: return 0;
	}
}
vector<doc_paragraph_t> TextParseParagraphs(const char_16 * szText, int_x iLength)
{
	vector<doc_paragraph_t> paragraphs;
	const char_16 * pText = szText;
	const char_16 * pTemp = szText;
	const char_16 * pEnd = szText + iLength;
	char_16 ch = 0;
	bool bRunning = true;
	paragraph_tag_e eParagraphTag = paragraph_tag_none;
	int_x iTagLength = 0;
	bool bAdd = false;
	while(bRunning && pTemp < pEnd)
	{
		switch(*pTemp)
		{
		case L'\r':
			eParagraphTag = paragraph_tag_cr;
			iTagLength = 0;
			break;
		case L'\u2028':
			eParagraphTag = paragraph_tag_ls;
			iTagLength = 1;
			bAdd = true;
			break;
		case L'\u2029':
			eParagraphTag = paragraph_tag_ps;
			iTagLength = 1;
			bAdd = true;
			break;
		case L'\n':
			if(eParagraphTag == paragraph_tag_cr)
			{
				eParagraphTag = paragraph_tag_crlf;
				iTagLength = 2;
			}
			else
			{
				eParagraphTag = paragraph_tag_lf;
				iTagLength = 1;
			}
			bAdd = true;
			break;
		case L'\0':
			bRunning = false;
			break;
		default:
			break;
		}

		++pTemp;
		if(bAdd)
		{
			doc_paragraph_t paragraph = {{pText - szText, pTemp - pText}, eParagraphTag};
			paragraphs.add(paragraph);
			pText = pTemp;
			eParagraphTag = paragraph_tag_none;
			iTagLength = 0;
			bAdd = false;
		}
	}

	if(pTemp > pText || eParagraphTag == paragraph_tag_none)
	{
		doc_paragraph_t paragraph = {{pText - szText, pTemp - pText}, eParagraphTag};
		paragraphs.add(paragraph);
	}

	return paragraphs;
}

VENUS_END
