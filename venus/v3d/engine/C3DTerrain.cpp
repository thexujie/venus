#include "stdafx.h"
#include "C3DTerrain.h"

VENUS_BEG

C3DTerrain::C3DTerrain(I3DExplorer * pExplorer) :
	C3DNode(pExplorer), m_pvb(nullptr), m_pib(nullptr), m_pEffect(nullptr), m_ptexNH(nullptr),
	m_pMaterial(nullptr), m_pbufDensity(nullptr), 
	m_iVtxCount(0), m_iIdxCount(0)
{
	m_transform.identity();
}

C3DTerrain::~C3DTerrain()
{
	SafeRelease(m_pEffect);
	SafeRelease(m_pMaterial);
	SafeRelease(m_ptexNH);
	SafeRelease(m_pbufDensity);
	SafeRelease(m_pvb);
	SafeRelease(m_pib);
}

err_t C3DTerrain::Load(const char_16 * szTerrain)
{
	if(!m_pExplorer)
		return err_not_initialized;

	xml_doc xml;
	object_ptr<I3DFile> pFile(m_pExplorer->LoadFile(szTerrain, ResourceTypeTerrain));
	if(!pFile)
		return err_no_file;

	xml.load_text((const char_16 *)pFile->GetBuffer(), pFile->GetSize() / 2);

	m_pMaterial = m_pExplorer->GetMaterial(xml(L"material")[L"file"].value);
	SafeAddRef(m_pMaterial);
	m_pEffect = m_pMaterial->GetEffect();
	SafeAddRef(m_pEffect);

	m_ptexNH = m_pMaterial->GetTexture(L"tex_nheight");
	SafeAddRef(m_ptexNH);

	TextureDescT desc = m_ptexNH->GetDesc();
	if(desc.CMode != cmode_a8r8g8b8)
		throw exp_err(err_bad_format);
	m_size = desc.size;

	float_32 step_x = xml[L"step_x"].value.tof32(1.0f);
	float_32 step_y = xml[L"step_y"].value.tof32(1.0f);
	float_32 step_z = xml[L"step_z"].value.tof32(1.0f);
	m_step = {step_x, step_y, step_z};

	int_x iColNum = TERRAIN_TESS_SIZE;
	int_x iRowNum = TERRAIN_TESS_SIZE;

	step_x = step_x * (m_size.w -1)/ (iColNum - 1);
	step_z = step_z * (m_size.h -1)/ (iRowNum - 1);

	int_x iVtxCount = TERRAIN_TESS_SIZE * TERRAIN_TESS_SIZE;
	vector<vertex_ter_t> vertices(iVtxCount, iVtxCount);
	float_32 fx_beg = -0.5f * (iColNum - 1) * step_x;
	float_32 fz_beg = 0.5f * (iRowNum  - 1)* step_z;
	float_32 fx = fx_beg, fz = fz_beg;
	for(int_x row = 0; row < iRowNum; ++row)
	{
		for(int_x col = 0; col < iColNum; ++col)
		{
			vertex_ter_t & vertex = vertices[row * iColNum + col];
			vertex.pos = float3(fx, 0.0f, fz);
			vertex.tex = float2((float_32)col / (iColNum - 1), (float_32)row / (iRowNum - 1));
			vertex.nom.set(0.0f, 1.0f, 0.0f);
			vertex.bin.set(0.0f, 0.0f, -1.0f);
			vertex.tan.set(1.0f, 0.0f, 0.0f);

			fx += step_x;
		}
		fz -= step_z;
		fx = fx_beg;
	}
	m_pvb = GetVedio()->CreateBuffer();
	m_pvb->Create(sizeof(vertex_ter_t), iVtxCount, 
		bufferusage_default, bufferbind_vertex, bufferaccess_none, cmode_invalid, 
		vertices.buffer(), iVtxCount * sizeof(vertex_ter_t));

	m_iTriCount = (iColNum - 1) * (iRowNum - 1) * 2;
	int_x iIdxCount = (iColNum - 1) * (iRowNum - 1) * 2 * 3;
	vector<int_16> indices(iIdxCount, iIdxCount);
	for(int_x index = 0, row = 0; row < iRowNum - 1; ++row)
	{
		for(int_x col = 0; col < iColNum - 1; ++col)
		{
			int_x index0 = row * iColNum + col;
			int_x index1 = index0 + 1;
			int_x index2 = index1 + iColNum;
			int_x index3 = index0 + iColNum;
			indices[index++] = (int_16)index0;
			indices[index++] = (int_16)index1;
			indices[index++] = (int_16)index2;
			indices[index++] = (int_16)index2;
			indices[index++] = (int_16)index3;
			indices[index++] = (int_16)index0;
		}
	}

	m_pib = GetVedio()->CreateBuffer();
	m_pib->Create(sizeof(int_16), iIdxCount, 
		bufferusage_default, bufferbind_index, bufferaccess_none, cmode_invalid, 
		indices, iIdxCount * sizeof(int_16));

	I3DVedio * pVedio = m_ptexNH->GetVedio();
	I3DTexture * pStaging = pVedio->CreateTexture();
	pStaging->Create(desc.size, desc.CMode, 1, bufferusage_staging, bufferbind_none, bufferaccess_read);
	m_ptexNH->Copy(pStaging);

	MappedBufferT buffer = {};
	pStaging->Map(buffer, buffermap_read);

	const color_argb_t * pixels = (const color_argb_t *)buffer.Data;
	m_nhs.reallocate(desc.size.w * desc.size.h, desc.size.w * desc.size.h);
	arraycpy(m_nhs.buffer(), m_nhs.size(), pixels, m_nhs.size());

	m_pbufDensity = GenerateDensityFromNH(pVedio, pixels, m_size, vertices, indices, step_x);

	pStaging->Unmap();
	SafeRelease(pStaging);

	m_iVtxCount = iVtxCount;
	m_iIdxCount = iIdxCount;
	return err_ok;
}

I3DBuffer * C3DTerrain::GetVBuffer() const
{
	return m_pib;
}

I3DBuffer * C3DTerrain::GetIBuffer() const
{
	return m_pvb;
}

void C3DTerrain::Update(int_x iPeriod)
{
	I3DCamera * pCamera = m_pScene->GetCamera();
	if(!pCamera)
		return;
}

err_t C3DTerrain::Render(I3DContext * pContext) const
{
	I3DCamera * pCamera = pContext->GetCamera();
	if(!pCamera)
		return err_invalidcall;

	BaseTransformT _global;
	_global.matrW = m_transform;
	_global.matrV = pCamera->GetMatrixV();
	_global.matrP = pCamera->GetMatrixP();
	_global.matrVP = _global.matrV * _global.matrP;
	_global.matrWV = _global.matrW * _global.matrV;
	_global.matrWVP = _global.matrWV * _global.matrP;

	float4 view_frustum[4];
	view_frustum_t vf(_global.matrVP);
	buffcpy<float4>(view_frustum, 4, vf.planes, 4);

	float3 eye_pos = pCamera->GetPosition();
	float1 visual_min = 0;
	float1 visual_max = pCamera->GetVisual();
	float1 tess_factor = 20;
	float1 tess_height_scale = m_step.y;
	float1 detail_max = 50;
	SafeRelease(pCamera);

	m_pEffect->SetVariable(L"gtm", &(_global.matrW), sizeof(_global.matrW));
	m_pEffect->SetVariable(L"gtv", &(_global.matrV), sizeof(_global.matrV));
	m_pEffect->SetVariable(L"gtp", &(_global.matrP), sizeof(_global.matrP));
	m_pEffect->SetVariable(L"gtvp", &(_global.matrVP), sizeof(_global.matrVP));
	m_pEffect->SetVariable(L"gtmv", &(_global.matrWV), sizeof(_global.matrWV));
	m_pEffect->SetVariable(L"gtmvp", &(_global.matrWVP), sizeof(_global.matrWVP));

	m_pEffect->SetVariable(L"view_frustum", view_frustum, sizeof(float4) * 4);
	m_pEffect->SetVariable(L"eye_pos", &eye_pos, sizeof(eye_pos));
	m_pEffect->SetVariable(L"visual_min", &visual_min, sizeof(visual_min));
	m_pEffect->SetVariable(L"visual_max", &visual_max, sizeof(visual_max));
	m_pEffect->SetVariable(L"tess_factor", &tess_factor, sizeof(tess_factor));
	m_pEffect->SetVariable(L"tess_height_scale", &tess_height_scale, sizeof(tess_height_scale));
	m_pEffect->SetVariable(L"detail_max", &detail_max, sizeof(detail_max));

	m_pEffect->SetBuffer(L"buf_density", m_pbufDensity);

	m_pMaterial->RenderBeg(pContext, 0);

	pContext->SetVertexBuffer(m_pvb, vertexformat_pos_tex_nor_bin_tan);
	pContext->SetIndexBuffer(m_pib, indexformat_16);

	for(int_x cnt = 0; cnt < m_pEffect->GetPassCount(); ++cnt)
	{
		m_pEffect->Flush();
		pContext->DrawIndexed(0, 0, m_iIdxCount, primitiveformat_tri_list);
	}

	return err_ok;
}

err_t C3DTerrain::PassBeg(I3DRenderEngine * pRenderEngine, renderpass_e pass)
{
	return err_ok;
}
err_t C3DTerrain::PassEnd(I3DRenderEngine * pRenderEngine, renderpass_e pass)
{
	return err_ok;
}
err_t C3DTerrain::Render(I3DContext * pContext, I3DPass * pPass) const
{
	return err_ok;
}

float C3DTerrain::GetHeight(vec2ix point) const
{
	int_x iIndex = point.y * m_size.w + point.x;
	return m_nhs[iIndex].a / 255.0f * m_step.y;
}

float3 C3DTerrain::GetCoords(vec2ix point) const
{
	float_32 fx_beg = -0.5f * (m_size.w - 1) * m_step.x;
	float_32 fz_beg = 0.5f * (m_size.h - 1) * m_step.z;
	return float3(fx_beg + point.x * m_step.x, GetHeight(point), fz_beg - point.y * m_step.z);
}

static float GetMaximumDensityAlongEdge(uint_8 * pDensities, int_x dwWidth, int_x dwHeight,
	float2 fTexcoord0, float2 fTexcoord1)
{
	// Convert texture coordinates to texel coordinates using round-to-nearest
	int_x nU0 = round(fTexcoord0.x * (dwWidth - 1));
	int_x nV0 = round(fTexcoord0.y * (dwHeight - 1));
	int_x nU1 = round(fTexcoord1.x * (dwWidth - 1));
	int_x nV1 = round(fTexcoord1.y * (dwHeight - 1));

	// Wrap texture coordinates
	nU0 = nU0 % dwWidth;
	nV0 = nV0 % dwHeight;
	nU1 = nU1 % dwWidth;
	nV1 = nV1 % dwHeight;

	// Find how many texels on edge
	int_x nNumTexelsOnEdge = maxof(abs(nU1 - nU0), abs(nV1 - nV0)) + 1;

	float fU, fV;
	float fMaxDensity = 0.0f;
	for(int_x i = 0; i < nNumTexelsOnEdge; ++i)
	{
		// Calculate current texel coordinates
		fU = lerp((float)nU0, (float)nU1, ((float)i) / (nNumTexelsOnEdge - 1));
		fV = lerp((float)nV0, (float)nV1, ((float)i) / (nNumTexelsOnEdge - 1));

		// Round texel texture coordinates to nearest
		int_x nCurrentU = round(fU);
		int_x nCurrentV = round(fV);

		// Update max density along edge
		uint_8 density = *(pDensities + nCurrentV * dwWidth + nCurrentU);
		fMaxDensity = maxof(fMaxDensity, (float)density / 255.0f);
	}

	return fMaxDensity;
}

I3DBuffer * C3DTerrain::GenerateDensityFromNH(I3DVedio * pVedio,
	const color_argb_t * nh, sizeix size,
	const vector<vertex_ter_t> & vertices,
	const vector<int_16> & indices,
	float_32 fDensityScale)
{
	if(!nh)
		return nullptr;

	int_x iTriCount = indices.size() / 3;
	float_32 fDiff = 1.0f / 255.0f;
	int_x iSize = size.w * size.h;
	uint_8 * pDensity = new uint_8[iSize];

	const color_argb_t * pixels = nh;

	for(int_x row = 0, index = 0; row < size.h; ++row)
	{
		for(int_x col = 0; col < size.w; ++col, ++index)
		{
			uint_8 uiDensity = 1;

			// 边框的密度为 1
			if(row && col &&
				(row != size.h - 1) &&
				(col != size.w - 1))
			{
				int_x index = row * size.w + col;
				float_32 heights[3][3];
				heights[0][0] = pixels[index - size.w - 1].a / 255.0f;
				heights[0][1] = pixels[index - size.w].a / 255.0f;
				heights[0][2] = pixels[index - size.w + 1].a / 255.0f;

				heights[1][0] = pixels[index - 1].a / 255.0f;
				heights[1][1] = pixels[index].a / 255.0f;
				heights[1][2] = pixels[index + 1].a / 255.0f;

				heights[2][0] = pixels[index + size.w - 1].a / 255.0f;
				heights[2][1] = pixels[index + size.w].a / 255.0f;
				heights[2][2] = pixels[index + size.w + 1].a / 255.0f;

				float_32 density = 0.0f;
				float_32 densities[4];
				densities[0] = abs((heights[1][2] - heights[1][1]) - (heights[1][1] - heights[1][0]));
				densities[1] = abs((heights[2][1] - heights[1][1]) - (heights[1][1] - heights[0][1]));
				densities[2] = abs((heights[2][0] - heights[1][1]) - (heights[1][1] - heights[0][2]));
				densities[3] = abs((heights[2][2] - heights[1][1]) - (heights[1][1] - heights[0][0]));

				// 线性分配
				float_32 rates[4] = {0.293f, 0.293f, 0.207f, 0.207f};
				for(int_x cnt = 0; cnt < 4; ++cnt)
				{
					if(densities[cnt] >= fDiff)
						density += rates[cnt] * densities[cnt];
				}

				density *= fDensityScale;
				density = maxof(minof(density, 1.0f), 1.0f / 255.0f);
				uiDensity = (uint_8)(density * 255.0f);
			}
			pDensity[index] = uiDensity;
		}
	}

	float GetMaximumDensityAlongEdge(uint_8 * pDensities, int_x dwWidth, int_x dwHeight,
		float2 fTexcoord0, float2 fTexcoord1);
	vector<float3> edge_densities(iTriCount, iTriCount);
	// 计算每个 patch 的密度
	for(int tri = 0; tri < iTriCount; ++tri)
	{
		const vertex_ter_t & vertex0 = vertices[indices[tri * 3 + 0]];
		const vertex_ter_t & vertex1 = vertices[indices[tri * 3 + 1]];
		const vertex_ter_t & vertex2 = vertices[indices[tri * 3 + 2]];

		float_32 fEdgeDensity0 = GetMaximumDensityAlongEdge(pDensity, size.w, size.h, vertex0.tex, vertex1.tex);
		float_32 fEdgeDensity1 = GetMaximumDensityAlongEdge(pDensity, size.w, size.h, vertex1.tex, vertex2.tex);
		float_32 fEdgeDensity2 = GetMaximumDensityAlongEdge(pDensity, size.w, size.h, vertex2.tex, vertex0.tex);

		// Store edge density in x,y,z and store maximum density in .w
		edge_densities[tri] = float3(fEdgeDensity1, fEdgeDensity2, fEdgeDensity0);
	}
	delete pDensity;

	I3DBuffer * pbufDensity = pVedio->CreateBuffer();
	pbufDensity->Create(sizeof(float3), iTriCount,
		bufferusage_default, bufferbind_resource, bufferaccess_none, cmode_b32g32r32f,
		(const void *)edge_densities, iTriCount * sizeof(float3), 0);
	return pbufDensity;
}

VENUS_END
