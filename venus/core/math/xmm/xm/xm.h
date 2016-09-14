#pragma once

//! ∆Ù”√ SSE
#define VXM_SSE
//#define VXM_DX9
//#define VXM_XMATH

#ifdef VXM_SSE
#include <intrin.h>
#elif defined VXM_DX9
#pragma push_macro("new")
#undef new
#include <d3dx9.h>
#pragma pop_macro("new")
#elif defined VXM_XMATH
#include <DirectXMath.h>
using namespace DirectX;
#endif

VENUS_BEG

#define xm_shuffle(fp0, fp1, fp2, fp3) (((fp0) << 0) | ((fp1) << 2) | ((fp2) << 4) | ((fp3 << 6)))

VENUS_END
