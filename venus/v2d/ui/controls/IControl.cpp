#include "stdafx.h"
#include "IControl.h"

VENUS_BEG

IMonitorManager * IMonitorManager::GetInstance()
{
	return GetService<IMonitorManager>(OID_IMonitorManager);
}

IUILoader * IUILoader::GetInstance()
{
	return GetService<IUILoader>(OID_IUILoader);
}

VENUS_END
