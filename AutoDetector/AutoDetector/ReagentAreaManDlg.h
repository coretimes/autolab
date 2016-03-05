//*****************************************************************************
// �ļ��� : ReagentAreaManDlg.h
// 
// ����   : �Լ������öԻ���
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2014-1       ��־��       �½�
// ------------------------------------------
// ���ܸ�Ҫ��
// ------------------------------------------
// 
//*****************************************************************************

#pragma once

#include "BaseDialog.h"
#include "ReagentAreaCtrl.h"
#include "CustomListCtrl.h"

class CReagentAreaManDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CReagentAreaManDlg)

public:
	// --------------------------------------------------------------
	// ���캯����
	//	   ���Ի�����ʹ��Ĭ��ֵΪNULL����Ϊֻ��Ϊ�ǿչ���ʱ����CBaseDialo���캯����
	//     �������ñ������߶�Ϊ0
	// --------------------------------------------------------------
	CReagentAreaManDlg(CWnd* pParent);   // ��׼���캯��
	virtual ~CReagentAreaManDlg();
	enum { IDD = IDD_ReagentAreaMan_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnReagentAreaDataUpdate(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

private:
	CCustomListCtrl m_ctrlReagentList;								// ���������б�ؼ�
	CReagentAreaCtrl m_ctrlReagentArea;
};
