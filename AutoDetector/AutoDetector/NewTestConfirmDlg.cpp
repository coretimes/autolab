// NewTestConfirmDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "NewTestConfirmDlg.h"
#include "SampleAreaData.h"
#include "TestConfirmListItem.h"
#include "TestManTab.h"

IMPLEMENT_DYNAMIC(CNewTestConfirmDlg, CBaseDialog)

CNewTestConfirmDlg::CNewTestConfirmDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CNewTestConfirmDlg::IDD, pParent)
{

}

CNewTestConfirmDlg::~CNewTestConfirmDlg()
{
}

void CNewTestConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_List_TestConfirm,m_ctrlNewTestList);
	DDX_Control(pDX,IDC_Start,m_StartButton);
}


BEGIN_MESSAGE_MAP(CNewTestConfirmDlg, CBaseDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_Start, &CNewTestConfirmDlg::OnBnClickedStart)
END_MESSAGE_MAP()

// CNewTestConfirmDlg 消息处理程序

void CNewTestConfirmDlg::OnPaint()
{
	CPaintDC dc(this);
	CBrush brWhiteBackGround(RGB(255,255,255));
	dc.FillRgn(&m_rgnBackGround,&brWhiteBackGround);
}

BOOL CNewTestConfirmDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CNewTestConfirmDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// 设置“试剂区列表”的“可伸缩属性”
	ControlsResizeProp itemList;
	CRect rect;
	m_ctrlNewTestList.GetWindowRect(rect);
	ScreenToClient(rect);
	itemList.ID = IDC_List_TestConfirm;
	itemList.ptOrigPosBps = rect.TopLeft();			// 重定位基点是左下顶点
	itemList.ctmXofPosBps = tmProportion;			// 基点x坐标按比例放大
	itemList.ctmYofPosBps = tmProportion;			// 基点y轴按比例放大
	itemList.ctmWigth = tmProportion;				// 控件宽度按比例缩放
	itemList.ctmHight = tmProportion;				// 控件长度按比例缩放
	SetResize(itemList);

	// 设置“开始按钮”的“可伸缩属性”
	ControlsResizeProp itemStartButton;
	m_StartButton.GetWindowRect(rect);
	ScreenToClient(rect);
	itemStartButton.ID = IDC_Start;
	itemStartButton.ptOrigPosBps = rect.TopLeft();			// 重定位基点是左下顶点
	itemStartButton.ctmXofPosBps = tmProportion;			// 基点x坐标按比例放大
	itemStartButton.ctmYofPosBps = tmProportion;			// 基点y轴按比例放大
	itemStartButton.ctmWigth = tmProportion;				// 控件宽度不变
	itemStartButton.ctmHight = tmProportion;				// 控件长度不变
	SetResize(itemStartButton);

	// 测试配置列表初始化
	m_ctrlNewTestList.InsertColumn(colType,_T("样品类型"),LVCFMT_CENTER);
	m_ctrlNewTestList.InsertColumn(colSampleNo,_T("样本编号"),LVCFMT_CENTER);
	m_ctrlNewTestList.InsertColumn(colConcPoint,_T("浓度点"),LVCFMT_CENTER);
	m_ctrlNewTestList.InsertColumn(colSamplePos,_T("样本位"),LVCFMT_CENTER);
	m_ctrlNewTestList.InsertColumn(colProject,_T("检测项目"),LVCFMT_CENTER);
	m_ctrlNewTestList.InsertColumn(colRepeatTimes,_T("重复次数"),LVCFMT_CENTER);
	
	ColumnProperty column;

	// "样品类型"列宽属性为控件总宽的0.15，最小宽度80
	column.ColumnIndex = 0;
	column.Width = 80;
	column.WidthRatio = 0.15;
	m_ctrlNewTestList.SetColumnWidthProp(column);	

	// "样本编号"列宽为控件总宽的0.25，最小宽度100
	column.ColumnIndex = 1;
	column.Width = 100;
	column.WidthRatio = 0.2;
	m_ctrlNewTestList.SetColumnWidthProp(column);

	// "浓度点"列宽为控件总宽的0.2，最小宽度80
	column.ColumnIndex = 2;
	column.Width = 80;
	column.WidthRatio = 0.15;
	m_ctrlNewTestList.SetColumnWidthProp(column);

	// "样本位"列宽属性为自动标头宽度
	column.ColumnIndex = 3;
	column.Width = 60;
	column.WidthRatio = 0.15;
	m_ctrlNewTestList.SetColumnWidthProp(column);

	// "检测项目"列宽为控件总宽的0.3，最小宽度140
	column.ColumnIndex = 4;
	column.Width = 140;
	column.WidthRatio = 0.3;
	m_ctrlNewTestList.SetColumnWidthProp(column);

	// "重复次数"列宽属性为自动标头宽度
	column.ColumnIndex = 5;
	column.Width = 0;
	m_ctrlNewTestList.SetColumnWidthProp(column);

	m_ctrlNewTestList.SetItemHeight(25);
	m_ctrlNewTestList.EnableSelect(FALSE);
	m_ctrlNewTestList.SetFocus();

	CSampleAreaData::GetInstance()->AttchDialog(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CNewTestConfirmDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (bShow == TRUE)
	{
		CSampleAreaData *pData = CSampleAreaData::GetInstance();
		m_ctrlNewTestList.DeleteAllItemsAndDatas();
		m_ctrlNewTestList.SetRedraw(FALSE);
		for(int pos = 1; pos<= NUM_SAMPLEPOS; pos++)
		{
			// 取得样本配置节点
			SampleConfigNode *pNode = pData->GetNode(pos);
			// 如果没有试管，则不往列表中插入数据
			if(pNode->IsThereTube == false)
				continue;

			// 如果没有配置样本或样本没有测试项目则
			if(pNode->IsSetingSample == false)
				continue;

			if(pNode->TestProjArr.size() < 1)
				continue;

			// 如果样本有测试项目则
			// 循环遍历待测项目数组
			map<Reagent_Pos,SampleConfigNode::TestProjInfo>::iterator iter = pNode->TestProjArr.begin();
			while(iter != pNode->TestProjArr.end())
			{
				if(iter->second.TestUintKeys == 0) // 关联进行中的测试单元key，如果测试未开始，则此key值为0，否则为非0	
				{
					CTestConfirmListItem * pNewItem = new CTestConfirmListItem(pNode->PosNum,iter->first);
					m_ctrlNewTestList.AddItem(0,pNewItem);
				}
				iter++;
			}
		}
		m_ctrlNewTestList.Reorder();
		m_ctrlNewTestList.SetRedraw(TRUE);

		// 如果列表中有数据，则开始按钮变为有效
		if(m_ctrlNewTestList.GetItemCount() > 0)
			m_StartButton.EnableWindow(TRUE);
	}

	CBaseDialog::OnShowWindow(bShow, nStatus);
}

void CNewTestConfirmDlg::OnBnClickedStart()
{
	// 光标编程沙漏形状
	CWaitCursor wait;

	// 更新测试缓冲区
	CSampleAreaData::GetInstance()->UpdateTestBuffer();

	// 删除确认列表中数据
	m_ctrlNewTestList.DeleteAllItemsAndDatas();

	// “开始”按钮变为无效
	m_StartButton.EnableWindow(FALSE);

	// 发送WM_NOTIFY消息，切换至“测试监控”界面
	CTabCtrl * pParentTabCtrl = static_cast<CTabCtrl*>(GetParent());
	pParentTabCtrl->SetCurSel(ViewMonitor);
	NMHDR nmhdr ;
	nmhdr.code = TCN_SELCHANGE;
	nmhdr.hwndFrom = pParentTabCtrl->GetSafeHwnd();
	nmhdr.idFrom = IDC_TestMan_Tab;
	pParentTabCtrl->GetParent()->SendMessage(WM_NOTIFY,(WPARAM)(pParentTabCtrl->GetSafeHwnd()),(LPARAM)&nmhdr);
	wait.Restore();
}