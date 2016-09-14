#pragma once

VENUS_BEG

const oid_t OID_I3DApp = {L"I3DApp"};
const oid_t OID_I3DFileDriver = {L"I3DFileDriver"};
const oid_t OID_I3DVedioDriver = {L"I3DVedioDriver"};
const oid_t OID_I3DAudioDriver = {L"I3DAudioDriver"};

const char_16 SHADER_NAME[shader_num][3] = {L"vs", L"ps", L"hs", L"ds", L"gs", L"cs"};

const int_x RENDER__MAX_RENDERTARGET = 8;

const int_x EFFECT_MAX_IDEITIFIER = 32;

const int_x EFFECT_MAX_VERTEX_BUFFER = 32;
const int_x EFFECT_MAX_CONST_BUFFER = 14;
const int_x EFFECT_MAX_SRV = 128;
const int_x EFFECT_MAX_SAMPLER = 16;

// ia 最多的元素数量
const int_x EFFECT_IA_MAX_ELEMENTS = 32;

struct multisample_t
{
	int_x count;
	int_x quality;
};

const multisample_t multisample_1 = {1, 0};
const multisample_t multisample_2 = {2, 0};
const multisample_t multisample_3 = {3, 0};
const multisample_t multisample_4 = {4, 0};
const multisample_t multisample_5 = {5, 0};
const multisample_t multisample_6 = {6, 0};
const multisample_t multisample_7 = {7, 0};
const multisample_t multisample_8 = {8, 0};
const multisample_t multisample_9 = {9, 0};
const multisample_t multisample_10 = {10, 0};
const multisample_t multisample_11 = {11, 0};
const multisample_t multisample_12 = {12, 0};
const multisample_t multisample_13 = {13, 0};
const multisample_t multisample_14 = {14, 0};
const multisample_t multisample_15 = {15, 0};
const multisample_t multisample_16 = {16, 0};

struct eff_paramid_t
{
	eff_paramid_t() :id(-1) {}
	eff_paramid_t(int_x _id) :id(_id) {}
	eff_paramid_t(const eff_paramid_t & another) = default;

	operator int_x() const { return id; }
	eff_paramid_t & operator = (const eff_paramid_t & another) = default;

	int_x id;
#ifdef _DEBUG
	chbufferw<EFFECT_MAX_IDEITIFIER> name;
#endif
};

struct shader_cbuffer_t
{
	chbufferw<EFFECT_MAX_IDEITIFIER> name;
	shader_e shader;
	int_x slot;
	int_x size;
};

struct sampler_state_t
{
	fitermode_e filtermode;
	addressmode_e addressmode_u;
	addressmode_e addressmode_v;
	addressmode_e addressmode_w;
	int_x anisotropy;
	compare_e cmp;
};

struct shader_param_t
{
	chbufferw<EFFECT_MAX_IDEITIFIER> name;
	eff_param_type_e type;
	int_x size;
	int_x slots[shader_num];
	int_x offsets[shader_num];
};

struct param_t
{
	const void * pointer;
	int_x size;
};

/**
* 帧缓冲属性
*/
struct FrameBufferDescT
{
	int_x Width;
	int_x Height;
	cmode_e ColorMode;
	int_x iCount;

	int_x SamplerCount;
	int_x SamplerQuality;
};

struct stencil_formula_t
{
	compare_e cmp;
	stencilop_e op_pass;
	stencilop_e op_fail;
	stencilop_e op_depth_fail;
};

struct blend_formula_t
{
	blendfactor_e src;
	blendop_e op;
	blendfactor_e dst;
};

struct passstate_t
{
	chbufferw<EFFECT_MAX_IDEITIFIER> name;
	chbufferw<MAX_FILE_PATH> shader;
	chbufferw<EFFECT_MAX_IDEITIFIER> main[shader_num];

	// rasterize
	cullmode_e cullmode;
	fillmode_e fillmode;
	bool clockwise;
	bool multisampler;
	bool scissor;
	int_x z_bias;
	float_32 z_bias_clamp;
	float_32 z_bias_slope_scaled;
	bool z_clip;

	// depth stencil
	bool depth_test;
	compare_e depth_cmp;
	bool depth_write;

	bool stencil_test;
	uint_8 stencil_read;
	uint_8 stencil_write;
	uint_8 stencil_ref;

	compare_e stencil_front_cmp;
	stencilop_e stencil_front_op_fail;
	stencilop_e stencil_front_op_pass;
	stencilop_e stencil_front_op_depth_fail;
	compare_e stencil_back_cmp;
	stencilop_e stencil_back_op_fail;
	stencilop_e stencil_back_op_pass;
	stencilop_e stencil_back_op_depth_fail;

	// blend
	bool blend_alpha2coverage;
	bool blend_enable;
	blendfactor_e blend_color_src;
	blendop_e blend_color_op;
	blendfactor_e blend_color_dst;
	blendfactor_e blend_alpha_src;
	blendop_e blend_alpha_op;
	blendfactor_e blend_alpha_dst;
	uint_32 blend_write_mask;

	float_32 blend_factor[4];
	uint_32 blend_sample_mask;
};

struct PipelineStateT
{
	// RasterizerStateT
	cullmode_e CullMode;
	fillmode_e FillMode;
	bool ClockWise;

	bool AntiAliase;
	bool DepthClip;
	bool MultiSample;
	bool Scissor;

	float_32 DepthBias;
	float_32 DepthBiasMax;


	// BlendStateT
	bool BlendEnable;
	bool AlphaToCoverage;

	// src.rgb * SrcColor <BlendColor> dst.rgb + Dst.Color
	blendfactor_e SrcColor;
	blendfactor_e DstColor;
	blendop_e BlendColor;

	// src.a * SrcAlpha + dst.a <BlendAlpha> Dst.DstAlpha
	blendfactor_e SrcAlpha;
	blendfactor_e DstAlpha;
	blendop_e BlendAlpha;

	uint_32 WriteMask;

	bool depth;
	compare_e depth_cmp;
	bool depth_write;
};

/**
* 光栅器状态
*/
struct RasterizerStateT
{
	cullmode_e CullMode;
	fillmode_e FillMode;
	bool ClockWise;

	bool AntiAliase;
	bool DepthClip;
	bool MultiSample;
	bool Scissor;

	float_32 DepthBias;
	float_32 DepthBiasMax;
};

/**
* 混合器状态
*/
struct BlendStateT
{
	bool BlendEnable;
	bool AlphaToCoverage;

	blendfactor_e SrcColor;
	blendfactor_e DstColor;
	blendop_e BlendColor;

	blendfactor_e SrcAlpha;
	blendfactor_e DstAlpha;
	blendop_e BlendAlpha;

	uint_32 WriteMask;
};

/**
* 采样器状态
*/
struct SamplerStateT
{
	fitermode_e FilterMode;
	addressmode_e AddressModeU;
	addressmode_e AddressModeV;
	addressmode_e AddressModeW;
	int_x Anisotropy;
	compare_e ComparisonFun;
};

struct InputElementT
{
	il_valuetype_e format;
	chbuffera<32> desc;
};

/**
* 基本的变换
*/
struct CBTransformT
{
	float4x4 matrWorld;
	float4x4 matrView;
	float4x4 matrProj;
};

struct IntersectPointT
{
	int_x Node;
	int_x Face;
	int_x Index0, Index1, Index2;

	int_x NodeFace;
	int_x NodeIndex0, NodeIndex1, NodeIndex2;

	float_32 PosU, PosV, PosD;
	float3 Pos;
};

class V3D_API MaterialDataT
{
public:
	int_x Size;
	const byte_t * Buffer;
};

struct vertex_pt
{
	float3 position;
	float2 texcoord;
};

struct vertex_pnt
{
	float3 position;
	float3 normal;
	float2 texcoord;
};

struct vertex_ptw4
{
	float3 position;
	float2 texcoord;
	float4 weight;
};

struct SampleDescT
{
	int_x Count;
	int_x Quality;
};

struct ResourceDescT
{
	bufferusage_e Usage;
	bufferbind_e Bind;
	bufferaccess_e Access;
};

struct TextureDescT
{
	sizeix size;
	cmode_e CMode;

	int_x MipLevel;

	SampleDescT SampleDesc;
	bufferusage_e Usage;
	bufferbind_e Bind;
	bufferaccess_e Access;
};

struct MappedBufferT
{
	byte_t * Data;
	int_x Lengh;
	int_x Pitch;
};

struct mesh_node_t
{
	int_x vertex_offset;
	int_x vertex_count;

	int_x index_offset;
	int_x index_count;
};

struct eff_param_float4x4xX_t
{
	shader_e shader;
	eff_param_type_e type;
	// bind slot.
	int_x slot;
	// offset in byts.
	int_x offset;
	// size in bytes.
	int_x size;
	function<err_t, shader_e, int_x, int_x, const float4x4 *> fun_set;
#ifdef _DEBUG
	chbuffera<EFFECT_MAX_IDEITIFIER> name;
#endif
	eff_param_float4x4xX_t & operator = (const float4x4 * value)
	{
		fun_set(shader, slot, offset, value);
		return *this;
	}
};

struct BaseTransformT
{
	float4x4 matrW;
	float4x4 matrV;
	float4x4 matrP;
	float4x4 matrWV;
	float4x4 matrVP;
	float4x4 matrWVP;
};

VENUS_END
