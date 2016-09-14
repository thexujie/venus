#pragma once

#include "BaseInc.h"

enum VerQuery
{
	VerQueryComments,
	VerQueryInternalName,
	VerQueryProductName,
	VerQueryCompanyName,
	VerQueryLegalCopyright,
	VerQueryProductVersion,
	VerQueryFileDescription,
	VerQueryLegalTrademarks,
	VerQueryPrivateBuild,
	VerQueryFileVersion,
	VerQueryOriginalFilename,
	VerQuerySpecialBuild,
};

class CFileVersion
{
public:
	CFileVersion(const char_16 * szFile);
	~CFileVersion();

	bool IsValid() const;
	bool GetLanguageAndCodePage(int_x & iLanguage, int_x & iCodePage) const;
	bool QueryVerInfo(VerQuery eQuery, char_16 * szText, int_x iSize) const;
	bool GetDescription(char_16 * szText, int_x iSize) const;
protected:
	void ConfirmValid() const;
	byte_t * m_pBlock;
	uint_32 m_uiBlockSize;
};
