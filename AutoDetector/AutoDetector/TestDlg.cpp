// TestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "TestDlg.h"
#include "ReagentAreaManDlg.h"
#include "SampleAreaManDlg.h"
#include "NewTestConfirmDlg.h"
#include "TestMonitorDlg.h"

// CTestDlg 对话框

IMPLEMENT_DYNAMIC(CTestDlg, CBaseDialog)

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CTestDlg::IDD, pParent)
{

}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_TestMan_Tab,m_TestManTab);
}


BEGIN_MESSAGE_MAP(CTestDlg, CBaseDialog)
	ON_WM_PAINT()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TestMan_Tab, &CTestDlg::OnTcnSelchangeTestmanTab)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CTestDlg 消息处理程序

BOOL CTestDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	CRect rect;
	GetClientRect(&rect);
	m_TestManTab.MoveWindow(rect);

	ControlsResizeProp item;

	// 设置“标签控件”的“可伸缩属性”
	m_TestManTab.GetClientRect(rect);
	item.ID = IDC_TestMan_Tab;
	item.ptOrigPosBps = rect.TopLeft();			// 重定位基点是左上顶点
	item.ctmXofPosBps = tmNotChange;			// 基点x坐标按比例放大
	item.ctmYofPosBps = tmNotChange;			// 基点y轴按比例放大
	item.ctmWigth = tmQuantity;					// 控件宽度按比例缩放
	item.ctmHight = tmQuantity;					// 控件长度按比例缩放
	SetResize(item);

	m_TestManTab.InsertItem(ViewReagentArea,_T("试剂区配置"));
	m_TestManTab.InsertItem(ViewSampleArea,_T("样本区配置"));
	m_TestManTab.InsertItem(ViewTestConfirm,_T("新测试确认"));
	m_TestManTab.InsertItem(ViewMonitor,_T("测试进度监控"));

	
	CRect rcTabs;
	m_TestManTab.GetClientRect(&rcTabs);
	rcTabs.left += m_TestManTab.GetHeaderWidth();

	// 创建“试剂区配置”对话框，并设置为初始显示状态
	CReagentAreaManDlg * pReagentAreaManDlg = new CReagentAreaManDlg(&m_TestManTab);
	pReagentAreaManDlg->Create(IDD_ReagentAreaMan_Dlg,&m_TestManTab);
	m_mapChildViews[ViewReagentArea] = pReagentAreaManDlg;
	m_mapChildViews[ViewReagentArea]->MoveWindow(&rcTabs);
	m_mapChildViews[ViewReagentArea]->ShowWindow(SW_SHOW);

	// 创建“样本区配置”对话框
	CSampleAreaManDlg * pSampleAreaManDlg = new CSampleAreaManDlg(&m_TestManTab);
	pSampleAreaManDlg->Create(IDD_SampleAreaMan_Dlg,&m_TestManTab);
	m_mapChildViews[ViewSampleArea] = pSampleAreaManDlg;
	m_mapChildViews[ViewSampleArea]->MoveWindow(&rcTabs);

	// 创建“测试确认”对话框
	CNewTestConfirmDlg * pNewTestConfirmDlg = new CNewTestConfirmDlg(&m_TestManTab);
	pNewTestConfirmDlg->Create(IDD_NewTestConfirm_Dlg,&m_TestManTab);
	m_mapChildViews[ViewTestConfirm] = pNewTestConfirmDlg;
	m_mapChildViews[ViewTestConfirm]->MoveWindow(&rcTabs);

	// 创建“测试监控”对话框
	CTestMonitorDlg * pTestMonitorDlg = new CTestMonitorDlg(&m_TestManTab);
	pTestMonitorDlg->Create(IDD_TestMonitor_Dlg,&m_TestManTab);
	m_mapChildViews[ViewMonitor] = pTestMonitorDlg;
	m_mapChildViews[ViewMonitor]->MoveWindow(&rcTabs);
	return TRUE;
}

void CTestDlg::OnPaint()
{
	CPaintDC dc(this);
	CBrush brWhiteBackGround(RGB(255,255,255));
	dc.FillRgn(&m_rgnBackGround,&brWhiteBackGround);
}

void CTestDlg::OnTcnSelchangeTestmanTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	TabView CurrentView = static_cast<TabView>(m_TestManTab.GetCurSel());
	for(int index = ViewReagentArea; index < NumTabs; index ++)
	{
		TabView indexView = static_cast<TabView>(index);
		if(indexView == CurrentView)
		{
			m_mapChildViews[indexView]->ShowWindow(TRUE);
		}
		else
			m_mapChildViews[indexView]->ShowWindow(FALSE);
	}

	*pResult = 0;
}

void CTestDlg::OnSize(UINT nType, int cx, int cy)
{
	CBaseDialog::OnSize(nType, cx, cy);

	if (IsWindow(m_TestManTab.GetSafeHwnd()))
	{
		CRect rcTabClient;
		m_TestManTab.GetClientRect(&rcTabClient);
		rcTabClient.left += m_TestManTab.GetHeaderWidth();
		for(int index = ViewReagentArea; index < NumTabs; index ++)
		{
			TabView indexView = static_cast<TabView>(index);
			m_mapChildViews[indexView]->MoveWindow(&rcTabClient);
		}
	}
}

void CTestDlg::OnDestroy()
{
	CBaseDialog::OnDestroy();

	map<TabView,CDialog *>::iterator iter = m_mapChildViews.begin();
	while(iter != m_mapChildViews.end())
	{
		iter->second->DestroyWindow();
		delete iter->second;
		iter++;
	}
}
