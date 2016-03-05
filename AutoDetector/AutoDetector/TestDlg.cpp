// TestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "TestDlg.h"
#include "ReagentAreaManDlg.h"
#include "SampleAreaManDlg.h"
#include "NewTestConfirmDlg.h"
#include "TestMonitorDlg.h"

// CTestDlg �Ի���

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

// CTestDlg ��Ϣ�������

BOOL CTestDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	CRect rect;
	GetClientRect(&rect);
	m_TestManTab.MoveWindow(rect);

	ControlsResizeProp item;

	// ���á���ǩ�ؼ����ġ����������ԡ�
	m_TestManTab.GetClientRect(rect);
	item.ID = IDC_TestMan_Tab;
	item.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������϶���
	item.ctmXofPosBps = tmNotChange;			// ����x���갴�����Ŵ�
	item.ctmYofPosBps = tmNotChange;			// ����y�ᰴ�����Ŵ�
	item.ctmWigth = tmQuantity;					// �ؼ���Ȱ���������
	item.ctmHight = tmQuantity;					// �ؼ����Ȱ���������
	SetResize(item);

	m_TestManTab.InsertItem(ViewReagentArea,_T("�Լ�������"));
	m_TestManTab.InsertItem(ViewSampleArea,_T("����������"));
	m_TestManTab.InsertItem(ViewTestConfirm,_T("�²���ȷ��"));
	m_TestManTab.InsertItem(ViewMonitor,_T("���Խ��ȼ��"));

	
	CRect rcTabs;
	m_TestManTab.GetClientRect(&rcTabs);
	rcTabs.left += m_TestManTab.GetHeaderWidth();

	// �������Լ������á��Ի��򣬲�����Ϊ��ʼ��ʾ״̬
	CReagentAreaManDlg * pReagentAreaManDlg = new CReagentAreaManDlg(&m_TestManTab);
	pReagentAreaManDlg->Create(IDD_ReagentAreaMan_Dlg,&m_TestManTab);
	m_mapChildViews[ViewReagentArea] = pReagentAreaManDlg;
	m_mapChildViews[ViewReagentArea]->MoveWindow(&rcTabs);
	m_mapChildViews[ViewReagentArea]->ShowWindow(SW_SHOW);

	// ���������������á��Ի���
	CSampleAreaManDlg * pSampleAreaManDlg = new CSampleAreaManDlg(&m_TestManTab);
	pSampleAreaManDlg->Create(IDD_SampleAreaMan_Dlg,&m_TestManTab);
	m_mapChildViews[ViewSampleArea] = pSampleAreaManDlg;
	m_mapChildViews[ViewSampleArea]->MoveWindow(&rcTabs);

	// ����������ȷ�ϡ��Ի���
	CNewTestConfirmDlg * pNewTestConfirmDlg = new CNewTestConfirmDlg(&m_TestManTab);
	pNewTestConfirmDlg->Create(IDD_NewTestConfirm_Dlg,&m_TestManTab);
	m_mapChildViews[ViewTestConfirm] = pNewTestConfirmDlg;
	m_mapChildViews[ViewTestConfirm]->MoveWindow(&rcTabs);

	// ���������Լ�ء��Ի���
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
