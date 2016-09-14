#pragma once

VENUS_BEG

enum MessageBoxButtonE
{
	MessageBoxButtonOk,
	MessageBoxButtonCancel,
	MessageBoxButtonOkCancel,
	//MessageBoxButtonYes,
	//MessageBoxButtonNo,
	//MessageBoxButtonYesNo,
};

class V2D_API CMessageBox : public CForm
{
public:
	CMessageBox();

	void SetShowText(const char_16 * szName, const char_16 * szText, MessageBoxButtonE eButton);
	void OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const;

	static DialogResultE ShowMessage(IForm * pOwner, const char_16 * szMessage,
		const char_16 * szName, const char_16 * szTitle, 
		MessageBoxButtonE eButton = MessageBoxButtonOk);
private:
	CStatic m_name;
	CStatic m_detail;
	CButton m_btnOk;
	CButton m_btnCancel;
};

VENUS_END
