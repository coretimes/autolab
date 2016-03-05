#include "stdafx.h"
#include "AutoDetector.h"
#include "StanderdDlg.h"
#include "MemDC.h"
#include "StandardTable.h"
#include "StandardsSelectListItem.h"

// CStanderdDlg �Ի���

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
	pWnd = GetDlgItem(IDC_LIST_Standerd);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	ControlsResizeProp itemListReagents;
	itemAtGroupSelectConditions.ID = IDC_LIST_Standerd;
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

	// ���á���׼���������ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_StdPointCount);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_StdPointCount;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��1����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_Std1);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std1;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��2����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_Std2);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std2;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��3����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_Std3);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std3;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��4����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_Std4);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std4;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��5����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_Std5);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std5;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��6����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_Std6);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std6;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��7����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_Std7);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std7;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��8����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_Std8);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std8;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��9����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_Std9);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std9;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��10����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_Std10);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Title_Std10;
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

	// ���á���׼����������ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_StdPointCount);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_StdPointCount;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��1�����ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_Std1);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std1;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��2�����ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_Std2);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std2;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��3�����ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_Std3);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std3;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��4�����ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_Std4);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std4;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��5�����ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_Std5);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std5;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��2�����ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_Std6);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std6;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��2�����ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_Std7);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std7;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��2�����ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_Std8);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std8;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��2�����ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_Std9);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std9;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ���á���׼��2�����ݱ�ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Data_Std10);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	itemAtGroupDetails.ID = IDC_Data_Std10;
	itemAtGroupDetails.ptOrigPosBps = rect.TopLeft();					// �ض�λ���������϶���
	SetResize(itemAtGroupDetails);

	// ��ֹ�������ڳ�ʼ��Ϊ���죬��ʼ�������ڳ�ʼ��Ϊ����(180��)��ǰ
	COleDateTime defaultTime;
	defaultTime = COleDateTime::GetCurrentTime();
	m_ctrlProductDateTo.SetTime(defaultTime);

	COleDateTimeSpan timespan(180,0,0,0);
	defaultTime = defaultTime - timespan;
	m_ctrlProductDateFrom.SetTime(defaultTime);

	// ��׼Ʒ�б��ʼ��
	m_ctrlStandardsList.InsertColumn(colBatchNum,_T("����"),LVCFMT_CENTER);
	m_ctrlStandardsList.InsertColumn(colProject,_T("�����Ŀ"),LVCFMT_CENTER);
	m_ctrlStandardsList.InsertColumn(colProductDate,_T("��������"),LVCFMT_CENTER);

	ColumnProperty column;

	// "����"�п�Ϊ�ؼ��ܿ��0.35����С���60
	column.ColumnIndex = colBatchNum;
	column.Width = 100;
	column.WidthRatio = 0.35;
	m_ctrlStandardsList.SetColumnWidthProp(column);

	// "����"�п�Ϊ�ؼ��ܿ��0.35����С���60
	column.ColumnIndex = colProject;
	column.Width = 100;
	column.WidthRatio = 0.35;
	m_ctrlStandardsList.SetColumnWidthProp(column);

	// "����"�п�Ϊ�ؼ��ܿ��0.3����С���60
	column.ColumnIndex = colProductDate;
	column.Width = 100;
	column.WidthRatio = 0.3;
	m_ctrlStandardsList.SetColumnWidthProp(column);

	m_ctrlStandardsList.SetItemHeight(25);
	m_ctrlStandardsList.EnableSelect(TRUE);

	// ��ʼ������Ŀ���ơ���Ͽ�
	_InitComboProjectName();

	// ˢ�¡���׼Ʒ�б�
	_RefreshStandardsList();
	
	// ˢ�¡���׼Ʒ��ϸ��Ϣ����
	m_nCurrentSelectedStanderd = 0;
	_RefreshDetails();
	return TRUE;
}

// CStanderdDlg ��Ϣ�������

void CStanderdDlg::OnBnClickedAddrecordButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CStanderdDlg::OnBnClickedModifyButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	// ˢ��ǰ����Ҫ�Ƚ�ֹ�б��ػ�
	m_ctrlStandardsList.SetRedraw(FALSE);
	// ���Լ��б��в����ѯ�����Լ���
	m_ctrlStandardsList.DeleteAllItemsAndDatas();
	for(int i=0; i < (int)(vtRecordList.size()); i++)
	{ 
		// �Լ�ID
		Reagent_Key key = vtRecordList[i].ID;

		// �Լ�����ȡ��
		CString strBatchNum = vtRecordList[i].BatchNum;

		// ��Ŀȫ�ƺͼ��ȡ��
		ProjectFullName projectname;
		projectname.ProjectName = vtRecordList[i].ProjectName;
		projectname.ShortName = vtRecordList[i].ShortName;

		// ��������ȡ��
		COleDateTime dtProductDate = vtRecordList[i].ProductionDate;
		// �����Լ��б���
		CStandardsSelectListItem * pNewItem = new CStandardsSelectListItem(key,strBatchNum,projectname,dtProductDate);
		m_ctrlStandardsList.AddItem(i,static_cast<CListItemData *>(pNewItem));
	}

	// Ĭ������
	m_ctrlStandardsList.Reorder();

	// ���Լ��б�����Ϊ���㴰��
	m_ctrlStandardsList.SetFocus();
	m_ctrlStandardsList.SetRedraw(TRUE);
}

void CStanderdDlg::_InitComboProjectName()
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
void CStanderdDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CBaseDialog::OnShowWindow(bShow, nStatus);
	if (bShow == TRUE)
	{
		// ���б���ɽ���״̬�����ⵥ�����²���ѡ��ĳ����¼
		m_ctrlStandardsList.SetFocus();
	} 
}


void CStanderdDlg::_RefreshDetails()
{
	CWnd * pWnd = NULL;
	// �����ǰ�Լ�ID��Ч����������ϸ��Ϣ����������Ϊ��
	if(m_nCurrentSelectedStanderd > 0)
	{
		CStandardTable * pTable = CStandardTable::GetInstance();
		StandardRecord record = pTable->GetRecordInfo(m_nCurrentSelectedStanderd);
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

		// ��׼Ʒ��ϸ��Ϣ����׼�������
		strTemp.Format(_T("%d"),record.CountOfStdPoints);
		pWnd = GetDlgItem(IDC_Data_StdPointCount);
		pWnd->SetWindowText(strTemp);

		// �����ر�ע��5~10
		for (int i=5; i<10; i++)
		{
			m_ctrlStdPointTitle[i].SetWindowText(_T(""));
			m_ctrlStdPointValue[i].SetWindowText(_T(""));
		}

		// ��׼��
		for(int i = 0; i < record.CountOfStdPoints; i++)
		{
			int indexConc = i + 1;

			strTemp.Format(_T("%cŨ�ȵ�"),indexConc + 64);
			m_ctrlStdPointTitle[i].SetWindowText(strTemp);

			strTemp.Format(_T("%g"),record.StdConc[i]);
			m_ctrlStdPointValue[i].SetWindowText(strTemp);
		}
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

		// ��׼Ʒ��ϸ��Ϣ����׼�������
		pWnd = GetDlgItem(IDC_Data_StdPointCount);
		pWnd->SetWindowText(_T(""));

		// ���ر�׼�����
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
		// ��ǰ�Լ�ID��Ϊ��ǰѡ���������
		m_nCurrentSelectedStanderd = pItem->GetStandardsKey();
		_RefreshDetails();
	}
	*pResult = 0;
}
