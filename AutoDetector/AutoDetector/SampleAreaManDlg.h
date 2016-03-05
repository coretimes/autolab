//*****************************************************************************
// �ļ��� : SampleAreaManDlg.h
// 
// ����   : ���������öԻ���
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
#include "CustomListCtrl.h"

// �б�����ӳ�ʱ���ʱ��
// ˵����������ڡ��������ؼ����ƶ�ʱ������һֱ�����仯���б�ؼ��Ͻ���Ҳ��Ӧ�ڱ仯��
//       ��ʱ�������ƶ�����������������˸�����ԣ��������ƶ��ӳ�300�����ӡ�
#define  TID_ListScroll_Delay			00001

class CSampleAreaManDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CSampleAreaManDlg)

public:
	// --------------------------------------------------------------
	// ���캯����
	//	   ���Ի�����ʹ��Ĭ��ֵΪNULL����Ϊֻ��Ϊ�ǿչ���ʱ����CBaseDialo���캯����
	//     �������ñ������߶�Ϊ0
	// --------------------------------------------------------------
	CSampleAreaManDlg(CWnd* pParent);   // ��׼���캯��
	virtual ~CSampleAreaManDlg();

	enum { IDD = IDD_Test_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	// --------------------------------------------------
	// ժҪ��
	//	   ������������״̬�������仯��Ӧ����
	// --------------------------------------------------
	afx_msg LRESULT OnSampleAreaStateChange(WPARAM wParam, LPARAM lParam);

	// --------------------------------------------------
	// ժҪ��
	//	   ������������ֵ�������仯��Ӧ����
	// --------------------------------------------------
	afx_msg LRESULT OnSampleAreaDataUpdate(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	CSampleAreaCtrl m_ctrlSampleArea;					// �������ؼ�
	CCustomListCtrl m_ctrlSampleList;					// �����������б�ؼ�
	
	Sample_Pos m_UpdatePos;								// �����ı������λ
};
