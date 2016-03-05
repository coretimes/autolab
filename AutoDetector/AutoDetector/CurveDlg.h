//*****************************************************************************
// �ļ��� : CurveDlg.h
// 
// ����   : ��׼���߹���Ի���
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2014-2       ��־��       �½�
// ------------------------------------------
// ���ܸ�Ҫ��
// ------------------------------------------
// 
//*****************************************************************************
#pragma once
#include "BaseDialog.h"
#include "CustomListCtrl.h"

class CCurveDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CCurveDlg)

public:
	CCurveDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCurveDlg();

// �Ի�������
	enum { IDD = IDD_Curve_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// ---------------------------------------------------------
	// ժҪ��
	//     ����Ŀ������ơ���Ͽ����ݷ����仯��Ӧ
	// ---------------------------------------------------------
	afx_msg void OnCbnSelchangeComboPojectname();

	// ---------------------------------------------------------
	// ժҪ��
	//     �����š���Ͽ����ݷ����仯��Ӧ
	// ---------------------------------------------------------
	afx_msg void OnCbnSelchangeComboBatchnum();

	// ---------------------------------------------------------
	// ժҪ��
	//     ��������ʼ���ڡ����ݷ����仯
	// ---------------------------------------------------------
	afx_msg void OnDtnDatetimechangeDateTestdatefrom(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// ժҪ��
	//     ��������ֹ���ڡ����ݷ����仯
	// ---------------------------------------------------------
	afx_msg void OnDtnDatetimechangeDateTestdate(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// ժҪ��
	//     �������б�ѡ������仯
	// ---------------------------------------------------------
	afx_msg void OnLvnItemchangedListCurve(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	// ---------------------------------------------------------
	// ժҪ��
	//     ��ʼ������Ŀ������ơ���Ͽ�
	// ---------------------------------------------------------
	void _InitComboProjectName();

	// ---------------------------------------------------------
	// ժҪ��
	//     ��ʼ�������š���Ͽ�
	// ---------------------------------------------------------
	void _InitComboBatchNums();

	// ---------------------------------------------------------
	// ժҪ��
	//     ˢ�¡������б���������������������
	// ---------------------------------------------------------
	void _RefreshCurveList();

	// ---------------------------------------------------------
	// ժҪ��
	//     ˢ�¡�������ϸ��Ϣ��
	// ---------------------------------------------------------
	void _RefreshDetails();

protected:
	CCustomListCtrl m_ctrlCurveList;								// �������б��ؼ�
	CCustomListCtrl m_ctrlDetailsList;								// ��������ϸ��Ϣ���ؼ�

	CComboBox m_ctrlProjectName;									// �������Ŀ����Ͽ�
	CComboBox m_ctrlBatchNum;										// �����š���Ͽ�
	vector<ProjectFullName> m_AllProjectNameList;					// ��������Ŀ���ơ��б�
	vector<CString> m_AllBatchNumList;								// ���������š��б�	
	CDateTimeCtrl m_ctrlTestDateFrom;								// ���������ڡ���ʼ��������
	CDateTimeCtrl m_ctrlTestDateTo;									// ���������ڡ���ֹ��������
	Curve_Key m_nCurrentSelectedCurve;								//  ��ǰѡ������ID

	HBRUSH m_hCtlBrush;												// �ؼ�������ˢ	
public:
};
