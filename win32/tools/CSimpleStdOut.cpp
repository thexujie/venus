#include "stdafx.h"
#include "CSimpleStdOut.h"

VENUS_BEG

CSimpleStdOut::CSimpleStdOut()
{
	m_hOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
	m_hInput  = ::GetStdHandle(STD_INPUT_HANDLE);

	//if(m_hInput)
	//	GetApp()->SetTimer(bind(this, &CSimpleStdOut::_cmd_ontimer), 100, 0);
}

CSimpleStdOut::~CSimpleStdOut()
{
	//if(m_hInput)
	//	GetApp()->KillTimer(bind(this, &CSimpleStdOut::_cmd_ontimer), 0);

	m_hOutput = m_hInput = NULL;
}

void CSimpleStdOut::out(int_x level, const char_8 * format, void * args)
{
	m_bufferA.format_args(format, args);
	if(m_hOutput)
	{
		DWORD dwWriten = 0;
		switch(level)
		{
		case 0:
			_cmd_setcolor(CCM_WHITE, CCM_BLACK);
			break;
		case 1:
			_cmd_setcolor(CCM_YELLOW, CCM_BLACK);
			break;
		case 2:
			_cmd_setcolor(CCM_RED, CCM_BLACK);
			break;
		case 3:
			_cmd_setcolor(CCM_BLUE, CCM_BLACK);
			break;
		case 4:
			_cmd_setcolor(CCM_GREEN, CCM_BLACK);
			break;
		default:
			break;
		}
		WriteConsoleA(m_hOutput, m_bufferA, (DWORD)m_bufferA.length(), &dwWriten, NULL);
		WriteConsoleA(m_hOutput, "\r\n", 2, &dwWriten, NULL);
	}
	OutputDebugStringA(m_bufferA);
	OutputDebugStringA("\r\n");
}

void CSimpleStdOut::out(int_x level, const char_16 * format, void * args)
{
	m_bufferW.format_args(format, args);
	if(m_hOutput)
	{
		DWORD dwWriten = 0;
		switch(level)
		{
		case 0:
			_cmd_setcolor(CCM_WHITE, CCM_BLACK);
			break;
		case 1:
			_cmd_setcolor(CCM_YELLOW, CCM_BLACK);
			break;
		case 2:
			_cmd_setcolor(CCM_RED, CCM_BLACK);
			break;
		case 3:
			_cmd_setcolor(CCM_BLUE, CCM_BLACK);
			break;
		case 4:
			_cmd_setcolor(CCM_GREEN, CCM_BLACK);
			break;
		default:
			break;
		}
		WriteConsoleW(m_hOutput, m_bufferW, (DWORD)m_bufferW.length(), &dwWriten, NULL);
		WriteConsoleW(m_hOutput, L"\r\n", 2, &dwWriten, NULL);
	}
	OutputDebugStringW(m_bufferW);
	OutputDebugStringW(L"\r\n");
}

void CSimpleStdOut::_cmd_setcolor(ConsoleColorModeE eForeColor, ConsoleColorModeE eBackColor)
{
	::SetConsoleTextAttribute(m_hOutput, (WORD)((eBackColor << 4) |  eForeColor));
}

int_x CSimpleStdOut::_cmd_ontimer(int_x time)
{
	if(!m_hInput)
		return 0;

	// 一次只处理 20 个.
	DWORD uiCount = 0;
	GetNumberOfConsoleInputEvents(m_hInput, &uiCount);
	if(uiCount)
	{
		INPUT_RECORD records[20];
		ReadConsoleInputW(m_hInput, records, 20, &uiCount);

		for(uint_32 cnt = 0; cnt < uiCount; ++cnt)
		{
			const INPUT_RECORD & record = records[cnt];
			if(record.EventType == MOUSE_EVENT)
			{
				const MOUSE_EVENT_RECORD & mrecord = record.Event.MouseEvent;
				if(mrecord.dwEventFlags & MOUSE_WHEELED)
				{
					int_16 iWheel = u32hi16(mrecord.dwButtonState) / WHEEL_DELTA;
					CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
					GetConsoleScreenBufferInfo(m_hOutput, &csbiInfo);

					SMALL_RECT srctWindow = {};
					if(iWheel > 0 && csbiInfo.srWindow.Top > 0)
						srctWindow.Top = /*csbiInfo.srWindow.Top*/ -iWheel;

					if(iWheel < 0 && csbiInfo.srWindow.Bottom < csbiInfo.dwSize.Y)
						srctWindow.Top = /*csbiInfo.srWindow.Top*/ -iWheel;
					srctWindow.Bottom = srctWindow.Top;
					srctWindow.Left = /*csbiInfo.srWindow.Left*/0;
					srctWindow.Right = /*csbiInfo.srWindow.Right*/0;

					SetConsoleWindowInfo(m_hOutput, FALSE, &srctWindow);

					// 滚动条没有重绘，在 win.xp 里不会，也没有办法解决。
					//HWND hWnd = GetConsoleWindow();
					//int iScrollPos = GetScrollPos(hWnd, SB_VERT);
					//SetScrollPos(hWnd, SB_VERT, iScrollPos, TRUE);
				}
			}
		}
	}
	return 0;
}

VENUS_END
