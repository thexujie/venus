#pragma once

VENUS_BEG

class V3D_API C3DScene : public ObjectT<I3DScene>
{
public:
	C3DScene(I3DExplorer * pExplorer);
	~C3DScene();

public: // I3DNode
	void AddNode(I3DNode * pNode);
	void RmvNode(I3DNode * pNode);
	void RmvAllNode();
	bool ContainNode(I3DNode * pNode) const;

	bbox_t GetBoundingBox() const;
	void Transform(const float4x4 & matr);

	err_t Update(int_x iPeriod);
	err_t Render(I3DRenderEngine * pRenderEngine);

public: // I3DScene
	void SetCamera(I3DCamera * pCamera);

	int_x GetTime() const;
	I3DVedio * GetVedio() const;
	IPlugin * GetPlugin() const;
	I3DExplorer * GetExplorer() const;
	I3DCamera * GetCamera() const;
private:
	I3DExplorer * m_pExplorer;
	I3DCamera * m_pCamera;

	vector<I3DLight *> m_lights;
	vector<I3DNode *> m_nodes;
	int_64 m_iTimeStart;
};

VENUS_END
