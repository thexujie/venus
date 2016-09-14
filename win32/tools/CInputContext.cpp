#include "stdafx.h"
#include "CInputContext.h"

VENUS_BEG

CInputContext::CInputContext():
m_hImc(NULL), m_hWnd(NULL)
{
	m_hModule = ::LoadLibrary(_T("Imm32.dll"));
	if(m_hModule)
	{
		m_pfnImmAssociateContext = (ImmAssociateContextFunT)::GetProcAddress(m_hModule, "ImmAssociateContext");
		m_pfnImmSetOpenStatus = (ImmSetOpenStatusFunT)::GetProcAddress(m_hModule, "ImmSetOpenStatus");
		m_pfnImmReleaseContext = (ImmReleaseContextFunT)::GetProcAddress(m_hModule, "ImmReleaseContext");
		m_pfnImmGetContext = (ImmGetContextFunT)::GetProcAddress(m_hModule, "ImmGetContext");
		m_pfnImmGetOpenStatus = (ImmGetOpenStatusFunT)::GetProcAddress(m_hModule, "ImmGetOpenStatus");
		m_pfnImmSetCompositionWindow = (ImmSetCompositionWindowFunT)::GetProcAddress(m_hModule, "ImmSetCompositionWindow");
		if(!m_pfnImmAssociateContext || !m_pfnImmSetOpenStatus || !m_pfnImmReleaseContext ||
		   !m_pfnImmGetContext || !m_pfnImmGetOpenStatus || !m_pfnImmSetCompositionWindow)
		{
			::FreeLibrary(m_hModule);
			m_hModule = NULL;
		}
	}

	if(!m_hModule)
	{
		log2(L"Error on initialize input context!");
		m_pfnImmAssociateContext = NULL;
		m_pfnImmSetOpenStatus = NULL;
		m_pfnImmReleaseContext = NULL;
		m_pfnImmGetContext = NULL;
		m_pfnImmGetOpenStatus = NULL;
		m_pfnImmSetCompositionWindow = NULL;
	}
}

CInputContext::~CInputContext()
{
	if(m_hWnd)
		SetHWND(NULL);
	if(m_hImc)
	{
		m_pfnImmReleaseContext(m_hWnd, m_hImc);
		m_hImc = NULL;
		m_pfnImmAssociateContext = NULL;
		m_pfnImmSetOpenStatus = NULL;
		m_pfnImmReleaseContext = NULL;
		m_pfnImmGetContext = NULL;
		m_pfnImmGetOpenStatus = NULL;
		m_pfnImmSetCompositionWindow = NULL;
	}
}

void CInputContext::SetHWND(HWND hWnd)
{
	if(!m_hModule)
		return;

	if(m_hImc)
	{
		m_pfnImmReleaseContext(m_hWnd, m_hImc);
		m_hImc = NULL;
	}
	m_hWnd = hWnd;
	if(m_hWnd)
		m_hImc = m_pfnImmGetContext(hWnd);
}

void CInputContext::SetImeMode(ImeModeE eMode)
{
	if(!m_hModule)
		return;

	if(m_eMode == eMode)
		return;

	m_eMode = eMode;
	switch(eMode)
	{
	case ImeModeDefault:
		log0(L"InputContext: Default.");
		m_pfnImmAssociateContext(m_hWnd, m_hImc);
		break;
	case ImeModeOn:
		log0(L"InputContext: On.");
		m_pfnImmAssociateContext(m_hWnd, m_hImc);
		m_pfnImmSetOpenStatus(m_hImc, true);
		break;
	case ImeModeOff:
		log0(L"InputContext: Off.");
		m_pfnImmAssociateContext(m_hWnd, m_hImc);
		m_pfnImmSetOpenStatus(m_hImc, false);
		break;
	case ImeModeDisable:
		log0(L"InputContext: Disabled.");
		m_pfnImmAssociateContext(m_hWnd, NULL);
		break;
	default:
		break;
	}
}

bool CInputContext::IsEnabled() const
{
	if(!m_hModule)
		return false;

	return m_pfnImmGetOpenStatus(m_hImc) != FALSE;
}

void CInputContext::SetCompositionWindow(COMPOSITIONFORM & form)
{
	if(!m_hModule)
		return;

	m_pfnImmSetCompositionWindow(m_hImc, &form);
}

void CInputContext::SetCompositionWindow(int_x iX, int_x iY)
{
	if(!m_hModule)
		return;

	COMPOSITIONFORM cpf = {0};
	cpf.dwStyle = CFS_POINT;
	cpf.ptCurrentPos.x = (int_32)iX;
	cpf.ptCurrentPos.y = (int_32)iY;
	SetCompositionWindow(cpf);
}

VENUS_END
