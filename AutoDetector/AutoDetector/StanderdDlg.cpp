#include "stdafx.h"
#include "AutoDetector.h"
#include "StanderdDlg.h"
#include "MemDC.h"
#include "StandardTable.h"
#include "StandardsSelectListItem.h"

// CStanderdDlg 对话框

IMPLEMENT_DYNAMIC(CStanderdDlg, CBaseDialog)

CStanderdDlg::CStanderdDlg(CWnd* pParent /*=NULL*/)
: CBaseDialog(CStanderdDlg::IDD, pParent)
{

}

CStanderdDlg::~CStanderdDlg()
{
	DeleteObject(m_hCtlBrush);
}

void CStanderdDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PojectName,m_ctrlProjectName);
	DDX_Control(pDX, IDC_Date_ProductFrom,m_ctrlProductDateFrom);
	DDX_Control(pDX, IDC_Date_ProductTo,m_ctrlProductDateTo);
	DDX_Control(pDX, IDC_LIST_Standerd,m_ctrlStandardsList);

	DDX_Control(pDX, IDC_Title_Std1,m_ctrlStdPointTitle[0]);
	DDX_Control(pDX, IDC_Title_Std2,m_ctrlStdPointTitle[1]);
	DDX_Control(pDX, IDC_Title_Std3,m_ctrlStdPointTitle[2]);
	DDX_Control(pDX, IDC_Title_Std4,m_ctrlStdPointTitle[3]);
	DDX_Control(pDX, IDC_Title_Std5,m_ctrlStdPointTitle[4]);
	DDX_Control(pDX, IDC_Title_Std6,m_ctrlStdPointTitle[5]);
	DDX_Control(pDX, IDC_Title_Std7,m_ctrlStdPointTitle[6]);
	DDX_Control(pDX, IDC_Title_Std8,m_ctrlStdPointTitle[7]);
	DDX_Control(pDX, IDC_Title_Std9,m_ctrlStdPointTitle[8]);
	DDX_Control(pDX, IDC_Title_Std10,m_ctrlStdPointTitle[9]);

	DDX_Control(pDX, IDC_Data_Std1,m_ctrlStdPointValue[0]);
	DDX_Control(pDX, IDC_Data_Std2,m_ctrlStdPointValue[1]);
	DDX_Control(pDX, IDC_Data_Std3,m_ctrlStdPointValue[2]);
	DDX_Control(pDX, IDC_Data_Std4,m_ctrlStdPointValue[3]);
	DDX_Control(pDX, IDC_Data_Std5,m_ctrlStdPointValue[4]);
	DDX_Control(pDX, IDC_Data_Std6,m_ctrlStdPointValue[5]);
	DDX_Control(pDX, IDC_Data_Std7,m_ctrlStdPointValue[6]);
	DDX_Control(pDX, IDC_Data_Std8,m_ctrlStdPointValue[7]);
	DDX_Control(pDX, IDC_Data_Std9,m_ctrlStdPointValue[8]);
	DDX_Control(pDX, IDC_Data_Std10,m_ctrlStdPointValue[9]);
}


BEGIN_MESSAGE_MAP(CStanderdDlg, CBaseDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_AddRecord_Button, &CStanderdDlg::OnBnClickedAddrecordButton)
	ON_CBN_SELCHANGE(IDC_COMBO_PojectName, &CStanderdDlg::OnCbnSelchangeComboPojectname)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_Date_ProductFrom, &CStanderdDlg::OnDtnDatetimechangeDateProductfrom)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_Date_ProductTo, &CStanderdDlg::OnDtnDatetimechangeDateProductto)
	ON_BN_CLICKED(IDC_DeleteRecord_Button, &CStanderdDlg::OnBnClickedDeleterecordButton)
	ON_BN_CLICKED(IDC_Modify_Button, &CStanderdDlg::OnBnClickedModifyButton)
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_Standerd, &CStanderdDlg::OnLvnItemchangedListStanderd)
END_MESSAGE_MAP()


BOOL CStanderdDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	CWnd * pWnd = NULL;
	m_hCtlBrush = CreateSolidBrush(RGB(255, 255, 255));

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
	pWnd = GetDlgItem(IDC_LIST_Standerd);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	ControlsResizeProp itemListReagents;
	itemAtGroupSelectConditions.ID = IDC_LIST_Standerd;
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

	// 设置“标准点个数”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_StdPointCount);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_StdPointCount;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点1”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_Std1);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std1;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点2”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_Std2);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std2;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点3”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_Std3);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std3;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点4”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_Std4);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std4;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点5”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_Std5);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std5;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点6”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_Std6);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std6;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点7”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_Std7);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std7;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点8”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_Std8);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std8;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点9”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_Std9);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std9;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点10”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_Std10);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std10;
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

	// 设置“标准点个数”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_StdPointCount);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_StdPointCount;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点1”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_Std1);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std1;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点2”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_Std2);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std2;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点3”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_Std3);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std3;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点4”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_Std4);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std4;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点5”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_Std5);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std5;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点2”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_Std6);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std6;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点2”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_Std7);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std7;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点2”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_Std8);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std8;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点2”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_Std9);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std9;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 设置“标准点2”数据标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Data_Std10);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std10;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// 重定位基点是左上顶点
	SetResize(itemAtGroupDetails);

	// 终止生产日期初始化为今天，起始生产日期初始化为半年(180天)以前
	COleDateTime defaultTime;
	defaultTime = COleDateTime::GetCurrentTime();
	m_ctrlProductDateTo.SetTime(defaultTime);

	COleDateTimeSpan timespan(180,0,0,0);
	defaultTime = defaultTime - timespan;
	m_ctrlProductDateFrom.SetTime(defaultTime);

	// 标准品列表初始化
	m_ctrlStandardsList.InsertColumn(colBatchNum,_T("批号"),LVCFMT_CENTER);
	m_ctrlStandardsList.InsertColumn(colProject,_T("检测项目"),LVCFMT_CENTER);
	m_ctrlStandardsList.InsertColumn(colProductDate,_T("生产日期"),LVCFMT_CENTER);

	ColumnProperty column;

	// "批号"列宽为控件总宽的0.35，最小宽度60
	column.ColumnIndex = colBatchNum;
	column.Width = 100;
	column.WidthRatio = 0.35;
	m_ctrlStandardsList.SetColumnWidthProp(column);

	// "批号"列宽为控件总宽的0.35，最小宽度60
	column.ColumnIndex = colProject;
	column.Width = 100;
	column.WidthRatio = 0.35;
	m_ctrlStandardsList.SetColumnWidthProp(column);

	// "批号"列宽为控件总宽的0.3，最小宽度60
	column.ColumnIndex = colProductDate;
	column.Width = 100;
	column.WidthRatio = 0.3;
	m_ctrlStandardsList.SetColumnWidthProp(column);

	m_ctrlStandardsList.SetItemHeight(25);
	m_ctrlStandardsList.EnableSelect(TRUE);

	// 初始化“项目名称”组合框
	_InitComboProjectName();

	// 刷新“标准品列表”
	_RefreshStandardsList();
	
	// 刷新“标准品详细信息区”
	m_nCurrentSelectedStanderd = 0;
	_RefreshDetails();
	return TRUE;
}

// CStanderdDlg 消息处理程序

void CStanderdDlg::OnBnClickedAddrecordButton()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CStanderdDlg::OnCbnSelchangeComboPojectname()
{
	_RefreshStandardsList();
	m_nCurrentSelectedStanderd = 0;
	_RefreshDetails();
}

void CStanderdDlg::OnDtnDatetimechangeDateProductfrom(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	_RefreshStandardsList();
	m_nCurrentSelectedStanderd = 0;
	_RefreshDetails();
	*pResult = 0;
}

void CStanderdDlg::OnDtnDatetimechangeDateProductto(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	_RefreshStandardsList();
	m_nCurrentSelectedStanderd = 0;
	_RefreshDetails();
	*pResult = 0;
}

void CStanderdDlg::OnBnClickedDeleterecordButton()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CStanderdDlg::OnBnClickedModifyButton()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CStanderdDlg::OnPaint()
{
	CPaintDC dc(this);
	CBrush brWhiteBackGround(RGB(255,255,255));
	dc.FillRgn(&m_rgnBackGround,&brWhiteBackGround);
}

HBRUSH CStanderdDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return m_hCtlBrush;
}

void CStanderdDlg::_RefreshStandardsList()
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
	CStandardTable * pTable = CStandardTable::GetInstance();

	vector<StandardRecord> vtRecordList;
	pTable->GetStandards(
		/*vector<ReagentRecord> & */vtRecordList,
		/*const bool &IsFuzzyQuery = */false,
		/*const Reagent_Key &ID =*/ 0,					          
		/*const CString &strBatchNum =*/ _T(""),			         
		/*const CString &strProjectName =*/strProjecName,			            
		/*const CString &strShortName = */_T(""),                  
		/*const DATE &ProductionDateUpper = */timeProductDateTo,
		/*const DATE &ProductionDateLower = */timeProductDateFrom);

	// 刷新前，需要先禁止列表重绘
	m_ctrlStandardsList.SetRedraw(FALSE);
	// 在试剂列表中插入查询到的试剂项
	m_ctrlStandardsList.DeleteAllItemsAndDatas();
	for(int i=0; i < (int)(vtRecordList.size()); i++)
	{ 
		// 试剂ID
		Reagent_Key key = vtRecordList[i].ID;

		// 试剂批号取得
		CString strBatchNum = vtRecordList[i].BatchNum;

		// 项目全称和简称取得
		ProjectFullName projectname;
		projectname.ProjectName = vtRecordList[i].ProjectName;
		projectname.ShortName = vtRecordList[i].ShortName;

		// 生产日期取得
		COleDateTime dtProductDate = vtRecordList[i].ProductionDate;
		// 构造试剂列表项
		CStandardsSelectListItem * pNewItem = new CStandardsSelectListItem(key,strBatchNum,projectname,dtProductDate);
		m_ctrlStandardsList.AddItem(i,static_cast<CListItemData *>(pNewItem));
	}

	// 默认排序
	m_ctrlStandardsList.Reorder();

	// 把试剂列表设置为焦点窗口
	m_ctrlStandardsList.SetFocus();
	m_ctrlStandardsList.SetRedraw(TRUE);
}

void CStanderdDlg::_InitComboProjectName()
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
void CStanderdDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	if (bShow == TRUE)
	{
		// 把列表设成焦点状态，避免单击两下才能选中某条记录
		m_ctrlStandardsList.SetFocus();
	} 
}


void CStanderdDlg::_RefreshDetails()
{
	CWnd * pWnd = NULL;
	// 如果当前试剂ID有效，则设置详细信息，否则设置为空
	if(m_nCurrentSelectedStanderd > 0)
	{
		CStandardTable * pTable = CStandardTable::GetInstance();
		StandardRecord record = pTable->GetRecordInfo(m_nCurrentSelectedStanderd);
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

		// 标准品详细信息“标准点个数”
		strTemp.Format(_T("%d"),record.CountOfStdPoints);
		pWnd = GetDlgItem(IDC_Data_StdPointCount);
		pWnd->SetWindowText(strTemp);

		// 先隐藏标注点5~10
		for (int i=5; i<10; i++)
		{
			m_ctrlStdPointTitle[i].SetWindowText(_T(""));
			m_ctrlStdPointValue[i].SetWindowText(_T(""));
		}

		// 标准点
		for(int i = 0; i < record.CountOfStdPoints; i++)
		{
			int indexConc = i + 1;

			strTemp.Format(_T("%c浓度点"),indexConc + 64);
			m_ctrlStdPointTitle[i].SetWindowText(strTemp);

			strTemp.Format(_T("%g"),record.StdConc[i]);
			m_ctrlStdPointValue[i].SetWindowText(strTemp);
		}
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

		// 标准品详细信息“标准点个数”
		pWnd = GetDlgItem(IDC_Data_StdPointCount);
		pWnd->SetWindowText(_T(""));

		// 隐藏标准点个数
		for (int i=0; i<10; i++)
		{
			m_ctrlStdPointTitle[i].SetWindowText(_T(""));
			m_ctrlStdPointValue[i].SetWindowText(_T(""));
		}
	}
}
void CStanderdDlg::OnLvnItemchangedListStanderd(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(pNMLV->uNewState & ODS_SELECTED)
	{
		CStandardsSelectListItem * pItem = reinterpret_cast<CStandardsSelectListItem *>(m_ctrlStandardsList.GetItemData(pNMLV->iItem));
		// 当前试剂ID设为当前选中项的数据
		m_nCurrentSelectedStanderd = pItem->GetStandardsKey();
		_RefreshDetails();
	}
	*pResult = 0;
}
