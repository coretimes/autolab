//*****************************************************************************
// �ļ��� : TestDlg.h
// 
// ����   : ���Թ���Ի���
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
#include "SampleAreaCtrl.h"
#include "BaseDialog.h"
#include "TestManTab.h"

class CTestDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent);   // ��׼���캯��
	virtual ~CTestDlg();
	enum { IDD = IDD_Test_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ---------------------------------------------------------
	// ժҪ��
	//     ����ǩ�ѡ��仯��Ӧ
	// ---------------------------------------------------------
	afx_msg void OnTcnSelchangeTestmanTab(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CTestManTab m_TestManTab;
	map<TabView,CDialog *>	m_mapChildViews;	// �ӶԻ���ָ��
};
