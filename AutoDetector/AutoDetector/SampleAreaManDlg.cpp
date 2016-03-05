// SampleAreaManDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "SampleAreaManDlg.h"
#include "SampleAreaData.h"
#include "SampleAreaListItem.h"

// CSampleAreaManDlg 对话框

IMPLEMENT_DYNAMIC(CSampleAreaManDlg, CBaseDialog)

CSampleAreaManDlg::CSampleAreaManDlg(CWnd* pParent /*=NULL*/)
: CBaseDialog(CSampleAreaManDlg::IDD, pParent), 
  m_ctrlSampleArea(saForConfig)
{

}

CSampleAreaManDlg::~CSampleAreaManDlg()
{
}

void CSampleAreaManDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_Ctrl_SampleArea,m_ctrlSampleArea);
	DDX_Control(pDX,IDC_LIST_SampleArea,m_ctrlSampleList);
}


BEGIN_MESSAGE_MAP(CSampleAreaManDlg, CBaseDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(UM_SampleArea_StateChange, &CSampleAreaManDlg::OnSampleAreaStateChange)
	ON_MESSAGE(UM_SampleArea_DataUpdate, &CSampleAreaManDlg::OnSampleAreaDataUpdate)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSampleAreaManDlg 消息处理程序

BOOL CSampleAreaManDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	CRect rect;
	ControlsResizeProp item;
	// 设置“样本区控件”的“可伸缩属性”
	m_ctrlSampleArea.GetClientRect(rect);
	item.ID = IDC_Ctrl_SampleArea;
	item.ptOrigPosBps = rect.TopLeft();			// 重定位基点是左上顶点
	item.ctmXofPosBps = tmProportion;			// 基点x坐标按比例放大
	item.ctmYofPosBps = tmProportion;			// 基点y轴按比例放大
	item.ctmWigth = tmProportion;				// 控件宽度按比例缩放
	item.ctmHight = tmProportion;				// 控件长度按比例缩放
	SetResize(item);

	// 设置“试剂区列表”的“可伸缩属性”
	ControlsResizeProp itemList;
	m_ctrlSampleList.GetWindowRect(rect);
	ScreenToClient(rect);
	itemList.ID = IDC_LIST_SampleArea;
	itemList.ptOrigPosBps = rect.TopLeft();			// 重定位基点是左下顶点
	itemList.ctmXofPosBps = tmProportion;			// 基点x坐标按比例放大
	itemList.ctmYofPosBps = tmProportion;			// 基点y轴按比例放大
	itemList.ctmWigth = tmProportion;				// 控件宽度按比例缩放
	itemList.ctmHight = tmProportion;				// 控件长度按比例缩放
	SetResize(itemList);

	// 测试配置列表初始化
	m_ctrlSampleList.InsertColumn(0,_T("坐标"),LVCFMT_CENTER);
	m_ctrlSampleList.InsertColumn(1,_T("样本类型"),LVCFMT_CENTER);
	m_ctrlSampleList.InsertColumn(2,_T("样本编号"),LVCFMT_CENTER);
	m_ctrlSampleList.InsertColumn(3,_T("检测项目"),LVCFMT_CENTER);
	m_ctrlSampleList.InsertColumn(4,_T("重复次数"),LVCFMT_CENTER);
	m_ctrlSampleList.InsertColumn(5,_T("测试状态"),LVCFMT_CENTER);

	ColumnProperty column;

	// "坐标"列宽属性为自动标头宽度
	column.ColumnIndex = 0;
	column.Width = 0;
	column.WidthRatio = 0;
	m_ctrlSampleList.SetColumnWidthProp(column);	

	// "样本类型"列宽为标头宽度
	column.ColumnIndex = 1;
	column.Width = 0;
	m_ctrlSampleList.SetColumnWidthProp(column);

	// "样本编号"列宽为控件总宽的0.25，最小宽度100
	column.ColumnIndex = 2;
	column.Width = 100;
	column.WidthRatio = 0.2;
	m_ctrlSampleList.SetColumnWidthProp(column);

	// "检测项目"列宽为控件总宽的0.45，最小宽度140
	column.ColumnIndex = 3;
	column.Width = 140;
	column.WidthRatio = 0.3;
	m_ctrlSampleList.SetColumnWidthProp(column);

	// "重复次数"列宽为标头宽度
	column.ColumnIndex = 4;
	column.Width = 0;
	m_ctrlSampleList.SetColumnWidthProp(column);

	// "测试状态"列宽属性为自动标头宽度
	column.ColumnIndex = 5;
	column.Width = 0;
	m_ctrlSampleList.SetColumnWidthProp(column);

	m_ctrlSampleList.SetItemHeight(25);
	m_ctrlSampleList.EnableSelect(FALSE);
	m_ctrlSampleList.SetFocus();

	CSampleAreaData::GetInstance()->AttchDialog(this);

	return TRUE;
}

void CSampleAreaManDlg::OnPaint()
{
	CPaintDC dc(this);
	CBrush brWhiteBackGround(RGB(255,255,255));
	dc.FillRgn(&m_rgnBackGround,&brWhiteBackGround);
}

BOOL CSampleAreaManDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

LRESULT CSampleAreaManDlg::OnSampleAreaStateChange(WPARAM wParam, LPARAM lParam)
{
	// 样本区某位置数据发生变化响应函数

	// 找到该位置，并使该位置数据重绘
	m_UpdatePos = static_cast<Sample_Pos>(wParam);
	m_ctrlSampleArea.InvalidNode(m_UpdatePos);
	KillTimer(TID_ListScroll_Delay);
	SetTimer(TID_ListScroll_Delay,300,NULL);
	return 0;
}

LRESULT CSampleAreaManDlg::OnSampleAreaDataUpdate(WPARAM wParam, LPARAM lParam)
{
	CSampleAreaData *pData = CSampleAreaData::GetInstance();
	m_ctrlSampleList.SetRedraw(FALSE);
	m_ctrlSampleList.DeleteAllItemsAndDatas();
	for(int pos = 1; pos<= NUM_SAMPLEPOS; pos++)
	{
		// 取得样本配置节点
		SampleConfigNode *pNode = pData->GetNode(pos);
		// 如果没有试管，则不往列表中插入数据
		if(pNode->IsThereTube == false)
			continue;

		// 如果没有配置样本或样本没有测试项目则
		if(pNode->IsSetingSample == false || pNode->TestProjArr.size() < 1)
		{
			CSampleAreaListItem * pNewItem = new CSampleAreaListItem(pNode->PosNum,0);
			m_ctrlSampleList.AddItem(0,pNewItem);
		}
		// 如果样本有测试项目则
		else
		{
			// 循环遍历待测项目数组
			map<Reagent_Pos,SampleConfigNode::TestProjInfo>::iterator iter = pNode->TestProjArr.begin();
			while(iter != pNode->TestProjArr.end())
			{
				CSampleAreaListItem * pNewItem = new CSampleAreaListItem(pNode->PosNum,iter->first);
				m_ctrlSampleList.AddItem(0,pNewItem);
				iter++;
			}
		}
	}
	m_ctrlSampleList.SetScrollRange(SB_VERT,0,m_ctrlSampleList.GetItemCount(),false);
	m_ctrlSampleList.Reorder();
	m_ctrlSampleList.SetRedraw(TRUE);
	return 0;
}

void CSampleAreaManDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TID_ListScroll_Delay:
		KillTimer(TID_ListScroll_Delay);
		// 找到该位置数据在列表中的索引，并发送“滚动至该位置”消息
		for(int index = 0; index < m_ctrlSampleList.GetItemCount(); index++)
		{
			CSampleAreaListItem * pNewItem = reinterpret_cast<CSampleAreaListItem *>(m_ctrlSampleList.GetItemData(index));
			if(pNewItem->GetSamplePosNum() == m_UpdatePos)
			{
				m_ctrlSampleList.SendMessage(WM_VSCROLL,MAKEWPARAM(SB_THUMBPOSITION,index),NULL);
				break;
			}
		}
		m_ctrlSampleList.RedrawAllItems();
		break;
	default:
		ASSERT(0);
	}
}