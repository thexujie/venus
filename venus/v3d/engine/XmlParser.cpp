#include "stdafx.h"
#include "XmlParser.h"

VENUS_BEG

fitermode_e MapFilterMode(const textw & text)
{
	if(text == L"Point")
		return fitermode_point;
	else if(text == L"Linear")
		return fitermode_linear;
	else
		return fitermode_point;
}

addressmode_e MapAddressMode(const textw & text)
{
	if(text == L"Wrap")
		return addressmode_wrap;
	else if(text == L"Mirror")
		return addressmode_mirror;
	else if(text == L"Clamp")
		return addressmode_clamp;
	else if(text == L"Border")
		return addressmode_border;
	else
		return addressmode_wrap;
}

compare_e MapComparison(const textw & text)
{
	if(text.equals(L"Never", -1, false))
		return compare_never;
	else if(text.equals(L"Always", -1, false))
		return compare_always;
	else if(text.equals(L"Equal", -1, false))
		return compare_equal;
	else if(text.equals(L"NotEqual", -1, false))
		return compare_notequal;
	else if(text.equals(L"Less", -1, false))
		return compare_less;
	else if(text.equals(L"LessEqual", -1, false))
		return compare_lessequal;
	else if(text.equals(L"Greater", -1, false))
		return compare_greater;
	else if(text.equals(L"GreaterEqual", -1, false))
		return compare_greaterequal;
	else
		return compare_never;
}

cullmode_e MapCullMode(const textw & text)
{
	if(text.equals(L"None", -1, false))
		return cullmode_none;
	else if(text.equals(L"Front", -1, false))
		return cullmode_front;
	else if(text.equals(L"Back", -1, false))
		return cullmode_back;
	else
		return cullmode_none;
}

fillmode_e MapFillMode(const textw & text)
{
	if(text.equals(L"Solid", -1, false))
		return fillmode_solid;
	else if(text.equals(L"Wireframe", -1, false))
		return fillmode_frame;
	else
		return fillmode_solid;
}

il_valuetype_e MapElementFormat(const textw & text)
{
	if(text.equals(L"float1", -1, false))
		return il_valuetype_float1;
	else if(text.equals(L"float2", -1, false))
		return il_valuetype_float2;
	else if(text.equals(L"float3", -1, false))
		return il_valuetype_float3;
	else if(text.equals(L"float4", -1, false))
		return il_valuetype_float4;
	else
		return valuetype_invalid;
}

blendfactor_e MapBlend(const textw & text)
{
	if(text.equals(L"Zero", -1, false))
		return blendfactor_zero;
	else if(text.equals(L"One", -1, false))
		return blendfactor_one;
	else if(text.equals(L"SrcAlpha", -1, false))
		return blendfactor_srcalpha;
	else if(text.equals(L"DstAlpha", -1, false))
		return blendfactor_dstalpha;
	else if(text.equals(L"SrcAlphaInv", -1, false))
		return blendfactor_srcalpha_inv;
	else if(text.equals(L"DstAlphaInv", -1, false))
		return blendfactor_dstalpha_inv;
	else if(text.equals(L"SrcColor", -1, false))
		return blendfactor_srccolor;
	else if(text.equals(L"DstColor", -1, false))
		return blendfactor_dstcolor;
	else if(text.equals(L"SrcColorInv", -1, false))
		return blendfactor_srccolor_inv;
	else if(text.equals(L"DstColorInv", -1, false))
		return blendfactor_dstcolor_inv;
	else if(text.equals(L"Factor", -1, false))
		return blendfactor_user;
	else if(text.equals(L"FactorInv", -1, false))
		return blendfactor_user_inv;
	else
		return blendfactor_zero;
}

blendop_e MapBlendMode(const textw & text)
{
	if(text.equals(L"Add", -1, false))
		return blendop_add;
	if(text.equals(L"Subtract", -1, false))
		return blendop_sub;
	if(text.equals(L"RevSubtract", -1, false))
		return blendop_revert_sub;
	if(text.equals(L"Min", -1, false))
		return blendop_min;
	if(text.equals(L"Max", -1, false))
		return blendop_max;
	else
		return blendop_add;
}

bool ParseSamplerState(const xml_node & node, SamplerStateT & state)
{
	state.FilterMode = MapFilterMode(node(L"filter").text);
	state.AddressModeU = MapAddressMode(node(L"address_u").text);
	state.AddressModeV = MapAddressMode(node(L"address_v").text);
	state.AddressModeW = MapAddressMode(node(L"address_w").text);
	state.Anisotropy = texttoix<char_16>(node(L"anisotropy").text, -1, 10);
	state.ComparisonFun = MapComparison(node(L"comparison").text);
	return true;
}

VENUS_END
