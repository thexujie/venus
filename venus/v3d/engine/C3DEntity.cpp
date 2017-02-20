#include "stdafx.h"
#include "C3DEntity.h"

VENUS_BEG

C3DEntity::C3DEntity(I3DExplorer * pExplorer) :
	C3DNode(pExplorer),
	m_pMesh(nullptr), m_pEffect(nullptr), m_iAction(-1), m_iTime(0)
{
	m_transform.identity();
}

C3DEntity::~C3DEntity()
{
	SafeRelease(m_pMesh);
	SafeRelease(m_pEffect);
	for(int_x cnt = 0, size = m_actions.size(); cnt < size; ++cnt)
		SafeRelease(m_actions[cnt]);
}

void C3DEntity::SetTransform(const float4x4 & matr)
{
	m_transform = matr;
}

float4x4 C3DEntity::GetTransform() const
{
	return m_transform;
}

err_t C3DEntity::Load(const char_16 * szEntity)
{
	if(!szEntity)
		return err_invalidarg;

	char_16 szObject[MAX_FILE_PATH] = {};
	textformat(szObject, MAX_FILE_PATH, L"objects/%s", szEntity);
	I3DFile * pFileData = m_pExplorer->LoadFile(szObject);
	if(!pFileData)
		return err_no_file;

	const char_16 * szText = (const char_16 *)pFileData->GetBuffer();
	int_x iLength = pFileData->GetSize();
	xml_doc doc;
	err_t err = doc.load_text(szText, iLength / 2);
	SafeRelease(pFileData);
	if(err)
		return err;

	const xml_attr & attr_mesh= doc(L"mesh")[L"file"];
	if(attr_mesh.is_valid())
		m_pMesh = m_pExplorer->GetMesh(attr_mesh.value);

	if(!m_pMesh)
		return err_ok;

	const xml_attr & attr_effect = doc[L"effect"];
	if(attr_effect.is_valid())
	{
		m_pEffect = m_pExplorer->GetEffect(attr_effect.value);
		SafeAddRef(m_pEffect);
	}

	int_x iNumNodes = m_pMesh->GetNodeCount();
	m_textures.resize(iNumNodes);
	const xml_node & node_materials = doc(L"materials");
	if(node_materials.is_valid())
	{
		for(const xml_node & node_material : node_materials.children)
		{
			int_x inode = node_material[L"subset"].value.toix(-1);
			if(inode < 0 || iNumNodes <= inode)
				continue;

			vector<texture_t> & textures = m_textures[inode];
			for(const xml_node & node_param: node_material.children)
			{
				if(node_param.name.equal(L"texture", -1, false))
				{
					const xml_attr & name = node_param[L"name"];
					const xml_attr & file = node_param[L"file"];

					texture_t & texture = textures.add();
					texture.id = m_pEffect->FindParam(name.value);
					texture.texture = m_pExplorer->GetTexture(file.value);
					SafeAddRef(texture.texture);
				}
			}
		}
	}

	const xml_node & actions = doc(L"actions");
	if(actions.is_valid())
	{
		int_x iCount = actions.children.size();
		m_actions.resize(iCount);
		for(int_x cnt = 0; cnt < iCount; ++cnt)
		{
			const xml_node & node = actions.children[cnt];
			m_actions[cnt] = m_pExplorer->GetAction(node.text);
		}
	}
	return err_ok;
}

err_t C3DEntity::SetMesh(I3DMesh * pMesh)
{
	SafeRelease(m_pMesh);
	m_pMesh = pMesh;
	SafeAddRef(m_pMesh);
	return err_ok;
}

err_t C3DEntity::SetAction(int_x iAction)
{
	if(iAction >= 0 && iAction >= m_actions.size())
		return err_invalidarg;

	m_iAction = iAction;
	if(m_iAction >= 0)
	{
		I3DAction * pAction = m_actions[m_iAction];
		pAction->GetPosture(m_posture, m_iTime);
	}
	else
		m_posture.resize(0);
	return err_ok;
}

void C3DEntity::Update(int_x iPeriod)
{
	if(iPeriod && m_iAction >= 0)
	{
		m_iTime += iPeriod;
		I3DAction * pAction = m_actions[m_iAction];
		pAction->GetPosture(m_posture, m_iTime);
	}
}

err_t C3DEntity::PassBeg(I3DRenderEngine * pRenderEngine, renderpass_e pass)
{
	//if(pass == renderpass_gbuffer)
	{
		pRenderEngine->AddRenderable(m_pEffect->GetPass(pass), this);
		return err_ok;
	}
	//else
		//return err_false;
}

err_t C3DEntity::PassEnd(I3DRenderEngine * pRenderEngine, renderpass_e pass)
{
	return err_ok;
}

err_t C3DEntity::Render(I3DContext * pContext, I3DPass * pPass) const
{
	I3DBuffer * pvtxBuffer = m_pMesh->GetVBuffer();
	I3DBuffer * pidxBuffer = m_pMesh->GetIBuffer();
	vertexformat_e vtxFormat = m_pMesh->GetVertexFormat();
	indexformat_e idxFormat = m_pMesh->GetIndexFormat();
	primitiveformat_e priFormat = m_pMesh->GetPrimitiveFormat();
	const vector<mesh_node_t> & nodes = m_pMesh->GetNodes();
	if(!pvtxBuffer || nodes.empty())
		return err_ok;

	I3DCamera * pCamera = pContext->GetCamera();
	if(!pCamera)
		return err_invalidcall;

	float4x4 matrWVP = m_transform * pCamera->GetMatrixVP();
	SafeRelease(pCamera);

	pPass->SetVariable(L"matrWVP", &(matrWVP), sizeof(matrWVP));
	pPass->SetVariable(L"posture", m_posture.buffer(), m_posture.size() * sizeof(float4x4));

	pContext->SetVertexBuffer(pvtxBuffer, vtxFormat);
	pContext->SetIndexBuffer(pidxBuffer, idxFormat);

	for(int_x isubset = 0; isubset < nodes.size(); ++isubset)
	{
		for(const texture_t & texture : m_textures[isubset])
			pPass->SetTexture(texture.id, texture.texture);

		const mesh_node_t & node = nodes[isubset];
		pContext->DrawIndexed(node.vertex_offset, node.index_offset, node.index_count, priFormat);
	}

	return err_ok;
}

VENUS_END
