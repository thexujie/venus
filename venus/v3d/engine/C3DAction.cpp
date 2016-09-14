#include "stdafx.h"
#include "C3DAction.h"

VENUS_BEG

C3DAction::C3DAction()
{

}

C3DAction::~C3DAction()
{

}

err_t C3DAction::Load(I3DFile * pFile)
{
	if(!pFile)
		return err_invalidarg;

	pFile->Read(&m_header, sizeof(va_header_t));
	m_bones.resize(m_header.BoneCount);
	m_frames.resize(m_header.BoneCount * m_header.FrameCount);
	pFile->Read(m_bones, m_header.BoneCount * sizeof(va_bone_t));
	pFile->Read(m_frames, m_header.BoneCount * m_header.FrameCount * sizeof(va_frame_t));

	m_matrInvert.resize(m_header.BoneCount);
	for(int_32 cnt = 0; cnt < m_header.BoneCount; ++cnt)
	{
		const va_bone_t & bone = m_bones[cnt];
		m_matrInvert[cnt].transform(bone.InitPos, bone.InitRot, bone.InitScl);
		m_matrInvert[cnt].invert();
	}
	return err_ok;
}

err_t C3DAction::GetPosture(vector<float4x4> & posture, int_x iTime) const
{
	int_x iFrame = m_header.Fps * iTime;
	int_x iIndex = (iFrame / 1000) % (m_header.FrameCount - 1);
	float_32 fRate = (iFrame % 1000) / 1000.0f;
	float3 pos;
	quat4 rot;
	float3 scl;
	posture.resize(m_header.BoneCount);
	for(int_32 cnt = 0; cnt < m_header.BoneCount; ++cnt)
	{
		const va_frame_t & beg = m_frames[cnt * m_header.FrameCount + iIndex];
		const va_frame_t & end = m_frames[cnt * m_header.FrameCount + iIndex + 1];
		pos.lerp(beg.Pos, end.Pos, fRate);
		rot.lerp(beg.Rot, end.Rot, fRate);
		scl.lerp(beg.Scl, end.Scl, fRate);
		posture[cnt] = m_matrInvert[cnt] * float4x4_transform(pos, rot, scl);
	}
	return err_ok;
}

VENUS_END
