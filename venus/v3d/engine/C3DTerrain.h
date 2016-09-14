#pragma once

#include "C3DNode.h"

VENUS_BEG


const int_x TERRAIN_TESS_SIZE = 50;
/**
 * LOD
 * 0 1 * 1
 * 1 2 * 2
 * 2 4 * 4
 * 3 8 * 8
 * 4 16 * 16
 * 5 32 * 32
 * 6 64 * 64
 * 7 128 * 128
 * 8 256 * 256
 */
class V3D_API I3DTerrain
{
public:
	virtual I3DScene * GetScene() const = 0;
	virtual float3 GetStep() const = 0;
	virtual sizeix GetSize() const = 0;
};

struct vertex_ter_t
{
	float3 pos;
	float2 tex;
	// 法线
	float3 nom;
	// 副法线
	float3 bin;
	// 切线
	float3 tan;
};

class V3D_API C3DTerrain : public C3DNode<I3DNode>
{
public:
	C3DTerrain(I3DExplorer * pExplorer);
	~C3DTerrain();

	err_t Load(const char_16 * szTerrain);
	I3DBuffer * GetVBuffer() const;
	I3DBuffer * GetIBuffer() const;

	void Update(int_x iPeriod);
	err_t Render(I3DContext * pContext) const;

	err_t PassBeg(I3DRenderEngine * pRenderEngine, renderpass_e pass);
	err_t PassEnd(I3DRenderEngine * pRenderEngine, renderpass_e pass);
	err_t Render(I3DContext * pContext, I3DPass * pPass) const;

	float GetHeight(vec2ix point) const;
	float3 GetCoords(vec2ix point) const;
protected:
	I3DBuffer * m_pbufDensity;

	I3DBuffer * m_pvb;
	I3DBuffer * m_pib;

	I3DEffect * m_pEffect;
	I3DMaterial * m_pMaterial;
	float_32 m_fHeightScale;

	int_x m_iVtxCount;
	int_x m_iIdxCount;
	int_x m_iTriCount;
	float4x4 m_transform;

	I3DTexture * m_ptexNH;
	float3 m_step;
	sizeix m_size;
	vector<color_argb_t> m_nhs;

	static I3DBuffer * GenerateDensityFromNH(I3DVedio * pVedio, 
		const color_argb_t * nh, sizeix size,
		const vector<vertex_ter_t> & vertices,
		const vector<int_16> & indices,
		float_32 fDensityScale);
};

VENUS_END
