// TestMonitorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "TestMonitorDlg.h"
#include "MorniterListItem.h"
#include "TestManagement.h"

// CTestMonitorDlg 对话框

IMPLEMENT_DYNAMIC(CTestMonitorDlg, CBaseDialog)

CTestMonitorDlg::CTestMonitorDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CTestMonitorDlg::IDD, pParent)
{

}

CTestMonitorDlg::~CTestMonitorDlg()
{
}

void CTestMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_List_Mornitor,m_ctrlMornitorList);
}


BEGIN_MESSAGE_MAP(CTestMonitorDlg, CBaseDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(UM_TestBuffer_Update, &CTestMonitorDlg::OnTestBufferUpdate)
END_MESSAGE_MAP()

void CTestMonitorDlg::OnPaint()
{
	CPaintDC dc(this);
	CBrush brWhiteBackGround(RGB(255,255,255));
	dc.FillRgn(&m_rgnBackGround,&brWhiteBackGround);
}

BOOL CTestMonitorDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CTestMonitorDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// 设置“监控列表”的“可伸缩属性”
	ControlsResizeProp itemList;
	CRect rect;
	m_ctrlMornitorList.GetWindowRect(rect);
	ScreenToClient(rect);
	itemList.ID = IDC_List_Mornitor;
	itemList.ptOrigPosBps = rect.TopLeft();			// 重定位基点是左下顶点
	itemList.ctmXofPosBps = tmProportion;			// 基点x坐标按比例放大
	itemList.ctmYofPosBps = tmProportion;			// 基点y轴按比例放大
	itemList.ctmWigth = tmProportion;				// 控件宽度按比例缩放
	itemList.ctmHight = tmProportion;				// 控件长度按比例缩放
	SetResize(itemList);

	// 监控列表列表初始化
	m_ctrlMornitorList.InsertColumn(colType,_T("样品类型"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colProject,_T("检测项目"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colSampleNo,_T("样品编号"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colConcPoint,_T("浓度点"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colSamplePos,_T("样本位"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colMeteringIndex,_T("测光节点"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colFinishTime,_T("完成时间"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colMeteringState,_T("测光状态"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colUseState,_T("使用状态"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colRlu,_T("发光值"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colAverageRlu,_T("平均发光值"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colCV,_T("变异系数"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colTheoryConc,_T("理论浓度"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colCalConc,_T("计算浓度"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colCurve,_T("标准曲线"),LVCFMT_CENTER);

	ColumnProperty column;

	// "样品类型"列宽属性为标题宽度
	column.ColumnIndex = colType;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);	

	// "检测项目"列宽为控件总宽的0.1，最小宽度100
	column.ColumnIndex = colProject;
	column.Width = 40;
	column.WidthRatio = 0.1;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "样品编号"列宽为控件总宽的0.1，最小宽度40
	column.ColumnIndex = colSampleNo;
	column.Width = 40;
	column.WidthRatio = 0.1;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "浓度点"列宽属性为自动标头宽度
	column.ColumnIndex = colConcPoint;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "样本位"列宽属性为自动标头宽度
	column.ColumnIndex = colSamplePos;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "测光节点"列宽属性为自动标头宽度
	column.ColumnIndex = colMeteringIndex;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "完成时间"列宽属性为自动标头宽度
	column.ColumnIndex = colFinishTime;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "测光状态"列宽属性为自动标头宽度
	column.ColumnIndex = colMeteringState;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "使用状态"列宽属性为自动标头宽度
	column.ColumnIndex = colUseState;
	column.Width = 40;
	column.WidthRatio = 0.08;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "测光状态"列宽属性为自动标头宽度
	column.ColumnIndex = colMeteringState;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "发光值"列宽属性为自动标头宽度
	column.ColumnIndex = colRlu;
	column.Width = 40;
	column.WidthRatio = 0.08;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "平均发光值"列宽属性为自动标头宽度
	column.ColumnIndex = colAverageRlu;
	column.Width = 80;
	column.WidthRatio = 0.08;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "变异系数"列宽属性为自动标头宽度
	column.ColumnIndex = colCV;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "理论浓度"列宽属性为自动标头宽度
	column.ColumnIndex = colTheoryConc;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "计算浓度"列宽属性为自动标头宽度
	column.ColumnIndex = colCalConc;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "标准曲线"列宽属性为自动标头宽度
	column.ColumnIndex = colCurve;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	m_ctrlMornitorList.SetItemHeight(25);
	m_ctrlMornitorList.EnableSelect(TRUE);
	m_ctrlMornitorList.SetFocus();

	// 取得测试管理器指针
	CTestManagement * pTestMangement = CTestManagement::GetInstance();
	pTestMangement->AttchDialog(this);

	return TRUE;
}

LRESULT CTestMonitorDlg::OnTestBufferUpdate(WPARAM wParam, LPARAM lParam)
{
	// 沙漏鼠标形状
	CWaitCursor wait;

	// 列表控件暂时禁止绘制
	m_ctrlMornitorList.SetRedraw(FALSE);

	TestUnit_ID unitID = static_cast<TestUnit_ID>(wParam);
	BOOL IsNewData = static_cast<BOOL>(lParam);

	// 删除指定测试单元的原有数据项
	int num = (int)(m_SearchMap.count(unitID));
	multimap<TestUnit_ID,int>::iterator iter = m_SearchMap.find(unitID);
	vector<int> vctIndex;
	for (int i=0; i<num; i++)
	{
		// 倒着删除行，否则列表索引会发生变化,所以先保存索引与数组中，在逆方向遍历数组，删除指定索引的项
		vctIndex.push_back(iter->second);
		iter++;
	}
	m_SearchMap.erase(unitID);

	for(int i = num - 1; i >= 0; i--)
	{
		m_ctrlMornitorList.DeleteItem(vctIndex[i]);
	}


	//multimap<TestUnit_ID,int>::iterator iterBegin = m_SearchMap.lower_bound(unitID);
	//multimap<TestUnit_ID,int>::iterator iterEnd = m_SearchMap.upper_bound(unitID);
	//if (iterBegin != m_SearchMap.end())
	//{
	//	// 倒着删除行，否则列表索引会发生变化,所以先保存索引与数组中，在逆方向遍历数组，删除指定索引的项
	//	vector<int> vctIndex;
	//	vctIndex.push_back(iterBegin->second);
	//	while(iterBegin != iterEnd)
	//	{
	//		m_SearchMap.erase(iterBegin);
	//		iterBegin++;
	//	}

	//	for(int i= (int)(vctIndex.size())-1; i>=0; i++)
	//	{
	//		m_ctrlMornitorList.DeleteItem(vctIndex[i]);
	//	}
	//}

	// 取得测试管理器指针
	CTestManagement * pTestMangement = CTestManagement::GetInstance();

	// 取得测试管理缓冲区引用
	const map<TestUnit_ID, CTestUnit *> & TestBuffer = pTestMangement->GetTestBuffer();

	// 根据测试单元ID查找测试单元，如果找不到，则说明是删除该数据，直接返回
	map<TestUnit_ID, CTestUnit *>::const_iterator iterUnit = TestBuffer.find(unitID);
	if(iterUnit == TestBuffer.end())
	{
		return 0;
	}
	CTestUnit * pTestUnit = iterUnit->second;

	// 取得单元内样本节点数组
	const map<ConcPoint_Index,SampleTestNode> & SampleNodes = pTestUnit->GetSampleTestNodes();

	// 遍历测试单元内每个样本节点
	map<ConcPoint_Index,SampleTestNode>::const_iterator iterSampleNode = SampleNodes.begin();
	while(iterSampleNode != SampleNodes.end())
	{
		const SampleTestNode & sampnode = iterSampleNode->second;

		// 遍历样本节点内每个测光节点节点
		vector<Metering_ID>::const_iterator iterMeteringNode = sampnode.arrMeterNodeID.begin();
		int indexListOfItem = m_ctrlMornitorList.GetItemCount();
		while(iterMeteringNode != sampnode.arrMeterNodeID.end())
		{
			// 项列表中插入新数据项
			CMorniterListItem * pNewItem = new CMorniterListItem(*iterMeteringNode);
			m_ctrlMornitorList.AddItem(indexListOfItem,pNewItem);
			// 向查询表中，插入数据
			m_SearchMap.insert(pair<TestUnit_ID,int>(unitID,indexListOfItem));		
			indexListOfItem++;
			iterMeteringNode++;
		}
		iterSampleNode++;
	}

	m_ctrlMornitorList.SetRedraw(TRUE);
	wait.Restore();
	return 0;
}

void CTestMonitorDlg::RefreshSearchMap()
{
	m_SearchMap.clear();
	for(int i=0; i<m_ctrlMornitorList.GetItemCount(); i++)
	{
		CMorniterListItem * pNewItem = reinterpret_cast<CMorniterListItem*>(m_ctrlMornitorList.GetItemData(i));
		m_SearchMap.insert(pair<TestUnit_ID,int>(pNewItem->GetTestUnit()->GetID(),i));		
	}
}