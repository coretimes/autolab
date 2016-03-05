// AutoDetectorDlg.cpp : 实现文件

#include "stdafx.h"
#include "AutoDetector.h"
#include "AutoDetectorDlg.h"
#include "SummaryDlg.h"
#include "TestDlg.h"
#include "ReagentDlg.h"
#include "TestDlg.h"
#include "SampleDlg.h"
#include "StanderdDlg.h"
#include "MaintainDlg.h"
#include "AnalysisDlg.h"
#include "AutoDetectorSkin.h"
#include "CurveDlg.h"

#include "CurveManagement.h"
#include "CurveTable.h"
#include "DBConnect.h"
#include "FontLib.h"
#include "GeneralFuction.h"
#include "MeteringManagement.h"
#include "ReagentAreaData.h"
#include "ReagentTable.h"
#include "ResultTable.h"
#include "ReagentTable.h"
#include "SampleAreaData.h"
#include "SampleTable.h"
#include "StandardTable.h"
#include "TestManagement.h"
#include "UserTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAutoDetectorDlg::CAutoDetectorDlg(CWnd* pParent /*=NULL*/):CBaseDialog(CAutoDetectorDlg::IDD, pParent)
{
	// 加载主窗口图标
	m_hIcon = AfxGetApp()->LoadIcon(IDC_MainFrame);
	m_pSkin = new CAutoDetectorSkin(this);
	m_BorderWidth = 4;
	m_BorderHeight = 4;
	m_CaptionHeight = 66;
}

CAutoDetectorDlg::~CAutoDetectorDlg()
{
	delete m_pSkin;
	map<AppView,CDialog *>::iterator iter = m_mapChildViews.begin();
	while(iter != m_mapChildViews.end())
	{
		delete iter->second;
		iter++;
	}
}

void CAutoDetectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_MainTab,m_ctrlMainTab);
}

BEGIN_MESSAGE_MAP(CAutoDetectorDlg, CBaseDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MainTab, &CAutoDetectorDlg::OnTcnSelchangeMaintab)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


BOOL CAutoDetectorDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// 设置本对话框皮肤ID，便于在皮肤动态库中识别
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 插入标签项
	m_ctrlMainTab.InsertItem(ViewSummary,_T("概况"));
	m_ctrlMainTab.InsertItem(ViewTests,_T("测试"));
	m_ctrlMainTab.InsertItem(ViewRegents,_T("试剂"));
	m_ctrlMainTab.InsertItem(ViewStanderds,_T("标品"));
	m_ctrlMainTab.InsertItem(ViewCurve,_T("标曲"));
	m_ctrlMainTab.InsertItem(ViewSamples,_T("样本"));
	m_ctrlMainTab.InsertItem(viewMaintence,_T("维护"));
	m_ctrlMainTab.InsertItem(ViewAnalysis,_T("分析"));

	CRect rect;
	GetClientRect(rect);
	m_ctrlMainTab.MoveWindow(rect,FALSE);

	ControlsResizeProp item;
	// 设置“标签控件”的“可伸缩属性”
	m_ctrlMainTab.GetClientRect(rect);
	item.ID = IDC_MainTab;
	item.ptOrigPosBps = rect.TopLeft();			// 重定位基点是左上顶点
	item.ctmWigth = tmQuantity;					// 控件宽度定量增加
	item.ctmHight = tmQuantity;					// 控件长度定量增加
	SetResize(item);

	CRect rcTabs;
 	m_ctrlMainTab.GetClientRect(&rcTabs);
	rcTabs.top += m_ctrlMainTab.GetHeaderHeight() - 4;
 
	// 创建“概况”对话框，并设置为初始显示状态,注：构造函数去掉了父对话框指针为NULL的默认值
	CSummaryDlg * pSummaryDlg = new CSummaryDlg(&m_ctrlMainTab);
	pSummaryDlg->Create(IDD_Summary_Dlg,&m_ctrlMainTab);
	m_mapChildViews[ViewSummary] = pSummaryDlg;
	m_mapChildViews[ViewSummary]->ShowWindow(TRUE);
	m_mapChildViews[ViewSummary]->MoveWindow(&rcTabs);
	m_CurrentView = ViewSummary;

	// 创建“测试”对话框
	CTestDlg * pTestDlg = new CTestDlg(&m_ctrlMainTab);
	pTestDlg->Create(IDD_Test_Dlg,&m_ctrlMainTab);
	m_mapChildViews[ViewTests] = pTestDlg;
	m_mapChildViews[ViewTests]->MoveWindow(&rcTabs);

	// 创建“试剂”对话框
	CReagentDlg * pReagentDlg = new CReagentDlg(&m_ctrlMainTab);
	pReagentDlg->Create(IDD_Reagent_Dlg,&m_ctrlMainTab);
	m_mapChildViews[ViewRegents] = pReagentDlg;
	m_mapChildViews[ViewRegents]->MoveWindow(&rcTabs);

	// 创建“样本”对话框
	CSampleDlg * pSampleDlg = new CSampleDlg(&m_ctrlMainTab);
	pSampleDlg->Create(IDD_Sample_Dlg,&m_ctrlMainTab);
	m_mapChildViews[ViewSamples] = pSampleDlg;
	m_mapChildViews[ViewSamples]->MoveWindow(&rcTabs);

	// 创建“标准品”对话框
	CStanderdDlg  * pStanderdDlg = new CStanderdDlg(&m_ctrlMainTab);
	pStanderdDlg->Create(IDD_Standerd_Dlg,&m_ctrlMainTab);
	m_mapChildViews[ViewStanderds] = pStanderdDlg;
	m_mapChildViews[ViewStanderds]->MoveWindow(&rcTabs);

	// 创建“标准曲线”对话框
	CCurveDlg  * pCurveDlg = new CCurveDlg(&m_ctrlMainTab);
	pCurveDlg->Create(IDD_Curve_Dlg,&m_ctrlMainTab);
	m_mapChildViews[ViewCurve] = pCurveDlg;
	m_mapChildViews[ViewCurve]->MoveWindow(&rcTabs);

	// 创建“维护”对话框
	CMaintainDlg * pMaintainDlg = new CMaintainDlg;
	pMaintainDlg->Create(IDD_Maintain_Dlg,&m_ctrlMainTab);
	m_mapChildViews[viewMaintence] = pMaintainDlg;
	m_mapChildViews[viewMaintence]->MoveWindow(&rcTabs);

	// 创建“分析”对话框
	CAnalysisDlg * pAnalysisDlg = new CAnalysisDlg;
	pAnalysisDlg->Create(IDD_Analysis_Dlg,&m_ctrlMainTab);
	m_mapChildViews[ViewAnalysis] = pAnalysisDlg;
	m_mapChildViews[ViewAnalysis]->MoveWindow(&rcTabs);

	return TRUE;					// 除非将焦点设置到控件，否则返回 TRUE
}

void CAutoDetectorDlg::OnPaint()
{
	// 如果窗口是最小化状态，则
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CBrush brWhiteBrush(RGB(255,255,255));
		dc.FillRgn(&m_rgnBackGround,&brWhiteBrush);
	}
}

HCURSOR CAutoDetectorDlg::OnQueryDragIcon()
{
	// 当用户拖动最小化窗口时系统调用此函数取得光标显示。
	return static_cast<HCURSOR>(m_hIcon);
}

void CAutoDetectorDlg::OnTcnSelchangeMaintab(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_CurrentView = static_cast<AppView>(m_ctrlMainTab.GetCurSel());
	for(int index = ViewSummary; index < NumViews; index ++)
	{
		AppView indexView = static_cast<AppView>(index);
		if(indexView == m_CurrentView)
		{
			m_mapChildViews[indexView]->ShowWindow(TRUE);
		}
 		else
			m_mapChildViews[indexView]->ShowWindow(FALSE);
	}

	*pResult = 0;
}

void CAutoDetectorDlg::OnSize(UINT nType, int cx, int cy)
{
	CBaseDialog::OnSize(nType, cx, cy);

	if (IsWindow(m_ctrlMainTab.GetSafeHwnd()))
	{
		CRect rcTabClient = m_ctrlMainTab.GetClienRect();
		rcTabClient.top += m_ctrlMainTab.GetHeaderHeight() - 4;
		for(int index = ViewSummary; index < NumViews; index ++)
		{
			AppView indexView = static_cast<AppView>(index);
			m_mapChildViews[indexView]->MoveWindow(&rcTabClient);
		}
	}
}

void CAutoDetectorDlg::OnClose()
{
	CCurveManagement::DestroyInstance();
	CCurveTable::DestroyInstance();
	CDBConnect::DestroyInstance();
	CFontsLib::DestroyInstance();
	CGeneralFuction::DestroyInstance();
	CMeteringManagement::DestroyInstance();
	CReagentAreaData::DestroyInstance();
	CReagentTable::DestroyInstance();
	CResultTable::DestroyInstance();
	CSampleAreaData::DestroyInstance();
	CSampleTable::DestroyInstance();
	CStandardTable::DestroyInstance();
	CTestManagement::DestroyInstance();
	CUserTable::DestroyInstance();

	CBaseDialog::OnClose();
}

BOOL CAutoDetectorDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}