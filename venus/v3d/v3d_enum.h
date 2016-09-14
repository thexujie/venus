#pragma once

VENUS_BEG

enum ResourceTypeE
{
	ResourceTypeDefault = 0,
	ResourceTypeTexture,
	ResourceTypeMesh,
	ResourceTypeTerrain,
	ResourceTypeObject,
	ResourceTypeAction,
	ResourceTypeMaterial,
	ResourceTypeEffect,
};

enum InputDeviceE
{
	InputDeviceUnknown = 0,
	InputDeviceMouse,
	InputDeviceKeyBoard,
};

enum VedioModeE
{
	VedioModeDefalult = 0,
	VedioModeNull,
	VedioModeHardware,
	VedioModeReference,
	VedioModeSoftware,
	VedioModeWrap,
};

enum NodeTypeE
{
	NodeTypeUnknown = 0,
	NodeTypeScene,
	NodeTypeBasic,
	NodeTypeAnimate,
};

enum renderpass_e
{
	renderpass_normal = 0,
	renderpass_gbuffer,
	renderpass_shadowmap,
};

enum primitiveformat_e
{
	primitiveformat_invalid,
	primitiveformat_tri_list,
	primitiveformat_tri_strip,
	primitiveformat_tri_patch,
};

enum shader_e
{
	shader_invalid = -1,
	shader_vs,
	shader_ps,
	shader_hs,
	shader_ds,
	shader_gs,
	shader_cs,
	shader_num,
};

inline constfun int_32 eff_param_type_make(int_32 type, int_32 row, int_32 col)
{
	return (type & 0xf) | ((row & 0xf) << 4) | ((col & 0xf) << 8);
}

enum eff_param_type_e
{
	eff_param_type_none = 0,
	eff_param_type_mask_type = 0x000f,
	eff_param_type_mask_row = 0x00f0,
	eff_param_type_mask_col = 0x0f00,
	eff_param_type_mask_array = 0xf000,
	eff_param_type_count_unkown = 0xf,

	eff_param_type_type_bool = 0x0001,
	eff_param_type_type_int,
	eff_param_type_type_float,
	eff_param_type_type_user,

	eff_param_type_bool1 = eff_param_type_make(eff_param_type_type_bool, 1, 1),
	eff_param_type_bool2 = eff_param_type_make(eff_param_type_type_bool, 1, 2),
	eff_param_type_bool3 = eff_param_type_make(eff_param_type_type_bool, 1, 3),
	eff_param_type_bool4 = eff_param_type_make(eff_param_type_type_bool, 1, 4),
	eff_param_type_bool4x4 = eff_param_type_make(eff_param_type_type_bool, 4, 4),
	eff_param_type_bool4x4xX = eff_param_type_make(eff_param_type_type_bool, 0xf, 0xf),

	eff_param_type_int1 = eff_param_type_make(eff_param_type_type_int, 1, 1),
	eff_param_type_int2 = eff_param_type_make(eff_param_type_type_int, 1, 2),
	eff_param_type_int3 = eff_param_type_make(eff_param_type_type_int, 1, 3),
	eff_param_type_int4 = eff_param_type_make(eff_param_type_type_int, 1, 4),
	eff_param_type_int4x4 = eff_param_type_make(eff_param_type_type_int, 4, 4),

	eff_param_type_float1 = eff_param_type_make(eff_param_type_type_float, 1, 1),
	eff_param_type_float2 = eff_param_type_make(eff_param_type_type_float, 1, 2),
	eff_param_type_float3 = eff_param_type_make(eff_param_type_type_float, 1, 3),
	eff_param_type_float4 = eff_param_type_make(eff_param_type_type_float, 1, 4),
	eff_param_type_float4x4 = eff_param_type_make(eff_param_type_type_float, 4, 4),

	eff_param_type_struct = eff_param_type_make(eff_param_type_type_user, 1, 0),

	eff_param_type_resource_min = 0x00010000,
	eff_param_type_texture_1d,
	eff_param_type_texture_2d,
	eff_param_type_texture_3d,
	eff_param_type_texture_1dxX,
	eff_param_type_texture_2dxX,
	eff_param_type_texture_3dxX,
	eff_param_type_buffer,
	eff_param_type_sampler,
	eff_param_type_resource_max = 0x00ff0000,
};

enum vertexformat_e
{
	//! 未知
	vertexformat_unknown = 0,
	vertexformat_pos = 0x0001,
	vertexformat_nor = 0x0002,
	vertexformat_bin = 0x0004,
	vertexformat_tan = 0x0008,

	vertexformat_tex = 0x0100,
	VertexFormat_tex1 = 0x0200,
	VertexFormat_tex2 = 0x0300,
	VertexFormat_tex3 = 0x0400,
	VertexFormat_tex4 = 0x0500,
	VertexFormat_tex5 = 0x0600,
	VertexFormat_tex6 = 0x0700,
	VertexFormat_tex7 = 0x0800,

	vertexformat_wgt = 0x4000,

	//! 位置 纹理坐标x1。
	vertexformat_pos_tex = vertexformat_pos | vertexformat_tex,
	//! 位置 法线 纹理坐标
	vertexformat_pos_tex_nor = vertexformat_pos | vertexformat_tex | vertexformat_nor,
	vertexformat_pos_tex_wgt = vertexformat_pos | vertexformat_tex | vertexformat_wgt,
	vertexformat_pos_tex_nor_wgt = vertexformat_pos | vertexformat_tex | vertexformat_nor | vertexformat_wgt,
	vertexformat_pos_tex_nor_bin_tan = vertexformat_pos | vertexformat_tex | vertexformat_nor | vertexformat_bin | vertexformat_tan,
};

constfun vertexformat_e operator | (vertexformat_e e0, vertexformat_e e1) { return (vertexformat_e)((int_32)e0 | (int_32)e1); }

enum indexformat_e
{
	indexformat_unkown = 0,
	indexformat_16,
	indexformat_32,
};

enum bufferbind_e
{
	bufferbind_none = 0x0000,
	bufferbind_resource = 0x0001,
	bufferbind_index = 0x0002,
	bufferbind_vertex = 0x0004,
	bufferbind_const = 0x0008,
	bufferbind_rendertarget = 0x0010,
	bufferbind_depthstencil = 0x0020,
};
constfun bufferbind_e operator | (bufferbind_e e0, bufferbind_e e1) { return (bufferbind_e)((int_32)e0 | (int_32)e1); }

enum bufferusage_e
{
	//! GPU 快速读写，CPU 慢速写，只能用 UpdateSubresource 来更新。
	bufferusage_default = 0,
	//! GPU 慢速只读，CPU 快速写。只能用 Map 来更新。
	bufferusage_dynamic,
	//! GPU 快速只读，CPU 无法读写
	bufferusage_immutable,

	bufferusage_staging,
};

enum bufferaccess_e
{
	bufferaccess_none = 0,
	bufferaccess_read,
	bufferaccess_write,
	bufferaccess_readwrite,
};

enum BufferFlagE
{
	BufferFlagGdiCompatible = 0x0001,
};

enum buffermap_e
{
	buffermap_read = 0,
	buffermap_write,
	buffermap_readwrite,
	buffermap_write_discard,
	buffermap_write_nooverwrite,
};

enum DepthStencilFormatE
{
	DepthStencilFormatD24S8,
};

enum fitermode_e
{
	fitermode_point = 0,
	fitermode_linear,
	fitermode_anisotropy,
};

enum addressmode_e
{
	addressmode_wrap = 0,
	addressmode_mirror,
	addressmode_clamp,
	addressmode_border,
};

enum compare_e
{
	compare_never = 0,
	compare_always,

	compare_equal,
	compare_notequal,

	compare_less,
	compare_lessequal,

	compare_greater,
	compare_greaterequal,
};

enum cullmode_e
{
	cullmode_none = 0,
	cullmode_front,
	cullmode_back,
};

enum fillmode_e
{
	fillmode_solid = 0,
	fillmode_frame,
};

enum blendfactor_e
{
	blendfactor_zero = 0,
	blendfactor_one,

	blendfactor_srcalpha,
	blendfactor_dstalpha,
	blendfactor_srcalpha_inv,
	blendfactor_dstalpha_inv,

	blendfactor_srccolor,
	blendfactor_dstcolor,
	blendfactor_srccolor_inv,
	blendfactor_dstcolor_inv,

	blendfactor_user,
	blendfactor_user_inv,
};

enum blendop_e
{
	blendop_add = 0,
	blendop_sub,
	blendop_revert_sub,

	blendop_min,
	blendop_max,
};

enum stencilop_e
{
	stencilop_none = 0,
	stencilop_zero,
	stencilop_replace,
	stencilop_inc_clamp,
	stencilop_dec_clamp,
	stencilop_inc_wrap,
	stencilop_dec_wrap,
	stencilop_invert,
};

/**
* 输入布局中的 element 格式
*/
enum il_valuetype_e
{
	valuetype_invalid = 0,

	il_valuetype_float1,
	il_valuetype_float2,
	il_valuetype_float3,
	il_valuetype_float4,
	il_valuetype_float4x4,
};

VENUS_END
