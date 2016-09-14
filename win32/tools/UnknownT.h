#pragma once

VENUS_BEG

template<typename ...Args>
class UnknownT : public Args...
{
public:
	UnknownT() :m_uiRef(1) {}
	virtual ~UnknownT() {}

	IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv)
	{
		return E_NOTIMPL;
	}

	IFACEMETHODIMP_(ULONG) AddRef()
	{
		return InterlockedIncrement(&m_uiRef);
	}
	IFACEMETHODIMP_(ULONG) Release()
	{
		uint_32 uiRef = InterlockedDecrement(&m_uiRef);
		if(0 == uiRef)
		{
			delete this;
		}
		return uiRef;
	}

protected:
	uint_32 m_uiRef;
};

VENUS_END
