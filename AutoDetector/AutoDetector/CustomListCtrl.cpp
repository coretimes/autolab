// CustomListCtrl.cpp : implementation file
//
#include "stdafx.h"
#include "AutoDetector.h"
#include "CustomListCtrl.h"
#include <algorithm>

// CCustomListCtrl

IMPLEMENT_DYNAMIC(CCustomListCtrl, CListCtrl)

CCustomListCtrl::CCustomListCtrl()
{
	m_bCanBeSelected = TRUE;
	m_GridFrameColor = 	RGB(208,208,208);	//  表格边框颜色
	m_SelectedBackColor = RGB(230, 230, 230);
}

CCustomListCtrl::~CCustomListCtrl()
{
}

BEGIN_MESSAGE_MAP(CCustomListCtrl, CListCtrl)
	ON_WM_MEASUREITEM_REFLECT()		//系统向父窗口发送WM_MEASUREITEM消息后，反射响应函数为MeasureItem
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_Float_Button,OnFloatBtnClk)
	ON_NOTIFY(UDN_DELTAPOS, IDC_Float_SpinButton, &CCustomListCtrl::OnDeltaposSpinBtn)
	ON_EN_CHANGE(IDC_Float_EditCtrl, &CCustomListCtrl::OnEnChangeEdit)
	ON_CBN_SELCHANGE(IDC_Float_Combox, &CCustomListCtrl::OnComboxSelChange)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY(HDN_BEGINTRACK, 0, &CCustomListCtrl::OnHdnBegintrack)
	ON_NOTIFY(HDN_BEGINTRACK, 0, &CCustomListCtrl::OnHdnBegintrack)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_Float_DateCtrl, &CCustomListCtrl::OnDtnDatetimechangeFloatDate)
	ON_NOTIFY(HDN_ITEMCLICKA, 0, &CCustomListCtrl::OnHdnItemclick)
	ON_NOTIFY(HDN_ITEMCLICKW, 0, &CCustomListCtrl::OnHdnItemclick)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CCustomListCtrl::PreSubclassWindow()
{
	CRect rectWin;
	GetWindowRect(&rectWin);
	ModifyStyle(0, LVS_OWNERDRAWFIXED | LVS_REPORT|WS_VSCROLL);				// 系统只为具有OWNERDRAWFIXED风格的列表发送WM_MEASUREITEM消息
	ModifyStyleEx(0, LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	SetExtendedStyle(GetExtendedStyle()|LVS_EX_DOUBLEBUFFER);
	m_FocusRow = -1;
	m_FocusCol = -1;

	VERIFY( m_HeaderCtrl.SubclassWindow( GetHeaderCtrl()->GetSafeHwnd()));//实现表头控件的子类化
}

void CCustomListCtrl::AddItem(int nItem,CListItemData * pItemData)
{
	CString strKey;
	strKey.Format(_T("%d"),pItemData->GetKey());
	InsertItem(nItem,strKey);	
	SetItemData(nItem,(DWORD_PTR)pItemData);
}

void CCustomListCtrl::SetItemHeight(const LONG &height)
{
	m_ItemHeight = height; 

	CRect rectWin;
	GetWindowRect(&rectWin);

	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rectWin.Width();
	wp.cy = rectWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;

	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);		// 重画窗口，促使系统发送WM_MEASUREITEM消息
}

void CCustomListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;													// 定义设备上下文
	dc.Attach(lpDrawItemStruct->hDC);						// 附加设备上下文句柄
	int nSavedDC = dc.SaveDC();								// 保存设备上下文

	lpDrawItemStruct->rcItem.bottom = lpDrawItemStruct->rcItem.top + m_ItemHeight;
	dc.SetBkColor(TRANSPARENT);

	// 2.绘制各子项内容
	CListItemData* pData =  reinterpret_cast<CListItemData *>(lpDrawItemStruct->itemData);

	if(pData == NULL)
	{
		ASSERT(0);
		return;
	}

	for (int i = 0; i < pData->GetSubItemCount(); i ++)
	{
		// 取得单元格矩形区域
		CRect rcCell;
		_GetCellRect(lpDrawItemStruct->itemID,i,rcCell);

		// 如果本行是选中状态，绘制选中状态背景色
		if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
			(lpDrawItemStruct->itemState & ODS_SELECTED))
		{
			// 选中状态浅蓝色画刷
			CBrush brSelected(m_SelectedBackColor);				
			CPen penSelected(PS_SOLID, 1, m_SelectedBackColor);	
			dc.SelectObject(brSelected);
			dc.SelectObject(penSelected);

			// 不画单元格的左边框，单元矩形区域缩小
			CRect rect = rcCell;
			rect.left++;
			dc.Rectangle(&rect);
		}
		
		// 根据数据属性，判断本行是否需要高亮显示
		if (pData->IsHighlight() == true)
		{
			// 选中状态浅蓝色画刷
			CBrush brSelected(RGB(230, 230, 230));				
			CPen penSelected(PS_SOLID, 1, RGB(230, 230, 230));	
			dc.SelectObject(brSelected);
			dc.SelectObject(penSelected);

			// 不画单元格的左边框，单元矩形区域缩小
			CRect rect = rcCell;
			rect.left++;
			dc.Rectangle(&rect);
		}

		// 取得合并单元格矩形区域
		CRect rcMergeCell;
		_GetMergeCellRect(lpDrawItemStruct->itemID,i,rcMergeCell);

		// 合并单元格的四角点
		CPoint ptTopLeft = rcMergeCell.TopLeft();
		CPoint ptBottomRight = rcMergeCell.BottomRight();
		CPoint ptTopRight = CPoint(ptBottomRight.x,ptTopLeft.y);
		CPoint ptBottomLeft = CPoint(ptTopLeft.x,ptBottomRight.y);

		// 边框画笔
		CPen pen(PS_SOLID, 1, m_GridFrameColor);
		dc.SelectObject(pen);

		// 如果是第一列，则需画左边框
		if(i == 0)
		{
			dc.MoveTo(ptBottomLeft);
			dc.LineTo(ptTopLeft);
		}
		// 如果是最后一行列，则需画下边框
		if(lpDrawItemStruct->itemID == GetItemCount() - 1)
		{
			dc.MoveTo(ptBottomLeft);
			dc.LineTo(ptBottomRight);
		}

		// 普通单元格只需画上边框和右边框
		dc.MoveTo(ptBottomRight);
		dc.LineTo(ptTopRight);

		// 如果与上单元格合并，则不需要再绘制横线和内容
		if(IsCellMergeWithTop(lpDrawItemStruct->itemID,i))
			continue;
		dc.LineTo(ptTopLeft);
		// 绘制单元格内容
		CString strtemp;
		UINT nforamt = pData->GetCellContex(i,strtemp);
		if(nforamt < 1)
			nforamt = DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER | DT_CENTER;
		if(strtemp != _T(""))
		{
			rcCell.left +=3;
			rcCell.right -=3;
			dc.DrawText(strtemp,&rcCell,nforamt);
		}
	}

	dc.RestoreDC(nSavedDC);
	dc.Detach();
}

void CCustomListCtrl::OnLButtonDown(uint32 nFlags, CPoint point)
{
	// 如果不允许被选中，则不响应单击消息
	if(m_bCanBeSelected == FALSE)
	{
		SetFocus();
		return;
	}

	if(GetFocus() != this)
	{
		// 首先隐藏所有浮动控件
		_HideAllFloatCtrl();
		SetFocus();
		return;
	}

	// 取得击中单元格的索引
	int RowIndex, ColumnIndex;
	if(!_GetCellIndex(point,RowIndex,ColumnIndex))
	{
		// 新焦点行、列索引变更
		m_FocusRow = -1;
		m_FocusCol = -1;
		return;
	}
	// 设置为焦点窗口
	SetFocus();

	// 取得合并单元格的首行和末行
	int FirstRowInMerges,LastRowInMerges;
	_GetMergeCellRows(RowIndex,ColumnIndex,FirstRowInMerges,LastRowInMerges);

	//如果没有按下ctrl键，则需要去掉原被选项目的选中状态
	if(!(nFlags & MK_CONTROL))										
	{
		POSITION pos = GetFirstSelectedItemPosition();
		while (pos)
		{
			int nItem = GetNextSelectedItem(pos);
 			SetItemState(nItem,0, LVIS_SELECTED|LVIS_FOCUSED);
			RedrawItems(nItem,nItem);
		}
	}

	// 定义第一个被选项和最后一个被选项的索引变量
	int FirstSelectedItem,LastSelectedItem;

	// 如果按下了shift键，则判断第一个被选项与最后一个被选项
	if(nFlags & MK_SHIFT)										
	{
		if(m_FocusRow < FirstRowInMerges)
		{
			FirstSelectedItem = m_FocusRow;
			LastSelectedItem = LastRowInMerges;
		}
		else if(m_FocusRow > LastRowInMerges)
		{
			FirstSelectedItem = FirstRowInMerges;
			LastSelectedItem = m_FocusRow;
		}
		else
		{
			FirstSelectedItem = FirstRowInMerges;
			LastSelectedItem = LastRowInMerges;
		}
	}
	// 如果没有按下shift键，则为本单击项
	else
	{
		FirstSelectedItem = FirstRowInMerges;
		LastSelectedItem = LastRowInMerges;
	}


	// 循环设置被选项状态
	for(int i=FirstSelectedItem; i <= LastSelectedItem; i++)
	{
 		SetItemState(i,LVIS_SELECTED, LVIS_SELECTED);
	}

	// 设置新焦点状态
	SetItemState(RowIndex,LVIS_FOCUSED,LVIS_FOCUSED);
	SetSelectionMark(RowIndex);

	// 重画所有被选项
	RedrawItems(FirstSelectedItem,LastSelectedItem);

	// 新焦点行、列索引变更
	m_FocusRow = RowIndex;
	m_FocusCol = ColumnIndex;

	// 如果按下了Control键或Shift键，则隐藏所有浮动控件
	if((nFlags & MK_CONTROL) || (nFlags & MK_SHIFT))
		_HideAllFloatCtrl();
	else
		_ControlFloatShowState(m_FocusRow,m_FocusCol);

}

BOOL CCustomListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// 滚动滑轮时，要删除所有浮动控件	
	_HideAllFloatCtrl();
	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CCustomListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// 如果单击的是窗口滚动条（pScrollBar==NULL时），则隐藏所有控件
	if(pScrollBar == NULL)
		_HideAllFloatCtrl();
	RedrawAllItems();
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CCustomListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// 如果单击的是窗口滚动条（pScrollBar==NULL时），则隐藏所有控件
	if(pScrollBar == NULL)
		_HideAllFloatCtrl();

	if(nSBCode == SB_THUMBPOSITION)
	{
		// 取得能显示的第一行行号
		UINT idxTop = GetTopIndex();
		// 如果当前行比能显示的第一行大于10，则开始向下滚动,多滚动10行
		if(nPos - idxTop > 10)
			Scroll(CSize(0,(nPos - idxTop + 8) * m_ItemHeight));
		// 如果当前行比能显示的第一行小，则开始向上滚动，多滚动10行
		if(nPos < idxTop )
			Scroll(CSize(0,(nPos - idxTop - 8) * m_ItemHeight));
	}
	RedrawAllItems();
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CCustomListCtrl::OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	_HideAllFloatCtrl();
// 	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
// 	*pResult = 0;
}

void CCustomListCtrl::OnHdnItemclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 标头单击事件
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	if (GetItemCount() < 1)
		return;

	_HideAllFloatCtrl();
	// 任意取一条数据，去改变排序模式静态变量
	CListItemData *pData = reinterpret_cast<CListItemData*>(GetItemData(0));
	if(pData->ChangeSortMode(phdr->iItem))
	{
		Reorder();
	}
}
BOOL CCustomListCtrl::DeleteAllItemsAndDatas()
{
	_HideAllFloatCtrl();								// 隐藏所有浮动控件

	int nItemCount = GetItemCount();
	for (int i=0; i < nItemCount; i++)
	{
		CListItemData *pData = reinterpret_cast<CListItemData*>(GetItemData(i));
		delete pData;
	}
	CListCtrl::DeleteAllItems();
	return TRUE;
}

BOOL CCustomListCtrl::DeleteMultItems(vector<int> &vctDeleteIndexes)
{
	// 倒着删除行
	sort(vctDeleteIndexes.begin(),vctDeleteIndexes.end());
	for(int i = (int)vctDeleteIndexes.size() - 1; i >= 0; i--)
	{
		CListItemData *pData = reinterpret_cast<CListItemData*>(GetItemData(vctDeleteIndexes[i]));
		delete pData;
		DeleteItem(vctDeleteIndexes[i]);
	}
	return TRUE;
}

void CCustomListCtrl::Reorder()
{
	// 取得所有数据指针保存于vector中
	vector<CListItemData *> vctDataList;
	for(int i=0; i<GetItemCount(); i++)
	{
		vctDataList.push_back( reinterpret_cast<CListItemData *>(GetItemData(i)));
	}

	// 把数据进行排序
	vector<CListItemData*>::iterator first =  vctDataList.begin();
	vector<CListItemData*>::iterator last =  vctDataList.end();
	sort(first,last,_SortFunc);

	SetRedraw(FALSE);
	_HideAllFloatCtrl();
	DeleteAllItems();
	for(int i=0; i < (int)vctDataList.size(); i++)
	{
		AddItem(i,vctDataList[i]);
	}
	SetRedraw(TRUE);
}

void CCustomListCtrl::_HideAllFloatCtrl()
{		
	// 浮动编辑控件指针数组
	for(int i = 0; i < (int)m_vctEditCtrl.size(); i++)
	{
		m_vctEditCtrl[i].pCtrlEdit->DestroyWindow();
		delete m_vctEditCtrl[i].pCtrlEdit;
	}
	m_vctEditCtrl.clear();

	// 浮动组合控件指针数组
	for(int i = 0; i < (int)m_vctCombox.size(); i++)
	{
		m_vctCombox[i].pCtrlComboBox->DestroyWindow();
		delete m_vctCombox[i].pCtrlComboBox;
	}
	m_vctCombox.clear();

	// 浮动组合控件指针数组
	for(int i = 0; i < (int)m_vctButton.size(); i++)
	{
		m_vctButton[i].pButton->DestroyWindow();
		delete m_vctButton[i].pButton;
	}
	m_vctButton.clear();

	// 浮动微调按钮控件指针数组
	for(int i = 0; i < (int)m_vctSpinBtn.size(); i++)
	{
		m_vctSpinBtn[i].pSpinButton->DestroyWindow();
		delete m_vctSpinBtn[i].pSpinButton;
	}
	m_vctSpinBtn.clear();

	// 时间日期控件指针数组
	for(int i = 0; i < (int)m_vctDateCtrl.size(); i++)
	{
		m_vctDateCtrl[i].pCtrlDate->DestroyWindow();
		delete m_vctDateCtrl[i].pCtrlDate;
	}
	m_vctDateCtrl.clear();
}

bool CCustomListCtrl::_GetCellIndex(const CPoint& pt, int& row, int& col)
{
	// 判断击中行的索引
	row	 = HitTest(pt,NULL);
	if(row < 0)
		return false;

	// 判断击中列的索引
	CListItemData * pData =reinterpret_cast<CListItemData *> (GetItemData(row));
	bool flgIsHit = false;
	for(int i=0; i < pData->GetSubItemCount(); i++)
	{
		CRect cellrect;
		_GetCellRect(row,i,cellrect);
		if (cellrect.PtInRect(pt))
		{
			col = i;
			flgIsHit = true;
			break;
		}
	}

	if(flgIsHit == false)
		return false;

	return true;
}

void CCustomListCtrl::_GetMergeCellRows(const int& row,const int& col,int &FirstRowInMerges,int &LastRowInMerges)
{
	// 往上判断，循环判断本单元格是否需要与上行同列单元格合并
	FirstRowInMerges = row;
	while(IsCellMergeWithTop(FirstRowInMerges,col))
	{
		FirstRowInMerges--;
	}

	// 往下判断，循环判断本单元格是否需要与下行同列单元格合并
	LastRowInMerges = row;
	while(IsCellMergeWithBottom(LastRowInMerges,col))
	{
		LastRowInMerges++;
	}
}

void CCustomListCtrl::_GetMergeCellRect(const int & row, const int &col,CRect &rect)
{
	// 根据实际索引获取首行和末行索引
	int FirstRow,LastRow;
	_GetMergeCellRows(row,col,FirstRow,LastRow);

	// 取得合并单元格的首行单元格区域
	CRect rcTopCell,rcBottomCell;
	_GetCellRect(FirstRow,col,rcTopCell);
	rect = rcTopCell;

	// 如果合并单元格末行与首行不同
	if(FirstRow != LastRow)
	{
		_GetCellRect(LastRow,col,rcBottomCell);
		rect.bottom = rcBottomCell.bottom;
	}
}

void CCustomListCtrl::_ControlFloatShowState(const int &RowIndex, const int &ColumnIndex)
{
	// 判断列更改属性，显示浮动控件
	CListItemData * pThisItem = reinterpret_cast<CListItemData *> (GetItemData(RowIndex));
	ColumnProperties colpro = pThisItem->GetColumnProperties(ColumnIndex);
	
	switch(colpro)
	{
	case DeletePrompt:			// 值可提示删除，显示“删除”按钮
		{
			FloatButton newButton;
			newButton.strCation = _T("删除");
			newButton.row = RowIndex;
			newButton.col = ColumnIndex;
			newButton.FuntionCode = FloatButton::fcDelete;
			_ShowButton(newButton);
			break;
		}
	case Optional:				// 可选择，显示组合框	
		{
			FloatCombox newCombox;
			newCombox.row = RowIndex;
			newCombox.col = ColumnIndex;
			if(pThisItem->GetComboxProperties(ColumnIndex,
											  newCombox.vctOptions,
											  newCombox.DefaultSelectIndex,
											  newCombox.IsEditable))
				_ShowCombox(newCombox);
			break;
		}

	case DiretEditable:			// 值可直接编辑，直接显示编辑控件
		{
			// 取得单元格内容
			FloatEdit newEdit;
			newEdit.col = ColumnIndex;
			newEdit.row = RowIndex;
			pThisItem->GetCellContex(ColumnIndex,newEdit.Context);
			_ShowEditCtrl(newEdit);
		}
		break;
	case PromptEditable:		// 值可提示编辑,显示“编辑”按钮
		{
			FloatButton newButton;
			newButton.strCation = _T("编辑");
			newButton.row = RowIndex;
			newButton.col = ColumnIndex;
			newButton.FuntionCode = FloatButton::fcEdit;
			_ShowButton(newButton);
		}
		break;
	case VariableSize:			// 大小可变，可显示微调按钮
		{
			FloatSpin newSpin;
			newSpin.row = RowIndex;
			newSpin.col = ColumnIndex;
			if(pThisItem->GetSpinBtnProperties(ColumnIndex,
											   newSpin.InitValue,
											   newSpin.MinValue,
											   newSpin.MaxValue,
											   newSpin.IncreamentOfSpin))
			{
				_ShowSpin(newSpin);
			}
			break;
		}
	case VariableDate:			// 日期可变，显示日期控件
		{
			FloatDateCtrl newDateCtrl;
			newDateCtrl.row = RowIndex;
			newDateCtrl.col = ColumnIndex;
			if(pThisItem->GetDateCtrlProperties(ColumnIndex,newDateCtrl.DefaultDate))
			{
				_ShowDateCtrl(newDateCtrl);
			}

			break;
		}
	default:
		_HideAllFloatCtrl();
	}
}

void CCustomListCtrl::_ShowEditCtrl(FloatEdit & newEdit)
{
	CRect rcMergeCell;
	_GetMergeCellRect(newEdit.row,newEdit.col,rcMergeCell);
	
	rcMergeCell.top++;
	rcMergeCell.left++;
	rcMergeCell.right--;
	rcMergeCell.bottom--;

	newEdit.pCtrlEdit = new CEdit;
	newEdit.pCtrlEdit->Create(WS_CHILD|WS_CLIPSIBLINGS|WS_EX_TOOLWINDOW,rcMergeCell,this,IDC_Float_EditCtrl);
	newEdit.pCtrlEdit->SetFont(GetFont());
	newEdit.pCtrlEdit->SetWindowText(newEdit.Context);
	newEdit.pCtrlEdit->SetSel(0,100);
	newEdit.pCtrlEdit->SetFocus();
	newEdit.pCtrlEdit->ShowWindow(SW_SHOW);
	m_vctEditCtrl.push_back(newEdit);
	UpdateWindow();
}

void CCustomListCtrl::_ShowButton(FloatButton & newButton)
{
	if(newButton.row < 0 || newButton.col < 0)
		return;

	CRect rcMergeCell,rcButton;
	_GetMergeCellRect(newButton.row,newButton.col,rcMergeCell);

	// 在单元格右侧显示“删除”按钮
	rcButton.right = rcMergeCell.right;
	rcButton.bottom = rcMergeCell.bottom;
	rcButton.top = rcButton.bottom - m_ItemHeight;
	rcButton.left = rcButton.right - 35;

	newButton.pButton = new CButton();
	newButton.pButton->Create(newButton.strCation,WS_CHILD,rcButton,this,IDC_Float_Button);
	newButton.pButton->SetFont(GetFont());
	newButton.pButton->SetFocus();
	newButton.pButton->ShowWindow(SW_SHOW);
	newButton.pButton->SetFocus();
	m_vctButton.push_back(newButton);
}

void CCustomListCtrl::_ShowCombox(FloatCombox newCombox)
{
	if(newCombox.row < 0 || newCombox.col < 0)
		return;

	// 定义合并单元格显示区
	CRect rcMergeCell;
	_GetMergeCellRect(newCombox.row,newCombox.col,rcMergeCell);

	// 浮动组合框
	newCombox.pCtrlComboBox = new CComboBox();
	if(newCombox.IsEditable)
		newCombox.pCtrlComboBox->Create(WS_CHILD|CBS_DROPDOWN,rcMergeCell,this,IDC_Float_Combox);
	else
		newCombox.pCtrlComboBox->Create(WS_CHILD|CBS_DROPDOWNLIST,rcMergeCell,this,IDC_Float_Combox);

	newCombox.pCtrlComboBox->ModifyStyle(1,CBS_DROPDOWNLIST,0);
	newCombox.pCtrlComboBox->MoveWindow(&rcMergeCell);
	newCombox.pCtrlComboBox->SetFont(GetFont());
	newCombox.pCtrlComboBox->ShowWindow(SW_SHOW);
	newCombox.pCtrlComboBox->SetFocus();

	for(int i=0; i< (int)newCombox.vctOptions.size();i++)
	{
		newCombox.pCtrlComboBox->AddString(newCombox.vctOptions[i]);
	}
	newCombox.pCtrlComboBox->SetCurSel(newCombox.DefaultSelectIndex);

	m_vctCombox.push_back(newCombox);
}

void CCustomListCtrl::_ShowSpin(FloatSpin newSpin)
{
	if(newSpin.row < 0 || newSpin.col < 0)
		return;

	// 定义合并单元格、微调按钮、编辑框显示区
	CRect rcMergeCell,rcButton,rcEdit;
	_GetMergeCellRect(newSpin.row,newSpin.col,rcMergeCell);

	// 取合并单元跟首行单元格区域
	rcMergeCell.bottom = rcMergeCell.top + m_ItemHeight;

	rcButton.right = rcMergeCell.right - 1;
	rcButton.bottom = rcMergeCell.bottom - 2;
	rcButton.top = rcMergeCell.top + 2;
	rcButton.left = rcButton.right - 20;

	rcEdit.left = rcMergeCell.left + 1;
	rcEdit.right = rcButton.left - 1;	
	rcEdit.top = rcMergeCell.top + 1;	
	rcEdit.bottom = rcMergeCell.bottom - 1;	

	// 浮动微调按钮
	newSpin.pSpinButton = new CSpinButtonCtrl();
	newSpin.pSpinButton->Create(UDS_WRAP|UDS_SETBUDDYINT|UDS_ALIGNRIGHT,rcButton,this,IDC_Float_SpinButton);
	newSpin.pSpinButton->MoveWindow(&rcButton);
	newSpin.pSpinButton->ShowWindow(SW_SHOW);

	FloatEdit newEdit;
	newEdit.row = m_FocusRow;
	newEdit.col = m_FocusCol;
	newEdit.pCtrlEdit = new CEdit();
	newEdit.pCtrlEdit->Create(WS_CHILD|WS_CLIPSIBLINGS|WS_EX_TOOLWINDOW,rcEdit,this,IDC_Float_EditCtrl);
	newEdit.pCtrlEdit->SetFont(GetFont());
	newEdit.pCtrlEdit->ShowWindow(SW_SHOW);

	newEdit.Context.Format(_T("%g"),newSpin.InitValue);
	newEdit.pCtrlEdit->SetWindowText(newEdit.Context);
	newEdit.pCtrlEdit->SetSel(0,100);
	newEdit.pCtrlEdit->SetFocus();
	newSpin.pBuddyEdit = newEdit.pCtrlEdit;

	m_vctEditCtrl.push_back(newEdit);
	m_vctSpinBtn.push_back(newSpin);
}

void CCustomListCtrl::_ShowDateCtrl(FloatDateCtrl newDateCtrl)
{
	if(newDateCtrl.row < 0 || newDateCtrl.col < 0)
		return;

	// 定义合并单元格
	CRect rcMergeCell;
	_GetMergeCellRect(newDateCtrl.row,newDateCtrl.col,rcMergeCell);

	// 取合并单元跟首行单元格区域
	rcMergeCell.bottom = rcMergeCell.top + m_ItemHeight;

	// 浮动微调按钮
	newDateCtrl.pCtrlDate = new CDateTimeCtrl();
	newDateCtrl.pCtrlDate->Create(DTS_APPCANPARSE,rcMergeCell,this,IDC_Float_DateCtrl);
	newDateCtrl.pCtrlDate->MoveWindow(&rcMergeCell);
	newDateCtrl.pCtrlDate->ShowWindow(SW_SHOW);
	newDateCtrl.pCtrlDate->SetFocus();

	m_vctDateCtrl.push_back(newDateCtrl);
}

void CCustomListCtrl::OnFloatBtnClk()
{
	if(m_FocusRow < 0)
		return;

	// 取得当前按钮指针
	CButton* pCurrBtn = NULL;
	for(int i=0; i<(int)m_vctButton.size(); i++)
	{
		if(m_vctButton[i].row == m_FocusRow && m_vctButton[i].col == m_FocusCol)
		{
			pCurrBtn = m_vctButton[i].pButton;
			break;
		}
	}

	if(pCurrBtn == NULL)
	{
		ASSERT(0);
		return;
	}

	// 查找本按钮对应的属性
	for(int i=0; i < (int)m_vctButton.size(); i++)
	{
		if(m_vctButton[i].pButton == pCurrBtn)
		{
			// 判断本按钮功能码
			switch(m_vctButton[i].FuntionCode)
			{
			case FloatButton::fcDelete:
				DeleteItem(m_FocusRow);
				RedrawAllItems();
				return;
			case FloatButton::fcEdit:
				{
					if(m_FocusRow < 0)
						return;

					pCurrBtn->ShowWindow(SW_HIDE);
					CListItemData * pThisItem = reinterpret_cast<CListItemData *> (GetItemData(m_FocusRow));
					FloatEdit newEdit;
					newEdit.col = m_FocusCol;
					newEdit.row = m_FocusRow;
					pThisItem->GetCellContex(m_FocusCol,newEdit.Context);
					_ShowEditCtrl(newEdit);
					return;
				}
			}
		}
	}
}

void CCustomListCtrl::OnComboxSelChange()
{
	if(m_FocusRow < 0)
		return;

	// 取得当前组合框指针
	CComboBox* pCurrCombo = NULL;
	for(int i=0; i<(int)m_vctCombox.size(); i++)
	{
		if(m_vctCombox[i].row == m_FocusRow && m_vctCombox[i].col == m_FocusCol)
		{
			pCurrCombo = m_vctCombox[i].pCtrlComboBox;
			break;
		}
	}

	if(pCurrCombo == NULL)
	{
		ASSERT(0);
		return;
	}

	CListItemData * pThisItem = reinterpret_cast<CListItemData *> (GetItemData(m_FocusRow));
	pThisItem->OnCbnSelchange(m_FocusCol,pCurrCombo->GetCurSel());
	return;	
}

void CCustomListCtrl::OnDeltaposSpinBtn(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	CEdit * pCurrEdit = NULL;
	// 取得当前组合框指针
	for(int i=0; i<(int)m_vctEditCtrl.size(); i++)
	{
		if(m_vctEditCtrl[i].row == m_FocusRow && m_vctEditCtrl[i].col == m_FocusCol)
		{
			pCurrEdit = m_vctEditCtrl[i].pCtrlEdit;
			break;
		}
	}

	if(pCurrEdit == NULL)
	{
		return;
	}

	CString strVolume;
	pCurrEdit->GetWindowText(strVolume);

	double fVolume;
	fVolume =  _tstof(strVolume);

	double increamt,maxVal,minVal;
	// 查找绑定本编辑控件的微调按钮控件的属性
	for(int i=0; i < (int)m_vctSpinBtn.size(); i++)
	{
		// 如果某微调按钮控件的绑定编辑控件是本控件
		if(m_vctSpinBtn[i].pBuddyEdit == pCurrEdit)
		{
			increamt = m_vctSpinBtn[i].IncreamentOfSpin;
			maxVal = m_vctSpinBtn[i].MaxValue;
			minVal = m_vctSpinBtn[i].MinValue;
			break;
		}
	}

	CString strValue;
	if(pNMUpDown->iDelta == -1)				// 如果此值为-1,即点击了Spin的向上箭头时
	{
		fVolume = fVolume + increamt;
		if(fVolume > maxVal)
			fVolume = maxVal;
	}
	else if(pNMUpDown->iDelta == 1)			// 如果此值为1,即点击了Spin的向下箭头时
	{
		fVolume = fVolume - increamt;
		if(fVolume < minVal)
			fVolume = minVal;
	}

	strVolume.Format(_T("%g"),fVolume);
	pCurrEdit->SetWindowText(strVolume);
	*pResult = 0;
}

void CCustomListCtrl::OnEnChangeEdit()
{
	if(m_FocusRow < 0)
		return;

	CEdit * pCurrEdit = NULL;
	// 取得当前组合框指针
	for(int i=0; i<(int)m_vctEditCtrl.size(); i++)
	{
		if(m_vctEditCtrl[i].row == m_FocusRow && m_vctEditCtrl[i].col == m_FocusCol)
		{
			pCurrEdit = m_vctEditCtrl[i].pCtrlEdit;
			break;
		}
	}

	if(pCurrEdit == NULL)
	{
		return;
	}

	CListItemData * pThisItem = reinterpret_cast<CListItemData *> (GetItemData(m_FocusRow));
	CString strNewContext;
	pCurrEdit->GetWindowText(strNewContext);
	pThisItem->OnEnChange(m_FocusCol,strNewContext);
}

void CCustomListCtrl::OnDtnDatetimechangeFloatDate(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(m_FocusRow < 0)
		return;

	CDateTimeCtrl * pCurrDate = NULL;
	// 取得当前组合框指针
	for(int i=0; i<(int)m_vctDateCtrl.size(); i++)
	{
		if(m_vctDateCtrl[i].row == m_FocusRow && m_vctDateCtrl[i].col == m_FocusCol)
		{
			pCurrDate = m_vctDateCtrl[i].pCtrlDate;
			break;
		}
	}

	if(pCurrDate == NULL)
	{
		return;
	}

	COleDateTime time;
	pCurrDate->GetTime(time);

	CListItemData * pThisItem = reinterpret_cast<CListItemData *> (GetItemData(m_FocusRow));
	pThisItem->OnDtnchange(m_FocusCol,time);
}


void CCustomListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl::OnSize(nType, cx, cy);

	// 列表控件总宽度
	int TotalWidth = cx;

	// 遍历列属性数组，设置列宽
	for (int i=0; i < (int)m_ColumnWidth.size();i++)
	{	
		// 如果列宽值为0,则设置为自动宽度
		if(m_ColumnWidth[i].Width == 0)
		{
			SetColumnWidth(m_ColumnWidth[i].ColumnIndex,LVSCW_AUTOSIZE_USEHEADER);
			continue;
		}

		// 如果列宽比例非零值,则设置为固定列宽值
		if(m_ColumnWidth[i].WidthRatio == 0)
		{
			SetColumnWidth(m_ColumnWidth[i].ColumnIndex,m_ColumnWidth[i].Width);
			continue;
		}

		// 如果列宽比例为非零值，且比例宽度值大于最小列宽值，则设置为此值
		if(TotalWidth * m_ColumnWidth[i].WidthRatio > m_ColumnWidth[i].Width )
		{
			SetColumnWidth(m_ColumnWidth[i].ColumnIndex,(int)(TotalWidth * m_ColumnWidth[i].WidthRatio));
		}
		else
			SetColumnWidth(m_ColumnWidth[i].ColumnIndex,m_ColumnWidth[i].Width);
	}
}

void CCustomListCtrl::SetColumnWidthProp(ColumnProperty columnProp)
{
	m_ColumnWidth.push_back(columnProp);
}
void CCustomListCtrl::OnDestroy()
{
	DeleteAllItemsAndDatas();
	CListCtrl::OnDestroy();
}
