#include "stdafx.h"
#include "C3DScene.h"

VENUS_BEG

C3DScene::C3DScene(I3DExplorer * pExplorer) :
	m_pExplorer(pExplorer), m_pCamera(nullptr), m_iTimeStart(time_ms())
{
	FakeAddRef(m_pExplorer);
}

C3DScene::~C3DScene()
{
	SafeRelease(m_pCamera);
	for(int_x cnt = 0, len = m_nodes.size(); cnt < len; ++cnt)
		SafeRelease(m_nodes[cnt]);
	FakeRelease(m_pExplorer);
}

void C3DScene::AddNode(I3DNode * pNode)
{
	m_nodes.add(pNode);
	SafeAddRef(pNode);
	pNode->OnAdded(this);
}

void C3DScene::RmvNode(I3DNode * pNode)
{
	m_nodes.remove(pNode);
	pNode->OnRemoved();
	SafeRelease(pNode);
}

void C3DScene::RmvAllNode()
{
	while(m_nodes.is_valid())
	{
		I3DNode *& pNode = m_nodes.back();
		pNode->OnRemoved();
		SafeRelease(pNode);
		m_nodes.remove(pNode);
	}
}

bool C3DScene::ContainNode(I3DNode * pNode) const
{
	return m_nodes.find(pNode) >= 0;
}

bbox_t C3DScene::GetBoundingBox() const
{
	return bbox_t();
}

void C3DScene::Transform(const float4x4 & matr)
{
	throw exp_not_supported();
}

err_t C3DScene::Update(int_x iPeriod)
{
	for(int_x cnt = 0, len = m_nodes.size(); cnt < len; ++cnt)
		m_nodes[cnt]->Update(iPeriod);
	return err_ok;
}

err_t C3DScene::Render(I3DRenderEngine * pRenderEngine)
{
	Verify(m_pCamera);
	for(int_x cnt = 0, len = m_nodes.size(); cnt < len; ++cnt)
		pRenderEngine->AddObject(m_nodes[cnt]);
	return err_ok;
}

void C3DScene::SetCamera(I3DCamera * pCamera)
{
	SafeRelease(m_pCamera);
	m_pCamera = pCamera;
	SafeAddRef(m_pCamera);
}

int_x C3DScene::GetTime() const
{
	int_64 iTimeNow = time_ms();
	return (int_x)(iTimeNow - m_iTimeStart);
}

I3DVedio * C3DScene::GetVedio() const
{
	return m_pExplorer->GetVedio();
}

IPlugin * C3DScene::GetPlugin() const
{
	return nullptr;
}

I3DExplorer * C3DScene::GetExplorer() const
{
	return m_pExplorer;
}

I3DCamera * C3DScene::GetCamera() const
{
	return m_pCamera;
}

VENUS_END
