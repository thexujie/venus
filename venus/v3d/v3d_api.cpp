#include "stdafx.h"
#include "v3d_api.h"

VENUS_BEG

template<typename MappingT, int_x COUNT>
static inline auto _mapping_match(MappingT(&mappings)[COUNT], const textw & text, int_x idefault = 0)
{
	for(int_x cnt = 0; cnt < COUNT; ++cnt)
	{
		const MappingT & mapping = mappings[cnt];
		if(text.equals(mapping.text, textlen(mapping.text), false))
			return mapping.mode;
	}

	return idefault == -1 ? mappings[COUNT - 1].mode : mappings[idefault].mode;
}

int_x GetVertexStrideFromFormat(vertexformat_e eVertexFormat)
{
	int_x iSize = 0;
	if(eVertexFormat & vertexformat_pos)
		iSize += sizeof(float3);
	if(eVertexFormat & vertexformat_nor)
		iSize += sizeof(float3);
	if(eVertexFormat & vertexformat_tex)
		iSize += sizeof(float2);
	if(eVertexFormat & vertexformat_bin)
		iSize += sizeof(float3);
	if(eVertexFormat & vertexformat_tan)
		iSize += sizeof(float4);
	if(eVertexFormat & vertexformat_wgt)
		iSize += sizeof(float4);
	return iSize;
}

int_x GetIndexStrideFromFormat(indexformat_e eIndexFormat)
{
	switch(eIndexFormat)
	{
	case indexformat_16:
		return 2;
	case indexformat_32:
		return 4;
	default:
		throw exp_not_supported();
	}
}

int_x GetIndexedPrimitiveCount(primitiveformat_e ePrimitiveFormat, int_x iIndexCount)
{
	switch(ePrimitiveFormat)
	{
	case primitiveformat_tri_list:
		return iIndexCount / 3;
	default:
		throw exp_not_supported();
	}
}

il_valuetype_e VnMapValueType(const textw & str)
{
	if(str.equals(L"float1", 6, false))
		return il_valuetype_float1;
	else if(str.equals(L"float2", 6, false))
		return il_valuetype_float2;
	else if(str.equals(L"float3", 6, false))
		return il_valuetype_float3;
	else if(str.equals(L"float4", 6, false))
		return il_valuetype_float4;
	else
		return valuetype_invalid;
}

shader_e VnMapShaderType(const textw & str)
{

	if(str.equals(L"vs", 2, false))
		return shader_vs;
	else if(str.equals(L"hs", 2, false))
		return shader_hs;
	else if(str.equals(L"ds", 2, false))
		return shader_ds;
	else if(str.equals(L"ps", 2, false))
		return shader_ps;
	else if(str.equals(L"cs", 2, false))
		return shader_cs;
	else
		return shader_invalid;
}

void V3d::Parse(fitermode_e & mode, const textw & text)
{
	struct _mapping_t
	{
		fitermode_e mode;
		const char_16 * text;
	};

	const _mapping_t mappings[] =
	{
		{fitermode_point, L"point"},
		{fitermode_linear, L"linear"},
		{fitermode_anisotropy, L"anisotropy"},
	};
	mode = _mapping_match(mappings, text);
}

void V3d::Parse(addressmode_e & mode, const textw & text)
{
	struct _mapping_t
	{
		addressmode_e mode;
		const char_16 * text;
	};

	const _mapping_t mappings[] =
	{
		{addressmode_wrap, L"wrap"},
		{addressmode_mirror, L"mirror"},
		{addressmode_clamp, L"clamp"},
		{addressmode_border, L"border"},
	};
	mode = _mapping_match(mappings, text);
}

void V3d::Parse(compare_e & mode, const textw & text)
{
	struct _mapping_t
	{
		compare_e mode;
		const char_16 * text;
	};

	const _mapping_t mappings[] =
	{
		{compare_never, L"never"},
		{compare_always, L"always"},
		{compare_equal, L"equal"},
		{compare_notequal, L"notequal"},
		{compare_less, L"less"},
		{compare_lessequal, L"lessequal"},
		{compare_greater, L"greater"},
		{compare_greaterequal, L"greaterequal"},
	};
	mode = _mapping_match(mappings, text);
}

void V3d::Parse(cullmode_e & mode, const textw & text)
{
	struct _mapping_t
	{
		cullmode_e mode;
		const char_16 * text;
	};

	const _mapping_t mappings[] =
	{
		{cullmode_none, L"none"},
		{cullmode_front, L"front"},
		{cullmode_back, L"back"},
	};
	mode = _mapping_match(mappings, text);
}

void V3d::Parse(fillmode_e & mode, const textw & text)
{
	struct _mapping_t
	{
		fillmode_e mode;
		const char_16 * text;
	};

	const _mapping_t mappings[] =
	{
		{fillmode_solid, L"Solid"},
		{fillmode_frame, L"Wireframe"},
	};
	mode = _mapping_match(mappings, text);
}

void V3d::Parse(blendfactor_e & mode, const textw & text)
{
	struct _mapping_t
	{
		blendfactor_e mode;
		const char_16 * text;
	};

	const _mapping_t mappings[] =
	{
		{blendfactor_zero, L"zero"},
		{blendfactor_one, L"one"},
		{blendfactor_srcalpha, L"srcalpha"},
		{blendfactor_dstalpha, L"dstalpha"},
		{blendfactor_srcalpha_inv, L"srcalphainv"},
		{blendfactor_dstalpha_inv, L"dstalphainv"},
		{blendfactor_srccolor, L"srccolor"},
		{blendfactor_dstcolor, L"dstcolor"},
		{blendfactor_srccolor_inv, L"srccolorinv"},
		{blendfactor_dstcolor_inv, L"dstcolorinv"},
		{blendfactor_user, L"factorinv"},
		{blendfactor_user_inv, L"one"},
	};
	mode = _mapping_match(mappings, text);
}

void V3d::Parse(blendop_e & mode, const textw & text)
{
	struct _mapping_t
	{
		blendop_e mode;
		const char_16 * text;
	};

	const _mapping_t mappings[] =
	{
		{blendop_add, L"add"},
		{blendop_sub, L"subtract"},
		{blendop_revert_sub, L"revsubtract"},
		{blendop_min, L"min"},
		{blendop_max, L"max"},
	};
	mode = _mapping_match(mappings, text);
}

void V3d::Parse(stencilop_e & mode, const textw & text)
{
	struct _mapping_t
	{
		stencilop_e mode;
		const char_16 * text;
	};

	const _mapping_t mappings[] =
	{
		{stencilop_none, L"none"},
		{stencilop_zero, L"zero"},
		{stencilop_replace, L"replace"},
		{stencilop_inc_clamp, L"inc_clamp"},
		{stencilop_dec_clamp, L"dec_clamp"},
		{stencilop_inc_wrap, L"inc_wrap"},
		{stencilop_dec_wrap, L"dec_wrap"},
	};
	mode = _mapping_match(mappings, text);
}

VENUS_END
