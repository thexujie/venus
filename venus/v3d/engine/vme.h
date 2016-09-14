#pragma once

#include "mesh.h"

VENUS_BEG

const int_32 vmp_magic = make_fourcc('m', 'e', 's', 'h');
const int_32 vmp_version = make_fourcc(1, 0, 0, 0);
const int_32 vme_action_magic = make_fourcc('a', 'c', 't', ' ');
const int_32 vme_action_version = make_fourcc(1, 0, 0, 0);

#pragma pack(push, 4)

struct V3D_API vm_header_t
{
	int_32 magic;
	int_32 version;

	vertexformat_e vertexformat;
	indexformat_e indexformat;
	primitiveformat_e primitiveformat;

	int_32 num_vertices;
	int_32 num_indices;
	int_32 num_nodes;
};

struct V3D_API vm_subset_t
{
	int_32 vertex_offset;
	int_32 vertex_count;

	int_32 index_offset;
	int_32 index_count;
};

struct V3D_API va_header_t
{
	int_32 Magic;
	int_32 Version;
	int_32 BoneCount;
	int_32 Fps;
	int_32 FrameCount;
};

struct V3D_API va_bone_t
{
	float3 InitPos;
	quat4 InitRot;
	float3 InitScl;
};

struct V3D_API va_frame_t
{
	float3 Pos;
	quat4 Rot;
	float3 Scl;
};

#pragma pack(pop)

VENUS_END
