//*****************************************************************************
// �ļ��� : CustomListCtrl.h
// 
// ����   : �Զ����б���ͼ�ؼ�
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-10       ��־��       �½�
// ------------------------------------------
// ��ע��
// ------------------------------------------
// 1.���б���ͼ�ؼ���ʵ������ͬ�е�Ԫ��ĺϲ�Ч��
//
// 2.ÿ���б������һ��ָ��CListItemData�������ݵ�ָ��
//
// 3.�ж�ĳ���б����ĳ����Ԫ���Ƿ��������б����ͬ�е�Ԫ��ϲ���
//   ����CListItemData�����жϾ�����
// 
// 4.ĳ����Ԫ���Ƿ�ɱ༭Ҳ����CListItemData�����жϾ�����
//
// 5.�������п���ɴ����ߣ����Ի��򣩾��������п����Ϊ0ʱ�������ظ���
//
// 6.��ָ��ָ���б�ĳ����Ԫ��ʱ������ʾ�����ؼ�
//   �磺�༭�򡢡�ɾ������ť�����༭����ť����Ͽ�΢����ť�ȵ�
//   ������ʾ�ĸ������ؼ�������CListItemData�����ж�
// 
// 7.�����б��ͷ������ʵ�������ܣ�����ʽ������CListItemData����
//*****************************************************************************
#pragma once
#include "ListItemData.h"

//***************************************
// �����༭��ؼ�����
//***************************************
struct FloatEdit
{
	CEdit * pCtrlEdit;					// �ؼ�ָ��
	CString Context;					// �༭�ؼ�����
	int row;							// �ؼ�����������
	int col;							// �ؼ�����������
};

//***************************************
// ������Ͽ�ؼ�����
//***************************************
struct FloatCombox
{
	CComboBox * pCtrlComboBox;			// �ؼ�ָ��
	vector<CString> vctOptions;			// ��Ͽ��ѡ����������
	bool IsEditable;					// �Ƿ�ɱ༭
	int DefaultSelectIndex;				// Ĭ������
	int row;							// �ؼ�����������
	int col;							// �ؼ�����������
};

//***************************************
// ������ת��ť��΢����ť���ؼ�����
//***************************************
struct FloatSpin
{
	CSpinButtonCtrl * pSpinButton;		// �ؼ�ָ��
	CEdit * pBuddyEdit;					// �󶨱༭�ؼ�ָ��
	double InitValue;					// ΢����ť�ĳ�ʼֵ
	double IncreamentOfSpin;			// ΢����ť����������
	double MaxValue;					// ΢����ť������
	double MinValue;					// ΢����ť������
	int row;							// �ؼ�����������
	int col;							// �ؼ�����������
};

//***************************************
// ������ť�ؼ�����
//***************************************
struct FloatButton
{
	enum ButtonFuncionCode				// ��ť������ö��
	{
		fcDelete = 0,					// "ɾ��"
		fcEdit							// "�༭"
	} FuntionCode;

	CButton * pButton;					// �ؼ�ָ��
	CString strCation;					// ����
	int row;							// �ؼ�����������
	int col;							// �ؼ�����������
};

//***************************************
// ���ڿؼ�����
//***************************************
struct FloatDateCtrl
{
	CDateTimeCtrl * pCtrlDate;			// �ؼ�ָ��
	COleDateTime DefaultDate;			// Ĭ��ʱ��
	int row;							// �ؼ�����������
	int col;							// �ؼ�����������
};

//***************************************
// �ؼ��п�����
//***************************************
struct ColumnProperty
{
	int ColumnIndex;					// ������
	int Width;							// ���ֵ�������ȱ���WidthRatioΪ��0ֵ�����ֵ��ʾ��С���ֵ
	double WidthRatio;					// ��ȱ��������Ϊ0�����п��ֵ��ΪWidth

};

//***************************************
// �Զ����б�ؼ�
//***************************************
class CCustomListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CCustomListCtrl)
public:
	CCustomListCtrl();
	virtual ~CCustomListCtrl();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// --------------------------------------------------
	// ժҪ��
	//	   �����б���
	// ������
	//     1.nItem - λ������
	//     2.pItemData - CListItemData��������ָ��
	// --------------------------------------------------
	void AddItem(int nItem,CListItemData * pItemData);

	// --------------------------------------------------
	// ժҪ��
	//	   �����б��Ƿ�����ѡ��
	// --------------------------------------------------
	void EnableSelect(BOOL IsEnableSelect)
	{
		m_bCanBeSelected = IsEnableSelect;
	}
	// --------------------------------------------------
	// ժҪ��
	//	   �����б���߶�
	// ������
	//     1.height - �߶�ֵ
	// --------------------------------------------------
	void SetItemHeight(const LONG &height);

	// --------------------------------------------------
	// ժҪ��
	//	   �ػ������б���
	// --------------------------------------------------
	void RedrawAllItems(void)
	{
		RedrawItems(0,GetItemCount()-1);
	}

	// --------------------------------------------------
	// ժҪ��
	//	   ɾ���������ɾ����������,
	//     ��CListc::DeleteAllItems��ɾ������
	// --------------------------------------------------
	BOOL DeleteAllItemsAndDatas();

	// --------------------------------------------------
	// ժҪ��
	//	   ɾ��ָ�����еĶ���
	// --------------------------------------------------
	BOOL DeleteMultItems(vector<int> &vctDeleteIndexes);

	// --------------------------------------------------
	// ժҪ��
	//	   ȡ��������
	// --------------------------------------------------
	int GetColumnCount() const
	{
		return m_HeaderCtrl.GetItemCount();
	}

	// --------------------------------------------------
	// ժҪ��
	//	   �ж�ĳ��Ԫ���Ƿ����ϵ�Ԫ��ϲ�
	// --------------------------------------------------
	bool IsCellMergeWithTop(const int& row,const int& col) const
	{
		if(row <= 0)
			return false;
		CListItemData * pThisItem = reinterpret_cast<CListItemData *>(GetItemData(row));
		CListItemData * pTopItem = reinterpret_cast<CListItemData *>(GetItemData(row - 1));
		return pThisItem->IsFieldEquivalent(col,pTopItem);
	}

	// --------------------------------------------------
	// ժҪ��
	//	   �ж�ĳ��Ԫ���Ƿ����µ�Ԫ��ϲ�
	// --------------------------------------------------
	bool IsCellMergeWithBottom(const int& row,const int& col) const
	{
		if(row >= GetItemCount() - 1)
			return false;
		CListItemData * pThisItem = reinterpret_cast<CListItemData *> (GetItemData(row));
		CListItemData * pBottomItem = reinterpret_cast<CListItemData *> (GetItemData(row + 1));
		return pThisItem->IsFieldEquivalent(col,pBottomItem);
	}

	// --------------------------------------------------
	// ժҪ��
	//	   �����������������к�����ʽ��ListItemData����
	// --------------------------------------------------
	void Reorder();

	// --------------------------------------------------
	// ժҪ��
	//	   �����п�����
	// ������
	//	   1.columnProp - ������
	// --------------------------------------------------
	void SetColumnWidthProp(ColumnProperty columnProp);

protected:
	DECLARE_MESSAGE_MAP()

	// --------------------------------------------------
	// ժҪ��
	//	   WM_MEASUREITEM��Ϣ��Ӧ����
	// --------------------------------------------------
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
	{
		lpMeasureItemStruct->itemHeight = m_ItemHeight;	    // �и�����
	}

	// --------------------------------------------------
	// ժҪ��
	//	   WM_SIZE��Ϣ��Ӧ����
	// --------------------------------------------------
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// --------------------------------------------------
	// ժҪ��
	//	   ������������Ϣ��Ӧ����
	// --------------------------------------------------
	afx_msg void OnLButtonDown(uint32 nFlags, CPoint point);

	// --------------------------------------------------
	// ժҪ��
	//	   ��껬�ֹ�����Ϣ��Ӧ����
	// --------------------------------------------------
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	// --------------------------------------------------
	// ժҪ��
	//	   ˮƽ������������Ϣ��Ӧ����
	// --------------------------------------------------
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	// --------------------------------------------------
	// ժҪ��
	//	   ��ֱ������������Ϣ��Ӧ����
	// --------------------------------------------------
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	// --------------------------------------------------
	// ժҪ��
	//	   ��ͷ�϶���Ϣ��Ӧ
	// --------------------------------------------------
	afx_msg void OnHdnBegintrack(NMHDR *pNMHDR, LRESULT *pResult);

	// --------------------------------------------------
	// ժҪ��
	//	   ��ͷ������Ϣ��Ӧ
	// --------------------------------------------------
	afx_msg void OnHdnItemclick(NMHDR *pNMHDR, LRESULT *pResult);

	// --------------------------------------------------
	// ժҪ��
	//	   ���໯ǰ�ĳ�ʼ������
	// --------------------------------------------------
	virtual void PreSubclassWindow();

	// --------------------------------------------------
	// ժҪ��
	//	   ����ָ����Ԫ���ϸ����ؼ�����ʾ״̬
	// ������
	//     1.row - ������
	//     2.col - ������
	// --------------------------------------------------
	void _ControlFloatShowState(const int &row, const int &col);

	// --------------------------------------------------
	// ժҪ��
	//	   ��ʾ�༭��
	// --------------------------------------------------
	void _ShowEditCtrl(FloatEdit & newEditCtrl);

	// --------------------------------------------------
	// ժҪ��
	//	   �����༭�����ݱ仯��Ӧ����
	// --------------------------------------------------
	afx_msg void OnEnChangeEdit();

	// --------------------------------------------------
	// ժҪ��
	//	   ��ʾ��ť
	// --------------------------------------------------
	void _ShowButton(FloatButton & newButton);

	// --------------------------------------------------
	// ժҪ��
	//	   ������ť������Ӧ��Ϣ
	// --------------------------------------------------
	afx_msg void OnFloatBtnClk(void);

	// --------------------------------------------------
	// ժҪ��
	//	   ��ʾ��Ͽ�
	// --------------------------------------------------
	void _ShowCombox(FloatCombox newCombox);

	// --------------------------------------------------
	// ժҪ��
	//	   ������Ͽ�ѡ��仯��Ӧ����
	// --------------------------------------------------
	afx_msg void OnComboxSelChange(void);

	// --------------------------------------------------
	// ժҪ��
	//	   ��ʾ΢����ť���󶨵ı༭��
	// --------------------------------------------------
	void _ShowSpin(FloatSpin newSpin);

	// --------------------------------------------------
	// ժҪ��
	//	   ����΢����ť������Ӧ����
	// --------------------------------------------------
	afx_msg void OnDeltaposSpinBtn(NMHDR *pNMHDR, LRESULT *pResult);

	// --------------------------------------------------
	// ժҪ��
	//	   ��ʾʱ��ռ�
	// --------------------------------------------------
	void _ShowDateCtrl(FloatDateCtrl newDateCtrl);

	// ---------------------------------------------------------
	// �������ܣ�
	//     ���ͼ����ڡ��仯��Ӧ����
	// ---------------------------------------------------------
	afx_msg void OnDtnDatetimechangeFloatDate(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// �������ܣ�
	//     ���ٺ���������֮ǰ��ɾ����������
	// ---------------------------------------------------------
	afx_msg void OnDestroy();

	// --------------------------------------------------
	// ժҪ��
	//	   �������и����ؼ�
	// --------------------------------------------------
	void _HideAllFloatCtrl();

	// --------------------------------------------------
	// ժҪ��
	//	   �������ص�ȡ�õ�Ԫ�������
	// ������
	//     1.pt - ���ص�
	//     2.row - ������
	//     3.col - ������
	// --------------------------------------------------
	bool _GetCellIndex(const CPoint& pt, int& row, int& col);

	// --------------------------------------------------
	// ժҪ��
	//	   ����ʵ������������ȡ�úϲ���Ԫ������к�ĩ��
	//     1.row - ʵ��������
	//     2.col - ʵ��������
	//     3.FirstRowInMerges - ����Ǻϲ���Ԫ����ֵΪ�ϲ���Ԫ������У�
	//                          ������Ǻϲ���Ԫ�������rowֵ
	//     4.LastRowInMerges - ����Ǻϲ���Ԫ����ֵΪ�ϲ���Ԫ���ĩ�У�
	//                         ������Ǻϲ���Ԫ�������rowֵ
	// ������
	// --------------------------------------------------
	void _GetMergeCellRows(const int& row,const int& col,int &FirstRowInMerges,int &LastRowInMerges);

	// --------------------------------------------------
	// ժҪ��
	//	   ȡ�õ�Ԫ�����
	// --------------------------------------------------
	void _GetCellRect(const int & row, const int &col,CRect &rect)
	{
		GetSubItemRect(row,col,LVIR_LABEL,rect);
		if( col == 0)
			rect.left -= 4;
	}

	// --------------------------------------------------
	// ժҪ��
	//	   �������ص㣬ȡ�úϲ���Ԫ��ľ���
	// ������
	//     1.pt - ���ص�
	// --------------------------------------------------
	void _GetMergeCellRect(const int & row, const int &col,CRect &rect);

	// --------------------------------------------------
	// ժҪ��
	//	   ����
	// ������
	//     1.pt - ���ص�
	// --------------------------------------------------
	static bool _SortFunc(CListItemData * v1, CListItemData * v2)
	{
		return v1->IsBeforThan(v2);
	}

protected:
	vector<ColumnProperty> m_ColumnWidth;	// �п���������

	LONG m_ItemHeight;						// �б��и�
	int m_FocusRow;							// ����������
	int m_FocusCol;							// ����������
	bool m_bMouseTrack;						// ��ʶ�Ƿ�׷�ٵ�ǰ���״̬��֮����Ҫ��������
											// ��Ҫ��������Ѿ��ڴ���֮��ʱ��
											// һ�ƶ����Ͳ����ظ����� WM_MOUSEHOVER

	CHeaderCtrl m_HeaderCtrl;				// ��ͷ�ؼ�ָ��
	vector<FloatEdit> m_vctEditCtrl;		// �����༭�ؼ�����
	vector<FloatCombox> m_vctCombox;		// ������Ͽؼ�����
	vector<FloatButton> m_vctButton;		// ������ťָ������
	vector<FloatSpin> m_vctSpinBtn;			// ����΢����ť�ؼ�����
	vector<FloatDateCtrl> m_vctDateCtrl;	// ����ʱ��ؼ�����

	BOOL m_bCanBeSelected;					// �Ƿ�����ѡ��

public:
	COLORREF m_GridFrameColor;				// �߿���ɫ
	COLORREF m_SelectedBackColor;			// ѡ�б�����ɫ
};