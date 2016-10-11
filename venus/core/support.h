#pragma once

VENUS_BEG

typedef IObject IService;

class CORE_API IStdOut : public IService
{
public:
	virtual void out(int_x level, const char_8 * format, void * args) = 0;
	virtual void out(int_x level, const char_16 * format, void * args) = 0;
};
static const oid_t OID_IStdOut = {L"IStdOut"};

enum AppRunE
{
	AppRunLoop,
	AppRunDialog,
	AppRunPopup,
};

class CORE_API IApp : public IObject
{
public:
	virtual ~IApp() {}

	virtual err_t Initialize() = 0;
	virtual void UnInitialize() = 0;

	virtual IObject * CreateObject(const oid_t & oid, void * pParam = nullptr) = 0;
	virtual bool RegisterObject(const oid_t & oid, function<IObject *(void *)> fnCreate, bool bReplace) = 0;
	virtual bool UnregisterObject(const oid_t & oid) = 0;

	virtual void SetService(const oid_t & oid, IService * pService) = 0;
	virtual IService * GetService(const oid_t & oid) const = 0;

	virtual void SetTimer(function<int_x(int_x)> fun, int_x iPeriod, int_x iId = 0) = 0;
	virtual void KillTimer(function<int_x(int_x)> fun, int_x iId = 0) = 0;

	virtual int_x OnRun() = 0;
	virtual int_x Run(int_x iFormId, AppRunE eAppRun) = 0;
	virtual int_x EndRun(int_x iFormId, int_x iResult) = 0;
};

template<typename ST>
ST * GetService(const oid_t & oid)
{
	IApp * pApp = GetApp();
	if(!pApp)
		return nullptr;

	IService * pService = pApp->GetService(oid);
	if(!pService)
		return nullptr;
#ifdef _CPPRTTI
	return dynamic_cast<ST *>(pService);
#else
	return (ST *)pService;
#endif
}

template<typename ST>
void SetService(const oid_t & oid, ST * pService)
{
	IApp * pApp = GetApp();
	if(pApp)
		pApp->SetService(oid, pService);
}

CORE_API IApp * GetApp();
CORE_API IApp * SetApp(IApp * pApp);

inline IStdOut * GetStdOut()
{
	return GetService<IStdOut>(OID_IStdOut);
}

VENUS_END
