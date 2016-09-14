#pragma once

VENUS_BEG

class C3DEffect : public ObjectT<I3DEffect>
{
public:
	C3DEffect(I3DExplorer * pExplorer);
	~C3DEffect();

	err_t Load(const I3DFile * pFile);
	const char_16 * GetName() const;

	int_x GetPassCount() const;
	I3DPass * GetPass(renderpass_e pass) const;
	I3DPass * GetPass(const char_16 * szName) const;

	err_t PassBeg(I3DContext * pContext);
	err_t Flush();
	err_t PassEnd();

	eff_paramid_t FindParam(const char_16 * name) const;

	err_t SetTexture(eff_paramid_t id, I3DTexture * pTexture);
	err_t SetTexture(const char_16 * name, I3DTexture * pTexture);

	err_t SetBuffer(eff_paramid_t id, I3DBuffer * pBuffer);
	err_t SetBuffer(const char_16 * name, I3DBuffer * pBuffer);

	err_t SetVariable(eff_paramid_t id, const void * data, int_x size);
	err_t SetVariable(const char_16 * name, const void * data, int_x size);
protected:

	void _ParseShaders(const xml_node & node_shader, passstate_t & state);
	void _ParseStates(linear_map<textw, textw> & states, passstate_t & state);
	void _ParseSampler(linear_map<textw, textw> & states, sampler_state_t & state);

public:
	struct effect_cbuffer_t
	{
		chbufferw<EFFECT_MAX_IDEITIFIER> name;
		int_x slot;
		int_x size;
		byte_t * buffer;
		bool invalid;
		bool binded;
		I3DBuffer * pointer;
	};

	struct effect_buffer_t
	{
		chbufferw<EFFECT_MAX_IDEITIFIER> name;
		int_x slot;
		bool binded;
		I3DBuffer * pointer;
	};

	struct effect_texture_t
	{
		chbufferw<EFFECT_MAX_IDEITIFIER> name;
		int_x slot;
		bool binded;
		I3DTexture * pointer;
	};

	struct effect_sampler_t
	{
		chbufferw<EFFECT_MAX_IDEITIFIER> name;
		sampler_state_t state;
	};

protected:
	I3DExplorer * m_pExplorer;
	textw m_name;

	vector<I3DPass *> m_passes;
	vector<shader_param_t> m_params;

	I3DContext * m_pContext;
	array<vector<effect_cbuffer_t>, shader_num> m_cbuffers;
	array<vector<effect_buffer_t>, shader_num> m_buffers;
	array<vector<effect_texture_t>, shader_num> m_textures;
};

VENUS_END
