// ReagentAreaManDlg.cpp : ʵ���ļ�
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

// CReagentAreaManDlg ��Ϣ�������

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

	// ���á��Լ����ؼ����ġ����������ԡ�
	ControlsResizeProp item;
	m_ctrlReagentArea.GetWindowRect(rect);
	ScreenToClient(rect);
	item.ID = IDC_Ctrl_ReagentArea;
	item.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������϶���
	item.ctmXofPosBps = tmProportion;			// ����x���갴�����Ŵ�
	item.ctmYofPosBps = tmProportion;			// ����y�ᰴ�����Ŵ�
	item.ctmWigth = tmProportion;				// �ؼ���Ȱ���������
	item.ctmHight = tmProportion;				// �ؼ����Ȱ���������
	SetResize(item);

	// ���á��Լ����б��ġ����������ԡ�
	ControlsResizeProp itemList;
	m_ctrlReagentList.GetWindowRect(rect);
	ScreenToClient(rect);
	itemList.ID = IDC_LIST_ReagentArea;
	itemList.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������¶���
	itemList.ctmXofPosBps = tmProportion;			// ����x���갴�����Ŵ�
	itemList.ctmYofPosBps = tmProportion;			// ����y�ᰴ�����Ŵ�
	itemList.ctmWigth = tmProportion;				// �ؼ���Ȱ���������
	itemList.ctmHight = tmProportion;				// �ؼ����Ȱ���������
	SetResize(itemList);

	// ���������б��ʼ��
	m_ctrlReagentList.InsertColumn(colPos,_T("λ�ñ��"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colProject,_T("�����Ŀ����"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colBatchNum,_T("�Լ���������"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colSampleAmount,_T("��������"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colMagnetciAmount,_T("�ű�����"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colHRPAmount,_T("ø������"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colDilution,_T("ϡ�ͱ���"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colWashTimes,_T("��ϴ����"),LVCFMT_CENTER);
	m_ctrlReagentList.InsertColumn(colIncuTime,_T("����ʱ��"),LVCFMT_CENTER);

	ColumnProperty column;
	// "λ�ñ��"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = colPos;
	column.Width = 0;
	column.WidthRatio = 0;
	m_ctrlReagentList.SetColumnWidthProp(column);	

	// "�����Ŀ"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = colProject;
	column.Width = 0;
	column.WidthRatio = 0.15;
	m_ctrlReagentList.SetColumnWidthProp(column);	

	// "��������"�п�Ϊ��ͷ���
	column.ColumnIndex = colBatchNum;
	column.Width = 0;
	column.WidthRatio = 0.15;
	m_ctrlReagentList.SetColumnWidthProp(column);

	// "��������"�п�Ϊ��ͷ���
	column.ColumnIndex = colSampleAmount;
	column.Width = 0;
	m_ctrlReagentList.SetColumnWidthProp(column);

	// "�ű�����"�п�Ϊ��ͷ���
	column.ColumnIndex = colMagnetciAmount;
	column.Width = 0;
	m_ctrlReagentList.SetColumnWidthProp(column);

	// "ø������"�п�Ϊ��ͷ���
	column.ColumnIndex = colHRPAmount;
	column.Width = 0;
	m_ctrlReagentList.SetColumnWidthProp(column);

	// "ϡ�ͱ���"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = colDilution;
	column.Width = 0;
	m_ctrlReagentList.SetColumnWidthProp(column);
	// "��ϴ����"�п�Ϊ��ͷ���
	column.ColumnIndex = colWashTimes;
	column.Width = 0;
	m_ctrlReagentList.SetColumnWidthProp(column);

	// "����ʱ��"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = colIncuTime;
	column.Width = 0;
	m_ctrlReagentList.SetColumnWidthProp(column);

	m_ctrlReagentList.EnableSelect(FALSE);

	for(int i=0; i<NUM_REAGENTPOS; i++)
	{
		CReagentAreaListItem * pNewItem = new CReagentAreaListItem(i + 1);
		m_ctrlReagentList.AddItem(i,static_cast<CListItemData *>(pNewItem));
	}

	// ����������ͼ�ؼ���ָ�봫�롰�Լ������ݡ�����
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
