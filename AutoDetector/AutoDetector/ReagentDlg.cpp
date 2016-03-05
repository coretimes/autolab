
#include "stdafx.h"
#include "AutoDetector.h"
#include "ReagentDlg.h"
#include "MemDC.h"
#include "ReagentSelectListItem.h"
// CReagentDlg �Ի���

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

	// ���á���ѯ��������ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_GrpTitle_SelectConditions);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	ControlsResizeProp itemGroupSelectConditions;
	itemGroupSelectConditions.ID = IDC_GrpTitle_SelectConditions;
	itemGroupSelectConditions.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������϶���
	itemGroupSelectConditions.ctmXofPosBps = tmProportion;				// ����x���갴�����Ŵ�
	itemGroupSelectConditions.ctmYofPosBps = tmProportion;				// ����y�ᰴ�����Ŵ�
	itemGroupSelectConditions.ctmWigth = tmProportion;					// �ؼ���Ȱ���������
	itemGroupSelectConditions.ctmHight = tmProportion;					// �ؼ����Ȱ���������
	itemGroupSelectConditions.IsPaintBackGround = false;				// GroupBox�ؼ����Ի汳��
	SetResize(itemGroupSelectConditions);

	// �ڡ���ѯ���������пؼ�����������
	ControlsResizeProp itemAtGroupSelectConditions;
	itemAtGroupSelectConditions.ctmXofPosBps = tmProportion;			// ����x���갴�����Ŵ�
	itemAtGroupSelectConditions.ctmYofPosBps = tmProportion;			// ����y�ᰴ�����Ŵ�
	itemAtGroupSelectConditions.ctmWigth = tmProportion;				// �ؼ���Ȱ���������
	itemAtGroupSelectConditions.ctmHight = tmNotChange;					// �ؼ��߶Ȳ���

	// ���á������Ŀ����ǩ�Ŀ���������
	pWnd = GetDlgItem(IDC_Title_ProjectName);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupSelectConditions.ID = IDC_Title_ProjectName;
	itemAtGroupSelectConditions.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������϶���
	SetResize(itemAtGroupSelectConditions);

	// ���á������Ŀ����Ͽ�Ŀ���������
	pWnd = GetDlgItem(IDC_COMBO_PojectName);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupSelectConditions.ID = IDC_COMBO_PojectName;
	itemAtGroupSelectConditions.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������϶���
	SetResize(itemAtGroupSelectConditions);

	// ���á��������ڡ���ǩ�Ŀ���������
	pWnd = GetDlgItem(IDC_Title_ProductDate);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupSelectConditions.ID = IDC_Title_ProductDate;
	itemAtGroupSelectConditions.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������϶���
	SetResize(itemAtGroupSelectConditions);

	// ���á�������ʼ���ڡ���ǩ�Ŀ���������
	pWnd = GetDlgItem(IDC_Title_ProductDateFrom);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupSelectConditions.ID = IDC_Title_ProductDateFrom;
	itemAtGroupSelectConditions.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������϶���
	SetResize(itemAtGroupSelectConditions);

	// ���á�������ʼ���ڡ��ؼ��Ŀ���������
	pWnd = GetDlgItem(IDC_Date_ProductFrom);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupSelectConditions.ID = IDC_Date_ProductFrom;
	itemAtGroupSelectConditions.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������϶���
	SetResize(itemAtGroupSelectConditions);

	// ���á�������ֹ���ڡ���ǩ�Ŀ���������
	pWnd = GetDlgItem(IDC_Title_ProductDateTo);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupSelectConditions.ID = IDC_Title_ProductDateTo;
	itemAtGroupSelectConditions.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������϶���
	SetResize(itemAtGroupSelectConditions);

	// ���á�������ֹ���ڡ��ؼ��Ŀ���������
	pWnd = GetDlgItem(IDC_Date_ProductTo);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupSelectConditions.ID = IDC_Date_ProductTo;
	itemAtGroupSelectConditions.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������϶���
	SetResize(itemAtGroupSelectConditions);

	// ���á��Լ��б��ؼ��Ŀ���������
	pWnd = GetDlgItem(IDC_LIST_Reagents);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	ControlsResizeProp itemListReagents;
	itemAtGroupSelectConditions.ID = IDC_LIST_Reagents;
	itemAtGroupSelectConditions.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������϶���
	itemAtGroupSelectConditions.ctmHight = tmProportion;				// �ؼ��߶Ȱ���������
	SetResize(itemAtGroupSelectConditions);

	// ���á�ɾ������ť�Ŀ���������
	pWnd = GetDlgItem(IDC_DeleteRecord_Button);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);

	CPoint ptDeleteButtonTopRight;
	ptDeleteButtonTopRight.x = rect.right;
	ptDeleteButtonTopRight.y = rect.top;

	itemAtGroupSelectConditions.ID = IDC_DeleteRecord_Button;
	itemAtGroupSelectConditions.ptOrigPosBps = ptDeleteButtonTopRight;	// �ض�λ���㡰ɾ������ť�����Ͻ�
	itemAtGroupSelectConditions.ctmWigth = tmNotChange;					// �ؼ���Ȳ���
	itemAtGroupSelectConditions.ctmHight = tmNotChange;					// �ؼ��߶Ȳ���
	SetResize(itemAtGroupSelectConditions);

	// ���á��޸ġ���ť�Ŀ���������
	itemAtGroupSelectConditions.ID = IDC_Modify_Button;
	itemAtGroupSelectConditions.ptOrigPosBps = ptDeleteButtonTopRight;	// �ض�λ���㡰ɾ������ť�����Ͻ�
	itemAtGroupSelectConditions.ctmWigth = tmNotChange;					// �ؼ���Ȳ���
	itemAtGroupSelectConditions.ctmHight = tmNotChange;					// �ؼ��߶Ȳ���
	SetResize(itemAtGroupSelectConditions);

	// ���á���ӡ���ť�Ŀ���������
	itemAtGroupSelectConditions.ID = IDC_AddRecord_Button;
	itemAtGroupSelectConditions.ptOrigPosBps = ptDeleteButtonTopRight;	// �ض�λ���㡰ɾ������ť�����Ͻ�
	itemAtGroupSelectConditions.ctmWigth = tmNotChange;					// �ؼ���Ȳ���
	itemAtGroupSelectConditions.ctmHight = tmNotChange;					// �ؼ��߶Ȳ���
	SetResize(itemAtGroupSelectConditions);

	// ���á���ϸ��Ϣ����ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_GrpTitle_Details);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	ControlsResizeProp itemGroupDetails;
	itemGroupDetails.ID = IDC_GrpTitle_Details;
	itemGroupDetails.ptOrigPosBps = ptDeleteButtonTopRight;				// �ض�λ���������϶���
	itemGroupDetails.ctmXofPosBps = tmProportion;						// ����x���갴�����Ŵ�
	itemGroupDetails.ctmYofPosBps = tmProportion;						// ����y�ᰴ�����Ŵ�
	itemGroupDetails.ctmWigth = tmProportion;							// �ؼ���Ȱ���������
	itemGroupDetails.ctmHight = tmProportion;							// �ؼ����Ȱ���������
	itemGroupDetails.IsPaintBackGround = false;							// GroupBox�ؼ����Ի汳��
	SetResize(itemGroupDetails);

	// �ڡ���ϸ��Ϣ�����пؼ�����������
	ControlsResizeProp itemAtGroupDetails;
	itemAtGroupDetails.ctmXofPosBps = tmProportion;						// ����x���갴�����Ŵ�
	itemAtGroupDetails.ctmYofPosBps = tmProportion;						// ����y�ᰴ�����Ŵ�
	itemAtGroupDetails.ctmWigth = tmProportion;							// �ؼ���Ȱ���������
	itemAtGroupDetails.ctmHight = tmNotChange;							// �ؼ��߶Ȳ���

	// ���á��Լ�ID����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_ID);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_ID;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á��������š���ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_BatchNum);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_BatchNum;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á������Ŀ���ơ���ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_ProjectName2);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_ProjectName2;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á��������ڡ���ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_ProductDate2);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_ProductDate2;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á�ʧЧ���ڡ���ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_ExpirationDate);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_ExpirationDate;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á��״μ���ʱ�䡱��ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_FirstLoadTime);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_FirstLoadTime;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á��״�ʹ��ʱ�䡱��ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_FirstUseTime);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_FirstUseTime;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á��ʵ���������ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_RatedTestTimes);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_RatedTestTimes;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á�ʣ��ʹ�ô�������ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_RemainTimes);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_RemainTimes;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á�������λ����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_Unit);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Unit;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á�������ޡ���ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_UpperLimit);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_UpperLimit;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á�������ޡ���ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_LowerLimit);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_LowerLimit;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á�������������ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_SampleAmount);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_SampleAmount;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á��ű���������ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_MagneticAmount);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_MagneticAmount;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á�ø����������ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_HRPAmount);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_HRPAmount;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á�ϡ�ͱ�������ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_Dilution2);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Dilution2;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���ϴ��������ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_WashTimes);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_WashTimes;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á�����ʱ�䡱��ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_IncubationTime);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_IncubationTime;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á��Լ�ID�����ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_ID);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_ID;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á��������š����ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_BatchNum);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_BatchNum;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á������Ŀ���ơ����ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_ProjectName2);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_ProjectName2;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á��������ڡ����ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_ProductDate2);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_ProductDate2;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á�ʧЧ���ڡ����ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_ExpirationDate);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_ExpirationDate;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á��״μ���ʱ�䡱���ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_FirstLoadTime);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_FirstLoadTime;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á��״�ʹ��ʱ�䡱���ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_FirstUseTime);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_FirstUseTime;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á��ʵ����������ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_RatedTestTimes);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_RatedTestTimes;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á�ʣ��ʹ�ô��������ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_RemainTimes);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_RemainTimes;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á�������λ�����ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_Unit);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Unit;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á�������ޡ����ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_UpperLimit);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_UpperLimit;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á�������ޡ����ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_LowerLimit);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_LowerLimit;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á��������������ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_SampleAmount);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_SampleAmount;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á��ű����������ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_MagneticAmount);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_MagneticAmount;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á�ø�����������ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_HRPAmount);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_HRPAmount;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á�ϡ�ͱ��������ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_Dilution);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Dilution;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���ϴ���������ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_WashTimes);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_WashTimes;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á�����ʱ�䡱���ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_IncubationTime);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_IncubationTime;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ��Ŀ����Ͽ��ʼ��
	_InitComboProjectName();

	// ��ֹ�������ڳ�ʼ��Ϊ���죬��ʼ�������ڳ�ʼ��Ϊ���꣨180�죩��ǰ
	COleDateTime defaultTime;
	defaultTime = COleDateTime::GetCurrentTime();
	m_ctrlProductDateTo.SetTime(defaultTime);

	COleDateTimeSpan timespan(180,0,0,0);
	defaultTime = defaultTime - timespan;

	m_ctrlProductDateFrom.SetTime(defaultTime);

	// �Լ��б��ʼ��
	m_ctrlReagentList.InsertColumn(colBatchNum,_T("����"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colProject,_T("�����Ŀ"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colProductDate,_T("��������"),LVCFMT_CENTER);

	ColumnProperty column;

	// "����"�п�Ϊ�ؼ��ܿ��0.35����С���60
	column.ColumnIndex = colBatchNum;
	column.Width = 100;
	column.WidthRatio = 0.35;
	m_ctrlReagentList.SetColumnWidthProp(column);

	// "����"�п�Ϊ�ؼ��ܿ��0.35����С���60
	column.ColumnIndex = colProject;
	column.Width = 100;
	column.WidthRatio = 0.35;
	m_ctrlReagentList.SetColumnWidthProp(column);

	// "����"�п�Ϊ�ؼ��ܿ��0.3����С���60
	column.ColumnIndex = colProductDate;
	column.Width = 100;
	column.WidthRatio = 0.3;
	m_ctrlReagentList.SetColumnWidthProp(column);

	m_ctrlReagentList.SetItemHeight(25);
	m_ctrlReagentList.EnableSelect(TRUE);
	m_ctrlReagentList.SetFocus();

	// ˢ���Լ��б���Լ���ϸ��Ϣ
	_RefreshReagentsList();

	m_nCurrentSelectedReagent = 0;
	_RefreshDetails();
	return TRUE;
}


void CReagentDlg::_InitComboProjectName()
{
	CReagentTable * pTable = CReagentTable::GetInstance();
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

void CReagentDlg::OnBnClickedAddrecordButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// �����ϸ��Ϣ
	m_nCurrentSelectedReagent = 0;
	_RefreshDetails();
	*pResult = 0;
}

void CReagentDlg::OnDtnDatetimechangeDateProductto(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	_RefreshReagentsList();
	// �����ϸ��Ϣ
	m_nCurrentSelectedReagent = 0;
	_RefreshDetails();
	*pResult = 0;
}

void CReagentDlg::OnBnClickedDeleterecordButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CReagentDlg::OnBnClickedModifyButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// ȡ����Ŀ����
	int index = m_ctrlProjectName.GetCurSel();

	CString strProjecName;
	if(index > 0)
		strProjecName = m_AllProjectNameList[index - 1].ProjectName;

	// ȡ����������
	COleDateTime timeProductDateFrom,timeProductDateTo;
	m_ctrlProductDateFrom.GetTime(timeProductDateFrom);
	m_ctrlProductDateTo.GetTime(timeProductDateTo);

	// ȡ���Լ���ָ��
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

	// ˢ��ǰ����Ҫ�Ƚ�ֹ�б��ػ�
	m_ctrlReagentList.SetRedraw(FALSE);
	// ���Լ��б��в����ѯ�����Լ���
	m_ctrlReagentList.DeleteAllItemsAndDatas();
	for(int i=0; i < (int)(vtReagentList.size()); i++)
	{ 
		// �Լ�ID
		Reagent_Key key = vtReagentList[i].ID;

		// �Լ�����ȡ��
		CString strBatchNum = vtReagentList[i].BatchNum;

		// ��Ŀȫ�ƺͼ��ȡ��
		ProjectFullName projectname;
		projectname.ProjectName = vtReagentList[i].ProjectName;
		projectname.ShortName = vtReagentList[i].ShortName;

		// ��������ȡ��
		COleDateTime dtProductDate = vtReagentList[i].ProductionDate;
		// �����Լ��б���
		CReagentSelectListItem * pNewItem = new CReagentSelectListItem(key,strBatchNum,projectname,dtProductDate);
		m_ctrlReagentList.AddItem(i,static_cast<CListItemData *>(pNewItem));
	}

	// Ĭ������
	m_ctrlReagentList.Reorder();
	m_ctrlReagentList.SetRedraw(TRUE);
	// ���Լ��б�����Ϊ���㴰��
	m_ctrlReagentList.SetFocus();
}

void CReagentDlg::_RefreshDetails()
{
	CWnd * pWnd = NULL;
	// �����ǰ�Լ�ID��Ч����������ϸ��Ϣ����������Ϊ��
	if(m_nCurrentSelectedReagent > 0)
	{
		CReagentTable * pTable = CReagentTable::GetInstance();
		ReagentRecord record = pTable->GetRecordInfo(m_nCurrentSelectedReagent);
		CString strTemp;
		COleDateTime dtDate;
		// ��׼Ʒ��ϸ��Ϣ���Լ�ID��
		strTemp.Format(_T("%d"),record.ID);
		pWnd = GetDlgItem(IDC_Data_ID);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ���������š�
		pWnd = GetDlgItem(IDC_Data_BatchNum2);
		pWnd->SetWindowText(record.BatchNum);

		// ��׼Ʒ��ϸ��Ϣ����Ŀ���ơ�
		pWnd = GetDlgItem(IDC_Data_ProjectName2);
		strTemp.Format(_T("%s(%s)"),record.ShortName,record.ProjectName);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ���������ڡ�
		dtDate = record.ProductionDate;
		pWnd = GetDlgItem(IDC_Data_ProductDate2);
		pWnd->SetWindowText(dtDate.Format(_T("%Y-%m-%d")));

		// ��׼Ʒ��ϸ��Ϣ��ʧЧ���ڡ�
		dtDate = record.ExpirationDate;
		pWnd = GetDlgItem(IDC_Data_ExpirationDate);
		pWnd->SetWindowText(dtDate.Format(_T("%Y-%m-%d")));

		// ��׼Ʒ��ϸ��Ϣ���״μ���ʱ�䡱
		dtDate = record.FirstLoadTime;
		pWnd = GetDlgItem(IDC_Data_FirstLoadTime);
		pWnd->SetWindowText(dtDate.Format(_T("%Y-%m-%d")));

		// ��׼Ʒ��ϸ��Ϣ���״�ʹ��ʱ�䡱
		dtDate = record.FirstUseTime;
		pWnd = GetDlgItem(IDC_Data_FirstUseTime);
		pWnd->SetWindowText(dtDate.Format(_T("%Y-%m-%d")));

		// ��׼Ʒ��ϸ��Ϣ���ʵ�������
		strTemp.Format(_T("%d"),record.RatedTestTimes);
		pWnd = GetDlgItem(IDC_Data_RatedTestTimes);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ��ʣ��ʹ�ô�����
		strTemp.Format(_T("%d"),record.RemainTimes);
		pWnd = GetDlgItem(IDC_Data_RemainTimes);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ��������λ��
		pWnd = GetDlgItem(IDC_Data_Unit);
		pWnd->SetWindowText(record.Unit);

		// ��׼Ʒ��ϸ��Ϣ��������ޡ�
		strTemp.Format(_T("%g"),record.UpperLimit);
		pWnd = GetDlgItem(IDC_Data_UpperLimit);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ��������ޡ�
		strTemp.Format(_T("%g"),record.LowerLimit);
		pWnd = GetDlgItem(IDC_Data_LowerLimit);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ������������
		strTemp.Format(_T("%g"),record.SampleAmount);
		pWnd = GetDlgItem(IDC_Data_SampleAmount);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ���ű�������
		strTemp.Format(_T("%g"),record.MagneticAmount);
		pWnd = GetDlgItem(IDC_Data_MagneticAmount);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ��ø��������
		strTemp.Format(_T("%g"),record.HRPAmount);
		pWnd = GetDlgItem(IDC_Data_HRPAmount);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ��ϡ�ͱ�����
		strTemp.Format(_T("%g"),record.Dilution);
		pWnd = GetDlgItem(IDC_Data_Dilution);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ����ϴ������
		strTemp.Format(_T("%g"),record.WashTimes);
		pWnd = GetDlgItem(IDC_Data_WashTimes);
		pWnd->SetWindowText(strTemp);

		// ��׼Ʒ��ϸ��Ϣ������ʱ�䡱
		strTemp.Format(_T("%g"),record.IncubationTime);
		pWnd = GetDlgItem(IDC_Data_IncubationTime);
		pWnd->SetWindowText(strTemp);
	}
	else
	{
		// ��׼Ʒ��ϸ��Ϣ���Լ�ID��
		pWnd = GetDlgItem(IDC_Data_ID);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ���������š�
		pWnd = GetDlgItem(IDC_Data_BatchNum2);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ����Ŀ���ơ�
		pWnd = GetDlgItem(IDC_Data_ProjectName2);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ���������ڡ�
		pWnd = GetDlgItem(IDC_Data_ProductDate2);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ��ʧЧ���ڡ�
		pWnd = GetDlgItem(IDC_Data_ExpirationDate);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ���״μ���ʱ�䡱
		pWnd = GetDlgItem(IDC_Data_FirstLoadTime);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ���״�ʹ��ʱ�䡱
		pWnd = GetDlgItem(IDC_Data_FirstUseTime);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ���ʵ�������
		pWnd = GetDlgItem(IDC_Data_RatedTestTimes);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ��ʣ��ʹ�ô�����
		pWnd = GetDlgItem(IDC_Data_RemainTimes);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ��������λ��
		pWnd = GetDlgItem(IDC_Data_Unit);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ��������ޡ�
		pWnd = GetDlgItem(IDC_Data_UpperLimit);
		pWnd->SetWindowText(_T(""));
		// ��׼Ʒ��ϸ��Ϣ��������ޡ�
		pWnd = GetDlgItem(IDC_Data_LowerLimit);
		pWnd->SetWindowText(_T(""));
		
		// ��׼Ʒ��ϸ��Ϣ������������
		pWnd = GetDlgItem(IDC_Data_SampleAmount);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ���ű�������
		pWnd = GetDlgItem(IDC_Data_MagneticAmount);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ��ø��������
		pWnd = GetDlgItem(IDC_Data_HRPAmount);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ��ϡ�ͱ�����
		pWnd = GetDlgItem(IDC_Data_Dilution);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ����ϴ������
		pWnd = GetDlgItem(IDC_Data_WashTimes);
		pWnd->SetWindowText(_T(""));

		// ��׼Ʒ��ϸ��Ϣ������ʱ�䡱
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
		// ��ǰ�Լ�ID��Ϊ��ǰѡ���������
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
		// ���б���ɽ���״̬�����ⵥ�����²���ѡ��ĳ����¼
		m_ctrlReagentList.SetFocus();
	} 
}
