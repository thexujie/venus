#pragma once

#ifdef _MSC_VER
#if (_MSC_VER <= MSVC_2003 )

#ifndef _ERRNO_T_DEFINED
#define _ERRNO_T_DEFINED
typedef int errno_t;
#endif

inline int_x memcpy_s(void * pDst, int_x iSize, const void * pSrc, int_x iCount)
{
	iSize = Min(iSize, iCount);
	memcpy(pDst, pSrc, iSize);
	return iSize;
}

inline int_x memmove_s(void * pDst, int_x iSize, const void * pSrc, int_x iCount)
{
	iSize = Min(iSize, iCount);
	memmove(pDst, pSrc, iSize);
	return iSize;
}

inline int_x _wsopen_s(int * pfh, const wchar_t * szFile, int oflag, int shflag, int pmode)
{
	if(pfh)
	{
		*pfh = _wsopen(szFile, oflag, shflag, pmode);
		return errno;
	}
	else
		return EIO;
}

inline errno_t mbstowcs_s(size_t * _PtNumOfCharConverted, wchar_t * _DstBuf, size_t _SizeInWords, const char * _SrcBuf, size_t _MaxCount)
{
	size_t size = mbstowcs(_DstBuf, _SrcBuf, _MaxCount);
	if(_PtNumOfCharConverted)
		*_PtNumOfCharConverted = size;
	return errno;
}

inline errno_t wcstombs_s(size_t * _PtNumOfCharConverted, char * _DstBuf, size_t _SizeInWords, const wchar_t * _SrcBuf, size_t _MaxCount)
{
	size_t size = wcstombs(_DstBuf, _SrcBuf, _MaxCount);
	if(_PtNumOfCharConverted)
		*_PtNumOfCharConverted = size;
	return errno;
}

inline int vsprintf_s(char * _DstBuf, int_x _SizeInBytes, const char * _Format, char * _ArgList)
{
	return vsprintf(_DstBuf, _Format, _ArgList);
}

inline int vswprintf_s(wchar_t * _DstBuf, int_x _SizeInBytes, const wchar_t * _Format, char * _ArgList)
{
	return vswprintf(_DstBuf, _Format, _ArgList);
}

inline int_x _localtime64_s(struct tm * _Tm, const int_64 *_Time)
{
	*_Tm = *_localtime64(_Time);
	return errno;
}

#endif
#endif
