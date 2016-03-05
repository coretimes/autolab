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
	m_GridFrameColor = 	RGB(208,208,208);	//  ���߿���ɫ
	m_SelectedBackColor = RGB(230, 230, 230);
}

CCustomListCtrl::~CCustomListCtrl()
{
}

BEGIN_MESSAGE_MAP(CCustomListCtrl, CListCtrl)
	ON_WM_MEASUREITEM_REFLECT()		//ϵͳ�򸸴��ڷ���WM_MEASUREITEM��Ϣ�󣬷�����Ӧ����ΪMeasureItem
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
	ModifyStyle(0, LVS_OWNERDRAWFIXED | LVS_REPORT|WS_VSCROLL);				// ϵͳֻΪ����OWNERDRAWFIXED�����б���WM_MEASUREITEM��Ϣ
	ModifyStyleEx(0, LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);
	SetExtendedStyle(GetExtendedStyle()|LVS_EX_DOUBLEBUFFER);
	m_FocusRow = -1;
	m_FocusCol = -1;

	VERIFY( m_HeaderCtrl.SubclassWindow( GetHeaderCtrl()->GetSafeHwnd()));//ʵ�ֱ�ͷ�ؼ������໯
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

	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);		// �ػ����ڣ���ʹϵͳ����WM_MEASUREITEM��Ϣ
}

void CCustomListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC dc;													// �����豸������
	dc.Attach(lpDrawItemStruct->hDC);						// �����豸�����ľ��
	int nSavedDC = dc.SaveDC();								// �����豸������

	lpDrawItemStruct->rcItem.bottom = lpDrawItemStruct->rcItem.top + m_ItemHeight;
	dc.SetBkColor(TRANSPARENT);

	// 2.���Ƹ���������
	CListItemData* pData =  reinterpret_cast<CListItemData *>(lpDrawItemStruct->itemData);

	if(pData == NULL)
	{
		ASSERT(0);
		return;
	}

	for (int i = 0; i < pData->GetSubItemCount(); i ++)
	{
		// ȡ�õ�Ԫ���������
		CRect rcCell;
		_GetCellRect(lpDrawItemStruct->itemID,i,rcCell);

		// ���������ѡ��״̬������ѡ��״̬����ɫ
		if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
			(lpDrawItemStruct->itemState & ODS_SELECTED))
		{
			// ѡ��״̬ǳ��ɫ��ˢ
			CBrush brSelected(m_SelectedBackColor);				
			CPen penSelected(PS_SOLID, 1, m_SelectedBackColor);	
			dc.SelectObject(brSelected);
			dc.SelectObject(penSelected);

			// ������Ԫ�����߿򣬵�Ԫ����������С
			CRect rect = rcCell;
			rect.left++;
			dc.Rectangle(&rect);
		}
		
		// �����������ԣ��жϱ����Ƿ���Ҫ������ʾ
		if (pData->IsHighlight() == true)
		{
			// ѡ��״̬ǳ��ɫ��ˢ
			CBrush brSelected(RGB(230, 230, 230));				
			CPen penSelected(PS_SOLID, 1, RGB(230, 230, 230));	
			dc.SelectObject(brSelected);
			dc.SelectObject(penSelected);

			// ������Ԫ�����߿򣬵�Ԫ����������С
			CRect rect = rcCell;
			rect.left++;
			dc.Rectangle(&rect);
		}

		// ȡ�úϲ���Ԫ���������
		CRect rcMergeCell;
		_GetMergeCellRect(lpDrawItemStruct->itemID,i,rcMergeCell);

		// �ϲ���Ԫ����Ľǵ�
		CPoint ptTopLeft = rcMergeCell.TopLeft();
		CPoint ptBottomRight = rcMergeCell.BottomRight();
		CPoint ptTopRight = CPoint(ptBottomRight.x,ptTopLeft.y);
		CPoint ptBottomLeft = CPoint(ptTopLeft.x,ptBottomRight.y);

		// �߿򻭱�
		CPen pen(PS_SOLID, 1, m_GridFrameColor);
		dc.SelectObject(pen);

		// ����ǵ�һ�У����軭��߿�
		if(i == 0)
		{
			dc.MoveTo(ptBottomLeft);
			dc.LineTo(ptTopLeft);
		}
		// ��������һ���У����軭�±߿�
		if(lpDrawItemStruct->itemID == GetItemCount() - 1)
		{
			dc.MoveTo(ptBottomLeft);
			dc.LineTo(ptBottomRight);
		}

		// ��ͨ��Ԫ��ֻ�軭�ϱ߿���ұ߿�
		dc.MoveTo(ptBottomRight);
		dc.LineTo(ptTopRight);

		// ������ϵ�Ԫ��ϲ�������Ҫ�ٻ��ƺ��ߺ�����
		if(IsCellMergeWithTop(lpDrawItemStruct->itemID,i))
			continue;
		dc.LineTo(ptTopLeft);
		// ���Ƶ�Ԫ������
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
	// ���������ѡ�У�����Ӧ������Ϣ
	if(m_bCanBeSelected == FALSE)
	{
		SetFocus();
		return;
	}

	if(GetFocus() != this)
	{
		// �����������и����ؼ�
		_HideAllFloatCtrl();
		SetFocus();
		return;
	}

	// ȡ�û��е�Ԫ�������
	int RowIndex, ColumnIndex;
	if(!_GetCellIndex(point,RowIndex,ColumnIndex))
	{
		// �½����С����������
		m_FocusRow = -1;
		m_FocusCol = -1;
		return;
	}
	// ����Ϊ���㴰��
	SetFocus();

	// ȡ�úϲ���Ԫ������к�ĩ��
	int FirstRowInMerges,LastRowInMerges;
	_GetMergeCellRows(RowIndex,ColumnIndex,FirstRowInMerges,LastRowInMerges);

	//���û�а���ctrl��������Ҫȥ��ԭ��ѡ��Ŀ��ѡ��״̬
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

	// �����һ����ѡ������һ����ѡ�����������
	int FirstSelectedItem,LastSelectedItem;

	// ���������shift�������жϵ�һ����ѡ�������һ����ѡ��
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
	// ���û�а���shift������Ϊ��������
	else
	{
		FirstSelectedItem = FirstRowInMerges;
		LastSelectedItem = LastRowInMerges;
	}


	// ѭ�����ñ�ѡ��״̬
	for(int i=FirstSelectedItem; i <= LastSelectedItem; i++)
	{
 		SetItemState(i,LVIS_SELECTED, LVIS_SELECTED);
	}

	// �����½���״̬
	SetItemState(RowIndex,LVIS_FOCUSED,LVIS_FOCUSED);
	SetSelectionMark(RowIndex);

	// �ػ����б�ѡ��
	RedrawItems(FirstSelectedItem,LastSelectedItem);

	// �½����С����������
	m_FocusRow = RowIndex;
	m_FocusCol = ColumnIndex;

	// ���������Control����Shift�������������и����ؼ�
	if((nFlags & MK_CONTROL) || (nFlags & MK_SHIFT))
		_HideAllFloatCtrl();
	else
		_ControlFloatShowState(m_FocusRow,m_FocusCol);

}

BOOL CCustomListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// ��������ʱ��Ҫɾ�����и����ؼ�	
	_HideAllFloatCtrl();
	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CCustomListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// ����������Ǵ��ڹ�������pScrollBar==NULLʱ�������������пؼ�
	if(pScrollBar == NULL)
		_HideAllFloatCtrl();
	RedrawAllItems();
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CCustomListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// ����������Ǵ��ڹ�������pScrollBar==NULLʱ�������������пؼ�
	if(pScrollBar == NULL)
		_HideAllFloatCtrl();

	if(nSBCode == SB_THUMBPOSITION)
	{
		// ȡ������ʾ�ĵ�һ���к�
		UINT idxTop = GetTopIndex();
		// �����ǰ�б�����ʾ�ĵ�һ�д���10����ʼ���¹���,�����10��
		if(nPos - idxTop > 10)
			Scroll(CSize(0,(nPos - idxTop + 8) * m_ItemHeight));
		// �����ǰ�б�����ʾ�ĵ�һ��С����ʼ���Ϲ����������10��
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
	// ��ͷ�����¼�
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	if (GetItemCount() < 1)
		return;

	_HideAllFloatCtrl();
	// ����ȡһ�����ݣ�ȥ�ı�����ģʽ��̬����
	CListItemData *pData = reinterpret_cast<CListItemData*>(GetItemData(0));
	if(pData->ChangeSortMode(phdr->iItem))
	{
		Reorder();
	}
}
BOOL CCustomListCtrl::DeleteAllItemsAndDatas()
{
	_HideAllFloatCtrl();								// �������и����ؼ�

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
	// ����ɾ����
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
	// ȡ����������ָ�뱣����vector��
	vector<CListItemData *> vctDataList;
	for(int i=0; i<GetItemCount(); i++)
	{
		vctDataList.push_back( reinterpret_cast<CListItemData *>(GetItemData(i)));
	}

	// �����ݽ�������
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
	// �����༭�ؼ�ָ������
	for(int i = 0; i < (int)m_vctEditCtrl.size(); i++)
	{
		m_vctEditCtrl[i].pCtrlEdit->DestroyWindow();
		delete m_vctEditCtrl[i].pCtrlEdit;
	}
	m_vctEditCtrl.clear();

	// ������Ͽؼ�ָ������
	for(int i = 0; i < (int)m_vctCombox.size(); i++)
	{
		m_vctCombox[i].pCtrlComboBox->DestroyWindow();
		delete m_vctCombox[i].pCtrlComboBox;
	}
	m_vctCombox.clear();

	// ������Ͽؼ�ָ������
	for(int i = 0; i < (int)m_vctButton.size(); i++)
	{
		m_vctButton[i].pButton->DestroyWindow();
		delete m_vctButton[i].pButton;
	}
	m_vctButton.clear();

	// ����΢����ť�ؼ�ָ������
	for(int i = 0; i < (int)m_vctSpinBtn.size(); i++)
	{
		m_vctSpinBtn[i].pSpinButton->DestroyWindow();
		delete m_vctSpinBtn[i].pSpinButton;
	}
	m_vctSpinBtn.clear();

	// ʱ�����ڿؼ�ָ������
	for(int i = 0; i < (int)m_vctDateCtrl.size(); i++)
	{
		m_vctDateCtrl[i].pCtrlDate->DestroyWindow();
		delete m_vctDateCtrl[i].pCtrlDate;
	}
	m_vctDateCtrl.clear();
}

bool CCustomListCtrl::_GetCellIndex(const CPoint& pt, int& row, int& col)
{
	// �жϻ����е�����
	row	 = HitTest(pt,NULL);
	if(row < 0)
		return false;

	// �жϻ����е�����
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
	// �����жϣ�ѭ���жϱ���Ԫ���Ƿ���Ҫ������ͬ�е�Ԫ��ϲ�
	FirstRowInMerges = row;
	while(IsCellMergeWithTop(FirstRowInMerges,col))
	{
		FirstRowInMerges--;
	}

	// �����жϣ�ѭ���жϱ���Ԫ���Ƿ���Ҫ������ͬ�е�Ԫ��ϲ�
	LastRowInMerges = row;
	while(IsCellMergeWithBottom(LastRowInMerges,col))
	{
		LastRowInMerges++;
	}
}

void CCustomListCtrl::_GetMergeCellRect(const int & row, const int &col,CRect &rect)
{
	// ����ʵ��������ȡ���к�ĩ������
	int FirstRow,LastRow;
	_GetMergeCellRows(row,col,FirstRow,LastRow);

	// ȡ�úϲ���Ԫ������е�Ԫ������
	CRect rcTopCell,rcBottomCell;
	_GetCellRect(FirstRow,col,rcTopCell);
	rect = rcTopCell;

	// ����ϲ���Ԫ��ĩ�������в�ͬ
	if(FirstRow != LastRow)
	{
		_GetCellRect(LastRow,col,rcBottomCell);
		rect.bottom = rcBottomCell.bottom;
	}
}

void CCustomListCtrl::_ControlFloatShowState(const int &RowIndex, const int &ColumnIndex)
{
	// �ж��и������ԣ���ʾ�����ؼ�
	CListItemData * pThisItem = reinterpret_cast<CListItemData *> (GetItemData(RowIndex));
	ColumnProperties colpro = pThisItem->GetColumnProperties(ColumnIndex);
	
	switch(colpro)
	{
	case DeletePrompt:			// ֵ����ʾɾ������ʾ��ɾ������ť
		{
			FloatButton newButton;
			newButton.strCation = _T("ɾ��");
			newButton.row = RowIndex;
			newButton.col = ColumnIndex;
			newButton.FuntionCode = FloatButton::fcDelete;
			_ShowButton(newButton);
			break;
		}
	case Optional:				// ��ѡ����ʾ��Ͽ�	
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

	case DiretEditable:			// ֵ��ֱ�ӱ༭��ֱ����ʾ�༭�ؼ�
		{
			// ȡ�õ�Ԫ������
			FloatEdit newEdit;
			newEdit.col = ColumnIndex;
			newEdit.row = RowIndex;
			pThisItem->GetCellContex(ColumnIndex,newEdit.Context);
			_ShowEditCtrl(newEdit);
		}
		break;
	case PromptEditable:		// ֵ����ʾ�༭,��ʾ���༭����ť
		{
			FloatButton newButton;
			newButton.strCation = _T("�༭");
			newButton.row = RowIndex;
			newButton.col = ColumnIndex;
			newButton.FuntionCode = FloatButton::fcEdit;
			_ShowButton(newButton);
		}
		break;
	case VariableSize:			// ��С�ɱ䣬����ʾ΢����ť
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
	case VariableDate:			// ���ڿɱ䣬��ʾ���ڿؼ�
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

	// �ڵ�Ԫ���Ҳ���ʾ��ɾ������ť
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

	// ����ϲ���Ԫ����ʾ��
	CRect rcMergeCell;
	_GetMergeCellRect(newCombox.row,newCombox.col,rcMergeCell);

	// ������Ͽ�
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

	// ����ϲ���Ԫ��΢����ť���༭����ʾ��
	CRect rcMergeCell,rcButton,rcEdit;
	_GetMergeCellRect(newSpin.row,newSpin.col,rcMergeCell);

	// ȡ�ϲ���Ԫ�����е�Ԫ������
	rcMergeCell.bottom = rcMergeCell.top + m_ItemHeight;

	rcButton.right = rcMergeCell.right - 1;
	rcButton.bottom = rcMergeCell.bottom - 2;
	rcButton.top = rcMergeCell.top + 2;
	rcButton.left = rcButton.right - 20;

	rcEdit.left = rcMergeCell.left + 1;
	rcEdit.right = rcButton.left - 1;	
	rcEdit.top = rcMergeCell.top + 1;	
	rcEdit.bottom = rcMergeCell.bottom - 1;	

	// ����΢����ť
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

	// ����ϲ���Ԫ��
	CRect rcMergeCell;
	_GetMergeCellRect(newDateCtrl.row,newDateCtrl.col,rcMergeCell);

	// ȡ�ϲ���Ԫ�����е�Ԫ������
	rcMergeCell.bottom = rcMergeCell.top + m_ItemHeight;

	// ����΢����ť
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

	// ȡ�õ�ǰ��ťָ��
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

	// ���ұ���ť��Ӧ������
	for(int i=0; i < (int)m_vctButton.size(); i++)
	{
		if(m_vctButton[i].pButton == pCurrBtn)
		{
			// �жϱ���ť������
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

	// ȡ�õ�ǰ��Ͽ�ָ��
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
	// ȡ�õ�ǰ��Ͽ�ָ��
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
	// ���Ұ󶨱��༭�ؼ���΢����ť�ؼ�������
	for(int i=0; i < (int)m_vctSpinBtn.size(); i++)
	{
		// ���ĳ΢����ť�ؼ��İ󶨱༭�ؼ��Ǳ��ؼ�
		if(m_vctSpinBtn[i].pBuddyEdit == pCurrEdit)
		{
			increamt = m_vctSpinBtn[i].IncreamentOfSpin;
			maxVal = m_vctSpinBtn[i].MaxValue;
			minVal = m_vctSpinBtn[i].MinValue;
			break;
		}
	}

	CString strValue;
	if(pNMUpDown->iDelta == -1)				// �����ֵΪ-1,�������Spin�����ϼ�ͷʱ
	{
		fVolume = fVolume + increamt;
		if(fVolume > maxVal)
			fVolume = maxVal;
	}
	else if(pNMUpDown->iDelta == 1)			// �����ֵΪ1,�������Spin�����¼�ͷʱ
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
	// ȡ�õ�ǰ��Ͽ�ָ��
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
	// ȡ�õ�ǰ��Ͽ�ָ��
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

	// �б�ؼ��ܿ��
	int TotalWidth = cx;

	// �������������飬�����п�
	for (int i=0; i < (int)m_ColumnWidth.size();i++)
	{	
		// ����п�ֵΪ0,������Ϊ�Զ����
		if(m_ColumnWidth[i].Width == 0)
		{
			SetColumnWidth(m_ColumnWidth[i].ColumnIndex,LVSCW_AUTOSIZE_USEHEADER);
			continue;
		}

		// ����п��������ֵ,������Ϊ�̶��п�ֵ
		if(m_ColumnWidth[i].WidthRatio == 0)
		{
			SetColumnWidth(m_ColumnWidth[i].ColumnIndex,m_ColumnWidth[i].Width);
			continue;
		}

		// ����п����Ϊ����ֵ���ұ������ֵ������С�п�ֵ��������Ϊ��ֵ
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
