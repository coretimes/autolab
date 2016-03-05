// NewTestConfirmDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "NewTestConfirmDlg.h"
#include "SampleAreaData.h"
#include "TestConfirmListItem.h"
#include "TestManTab.h"

IMPLEMENT_DYNAMIC(CNewTestConfirmDlg, CBaseDialog)

CNewTestConfirmDlg::CNewTestConfirmDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CNewTestConfirmDlg::IDD, pParent)
{

}

CNewTestConfirmDlg::~CNewTestConfirmDlg()
{
}

void CNewTestConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_List_TestConfirm,m_ctrlNewTestList);
	DDX_Control(pDX,IDC_Start,m_StartButton);
}


BEGIN_MESSAGE_MAP(CNewTestConfirmDlg, CBaseDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_Start, &CNewTestConfirmDlg::OnBnClickedStart)
END_MESSAGE_MAP()

// CNewTestConfirmDlg ��Ϣ�������

void CNewTestConfirmDlg::OnPaint()
{
	CPaintDC dc(this);
	CBrush brWhiteBackGround(RGB(255,255,255));
	dc.FillRgn(&m_rgnBackGround,&brWhiteBackGround);
}

BOOL CNewTestConfirmDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CNewTestConfirmDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// ���á��Լ����б��ġ����������ԡ�
	ControlsResizeProp itemList;
	CRect rect;
	m_ctrlNewTestList.GetWindowRect(rect);
	ScreenToClient(rect);
	itemList.ID = IDC_List_TestConfirm;
	itemList.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������¶���
	itemList.ctmXofPosBps = tmProportion;			// ����x���갴�����Ŵ�
	itemList.ctmYofPosBps = tmProportion;			// ����y�ᰴ�����Ŵ�
	itemList.ctmWigth = tmProportion;				// �ؼ���Ȱ���������
	itemList.ctmHight = tmProportion;				// �ؼ����Ȱ���������
	SetResize(itemList);

	// ���á���ʼ��ť���ġ����������ԡ�
	ControlsResizeProp itemStartButton;
	m_StartButton.GetWindowRect(rect);
	ScreenToClient(rect);
	itemStartButton.ID = IDC_Start;
	itemStartButton.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������¶���
	itemStartButton.ctmXofPosBps = tmProportion;			// ����x���갴�����Ŵ�
	itemStartButton.ctmYofPosBps = tmProportion;			// ����y�ᰴ�����Ŵ�
	itemStartButton.ctmWigth = tmProportion;				// �ؼ���Ȳ���
	itemStartButton.ctmHight = tmProportion;				// �ؼ����Ȳ���
	SetResize(itemStartButton);

	// ���������б��ʼ��
	m_ctrlNewTestList.InsertColumn(colType,_T("��Ʒ����"),LVCFMT_CENTER);
	m_ctrlNewTestList.InsertColumn(colSampleNo,_T("�������"),LVCFMT_CENTER);
	m_ctrlNewTestList.InsertColumn(colConcPoint,_T("Ũ�ȵ�"),LVCFMT_CENTER);
	m_ctrlNewTestList.InsertColumn(colSamplePos,_T("����λ"),LVCFMT_CENTER);
	m_ctrlNewTestList.InsertColumn(colProject,_T("�����Ŀ"),LVCFMT_CENTER);
	m_ctrlNewTestList.InsertColumn(colRepeatTimes,_T("�ظ�����"),LVCFMT_CENTER);
	
	ColumnProperty column;

	// "��Ʒ����"�п�����Ϊ�ؼ��ܿ��0.15����С���80
	column.ColumnIndex = 0;
	column.Width = 80;
	column.WidthRatio = 0.15;
	m_ctrlNewTestList.SetColumnWidthProp(column);	

	// "�������"�п�Ϊ�ؼ��ܿ��0.25����С���100
	column.ColumnIndex = 1;
	column.Width = 100;
	column.WidthRatio = 0.2;
	m_ctrlNewTestList.SetColumnWidthProp(column);

	// "Ũ�ȵ�"�п�Ϊ�ؼ��ܿ��0.2����С���80
	column.ColumnIndex = 2;
	column.Width = 80;
	column.WidthRatio = 0.15;
	m_ctrlNewTestList.SetColumnWidthProp(column);

	// "����λ"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = 3;
	column.Width = 60;
	column.WidthRatio = 0.15;
	m_ctrlNewTestList.SetColumnWidthProp(column);

	// "�����Ŀ"�п�Ϊ�ؼ��ܿ��0.3����С���140
	column.ColumnIndex = 4;
	column.Width = 140;
	column.WidthRatio = 0.3;
	m_ctrlNewTestList.SetColumnWidthProp(column);

	// "�ظ�����"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = 5;
	column.Width = 0;
	m_ctrlNewTestList.SetColumnWidthProp(column);

	m_ctrlNewTestList.SetItemHeight(25);
	m_ctrlNewTestList.EnableSelect(FALSE);
	m_ctrlNewTestList.SetFocus();

	CSampleAreaData::GetInstance()->AttchDialog(this);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CNewTestConfirmDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (bShow == TRUE)
	{
		CSampleAreaData *pData = CSampleAreaData::GetInstance();
		m_ctrlNewTestList.DeleteAllItemsAndDatas();
		m_ctrlNewTestList.SetRedraw(FALSE);
		for(int pos = 1; pos<= NUM_SAMPLEPOS; pos++)
		{
			// ȡ���������ýڵ�
			SampleConfigNode *pNode = pData->GetNode(pos);
			// ���û���Թܣ������б��в�������
			if(pNode->IsThereTube == false)
				continue;

			// ���û����������������û�в�����Ŀ��
			if(pNode->IsSetingSample == false)
				continue;

			if(pNode->TestProjArr.size() < 1)
				continue;

			// ��������в�����Ŀ��
			// ѭ������������Ŀ����
			map<Reagent_Pos,SampleConfigNode::TestProjInfo>::iterator iter = pNode->TestProjArr.begin();
			while(iter != pNode->TestProjArr.end())
			{
				if(iter->second.TestUintKeys == 0) // ���������еĲ��Ե�Ԫkey���������δ��ʼ�����keyֵΪ0������Ϊ��0	
				{
					CTestConfirmListItem * pNewItem = new CTestConfirmListItem(pNode->PosNum,iter->first);
					m_ctrlNewTestList.AddItem(0,pNewItem);
				}
				iter++;
			}
		}
		m_ctrlNewTestList.Reorder();
		m_ctrlNewTestList.SetRedraw(TRUE);

		// ����б��������ݣ���ʼ��ť��Ϊ��Ч
		if(m_ctrlNewTestList.GetItemCount() > 0)
			m_StartButton.EnableWindow(TRUE);
	}

	CBaseDialog::OnShowWindow(bShow, nStatus);
}

void CNewTestConfirmDlg::OnBnClickedStart()
{
	// �����ɳ©��״
	CWaitCursor wait;

	// ���²��Ի�����
	CSampleAreaData::GetInstance()->UpdateTestBuffer();

	// ɾ��ȷ���б�������
	m_ctrlNewTestList.DeleteAllItemsAndDatas();

	// ����ʼ����ť��Ϊ��Ч
	m_StartButton.EnableWindow(FALSE);

	// ����WM_NOTIFY��Ϣ���л��������Լ�ء�����
	CTabCtrl * pParentTabCtrl = static_cast<CTabCtrl*>(GetParent());
	pParentTabCtrl->SetCurSel(ViewMonitor);
	NMHDR nmhdr ;
	nmhdr.code = TCN_SELCHANGE;
	nmhdr.hwndFrom = pParentTabCtrl->GetSafeHwnd();
	nmhdr.idFrom = IDC_TestMan_Tab;
	pParentTabCtrl->GetParent()->SendMessage(WM_NOTIFY,(WPARAM)(pParentTabCtrl->GetSafeHwnd()),(LPARAM)&nmhdr);
	wait.Restore();
}