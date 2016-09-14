#pragma once

VENUS_BEG

class V3D_API C3DExplorer : public ObjectT<I3DExplorer>
{
public:
	C3DExplorer(I3DVedio * pDevice);
	~C3DExplorer();

	void ReleaseResouces();

	I3DVedio * GetVedio();

	I3DFile * LoadFile(const char_16 * szFile, ResourceTypeE eType = ResourceTypeDefault);
	I3DTexture * GetTexture(const char_16 * szFile, ResourceTypeE eType = ResourceTypeTexture);
	I3DMesh * GetMesh(const char_16 * szFile, ResourceTypeE eType = ResourceTypeMesh);
	I3DAction * GetAction(const char_16 * szFile, ResourceTypeE eType = ResourceTypeAction);
	I3DMaterial * GetMaterial(const char_16 * szFile, ResourceTypeE eType = ResourceTypeMaterial);
	I3DEffect * GetEffect(const char_16 * szFile, ResourceTypeE eType = ResourceTypeEffect);
private:
	IApp3D * m_pApp;
	I3DVedio * m_pVedio;

	vector<I3DTexture *> m_textures;
	vector<I3DMaterial *> m_materials;
	vector<I3DEffect *> m_effects;
};

VENUS_END
