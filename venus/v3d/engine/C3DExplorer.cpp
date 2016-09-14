#include "stdafx.h"
#include "C3DExplorer.h"
#include "C3DMesh.h"
#include "C3DAction.h"
#include "C3DMaterial.h"

VENUS_BEG

inline const char_16 * RES_FOLDER(ResourceTypeE eType)
{
	switch(eType)
	{
	case ResourceTypeDefault:
		return L"";
	case ResourceTypeTexture:
		return L"textures";
	case ResourceTypeMesh:
		return L"meshs";
	case ResourceTypeTerrain:
		return L"terrains";
	case ResourceTypeObject:
		return L"objects";
	case ResourceTypeAction:
		return L"actions";
	case ResourceTypeMaterial:
		return L"materials";
	case ResourceTypeEffect:
		return L"effects";
	default:
		return L"";
	}
}

C3DExplorer::C3DExplorer(I3DVedio * pVedio) :
	m_pVedio(pVedio)
{
	SafeAddRef(m_pVedio);
}

C3DExplorer::~C3DExplorer()
{
	ReleaseResouces();
	SafeRelease(m_pVedio);
}

void C3DExplorer::ReleaseResouces()
{
	for(I3DTexture *& pTexture : m_textures) SafeRelease(pTexture);
	for(I3DMaterial *& pMaterial : m_materials) SafeRelease(pMaterial);
	for(I3DEffect *& pEffect : m_effects) SafeRelease(pEffect);
	m_textures.clear();
	m_materials.clear();
	m_effects.clear();
}

I3DVedio * C3DExplorer::GetVedio()
{
	return m_pVedio;
}

I3DFile * C3DExplorer::LoadFile(const char_16 * szFile, ResourceTypeE eType)
{
	char_16 szPath[MAX_FILE_PATH] = {};
	textmakepath(szPath, MAX_FILE_PATH, RES_FOLDER(eType), -1, szFile, -1);

	return Get3DFileDriver()->GetFile(szPath);
}

I3DTexture * C3DExplorer::GetTexture(const char_16 * szFile, ResourceTypeE eType)
{
	if(!szFile || !m_pVedio)
		return nullptr;

	I3DFile * pFile = LoadFile(szFile, eType);
	if(!pFile)
		return nullptr;

	I3DTexture * pTexture = m_pVedio->CreateTexture();
	if(!pTexture)
		return nullptr;

	pTexture->Load(pFile);
	SafeRelease(pFile);

	m_textures.add(pTexture);
	return pTexture;
}

I3DMesh * C3DExplorer::GetMesh(const char_16 * szFile, ResourceTypeE eType)
{
	if(!szFile || !m_pVedio)
		return nullptr;

	I3DFile * pFile = LoadFile(szFile, eType);
	if(!pFile)
		return nullptr;

	C3DMesh * pMesh = new C3DMesh(this);
	pMesh->Load(pFile);
	SafeRelease(pFile);

	return pMesh;
}

I3DAction * C3DExplorer::GetAction(const char_16 * szFile, ResourceTypeE eType)
{
	if(!szFile || !m_pVedio)
		return nullptr;

	I3DFile * pFile = LoadFile(szFile, eType);
	if(!pFile)
		return nullptr;

	C3DAction * pAction = new C3DAction();
	pAction->Load(pFile);
	SafeRelease(pFile);

	return pAction;
}

I3DMaterial * C3DExplorer::GetMaterial(const char_16 * szFile, ResourceTypeE eType)
{
	if(!szFile || !m_pVedio)
		return nullptr;

	I3DFile * pFile = LoadFile(szFile, eType);
	if(!pFile)
		return nullptr;

	C3DMaterial * pMaterial = new C3DMaterial(this);
	pMaterial->Load(pFile);
	SafeRelease(pFile);
	m_materials.add(pMaterial);

	return pMaterial;
}

I3DEffect * C3DExplorer::GetEffect(const char_16 * szFile, ResourceTypeE eType)
{
	if(!szFile || !m_pVedio)
		return nullptr;

	char_16 szName[MAX_FILE_PATH] = {};
	textsplitpath_filename(szFile, -1, szName, MAX_FILE_PATH);

	for(int_x cnt = 0, len = m_effects.size(); cnt < len; ++cnt)
	{
		I3DEffect * pEffect = m_effects[cnt];
		if(textequalex(pEffect->GetName(), -1, szName, -1, false))
		{
			return pEffect;
		}
	}

	if(!textendwith(szFile, -1, L".eff", 4, false))
	{
		textcpy(szName, MAX_FILE_PATH, szFile, -1);
		textcat(szName, MAX_FILE_PATH, L".eff", 4);
		szFile = szName;
	}

	I3DFile * pFile = LoadFile(szFile, eType);
	if(!pFile)
		return nullptr;

	C3DEffect * pEffect = new C3DEffect(this);
	pEffect->Load(pFile);
	SafeRelease(pFile);

	m_effects.add(pEffect);
	return pEffect;
}

VENUS_END
