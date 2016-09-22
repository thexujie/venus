#include "stdafx.h"
#include "CFolderListView.h"

VENUS_BEG

CFolderListView::CFolderListView():m_bThumbnails(false)
{
	AddCol(L"名称", 200);
	AddCol(L"修改日期", 120);
	AddCol(L"类型", 80);
	AddCol(L"大小", 80, AlignTopRight);
	SetInterestCols<2>({2, 3});
}

CFolderListView::~CFolderListView()
{

}

void CFolderListView::SetPath(const char_16 * szPath)
{
	pathinfo_t pinfo(szPath);
	if(!pinfo.exists)
		return;

	RemoveAllRows();
	CSHFileInfoBuffer * pShellBuffer = GetService<CSHFileInfoBuffer>(OID_CSHFileInfoBuffer);
	vector<pathinfo_t> infos = pinfo.get_paths();
	//infos.sort(small_then<pathinfo_t>);
	arraysortinsert(infos.buffer(), infos.size());
	BegUpdate();
	chbufferw<256> buffer;
	for(int_x cnt = 0, size = infos.size(); cnt < size; ++cnt)
	{
		const pathinfo_t & info = infos[cnt];
		IImage * pImage = pShellBuffer->GetShellIcon(info.path);
		textw name = info.name();
		FolderListViewRowT * pRow = new FolderListViewRowT();
		pRow->path = info.path;
		int_x iRow = AddRow(pRow);

		AddItem(iRow, name, pImage);

		SHFileInfoT shfi = pShellBuffer->GetInfo(info.path);
		date_t dtCreate(shfi.tWrite / 10000);
		buffer.format(L"%d/%02d/%02d %02d:%02d", dtCreate.year, dtCreate.month, dtCreate.day, dtCreate.hour, dtCreate.minute);
		AddItem(iRow, buffer, nullptr);
		AddItem(iRow, shfi.chbType, nullptr);
		if(!info.directory)
			AddItem(iRow, buffer.format(L"%d KB", shfi.size / 1000), nullptr);
		else
			AddItem(iRow, L"", nullptr);
	}
	EndUpdate();
}

void CFolderListView::SetThumbnails(bool bThumbnails)
{
	if(m_bThumbnails != bThumbnails)
	{
		m_bThumbnails = bThumbnails;
		Repaint();
	}
}

void CFolderListView::OnMouseWheelY(pointix point, int_x iWhell)
{
	if(IsKeyDown(KeyCodeControl) && m_eViewMode == ListViewModeIcon)
	{
		SetIconSize(m_iIconSize + iWhell);
	}
	CListView::OnMouseWheelY(point, iWhell);
}

void CFolderListView::QueryDebugMenu(int_x & iBase, IMenu * pMenu) const
{
	CListView::QueryDebugMenu(iBase, pMenu);
	MenuT * pMenuThis = new MenuT();
	pMenu->AddItem(0, L"CFolderView", pMenuThis);
	pMenuThis->AddItem(iBase + 1, L"Thumbnails", m_bThumbnails);
	SafeRelease(pMenuThis);
	iBase += 1;
}

void CFolderListView::OnDebugMenu(int_x iBase, int_x iResult)
{
	iBase -= 1;
	switch(iResult - iBase)
	{
	case 1:
		SetThumbnails(!m_bThumbnails);
		break;
	default:
		CListView::OnDebugMenu(iBase, iResult);
		break;
	}
}

void CFolderListView::_PaintImage(IPaint * pPaint, IImage * pImage, rectix rcImage) const
{
	if(!pPaint || !pImage || rcImage.is_empty())
		return;

	CShellItemImage * psiImage = dynamic_cast<CShellItemImage *>(pImage);
	if(psiImage->Prepare(rcImage.size, (m_bThumbnails && m_eViewMode != ListViewModeDetails)))
	{
		sizeix size = psiImage->GetSize();
		rcImage.x = rcImage.x + (rcImage.w - size.w) / 2;
		if(m_eViewMode == ListViewModeIcon)
			rcImage.y = rcImage.y + rcImage.h - size.h;
		else
			rcImage.y = rcImage.y + (rcImage.h - size.h) / 2;
		pPaint->DrawImage(pImage, rcImage.x, rcImage.y);
	}
}

VENUS_END
