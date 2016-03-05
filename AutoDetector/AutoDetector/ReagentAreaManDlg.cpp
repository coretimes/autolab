// ReagentAreaManDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "ReagentAreaManDlg.h"
#include "ReagentAreaListItem.h"

IMPLEMENT_DYNAMIC(CReagentAreaManDlg, CBaseDialog)

CReagentAreaManDlg::CReagentAreaManDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CReagentAreaManDlg::IDD, pParent),m_ctrlReagentArea(raForConfig)
{

}

CReagentAreaManDlg::~CReagentAreaManDlg()
{
}

void CReagentAreaManDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_Ctrl_ReagentArea,m_ctrlReagentArea);
	DDX_Control(pDX,IDC_LIST_ReagentArea,m_ctrlReagentList);
}

BEGIN_MESSAGE_MAP(CReagentAreaManDlg, CBaseDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(UM_ReagentArea_Update, &CReagentAreaManDlg::OnReagentAreaDataUpdate)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CReagentAreaManDlg 消息处理程序

void CReagentAreaManDlg::OnPaint()
{
	CPaintDC dc(this);
	CBrush brWhiteBackGround(RGB(255,255,255));
	dc.FillRgn(&m_rgnBackGround,&brWhiteBackGround);
}

BOOL CReagentAreaManDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CReagentAreaManDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	CRect rect;

	// 设置“试剂区控件”的“可伸缩属性”
	ControlsResizeProp item;
	m_ctrlReagentArea.GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Ctrl_ReagentArea;
	item.ptOrigPosBps = rect.TopLeft();			// 重定位基点是左上顶点
	item.ctmXofPosBps = tmProportion;			// 基点x坐标按比例放大
	item.ctmYofPosBps = tmProportion;			// 基点y轴按比例放大
	item.ctmWigth = tmProportion;				// 控件宽度按比例缩放
	item.ctmHight = tmProportion;				// 控件长度按比例缩放
	SetResize(item);

	// 设置“试剂区列表”的“可伸缩属性”
	ControlsResizeProp itemList;
	m_ctrlReagentList.GetWindowRect(rect);
	ScreenToClient(rect);
	itemList.ID = IDC_LIST_ReagentArea;
	itemList.ptOrigPosBps = rect.TopLeft();			// 重定位基点是左下顶点
	itemList.ctmXofPosBps = tmProportion;			// 基点x坐标按比例放大
	itemList.ctmYofPosBps = tmProportion;			// 基点y轴按比例放大
	itemList.ctmWigth = tmProportion;				// 控件宽度按比例缩放
	itemList.ctmHight = tmProportion;				// 控件长度按比例缩放
	SetResize(itemList);

	// 测试配置列表初始化
	m_ctrlReagentList.InsertColumn(colPos,_T("位置编号"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colProject,_T("检测项目名称"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colBatchNum,_T("试剂生产批号"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colSampleAmount,_T("样本用量"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colMagnetciAmount,_T("磁标用量"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colHRPAmount,_T("酶标用量"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colDilution,_T("稀释倍数"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colWashTimes,_T("清洗次数"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colIncuTime,_T("温育时间"),LVCFMT_CENTER);

	ColumnProperty column;
	// "位置编号"列宽属性为自动标头宽度
	column.ColumnIndex = colPos;
	column.Width = 0;
	column.WidthRatio = 0;
	m_ctrlReagentList.SetColumnWidthProp(column);	

	// "检测项目"列宽属性为自动标头宽度
	column.ColumnIndex = colProject;
	column.Width = 0;
	column.WidthRatio = 0.15;
	m_ctrlReagentList.SetColumnWidthProp(column);	

	// "生产批号"列宽为标头宽度
	column.ColumnIndex = colBatchNum;
	column.Width = 0;
	column.WidthRatio = 0.15;
	m_ctrlReagentList.SetColumnWidthProp(column);

	// "样本用量"列宽为标头宽度
	column.ColumnIndex = colSampleAmount;
	column.Width = 0;
	m_ctrlReagentList.SetColumnWidthProp(column);

	// "磁标用量"列宽为标头宽度
	column.ColumnIndex = colMagnetciAmount;
	column.Width = 0;
	m_ctrlReagentList.SetColumnWidthProp(column);

	// "酶标用量"列宽为标头宽度
	column.ColumnIndex = colHRPAmount;
	column.Width = 0;
	m_ctrlReagentList.SetColumnWidthProp(column);

	// "稀释倍数"列宽属性为自动标头宽度
	column.ColumnIndex = colDilution;
	column.Width = 0;
	m_ctrlReagentList.SetColumnWidthProp(column);
	// "清洗次数"列宽为标头宽度
	column.ColumnIndex = colWashTimes;
	column.Width = 0;
	m_ctrlReagentList.SetColumnWidthProp(column);

	// "温育时间"列宽属性为自动标头宽度
	column.ColumnIndex = colIncuTime;
	column.Width = 0;
	m_ctrlReagentList.SetColumnWidthProp(column);

	m_ctrlReagentList.EnableSelect(FALSE);

	for(int i=0; i<NUM_REAGENTPOS; i++)
	{
		CReagentAreaListItem * pNewItem = new CReagentAreaListItem(i + 1);
		m_ctrlReagentList.AddItem(i,static_cast<CListItemData *>(pNewItem));
	}

	// 把这两个视图控件的指针传入“试剂区数据”对象
	CReagentAreaData::GetInstance()->AttchDialog(this);

	return TRUE;
}

LRESULT CReagentAreaManDlg::OnReagentAreaDataUpdate(WPARAM wParam, LPARAM lParam)
{
	Reagent_Pos pos = static_cast<Reagent_Pos>(wParam);
	m_ctrlReagentList.RedrawAllItems();
	return 0;
}

void CReagentAreaManDlg::OnSize(UINT nType, int cx, int cy)
{
	CBaseDialog::OnSize(nType, cx, cy);

	if(IsWindow(m_ctrlReagentList.GetSafeHwnd()))
	{
		CRect listrect;
		m_ctrlReagentList.GetClientRect(listrect);
		m_ctrlReagentList.SetItemHeight(listrect.Height()/15 - 1);
	}
}
