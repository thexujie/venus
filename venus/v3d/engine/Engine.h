#pragma once

VENUS_BEG

class IApp3D;
class I3DFile;
class I3DVedio;
class I3DFrameBuffer;
class I3DExplorer;
class I3DBuffer;
class I3DTexture;
class I3DMesh;
class I3DMaterial;
class I3DEffect;
class I3DPass;
class I3DTechnique;
class I3DContext;
class I3DNode;
class I3DRenderable;
class I3DRenderObject;
class I3DFileDriver;
class I3DLight;

class V3D_API I3DObject : public IObject
{
public:
};

// 显示器
class V3D_API I3DOutput : public I3DObject
{
public:
	virtual rectix GetCoordinates() const = 0;
};

// 显卡
class V3D_API I3DAdapter : public I3DObject
{
public:
	virtual I3DOutput * EnumOutput(int_x iIndex) = 0;
};

class V3D_API I3DAnimator : public I3DObject
{
public:
	virtual ~I3DAnimator() {}
	virtual void Update(int_x iPeriod) = 0;
	virtual const float4x4 & GetTransform() const = 0;
};

class V3D_API I3DAction : public I3DObject
{
public:
	virtual err_t GetPosture(vector<float4x4> & posture, int_x iTime) const = 0;
};

class V3D_API I3DCamera : public I3DObject
{
public:
	virtual ~I3DCamera() {}

	virtual void SetViewPort(rectix viewport) = 0;
	virtual rectix GetViewPort() const = 0;

	virtual void SetMatrixP(const float4x4 & matProj) = 0;
	virtual float4x4 GetMatrixV() const = 0;
	virtual float4x4 GetMatrixP() const = 0;
	virtual float4x4 GetMatrixVP() const = 0;

	virtual float_32 GetVisual() const = 0;
	virtual float3 GetPosition() const = 0;
	virtual float3 GetLook() const = 0;
	virtual float3 GetUp() const = 0;
	virtual float3 GetRight() const = 0;

	virtual void SetVisual(float_32 fVisualDistance) = 0;
	virtual void SetPosition(const float3 & vPosition) = 0;
	virtual void SetLook(const float3 & vLook) = 0;
	virtual void SetUp(const float3 & vUp) = 0;

	// 平移
	virtual void ShiftY(float_32 fFLy) = 0;
	virtual void ShiftX(float_32 fStrafe) = 0;
	virtual void ShiftZ(float_32 fWalk) = 0;
	// 旋转
	virtual void RollX(float_32 fPitch) = 0;
	virtual void RollY(float_32 fYaw) = 0;
	virtual void RollZ(float_32 fRoll) = 0;
};

class V3D_API IScript : public IObject
{
public:
	virtual ~IScript() {}
};

class V3D_API IPlugin : public IObject
{
public:
	virtual ~IPlugin() {}
	virtual err_t Load(const char_16 * szPlugin) = 0;

	virtual void OnAppInitialize(IApp3D * pApp) = 0;
	virtual void OnUninitialize() = 0;
	virtual err_t OnIdle(int_x iPeriod) = 0;
	virtual void OnUpdate(int_x iPeriod) = 0;
	virtual void OnRender(int_x iPeriod) = 0;

	virtual IScript * GetScript(const char_16 * szScript) = 0;
};

class V3D_API I3DResource : public I3DObject
{
public:
};

class V3D_API I3DEngineResource : public I3DResource
{
public:
	virtual I3DExplorer * GetExplorer() const = 0;
};


class V3D_API I3DExplorer : public I3DObject
{
public:
	virtual ~I3DExplorer() {}

	virtual I3DVedio * GetVedio() = 0;
	virtual I3DFile * LoadFile(const char_x * szFile, ResourceTypeE eType = ResourceTypeDefault) = 0;
	virtual I3DTexture * GetTexture(const char_16 * szFile, ResourceTypeE eType = ResourceTypeTexture) = 0;
	virtual I3DMesh * GetMesh(const char_16 * szFile, ResourceTypeE eType = ResourceTypeMesh) = 0;
	virtual I3DAction * GetAction(const char_16 * szFile, ResourceTypeE eType = ResourceTypeAction) = 0;
	virtual I3DMaterial * GetMaterial(const char_16 * szFile, ResourceTypeE eType = ResourceTypeMaterial) = 0;
	virtual I3DEffect * GetEffect(const char_16 * szFile, ResourceTypeE eType = ResourceTypeEffect) = 0;

	virtual void ReleaseResouces() = 0;
};

class V3D_API I3DDriver : public I3DObject
{
public:
	virtual ~I3DDriver() {}

	virtual err_t Initialize() = 0;
};

#pragma region file
class V3D_API I3DFile : public IInputStream, public I3DResource
{
public:
	virtual ~I3DFile() {}
	virtual int_x GetSize() const = 0;
	virtual const byte_t * GetBuffer() const = 0;
};

class V3D_API I3DFileDriver : public I3DDriver
{
public:
	virtual ~I3DFileDriver() {}
	virtual I3DFile * GetFile(const char_16 * szFile) = 0;
};
#pragma endregion

#pragma region vedio

class V3D_API I3DVedio : public I3DObject
{
public:
	virtual ~I3DVedio() {}

	virtual I3DContext * CreateContext() = 0;
	virtual I3DFrameBuffer * CreateFrameBuffer() = 0;
	virtual I3DTexture * CreateTexture() = 0;
	virtual I3DBuffer * CreateBuffer() = 0;
	virtual I3DPass * CreatePass() = 0;
};

class V3D_API I3DVedioDriver : public I3DDriver
{
public:
	virtual ~I3DVedioDriver() {}

	virtual I3DAdapter * EnumAdapter(int_x iIndex) = 0;
	virtual I3DVedio * CreateVedio(VedioModeE eMode, I3DAdapter * pAdapter) = 0;
};

class V3D_API I3DVedioResource : public I3DResource
{
public:
	virtual I3DVedio * GetVedio() const = 0;
};

class V3D_API I3DShaderResource : public I3DVedioResource
{
public:
};

class V3D_API I3DContext : public I3DVedioResource
{
public:
	virtual err_t SetViewPort(rectix viewport) = 0;
	virtual err_t SetRenderTarget(I3DTexture ** ppBackBuffer, int_x iCount, I3DTexture * pDepthStencil) = 0;
	virtual err_t SetBackBuffer(I3DTexture * pBackBuffer, int_x iIndex = 0) = 0;
	virtual err_t SetDepthStencil(I3DTexture * pDepthStencil) = 0;
	virtual err_t SetCamera(I3DCamera * pCamera) = 0;
	virtual I3DCamera * GetCamera() const = 0;

	virtual err_t ClearBackBuffer(int_x iIndex, float4 color) = 0;
	virtual err_t ClearDepthStencil(bool bDepth, float_32 fDepth, bool bStencil, uint_8 uiStencil) = 0;

	virtual err_t UpdateBuffer(I3DBuffer * pBuffer, rectix rect, const void * data, int_x size) = 0;


	virtual err_t PassBeg(I3DPass * pPass) = 0;
	virtual err_t PassEnd() = 0;
	virtual err_t SetCBuffer(shader_e eShader, I3DBuffer * pBuffer, int_x iSlot) = 0;
	virtual err_t SetBuffer(shader_e eShader, I3DBuffer * pBuffer, int_x iSlot) = 0;
	virtual void SetTexture(shader_e eShader, I3DTexture * pTexture, int_x iSlot) = 0;

	virtual void SetVertexBuffer(I3DBuffer * pvBuffer, vertexformat_e eVertexFormat) = 0;
	virtual void SetIndexBuffer(I3DBuffer * piBuffer, indexformat_e eIndexFormat) = 0;
	virtual void DrawIndexed(int_x iVertexOffset, int_x iIndexOffset, int_x iIndexCount, primitiveformat_e ePrimitiveFormat) = 0;
};

class V3D_API I3DEffect2 : public I3DVedioResource
{
public:

};

class V3D_API I3DEffectObject : public I3DResource
{
public:
	virtual eff_paramid_t FindParam(const char_16 * name) const = 0;

	virtual err_t SetTexture(eff_paramid_t id, I3DTexture * pTexture) = 0;
	virtual err_t SetTexture(const char_16 * name, I3DTexture * pTexture) = 0;

	virtual err_t SetBuffer(eff_paramid_t id, I3DBuffer * pBuffer) = 0;
	virtual err_t SetBuffer(const char_16 * name, I3DBuffer * pBuffer) = 0;

	virtual err_t SetVariable(eff_paramid_t id, const void * data, int_x size) = 0;
	virtual err_t SetVariable(const char_16 * name, const void * data, int_x size) = 0;

	template<typename VT>
	err_t SetVariable(const char_16 * name, const VT & v)
	{
		return SetVariable(name, &v, sizeof(VT));
	}
};

class V3D_API I3DPass : public I3DEffectObject
{
public:
	virtual const char_16 * GetName() const = 0;
	virtual I3DEffect * GetEffect() const = 0;
	virtual void SetState(I3DEffect * pEffect, const passstate_t & state) = 0;
	virtual void SetSampler(shader_e shader, int_x slot, const sampler_state_t & state) = 0;

	virtual void GetShaderDesc(vector<shader_param_t> & params, vector<shader_cbuffer_t> & cbuffers) const = 0;

	virtual err_t PassBeg(I3DContext * pContext) = 0;
	virtual err_t Flush() = 0;
	virtual err_t PassEnd() = 0;
};

class V3D_API I3DEffect : public I3DEffectObject
{
public:
	virtual const char_16 * GetName() const = 0;
	virtual int_x GetPassCount() const = 0;
	virtual I3DPass * GetPass(renderpass_e pass) const = 0;
	virtual I3DPass * GetPass(const char_16 * szName) const = 0;

	virtual err_t PassBeg(I3DContext * pContext) = 0;
	virtual err_t Flush() = 0;
	virtual err_t PassEnd() = 0;
};

class V3D_API I3DFrameBuffer : public I3DVedioResource
{
public:
	virtual int_x Present() = 0;
	virtual int_x Create(int_x iWindowId, int_x iWidth, int_x iHeight, cmode_e eColorMode, int_x iCount,
		multisample_t multisample = multisample_1) = 0;
	virtual sizeix GetSize() const = 0;
	virtual err_t Resize(int_x iWidth, int_x iHeight) = 0;
	virtual err_t SetFullScreen(bool bFullScreen, I3DOutput * pOutput) = 0;
	virtual bool IsFullScreen() const = 0;
	virtual I3DTexture * GetBackBuffer(int_x iIndex) = 0;
};

class V3D_API I3DBuffer : public I3DShaderResource
{
public:
	virtual err_t Create(int_x iStride, int_x iCount,
		bufferusage_e eUsage = bufferusage_default,
		bufferbind_e eBind = bufferbind_resource,
		bufferaccess_e eAccess = bufferaccess_none,
		cmode_e mode = cmode_invalid,
		const void * pData = nullptr,
		int_x iDataSize = 0,
		int_x iDataSliceSize = 0) = 0;

	virtual err_t DestroyBuffer() = 0;

	virtual int_x GetStride() const = 0;
	virtual int_x GetSize() const = 0;
	virtual cmode_e GetMode() const = 0;

	virtual err_t SetBuffer(int_x iOffset, int_x iLength, const void * pData) = 0;
};

class V3D_API I3DTexture : public I3DShaderResource
{
public:
	virtual ~I3DTexture() {}
	virtual TextureDescT GetDesc() const = 0;

	virtual err_t Load(I3DFile * pFile) = 0;
	virtual err_t Create(sizeix size, cmode_e cmode, int_x iMipLevel = 1,
		bufferusage_e eUsage = bufferusage_default,
		bufferbind_e eBind = bufferbind_resource,
		bufferaccess_e eAccess = bufferaccess_none,
		multisample_t multisample = multisample_1,
		int_x iFlags = 0) = 0;

	virtual err_t Map(MappedBufferT & buffer, buffermap_e eMap) = 0;
	virtual err_t Unmap() = 0;
	virtual err_t Copy(I3DTexture * pTarget, const rectix & rect = rectix()) const = 0;
	virtual I2DRTarget * Get2DRTarget() = 0;
};

class V3D_API I3DMesh : public I3DResource
{
public:
	virtual ~I3DMesh() {}

	virtual bbox_t GetBoundingBox() const = 0;
	virtual I3DBuffer * GetIBuffer() const = 0;
	virtual I3DBuffer * GetVBuffer() const = 0;
	virtual vertexformat_e GetVertexFormat() const = 0;
	virtual indexformat_e GetIndexFormat() const = 0;
	virtual primitiveformat_e GetPrimitiveFormat() const = 0;

	virtual int_x GetNodeCount() const = 0;
	virtual const vector<mesh_node_t> & GetNodes() const = 0;
};

class V3D_API I3DMaterial : public I3DResource
{
public:
	virtual err_t SetEffect(I3DEffect * pEffect) = 0;
	virtual err_t SetTexture(eff_paramid_t id, I3DTexture * pTexture) = 0;
	virtual err_t SetTexture(const char_16 * name, I3DTexture * pTexture) = 0;

	virtual I3DEffect * GetEffect() const = 0;
	virtual I3DTexture * GetTexture(eff_paramid_t id) const = 0;
	virtual I3DTexture * GetTexture(const char_16 * name) const = 0;

	virtual err_t RenderBeg(I3DContext * pContext, int_x iSubSet) = 0;
	virtual err_t RenderEnd(I3DContext * pContext, int_x iSubSet) = 0;
};

#pragma endregion // vedio

class V3D_API I3DAudioDriver : public I3DDriver
{
public:
	virtual ~I3DAudioDriver() {}

};

class V3D_API I3DInput :public I3DObject
{
public:
	virtual err_t Create(int_x iWindowId) = 0;
	virtual err_t OnUpdate(int_x iPeriod) = 0;
};

class V3D_API I3DInputMouse : public I3DInput
{
public:
	virtual bool IsButtonDown(MouseButtonE eButton) const = 0;
	virtual pointix GetMousePos() const = 0;
	virtual event<I3DInputMouse *, MouseButtonE, bool> & OnButtonToggle() = 0;
	virtual event<I3DInputMouse *, int_x, int_x, int_x> & OnMouseMove() = 0;
};

class V3D_API I3DInputKeybd : public I3DInput
{
public:
	virtual bool IsKeyDown(KeyCodeE eKey) const = 0;
	virtual event<I3DInputKeybd *, KeyCodeE, bool> & OnKeyToggle() = 0;
};

class V3D_API I3DInputDriver : public I3DDriver
{
public:
	virtual ~I3DInputDriver() {}
	virtual I3DInputMouse * CreateMouseDevice(int_x iIndex) = 0;
	virtual I3DInputKeybd * CreateKeybdDevice(int_x iIndex) = 0;
};

class V3D_API I3DRenderEngine : public I3DObject
{
public:
	virtual ~I3DRenderEngine() {}

	virtual void Reset() = 0;
	virtual void AddLight(I3DLight * pLight) = 0;
	virtual void AddObject(I3DRenderObject * pObject) = 0;
	virtual void AddRenderable(I3DPass * pPass, I3DRenderable * pRenderable) = 0;

	virtual err_t RenderBeg() = 0;
	virtual err_t RenderEnd() = 0;
	virtual err_t RenderTo(I3DContext * pContext, I3DTexture * pRenderTarget, rectix viewport) = 0;
};

class V3D_API I3DScene : public I3DObject
{
public:
	virtual ~I3DScene() {}

	virtual void SetCamera(I3DCamera * pCamera) = 0;
	virtual I3DCamera * GetCamera() const = 0;

	virtual int_x GetTime() const = 0;
	//virtual I3DExplorer * GetExplorer() const = 0;
	virtual IPlugin * GetPlugin() const = 0;

	virtual void AddNode(I3DNode * pNode) = 0;
	virtual void RmvNode(I3DNode * pNode) = 0;
	virtual void RmvAllNode() = 0;

	virtual err_t Update(int_x iPeriod) = 0;
	virtual err_t Render(I3DRenderEngine * pRenderEngine) = 0;
};

class V3D_API IApp3D : public IService
{
public:
	virtual ~IApp3D() {}

	virtual I3DFileDriver * GetFileDriver() const = 0;
	virtual I3DVedioDriver * GetVedioDriver() const = 0;
	virtual I3DAudioDriver * GetAudioDriver() const = 0;
	virtual I3DInputDriver * GetInputDriver() const = 0;

	virtual I3DExplorer * CreateExplorer(I3DVedio * pVedio) = 0;
	virtual I3DScene * CreateScene(I3DExplorer * pExplorer) = 0;
};

class V3D_API I3DRenderable : public I3DObject
{
public:
	virtual err_t Render(I3DContext * pContext, I3DPass * pPass) const = 0;
};

class V3D_API I3DRenderObject : public I3DObject
{
public:
	virtual err_t PassBeg(I3DRenderEngine * pRenderEngine, renderpass_e pass) = 0;
	virtual err_t PassEnd(I3DRenderEngine * pRenderEngine, renderpass_e pass) = 0;
};

class V3D_API I3DNode : public I3DRenderObject
{
public:
	virtual ~I3DNode() {}

	virtual NodeTypeE GetNodeType() const = 0;

	//virtual I3DScene * GetScene() const = 0;
	virtual I3DExplorer * GetExplorer() const = 0;
	virtual I3DVedio * GetVedio() const = 0;

	virtual void SetFillMode(fillmode_e eFillMode) = 0;
	virtual fillmode_e GetFillMode() const = 0;
	virtual void SetTransform(const float4x4 & matr) = 0;
	virtual float4x4 GetTransform() const = 0;

	virtual void OnAdded(I3DScene * pScene) = 0;
	virtual void OnRemoved() = 0;
	virtual void Update(int_x iPeriod) = 0;
	virtual err_t Render(I3DContext * pContext) const = 0;

	virtual err_t PassBeg(I3DRenderEngine * pRenderEngine, renderpass_e pass) = 0;
	virtual err_t PassEnd(I3DRenderEngine * pRenderEngine, renderpass_e pass) = 0;
};

class V3D_API I3DLight : public I3DObject
{
public:
	virtual I3DTexture * GetShadowMap() const = 0;
};

/**
* .mesh 网格数据
* .act 一个动作的数据
* .mtl 材质
* .obj 对象文件
*/
class V3D_API I3DEntity : public I3DRenderObject
{
public:
	virtual err_t Load(const char_16 * szEntity) = 0;
	virtual err_t SetMesh(I3DMesh * pMesh) = 0;
	virtual err_t SetAction(int_x iAction) = 0;
};

VENUS_END
