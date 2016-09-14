#pragma once

#include "mesh.h"
#include "vme.h"

VENUS_BEG

class C3DMesh : public ObjectT<I3DMesh>
{
public:
	C3DMesh(I3DExplorer * pExplorer);
	~C3DMesh();

	err_t Load(I3DFile * pFile);
	err_t Create();
	bbox_t GetBoundingBox() const;

	I3DBuffer * GetIBuffer() const;
	I3DBuffer * GetVBuffer() const;
	vertexformat_e GetVertexFormat() const;
	indexformat_e GetIndexFormat() const;
	primitiveformat_e GetPrimitiveFormat() const;

	int_x GetNodeCount() const;
	const vector<mesh_node_t> & GetNodes() const;

	int_x RayIntersect(const ray_t & ray, vector<IntersectPointT> & result, float_32 tolerance);
private:
	I3DExplorer * m_pExplorer;
	I3DBuffer * m_pvBuffer;
	I3DBuffer * m_piBuffer;
	// mesh
	vm_header_t m_header;

	byte_buffer_t m_bbuffVertices;
	byte_buffer_t m_bbuffIndices;
	vector<mesh_node_t> m_nodes;

	bbox_t m_bbox;
};

VENUS_END
