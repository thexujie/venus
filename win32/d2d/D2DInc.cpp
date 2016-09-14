#include "stdafx.h"
#include "D2DInc.h"

VENUS_BEG

DWRITE_WORD_WRAPPING GetDwTextWrapping(TextWrapE eTextWrap)
{
	switch(eTextWrap)
	{
	case TextWrapNone:
		return DWRITE_WORD_WRAPPING_NO_WRAP;
	case TextWrapChar:
		return DWRITE_WORD_WRAPPING_WRAP;
#if WINVER > WINVER_WINBLUE
	case TextWrapWord:
		return DWRITE_WORD_WRAPPING_EMERGENCY_BREAK;
	case TextWrapWordNoBreaking:
		return DWRITE_WORD_WRAPPING_WHOLE_WORD;
#endif
	default:
		return DWRITE_WORD_WRAPPING_NO_WRAP;
	}
}

VENUS_END

