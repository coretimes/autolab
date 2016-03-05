// SampleAreaManDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "SampleAreaManDlg.h"
#include "SampleAreaData.h"
#include "SampleAreaListItem.h"

// CSampleAreaManDlg �Ի���

IMPLEMENT_DYNAMIC(CSampleAreaManDlg, CBaseDialog)

CSampleAreaManDlg::CSampleAreaManDlg(CWnd* pParent /*=NULL*/)
: CBaseDialog(CSampleAreaManDlg::IDD, pParent), 
  m_ctrlSampleArea(saForConfig)
{

}

CSampleAreaManDlg::~CSampleAreaManDlg()
{
}

void CSampleAreaManDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_Ctrl_SampleArea,m_ctrlSampleArea);
	DDX_Control(pDX,IDC_LIST_SampleArea,m_ctrlSampleList);
}


BEGIN_MESSAGE_MAP(CSampleAreaManDlg, CBaseDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(UM_SampleArea_StateChange, &CSampleAreaManDlg::OnSampleAreaStateChange)
	ON_MESSAGE(UM_SampleArea_DataUpdate, &CSampleAreaManDlg::OnSampleAreaDataUpdate)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSampleAreaManDlg ��Ϣ�������

BOOL CSampleAreaManDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	CRect rect;
	ControlsResizeProp item;
	// ���á��������ؼ����ġ����������ԡ�
	m_ctrlSampleArea.GetClientRect(rect);
	item.ID = IDC_Ctrl_SampleArea;
	item.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������϶���
	item.ctmXofPosBps = tmProportion;			// ����x���갴�����Ŵ�
	item.ctmYofPosBps = tmProportion;			// ����y�ᰴ�����Ŵ�
	item.ctmWigth = tmProportion;				// �ؼ���Ȱ���������
	item.ctmHight = tmProportion;				// �ؼ����Ȱ���������
	SetResize(item);

	// ���á��Լ����б��ġ����������ԡ�
	ControlsResizeProp itemList;
	m_ctrlSampleList.GetWindowRect(rect);
	ScreenToClient(rect);
	itemList.ID = IDC_LIST_SampleArea;
	itemList.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������¶���
	itemList.ctmXofPosBps = tmProportion;			// ����x���갴�����Ŵ�
	itemList.ctmYofPosBps = tmProportion;			// ����y�ᰴ�����Ŵ�
	itemList.ctmWigth = tmProportion;				// �ؼ���Ȱ���������
	itemList.ctmHight = tmProportion;				// �ؼ����Ȱ���������
	SetResize(itemList);

	// ���������б��ʼ��
	m_ctrlSampleList.InsertColumn(0,_T("����"),LVCFMT_CENTER);
	m_ctrlSampleList.InsertColumn(1,_T("��������"),LVCFMT_CENTER);
	m_ctrlSampleList.InsertColumn(2,_T("�������"),LVCFMT_CENTER);
	m_ctrlSampleList.InsertColumn(3,_T("�����Ŀ"),LVCFMT_CENTER);
	m_ctrlSampleList.InsertColumn(4,_T("�ظ�����"),LVCFMT_CENTER);
	m_ctrlSampleList.InsertColumn(5,_T("����״̬"),LVCFMT_CENTER);

	ColumnProperty column;

	// "����"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = 0;
	column.Width = 0;
	column.WidthRatio = 0;
	m_ctrlSampleList.SetColumnWidthProp(column);	

	// "��������"�п�Ϊ��ͷ���
	column.ColumnIndex = 1;
	column.Width = 0;
	m_ctrlSampleList.SetColumnWidthProp(column);

	// "�������"�п�Ϊ�ؼ��ܿ��0.25����С���100
	column.ColumnIndex = 2;
	column.Width = 100;
	column.WidthRatio = 0.2;
	m_ctrlSampleList.SetColumnWidthProp(column);

	// "�����Ŀ"�п�Ϊ�ؼ��ܿ��0.45����С���140
	column.ColumnIndex = 3;
	column.Width = 140;
	column.WidthRatio = 0.3;
	m_ctrlSampleList.SetColumnWidthProp(column);

	// "�ظ�����"�п�Ϊ��ͷ���
	column.ColumnIndex = 4;
	column.Width = 0;
	m_ctrlSampleList.SetColumnWidthProp(column);

	// "����״̬"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = 5;
	column.Width = 0;
	m_ctrlSampleList.SetColumnWidthProp(column);

	m_ctrlSampleList.SetItemHeight(25);
	m_ctrlSampleList.EnableSelect(FALSE);
	m_ctrlSampleList.SetFocus();

	CSampleAreaData::GetInstance()->AttchDialog(this);

	return TRUE;
}

void CSampleAreaManDlg::OnPaint()
{
	CPaintDC dc(this);
	CBrush brWhiteBackGround(RGB(255,255,255));
	dc.FillRgn(&m_rgnBackGround,&brWhiteBackGround);
}

BOOL CSampleAreaManDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

LRESULT CSampleAreaManDlg::OnSampleAreaStateChange(WPARAM wParam, LPARAM lParam)
{
	// ������ĳλ�����ݷ����仯��Ӧ����

	// �ҵ���λ�ã���ʹ��λ�������ػ�
	m_UpdatePos = static_cast<Sample_Pos>(wParam);
	m_ctrlSampleArea.InvalidNode(m_UpdatePos);
	KillTimer(TID_ListScroll_Delay);
	SetTimer(TID_ListScroll_Delay,300,NULL);
	return 0;
}

LRESULT CSampleAreaManDlg::OnSampleAreaDataUpdate(WPARAM wParam, LPARAM lParam)
{
	CSampleAreaData *pData = CSampleAreaData::GetInstance();
	m_ctrlSampleList.SetRedraw(FALSE);
	m_ctrlSampleList.DeleteAllItemsAndDatas();
	for(int pos = 1; pos<= NUM_SAMPLEPOS; pos++)
	{
		// ȡ���������ýڵ�
		SampleConfigNode *pNode = pData->GetNode(pos);
		// ���û���Թܣ������б��в�������
		if(pNode->IsThereTube == false)
			continue;

		// ���û����������������û�в�����Ŀ��
		if(pNode->IsSetingSample == false || pNode->TestProjArr.size() < 1)
		{
			CSampleAreaListItem * pNewItem = new CSampleAreaListItem(pNode->PosNum,0);
			m_ctrlSampleList.AddItem(0,pNewItem);
		}
		// ��������в�����Ŀ��
		else
		{
			// ѭ������������Ŀ����
			map<Reagent_Pos,SampleConfigNode::TestProjInfo>::iterator iter = pNode->TestProjArr.begin();
			while(iter != pNode->TestProjArr.end())
			{
				CSampleAreaListItem * pNewItem = new CSampleAreaListItem(pNode->PosNum,iter->first);
				m_ctrlSampleList.AddItem(0,pNewItem);
				iter++;
			}
		}
	}
	m_ctrlSampleList.SetScrollRange(SB_VERT,0,m_ctrlSampleList.GetItemCount(),false);
	m_ctrlSampleList.Reorder();
	m_ctrlSampleList.SetRedraw(TRUE);
	return 0;
}

void CSampleAreaManDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TID_ListScroll_Delay:
		KillTimer(TID_ListScroll_Delay);
		// �ҵ���λ���������б��е������������͡���������λ�á���Ϣ
		for(int index = 0; index < m_ctrlSampleList.GetItemCount(); index++)
		{
			CSampleAreaListItem * pNewItem = reinterpret_cast<CSampleAreaListItem *>(m_ctrlSampleList.GetItemData(index));
			if(pNewItem->GetSamplePosNum() == m_UpdatePos)
			{
				m_ctrlSampleList.SendMessage(WM_VSCROLL,MAKEWPARAM(SB_THUMBPOSITION,index),NULL);
				break;
			}
		}
		m_ctrlSampleList.RedrawAllItems();
		break;
	default:
		ASSERT(0);
	}
}