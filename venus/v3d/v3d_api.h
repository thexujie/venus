#pragma once

VENUS_BEG

V3D_API int_x GetVertexStrideFromFormat(vertexformat_e eVertexFormat);
V3D_API int_x GetIndexStrideFromFormat(indexformat_e eIndexFormat);
V3D_API int_x GetIndexedPrimitiveCount(primitiveformat_e ePrimitiveFormat, int_x iIndexCount);

V3D_API il_valuetype_e VnMapValueType(const textw & str);
V3D_API shader_e VnMapShaderType(const textw & str);

class V3D_API V3d
{
public:
	V3d() = delete;
	~V3d() = delete;

	static void Parse(fitermode_e & mode, const textw & text);
	static void Parse(addressmode_e & mode, const textw & text);
	static void Parse(compare_e & mode, const textw & text);
	static void Parse(cullmode_e & mode, const textw & text);
	static void Parse(fillmode_e & mode, const textw & text);
	static void Parse(blendfactor_e & mode, const textw & text);
	static void Parse(blendop_e & mode, const textw & text);
	static void Parse(stencilop_e & mode, const textw & text);
};

VENUS_END
