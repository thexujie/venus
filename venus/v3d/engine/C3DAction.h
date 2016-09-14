#pragma once

#include "vme.h"

VENUS_BEG

class C3DAction : public ObjectT<I3DAction>
{
public:
	C3DAction();
	~C3DAction();

	err_t Load(I3DFile * pFile);
	err_t GetPosture(vector<float4x4> & posture, int_x iTime) const;

private:
	va_header_t m_header;
	vector<va_bone_t> m_bones;
	vector<va_frame_t> m_frames;
	vector<float4x4> m_matrInvert;
};

VENUS_END
