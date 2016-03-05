// CurveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "CurveDlg.h"
#include "StandardTable.h"
#include "CurveTable.h"
#include "CurveListItem.h"
#include "CurveDetailListItem.h"
#include "GeneralFuction.h"

// CCurveDlg 对话框

IMPLEMENT_DYNAMIC(CCurveDlg, CBaseDialog)

CCurveDlg::CCurveDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CCurveDlg::IDD, pParent)
{
	
}

CCurveDlg::~CCurveDlg()
{
}

void CCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Date_TestDateFrom,m_ctrlTestDateFrom);
	DDX_Control(pDX, IDC_Date_TestDateTo,m_ctrlTestDateTo);
	DDX_Control(pDX, IDC_COMBO_PojectName,m_ctrlProjectName);
	DDX_Control(pDX, IDC_Combo_BatchNum,m_ctrlBatchNum);
	DDX_Control(pDX,IDC_LIST_Curve,m_ctrlCurveList);
	DDX_Control(pDX,IDC_LIST_CurveData,m_ctrlDetailsList);
}


BEGIN_MESSAGE_MAP(CCurveDlg, CBaseDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_PojectName, &CCurveDlg::OnCbnSelchangeComboPojectname)
	ON_CBN_SELCHANGE(IDC_Combo_BatchNum, &CCurveDlg::OnCbnSelchangeComboBatchnum)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_Date_TestDateFrom, &CCurveDlg::OnDtnDatetimechangeDateTestdatefrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_Date_TestDateTo, &CCurveDlg::OnDtnDatetimechangeDateTestdate)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_Curve, &CCurveDlg::OnLvnItemchangedListCurve)
END_MESSAGE_MAP()



BOOL CCurveDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	m_hCtlBrush = CreateSolidBrush(RGB(255, 255, 255));

	CRect rect;

	// 设置“查询条件”组控件的“可伸缩属性”
	CWnd * pWnd = GetDlgItem(IDC_GrpTitle_SelectConditions);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	ControlsResizeProp item;
	item.ID = IDC_GrpTitle_SelectConditions;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	item.ctmXofPosBps = tmProportion;				// 基点x坐标按比例放大
	item.ctmYofPosBps = tmProportion;				// 基点y轴按比例放大
	item.ctmWigth = tmProportion;					// 控件宽度按比例缩放
	item.ctmHight = tmProportion;					// 控件长度按比例缩放
	item.IsPaintBackGround = false;					// GroupBox控件不自绘背景
	SetResize(item);

	// 设置“详细信息”组控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_GrpTitle_CurveDetails);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	item.ID = IDC_GrpTitle_CurveDetails;
	SetResize(item);

	// 设置“检测项目”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_ProjectName);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	item.ID = IDC_Title_ProjectName;
	item.ctmHight = tmNotChange;					// 控件高度不变
	SetResize(item);

	// 设置“检测项目”组合控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_COMBO_PojectName);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	item.ID = IDC_COMBO_PojectName;
	item.ctmHight = tmNotChange;					// 控件高度不变
	SetResize(item);

	// 设置“批号”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_BatchNum);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	item.ID = IDC_Title_BatchNum;
	item.ctmHight = tmNotChange;					// 控件高度不变
	SetResize(item);

	// 设置“批号”组合控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Combo_BatchNum);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Combo_BatchNum;
	item.ctmHight = tmNotChange;					// 控件高度不变
	SetResize(item);

	// 设置“定标日期”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_TestDate);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	item.ID = IDC_Title_TestDate;
	item.ctmHight = tmNotChange;					// 控件高度不变
	SetResize(item);

	// 设置“定标起始日期”日期控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Date_TestDateFrom);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	item.ID = IDC_Date_TestDateFrom;
	item.ctmHight = tmNotChange;					// 控件高度不变
	SetResize(item);

	// 设置“～”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_TildeSymbol);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_TildeSymbol;
	item.ctmHight = tmNotChange;					// 控件高度不变
	SetResize(item);

	// 设置“定标终止日期”日期控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Date_TestDateTo);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	item.ID = IDC_Date_TestDateTo;
	item.ctmHight = tmNotChange;					// 控件高度不变
	SetResize(item);

	// 设置“曲线列表”控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_LIST_Curve);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	item.ID = IDC_LIST_Curve;
	item.ctmHight = tmProportion;					// 控件高度按比例缩放
	SetResize(item);

	// 设置“详细信息”组控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_GrpTitle_CurveDetails);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_GrpTitle_CurveDetails;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	item.IsPaintBackGround = false;					// GroupBox控件不自绘背景
	SetResize(item);

	// 设置“曲线数据”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_CurveData);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_CurveData;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“曲线数据”列表控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_LIST_CurveData);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_LIST_CurveData;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“曲线示意图”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_CurveDiagram);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_CurveDiagram;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“曲线示意图”控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Static_CurveDrawing);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Static_CurveDrawing;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“对数尺度”单选按钮的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Raido_LogScale);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Raido_LogScale;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“实际尺度”单选按钮的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Raido_ActualScale);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Raido_ActualScale;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“对数尺度”单选按钮的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Check_Spline);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Check_Spline;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“对数尺度”单选按钮的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Check_FourParams);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Check_FourParams;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“删除”按钮的可伸缩属性
	pWnd = GetDlgItem(IDC_DeleteRecord_Button);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);

	CPoint ptDeleteButtonTopRight;
	ptDeleteButtonTopRight.x = rect.right;
	ptDeleteButtonTopRight.y = rect.top;

	item.ID = IDC_DeleteRecord_Button;
	item.ptOrigPosBps = ptDeleteButtonTopRight;		// 重定位基点“删除”按钮的右上角
	item.ctmWigth = tmNotChange;					// 控件宽度不变
	item.ctmHight = tmNotChange;					// 控件高度不变
	SetResize(item);

	// 设置“修改”按钮的可伸缩属性
	item.ID = IDC_Modify_Button;
	SetResize(item);

	// 设置“打印”按钮的可伸缩属性
	item.ID = IDC_Print_Button;
	SetResize(item);

	// 初始化“项目名称”组合框
	_InitComboProjectName();
	// 初始化“批号”组合框
	_InitComboBatchNums();

	// 终止测试日期初始化为今天24点以前，起始测试日期初始化为半年(7天)以前
	COleDateTime defaultTime;
	defaultTime = COleDateTime::GetCurrentTime();
	defaultTime.SetDateTime(defaultTime.GetYear(),defaultTime.GetMonth(),defaultTime.GetDay(),23,59,59);
	m_ctrlTestDateTo.SetTime(defaultTime);
	COleDateTimeSpan timespan(6,0,0,0);
	defaultTime = defaultTime - timespan;
	defaultTime.SetDateTime(defaultTime.GetYear(),defaultTime.GetMonth(),defaultTime.GetDay(),0,0,0);

	m_ctrlTestDateFrom.SetTime(defaultTime);

	// 测试配置列表初始化
	m_ctrlCurveList.InsertColumn(colProject,_T("检测项目"),LVCFMT_CENTER);
	m_ctrlCurveList.InsertColumn(colBatchNum,_T("批号"),LVCFMT_CENTER);
	m_ctrlCurveList.InsertColumn(colTestDate,_T("测试日期"),LVCFMT_CENTER);
	m_ctrlCurveList.InsertColumn(colDefaultFitType,_T("曲线类型"),LVCFMT_CENTER);
	m_ctrlCurveList.InsertColumn(colSurveyor,_T("检验员"),LVCFMT_CENTER);

	ColumnProperty column;

	// "检测项目"列宽为控件总宽的0.23，最小宽度60
	column.ColumnIndex = colProject;
	column.Width = 60;
	column.WidthRatio = 0.23;
	m_ctrlCurveList.SetColumnWidthProp(column);

	// "批号"列宽为控件总宽的0.23，最小宽度60
	column.ColumnIndex = colBatchNum;
	column.Width = 60;
	column.WidthRatio = 0.23;
	m_ctrlCurveList.SetColumnWidthProp(column);

	// "测试日期"列宽为控件总宽的0.20，最小宽度60
	column.ColumnIndex = colTestDate;
	column.Width = 60;
	column.WidthRatio = 0.18;
	m_ctrlCurveList.SetColumnWidthProp(column);

	// "默认拟合模型"列宽为控件总宽的0.17，最小宽度60
	column.ColumnIndex = colDefaultFitType;
	column.Width = 60;
	column.WidthRatio = 0.18;
	m_ctrlCurveList.SetColumnWidthProp(column);

	// "检验员"列宽为控件总宽的0.17，最小宽度60
	column.ColumnIndex = colSurveyor;
	column.Width = 60;
	column.WidthRatio = 0.18;
	m_ctrlCurveList.SetColumnWidthProp(column);
	m_ctrlCurveList.SetItemHeight(25);

	// 标准品数据详细信息列表初始化
	m_ctrlDetailsList.InsertColumn(colIndex,_T("标准点"),LVCFMT_CENTER);
	m_ctrlDetailsList.InsertColumn(colTheoryConc,_T("理论浓度"),LVCFMT_CENTER);
	m_ctrlDetailsList.InsertColumn(colTestRlu,_T("测量发光值"),LVCFMT_CENTER);
	m_ctrlDetailsList.InsertColumn(colAverageRlu,_T("平均发光值"),LVCFMT_CENTER);
	m_ctrlDetailsList.InsertColumn(colCalConc,_T("计算浓度"),LVCFMT_CENTER);
	m_ctrlDetailsList.InsertColumn(colFitError,_T("拟合误差"),LVCFMT_CENTER);
	m_ctrlDetailsList.InsertColumn(colCV,_T("变异系数"),LVCFMT_CENTER);
	m_ctrlDetailsList.InsertColumn(colInhibitionRate,_T("抑制率"),LVCFMT_CENTER);

	// "标准点索引"列宽为标题宽度
	column.ColumnIndex = colIndex;
	column.Width = 0;
	column.WidthRatio = 0;
	m_ctrlDetailsList.SetColumnWidthProp(column);

	// "理论浓度"列宽为标题宽度
	column.ColumnIndex = colTheoryConc;
	column.Width = 80;
	column.WidthRatio = 0.17;
	m_ctrlDetailsList.SetColumnWidthProp(column);

	// "测量发光值"列宽为标题宽度
	column.ColumnIndex = colTestRlu;
	column.Width = 0;
	column.WidthRatio = 0;
	m_ctrlDetailsList.SetColumnWidthProp(column);

	// "平均发光值"列宽为标题宽度
	column.ColumnIndex = colAverageRlu;
	column.Width = 0;
	column.WidthRatio = 0;
	m_ctrlDetailsList.SetColumnWidthProp(column);

	// "计算浓度"列宽为标题宽度
	column.ColumnIndex = colCalConc;
	column.Width = 80;
	column.WidthRatio = 0.17;
	m_ctrlDetailsList.SetColumnWidthProp(column);

	// "拟合误差"列宽为标题宽度
	column.ColumnIndex = colFitError;
	column.Width = 0;
	column.WidthRatio = 0;
	m_ctrlDetailsList.SetColumnWidthProp(column);


	// "变异系数"列宽为标题宽度
	column.ColumnIndex = colCV;
	column.Width = 0;
	column.WidthRatio = 0;
	m_ctrlDetailsList.SetColumnWidthProp(column);

	// "抑制率"列宽为标题宽度
	column.ColumnIndex = colInhibitionRate;
	column.Width = 0;
	column.WidthRatio = 0;
	m_ctrlDetailsList.SetColumnWidthProp(column);
	m_ctrlDetailsList.SetItemHeight(25);

	m_nCurrentSelectedCurve = 0;
	return TRUE;
}

void CCurveDlg::OnCbnSelchangeComboPojectname()
{
	_RefreshCurveList();
	m_nCurrentSelectedCurve = 0;
	_RefreshDetails();
}

void CCurveDlg::OnCbnSelchangeComboBatchnum()
{
	_RefreshCurveList();
	m_nCurrentSelectedCurve = 0;
	_RefreshDetails();
}

void CCurveDlg::OnDtnDatetimechangeDateTestdatefrom(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	_RefreshCurveList();
	m_nCurrentSelectedCurve = 0;
	_RefreshDetails();
	*pResult = 0;
}

void CCurveDlg::OnDtnDatetimechangeDateTestdate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	_RefreshCurveList();
	*pResult = 0;
}

void CCurveDlg::OnPaint()
{
	CPaintDC dc(this);
	CBrush brWhiteBackGround(RGB(255,255,255));
	dc.FillRgn(&m_rgnBackGround,&brWhiteBackGround);
}

HBRUSH CCurveDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return m_hCtlBrush;
}

void CCurveDlg::_InitComboProjectName()
{
	CStandardTable * pTable = CStandardTable::GetInstance();
	pTable->GetDistinctProjectNames(m_AllProjectNameList);

	// 插入第一条数据位空
	m_ctrlProjectName.InsertString(0,_T(""));

	// 插入其他检测项目数据
	for(int i = 0; i < (int)(m_AllProjectNameList.size()); i++)
	{
		// 项目名称列表输出格式为： 项目简称（项目全称）
		CString strDuName;
		strDuName.Format(_T("%s (%s)"),m_AllProjectNameList[i].ShortName,m_AllProjectNameList[i].ProjectName);
		m_ctrlProjectName.InsertString( i + 1,strDuName);	
	}
}

void CCurveDlg::_InitComboBatchNums()
{
	CStandardTable * pTable = CStandardTable::GetInstance();
	pTable->GetDistinctBatchNums(m_AllBatchNumList);

	// 插入第一条数据位空
	m_ctrlBatchNum.InsertString(0,_T(""));

	// 插入其他检测项目数据
	for(int i = 0; i < (int)(m_AllProjectNameList.size()); i++)
	{
		// 项目名称列表输出格式为： 项目简称（项目全称）
		CString strDuName;
		strDuName.Format(_T("%s"),m_AllBatchNumList[i]);
		m_ctrlBatchNum.InsertString( i + 1,strDuName);	
	}
}

void CCurveDlg::_RefreshCurveList()
{
	// 取得项目名称
	int index = m_ctrlProjectName.GetCurSel();
	CString strProjecName,strShortName;
	if(index > 0)
	{
		strProjecName = m_AllProjectNameList[index - 1].ProjectName;
		strShortName = m_AllProjectNameList[index - 1].ShortName;
	}
	// 取得批号
	index = m_ctrlBatchNum.GetCurSel();
	CString strBatchNum;
	if(index > 0)
		strBatchNum = m_AllBatchNumList[index - 1];

	// 取得生产日期
	COleDateTime timeTestDateFrom,timeTestDateTo;
	m_ctrlTestDateFrom.GetTime(timeTestDateFrom);
	m_ctrlTestDateTo.GetTime(timeTestDateTo);

	// 取得曲线记录
	CCurveTable * pTable = CCurveTable::GetInstance();
	vector<CurveRecord> vtRecordList;
	pTable->GetCurves(
		/*vector<ReagentRecord> & */vtRecordList,
		/*const bool &IsFuzzyQuery = */false,
		/*const CString &strBatchNum =*/ strBatchNum,			         
		/*const CString &strProjectName =*/strProjecName,			            
		/*const CString &strShortName = */strShortName,                  
		/*const DATE &ProductionDateUpper = */timeTestDateTo,
		/*const DATE &ProductionDateLower = */timeTestDateFrom);

	// 刷新前，需要先禁止列表重绘
	m_ctrlCurveList.SetRedraw(FALSE);
	// 在试剂列表中插入查询到的试剂项
	m_ctrlCurveList.DeleteAllItemsAndDatas();
	for(int i=0; i < (int)(vtRecordList.size()); i++)
	{ 
		// 构造试剂列表项
		CCurveListItem * pNewItem = new CCurveListItem(vtRecordList[i]);
		m_ctrlCurveList.AddItem(i,static_cast<CListItemData *>(pNewItem));
	}

	// 默认排序
	m_ctrlCurveList.Reorder();
	
	m_ctrlCurveList.SetFocus();
	m_ctrlCurveList.SetRedraw(TRUE);
}

void CCurveDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	if (bShow == TRUE)
	{
		_RefreshCurveList();
		// 把列表设成焦点状态，避免单击两下才能选中某条记录
		m_ctrlCurveList.SetFocus();
	} 
}

void CCurveDlg::OnLvnItemchangedListCurve(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(pNMLV->uNewState & ODS_SELECTED)
	{
		CCurveListItem * pItem = reinterpret_cast<CCurveListItem *>(m_ctrlCurveList.GetItemData(pNMLV->iItem));
		// 当前试剂ID设为当前选中项的数据
		m_nCurrentSelectedCurve = pItem->GetCurveKey();
		_RefreshDetails();
	}
	*pResult = 0;
}

void CCurveDlg::_RefreshDetails()
{
	// 刷新曲线数据列表
	m_ctrlDetailsList.SetRedraw( FALSE);
	m_ctrlDetailsList.DeleteAllItemsAndDatas();
	if (m_nCurrentSelectedCurve > 0)
	{
		// 取得曲线数据库表
		CCurveTable * pCurveTable = CCurveTable::GetInstance();

		// 取得曲线记录
		CurveRecord recCurve = pCurveTable->GetRecordInfo(m_nCurrentSelectedCurve);

		// 取得标准品记录
		CStandardTable *pStandand = CStandardTable::GetInstance();
		StandardRecord recStandard = pStandand->GetRecordInfo(recCurve.StandardID);

		// 取得通用函数
		CGeneralFuction *pFunction = CGeneralFuction::GetInstance();

		// 遍历发光值列表
		for(int nConcIndex = 0; nConcIndex < 10; nConcIndex ++ )
		{
			// 遍历发光值字符串，取出发光值
			vector<double> rlulist;
			pFunction->ParseRluString(recCurve.RluString[nConcIndex],rlulist);
			double dAverRlu = pFunction->Average(rlulist);
			//double dCalConc = 
			for(int i=0; i<(int)rlulist.size(); i++)
			{
				CCurveDetailListItem * pItem = new CCurveDetailListItem(nConcIndex + 1,
																		recStandard.StdConc[nConcIndex],
																		recStandard.Unit,
																		rlulist[i],
																		dAverRlu,
																		0,
																		0,
																		0,
																		1);
				m_ctrlDetailsList.AddItem(0,pItem);
			}
		}
	}
	m_ctrlDetailsList.SetRedraw(TRUE);
	// 刷新曲线示意图区域
}