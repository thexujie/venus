#pragma once

#include "stdafx.h"
#include "XmlParser.h"

VENUS_BEG

V3D_API fitermode_e MapFilterMode(const textw & text);
V3D_API addressmode_e MapAddressMode(const textw & text);
V3D_API compare_e MapComparison(const textw & text);
V3D_API cullmode_e MapCullMode(const textw & text);
V3D_API fillmode_e MapFillMode(const textw & text);
V3D_API il_valuetype_e MapElementFormat(const textw & text);
V3D_API blendfactor_e MapBlend(const textw & text);
V3D_API blendop_e MapBlendMode(const textw & text);

V3D_API bool ParseSamplerState(const xml_node & node, SamplerStateT & state);

VENUS_END
