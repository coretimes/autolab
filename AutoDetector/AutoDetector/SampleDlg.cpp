// SampleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "SampleDlg.h"


// CSampleDlg �Ի���

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


// CSampleDlg ��Ϣ�������

void CSampleDlg::OnStnClickedStatic1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

BOOL CSampleDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	m_hCtlBrush = CreateSolidBrush(RGB(255, 255, 255));

	CRect rect;
	CWnd *pWnd = NULL;
	// ���á�������š���ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_SampleNo);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	ControlsResizeProp item;
	item.ID = IDC_Title_SampleNo;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	item.ctmXofPosBps = tmProportion;				// ����x���갴�����Ŵ�
	item.ctmYofPosBps = tmProportion;				// ����y�ᰴ�����Ŵ�
	item.ctmWigth = tmProportion;					// �ؼ���Ȱ���������
	item.ctmHight = tmNotChange;					// �ؼ��߶Ȳ���
	SetResize(item);

	// ���á�������š��༭�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Edit_SampleNo);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Edit_SampleNo;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	item.ctmXofPosBps = tmProportion;				// ����x���갴�����Ŵ�
	item.ctmYofPosBps = tmProportion;				// ����y�ᰴ�����Ŵ�
	item.ctmWigth = tmProportion;					// �ؼ���Ȱ���������
	item.ctmHight = tmNotChange;					// �ؼ��߶Ȳ���
	SetResize(item);

	// ���á��������͡���ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_SampleType);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_SampleType;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á��������͡���Ͽؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Combo_SampleType);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Combo_SampleType;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á��ͼ쵥λ����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_Dept);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_Dept;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á��ͼ쵥λ���༭�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Edit_Dept);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Edit_Dept;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á��ͼ��ˡ���ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_Applicant);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_Applicant;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á��ͼ��ˡ��༭�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Edit_Applicant);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Edit_Applicant;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á������Ŀ����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_ProjectName);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_ProjectName;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á������Ŀ����Ͽؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_COMBO_PojectName);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_COMBO_PojectName;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á�������ڡ���ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_TestDate);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_TestDate;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á������ʼ���ڡ��ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Date_TestDateFrom);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Date_TestDateFrom;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á�~����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_ProductDateTo);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_ProductDateTo;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á������ֹ���ڡ��ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Date_TestDateTo);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Date_TestDateTo;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á�����Ա����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_Operator);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_Operator;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);
	
	// ���á�����Ա����ǩ�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Title_Operator);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Title_Operator;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á�����Ա���༭�ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_Edit_Operator);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Edit_Operator;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	SetResize(item);

	// ���á������б��ؼ��ġ����������ԡ�
	pWnd = GetDlgItem(IDC_List_Sample);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_List_Sample;
	item.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	item.ctmHight = tmProportion;					// �ؼ��߶Ȱ���������
	SetResize(item);

	pWnd = GetDlgItem(IDC_DeleteRecord_Button);
	pWnd->GetWindowRect(rect);
	ScreenToClient(rect);
	CPoint ptDeleteButtonTopRight;
	ptDeleteButtonTopRight.x = rect.right;
	ptDeleteButtonTopRight.y = rect.top;

	item.ID = IDC_DeleteRecord_Button;
	item.ptOrigPosBps = ptDeleteButtonTopRight;		// �ض�λ���㡰ɾ������ť�����Ͻ�
	item.ctmXofPosBps = tmProportion;				// ����x���갴�����Ŵ�
	item.ctmYofPosBps = tmProportion;				// ����y�ᰴ�����Ŵ�
	item.ctmWigth = tmNotChange;					// �ؼ���Ȳ���
	item.ctmHight = tmNotChange;					// �ؼ��߶Ȳ���
	SetResize(item);

	// ���á��޸ġ���ť�Ŀ���������
	item.ID = IDC_Modify_Button;
	item.ptOrigPosBps = ptDeleteButtonTopRight;		// �ض�λ���㡰ɾ������ť�����Ͻ�
	SetResize(item);

	// ���á���ӡ���ť�Ŀ���������
	item.ID = IDC_Print_Button;
	item.ptOrigPosBps = ptDeleteButtonTopRight;		// �ض�λ���㡰ɾ������ť�����Ͻ�
	SetResize(item);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
