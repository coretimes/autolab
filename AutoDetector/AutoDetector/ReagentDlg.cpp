
#include "stdafx.h"
#include "AutoDetector.h"
#include "ReagentDlg.h"
#include "MemDC.h"
#include "ReagentSelectListItem.h"
// CReagentDlg 对话框

IMPLEMENT_DYNAMIC(CReagentDlg, CBaseDialog)

CReagentDlg::CReagentDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CReagentDlg::IDD, pParent)
{

}

CReagentDlg::~CReagentDlg()
{
	DeleteObject(m_hCtlBrush);
}

void CReagentDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PojectName,m_ctrlProjectName);
	DDX_Control(pDX, IDC_Date_ProductFrom,m_ctrlProductDateFrom);
	DDX_Control(pDX, IDC_Date_ProductTo,m_ctrlProductDateTo);
	DDX_Control(pDX, IDC_LIST_Reagents,m_ctrlReagentList);
}


BEGIN_MESSAGE_MAP(CReagentDlg, CBaseDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_AddRecord_Button, &CReagentDlg::OnBnClickedAddrecordButton)
	ON_CBN_SELCHANGE(IDC_COMBO_PojectName, &CReagentDlg::OnCbnSelchangeComboPojectname)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_Date_ProductFrom, &CReagentDlg::OnDtnDatetimechangeDateProductfrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_Date_ProductTo, &CReagentDlg::OnDtnDatetimechangeDateProductto)
	ON_BN_CLICKED(IDC_DeleteRecord_Button, &CReagentDlg::OnBnClickedDeleterecordButton)
	ON_BN_CLICKED(IDC_Modify_Button, &CReagentDlg::OnBnClickedModifyButton)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_Reagents, &CReagentDlg::OnLvnItemchangedListReagents)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


BOOL CReagentDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	
	m_hCtlBrush = CreateSolidBrush(RGB(255, 255, 255));
	CWnd * pWnd = NULL;

	CRect rect;

	// 设置“查询条件”组控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_GrpTitle_SelectConditions);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	ControlsResizeProp itemGroupSelectConditions;
	itemGroupSelectConditions.ID = IDC_GrpTitle_SelectConditions;
	itemGroupSelectConditions.ptOrigPosBps = rect.TopLeft();			// 重定位基点是左上顶点
	itemGroupSelectConditions.ctmXofPosBps = tmProportion;				// 基点x坐标按比例放大
	itemGroupSelectConditions.ctmYofPosBps = tmProportion;				// 基点y轴按比例放大
	itemGroupSelectConditions.ctmWigth = tmProportion;					// 控件宽度按比例缩放
	itemGroupSelectConditions.ctmHight = tmProportion;					// 控件长度按比例缩放
	itemGroupSelectConditions.IsPaintBackGround = false;				// GroupBox控件不自绘背景
	SetResize(itemGroupSelectConditions);

	// 在“查询条件”组中控件的伸缩属性
	ControlsResizeProp itemAtGroupSelectConditions;
	itemAtGroupSelectConditions.ctmXofPosBps = tmProportion;			// 基点x坐标按比例放大
	itemAtGroupSelectConditions.ctmYofPosBps = tmProportion;			// 基点y轴按比例放大
	itemAtGroupSelectConditions.ctmWigth = tmProportion;				// 控件宽度按比例缩放
	itemAtGroupSelectConditions.ctmHight = tmNotChange;					// 控件高度不变

	// 设置“检测项目”标签的可伸缩属性
	pWnd = GetDlgItem(IDC_Title_ProjectName);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupSelectConditions.ID = IDC_Title_ProjectName;
	itemAtGroupSelectConditions.ptOrigPosBps = rect.TopLeft();			// 重定位基点是左上顶点
	SetResize(itemAtGroupSelectConditions);

	// 设置“检测项目”组合框的可伸缩属性
	pWnd = GetDlgItem(IDC_COMBO_PojectName);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupSelectConditions.ID = IDC_COMBO_PojectName;
	itemAtGroupSelectConditions.ptOrigPosBps = rect.TopLeft();			// 重定位基点是左上顶点
	SetResize(itemAtGroupSelectConditions);

	// 设置“生产日期”标签的可伸缩属性
	pWnd = GetDlgItem(IDC_Title_ProductDate);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupSelectConditions.ID = IDC_Title_ProductDate;
	itemAtGroupSelectConditions.ptOrigPosBps = rect.TopLeft();			// 重定位基点是左上顶点
	SetResize(itemAtGroupSelectConditions);

	// 设置“生产起始日期”标签的可伸缩属性
	pWnd = GetDlgItem(IDC_Title_ProductDateFrom);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupSelectConditions.ID = IDC_Title_ProductDateFrom;
	itemAtGroupSelectConditions.ptOrigPosBps = rect.TopLeft();			// 重定位基点是左上顶点
	SetResize(itemAtGroupSelectConditions);

	// 设置“生产起始日期”控件的可伸缩属性
	pWnd = GetDlgItem(IDC_Date_ProductFrom);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupSelectConditions.ID = IDC_Date_ProductFrom;
	itemAtGroupSelectConditions.ptOrigPosBps = rect.TopLeft();			// 重定位基点是左上顶点
	SetResize(itemAtGroupSelectConditions);

	// 设置“生产终止日期”标签的可伸缩属性
	pWnd = GetDlgItem(IDC_Title_ProductDateTo);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupSelectConditions.ID = IDC_Title_ProductDateTo;
	itemAtGroupSelectConditions.ptOrigPosBps = rect.TopLeft();			// 重定位基点是左上顶点
	SetResize(itemAtGroupSelectConditions);

	// 设置“生产终止日期”控件的可伸缩属性
	pWnd = GetDlgItem(IDC_Date_ProductTo);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupSelectConditions.ID = IDC_Date_ProductTo;
	itemAtGroupSelectConditions.ptOrigPosBps = rect.TopLeft();			// 重定位基点是左上顶点
	SetResize(itemAtGroupSelectConditions);

	// 设置“试剂列表”控件的可伸缩属性
	pWnd = GetDlgItem(IDC_LIST_Reagents);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	ControlsResizeProp itemListReagents;
	itemAtGroupSelectConditions.ID = IDC_LIST_Reagents;
	itemAtGroupSelectConditions.ptOrigPosBps = rect.TopLeft();			// 重定位基点是左上顶点
	itemAtGroupSelectConditions.ctmHight = tmProportion;				// 控件高度按比例缩放
	SetResize(itemAtGroupSelectConditions);

	// 设置“删除”按钮的可伸缩属性
	pWnd = GetDlgItem(IDC_DeleteRecord_Button);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);

	CPoint ptDeleteButtonTopRight;
	ptDeleteButtonTopRight.x = rect.right;
	ptDeleteButtonTopRight.y = rect.top;

	itemAtGroupSelectConditions.ID = IDC_DeleteRecord_Button;
	itemAtGroupSelectConditions.ptOrigPosBps = ptDeleteButtonTopRight;	// 重定位基点“删除”按钮的右上角
	itemAtGroupSelectConditions.ctmWigth = tmNotChange;					// 控件宽度不变
	itemAtGroupSelectConditions.ctmHight = tmNotChange;					// 控件高度不变
	SetResize(itemAtGroupSelectConditions);

	// 设置“修改”按钮的可伸缩属性
	itemAtGroupSelectConditions.ID = IDC_Modify_Button;
	itemAtGroupSelectConditions.ptOrigPosBps = ptDeleteButtonTopRight;	// 重定位基点“删除”按钮的右上角
	itemAtGroupSelectConditions.ctmWigth = tmNotChange;					// 控件宽度不变
	itemAtGroupSelectConditions.ctmHight = tmNotChange;					// 控件高度不变
	SetResize(itemAtGroupSelectConditions);

	// 设置“添加”按钮的可伸缩属性
	itemAtGroupSelectConditions.ID = IDC_AddRecord_Button;
	itemAtGroupSelectConditions.ptOrigPosBps = ptDeleteButtonTopRight;	// 重定位基点“删除”按钮的右上角
	itemAtGroupSelectConditions.ctmWigth = tmNotChange;					// 控件宽度不变
	itemAtGroupSelectConditions.ctmHight = tmNotChange;					// 控件高度不变
	SetResize(itemAtGroupSelectConditions);

	// 设置“详细信息”组控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_GrpTitle_Details);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	ControlsResizeProp itemGroupDetails;
	itemGroupDetails.ID = IDC_GrpTitle_Details;
	itemGroupDetails.ptOrigPosBps = ptDeleteButtonTopRight;				// 重定位基点是左上顶点
	itemGroupDetails.ctmXofPosBps = tmProportion;						// 基点x坐标按比例放大
	itemGroupDetails.ctmYofPosBps = tmProportion;						// 基点y轴按比例放大
	itemGroupDetails.ctmWigth = tmProportion;							// 控件宽度按比例缩放
	itemGroupDetails.ctmHight = tmProportion;							// 控件长度按比例缩放
	itemGroupDetails.IsPaintBackGround = false;							// GroupBox控件不自绘背景
	SetResize(itemGroupDetails);

	// 在“详细信息”组中控件的伸缩属性
	ControlsResizeProp itemAtGroupDetails;
	itemAtGroupDetails.ctmXofPosBps = tmProportion;						// 基点x坐标按比例放大
	itemAtGroupDetails.ctmYofPosBps = tmProportion;						// 基点y轴按比例放大
	itemAtGroupDetails.ctmWigth = tmProportion;							// 控件宽度按比例缩放
	itemAtGroupDetails.ctmHight = tmNotChange;							// 控件高度不变

	// 设置“试剂ID”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_ID);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_ID;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“生产批号”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_BatchNum);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_BatchNum;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“检测项目名称”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_ProjectName2);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_ProjectName2;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“生产日期”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_ProductDate2);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_ProductDate2;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“失效日期”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_ExpirationDate);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_ExpirationDate;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“首次加载时间”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_FirstLoadTime);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_FirstLoadTime;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“首次使用时间”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_FirstUseTime);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_FirstUseTime;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“额定实验次数”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_RatedTestTimes);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_RatedTestTimes;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“剩余使用次数”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_RemainTimes);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_RemainTimes;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“测量单位”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_Unit);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Unit;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“结果上限”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_UpperLimit);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_UpperLimit;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“结果下限”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_LowerLimit);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_LowerLimit;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“样本用量”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_SampleAmount);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_SampleAmount;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“磁标用量”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_MagneticAmount);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_MagneticAmount;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“酶标用量”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_HRPAmount);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_HRPAmount;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“稀释倍数”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_Dilution2);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Dilution2;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“清洗次数”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_WashTimes);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_WashTimes;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“孵育时间”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_IncubationTime);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_IncubationTime;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“试剂ID”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_ID);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_ID;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“生产批号”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_BatchNum);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_BatchNum;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“检测项目名称”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_ProjectName2);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_ProjectName2;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“生产日期”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_ProductDate2);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_ProductDate2;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“失效日期”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_ExpirationDate);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_ExpirationDate;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“首次加载时间”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_FirstLoadTime);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_FirstLoadTime;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“首次使用时间”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_FirstUseTime);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_FirstUseTime;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“额定实验次数”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_RatedTestTimes);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_RatedTestTimes;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“剩余使用次数”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_RemainTimes);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_RemainTimes;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“测量单位”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_Unit);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Unit;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“结果上限”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_UpperLimit);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_UpperLimit;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“结果下限”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_LowerLimit);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_LowerLimit;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“样本用量”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_SampleAmount);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_SampleAmount;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“磁标用量”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_MagneticAmount);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_MagneticAmount;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“酶标用量”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_HRPAmount);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_HRPAmount;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“稀释倍数”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_Dilution);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Dilution;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“清洗次数”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_WashTimes);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_WashTimes;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“孵育时间”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_IncubationTime);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_IncubationTime;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 项目名组合框初始化
	_InitComboProjectName();

	// 终止生产日期初始化为今天，起始生产日期初始化为半年（180天）以前
	COleDateTime defaultTime;
	defaultTime = COleDateTime::GetCurrentTime();
	m_ctrlProductDateTo.SetTime(defaultTime);

	COleDateTimeSpan timespan(180,0,0,0);
	defaultTime = defaultTime - timespan;

	m_ctrlProductDateFrom.SetTime(defaultTime);

	// 试剂列表初始化
	m_ctrlReagentList.InsertColumn(colBatchNum,_T("批号"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colProject,_T("检测项目"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colProductDate,_T("生产日期"),LVCFMT_CENTER);

	ColumnProperty column;

	// "批号"列宽为控件总宽的0.35，最小宽度60
	column.ColumnIndex = colBatchNum;
	column.Width = 100;
	column.WidthRatio = 0.35;
	m_ctrlReagentList.SetColumnWidthProp(column);

	// "批号"列宽为控件总宽的0.35，最小宽度60
	column.ColumnIndex = colProject;
	column.Width = 100;
	column.WidthRatio = 0.35;
	m_ctrlReagentList.SetColumnWidthProp(column);

	// "批号"列宽为控件总宽的0.3，最小宽度60
	column.ColumnIndex = colProductDate;
	column.Width = 100;
	column.WidthRatio = 0.3;
	m_ctrlReagentList.SetColumnWidthProp(column);

	m_ctrlReagentList.SetItemHeight(25);
	m_ctrlReagentList.EnableSelect(TRUE);
	m_ctrlReagentList.SetFocus();

	// 刷新试剂列表和试剂详细信息
	_RefreshReagentsList();

	m_nCurrentSelectedReagent = 0;
	_RefreshDetails();
	return TRUE;
}


void CReagentDlg::_InitComboProjectName()
{
	CReagentTable * pTable = CReagentTable::GetInstance();
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

void CReagentDlg::OnBnClickedAddrecordButton()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CReagentDlg::OnCbnSelchangeComboPojectname()
{
	_RefreshReagentsList();
	m_nCurrentSelectedReagent = 0;
	_RefreshDetails();
}

void CReagentDlg::OnDtnDatetimechangeDateProductfrom(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	_RefreshReagentsList();
	// 清空详细信息
	m_nCurrentSelectedReagent = 0;
	_RefreshDetails();
	*pResult = 0;
}

void CReagentDlg::OnDtnDatetimechangeDateProductto(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	_RefreshReagentsList();
	// 清空详细信息
	m_nCurrentSelectedReagent = 0;
	_RefreshDetails();
	*pResult = 0;
}

void CReagentDlg::OnBnClickedDeleterecordButton()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CReagentDlg::OnBnClickedModifyButton()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CReagentDlg::OnPaint()
{
	CPaintDC dc(this);
	CBrush brWhiteBackGround(RGB(255,255,255));
	dc.FillRgn(&m_rgnBackGround,&brWhiteBackGround);
}

HBRUSH CReagentDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return m_hCtlBrush;
}

void CReagentDlg::_RefreshReagentsList()
{
	// 取得项目名称
	int index = m_ctrlProjectName.GetCurSel();

	CString strProjecName;
	if(index > 0)
		strProjecName = m_AllProjectNameList[index - 1].ProjectName;

	// 取得生产日期
	COleDateTime timeProductDateFrom,timeProductDateTo;
	m_ctrlProductDateFrom.GetTime(timeProductDateFrom);
	m_ctrlProductDateTo.GetTime(timeProductDateTo);

	// 取得试剂表指针
	CReagentTable * pTable = CReagentTable::GetInstance();

	vector<ReagentRecord> vtReagentList;
	pTable->GetReagents(
		/*vector<ReagentRecord> & */vtReagentList,
		/*const bool &IsFuzzyQuery = */false,
		/*const Reagent_Key &ID =*/ 0,					          
		/*const CString &strBatchNum =*/ _T(""),			         
		/*const CString &strProjectName =*/strProjecName,			            
		/*const CString &strShortName = */_T(""),                  
		/*const DATE &ProductionDateUpper = */timeProductDateTo,
		/*const DATE &ProductionDateLower = */timeProductDateFrom);

	// 刷新前，需要先禁止列表重绘
	m_ctrlReagentList.SetRedraw(FALSE);
	// 在试剂列表中插入查询到的试剂项
	m_ctrlReagentList.DeleteAllItemsAndDatas();
	for(int i=0; i < (int)(vtReagentList.size()); i++)
	{ 
		// 试剂ID
		Reagent_Key key = vtReagentList[i].ID;

		// 试剂批号取得
		CString strBatchNum = vtReagentList[i].BatchNum;

		// 项目全称和简称取得
		ProjectFullName projectname;
		projectname.ProjectName = vtReagentList[i].ProjectName;
		projectname.ShortName = vtReagentList[i].ShortName;

		// 生产日期取得
		COleDateTime dtProductDate = vtReagentList[i].ProductionDate;
		// 构造试剂列表项
		CReagentSelectListItem * pNewItem = new CReagentSelectListItem(key,strBatchNum,projectname,dtProductDate);
		m_ctrlReagentList.AddItem(i,static_cast<CListItemData *>(pNewItem));
	}

	// 默认排序
	m_ctrlReagentList.Reorder();
	m_ctrlReagentList.SetRedraw(TRUE);
	// 把试剂列表设置为焦点窗口
	m_ctrlReagentList.SetFocus();
}

void CReagentDlg::_RefreshDetails()
{
	CWnd * pWnd = NULL;
	// 如果当前试剂ID有效，则设置详细信息，否则设置为空
	if(m_nCurrentSelectedReagent > 0)
	{
		CReagentTable * pTable = CReagentTable::GetInstance();
		ReagentRecord record = pTable->GetRecordInfo(m_nCurrentSelectedReagent);
		CString strTemp;
		COleDateTime dtDate;
		// 标准品详细信息“试剂ID”
		strTemp.Format(_T("%d"),record.ID);
		pWnd = GetDlgItem(IDC_Data_ID);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“生产批号”
		pWnd = GetDlgItem(IDC_Data_BatchNum2);
		pWnd->SetWindowText(record.BatchNum);

		// 标准品详细信息“项目名称”
		pWnd = GetDlgItem(IDC_Data_ProjectName2);
		strTemp.Format(_T("%s(%s)"),record.ShortName,record.ProjectName);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“生产日期”
		dtDate = record.ProductionDate;
		pWnd = GetDlgItem(IDC_Data_ProductDate2);
		pWnd->SetWindowText(dtDate.Format(_T("%Y-%m-%d")));

		// 标准品详细信息“失效日期”
		dtDate = record.ExpirationDate;
		pWnd = GetDlgItem(IDC_Data_ExpirationDate);
		pWnd->SetWindowText(dtDate.Format(_T("%Y-%m-%d")));

		// 标准品详细信息“首次加载时间”
		dtDate = record.FirstLoadTime;
		pWnd = GetDlgItem(IDC_Data_FirstLoadTime);
		pWnd->SetWindowText(dtDate.Format(_T("%Y-%m-%d")));

		// 标准品详细信息“首次使用时间”
		dtDate = record.FirstUseTime;
		pWnd = GetDlgItem(IDC_Data_FirstUseTime);
		pWnd->SetWindowText(dtDate.Format(_T("%Y-%m-%d")));

		// 标准品详细信息“额定实验次数”
		strTemp.Format(_T("%d"),record.RatedTestTimes);
		pWnd = GetDlgItem(IDC_Data_RatedTestTimes);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“剩余使用次数”
		strTemp.Format(_T("%d"),record.RemainTimes);
		pWnd = GetDlgItem(IDC_Data_RemainTimes);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“测量单位”
		pWnd = GetDlgItem(IDC_Data_Unit);
		pWnd->SetWindowText(record.Unit);

		// 标准品详细信息“结果上限”
		strTemp.Format(_T("%g"),record.UpperLimit);
		pWnd = GetDlgItem(IDC_Data_UpperLimit);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“结果下限”
		strTemp.Format(_T("%g"),record.LowerLimit);
		pWnd = GetDlgItem(IDC_Data_LowerLimit);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“样本用量”
		strTemp.Format(_T("%g"),record.SampleAmount);
		pWnd = GetDlgItem(IDC_Data_SampleAmount);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“磁标用量”
		strTemp.Format(_T("%g"),record.MagneticAmount);
		pWnd = GetDlgItem(IDC_Data_MagneticAmount);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“酶标用量”
		strTemp.Format(_T("%g"),record.HRPAmount);
		pWnd = GetDlgItem(IDC_Data_HRPAmount);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“稀释倍数”
		strTemp.Format(_T("%g"),record.Dilution);
		pWnd = GetDlgItem(IDC_Data_Dilution);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“清洗次数”
		strTemp.Format(_T("%g"),record.WashTimes);
		pWnd = GetDlgItem(IDC_Data_WashTimes);
		pWnd->SetWindowText(strTemp);

		// 标准品详细信息“孵育时间”
		strTemp.Format(_T("%g"),record.IncubationTime);
		pWnd = GetDlgItem(IDC_Data_IncubationTime);
		pWnd->SetWindowText(strTemp);
	}
	else
	{
		// 标准品详细信息“试剂ID”
		pWnd = GetDlgItem(IDC_Data_ID);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“生产批号”
		pWnd = GetDlgItem(IDC_Data_BatchNum2);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“项目名称”
		pWnd = GetDlgItem(IDC_Data_ProjectName2);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“生产日期”
		pWnd = GetDlgItem(IDC_Data_ProductDate2);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“失效日期”
		pWnd = GetDlgItem(IDC_Data_ExpirationDate);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“首次加载时间”
		pWnd = GetDlgItem(IDC_Data_FirstLoadTime);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“首次使用时间”
		pWnd = GetDlgItem(IDC_Data_FirstUseTime);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“额定实验次数”
		pWnd = GetDlgItem(IDC_Data_RatedTestTimes);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“剩余使用次数”
		pWnd = GetDlgItem(IDC_Data_RemainTimes);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“测量单位”
		pWnd = GetDlgItem(IDC_Data_Unit);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“结果上限”
		pWnd = GetDlgItem(IDC_Data_UpperLimit);
		pWnd->SetWindowText(_T(""));
		// 标准品详细信息“结果下限”
		pWnd = GetDlgItem(IDC_Data_LowerLimit);
		pWnd->SetWindowText(_T(""));
		
		// 标准品详细信息“样本用量”
		pWnd = GetDlgItem(IDC_Data_SampleAmount);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“磁标用量”
		pWnd = GetDlgItem(IDC_Data_MagneticAmount);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“酶标用量”
		pWnd = GetDlgItem(IDC_Data_HRPAmount);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“稀释倍数”
		pWnd = GetDlgItem(IDC_Data_Dilution);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“清洗次数”
		pWnd = GetDlgItem(IDC_Data_WashTimes);
		pWnd->SetWindowText(_T(""));

		// 标准品详细信息“孵育时间”
		pWnd = GetDlgItem(IDC_Data_IncubationTime);
		pWnd->SetWindowText(_T(""));

	}
}
void CReagentDlg::OnLvnItemchangedListReagents(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(pNMLV->uNewState & ODS_SELECTED)
	{
		CReagentSelectListItem * pItem = reinterpret_cast<CReagentSelectListItem *>(m_ctrlReagentList.GetItemData(pNMLV->iItem));
		// 当前试剂ID设为当前选中项的数据
		m_nCurrentSelectedReagent = pItem->GetReagentKey();
		_RefreshDetails();
	}
	*pResult = 0;
}

void CReagentDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	
	if (bShow == TRUE)
	{
		// 把列表设成焦点状态，避免单击两下才能选中某条记录
		m_ctrlReagentList.SetFocus();
	} 
}
