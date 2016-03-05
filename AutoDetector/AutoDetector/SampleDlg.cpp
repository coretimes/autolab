// SampleDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "SampleDlg.h"


// CSampleDlg 对话框

IMPLEMENT_DYNAMIC(CSampleDlg, CBaseDialog)

CSampleDlg::CSampleDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CSampleDlg::IDD, pParent)
{
}

CSampleDlg::~CSampleDlg()
{
}

void CSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	//DDX_Control(pDX,IDC_STATIC1,m_ctrlSampleArea);
}


BEGIN_MESSAGE_MAP(CSampleDlg, CBaseDialog)
	//ON_STN_CLICKED(IDC_STATIC1, &CSampleDlg::OnStnClickedStatic1)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CSampleDlg 消息处理程序

void CSampleDlg::OnStnClickedStatic1()
{
	// TODO: 在此添加控件通知处理程序代码
}

BOOL CSampleDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	m_hCtlBrush = CreateSolidBrush(RGB(255, 255, 255));

	CRect rect;
	CWnd *pWnd = NULL;
	// 设置“样本编号”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_SampleNo);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	ControlsResizeProp item;
	item.ID = IDC_Title_SampleNo;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	item.ctmXofPosBps = tmProportion;				// 基点x坐标按比例放大
	item.ctmYofPosBps = tmProportion;				// 基点y轴按比例放大
	item.ctmWigth = tmProportion;					// 控件宽度按比例缩放
	item.ctmHight = tmNotChange;					// 控件高度不变
	SetResize(item);

	// 设置“样本编号”编辑控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Edit_SampleNo);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Edit_SampleNo;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	item.ctmXofPosBps = tmProportion;				// 基点x坐标按比例放大
	item.ctmYofPosBps = tmProportion;				// 基点y轴按比例放大
	item.ctmWigth = tmProportion;					// 控件宽度按比例缩放
	item.ctmHight = tmNotChange;					// 控件高度不变
	SetResize(item);

	// 设置“样本类型”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_SampleType);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_SampleType;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“样本类型”组合控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Combo_SampleType);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Combo_SampleType;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“送检单位”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_Dept);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_Dept;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“送检单位”编辑控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Edit_Dept);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Edit_Dept;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“送检人”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_Applicant);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_Applicant;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“送检人”编辑控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Edit_Applicant);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Edit_Applicant;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“检测项目”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_ProjectName);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_ProjectName;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“检测项目”组合控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_COMBO_PojectName);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_COMBO_PojectName;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“检测日期”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_TestDate);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_TestDate;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“检测起始日期”控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Date_TestDateFrom);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Date_TestDateFrom;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“~”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_ProductDateTo);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_ProductDateTo;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“检测终止日期”控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Date_TestDateTo);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Date_TestDateTo;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“检验员”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_Operator);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_Operator;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);
	
	// 设置“检验员”标签控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Title_Operator);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_Operator;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“检验员”编辑控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_Edit_Operator);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Edit_Operator;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	SetResize(item);

	// 设置“样本列表”控件的“可伸缩属性”
	pWnd = GetDlgItem(IDC_List_Sample);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_List_Sample;
	item.ptOrigPosBps = rect.TopLeft();				// 重定位基点是左上顶点
	item.ctmHight = tmProportion;					// 控件高度按比例缩放
	SetResize(item);

	pWnd = GetDlgItem(IDC_DeleteRecord_Button);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	CPoint ptDeleteButtonTopRight;
	ptDeleteButtonTopRight.x = rect.right;
	ptDeleteButtonTopRight.y = rect.top;

	item.ID = IDC_DeleteRecord_Button;
	item.ptOrigPosBps = ptDeleteButtonTopRight;		// 重定位基点“删除”按钮的右上角
	item.ctmXofPosBps = tmProportion;				// 基点x坐标按比例放大
	item.ctmYofPosBps = tmProportion;				// 基点y轴按比例放大
	item.ctmWigth = tmNotChange;					// 控件宽度不变
	item.ctmHight = tmNotChange;					// 控件高度不变
	SetResize(item);

	// 设置“修改”按钮的可伸缩属性
	item.ID = IDC_Modify_Button;
	item.ptOrigPosBps = ptDeleteButtonTopRight;		// 重定位基点“删除”按钮的右上角
	SetResize(item);

	// 设置“添加”按钮的可伸缩属性
	item.ID = IDC_Print_Button;
	item.ptOrigPosBps = ptDeleteButtonTopRight;		// 重定位基点“删除”按钮的右上角
	SetResize(item);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSampleDlg::OnPaint()
{
	CPaintDC dc(this);
	CBrush brWhiteBackGround(RGB(255,255,255));
	dc.FillRgn(&m_rgnBackGround,&brWhiteBackGround);
}

HBRUSH CSampleDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return m_hCtlBrush;
}
