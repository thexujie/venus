#pragma once

VENUS_BEG

class WIN32_API CHandle
{
public:
	CHandle();
	CHandle(HANDLE handle);
	CHandle(const CHandle & handle);
	~CHandle();

	CHandle & operator= (HANDLE handle);
	operator HANDLE();
	HANDLE * operator & ();
	const HANDLE * operator & () const;
	void Attach(HANDLE handle);
	void Detach();
	void Close();
protected:
	HANDLE m_handle;
};

VENUS_END
