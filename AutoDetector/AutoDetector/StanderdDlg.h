//*****************************************************************************
// �ļ��� : StanderdDlg.h
// 
// ����   : ��׼Ʒ����Ի�����
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2014-02      ��־��       �½�
// ------------------------------------------
// ��ע��
// ------------------------------------------
// 
//*****************************************************************************
#pragma once
#include "BaseDialog.h"
#include "CustomListCtrl.h"
#include "StandardTable.h"

class CStanderdDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CStanderdDlg)

public:
	CStanderdDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStanderdDlg();

// �Ի�������
	enum { IDD = IDD_Standerd_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	// ---------------------------------------------------------
	// ժҪ��
	//     ����ӡ���ť������Ӧ
	// ---------------------------------------------------------
	afx_msg void OnBnClickedAddrecordButton();

	// ---------------------------------------------------------
	// ժҪ��
	//     �������Ŀ����Ͽ�ѡ����仯��Ӧ
	// ---------------------------------------------------------
	afx_msg void OnCbnSelchangeComboPojectname();

	// ---------------------------------------------------------
	// ժҪ��
	//     ��������ʼ���ڡ��ؼ����ڷ����仯��Ӧ
	// ---------------------------------------------------------
	afx_msg void OnDtnDatetimechangeDateProductfrom(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// ժҪ��
	//     ��������ֹ���ڡ��ؼ����ڷ����仯��Ӧ
	// ---------------------------------------------------------
	afx_msg void OnDtnDatetimechangeDateProductto(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// ժҪ��
	//     ��ɾ������ť������Ӧ
	// ---------------------------------------------------------
	afx_msg void OnBnClickedDeleterecordButton();

	// ---------------------------------------------------------
	// ժҪ��
	//     ���޸ġ���ť������Ӧ
	// ---------------------------------------------------------
	afx_msg void OnBnClickedModifyButton();

	// ---------------------------------------------------------
	// ժҪ��
	//     ���ؼ�����ɫ��ȡ����Ϣ��Ӧ
	// ---------------------------------------------------------
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	// ---------------------------------------------------------
	// ժҪ��
	//     ��ʼ������Ŀ������ơ���Ͽ�
	// ---------------------------------------------------------
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	// ---------------------------------------------------------
	// ժҪ��
	//     ��׼Ʒѡ����仯
	// ---------------------------------------------------------
	afx_msg void OnLvnItemchangedListStanderd(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// ժҪ��
	//     ��ʼ������Ŀ������ơ���Ͽ�
	// ---------------------------------------------------------
	void _InitComboProjectName();

	// ---------------------------------------------------------
	// ժҪ��
	//     ˢ�¡���׼Ʒ�б���������������������
	// ---------------------------------------------------------
	void _RefreshStandardsList();

	// ---------------------------------------------------------
	// ժҪ��
	//     ˢ�¡���ϸ��Ϣ����
	// ---------------------------------------------------------
	void _RefreshDetails();

protected:
	CCustomListCtrl m_ctrlStandardsList;							// ����׼Ʒ���б�ؼ�

	CComboBox m_ctrlProjectName;									// �������Ŀ����Ͽ�
	vector<ProjectFullName> m_AllProjectNameList;					// ��������Ŀ���ơ��б�
	CDateTimeCtrl m_ctrlProductDateFrom;							// ���������ڡ���ʼ��������
	CDateTimeCtrl m_ctrlProductDateTo;								// ���������ڡ���ֹ��������
	Standard_Key m_nCurrentSelectedStanderd;						//  ��ǰѡ�б�׼ƷID
	CStatic m_ctrlStdPointTitle[10];								// ����׼�㡱��������
	CStatic m_ctrlStdPointValue[10];								// ����׼�㡱Ũ��ֵ

	HBRUSH m_hCtlBrush;												// �ؼ�������ˢ	
};
