#pragma once

VENUS_BEG

enum ConsoleColorModeE
{
	CCM_BLACK,
	CCM_DACK_BLUE,
	CCM_DARK_GREEN,
	CCM_DARK_SKY_BLUE,
	CCM_DARK_RED,
	CCM_DARK_PERPLE,
	CCM_DARK_YELLOW,
	CCM_DARK_GRAY,

	CCM_GRAY,
	CCM_BLUE,
	CCM_GREEN,
	CCM_SKY_BLUE,
	CCM_RED,
	CCM_PERPLE,
	CCM_YELLOW,
	CCM_WHITE,
};

class WIN32_API CSimpleStdOut : public ObjectT<IStdOut>
{
public:
	CSimpleStdOut();
	~CSimpleStdOut();

	void out(int_x level, const char_8 * format, void * args);
	void out(int_x level, const char_16 * format, void * args);

protected:
	void _cmd_setcolor(ConsoleColorModeE eForeColor, ConsoleColorModeE eBackColor);
	int_x _cmd_ontimer(int_x time);
private:
	textw m_bufferW;
	texta m_bufferA;

	HANDLE m_hOutput;
	HANDLE m_hInput;
};

VENUS_END
