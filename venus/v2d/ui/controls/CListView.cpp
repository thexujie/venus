#include "stdafx.h"
#include "CListView.h"
#include "CScrollBar.h"

VENUS_BEG

CListView::CListView() :
	m_bDisableSelectNullRow(false), m_iUpdating(0), m_iColMain(0), m_eViewMode(ListViewModeDetails), m_iIconSize(ICON_DEF_SIZE_DETAILS)
{
	m_iRowSpace = DEF_SPACE;
	m_iColSpace = DEF_SPACE;
	m_bShowHeader = true;
	m_iRowHoving = -1;
	m_iRowSelected = -1;
	m_iColHoving = -1;
	m_iColSelected = -1;
	m_bShowFrameX = false;
	m_bShowFrameY = false;
	m_iHeaderHeight = m_font.size + m_iRowSpace * 2;
	m_iColSizing = -1;
	m_iSizingParam = 0;

	m_bRepaintFocusedChanged = true;
	m_bRepaintScrollX = true;
	m_bRepaintScrollY = true;
	m_bFocusAble = true;

	m_eBorderType = BorderType3;
	m_margin.set(3, 3, 3, 3);

	CScrollBar * pScrollBarX = new CScrollBar();
	CScrollBar * pScrollBarY = new CScrollBar();
	pScrollBarX->SetDynamic(true);
	pScrollBarY->SetDynamic(true);
	AddControl(pScrollBarX);
	AddControl(pScrollBarY);
	CControl::SetScrollBarX(pScrollBarX);
	CControl::SetScrollBarY(pScrollBarY);
}

CListView::~CListView()
{
	for(int_x cnt = 0, size = m_rows.size(); cnt < size; ++cnt)
		SafeRelease(m_rows[cnt]);
	m_rows.clear();
	m_cols.clear();
}

const oid_t & CListView::GetOid() const
{
	return OID_ListView;
}

void CListView::GetMeta(IMetaHelper * pHelper)
{
	CControl::GetMeta(pHelper);
	pHelper->help_evt(L"HeaderMouseDownL", HeaderMouseDownL);
	pHelper->help_evt(L"RowSelecting", RowSelecting);
	pHelper->help_evt(L"RowSelected", RowSelected);
	pHelper->help_evt(L"ColSelecting", ColSelecting);
	pHelper->help_evt(L"ColSelected", ColSelected);
	pHelper->help_evt(L"ItemSelecting", ItemSelecting);
	pHelper->help_evt(L"ItemSelected", ItemSelected);
}

int_x CListView::AddCol(const char_16 * szText, int_x iWidth, AlignE eTextAlign, int_x iUserData)
{
	ListViewColT & col = m_cols.add();
	col.Col = m_cols.size() - 1;
	col.Text = szText;
	if(iWidth > 0)
		col.Width = iWidth;
	else
		col.Width = Get2DDevice()->GetTextSize(col.Text, col.Text.length(), m_font).w + m_iColSpace * 2;
	col.TextAlign = eTextAlign;
	col.UserData = iUserData;
	UpdateScroll();
	Repaint();
	return col.Col;
}

int_x CListView::AddRow(int_x iUserData)
{
	ListViewRowT * pRow = new ListViewRowT();
	pRow->UserData = iUserData;
	return AddRow(pRow);
}

int_x CListView::AddRow(ListViewRowT * pRow)
{
	if(!pRow)
		return -1;

	int_x iRow = m_rows.size();
	pRow->Row = iRow;
	pRow->Height = m_font.size + m_iRowSpace * 2;
	m_rows.add(pRow);
	return iRow;
}

int_x CListView::AddItem(int_x iRow, const char_16 * szText, IImage * pImage, int_x iUserData)
{
	ListViewItemT * pItem = new ListViewItemT();
	pItem->Text = szText;
	pItem->Image = pImage;
	pItem->UserData = iUserData;
	pItem->ContentSize = Get2DDevice()->GetTextSize(szText, -1, m_font);
	SafeAddRef(pImage);
	return AddItem(iRow, pItem);
}

int_x CListView::AddItem(int_x iRow, ListViewItemT * pItem)
{
	if(!pItem)
		return -1;

	ListViewRowT & row = *m_rows[iRow];
	int_x iCol = row.Items.size();
	pItem->Row = iRow;
	pItem->Col = iCol;
	row.Items.add(pItem);
	Update();
	return iCol;
}

void CListView::BegUpdate()
{
	if(m_iUpdating <= 0)
		m_iUpdating = 1;
}

void CListView::EndUpdate()
{
	if(m_iUpdating > 1)
	{
		UpdateScroll();
		Repaint();
		m_iUpdating = 0;
	}
}

void CListView::Update()
{
	if(m_iUpdating <= 0)
	{
		UpdateScroll();
		Repaint();
	}
	else
		++m_iUpdating;
}

const ListViewColT & CListView::GetCol(int_x iIndex) const
{
	return m_cols[iIndex];
}

ListViewItemT * CListView::GetItem(int_x iRow, int_x iCol)
{
	return m_rows[iRow]->Items[iCol];
}

const ListViewItemT * CListView::GetItem(int_x iRow, int_x iCol) const
{
	return m_rows[iRow]->Items[iCol];
}

int_x CListView::GetColCount() const
{
	return m_cols.size();
}

int_x CListView::GetRowCount() const
{
	return m_rows.size();
}

void CListView::RemoveAllCols()
{
	m_cols.clear();
	Update();
}

void CListView::RemoveAllRows()
{
	if(m_rows.is_valid())
	{
		SetHovingRow(-1);
		SetSelectedRow(-1);

		for(int_x cnt = 0, size = m_rows.size(); cnt < size; ++cnt)
			SafeRelease(m_rows[cnt]);
		m_rows.clear();
		Repaint();
	}
}

void CListView::RemoveAll()
{
	RemoveAllRows();
	RemoveAllCols();
	Repaint();
}

int_x CListView::GetRowHeight(int_x iRow) const
{
	return m_font.size + m_iRowSpace * 2;
}

void CListView::SetColSpace(int_x iSpace)
{
	if(iSpace < 0)
		iSpace = 0;
	if(m_iColSpace != iSpace)
	{
		m_iColSpace = iSpace;
		for(int_x cnt = 0, size = m_cols.size(); cnt < size; ++cnt)
		{
			ListViewColT & col = m_cols[cnt];
		}
		UpdateScroll();
		Repaint();
	}
}

int_x CListView::GetColSpace() const
{
	return m_iColSpace;
}

void CListView::SetRowSpace(int_x iSpace)
{
	if(iSpace < 0)
		iSpace = 0;
	if(iSpace != m_iRowSpace)
	{
		m_iRowSpace = iSpace;
		for(int_x cnt = 0, size = m_rows.size(); cnt != size; ++cnt)
		{
			ListViewRowT & row = *m_rows[cnt];
			row.Height = m_font.size + m_iRowSpace * 2;
		}
		UpdateScroll();
		Repaint();
	}
}

int_x CListView::GetRowSpace() const
{
	return m_iRowSpace;
}

void CListView::SetColWidth(int_x iCol, int_x iWidth)
{
	int_x iSpace = m_iColSpace * 2;
	if(iWidth < iSpace)
		iWidth = iSpace;

	ListViewColT & col = m_cols[iCol];
	if(col.Width != iWidth)
	{
		col.Width = iWidth;
		UpdateScroll();
		Repaint();
	}
}

void CListView::SetRowHeight(int_x iRow, int_x iHeight)
{
	int_x iSpace = m_iRowSpace * 2;
	if(iHeight < iSpace)
		iHeight = iSpace;

	ListViewRowT & row = *m_rows[iRow];
	if(row.Height != iHeight)
	{
		row.Height = iHeight;
		UpdateScroll();
		Repaint();
	}
}

int_x CListView::GetColWidth(int_x iCol) const
{
	return m_cols[iCol].Width;
}

int_x CListView::GetColX(int_x iIndex) const
{
	int_x iLeft = 0;
	for(int_x cnt = 0; cnt < iIndex; ++cnt)
		iLeft += m_cols[cnt].Width;
	return iLeft;
}

int_x CListView::GetRowY(int_x iRow) const
{
	if(iRow >= m_rows.size())
		throw exp_out_of_bound();

	if(m_eViewMode == ListViewModeDetails)
		return iRow * (m_font.size + m_iRowSpace * 2);
	else
	{
		intx2 i2Num = GetTileNum();
		sizeix siTile = GetTileSize();
		int_x iRowIndex = iRow / i2Num.col;
		int_x iColIndex = iRow % i2Num.col;
		return iRowIndex * siTile.h;
	}
}

rectix CListView::GetRowRect(int_x iRow) const
{
	if(iRow < 0 || m_rows.size() <= iRow)
		throw exp_out_of_bound();

	if(m_eViewMode == ListViewModeDetails)
		return rectix(0, iRow * (m_font.size + m_iRowSpace * 2), GetAllWidth(), GetRowHeight(iRow));
	else
	{
		rectix rect;
		intx2 i2Num = GetTileNum();
		sizeix siTile = GetTileSize();
		int_x iRowIndex = iRow / i2Num.col;
		int_x iColIndex = iRow % i2Num.col;
		rect.x = iColIndex * siTile.w;
		rect.y = iRowIndex *  +siTile.h;
		rect.size = siTile;
		return rect;
	}
}

rectix CListView::GetTileRect(int_x iRow, int_x iCol) const
{
	return rectix(GetColX(iCol), GetRowY(iRow), GetColWidth(iCol), GetRowHeight(iRow));
}

intx2 CListView::GetTileNum() const
{
	rectix rcClient = GetClient();
	sizeix siTile = GetTileSize();

	intx2 i2Num;
	i2Num.cx = rcClient.w / siTile.w;
	if(!i2Num.cx)
		i2Num.cx = 1;
	i2Num.cy = (m_rows.size() + i2Num.cx - 1) / i2Num.cx;
	return i2Num;
}

sizeix CListView::GetTileSize() const
{
	if(m_eViewMode == ListViewModeDetails)
		return sizeix(GetAllWidth(), m_iRowSpace + m_font.size + m_iRowSpace);
	else if(m_eViewMode == ListViewModeTile)
	{
		int_x iInterestCount = m_colsOfInterest.size();
		int_x iStringH = m_font.size * (iInterestCount + 1) + m_iRowSpace * iInterestCount;
		int_x iTileW = m_iColSpace + m_iColSpace * 2 + m_iIconSize + m_iColSpace + m_font.size * 10 + m_iColSpace * 2 + m_iColSpace;
		int_x iTileH = m_iRowSpace + m_iRowSpace * 2 + maxof(m_iIconSize, iStringH) + m_iRowSpace * 2 + m_iRowSpace;
		return sizeix(iTileW, iTileH);
	}
	else
	{
		int_x iTileW = m_iColSpace + m_iColSpace * 2 + m_iIconSize + m_iColSpace * 2 + m_iColSpace;
		int_x iTileH = m_iRowSpace + m_iRowSpace * 2 + m_iIconSize + m_iColSpace * 2 + m_iRowSpace;
		if(m_eViewMode != ListViewModeIconOnly)
			iTileH += m_iRowSpace + m_font.size;
		return sizeix(iTileW, iTileH);
	}
}

intx2 CListView::GetRowTile(int_x iRow) const
{
	intx2 i2Tile;
	intx2 i2Num = GetTileNum();
	sizeix siTile = GetTileSize();
	i2Tile.row = iRow / i2Num.col;
	i2Tile.col = iRow % i2Num.col;
	return i2Tile;
}

void CListView::SetRowUserData(int_x iRow, int_x iUserData)
{
	m_rows[iRow]->UserData = iUserData;
}

int_x CListView::GetRowUserData(int_x iRow) const
{
	return m_rows[iRow]->UserData;
}

void CListView::SetItemUserData(int_x iRow, int_x iCol, int_x iUserData)
{
	m_rows[iRow]->Items[iCol]->UserData = iUserData;
}

int_x CListView::GetAllWidth() const
{
	int_x iWidth = 0;
	for(int_x cnt = 0, len = m_cols.size(); cnt < len; ++cnt)
		iWidth += m_cols[cnt].Width;
	return iWidth;
}

int_x CListView::GetAllHeight() const
{
	int_x iHeight = 0;
	for(int_x cnt = 0, len = m_rows.size(); cnt < len; ++cnt)
		iHeight += m_rows[cnt]->Height;
	return iHeight;
}

void CListView::SetMainCol(int_x iCol)
{
	if(iCol < 0)
		return;

	if(m_iColMain != iCol)
	{
		m_iColMain = iCol;
		if(m_eViewMode != ListViewModeDetails)
		{
			UpdateScroll();
			Repaint();
		}
	}
}

int_x CListView::GetMainCol() const
{
	return m_iColMain;
}

void CListView::SetInterestCols(const int_x * pColIds, int_x iCount)
{
	m_colsOfInterest.resize(iCount);
	m_colsOfInterest.assign(0, pColIds, iCount);
}

void CListView::SetHeaderHeight(int_x iHeight)
{
	if(iHeight != m_iHeaderHeight)
	{
		m_iHeaderHeight = iHeight;
		UpdateScroll();
		Repaint();
	}
}

int_x CListView::GetHeaderHeight() const
{
	return m_iHeaderHeight;
}

void CListView::SetViewMode(ListViewModeE eMode, int_x iIconSize)
{
	if(iIconSize <= 0)
	{
		if(eMode == ListViewModeDetails)
			iIconSize = ICON_DEF_SIZE_DETAILS;
		else
			iIconSize = ICON_DEF_SIZE_TITLE;
	}

	if(m_eViewMode != eMode || m_iIconSize != iIconSize)
	{
		m_eViewMode = eMode;
		m_iIconSize = iIconSize;
		UpdateScroll();
		Repaint();
	}
}

ListViewModeE CListView::GetViewMode() const
{
	return m_eViewMode;
}

void CListView::SetIconSize(int_x iIconSize)
{
	if(iIconSize < ICON_MIN_SIZE)
		iIconSize = ICON_MIN_SIZE;
	else if(iIconSize > ICON_MAX_SIZE)
		iIconSize = ICON_MAX_SIZE;
	else {}

	if(m_iIconSize != iIconSize)
	{
		m_iIconSize = iIconSize;
		UpdateScroll();
		Repaint();
	}
}

int_x CListView::GetIconSize() const
{
	return m_iIconSize;
}

void CListView::SetHovingRow(int_x iRow)
{
	if(iRow != m_iRowHoving)
	{
		m_iRowHoving = iRow;
		Repaint();
	}
	if(m_iRowHoving >= 0)
		SetHovingCol(-1);
}

int_x CListView::GetHovingRow() const
{
	return m_iRowHoving;
}

void CListView::SetSelectedRow(int_x iRow)
{
	if(iRow != m_iRowSelected)
	{
		if(!RowSelecting(this, iRow) && !ItemSelecting(this, iRow, m_iColSelected))
		{
			m_iRowSelected = iRow;
			RowSelected(this, iRow);
			ItemSelected(this, iRow, m_iColSelected);
			EnsureRowVisible(iRow);
			Repaint();
		}
	}
}

int_x CListView::GetSelectedRow() const
{
	return m_iRowSelected;
}

ListViewRowT * CListView::GetRowSelected() const
{
	if(m_iRowSelected >= 0)
		return m_rows[m_iRowSelected];
	else
		return nullptr;
}

ListViewColT * CListView::GetColSelected() const
{
	if(m_iColSelected >= 0)
		return (ListViewColT *)&m_cols[m_iColSelected];
	else
		return nullptr;
}

ListViewItemT * CListView::GetItemSelected() const
{
	if(m_iRowSelected >= 0 && m_iColSelected >= 0)
		return m_rows[m_iRowSelected]->Items[m_iColSelected];
	else
		return nullptr;
}

void CListView::SetHovingCol(int_x iCol)
{
	if(iCol != m_iColHoving)
	{
		m_iColHoving = iCol;
		// 标题 hoving，需要重绘
		if(m_iRowHoving == LISTVIEW_HEADER_ROW)
			Repaint();
	}
}

int_x CListView::GetHovingCol() const
{
	return m_iColHoving;
}

void CListView::SetSelectedCol(int_x iCol)
{
	if(iCol != m_iColSelected)
	{
		if(!ColSelecting(this, iCol) && !ItemSelecting(this, m_iRowSelected, iCol))
		{
			m_iColSelected = iCol;
			ColSelected(this, iCol);
			ItemSelected(this, m_iRowSelected, iCol);
			Repaint();
		}
	}
}

int_x CListView::GetSelectedCol() const
{
	return m_iColSelected;
}

void CListView::SetSelectedItem(int_x iRow, int_x iCol)
{
	SetSelectedRow(iRow);
	SetSelectedCol(iCol);
}

void CListView::GetSelectedItem(int_x & iRow, int_x & iCol)
{
	iRow = m_iRowSelected;
	iCol = m_iColSelected;
}

void CListView::SetSizingCol(int_x iCol, int_x iOffset)
{
	if(iCol != m_iColSizing)
	{
		m_iColSizing = iCol;
		m_iSizingParam = iOffset;
		Repaint();
	}
}

int_x CListView::GetSizingCol() const
{
	return m_iColSizing;
}

void CListView::SetCanSelectNullRow(bool bCanSelectNullRow)
{
	m_bDisableSelectNullRow = !bCanSelectNullRow;
}

bool CListView::CanSelectNullRow() const
{
	return !m_bDisableSelectNullRow;
}

void CListView::UpdateScroll()
{
	if(m_eViewMode == ListViewModeDetails)
	{
		int_x iWidth = GetAllWidth() + 1;
		int_x iHeight = GetAllHeight();
		if(m_bShowHeader)
			iHeight += m_iHeaderHeight;
		SetScroll(iWidth, iHeight);
	}
	else
	{
		intx2 i2Tile = GetTileNum();
		sizeix siTile = GetTileSize();
		SetScroll(i2Tile.cx * siTile.w, i2Tile.cy * siTile.h);
	}
}

void CListView::EnsureRowVisible(int_x iRow)
{
	if(iRow < 0 || m_rows.size() <= iRow)
		return;

	if(m_eViewMode == ListViewModeDetails)
	{
		int_x iBase = m_bShowHeader ? m_iHeaderHeight : 0;
		int_x iY = GetRowY(iRow);
		int_x iScrollY = GetScrollPosY();
		int_x iItemH = GetRowHeight(iRow);
		int_x iClientH = GetClient().h;
		if(iY - iScrollY < 0)
			SetScrollPosY(iY);
		else if(iY + iItemH - iScrollY > iClientH - iBase - 1)
			SetScrollPosY(iY + iItemH - iClientH + iBase + 1);
		else {}
	}
	else
	{
		rectix rcClient = GetClient();
		rectix rcRow = GetRowRect(iRow);
		intx2 i2Scroll = GetScrollPos();
		if(rcRow.top - i2Scroll.y < 0)
			SetScrollPosY(rcRow.top);
		else if(rcRow.bottom() - i2Scroll.y > rcClient.h)
			SetScrollPosY(rcRow.bottom() - rcClient.h);
		else {}
	}
}

bool CListView::GetItemAt(int_x iX, int_x iY, int_x & iRow, int_x & iCol, int_x & iColSizing)
{
	iRow = iCol = iColSizing = -1;
	intx2 i2Scroll = GetScrollPos();

	if(m_eViewMode == ListViewModeDetails)
	{
		iX += i2Scroll.x;
		for(int_x cnt = 0, size = m_cols.size(); cnt < size; ++cnt)
		{
			const ListViewColT & col = m_cols[cnt];
			if(iColSizing < 0 && col.Width - m_iColSpace <= iX && iX < col.Width + m_iColSpace)
				iColSizing = cnt;

			if(iX < col.Width)
			{
				iCol = cnt;
				break;
			}
			else
				iX -= col.Width;
		}

		if(m_bShowHeader && iY < m_iHeaderHeight)
		{
			iRow = LISTVIEW_HEADER_ROW;
		}
		else
		{
			if(m_bShowHeader)
				iY -= m_iHeaderHeight;

			iY += i2Scroll.y;

			for(int_x cnt = 0, size = m_rows.size(); cnt < size; ++cnt)
			{
				const ListViewRowT * pRow = m_rows[cnt];
				if(iY < pRow->Height)
				{
					iRow = cnt;
					break;
				}
				else
					iY -= pRow->Height;
			}
		}
		return iRow >= 0 && iCol >= 0;
	}
	else
	{
		iRow = -1;
		iCol = -1;
		iColSizing = -1;

		iX += i2Scroll.x;
		iY += i2Scroll.y;

		intx2 i2Num = GetTileNum();
		sizeix siTile = GetTileSize();
		int_x iRowIndex = iY / siTile.h;
		int_x iColIndex = iX / siTile.w;
		if(iColIndex < i2Num.cx)
		{
			int_x iTile = iRowIndex * i2Num.col + iColIndex;
			if(iTile < m_rows.size())
			{
				rectix rcRow = GetRowRect(iTile);
				rcRow = rcRow.expand(-m_iColSpace, -m_iRowSpace);
				if(rcRow.is_point_in(iX, iY))
					iRow = iTile;
			}
		}
		return iRow >= 0;
	}
}

void CListView::SetShowFrameX(bool bFrameX)
{
	if(bFrameX != m_bShowFrameX)
	{
		m_bShowFrameX = bFrameX;
		if(m_eViewMode == ListViewModeDetails)
			Repaint();
	}
}

void CListView::SetShowFrameY(bool bFrameY)
{
	if(bFrameY != m_bShowFrameY)
	{
		m_bShowFrameY = bFrameY;
		if(m_eViewMode == ListViewModeDetails)
			Repaint();
	}
}

void CListView::SetShowHeader(bool bHeader)
{
	if(bHeader != m_bShowHeader)
	{
		m_bShowHeader = bHeader;
		if(m_eViewMode == ListViewModeDetails)
		{
			if(!bHeader)
			{
				if(m_iRowHoving == LISTVIEW_HEADER_ROW)
					SetHovingRow(-1);
			}
			UpdateScroll();
			Repaint();
		}
	}
}

bool CListView::IsShowHeader() const
{
	return m_bShowHeader;
}

bool CListView::IsShowFrameX() const
{
	return m_bShowFrameX;
}

bool CListView::IsShowFrameY() const
{
	return m_bShowFrameY;
}

void CListView::Sort(bool(*pfnCmp)(const ListViewRowT * pLeftItem, const ListViewRowT * pRightItem))
{
	int_x iRowSelected = -1;
	m_rows.sort(pfnCmp);
	for(int_x cnt = 0, size = m_rows.size(); cnt < size; ++cnt)
	{
		if(m_rows[cnt]->Row == m_iRowSelected)
			iRowSelected = cnt;

		m_rows[cnt]->Row = cnt;
	}
	m_iRowSelected = iRowSelected;
	Repaint();
}

void CListView::Sort(function<int_x(const ListViewRowT *, const ListViewRowT *)> fun)
{
	int_x iRowSelected = -1;
	m_rows.sort(fun, ListViewItemSwap);
	for(int_x cnt = 0, size = m_rows.size(); cnt < size; ++cnt)
	{
		if(m_rows[cnt]->Row == m_iRowSelected)
			iRowSelected = cnt;

		m_rows[cnt]->Row = cnt;
	}
	m_iRowSelected = iRowSelected;
	Repaint();
}

void CListView::RequestCursor(pointix point, CursorE & eCursor) const
{
	if(m_iRowHoving == LISTVIEW_HEADER_ROW && m_iColSizing >= 0)
		eCursor = CursorSizeLR;
	else
		return CControl::RequestCursor(point, eCursor);
}

void CListView::OnPaint(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	switch(m_eViewMode)
	{
	case ListViewModeDetails:
		OnPaintDetails(pPaint, rcClip, pTheme);
		break;
	case ListViewModeTile:
		OnPaintTile(pPaint, rcClip, pTheme);
		break;
	case ListViewModeIcon:
	case ListViewModeIconOnly:
		OnPaintIcon(pPaint, rcClip, pTheme);
		break;
	default:
		break;
	}
}

void CListView::OnPaintDetails(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	/**
	* 先绘制 rows(items)，再绘制 frame，最后绘制 header。
	*/
	rectix rcItems(pointix(), GetClient().size);
	int_x iBaseY = 0;
	if(m_bShowHeader)
	{
		rcItems.y = m_iHeaderHeight;
		rcItems.h -= m_iHeaderHeight;
	}
	if(rcItems.is_empty())
		return;

	pPaint->PushClip(rcItems);
	intx2 i2Scroll = GetScrollPos();
	int_x iX = -i2Scroll.x, iY = rcItems.y - i2Scroll.y, iHeight = 0;

	for(int_x cnt = 0, size = m_rows.size(); cnt < size; ++cnt, iY += iHeight)
	{
		const ListViewRowT * pRow = m_rows[cnt];
		iHeight = pRow->Height;
		if(iY + iHeight < 0)
			continue;
		else if(iY >= rcItems.bottom())
			break;
		else
			PaintRow(pPaint, pRow, iX, iY, pTheme);
	}
	pPaint->PopClip();

	PaintFrame(pPaint, pTheme);
	PaintHeader(pPaint, pTheme);
}

void CListView::OnPaintTile(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	rectix rcClient = GetClient();
	intx2 i2Scroll = GetScrollPos();
	sizeix siTile = GetTileSize();
	intx2 i2Num = GetTileNum();

	sizeix siItem(siTile.w - m_iColSpace * 2, siTile.h - m_iRowSpace * 2);
	int_x iInterestCount = m_colsOfInterest.size();
	int_x iStringW = siTile.w - (m_iColSpace + m_iColSpace * 2 + m_iIconSize + m_iColSpace + /*iStringW +*/ m_iColSpace * 2 + m_iColSpace);
	int_x iStringH = m_font.size * (iInterestCount + 1) + m_iRowSpace * iInterestCount;
	int_x iContentH = siItem.h - m_iRowSpace * 2 - m_iRowSpace * 2;
	textformat_t format(m_font, AlignLeft | AlignTop, TextWrapNone, TextTrimmingCharEllipsis);
	for(int_x cnt = 0, size = m_rows.size(); cnt < size; ++cnt)
	{
		int_x iRowIndex = cnt / i2Num.cx;
		int_x iColIndex = cnt % i2Num.cx;

		int_x iItemX = -i2Scroll.x + iColIndex * siTile.w + m_iColSpace;
		int_x iItemY = -i2Scroll.y + iRowIndex * siTile.h + m_iRowSpace;

		if(iItemY + siItem.h <= 0)
			continue;
		else if(iItemY > rcClient.h)
			break;

		const ListViewRowT * pRow = m_rows[cnt];
		const ListViewItemT * pItem = pRow->Items[m_iColMain];
		if(m_iRowHoving == cnt)
		{
			pPaint->FillRect(iItemX, iItemY, siItem.w, siItem.h, SysColorBackHoving);
			if(m_iRowSelected == cnt)
				pPaint->DrawRect(iItemX, iItemY, siItem.w - 1, siItem.h - 1, SysColorFrameSelected);
		}
		else if(m_iRowSelected == cnt)
			pPaint->FillRect(iItemX, iItemY, siItem.w, siItem.h, IsFocused() ? SysColorBackSelected : SysColorBackDisable);
		else {}

		if(pItem->Image)
		{
			rectix rcImage(iItemX + m_iColSpace * 2, iItemY + m_iRowSpace * 2 + (iContentH - m_iIconSize) / 2, m_iIconSize, m_iIconSize);
			_PaintImage(pPaint, pItem->Image, rcImage);
			//pPaint->DrawRect(rcImage, Colors::Red);
		}

		rectix rcString(iItemX + m_iColSpace * 2 + m_iIconSize + m_iColSpace, iItemY + m_iRowSpace * 2 + (iContentH - iStringH) / 2, iStringW, m_font.size);
		pPaint->DrawString(pItem->Text, format, rcString, m_uiForeColor);
		for(int_x cnt = 0, size = m_colsOfInterest.size(); cnt < size; ++cnt)
		{
			rcString.y += m_font.size + m_iRowSpace;
			int_x iInterest = m_colsOfInterest[cnt];
			const ListViewItemT * pSub = pRow->Items[iInterest];
			pPaint->DrawString(pSub->Text, format, rcString, m_uiForeColor);
		}
	}
}

void CListView::OnPaintIcon(IPaint * pPaint, const rectix & rcClip, const IUITheme * pTheme) const
{
	rectix rcClient = GetClient();
	intx2 i2Scroll = GetScrollPos();
	sizeix siTile = GetTileSize();
	intx2 i2Num = GetTileNum();

	sizeix siItem(siTile.w - m_iColSpace * 2, siTile.h - m_iRowSpace * 2);
	textformat_t format0(m_font, AlignLeft | AlignTop, TextWrapNone, TextTrimmingCharEllipsis);
	textformat_t format1(m_font, AlignCenterX | AlignTop, TextWrapNone, TextTrimmingNone);

	for(int_x cnt = 0, size = m_rows.size(); cnt < size; ++cnt)
	{
		int_x iRow = cnt / i2Num.cx;
		int_x iCol = cnt % i2Num.cx;

		int_x iItemX = -i2Scroll.x + iCol * siTile.w + m_iColSpace;
		int_x iItemY = -i2Scroll.y + iRow * siTile.h + m_iRowSpace;

		if(iItemY + siItem.h <= 0)
			continue;
		else if(iItemY > rcClient.h)
			break;

		const ListViewRowT * pRow = m_rows[cnt];
		const ListViewItemT * pItem = pRow->Items[m_iColMain];
		if(m_iRowHoving == cnt)
		{
			pPaint->FillRect(iItemX, iItemY, siItem.w, siItem.h, SysColorBackHoving);
			if(m_iRowSelected == cnt)
				pPaint->DrawRect(iItemX, iItemY, siItem.w - 1, siItem.h - 1, SysColorFrameSelected);
		}
		else if(m_iRowSelected == cnt)
			pPaint->FillRect(iItemX, iItemY, siItem.w, siItem.h, m_bFocused ? SysColorBackSelected : SysColorBackDisable);
		else {}

		rectix rcImage(iItemX + m_iColSpace * 2, iItemY + m_iRowSpace * 2, m_iIconSize, m_iIconSize);
		_PaintImage(pPaint, pItem->Image, rcImage);

		if(m_eViewMode == ListViewModeIcon)
		{
			rectix rcString(iItemX + m_iColSpace * 2, rcImage.bottom() + m_iRowSpace, m_iIconSize, m_font.size);
			if(pItem->ContentSize.w > rcString.w)
				pPaint->DrawString(pItem->Text, format0, rcString, m_uiForeColor);
			else
				pPaint->DrawString(pItem->Text, format1, rcString, m_uiForeColor);
		}
	}
}

void CListView::PaintHeader(IPaint * pPaint, const IUITheme * pTheme) const
{
	if(!m_bShowHeader)
		return;

	sizeix szClient = GetClient().size;
	//pPaint->FillRect(0, 0, szClient.w, m_iHeaderHeight, SysColorBarBackNormal);
	pPaint->DrawLine(0, m_iHeaderHeight - 1, szClient.w, m_iHeaderHeight - 1, SysColorLineNormal);
	int_x iX = -GetScrollPosX(), iWidth = 0, iWidth2 = 0;

	for(int_x cnt = 0, size = m_cols.size(); cnt < size; ++cnt)
	{
		const ListViewColT & col = m_cols[cnt];
		iWidth = col.Width;
		iWidth2 = iWidth - 1;
		if(iX + iWidth < 0)
		{
			iX += iWidth;
			continue;
		}
		else if(iX >= szClient.w)
			break;
		else
		{
			if(m_iRowHoving == LISTVIEW_HEADER_ROW)
			{
				if(cnt == m_iColSizing)
				{
					pPaint->FillRect(rectix(iX + 1, 0, iWidth2, m_iHeaderHeight), SysColorBackHoving);
					pPaint->DrawLine(iX + 1, m_iHeaderHeight - 1,
						iX + iWidth2, m_iHeaderHeight - 1, SysColorLineHighlight);
				}
				else if(m_iColSizing < 0 && cnt == m_iColHoving)
				{
					if(IsMouseDownL())
					{
						pPaint->FillRect(rectix(iX + 1, 0, iWidth2, m_iHeaderHeight - 1), SysColorBackPressed);
						pPaint->DrawLine(iX + 1, m_iHeaderHeight - 1,
							iX + iWidth2, m_iHeaderHeight - 1, SysColorLineHighlight);
					}
					else
					{
						pPaint->FillRect(rectix(iX + 1, 0, iWidth2, m_iHeaderHeight), SysColorBackHoving);
						pPaint->DrawLine(iX + 1, m_iHeaderHeight - 1,
							iX + iWidth2, m_iHeaderHeight - 1, SysColorLineHighlight);
					}
				}
				else
				{
				}
			}

			pPaint->DrawLine(iX + iWidth, 2, iX + iWidth, m_iHeaderHeight - 2 - HEADER_H_EX, SysColorFrameNormal);
			if(iWidth > m_iColSpace * 2)
			{
				textformat_t format(m_font, col.TextAlign, TextWrapNone, TextTrimmingCharEllipsis);
				pPaint->DrawString(col.Text, format,
					rectix(iX + m_iColSpace, m_iRowSpace, iWidth - m_iColSpace * 2, m_font.size),
					IsAviliable() ? m_uiForeColor : SysColorForeDisable);
			}
			iX += iWidth;
		}
	}
}

void CListView::PaintRow(IPaint * pPaint, const ListViewRowT * pRow, int_x iX, int_x iY, const IUITheme * pTheme) const
{
	const ListViewRowT & row = *pRow;
	int_x iScrollX = GetScrollPosX(), iScrollY = GetScrollPosY();
	int_x iRowH = row.Height;
	int_x iRowW = GetAllWidth();

	if(row.Row == m_iRowHoving)
	{
		pPaint->FillRect(iX, iY, iRowW, iRowH, SysColorBackHoving);
		if(row.Row == m_iRowSelected)
			pPaint->DrawRect(iX, iY, iRowW - 1, iRowH - 1, SysColorFrameHoving);
	}
	else if(row.Row == m_iRowSelected)
		pPaint->FillRect(iX, iY, iRowW, iRowH, IsFocused() ? SysColorBackSelected : SysColorBackDisable);
	else {}

	uint_32 clrText = m_uiForeColor;
	if(!IsAviliable())
		clrText = SysColorForeDisable;

	int_x iWidth = 0;
	textformat_t format(m_font, AlignLeftTop, TextWrapNone, TextTrimmingCharEllipsis);
	for(int_x cnt = 0, len = row.Items.size(); cnt < len; ++cnt, iX += iWidth)
	{
		const ListViewItemT * pItem = row.Items[cnt];
		const ListViewColT & col = m_cols[cnt];
		iWidth = col.Width;

		if(col.Width <= m_iColSpace * 2)
			continue;

		format.align = col.TextAlign;
		if(pItem->Image)
		{
			rectix rcImage(iX + m_iColSpace, iY + m_iRowSpace, m_iIconSize, m_iIconSize);
			_PaintImage(pPaint, pItem->Image, rcImage);
			rectix rcString(iX + m_iColSpace + m_iIconSize + m_iColSpace, iY + m_iRowSpace,
				col.Width - m_iColSpace * 2 - m_iColSpace - m_iIconSize, iRowH - m_iRowSpace * 2);
			pPaint->DrawString(pItem->Text, format, rcString, clrText);
		}
		else
		{
			pPaint->DrawString(pItem->Text, format,
				rectix(iX + m_iColSpace, iY + m_iRowSpace, col.Width - m_iColSpace * 2, iRowH - m_iRowSpace * 2),
				clrText);
		}
	}
}

void CListView::PaintFrame(IPaint * pPaint, const IUITheme * pTheme) const
{
	if(!m_bShowFrameX && !m_bShowFrameY)
		return;

	sizeix szClient = GetClient().size;
	uint_32 clrLine = IsAviliable() ? SysColorLineNormal : SysColorLineDisable;

	int_x iScrollX = GetScrollPosX(), iScrollY = GetScrollPosY();
	int_x iHeaderH = 0;
	if(m_bShowHeader)
		iHeaderH = m_iHeaderHeight;

	if(m_bShowFrameX)
	{
		int_x iY = -iScrollY + iHeaderH;
		int_x iWidth = GetAllWidth() - iScrollX;
		if(iWidth > szClient.w)
			iWidth = szClient.w;

		for(int_x cnt = 0, size = m_rows.size(); cnt < size; ++cnt)
		{
			const ListViewRowT * pRow = m_rows[cnt];
			iY += pRow->Height;
			if(iY <= iHeaderH)
				continue;
			else if(iY >= szClient.h)
				break;
			else
				pPaint->DrawLine(0, iY, iWidth, iY, clrLine);
		}
	}

	// 竖线。
	if(m_bShowFrameY)
	{
		int_x iX = -iScrollX, iBottom = 0;

		for(int_x cnt = 0, size = m_cols.size(); cnt < size; ++cnt)
		{
			const ListViewColT & col = m_cols[cnt];
			iX += col.Width;
			if(iX < 0)
				continue;
			else if(iX > szClient.w)
				break;
			else
			{
				if(cnt == m_iColSizing)
					pPaint->DrawLine(iX, iHeaderH, iX, szClient.h, clrLine, 2.0f);
				else
					pPaint->DrawLine(iX, iHeaderH, iX, szClient.h, clrLine);
			}
		}
	}
}

void CListView::_PaintImage(IPaint * pPaint, IImage * pImage, rectix rcImage) const
{
	if(!pPaint || !pImage || rcImage.is_empty())
		return;

	pImage->Prepare(rcImage.size);
	pPaint->DrawImage(pImage, rcImage);
}

void CListView::OnVisualSizeChanged()
{
	CControl::OnVisualSizeChanged();
	UpdateScroll();
}

void CListView::OnMouseDownL(pointix point)
{
	// 标题栏内
	if(m_iRowHoving == LISTVIEW_HEADER_ROW)
	{
		// 调整大小
		if(m_iColSizing >= 0)
			Capture(true);
	}
	else
	{
		if(m_iRowHoving >= 0 || !m_bDisableSelectNullRow)
		{
			SetSelectedRow(m_iRowHoving);
			SetSelectedCol(m_iColHoving);
		}
		else {}
		SetSizingCol(-1, 0);
	}
	CControl::OnMouseDownL(point);
}

void CListView::OnMouseDownR(pointix point)
{
	int_x iItemX = point.x + GetScrollPosX(), iItemY = point.y;
	if(m_iColSizing < 0 && (!m_bShowHeader || iItemY >= m_iHeaderHeight))
	{
		iItemY += GetScrollPosY();
		SetSelectedRow(m_iRowHoving);
	}
	CControl::OnMouseDownR(point);
}

void CListView::OnMouseUpL(pointix point)
{
	Capture(false);
	CControl::OnMouseUpL(point);
}

void CListView::OnMouseDBClickL(pointix point)
{
	CControl::OnMouseDBClickL(point);
}

void CListView::OnMouseMove(pointix point)
{
	CControl::OnMouseMove(point);
	// 调整列宽
	if(IsMouseDownL() && m_iColSizing >= 0)
	{
		int_x iLeft = GetColX(m_iColSizing);
		int_x iWidth = point.x + GetScrollPosX() - iLeft + m_iSizingParam;
		SetColWidth(m_iColSizing, iWidth);
	}
	else if(!IsAnyButtonDown())
	{
		int_x iRow = -1, iCol = -1, iColSizing = -1;
		GetItemAt(point.x, point.y, iRow, iCol, iColSizing);
		SetHovingRow(iRow);
		SetHovingCol(iCol);
		if(iColSizing >= 0 && iRow == LISTVIEW_HEADER_ROW)
		{
			int_x iOffset = GetColX(iColSizing + 1) - GetColX(iColSizing + 1);
			SetSizingCol(iColSizing, iOffset);
		}
		else
			SetSizingCol(-1, 0);
	}
}

void CListView::OnMouseOut(pointix point)
{
	SetHovingCol(-1);
	SetHovingRow(-1);
	SetSizingCol(-1, 0);
	CControl::OnMouseOut(point);
}

void CListView::OnKeyDown(KeyCodeE eKeyCode)
{
	CControl::OnKeyDown(eKeyCode);
	switch(eKeyCode)
	{
	case KeyCodeUp:
		if(m_iRowSelected > 0)
			SetSelectedRow(m_iRowSelected - 1);
		break;
	case KeyCodeDown:
		if(m_iRowSelected < m_rows.size() - 1)
			SetSelectedRow(m_iRowSelected + 1);
		break;
	case KeyCodeHome:
		if(m_rows.size() > 0)
			SetSelectedRow(0);
		break;
	case KeyCodeEnd:
		if(m_rows.size() > 0)
			SetSelectedRow(m_rows.size() - 1);
		break;
	default:
		break;
	}
}

void CListView::OnScroll(intx2 scroll, intx2 offset)
{
	Repaint();
	CControl::OnScroll(scroll, offset);
}

void CListView::QueryDebugMenu(int_x & iBase, IMenu * pMenu) const
{
	CControl::QueryDebugMenu(iBase, pMenu);
	MenuT * pMenuThis = new MenuT();
	MenuT * pMenuIconSize = new MenuT();
	pMenuThis->AddItem(iBase + 1, L"ShowHeader", m_bShowHeader);
	pMenuThis->AddItem(iBase + 2, L"ShowFrameX", m_bShowFrameX);
	pMenuThis->AddItem(iBase + 3, L"ShowFrameY", m_bShowFrameY);
	pMenuThis->AddItem(iBase + 12, L"SelectNull", !m_bDisableSelectNullRow);
	pMenuThis->AddSplit();
	pMenuThis->AddItem(iBase + 4, L"Details", MenuGetFlags(m_eViewMode == ListViewModeDetails));
	pMenuThis->AddItem(iBase + 5, L"Title", MenuGetFlags(m_eViewMode == ListViewModeTile));
	pMenuThis->AddItem(iBase + 6, L"Icon", MenuGetFlags(m_eViewMode == ListViewModeIcon));
	pMenuThis->AddItem(iBase + 13, L"IconOnly", MenuGetFlags(m_eViewMode == ListViewModeIconOnly));
	pMenuThis->AddSplit();
	pMenuThis->AddItem(0, L"IconSize", pMenuIconSize);
	pMenuIconSize->AddItem(iBase + 7, L"16", MenuGetFlags(m_iIconSize == 16));
	pMenuIconSize->AddItem(iBase + 8, L"32", MenuGetFlags(m_iIconSize == 32));
	pMenuIconSize->AddItem(iBase + 9, L"48", MenuGetFlags(m_iIconSize == 48));
	pMenuIconSize->AddItem(iBase + 10, L"128", MenuGetFlags(m_iIconSize == 128));
	pMenuIconSize->AddItem(iBase + 11, L"256", MenuGetFlags(m_iIconSize == 256));

	pMenu->AddSplit();
	pMenu->AddItem(0, L"ListView", pMenuThis);
	SafeRelease(pMenuThis);
	SafeRelease(pMenuIconSize);
	iBase += 20;
}

void CListView::OnDebugMenu(int_x iBase, int_x iResult)
{
	iBase -= 20;
	switch(iResult - iBase)
	{
	case 1:
		SetShowHeader(!m_bShowHeader);
		break;
	case 2:
		SetShowFrameX(!m_bShowFrameX);
		break;
	case 3:
		SetShowFrameY(!m_bShowFrameY);
		break;
	case 4:
		SetViewMode(ListViewModeDetails);
		break;
	case 5:
		SetViewMode(ListViewModeTile);
		break;
	case 6:
		SetViewMode(ListViewModeIcon);
		break;
	case 13:
		SetViewMode(ListViewModeIconOnly);
		break;
	case 7: SetIconSize(16); break;
	case 8: SetIconSize(32); break;
	case 9: SetIconSize(48); break;
	case 10: SetIconSize(128); break;
	case 11: SetIconSize(256); break;
	case 12:
		SetCanSelectNullRow(m_bDisableSelectNullRow);
		break;
	default:
		CControl::OnDebugMenu(iBase, iResult);
		break;
	}
}

void CListView::ListViewItemSwap(ListViewRowT *& pLeft, ListViewRowT *& pRight)
{
	ListViewRowT * pTemp = pLeft;
	pLeft = pRight;
	pRight = pTemp;
}

VENUS_END

