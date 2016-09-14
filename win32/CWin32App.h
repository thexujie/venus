#pragma once

VENUS_BEG

class WIN32_API CWin32App : public ObjectT<IApp>
{
public:
	CWin32App();
	~CWin32App();

	err_t Initialize();
	void UnInitialize();

	IObject * CreateObject(const oid_t & oid, void * pParam);
	bool RegisterObject(const oid_t & oid, function<IObject *, void *> fnCreate, bool bReplace);
	bool UnregisterObject(const oid_t & oid);

	void SetService(const oid_t & oid, IService * pService);
	IService * GetService(const oid_t & oid) const;

	void SetTimer(function<int_x, int_x> fun, int_x iPeriod, int_x iId = 0);
	void KillTimer(function<int_x, int_x> fun, int_x iId = 0);

	void Create2DDevice(Device2DTypeE eType);

	int_x OnRun();
	int_x Run(int_x iFormId, AppRunE eAppRun);
	int_x EndRun(int_x iFormId, int_x iResult);

	int_x RunLoop(int_x iFormId);
	int_x RunDialog(int_x iFormId);
	int_x RunPopup(int_x iFormId);
public:
	void OnTimerProc();

private:
	struct ServiceT
	{
		oid_t oid;
		IService * pService;
	};
	vector<ServiceT> m_services;

	struct TimerT
	{
		function<int_x, int_x> fun;
		//! 间隔
		int_x iPeriod;
		//! Id，当 fun 和 iPeriod 都相同时，可以用于区分
		int_x iId;
		//! 自注册以来，累计被调用的次数
		int_x iTime;

		bool operator == (const TimerT & another) const
		{
			return fun == another.fun && iId == another.iId;
		}
	};
	vector<TimerT> m_timers;
	int_x m_iTimerId;

	struct ObjectInfoT
	{
		oid_t oid;
		function<IObject *, void *> fnCreate;
	};
	vector<ObjectInfoT> m_objectInfos;

	event<int_x> OnTimer100Event;
};

VENUS_END
