//*************************************************************************
// �ļ��� : AutoDetectorDlg.h
// 
// ��Ҫ   : ���Ի�����
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-11       ��־��       �½�
// ------------------------------------------
//*************************************************************************
#pragma once
#include "MainTab.h"
#include "BaseDialog.h"

#include <map>
using namespace std;

enum AppView
{
	ViewSummary = 0,
	ViewTests,
	ViewRegents,
	ViewStanderds,
	ViewCurve,
	ViewSamples,
	ViewAnalysis,
	viewMaintence,
	NumViews
};

class CAutoDetectorDlg : public CBaseDialog
{
public:
	// --------------------------------------------------
	// ժҪ��
	//		��׼���캯��
	// ������
	//     1.pParent - ���Ի���ָ��
	// --------------------------------------------------
	CAutoDetectorDlg(CWnd* pParent = NULL);	
	~CAutoDetectorDlg();	

	// --------------------------------------------------
	// ժҪ��
	//		�Ի�����Դ����
	// --------------------------------------------------
	enum { IDD = IDD_AutoDetector_Dlg };

	// --------------------------------------------------
	// ժҪ��
	//		DDX/DDV ֧��
	// --------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	// --------------------------------------------------
	// ժҪ��
	//		��ʼ������
	// --------------------------------------------------
	virtual BOOL OnInitDialog();

	//		������Ϣ��Ӧ����
	// --------------------------------------------------
	afx_msg void OnPaint();

	// --------------------------------------------------
	// ժҪ��
	//		���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
	// --------------------------------------------------
	afx_msg HCURSOR OnQueryDragIcon();

	// --------------------------------------------------
	// ժҪ��
	//		��ǩѡ��仯��Ӧ����
	// --------------------------------------------------
	afx_msg void OnTcnSelchangeMaintab(NMHDR *pNMHDR, LRESULT *pResult);

	// --------------------------------------------------
	// ժҪ��
	//		���ڴ�С�����仯��Ӧ����
	// --------------------------------------------------
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
protected:
	HICON m_hIcon;								// �Ի���ͼ��
	CMainTab m_ctrlMainTab;						// ����ǩ�ؼ�
	map<AppView,CDialog *>	m_mapChildViews;	// �ӶԻ���ָ��
	AppView m_CurrentView;						// ��ǰѡ������
public:
};
