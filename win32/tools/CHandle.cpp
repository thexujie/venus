#include "stdafx.h"
#include "CHandle.h"

VENUS_BEG

CHandle::CHandle() : m_handle(NULL) {}
CHandle::CHandle(HANDLE handle) : m_handle(handle) {}
CHandle::~CHandle() {if(m_handle) ::CloseHandle(m_handle);}
CHandle::CHandle(const CHandle & another) : m_handle(another.m_handle) {}


CHandle & CHandle::operator= (HANDLE handle) { Close(); Attach(handle); return *this;}
CHandle::operator HANDLE() { return m_handle;}

HANDLE * CHandle::operator & () { return &m_handle;}
const HANDLE * CHandle::operator & () const { return &m_handle;}

void CHandle::Attach(HANDLE handle) { m_handle = handle;}
void CHandle::Detach() { m_handle = NULL;}
void CHandle::Close() {if(m_handle) ::CloseHandle(m_handle);}

VENUS_END
