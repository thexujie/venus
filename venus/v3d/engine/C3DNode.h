#pragma once

VENUS_BEG

template<typename ...BaseT>
class C3DNode : public ObjectT<BaseT...>
{
public:
	C3DNode(I3DExplorer * pExplorer) : m_pExplorer(pExplorer), m_eFillMode(fillmode_solid)
	{
	}

	~C3DNode()
	{
	}

	NodeTypeE GetNodeType() const { return NodeTypeUnknown; }
	I3DScene * GetScene() const { return m_pScene; }
	I3DExplorer * GetExplorer() const { return m_pExplorer; }
	I3DVedio * GetVedio() const { return m_pExplorer->GetVedio(); }

	void SetFillMode(fillmode_e eFillMode) { m_eFillMode = eFillMode; }
	fillmode_e GetFillMode() const { return m_eFillMode; }
	void SetTransform(const float4x4 & matr) {}
	float4x4 GetTransform() const { return float4x4(); }

	void OnAdded(I3DScene * pScene) { m_pScene = pScene; }
	void OnRemoved() { m_pScene = nullptr; }

	void Update(int_x iPeriod) {}
	void Render(I3DContext * pContext) {}
	void Render(I3DContext * pContext, I3DEffect * pEffect, int_x iSubSet) const {}
protected:
	I3DScene * m_pScene;
	I3DExplorer * m_pExplorer;

	fillmode_e m_eFillMode;
};

VENUS_END
