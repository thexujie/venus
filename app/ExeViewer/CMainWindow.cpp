#include "stdafx.h"
#include "CMainWindow.h"

#include "CMapFile.h"

#include <ImageHlp.h>
#pragma comment(lib, "imagehlp.lib")

CMainWindow::CMainWindow() :
m_bUnDecorate(false)
{
	SetFormType(FormTypePopup);
	SetBorderType(BorderType2Inactive);
	m_txbInfo.SetAutoHideScrollBarY(false);
}

CMainWindow::~CMainWindow()
{

}

void CMainWindow::OnCreate()
{
	m_dropTarget.Register((HWND)GetHost()->GetFormId(), this);
	CForm::OnCreate();
}

void CMainWindow::OnLoaded()
{
}

void CMainWindow::PreOnKeyDown(KeyCodeE eKeyCode)
{
	CForm::PreOnKeyDown(eKeyCode);
}


static int_x RvaToOffset(int_x iRva, int_x(*secTable)[3], int_x iSecCount)
{
	int_x iBeg = 0, iEnd = 0, iPos = 0;
	for(int_x cnt = 0; cnt != iSecCount; ++cnt)
	{
		iBeg = secTable[cnt][0];
		iEnd = secTable[cnt][1];
		iPos = secTable[cnt][2];
		if(iRva >= iBeg && iRva < iEnd)
		{
			iRva -= iBeg;
			iRva += iPos;
			return iRva;
		}
	}
	return -1;
}

int_x CMainWindow::OnTextBoxShowMenu(IControl * pControl, pointix point)
{
	if(!m_txbInfo.IsFocused())
		m_txbInfo.Focus(true);
	MenuT menu;

	bool bCanPaste = !m_txbInfo.IsReadOnly() && ClipboardCheckData(ClipboardDataTypeText16);

	menu.AddItem(1, L"����", m_txbInfo.HasTextSelected() && !m_txbInfo.IsReadOnly() ? MenuFlagNone : MenuFlagDisable);
	menu.AddItem(2, L"����", m_txbInfo.HasTextSelected() ? MenuFlagNone : MenuFlagDisable);
	menu.AddItem(3, L"ճ��", bCanPaste ? 0 : MenuFlagDisable);
	menu.AddItem(4, L"ɾ��", m_txbInfo.HasTextSelected() && !m_txbInfo.IsReadOnly() ? MenuFlagNone : MenuFlagDisable);
	menu.AddSplit();
	menu.AddItem(5, L"ѡ��ȫ��", m_txbInfo.IsSelectAll() ? MenuFlagDisable : MenuFlagNone);
	menu.AddSplit();
	menu.AddItem(6, L"ת������", m_bUnDecorate);
	menu.AddSplit();
	menu.AddItem(7, L"�Զ�����", m_txbInfo.GetTextWrap() != TextWrapNone);
	menu.AddItem(8, L"ֻ��", m_txbInfo.IsReadOnly());
	switch(m_txbInfo.PopupMenu(point, &menu))
	{
	case 1:
		m_txbInfo.DoCut();
		break;
	case 2:
		m_txbInfo.DoCopy();
		break;
	case 3:
		m_txbInfo.DoPaste();
		break;
	case 4:
		m_txbInfo.DeleteSelected();
		break;
	case 5:
		m_txbInfo.SelectAll();
		break;
	case 6:
		m_bUnDecorate = !m_bUnDecorate;
		SetExeFileName(m_filePath);
		break;
	case 7:
		m_txbInfo.SetTextWrap(m_txbInfo.GetTextWrap() == TextWrapNone ? TextWrapWord : TextWrapNone);
		break;
	case 8:
		m_txbInfo.SetReadOnly(!m_txbInfo.IsReadOnly());
		break;
	default:
		break;
	}

	return 1;
}

int_x CMainWindow::OnPaneMouseDown(IControl * pControl, pointix point)
{
	return 0;
}

void CMainWindow::SetExeFileName(const char_16 * szExeFile)
{
	m_filePath = szExeFile;
	const int_x MAX_SEC_COUNT = 64;
	const int_x MAX_NAME_LEN = 512;

	m_txlFile.SetText(szExeFile);
	m_txbInfo.SetText(L"");
	CMapFile file(szExeFile);
	if(!file.GetHandle())
	{
		char_16 szError[128];
		Win32::FormatWinError(szError, 128);

		m_txbInfo.SetText(L"��������ԭ���޷����ļ���\n");
		m_txbInfo.AddText(szError);
		return;
	}
	CMapFileView view;

	IMAGE_DOS_HEADER dosHeader;
	struct ImageHeaderT
	{
		DWORD Signature;
		IMAGE_FILE_HEADER FileHeader;
	};
	// IMAGE_NT_HEADERS ������ 32/64 λ�ġ�
	//IMAGE_NT_HEADERS ntHeaders;
	ImageHeaderT ntHeader;
	IMAGE_OPTIONAL_HEADER32 opHeader32;
	IMAGE_OPTIONAL_HEADER64 opHeader64;

	int_x secTable[MAX_SEC_COUNT][3];
	int_x iSecCount = 0;

	if(!file.MapView(view))
	{
		m_txbInfo.SetText(L"û�ж�ȡ���ļ����ݣ��������ļ������ڻ����ļ���СΪ0��");
		return;
	}

	if(!view.ReadFull(0, &dosHeader, sizeof(dosHeader)))
	{
		m_txbInfo.SetText(L"���ǺϷ��� DOS �ļ�����Ϊȱʧ DOS_HEADER��");
		return;
	}

	if(dosHeader.e_magic != IMAGE_DOS_SIGNATURE)
	{
		m_txbInfo.SetText(L"���ǺϷ��� DOS �ļ���DOS_SIGNATURE ��ƥ�䡣");
		return;
	}

	if(!view.ReadFull(dosHeader.e_lfanew, &ntHeader, sizeof(ntHeader)))
	{
		m_txbInfo.SetText(L"���ǺϷ��� NT �ļ�����Ϊȱʧ NT_HEADERS��");
		return;
	}

	if(ntHeader.Signature != IMAGE_NT_SIGNATURE)
	{
		m_txbInfo.SetText(L"���ǺϷ��� NT �ļ���NT_SIGNATURE ��ƥ�䡣");
		return;
	}

	int_x iReadPosition = dosHeader.e_lfanew + sizeof(ntHeader);
	int_x iExportAddr = 0;
	switch(ntHeader.FileHeader.Machine)
	{
	case IMAGE_FILE_MACHINE_I386: // x86
		if(!view.ReadFull(iReadPosition, &opHeader32, sizeof(IMAGE_OPTIONAL_HEADER32)))
		{
			m_txbInfo.SetText(L"���ǺϷ��� NT �ļ�����Ϊȱʧ IMAGE_OPTIONAL_HEADER32��");
			return;
		}
		iReadPosition += sizeof(IMAGE_OPTIONAL_HEADER32);
		iExportAddr = opHeader32.DataDirectory[0].VirtualAddress;
		break;
	case IMAGE_FILE_MACHINE_AMD64: // x64
		if(!view.ReadFull(iReadPosition, &opHeader64, sizeof(IMAGE_OPTIONAL_HEADER64)))
		{
			m_txbInfo.SetText(L"���ǺϷ��� NT �ļ�����Ϊȱʧ IMAGE_OPTIONAL_HEADER64��");
			return;
		}
		iReadPosition += sizeof(IMAGE_OPTIONAL_HEADER64);
		iExportAddr = opHeader64.DataDirectory[0].VirtualAddress;
		break;
	default:
		m_txbInfo.SetText(L"�޷�ʶ��Ĵ������ܹ���");
		return;
	}

	if(iExportAddr == 0)
	{
		m_txbInfo.SetText(L"û�е�����Ϣ��");
		return;
	}

	iSecCount = ntHeader.FileHeader.NumberOfSections;
	if(iSecCount > MAX_SEC_COUNT)
	{
		m_txbInfo.SetText(L"�ļ��� �ε�����̫�࣬�޷�����");
		return;
	}

	IMAGE_SECTION_HEADER secHeader;
	for(int_x cnt = 0; cnt != iSecCount; ++cnt)
	{
		if(!view.ReadFull(iReadPosition + sizeof(secHeader)* cnt, &secHeader, sizeof(secHeader)))
		{
			m_txbInfo.SetText(L"�޷���ȡ����Ϣ��");
			return;
		}

		secTable[cnt][0] = secHeader.VirtualAddress;
		secTable[cnt][1] = secHeader.VirtualAddress + secHeader.SizeOfRawData;
		secTable[cnt][2] = secHeader.PointerToRawData;
	}

	int_x iExpOffset = RvaToOffset(iExportAddr, secTable, iSecCount);
	if(iExpOffset < 0)
	{
		m_txbInfo.SetText(L"�ļ����ݴ����ڶ����޷���λ�� ������");
		return;
	}

	IMAGE_EXPORT_DIRECTORY expDirectory;
	if(!view.ReadFull(iExpOffset, &expDirectory, sizeof(expDirectory)))
	{
		m_txbInfo.SetText(L"�޷���ȡ��������");
		return;
	}

	//Printf("��ģ�����ơ�%s\n", (TCHAR*)((DWORD)pStartAddress + iOffset));
	//printf("Created Time    = %ld\n",pExp->TimeDateStamp);
	//printf("NumbersOfExport    = %ld\n",pExp->NumberOfFunctions);
	//printf("ExportBaseOrdinals = %ld\n",pExp->Base);

	//�������������
	int_x iNameOffset = RvaToOffset(expDirectory.AddressOfNames, secTable, iSecCount);
	if(iNameOffset < 0)
	{
		m_txbInfo.SetText(L"�ļ����ݴ����޷���λ�����ֱ��е����֡�");
		return;
	}
	int_x iOrdinalsOffset = RvaToOffset(expDirectory.AddressOfNameOrdinals, secTable, iSecCount);
	if(iOrdinalsOffset < 0)
	{
		m_txbInfo.SetText(L"�ļ����ݴ����޷���λ�����ֱ��е�������");
		return;
	}

	int_x iAddrOffset = RvaToOffset(expDirectory.AddressOfFunctions, secTable, iSecCount);
	if(iAddrOffset < 0)
	{
		m_txbInfo.SetText(L"�ļ����ݴ����޷���λ�����ֱ��еĻ�ַ��");
		return;
	}

	char_8 szName[MAX_NAME_LEN];
	char_8 szNameA[MAX_NAME_LEN];
	char_16 szNameW[MAX_NAME_LEN];
	char_16 szBuffer[1024];
	int_x iNameCount = expDirectory.NumberOfNames;

	try
	{
		textw text;
		int_32 iName = 0;
		int_32 iOff = 0;
		int_16 iOrdinal = 0;
		int_32 iAddr = 0;
		for(int_x cnt = 0; cnt != iNameCount; ++cnt)
		{
			if(!view.ReadFull(iNameOffset + cnt * sizeof(int_32), &iOff, sizeof(int_32)))
			{
				text.set(L"�ļ����ݴ���");
				break;
			}

			if(iOff < 0)
			{
				text.set(L"�ļ����ݴ���");
				break;
			}

			iOff = (int_32)RvaToOffset(iOff, secTable, iSecCount);
			if(iOff < 0)
			{
				text.set(L"�ļ����ݴ���");
				break;
			}
			if(!view.ReadText(iOff, szName, MAX_NAME_LEN))
			{
				text.set(L"�ļ����ݴ���");
				break;
			}

			if(!view.ReadFull(iOrdinalsOffset + cnt * sizeof(int_16), &iOrdinal, sizeof(int_16)))
			{
				text.set(L"�ļ����ݴ���");
				break;
			}

			if(!view.ReadFull(iAddrOffset + iOrdinal * sizeof(int_32), &iAddr, sizeof(int_32)))
			{
				text.set(L"�ļ����ݴ���");
				break;
			}

			if(m_bUnDecorate)
			{
				UnDecorateSymbolName(szName, szNameA, MAX_NAME_LEN, UNDNAME_COMPLETE);
				ansitounicode(szNameA, MAX_NAME_LEN, szNameW, MAX_NAME_LEN);
			}
			else
				ansitounicode(szName, MAX_NAME_LEN, szNameW, MAX_NAME_LEN);

			textformat(szBuffer, 1024, L"(%05d)[0x%08X] %s\n", iOrdinal + expDirectory.Base, iAddr, szNameW);
			text.append(szBuffer);
		}
		m_txbInfo.SetText(text, text.length());
		m_txbInfo.TlSetIndex(0);
	}
	catch(...)
	{

	}
}

void CMainWindow::GetMeta(IMetaHelper * pHelper)
{
	pHelper->help_fun(L"OnTextLineDragRequest", bind(this, &CMainWindow::OnTextLineDragRequest));
	pHelper->help_fun(L"OnTextLineDragDrop", bind(this, &CMainWindow::OnTextLineDragDrop));
	pHelper->help_fun(L"OnTextBoxShowMenu", bind(this, &CMainWindow::OnTextBoxShowMenu));
	pHelper->help_fun(L"OnPaneMouseDown", bind(this, &CMainWindow::OnPaneMouseDown));

	pHelper->help(L"txlFile", &m_txlFile);
	pHelper->help(L"txbInfo", &m_txbInfo);
}

int_x CMainWindow::OnTextLineDragRequest(IControl * pControl, IData * pData, DropResultE * peReault)
{
	if(pData->GetType() == ClipboardDataTypeFile)
		*peReault = DragRequestCopy;
	else
		*peReault = DragRequestNone;
	return 0;
}

int_x CMainWindow::OnTextLineDragDrop(IControl * pControl, IData * pData)
{
	if(pData->GetType() == ClipboardDataTypeFile)
	{
		char_16 szText[512];
		pData->GetFileName16(0, szText, 512);
		SetExeFileName(szText);
	}
	return 0;
}
