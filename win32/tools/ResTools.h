#pragma once

VENUS_BEG

enum ResTypeE
{
	// "FILE"
	ResTypeFile = 0,
	ResTypeCount,
};

class WIN32_API IResFile : virtual public IObject
{
public:
	virtual int_x GetSize() const = 0;
	virtual const byte_t * GetBuffer() const = 0;
};

class WIN32_API Resources
{
public:
	Resources() = delete;
	static IResFile * GetFile(int_x iId);
	static int_x GetText(int_x iId, char_16 * szText, int_x iSize);
};

VENUS_END
