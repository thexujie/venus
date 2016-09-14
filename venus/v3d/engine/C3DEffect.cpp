#include "stdafx.h"
#include "C3DEffect.h"

VENUS_BEG

C3DEffect::C3DEffect(I3DExplorer * pExplorer) :
	m_pExplorer(pExplorer), m_pContext(nullptr)
{
	FakeAddRef(m_pExplorer);
}

C3DEffect::~C3DEffect()
{
	for(I3DPass *& pPass : m_passes) SafeRelease(pPass);
	m_passes.clear();

	for(int_x ishader = 0; ishader < shader_num; ++ishader)
	{
		for(effect_cbuffer_t & ecb : m_cbuffers[ishader]) { SafeDelete(ecb.buffer); SafeRelease(ecb.pointer); }
		for(effect_buffer_t & eb : m_buffers[ishader]) SafeRelease(eb.pointer);
		for(effect_texture_t & etex : m_textures[ishader]) SafeRelease(etex.pointer);

		m_cbuffers[ishader].clear();
		m_buffers[ishader].clear();
		m_textures[ishader].clear();
	}
	m_params.clear();
	FakeRelease(m_pExplorer);
}

err_t C3DEffect::Load(const I3DFile * pFile)
{
	if(!pFile)
		return err_invalidarg;

	xml_doc doc;
	err_t err = doc.load_text((const char_16 *)pFile->GetBuffer(), pFile->GetSize() / 2);
	if(err)
		return err_bad_format;

	vector<xml_node> includes;
	for(int_x cnt = 0; cnt < doc.children.size(); ++cnt)
	{
		const xml_node & node = doc.children[cnt];
		if(node.name.equal(L"include", 7, false))
		{
			textw file = node[L"name"].value;
			file.insert(0, L"effects/", 8);
			if(file.is_valid())
			{
				I3DFile * pIncludeFile = Get3DFileDriver()->GetFile(file.buffer());
				if(!pIncludeFile)
					continue;

				xml_doc doc_include;
				err = doc_include.load_text((const char_16 *)pIncludeFile->GetBuffer(), pIncludeFile->GetSize() / 2);
				SafeRelease(pIncludeFile);

				includes.join(move(doc_include.children));
			}
		}
	}
	if(includes.size() > 0)
	{
		includes.join(move(doc.children));
		doc.children = move(includes);
	}

	m_name = doc[L"name"];

	linear_map<textw, textw> state_nodes;
	vector<const xml_node *> pass_nodes;
	vector<const xml_node *> sampler_nodes;
	passstate_t state_base = {};

	for(int_x cnt = 0; cnt < doc.children.size(); ++cnt)
	{
		const xml_node & node = doc.children[cnt];
		if(node.name.equal(L"state", 5, false))
		{
			state_nodes[node[L"name"].value] = node[L"value"].value;
		}
		else if(node.name.equal(L"pass", 4, false))
		{
			pass_nodes.add(&node);
		}
		else if(node.name.equal(L"sampler", 7, false))
		{
			sampler_nodes.add(&node);
		}
		else if(node.name.equal(L"shader", 6, false))
		{
			_ParseShaders(node, state_base);
		}
		else {}
	}

	_ParseStates(state_nodes, state_base);
	for(int_x ipass = 0; ipass < pass_nodes.size(); ++ipass)
	{
		const xml_node & pass_node = *pass_nodes[ipass];
		state_nodes.clear();
		for(int_x cnt = 0; cnt < pass_node.children.size(); ++cnt)
		{
			const xml_node & node = pass_node.children[cnt];
			if(node.name.equal(L"state", 5, false))
			{
				state_nodes[node[L"name"].value] = node[L"value"].value;
			}
			else {}
		}

		passstate_t state_pass = state_base;
		_ParseStates(state_nodes, state_pass);
		_ParseShaders(pass_node(L"shader"), state_pass);
		state_pass.name = pass_node[L"name"].value.buffer();

		I3DPass * pPass = m_pExplorer->GetVedio()->CreatePass();
		pPass->SetState(this, state_pass);
		m_passes.add(pPass);
	}

	if(m_passes.is_empty())
	{
		state_base.name = L"[system pass]";
		I3DPass * pPass = m_pExplorer->GetVedio()->CreatePass();
		pPass->SetState(this, state_base);
		m_passes.add(pPass);
	}
	else {}

	vector<shader_param_t> params;
	vector<shader_cbuffer_t> cbuffers;
	m_passes[0]->GetShaderDesc(params, cbuffers);

	m_params = params;

	for(const shader_param_t & spa : m_params)
	{
		if(spa.type == eff_param_type_buffer)
		{
			for(int_x ishader = 0; ishader < shader_num; ++ishader)
			{
				if(spa.slots[ishader] < 0)
					continue;

				effect_buffer_t & eb = m_buffers[ishader].add();
				eb.name = spa.name;
				eb.slot = spa.slots[ishader];
			}
		}
		else if(spa.type == eff_param_type_texture_2d)
		{
			for(int_x ishader = 0; ishader < shader_num; ++ishader)
			{
				if(spa.slots[ishader] < 0)
					continue;

				effect_texture_t & etex = m_textures[ishader].add();
				etex.name = spa.name;
				etex.slot = spa.slots[ishader];
			}
		}
		else {}
	}

	for(const shader_cbuffer_t & scb : cbuffers)
	{
		vector<effect_cbuffer_t> & cbuffers = m_cbuffers[scb.shader];
		effect_cbuffer_t & ecb = cbuffers.add();
		ecb.name = scb.name;
		ecb.slot = scb.slot;
		ecb.size = scb.size;
	}

	sampler_state_t sampler_state;
	for(int_x isampler = 0; isampler < sampler_nodes.size(); ++isampler)
	{
		const xml_node & sampler_node = *sampler_nodes[isampler];
		textw name = sampler_node[L"name"].value;
		eff_paramid_t id = FindParam(name);
		if(id.id < 0)
			continue;

		shader_param_t & param = m_params[id.id];
		if(param.type != eff_param_type_sampler)
			continue;

		state_nodes.clear();
		for(int_x cnt = 0; cnt < sampler_node.children.size(); ++cnt)
		{
			const xml_node & node = sampler_node.children[cnt];
			if(node.name.equal(L"state", 5, false))
			{
				state_nodes[node[L"name"].value] = node[L"value"].value;
			}
			else {}
		}

		_ParseSampler(state_nodes, sampler_state);
		for(I3DPass * pPass : m_passes)
		{
			for(int_x ishader = 0; ishader < shader_num; ++ishader)
			{
				int_x slot = param.slots[ishader];
				if(slot >= 0)
					pPass->SetSampler((shader_e)ishader, slot, sampler_state);
			}
		}
	}

	return err_ok;
}

const char_16 * C3DEffect::GetName() const
{
	return m_name;
}

int_x C3DEffect::GetPassCount() const
{
	return m_passes.size();
}

I3DPass * C3DEffect::GetPass(renderpass_e pass) const
{
	// TODO:pass ·ÖÀà
	return m_passes[pass];
}

I3DPass * C3DEffect::GetPass(const char_16 * szName) const
{
	for(int_x cnt = 0, len = m_passes.size(); cnt < len; ++cnt)
	{
		I3DPass * pPass = m_passes[cnt];
		if(textequalex(pPass->GetName(), -1, szName, -1, false))
		{
			return pPass;
		}
	}
	return nullptr;
}


err_t C3DEffect::PassBeg(I3DContext * pContext)
{
	SafeRelease(m_pContext);
	m_pContext = pContext;
	SafeAddRef(m_pContext);

	return err_ok;
}

err_t C3DEffect::Flush()
{
	if(!m_pContext)
		return err_no_context;

	for(int_x ishader = 0; ishader < shader_num; ++ishader)
	{
		// const buffer.
		for(effect_cbuffer_t & ecb : m_cbuffers[ishader])
		{
			if(!ecb.size || !ecb.buffer)
				continue;

			if(ecb.invalid || !ecb.pointer)
			{
				if(ecb.pointer)
				{
					ecb.pointer->SetBuffer(0, ecb.size, ecb.buffer);
					//pContext->UpdateBuffer(ecb.pointer, rectix(), ecb.buffer, ecb.size);
				}
				else
				{
					ecb.pointer = m_pExplorer->GetVedio()->CreateBuffer();
					ecb.pointer->Create(ecb.size, 1, bufferusage_dynamic, bufferbind_const, bufferaccess_write, cmode_invalid,
						ecb.buffer, ecb.size, 0);
				}

				ecb.invalid = false;
			}
			if(!ecb.binded)
			{
				m_pContext->SetCBuffer((shader_e)ishader, ecb.pointer, ecb.slot);
				ecb.binded = true;
			}
		}

		// buffer
		for(effect_buffer_t & eb : m_buffers[ishader])
		{
			if(!eb.pointer)
				continue;

			if(!eb.binded)
			{
				m_pContext->SetBuffer((shader_e)ishader, eb.pointer, eb.slot);
				eb.binded = true;
			}
		}

		// texture
		for(effect_texture_t & etex : m_textures[ishader])
		{
			if(!etex.pointer)
				continue;

			if(!etex.binded)
			{
				m_pContext->SetTexture((shader_e)ishader, etex.pointer, etex.slot);
				etex.binded = true;
			}
		}
	}
	return err_ok;
}

err_t C3DEffect::C3DEffect::PassEnd()
{

	// const buffer.
	for(int_x ishader = 0; ishader < shader_num; ++ishader)
	{
		for(effect_cbuffer_t & ecb : m_cbuffers[ishader])
		{
			if(ecb.pointer && ecb.binded)
			{
				m_pContext->SetCBuffer((shader_e)ishader, nullptr, ecb.slot);
				ecb.binded = false;
			}
			else {}
			//SafeRelease(ecb.pointer); // unnecessariy.
		}

		for(effect_buffer_t & eb : m_buffers[ishader])
		{
			if(eb.pointer && eb.binded)
			{
				m_pContext->SetBuffer((shader_e)ishader, nullptr, eb.slot);
				eb.binded = false;
			}
			else {}

			SafeRelease(eb.pointer);
		}

		for(effect_texture_t & etex : m_textures[ishader])
		{
			if(etex.pointer && etex.binded)
			{
				m_pContext->SetTexture((shader_e)ishader, nullptr, etex.slot);
				etex.binded = false;
			}
			else {}

			SafeRelease(etex.pointer);
		}
	}

	SafeRelease(m_pContext);
	return err_ok;
}

eff_paramid_t C3DEffect::FindParam(const char_16 * name) const
{
	for(int_x cnt = 0; cnt < m_params.size(); ++cnt)
	{
		const shader_param_t & param = m_params[cnt];
		if(param.name == name)
		{
			eff_paramid_t id(cnt);
#ifdef _DEBUG
			id.name = param.name;
#endif
			return id;
		}
	}

	eff_paramid_t id(-1);
#ifdef _DEBUG
	id.name = name;
#endif
	return id;
}

err_t C3DEffect::SetTexture(eff_paramid_t id, I3DTexture * pTexture)
{
	if(id.id < 0)
		return err_invalidarg;

	const shader_param_t & param = m_params[id];
	for(int_x ishader = 0; ishader < shader_num; ++ishader)
	{
		int_x slot = param.slots[ishader];
		if(slot < 0)
			continue;

		effect_texture_t & etex = m_textures[ishader][slot];
		SafeRelease(etex.pointer);
		etex.pointer = pTexture;
		SafeAddRef(etex.pointer);
		etex.binded = false;
	}
	return err_ok;
}

err_t C3DEffect::SetTexture(const char_16 * name, I3DTexture * pTexture)
{
	return SetTexture(FindParam(name), pTexture);
}

err_t C3DEffect::SetBuffer(eff_paramid_t id, I3DBuffer * pBuffer)
{
	if(id.id < 0)
		return err_invalidarg;

	const shader_param_t & param = m_params[id];
	for(int_x ishader = 0; ishader < shader_num; ++ishader)
	{
		vector<effect_buffer_t> & ebs = m_buffers[ishader];
		int_x slot = param.slots[ishader];
		if(slot < 0)
			continue;

		for(int_x ieb = 0; ieb < ebs.size(); ++ieb)
		{
			effect_buffer_t & eb = ebs[ieb];
			if(eb.slot == slot)
			{
				SafeRelease(eb.pointer);
				eb.pointer = pBuffer;
				SafeAddRef(eb.pointer);
				eb.binded = false;
			}
		}
	}
	return err_ok;
}

err_t C3DEffect::SetBuffer(const char_16 * name, I3DBuffer * pBuffer)
{
	return SetBuffer(FindParam(name), pBuffer);
}



err_t C3DEffect::SetVariable(eff_paramid_t id, const void * data, int_x size)
{
	if(id.id < 0)
	{
#ifdef _DEBUG
		log2(L"can not find variable \"%s\"", id.name.buffer);
#endif
		return err_invalidarg;
	}

	const shader_param_t & param = m_params[id];
	for(int_x ishader = 0; ishader < shader_num; ++ishader)
	{
		int_x slot = param.slots[ishader];
		if(slot < 0)
			continue;

		int_x offset = param.offsets[ishader];

		vector<effect_cbuffer_t> & ecbs = m_cbuffers[ishader];
		for(int_x iecb = 0; iecb < ecbs.size(); ++iecb)
		{
			effect_cbuffer_t & ecb = ecbs[iecb];
			if(ecb.slot != slot)
				continue;

			if(offset + size > ecb.size)
			{
				Assert(false);
				return err_invalidarg;
			}

			if(!ecb.buffer)
			{
				ecb.buffer = new byte_t[ecb.size];
				buffset8(ecb.buffer, ecb.size, 0);
			}

			buffcpy8(ecb.buffer + offset, data, size);
			ecb.invalid = true;
			ecb.binded = false;
			break;
		}
	}
	return err_ok;
}

err_t C3DEffect::SetVariable(const char_16 * name, const void * data, int_x size)
{
	return SetVariable(FindParam(name), data, size);
}

template<typename LamdaT>
static void effect_parse_state(linear_map<textw, textw> & states, const char_16 * szName, LamdaT lamda)
{
	textw value;
	if(states.get_value(szName, value))
		lamda(value);
}

void C3DEffect::_ParseShaders(const xml_node & node_shader, passstate_t & state)
{
	if(!node_shader.is_valid())
		return;
	const xml_attr & attr_file = node_shader[L"file"];
	const xml_attr & attr_vs = node_shader[L"vs"];
	const xml_attr & attr_ps = node_shader[L"ps"];
	const xml_attr & attr_hs = node_shader[L"hs"];
	const xml_attr & attr_ds = node_shader[L"ds"];
	const xml_attr & attr_gs = node_shader[L"gs"];
	const xml_attr & attr_cs = node_shader[L"cs"];
	if(attr_file.is_valid()) state.shader = attr_file.value;
	if(attr_vs.is_valid()) state.main[shader_vs] = attr_vs.value;
	if(attr_ps.is_valid()) state.main[shader_ps] = attr_ps.value;
	if(attr_hs.is_valid()) state.main[shader_hs] = attr_hs.value;
	if(attr_ds.is_valid()) state.main[shader_ds] = attr_ds.value;
	if(attr_gs.is_valid()) state.main[shader_gs] = attr_gs.value;
	if(attr_cs.is_valid()) state.main[shader_cs] = attr_cs.value;
}

void C3DEffect::_ParseStates(linear_map<textw, textw> & states, passstate_t & state)
{
	// rasterrize state
	effect_parse_state(states, L"cullmode", [&state](const textw & text) { V3d::Parse(state.cullmode, text); });
	effect_parse_state(states, L"fillmode", [&state](const textw & text) { V3d::Parse(state.fillmode, text); });
	effect_parse_state(states, L"clockwise", [&state](const textw & text) { state.clockwise = text.tobool(true); });
	effect_parse_state(states, L"multisampler", [&state](const textw & text) { state.multisampler = text.tobool(true); });
	effect_parse_state(states, L"scissor", [&state](const textw & text) { state.scissor = text.tobool(true); });
	effect_parse_state(states, L"z_bias", [&state](const textw & text) { state.z_bias = text.toix(0); });
	effect_parse_state(states, L"z_bias_clamp", [&state](const textw & text) { state.z_bias_clamp = text.tof32(0.0f); });
	effect_parse_state(states, L"z_bias_slope_scaled", [&state](const textw & text) { state.z_bias_slope_scaled = text.tof32(0.0f); });
	effect_parse_state(states, L"z_clip", [&state](const textw & text) { state.z_clip = text.tobool(true); });

	// depth stencial state
	effect_parse_state(states, L"depth_test", [&state](const textw & text) { state.depth_test = text.tobool(true); });
	effect_parse_state(states, L"depth_cmp", [&state](const textw & text) { V3d::Parse(state.depth_cmp, text); });
	effect_parse_state(states, L"depth_write", [&state](const textw & text) { state.depth_write = text.tobool(true); });

	effect_parse_state(states, L"stencil_test", [&state](const textw & text) { state.stencil_test = text.tobool(false); });
	effect_parse_state(states, L"stencil_read", [&state](const textw & text) { state.stencil_read = (uint_8)text.toix(0); });
	effect_parse_state(states, L"stencil_write", [&state](const textw & text) { state.stencil_write = (uint_8)text.toix(0); });
	effect_parse_state(states, L"stencil_ref", [&state](const textw & text) { state.stencil_write = (uint_8)text.toix(0); });

	effect_parse_state(states, L"stencil_front_cmp", [&state](const textw & text) { V3d::Parse(state.stencil_front_cmp, text); });
	effect_parse_state(states, L"stencil_front_op_pass", [&state](const textw & text) { V3d::Parse(state.stencil_front_op_pass, text); });
	effect_parse_state(states, L"stencil_front_op_fail", [&state](const textw & text) { V3d::Parse(state.stencil_front_op_fail, text); });
	effect_parse_state(states, L"stencil_front_op_depth_fail",
		[&state](const textw & text) { V3d::Parse(state.stencil_front_op_depth_fail, text); });
	effect_parse_state(states, L"stencil_back_cmp", [&state](const textw & text) { V3d::Parse(state.stencil_back_cmp, text); });
	effect_parse_state(states, L"stencil_back_op_pass", [&state](const textw & text) { V3d::Parse(state.stencil_back_op_pass, text); });
	effect_parse_state(states, L"stencil_back_op_fail", [&state](const textw & text) { V3d::Parse(state.stencil_back_op_fail, text); });
	effect_parse_state(states, L"stencil_back_op_depth_fail",
		[&state](const textw & text) { V3d::Parse(state.stencil_back_op_depth_fail, text); });

	// blend state
	effect_parse_state(states, L"blend_alpha2coverage", [&state](const textw & text) { state.blend_alpha2coverage = text.tobool(true); });
	effect_parse_state(states, L"blend_enable", [&state](const textw & text) { state.blend_enable = text.tobool(true); });
	effect_parse_state(states, L"blend_color_src", [&state](const textw & text) { V3d::Parse(state.blend_color_src, text); });
	effect_parse_state(states, L"blend_color_op", [&state](const textw & text) { V3d::Parse(state.blend_color_op, text); });
	effect_parse_state(states, L"blend_color_dst", [&state](const textw & text) { V3d::Parse(state.blend_color_dst, text); });
	effect_parse_state(states, L"blend_alpha_src", [&state](const textw & text) { V3d::Parse(state.blend_alpha_src, text); });
	effect_parse_state(states, L"blend_alpha_op", [&state](const textw & text) { V3d::Parse(state.blend_alpha_op, text); });
	effect_parse_state(states, L"blend_alpha_dst", [&state](const textw & text) { V3d::Parse(state.blend_alpha_dst, text); });
	effect_parse_state(states, L"blend_write_mask", [&state](const textw & text) { state.blend_write_mask = text.toui32(0); });
	effect_parse_state(states, L"blend_factor", [&state](const textw & text)
	{
		texttofarr(text.buffer(), text.length(), &state.blend_factor[0], arraysize(state.blend_factor), L',');
	});
	effect_parse_state(states, L"blend_sample_mask", [&state](const textw & text) { state.blend_sample_mask = text.toui32(0); });
}

void C3DEffect::_ParseSampler(linear_map<textw, textw> & states, sampler_state_t & state)
{
	effect_parse_state(states, L"filtermode", [&state](const textw & text) { V3d::Parse(state.filtermode, text); });
	effect_parse_state(states, L"address_u", [&state](const textw & text) { V3d::Parse(state.addressmode_u, text); });
	effect_parse_state(states, L"address_v", [&state](const textw & text) { V3d::Parse(state.addressmode_v, text); });
	effect_parse_state(states, L"address_w", [&state](const textw & text) { V3d::Parse(state.addressmode_w, text); });
	effect_parse_state(states, L"anisotropy", [&state](const textw & text) { state.anisotropy = text.toix(0); });
	effect_parse_state(states, L"cmp", [&state](const textw & text) { V3d::Parse(state.cmp, text); });
}

VENUS_END

