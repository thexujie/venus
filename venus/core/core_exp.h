#pragma once

VENUS_BEG

const uint_x EXCEPTION_MAX_NAME = 64;
const uint_x EXCEPTION_MAX_WHAT = 256;

enum /*SystemException*/
{
	NotSurpportException,
	IllegalArgumentException,
	IndexOutOfBoundsException,
	NullPointerException,
	IllegalOperationException,
	BadStateException,
	OutOfMemoryException,
	NotImplementException,
	EndOfStreamException,
	StreamBadStateException,
	FileNotFoundException,
	OverflowException,
};

class CORE_API exp_base
{
public:
	exp_base()
	{
		m_uiNumber = 0;
		textempty(m_szName, EXCEPTION_MAX_NAME);
		textempty(m_szWhat, EXCEPTION_MAX_WHAT);
	}

	exp_base(const char_x * szWhat)
	{
		m_uiNumber = 0;
		textempty(m_szName, EXCEPTION_MAX_NAME);
		textcpy(m_szWhat, EXCEPTION_MAX_WHAT, szWhat);
	}

	exp_base(uint_x uiNumber, const char_x * szName)
	{
		m_uiNumber = uiNumber;
		textcpy(m_szName, EXCEPTION_MAX_WHAT, szName);
		textempty(m_szWhat, EXCEPTION_MAX_WHAT);
	}

	exp_base(uint_x uiNumber, const char_x * szName, const char_x * szWhat)
	{
		m_uiNumber = uiNumber;
		textcpy(m_szName, EXCEPTION_MAX_NAME, szName);
		textcpy(m_szWhat, EXCEPTION_MAX_WHAT, szWhat);
	}

	uint_x GetNumber() const { return m_uiNumber; }
	const char_x * GetName() const { return m_szName; }
	char_x * GetWhat() { return m_szWhat; }
	const char_x * GetWhat() const { return m_szWhat; }
protected:
	uint_x m_uiNumber;
	char_x m_szName[EXCEPTION_MAX_NAME];
	char_x m_szWhat[EXCEPTION_MAX_WHAT];
};

class CORE_API exp_err : exp_base
{
public:
	exp_err()
	{
		m_uiNumber = 0;
	}

	exp_err(err_t err)
	{
		m_uiNumber = err;
	}
};

class CORE_API exp_not_supported : public exp_base
{
public:
	exp_not_supported(const char_x * szWhat = PCHAR_EMPTY)
		: exp_base(NotSurpportException, _T("exp_not_supported"), szWhat)
	{
	}
};

class CORE_API exp_illegal_argument : public exp_base
{
public:
	exp_illegal_argument(const char_x * szWhat = PCHAR_EMPTY)
		: exp_base(IllegalArgumentException, _T("exp_illegal_argument"), szWhat)
	{
	}
};

class CORE_API exp_out_of_bound : public exp_base
{
public:
	exp_out_of_bound(const char_x * szWhat = PCHAR_EMPTY)
		: exp_base(IndexOutOfBoundsException, _T("exp_out_of_bound"), szWhat)
	{
	}
};

class CORE_API exp_nullptr : public exp_base
{
public:
	exp_nullptr(const char_x * szWhat = PCHAR_EMPTY)
		: exp_base(NullPointerException, _T("exp_nullptr"), szWhat)
	{
	}
};

class CORE_API exp_illegal_operation : public exp_base
{
public:
	exp_illegal_operation(const char_x * szWhat = PCHAR_EMPTY)
		: exp_base(IllegalOperationException, _T("exp_illegal_operation"), szWhat)
	{
	}
};

class CORE_API exp_bad_state : public exp_base
{
public:
	exp_bad_state(const char_x * szWhat = PCHAR_EMPTY)
		: exp_base(BadStateException, _T("exp_bad_state"), szWhat)
	{
	}
};

class CORE_API exp_out_of_memory : public exp_base
{
public:
	exp_out_of_memory(const char_x * szWhat = PCHAR_EMPTY)
		: exp_base(OutOfMemoryException, _T("exp_out_of_memory"), szWhat)
	{
	}
};

class CORE_API exp_not_implement : public exp_base
{
public:
	exp_not_implement(const char_x * szWhat = PCHAR_EMPTY)
		: exp_base(NotImplementException, _T("exp_not_implement"), szWhat)
	{
	}
};

class CORE_API exp_overflow : public exp_base
{
public:
	exp_overflow(const char_x * szWhat = PCHAR_EMPTY)
		: exp_base(OverflowException, _T("exp_overflow"), szWhat)
	{
	}
};

class CORE_API exp_io : public exp_base
{
public:
	exp_io()
	{
	}
	exp_io(const char_x * szWhat)
		: exp_base(szWhat)
	{
	}
};

class CORE_API exp_end_of_stream : public exp_io
{
public:
	exp_end_of_stream(){}
	exp_end_of_stream(const char_x * szWhat)
		: exp_io(szWhat)
	{
	}
};

class CORE_API exp_file_not_found : public exp_io
{
public:
	exp_file_not_found()
	{
	}
	exp_file_not_found(const char_x * szWhat)
		: exp_io(szWhat)
	{
	}
};

VENUS_END
