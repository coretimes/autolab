// CurveDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "CurveDlg.h"
#include "StandardTable.h"
#include "CurveTable.h"
#include "CurveListItem.h"
#include "CurveDetailListItem.h"
#include "GeneralFuction.h"

// CCurveDlg �Ի���

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

	// ���á���ѯ��������ؼ��ġ����������ԡ�
	CWnd * pWnd = GetDlgItem(IDC_GrpTitle_SelectConditions);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	ControlsResizeProp item;
	item.ID = IDC_GrpTitle_SelectConditions;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	item.ctmXofPosBps = tmProportion;				// ����x���갴�����Ŵ�
	item.ctmYofPosBps = tmProportion;				// ����y�ᰴ�����Ŵ�
	item.ctmWigth = tmProportion;					// �ؼ���Ȱ���������
	item.ctmHight = tmProportion;					// �ؼ����Ȱ���������
	item.IsPaintBackGround = false;					// GroupBox�ؼ����Ի汳��
	SetResize(item);

	// ���á���ϸ��Ϣ����ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_GrpTitle_CurveDetails);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	item.ID = IDC_GrpTitle_CurveDetails;
	SetResize(item);

	// ���á������Ŀ����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_ProjectName);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	item.ID = IDC_Title_ProjectName;
	item.ctmHight = tmNotChange;					// �ؼ��߶Ȳ���
	SetResize(item);

	// ���á������Ŀ����Ͽؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_COMBO_PojectName);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	item.ID = IDC_COMBO_PojectName;
	item.ctmHight = tmNotChange;					// �ؼ��߶Ȳ���
	SetResize(item);

	// ���á����š���ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_BatchNum);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	item.ID = IDC_Title_BatchNum;
	item.ctmHight = tmNotChange;					// �ؼ��߶Ȳ���
	SetResize(item);

	// ���á����š���Ͽؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Combo_BatchNum);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Combo_BatchNum;
	item.ctmHight = tmNotChange;					// �ؼ��߶Ȳ���
	SetResize(item);

	// ���á��������ڡ���ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_TestDate);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	item.ID = IDC_Title_TestDate;
	item.ctmHight = tmNotChange;					// �ؼ��߶Ȳ���
	SetResize(item);

	// ���á�������ʼ���ڡ����ڿؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Date_TestDateFrom);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	item.ID = IDC_Date_TestDateFrom;
	item.ctmHight = tmNotChange;					// �ؼ��߶Ȳ���
	SetResize(item);

	// ���á�������ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_TildeSymbol);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_TildeSymbol;
	item.ctmHight = tmNotChange;					// �ؼ��߶Ȳ���
	SetResize(item);

	// ���á�������ֹ���ڡ����ڿؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Date_TestDateTo);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	item.ID = IDC_Date_TestDateTo;
	item.ctmHight = tmNotChange;					// �ؼ��߶Ȳ���
	SetResize(item);

	// ���á������б��ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_LIST_Curve);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	item.ID = IDC_LIST_Curve;
	item.ctmHight = tmProportion;					// �ؼ��߶Ȱ���������
	SetResize(item);

	// ���á���ϸ��Ϣ����ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_GrpTitle_CurveDetails);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_GrpTitle_CurveDetails;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	item.IsPaintBackGround = false;					// GroupBox�ؼ����Ի汳��
	SetResize(item);

	// ���á��������ݡ���ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_CurveData);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_CurveData;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á��������ݡ��б�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_LIST_CurveData);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_LIST_CurveData;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á�����ʾ��ͼ����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_CurveDiagram);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_CurveDiagram;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á�����ʾ��ͼ���ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Static_CurveDrawing);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Static_CurveDrawing;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á������߶ȡ���ѡ��ť�ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Raido_LogScale);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Raido_LogScale;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á�ʵ�ʳ߶ȡ���ѡ��ť�ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Raido_ActualScale);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Raido_ActualScale;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á������߶ȡ���ѡ��ť�ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Check_Spline);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Check_Spline;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á������߶ȡ���ѡ��ť�ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Check_FourParams);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Check_FourParams;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á�ɾ������ť�Ŀ���������
	pWnd = GetDlgItem(IDC_DeleteRecord_Button);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);

	CPoint ptDeleteButtonTopRight;
	ptDeleteButtonTopRight.x = rect.right;
	ptDeleteButtonTopRight.y = rect.top;

	item.ID = IDC_DeleteRecord_Button;
	item.ptOrigPosBps = ptDeleteButtonTopRight;		// �ض�λ���㡰ɾ������ť�����Ͻ�
	item.ctmWigth = tmNotChange;					// �ؼ���Ȳ���
	item.ctmHight = tmNotChange;					// �ؼ��߶Ȳ���
	SetResize(item);

	// ���á��޸ġ���ť�Ŀ���������
	item.ID = IDC_Modify_Button;
	SetResize(item);

	// ���á���ӡ����ť�Ŀ���������
	item.ID = IDC_Print_Button;
	SetResize(item);

	// ��ʼ������Ŀ���ơ���Ͽ�
	_InitComboProjectName();
	// ��ʼ�������š���Ͽ�
	_InitComboBatchNums();

	// ��ֹ�������ڳ�ʼ��Ϊ����24����ǰ����ʼ�������ڳ�ʼ��Ϊ����(7��)��ǰ
	COleDateTime defaultTime;
	defaultTime = COleDateTime::GetCurrentTime();
	defaultTime.SetDateTime(defaultTime.GetYear(),defaultTime.GetMonth(),defaultTime.GetDay(),23,59,59);
	m_ctrlTestDateTo.SetTime(defaultTime);
	COleDateTimeSpan timespan(6,0,0,0);
	defaultTime = defaultTime - timespan;
	defaultTime.SetDateTime(defaultTime.GetYear(),defaultTime.GetMonth(),defaultTime.GetDay(),0,0,0);

	m_ctrlTestDateFrom.SetTime(defaultTime);

	// ���������б��ʼ��
	m_ctrlCurveList.InsertColumn(colProject,_T("�����Ŀ"),LVCFMT_CENTER);
	m_ctrlCurveList.InsertColumn(colBatchNum,_T("����"),LVCFMT_CENTER);
	m_ctrlCurveList.InsertColumn(colTestDate,_T("��������"),LVCFMT_CENTER);
	m_ctrlCurveList.InsertColumn(colDefaultFitType,_T("��������"),LVCFMT_CENTER);
	m_ctrlCurveList.InsertColumn(colSurveyor,_T("����Ա"),LVCFMT_CENTER);

	ColumnProperty column;

	// "�����Ŀ"�п�Ϊ�ؼ��ܿ��0.23����С���60
	column.ColumnIndex = colProject;
	column.Width = 60;
	column.WidthRatio = 0.23;
	m_ctrlCurveList.SetColumnWidthProp(column);

	// "����"�п�Ϊ�ؼ��ܿ��0.23����С���60
	column.ColumnIndex = colBatchNum;
	column.Width = 60;
	column.WidthRatio = 0.23;
	m_ctrlCurveList.SetColumnWidthProp(column);

	// "��������"�п�Ϊ�ؼ��ܿ��0.20����С���60
	column.ColumnIndex = colTestDate;
	column.Width = 60;
	column.WidthRatio = 0.18;
	m_ctrlCurveList.SetColumnWidthProp(column);

	// "Ĭ�����ģ��"�п�Ϊ�ؼ��ܿ��0.17����С���60
	column.ColumnIndex = colDefaultFitType;
	column.Width = 60;
	column.WidthRatio = 0.18;
	m_ctrlCurveList.SetColumnWidthProp(column);

	// "����Ա"�п�Ϊ�ؼ��ܿ��0.17����С���60
	column.ColumnIndex = colSurveyor;
	column.Width = 60;
	column.WidthRatio = 0.18;
	m_ctrlCurveList.SetColumnWidthProp(column);
	m_ctrlCurveList.SetItemHeight(25);

	// ��׼Ʒ������ϸ��Ϣ�б��ʼ��
	m_ctrlDetailsList.InsertColumn(colIndex,_T("��׼��"),LVCFMT_CENTER);
	m_ctrlDetailsList.InsertColumn(colTheoryConc,_T("����Ũ��"),LVCFMT_CENTER);
	m_ctrlDetailsList.InsertColumn(colTestRlu,_T("��������ֵ"),LVCFMT_CENTER);
	m_ctrlDetailsList.InsertColumn(colAverageRlu,_T("ƽ������ֵ"),LVCFMT_CENTER);
	m_ctrlDetailsList.InsertColumn(colCalConc,_T("����Ũ��"),LVCFMT_CENTER);
	m_ctrlDetailsList.InsertColumn(colFitError,_T("������"),LVCFMT_CENTER);
	m_ctrlDetailsList.InsertColumn(colCV,_T("����ϵ��"),LVCFMT_CENTER);
	m_ctrlDetailsList.InsertColumn(colInhibitionRate,_T("������"),LVCFMT_CENTER);

	// "��׼������"�п�Ϊ������
	column.ColumnIndex = colIndex;
	column.Width = 0;
	column.WidthRatio = 0;
	m_ctrlDetailsList.SetColumnWidthProp(column);

	// "����Ũ��"�п�Ϊ������
	column.ColumnIndex = colTheoryConc;
	column.Width = 80;
	column.WidthRatio = 0.17;
	m_ctrlDetailsList.SetColumnWidthProp(column);

	// "��������ֵ"�п�Ϊ������
	column.ColumnIndex = colTestRlu;
	column.Width = 0;
	column.WidthRatio = 0;
	m_ctrlDetailsList.SetColumnWidthProp(column);

	// "ƽ������ֵ"�п�Ϊ������
	column.ColumnIndex = colAverageRlu;
	column.Width = 0;
	column.WidthRatio = 0;
	m_ctrlDetailsList.SetColumnWidthProp(column);

	// "����Ũ��"�п�Ϊ������
	column.ColumnIndex = colCalConc;
	column.Width = 80;
	column.WidthRatio = 0.17;
	m_ctrlDetailsList.SetColumnWidthProp(column);

	// "������"�п�Ϊ������
	column.ColumnIndex = colFitError;
	column.Width = 0;
	column.WidthRatio = 0;
	m_ctrlDetailsList.SetColumnWidthProp(column);


	// "����ϵ��"�п�Ϊ������
	column.ColumnIndex = colCV;
	column.Width = 0;
	column.WidthRatio = 0;
	m_ctrlDetailsList.SetColumnWidthProp(column);

	// "������"�п�Ϊ������
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

	// �����һ������λ��
	m_ctrlProjectName.InsertString(0,_T(""));

	// �������������Ŀ����
	for(int i = 0; i < (int)(m_AllProjectNameList.size()); i++)
	{
		// ��Ŀ�����б������ʽΪ�� ��Ŀ��ƣ���Ŀȫ�ƣ�
		CString strDuName;
		strDuName.Format(_T("%s (%s)"),m_AllProjectNameList[i].ShortName,m_AllProjectNameList[i].ProjectName);
		m_ctrlProjectName.InsertString( i + 1,strDuName);	
	}
}

void CCurveDlg::_InitComboBatchNums()
{
	CStandardTable * pTable = CStandardTable::GetInstance();
	pTable->GetDistinctBatchNums(m_AllBatchNumList);

	// �����һ������λ��
	m_ctrlBatchNum.InsertString(0,_T(""));

	// �������������Ŀ����
	for(int i = 0; i < (int)(m_AllProjectNameList.size()); i++)
	{
		// ��Ŀ�����б������ʽΪ�� ��Ŀ��ƣ���Ŀȫ�ƣ�
		CString strDuName;
		strDuName.Format(_T("%s"),m_AllBatchNumList[i]);
		m_ctrlBatchNum.InsertString( i + 1,strDuName);	
	}
}

void CCurveDlg::_RefreshCurveList()
{
	// ȡ����Ŀ����
	int index = m_ctrlProjectName.GetCurSel();
	CString strProjecName,strShortName;
	if(index > 0)
	{
		strProjecName = m_AllProjectNameList[index - 1].ProjectName;
		strShortName = m_AllProjectNameList[index - 1].ShortName;
	}
	// ȡ������
	index = m_ctrlBatchNum.GetCurSel();
	CString strBatchNum;
	if(index > 0)
		strBatchNum = m_AllBatchNumList[index - 1];

	// ȡ����������
	COleDateTime timeTestDateFrom,timeTestDateTo;
	m_ctrlTestDateFrom.GetTime(timeTestDateFrom);
	m_ctrlTestDateTo.GetTime(timeTestDateTo);

	// ȡ�����߼�¼
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

	// ˢ��ǰ����Ҫ�Ƚ�ֹ�б��ػ�
	m_ctrlCurveList.SetRedraw(FALSE);
	// ���Լ��б��в����ѯ�����Լ���
	m_ctrlCurveList.DeleteAllItemsAndDatas();
	for(int i=0; i < (int)(vtRecordList.size()); i++)
	{ 
		// �����Լ��б���
		CCurveListItem * pNewItem = new CCurveListItem(vtRecordList[i]);
		m_ctrlCurveList.AddItem(i,static_cast<CListItemData *>(pNewItem));
	}

	// Ĭ������
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
		// ���б���ɽ���״̬�����ⵥ�����²���ѡ��ĳ����¼
		m_ctrlCurveList.SetFocus();
	} 
}

void CCurveDlg::OnLvnItemchangedListCurve(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if(pNMLV->uNewState & ODS_SELECTED)
	{
		CCurveListItem * pItem = reinterpret_cast<CCurveListItem *>(m_ctrlCurveList.GetItemData(pNMLV->iItem));
		// ��ǰ�Լ�ID��Ϊ��ǰѡ���������
		m_nCurrentSelectedCurve = pItem->GetCurveKey();
		_RefreshDetails();
	}
	*pResult = 0;
}

void CCurveDlg::_RefreshDetails()
{
	// ˢ�����������б�
	m_ctrlDetailsList.SetRedraw( FALSE);
	m_ctrlDetailsList.DeleteAllItemsAndDatas();
	if (m_nCurrentSelectedCurve > 0)
	{
		// ȡ���������ݿ��
		CCurveTable * pCurveTable = CCurveTable::GetInstance();

		// ȡ�����߼�¼
		CurveRecord recCurve = pCurveTable->GetRecordInfo(m_nCurrentSelectedCurve);

		// ȡ�ñ�׼Ʒ��¼
		CStandardTable *pStandand = CStandardTable::GetInstance();
		StandardRecord recStandard = pStandand->GetRecordInfo(recCurve.StandardID);

		// ȡ��ͨ�ú���
		CGeneralFuction *pFunction = CGeneralFuction::GetInstance();

		// ��������ֵ�б�
		for(int nConcIndex = 0; nConcIndex < 10; nConcIndex ++ )
		{
			// ��������ֵ�ַ�����ȡ������ֵ
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
	// ˢ������ʾ��ͼ����
}