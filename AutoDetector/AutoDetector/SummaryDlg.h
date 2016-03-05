//*****************************************************************************
// �ļ��� : SummaryDlg.h
// 
// ����   : ��Ҫ�Ի���
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-12       ��־��       �½�
// ------------------------------------------
// ���ܸ�Ҫ��
// ------------------------------------------
// �Ի�������ʾ�������ؼ����������ؼ����Լ����ؼ��ȿؼ���
// ��ʾ������������Ҫ������Ϣ
//*****************************************************************************
#pragma once

#include "SampleAreaCtrl.h"
#include "ReagentAreaCtrl.h"
#include "BaseDialog.h"
#include "IncuAreaCtrl.h"

class CSummaryDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CSummaryDlg)

public:
	// --------------------------------------------------------------
	// ���캯����
	//	   ���Ի�����ʹ��Ĭ��ֵΪNULL����Ϊֻ��Ϊ�ǿչ���ʱ����CBaseDialo���캯����
	//     �������ñ������߶�Ϊ0
	// --------------------------------------------------------------
	CSummaryDlg(CWnd* pParent);
	virtual ~CSummaryDlg();

	enum { IDD = IDD_Summary_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	// --------------------------------------------------
	// ժҪ��
	//	   ������������ֵ�������仯��Ӧ����
	// --------------------------------------------------
	afx_msg LRESULT OnSampleAreaDataUpdate(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	CSampleAreaCtrl m_ctrlSampleArea;			// �������ؼ�
	CReagentAreaCtrl m_ctrlReagentArea;			// �Լ����ؼ�
	CIncuAreaCtrl m_ctrlIncuArea;				// �Լ����ؼ�
};
