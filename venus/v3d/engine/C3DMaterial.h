#pragma once

VENUS_BEG

struct mtl_texture_t
{
	chbufferw<EFFECT_MAX_IDEITIFIER> name;
	int_x subset;
	eff_paramid_t id;
	I3DTexture * texture;
};

class V3D_API C3DMaterial : public ObjectT<I3DMaterial>
{
public:
	C3DMaterial(I3DExplorer * pExplorer);
	~C3DMaterial();

	err_t Load(I3DFile * pFile);

	err_t SetEffect(I3DEffect * pEffect);
	err_t SetTexture(eff_paramid_t id, I3DTexture * pTexture);
	err_t SetTexture(const char_16 * name, I3DTexture * pTexture);

	I3DEffect * GetEffect() const;
	I3DTexture * GetTexture(eff_paramid_t id) const;
	I3DTexture * GetTexture(const char_16 * name) const;

	err_t RenderBeg(I3DContext * pContext, int_x iSubSet);
	err_t RenderEnd(I3DContext * pContext, int_x iSubSet);
protected:

	I3DExplorer * m_pExplorer;
	I3DEffect * m_pEffect;
	vector<mtl_texture_t> m_textures;
};

VENUS_END
