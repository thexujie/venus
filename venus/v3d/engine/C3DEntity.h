#pragma once

VENUS_BEG

class V3D_API C3DEntity : public C3DNode<I3DEntity, I3DRenderable>
{
	typedef C3DNode<I3DEntity> ParentT;
public:
	C3DEntity(I3DExplorer * pExplorer);
	~C3DEntity();

	void SetTransform(const float4x4 & matr);
	float4x4 GetTransform() const;

	err_t Load(const char_16 * szEntity);

	err_t SetMesh(I3DMesh * pMesh);
	err_t SetAction(int_x iAction);

	void Update(int_x iPeriod);

	err_t PassBeg(I3DRenderEngine * pRenderEngine, renderpass_e pass);
	err_t PassEnd(I3DRenderEngine * pRenderEngine, renderpass_e pass);
	err_t Render(I3DContext * pContext, I3DPass * pPass) const;

private:
	I3DMesh * m_pMesh;
	I3DEffect * m_pEffect;
	vector<I3DAction *> m_actions;

	float4x4 m_transform;
	vector<float4x4> m_posture;
	int_x m_iAction;
	int_x m_iTime;

	struct texture_t
	{
		eff_paramid_t id;
		I3DTexture * texture;
	};
	vector<vector<texture_t>> m_textures;
};

VENUS_END
