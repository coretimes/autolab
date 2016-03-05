//*****************************************************************************
// �ļ��� : NewTestConfirmDlg.h
// 
// ����   : �²���ȷ�϶Ի���
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2014-02       ��־��       �½�
// ------------------------------------------
// ���ܸ�Ҫ��
// ------------------------------------------
//	   ������ӵĲ��Խ������򣬲�ȷ�Ϻ��·�
//*****************************************************************************

#pragma once

#include "BaseDialog.h"
#include "CustomListCtrl.h"

class CNewTestConfirmDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CNewTestConfirmDlg)

public:
	// --------------------------------------------------------------
	// ���캯����
	//	   ���Ի�����ʹ��Ĭ��ֵΪNULL����Ϊֻ��Ϊ�ǿչ���ʱ����CBaseDialo���캯����
	//     �������ñ������߶�Ϊ0
	// --------------------------------------------------------------
	CNewTestConfirmDlg(CWnd* pParent);   // ��׼���캯��
	virtual ~CNewTestConfirmDlg();
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_NewTestConfirm_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// --------------------------------------------------------------
	//	�����뵽�˽���ʱ������ȡ����������������
	// --------------------------------------------------------------
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	// --------------------------------------------------------------
	//	����ʼ����ť���������������ݷ����������Ի�����������ʼ����
	// --------------------------------------------------------------
	afx_msg void OnBnClickedStart();

private:
	CCustomListCtrl m_ctrlNewTestList;					// �����������б�ؼ�
	CButton m_StartButton;								// ��ʼ��ť
};
