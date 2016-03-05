//*************************************************************************
// 文件名 : AutoDetectorDlg.h
// 
// 概要   : 主对话框类
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-11       崔志雷       新建
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
	// 摘要：
	//		标准构造函数
	// 参数：
	//     1.pParent - 父对话框指针
	// --------------------------------------------------
	CAutoDetectorDlg(CWnd* pParent = NULL);	
	~CAutoDetectorDlg();	

	// --------------------------------------------------
	// 摘要：
	//		对话框资源数据
	// --------------------------------------------------
	enum { IDD = IDD_AutoDetector_Dlg };

	// --------------------------------------------------
	// 摘要：
	//		DDX/DDV 支持
	// --------------------------------------------------
protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	// --------------------------------------------------
	// 摘要：
	//		初始化函数
	// --------------------------------------------------
	virtual BOOL OnInitDialog();

	//		绘制消息响应函数
	// --------------------------------------------------
	afx_msg void OnPaint();

	// --------------------------------------------------
	// 摘要：
	//		当用户拖动最小化窗口时系统调用此函数取得光标显示。
	// --------------------------------------------------
	afx_msg HCURSOR OnQueryDragIcon();

	// --------------------------------------------------
	// 摘要：
	//		便签选项变化响应函数
	// --------------------------------------------------
	afx_msg void OnTcnSelchangeMaintab(NMHDR *pNMHDR, LRESULT *pResult);

	// --------------------------------------------------
	// 摘要：
	//		窗口大小发生变化响应函数
	// --------------------------------------------------
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
protected:
	HICON m_hIcon;								// 对话框图标
	CMainTab m_ctrlMainTab;						// 主标签控件
	map<AppView,CDialog *>	m_mapChildViews;	// 子对话框指针
	AppView m_CurrentView;						// 当前选项索引
public:
};
