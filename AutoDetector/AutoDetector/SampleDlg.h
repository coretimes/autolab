//*****************************************************************************
// �ļ��� : SampleDlg.h
// 
// ����   : ��������Ի���
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-12       ��־��       �½�
// ------------------------------------------
// ���ܸ�Ҫ��
// ------------------------------------------
// 
//*****************************************************************************

#pragma once
#include "BaseDialog.h"
#include "CustomListCtrl.h"

class CSampleDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CSampleDlg)

public:
	CSampleDlg(CWnd* pParent = NULL);					// ��׼���캯��
	virtual ~CSampleDlg();

	enum { IDD = IDD_Sample_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedStatic1();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

protected:
	CCustomListCtrl m_ctrlReagentList;								// ���������á��б�ؼ�
	HBRUSH m_hCtlBrush;												// �ؼ�������ˢ	

};
