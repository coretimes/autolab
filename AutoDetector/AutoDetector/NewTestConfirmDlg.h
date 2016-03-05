//*****************************************************************************
// 文件名 : NewTestConfirmDlg.h
// 
// 类名   : 新测试确认对话框
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2014-02       崔志雷       新建
// ------------------------------------------
// 功能概要：
// ------------------------------------------
//	   对新添加的测试进行排序，并确认和下发
//*****************************************************************************

#pragma once

#include "BaseDialog.h"
#include "CustomListCtrl.h"

class CNewTestConfirmDlg : public CBaseDialog
{
	DECLARE_DYNAMIC(CNewTestConfirmDlg)

public:
	// --------------------------------------------------------------
	// 构造函数：
	//	   父对话框不能使用默认值为NULL，因为只有为非空构造时，在CBaseDialo构造函数中
	//     才能设置标题栏高度为0
	// --------------------------------------------------------------
	CNewTestConfirmDlg(CWnd* pParent);   // 标准构造函数
	virtual ~CNewTestConfirmDlg();
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_NewTestConfirm_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// --------------------------------------------------------------
	//	当进入到此界面时，重新取得样本新配置数据
	// --------------------------------------------------------------
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	// --------------------------------------------------------------
	//	“开始”按钮单击，把配置数据发送至“测试缓冲区”，开始测试
	// --------------------------------------------------------------
	afx_msg void OnBnClickedStart();

private:
	CCustomListCtrl m_ctrlNewTestList;					// 样本区数据列表控件
	CButton m_StartButton;								// 开始按钮
};
