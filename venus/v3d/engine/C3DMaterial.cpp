#include "stdafx.h"
#include "C3DMaterial.h"

VENUS_BEG

C3DMaterial::C3DMaterial(I3DExplorer * pExplorer) :
	m_pExplorer(pExplorer), m_pEffect(nullptr)
{
	FakeAddRef(m_pExplorer);
}

C3DMaterial::~C3DMaterial()
{
	for(int_x cnt = 0; cnt < m_textures.size(); ++cnt)
		SafeRelease(m_textures[cnt].texture);
	m_textures.clear();
	SafeRelease(m_pEffect);
	FakeRelease(m_pExplorer);
}

err_t C3DMaterial::Load(I3DFile * pFile)
{
	if(!pFile)
		return err_invalidarg;

	if(!m_pExplorer)
		return err_not_initialized;


	const char_16 * szText = (const char_16 *)pFile->GetBuffer();
	int_x iLength = pFile->GetSize();
	xml_doc doc;
	err_t err = doc.load_text(szText, iLength / 2);
	if(err)
		return err;

	m_pEffect = m_pExplorer->GetEffect(doc[L"effect"].value);
	SafeAddRef(m_pEffect);
	for(int_x inode = 0; inode < doc.children.size(); ++inode)
	{
		const xml_node & node_sbuset = doc.children[inode];
		if(node_sbuset.name != L"subset")
			continue;

		int_x subset = node_sbuset[L"index"].value.toix(-1);
		if(subset < 0)
			continue;

		for(int_x iparam = 0; iparam < node_sbuset.children.size(); ++iparam)
		{
			const xml_node & node_param = node_sbuset.children[iparam];
			if(node_param.name == L"texture")
			{
				const xml_attr & name = node_param[L"name"];
				const xml_attr & file = node_param[L"file"];

				mtl_texture_t & texture = m_textures.add();
				texture.name = name.value.buffer();
				texture.subset = subset;
				texture.id = m_pEffect->FindParam(name.value);
				texture.texture = m_pExplorer->GetTexture(file.value);
				SafeAddRef(texture.texture);
			}
		}
	}
	return err_ok;
}

err_t C3DMaterial::SetEffect(I3DEffect * pEffect)
{
	if(!pEffect)
		return err_nullptr;

	SafeRelease(m_pEffect);
	m_pEffect = pEffect;
	SafeAddRef(pEffect);

	for(int_x cnt = 0; cnt < m_textures.size(); ++cnt)
	{
		m_textures[cnt].id = m_pEffect->FindParam(m_textures[cnt].name);
	}
	return err_ok;
}

err_t C3DMaterial::SetTexture(eff_paramid_t id, I3DTexture * pTexture)
{
	if(id.id < 0)
		return err_invalidarg;

	for(int_x cnt = 0; cnt < m_textures.size(); ++cnt)
	{
		if(m_textures[cnt].id == id)
		{
			SafeRelease(m_textures[cnt].texture);
			m_textures[cnt].texture = pTexture;
			SafeAddRef(pTexture);
			return err_ok;
		}
	}
	return err_no_data;
}

err_t C3DMaterial::SetTexture(const char_16 * name, I3DTexture * pTexture)
{
	eff_paramid_t id = m_pEffect->FindParam(name);
	return SetTexture(id, pTexture);
}

I3DEffect * C3DMaterial::GetEffect() const
{
	return m_pEffect;
}

I3DTexture * C3DMaterial::GetTexture(eff_paramid_t id) const
{
	for(int_x cnt = 0; cnt < m_textures.size(); ++cnt)
	{
		const mtl_texture_t & texture = m_textures[cnt];
		if(texture.id == id)
		{
			return texture.texture;
		}
	}
	return nullptr;
}

I3DTexture * C3DMaterial::GetTexture(const char_16 * name) const
{
	if(!m_pEffect)
		return nullptr;
	return GetTexture(m_pEffect->FindParam(name));
}

err_t C3DMaterial::RenderBeg(I3DContext * pContext, int_x iSubSet)
{
	for(int_x cnt = 0; cnt < m_textures.size(); ++cnt)
	{
		const mtl_texture_t & texture = m_textures[cnt];
		if(texture.subset == iSubSet)
			m_pEffect->SetTexture(texture.id, texture.texture);
	}
	return err_ok;
}

err_t C3DMaterial::RenderEnd(I3DContext * pContext, int_x iSubSet)
{
	return err_ok;
}

VENUS_END
