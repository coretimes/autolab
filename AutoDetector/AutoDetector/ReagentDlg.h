//*****************************************************************************
// �ļ��� : ReagentDlg.h
// 
// ����   : �Լ�����Ի�����
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
#include "ReagentTable.h"

class CReagentDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CReagentDlg)

public:
	// ---------------------------------------------------------
	// ժҪ��
	//		���캯��
	// ---------------------------------------------------------
	CReagentDlg(CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~CReagentDlg();

// �Ի�������
	enum { IDD = IDD_Reagent_Dlg };

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
	//     ���Լ��б�ѡ����仯����Ϣ��Ӧ
	// ---------------------------------------------------------
	afx_msg void OnLvnItemchangedListReagents(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	// ---------------------------------------------------------
	// ժҪ��
	//     ��ʼ������Ŀ������ơ���Ͽ�
	// ---------------------------------------------------------
	void _InitComboProjectName();

	// ---------------------------------------------------------
	// ժҪ��
	//     ˢ�¡��Լ��б���������������������
	// ---------------------------------------------------------
	void _RefreshReagentsList();

	// ---------------------------------------------------------
	// ժҪ��
	//     ˢ�¡���ϸ��Ϣ����
	// ---------------------------------------------------------
	void _RefreshDetails();

protected:
	CCustomListCtrl m_ctrlReagentList;								// ���������á��б�ؼ�

	CComboBox m_ctrlProjectName;									// �������Ŀ����Ͽ�
	vector<ProjectFullName> m_AllProjectNameList;					// ��������Ŀ���ơ��б�
	CDateTimeCtrl m_ctrlProductDateFrom;							// ���������ڡ���ʼ��������
	CDateTimeCtrl m_ctrlProductDateTo;								// ���������ڡ���ֹ��������
	Reagent_Key m_nCurrentSelectedReagent;							//  ��ǰѡ���Լ�ID

	HBRUSH m_hCtlBrush;												// �ؼ�������ˢ	
};
