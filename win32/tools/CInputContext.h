#pragma once

#include <Imm.h>

VENUS_BEG

class WIN32_API CInputContext
{
public:
	CInputContext();
	~CInputContext();

	void SetHWND(HWND hWnd);
	void SetImeMode(ImeModeE eMode = ImeModeDefault);

	bool IsEnabled() const;

	void SetCompositionWindow(COMPOSITIONFORM & form);
	void SetCompositionWindow(int_x iX, int_x iY);

	bool IsValid() const { return m_hImc != NULL; }
	operator HIMC() { return m_hImc;}
private:

	typedef HIMC(WINAPI * ImmAssociateContextFunT)(HWND, HIMC);
	typedef BOOL(WINAPI * ImmSetOpenStatusFunT)(HIMC, BOOL);
	typedef BOOL(WINAPI * ImmReleaseContextFunT)(HWND, HIMC);
	typedef HIMC(WINAPI * ImmGetContextFunT)(HWND);
	typedef BOOL(WINAPI * ImmGetOpenStatusFunT)(HIMC);
	typedef BOOL(WINAPI * ImmSetCompositionWindowFunT)(HIMC, LPCOMPOSITIONFORM);

	HMODULE m_hModule;

	ImmAssociateContextFunT m_pfnImmAssociateContext;
	ImmSetOpenStatusFunT m_pfnImmSetOpenStatus;
	ImmReleaseContextFunT m_pfnImmReleaseContext;
	ImmGetContextFunT m_pfnImmGetContext;
	ImmGetOpenStatusFunT m_pfnImmGetOpenStatus;
	ImmSetCompositionWindowFunT m_pfnImmSetCompositionWindow;

	HWND m_hWnd;
	HIMC m_hImc;
	ImeModeE m_eMode;
};

VENUS_END
