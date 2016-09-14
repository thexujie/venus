#include "stdafx.h"
#include "C3DMesh.h"

VENUS_BEG

C3DMesh::C3DMesh(I3DExplorer * pExplorer) :
	m_pExplorer(pExplorer), m_pvBuffer(nullptr), m_piBuffer(nullptr)
{
	FakeAddRef(m_pExplorer);
}

C3DMesh::~C3DMesh()
{
	SafeRelease(m_piBuffer);
	SafeRelease(m_pvBuffer);
	FakeRelease(m_pExplorer);
}

err_t C3DMesh::Load(I3DFile * pFile)
{
	if(!pFile)
		return err_invalidarg;

	pFile->Read(&m_header, sizeof(m_header));
	if(!m_header.num_nodes)
		return err_ok;

	int_x ivStride = GetVertexStrideFromFormat(m_header.vertexformat);
	int_x iiStride = GetIndexStrideFromFormat(m_header.indexformat);
	int_x ivSize = ivStride * m_header.num_vertices;
	int_x iiSize = iiStride * m_header.num_indices;

	m_bbuffVertices.reallocate(ivSize, ivSize);
	m_bbuffIndices.reallocate(iiSize, iiSize);
	pFile->Read(m_bbuffVertices, ivSize);
	pFile->Read(m_bbuffIndices, iiSize);

	vm_subset_t vmeshnode = {};
	for(int_32 cnt = 0; cnt < m_header.num_nodes; ++cnt)
	{
		pFile->Read(&vmeshnode, sizeof(vm_subset_t));
		mesh_node_t meshnode = {vmeshnode.vertex_offset, vmeshnode.vertex_count,
			vmeshnode.index_offset, vmeshnode.index_count};
		m_nodes.add(meshnode);

	}

	bool bReset = false;
	int_x stride = GetVertexStrideFromFormat(m_header.vertexformat);
	const byte_t * vertex = m_bbuffVertices.buffer;
	for(int_x iVert = 0; iVert < m_header.num_vertices; ++iVert)
	{
		const float3 & point = *(const float3 *)vertex;
		if(!bReset)
		{
			m_bbox.reset(point);
			bReset = true;
		}
		else
			m_bbox.add_point(point);

		vertex += stride;
	}
	return Create();
}

err_t C3DMesh::Create()
{
	int_x ivStride = GetVertexStrideFromFormat(m_header.vertexformat);
	int_x iiStride = GetIndexStrideFromFormat(m_header.indexformat);

	m_pvBuffer = m_pExplorer->GetVedio()->CreateBuffer();
	m_piBuffer = m_pExplorer->GetVedio()->CreateBuffer();

	m_pvBuffer->Create(ivStride, m_header.num_vertices,
		bufferusage_default, bufferbind_vertex, bufferaccess_none, cmode_invalid,
		m_bbuffVertices, m_bbuffVertices.size, 0);
	m_piBuffer->Create(iiStride, m_header.num_indices,
		bufferusage_default, bufferbind_index, bufferaccess_none, cmode_invalid,
		m_bbuffIndices, m_bbuffIndices.size, 0);

	return err_ok;
}

bbox_t C3DMesh::GetBoundingBox() const
{
	return m_bbox;
}

I3DBuffer * C3DMesh::GetIBuffer() const
{
	return m_piBuffer;
}

I3DBuffer * C3DMesh::GetVBuffer() const
{
	return m_pvBuffer;
}

vertexformat_e C3DMesh::GetVertexFormat() const
{
	return m_header.vertexformat;
}

indexformat_e C3DMesh::GetIndexFormat() const
{
	return m_header.indexformat;
}

primitiveformat_e C3DMesh::GetPrimitiveFormat() const
{
	return m_header.primitiveformat;
}

int_x C3DMesh::GetNodeCount() const
{
	return m_header.num_nodes;
}

const vector<mesh_node_t> & C3DMesh::GetNodes() const
{
	return m_nodes;
}

int_x C3DMesh::RayIntersect(const ray_t & ray, vector<IntersectPointT> & result, float_32 tolerance)
{
	int_x stride = GetVertexStrideFromFormat(m_header.vertexformat);
	const byte_t * vertex = m_bbuffVertices.buffer;
	result.resize(0);
	const int_16 * index = (const int_16 *)m_bbuffIndices.buffer;
	int_x iFaceBase = 0;
	float_32 u, v, d;
	for(int_x iNode = 0; iNode < m_header.num_nodes; ++iNode)
	{
		const mesh_node_t & node = m_nodes[iNode];
		int_x iFaceNum = node.index_count / 3;
		for(int_x iFace = 0; iFace < iFaceNum; ++iFace)
		{
			int_x index0 = node.vertex_offset + *index++;
			int_x index1 = node.vertex_offset + *index++;
			int_x index2 = node.vertex_offset + *index++;

			const float3 & pos0 = *(const float3 *)(vertex + stride * index0);
			const float3 & pos1 = *(const float3 *)(vertex + stride * index1);
			const float3 & pos2 = *(const float3 *)(vertex + stride * index2);

			bool intersect = intersect_tri(ray.pos, ray.dir, pos0, pos1, pos2, u, v, d, tolerance);
			if(intersect)
			{
				float3 pos = pos0 + (pos1 - pos0) * u + (pos2 - pos0) * v;
				IntersectPointT iPoint;
				iPoint.Node = iNode;
				iPoint.Face = iFaceBase + iFace;
				iPoint.Index0 = index0;
				iPoint.Index1 = index1;
				iPoint.Index2 = index2;
				iPoint.Pos = pos;

				iPoint.NodeFace = iFace;
				iPoint.NodeIndex0 = index0;
				iPoint.NodeIndex1 = index1;
				iPoint.NodeIndex2 = index2;

				iPoint.PosU = u;
				iPoint.PosV = v;
				iPoint.PosD = d;
				result.add(iPoint);
			}
		}
		iFaceBase += node.index_count / 3;
	}
	return result.size();
}

VENUS_END
