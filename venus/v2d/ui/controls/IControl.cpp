#include "stdafx.h"
#include "IControl.h"

VENUS_BEG

IMonitorManager * IMonitorManager::GetInstance()
{
	return GetService<IMonitorManager>(OID_IMonitorManager);
}

VENUS_END
