#pragma once

#define CPP_VER_C97 199711L
#define CPP_VER_C11 201103
#define PRINT_MACRO_HELPER(x) #x 
#define PRINT_MACRO(x) #x"="PRINT_MACRO_HELPER(x)

#ifdef _WIN32

//#define NTDDI_VERSION NTDDI_WINXP
//#define _WIN32_WINNT _WIN32_WINNT_WINXP
//#define WINVER _WIN32_WINNT

#define MSC_2015 1900 // vs2015
#define MSC_2013 1800 // vs2013
#define MSC_2012 1700 // vs2012
#define MSC_2010 1600 // vs1010
#define MSC_2008 1500 // vs2008
#define MSC_2005 1400 // vs2005
#define MSC_2003 1310 // vs2003
#define MSC_2002 1200 // vs2002
#define MSC_vc6  1100 // vc 6.0
#define MSC_none 0000 // none

#ifdef _MSC_VER
#define MSC_VER _MSC_VER
#else
#define MSC_VER MSVC_none
#endif

//! 是否支持 C++ 11
#if MSC_VER < MSC_2013
#error msvc compiler less than MSC_2013 is not supported!
#endif

#ifdef _USING_V110_SDK71_
#define _WIN32_WINNT 0x0501
#endif

#include <sdkddkver.h>

#define WINVER_NT4                    0x0400
#define WINVER_WIN2K                  0x0500
#define WINVER_WINXP                  0x0501
#define WINVER_WS03                   0x0502
#define WINVER_WIN6                   0x0600
#define WINVER_VISTA                  0x0600
#define WINVER_WS08                   0x0600
#define WINVER_LONGHORN               0x0600
#define WINVER_WIN7                   0x0601
#define WINVER_WIN8                   0x0602
#define WINVER_WINBLUE                0x0603

#if (MSC_VER < MSC_2015)
#define constfun
#define constdata
#else
#define constfun constexpr
#define constdata constexpr
#endif

#if defined(_M_X64) || defined(_M_AMD64)
#define BIT64 1 // 64 位操作系统
#elif defined(_M_IX86)
#define BIT32 1 // 32 位操作系统
#endif


#else
#error not supported target platform.
#endif

//! 是否使用 venus 名名空间
#define VENUS_USE_NAMESPACE
//! 是否跟踪内存泄露
#define VENUS_MEMORY_CHYECK
