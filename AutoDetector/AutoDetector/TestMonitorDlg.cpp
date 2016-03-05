// TestMonitorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "TestMonitorDlg.h"
#include "MorniterListItem.h"
#include "TestManagement.h"

// CTestMonitorDlg �Ի���

IMPLEMENT_DYNAMIC(CTestMonitorDlg, CBaseDialog)

CTestMonitorDlg::CTestMonitorDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CTestMonitorDlg::IDD, pParent)
{

}

CTestMonitorDlg::~CTestMonitorDlg()
{
}

void CTestMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_List_Mornitor,m_ctrlMornitorList);
}


BEGIN_MESSAGE_MAP(CTestMonitorDlg, CBaseDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(UM_TestBuffer_Update, &CTestMonitorDlg::OnTestBufferUpdate)
END_MESSAGE_MAP()

void CTestMonitorDlg::OnPaint()
{
	CPaintDC dc(this);
	CBrush brWhiteBackGround(RGB(255,255,255));
	dc.FillRgn(&m_rgnBackGround,&brWhiteBackGround);
}

BOOL CTestMonitorDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CTestMonitorDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	// ���á�����б��ġ����������ԡ�
	ControlsResizeProp itemList;
	CRect rect;
	m_ctrlMornitorList.GetWindowRect(rect);
	ScreenToClient(rect);
	itemList.ID = IDC_List_Mornitor;
	itemList.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������¶���
	itemList.ctmXofPosBps = tmProportion;			// ����x���갴�����Ŵ�
	itemList.ctmYofPosBps = tmProportion;			// ����y�ᰴ�����Ŵ�
	itemList.ctmWigth = tmProportion;				// �ؼ���Ȱ���������
	itemList.ctmHight = tmProportion;				// �ؼ����Ȱ���������
	SetResize(itemList);

	// ����б��б��ʼ��
	m_ctrlMornitorList.InsertColumn(colType,_T("��Ʒ����"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colProject,_T("�����Ŀ"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colSampleNo,_T("��Ʒ���"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colConcPoint,_T("Ũ�ȵ�"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colSamplePos,_T("����λ"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colMeteringIndex,_T("���ڵ�"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colFinishTime,_T("���ʱ��"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colMeteringState,_T("���״̬"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colUseState,_T("ʹ��״̬"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colRlu,_T("����ֵ"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colAverageRlu,_T("ƽ������ֵ"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colCV,_T("����ϵ��"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colTheoryConc,_T("����Ũ��"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colCalConc,_T("����Ũ��"),LVCFMT_CENTER);
	m_ctrlMornitorList.InsertColumn(colCurve,_T("��׼����"),LVCFMT_CENTER);

	ColumnProperty column;

	// "��Ʒ����"�п�����Ϊ������
	column.ColumnIndex = colType;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);	

	// "�����Ŀ"�п�Ϊ�ؼ��ܿ��0.1����С���100
	column.ColumnIndex = colProject;
	column.Width = 40;
	column.WidthRatio = 0.1;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "��Ʒ���"�п�Ϊ�ؼ��ܿ��0.1����С���40
	column.ColumnIndex = colSampleNo;
	column.Width = 40;
	column.WidthRatio = 0.1;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "Ũ�ȵ�"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = colConcPoint;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "����λ"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = colSamplePos;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "���ڵ�"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = colMeteringIndex;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "���ʱ��"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = colFinishTime;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "���״̬"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = colMeteringState;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "ʹ��״̬"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = colUseState;
	column.Width = 40;
	column.WidthRatio = 0.08;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "���״̬"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = colMeteringState;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "����ֵ"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = colRlu;
	column.Width = 40;
	column.WidthRatio = 0.08;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "ƽ������ֵ"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = colAverageRlu;
	column.Width = 80;
	column.WidthRatio = 0.08;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "����ϵ��"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = colCV;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "����Ũ��"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = colTheoryConc;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "����Ũ��"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = colCalConc;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	// "��׼����"�п�����Ϊ�Զ���ͷ���
	column.ColumnIndex = colCurve;
	column.Width = 0;
	m_ctrlMornitorList.SetColumnWidthProp(column);

	m_ctrlMornitorList.SetItemHeight(25);
	m_ctrlMornitorList.EnableSelect(TRUE);
	m_ctrlMornitorList.SetFocus();

	// ȡ�ò��Թ�����ָ��
	CTestManagement * pTestMangement = CTestManagement::GetInstance();
	pTestMangement->AttchDialog(this);

	return TRUE;
}

LRESULT CTestMonitorDlg::OnTestBufferUpdate(WPARAM wParam, LPARAM lParam)
{
	// ɳ©�����״
	CWaitCursor wait;

	// �б�ؼ���ʱ��ֹ����
	m_ctrlMornitorList.SetRedraw(FALSE);

	TestUnit_ID unitID = static_cast<TestUnit_ID>(wParam);
	BOOL IsNewData = static_cast<BOOL>(lParam);

	// ɾ��ָ�����Ե�Ԫ��ԭ��������
	int num = (int)(m_SearchMap.count(unitID));
	multimap<TestUnit_ID,int>::iterator iter = m_SearchMap.find(unitID);
	vector<int> vctIndex;
	for (int i=0; i<num; i++)
	{
		// ����ɾ���У������б������ᷢ���仯,�����ȱ��������������У����淽��������飬ɾ��ָ����������
		vctIndex.push_back(iter->second);
		iter++;
	}
	m_SearchMap.erase(unitID);

	for(int i = num - 1; i >= 0; i--)
	{
		m_ctrlMornitorList.DeleteItem(vctIndex[i]);
	}


	//multimap<TestUnit_ID,int>::iterator iterBegin = m_SearchMap.lower_bound(unitID);
	//multimap<TestUnit_ID,int>::iterator iterEnd = m_SearchMap.upper_bound(unitID);
	//if (iterBegin != m_SearchMap.end())
	//{
	//	// ����ɾ���У������б������ᷢ���仯,�����ȱ��������������У����淽��������飬ɾ��ָ����������
	//	vector<int> vctIndex;
	//	vctIndex.push_back(iterBegin->second);
	//	while(iterBegin != iterEnd)
	//	{
	//		m_SearchMap.erase(iterBegin);
	//		iterBegin++;
	//	}

	//	for(int i= (int)(vctIndex.size())-1; i>=0; i++)
	//	{
	//		m_ctrlMornitorList.DeleteItem(vctIndex[i]);
	//	}
	//}

	// ȡ�ò��Թ�����ָ��
	CTestManagement * pTestMangement = CTestManagement::GetInstance();

	// ȡ�ò��Թ�����������
	const map<TestUnit_ID, CTestUnit *> & TestBuffer = pTestMangement->GetTestBuffer();

	// ���ݲ��Ե�ԪID���Ҳ��Ե�Ԫ������Ҳ�������˵����ɾ�������ݣ�ֱ�ӷ���
	map<TestUnit_ID, CTestUnit *>::const_iterator iterUnit = TestBuffer.find(unitID);
	if(iterUnit == TestBuffer.end())
	{
		return 0;
	}
	CTestUnit * pTestUnit = iterUnit->second;

	// ȡ�õ�Ԫ�������ڵ�����
	const map<ConcPoint_Index,SampleTestNode> & SampleNodes = pTestUnit->GetSampleTestNodes();

	// �������Ե�Ԫ��ÿ�������ڵ�
	map<ConcPoint_Index,SampleTestNode>::const_iterator iterSampleNode = SampleNodes.begin();
	while(iterSampleNode != SampleNodes.end())
	{
		const SampleTestNode & sampnode = iterSampleNode->second;

		// ���������ڵ���ÿ�����ڵ�ڵ�
		vector<Metering_ID>::const_iterator iterMeteringNode = sampnode.arrMeterNodeID.begin();
		int indexListOfItem = m_ctrlMornitorList.GetItemCount();
		while(iterMeteringNode != sampnode.arrMeterNodeID.end())
		{
			// ���б��в�����������
			CMorniterListItem * pNewItem = new CMorniterListItem(*iterMeteringNode);
			m_ctrlMornitorList.AddItem(indexListOfItem,pNewItem);
			// ���ѯ���У���������
			m_SearchMap.insert(pair<TestUnit_ID,int>(unitID,indexListOfItem));		
			indexListOfItem++;
			iterMeteringNode++;
		}
		iterSampleNode++;
	}

	m_ctrlMornitorList.SetRedraw(TRUE);
	wait.Restore();
	return 0;
}

void CTestMonitorDlg::RefreshSearchMap()
{
	m_SearchMap.clear();
	for(int i=0; i<m_ctrlMornitorList.GetItemCount(); i++)
	{
		CMorniterListItem * pNewItem = reinterpret_cast<CMorniterListItem*>(m_ctrlMornitorList.GetItemData(i));
		m_SearchMap.insert(pair<TestUnit_ID,int>(pNewItem->GetTestUnit()->GetID(),i));		
	}
}