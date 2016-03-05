//*****************************************************************************
// �ļ��� : TestMonitorDlg.h
// 
// ����   : ���Լ�ضԻ���
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2014-02       ��־��       �½�
// ------------------------------------------
// ���ܸ�Ҫ��
// ------------------------------------------
//	   ʵʱ��ز���״̬
//*****************************************************************************
#pragma once

#include "BaseDialog.h"
#include "CustomListCtrl.h"
#include <map>

class CTestMonitorDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CTestMonitorDlg)

public:
	// --------------------------------------------------------------
	// ���캯����
	//	   ���Ի�����ʹ��Ĭ��ֵΪNULL����Ϊֻ��Ϊ�ǿչ���ʱ����CBaseDialo���캯����
	//     �������ñ������߶�Ϊ0
	// --------------------------------------------------------------
	CTestMonitorDlg(CWnd* pParent);   // ��׼���캯��
	virtual ~CTestMonitorDlg();
	enum { IDD = IDD_TestMonitor_Dlg };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// --------------------------------------------------
	// ժҪ��
	//	   �����Ի����������ݷ����仯��Ӧ����
	// --------------------------------------------------
	afx_msg LRESULT OnTestBufferUpdate(WPARAM wParam, LPARAM lParam);
	
	// --------------------------------------------------
	// ժҪ��
	//	   ˢ�²��ұ�
	// --------------------------------------------------
	void RefreshSearchMap();

private:
	CCustomListCtrl m_ctrlMornitorList;							// ���Լ���б�
	multimap<TestUnit_ID,int> m_SearchMap;						// ���ݲ��Ե�ԪID����λ�б���Ĳ��ұ�<���Ե�ԪID���б�������>
};
