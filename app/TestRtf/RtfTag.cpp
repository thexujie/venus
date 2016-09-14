#include "stdafx.h"
#include "RtfTag.h"

static bool RtfTagEqual(const char_8 * pText, const char_8 * pEnd, const char_8 * pAttr, int_x iLength)
{
	if(iLength < 0)
		iLength = textlen(pAttr);

	if(iLength != pEnd - pText)
		return false;
	else
	{
		while(iLength--)
		{
			if(*pText != *pAttr)
				return false;
			else
				++pText, ++pAttr;
		}
		return true;
	}
}

#define RTF_TAG_CASE(tag) else if(RtfTagEqual(pText, pEnd, #tag, sizeof(#tag) - 1)) return RtfTag_##tag

RtfTagE GetRtfTag(const char_8 * pText, const char_8 * pEnd)
{
	if(pText >= pEnd) return RtfTag_invalid;
	else if(RtfTagEqual(pText, pEnd, "*", 1)) return RtfTag_ref;
	else if(RtfTagEqual(pText, pEnd, "u", 1)) return RtfTag_u;
	RTF_TAG_CASE(rtf);
	RTF_TAG_CASE(tab);
	RTF_TAG_CASE(fcharset);
	RTF_TAG_CASE(fs);
	RTF_TAG_CASE(f);
	RTF_TAG_CASE(fnil);
	RTF_TAG_CASE(ansi);
	RTF_TAG_CASE(ansicpg);
	RTF_TAG_CASE(deff);
	RTF_TAG_CASE(nouicompat);
	RTF_TAG_CASE(fonttbl);
	RTF_TAG_CASE(colortbl);
	RTF_TAG_CASE(red);
	RTF_TAG_CASE(green);
	RTF_TAG_CASE(blue);
	RTF_TAG_CASE(viewkind);
	RTF_TAG_CASE(uc);
	RTF_TAG_CASE(pard);
	RTF_TAG_CASE(par);
	RTF_TAG_CASE(cf);
	RTF_TAG_CASE(lang);
	RTF_TAG_CASE(b);
	RTF_TAG_CASE(deflangfe);
	RTF_TAG_CASE(deflang);

	RTF_TAG_CASE(chbrdr);
	RTF_TAG_CASE(chshdng);
	RTF_TAG_CASE(chcfpat);
	RTF_TAG_CASE(chcbpat);
	RTF_TAG_CASE(chbghoriz);
	RTF_TAG_CASE(chbgvert);
	RTF_TAG_CASE(chbgfdiag);
	RTF_TAG_CASE(chbgbdiag);
	RTF_TAG_CASE(chbgcross);
	RTF_TAG_CASE(chbgdcross);
	RTF_TAG_CASE(chbgdkhoriz);
	RTF_TAG_CASE(chbgdkvert);
	RTF_TAG_CASE(chbgdkfdiag);
	RTF_TAG_CASE(chbgdkbdiag);
	RTF_TAG_CASE(chbgcross);
	RTF_TAG_CASE(chbgdcross);

	RTF_TAG_CASE(ul);
	RTF_TAG_CASE(ulc);
	RTF_TAG_CASE(uld);
	RTF_TAG_CASE(uldash);
	RTF_TAG_CASE(uldashd);
	RTF_TAG_CASE(uldashdd);
	RTF_TAG_CASE(uldb);
	RTF_TAG_CASE(ulhwave);
	RTF_TAG_CASE(ulldash);
	RTF_TAG_CASE(ulnone);
	RTF_TAG_CASE(ulth);
	RTF_TAG_CASE(ulthd);
	RTF_TAG_CASE(ulthdash);
	RTF_TAG_CASE(ulthdashd);
	RTF_TAG_CASE(ulthdashdd);
	RTF_TAG_CASE(ulthldash);
	RTF_TAG_CASE(ululdbwave);
	RTF_TAG_CASE(ulw);
	RTF_TAG_CASE(ulwave);


	RTF_TAG_CASE(outl);
	RTF_TAG_CASE(scaps);
	RTF_TAG_CASE(shad);
	RTF_TAG_CASE(strike);
	RTF_TAG_CASE(striked1);
	RTF_TAG_CASE(sub);
	RTF_TAG_CASE(super);

	RTF_TAG_CASE(qc);
	RTF_TAG_CASE(qj);
	RTF_TAG_CASE(ql);
	RTF_TAG_CASE(qr);
	RTF_TAG_CASE(qd);
	RTF_TAG_CASE(qk);
	RTF_TAG_CASE(qt);

	RTF_TAG_CASE(fi);
	RTF_TAG_CASE(cufi);
	RTF_TAG_CASE(li);
	RTF_TAG_CASE(lin);
	RTF_TAG_CASE(culi);
	RTF_TAG_CASE(ri);
	RTF_TAG_CASE(rin);
	RTF_TAG_CASE(curi);
	RTF_TAG_CASE(adjustright);

	RTF_TAG_CASE(sb);
	RTF_TAG_CASE(sa);
	RTF_TAG_CASE(sbauto);
	RTF_TAG_CASE(saauto);
	RTF_TAG_CASE(lisb);
	RTF_TAG_CASE(lisa);
	RTF_TAG_CASE(sl);
	RTF_TAG_CASE(slmult);
	RTF_TAG_CASE(nosnaplinegrid);
	return RtfTag_unknown;
}
