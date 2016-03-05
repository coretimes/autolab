// SummaryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "SummaryDlg.h"
#include "SampleAreaData.h"

// CSummaryDlg dialog

IMPLEMENT_DYNAMIC(CSummaryDlg, CBaseDialog)

CSummaryDlg::CSummaryDlg(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CSummaryDlg::IDD, pParent),
	  m_ctrlSampleArea(saForShowing),
	  m_ctrlReagentArea(raForShowing)
{
}

CSummaryDlg::~CSummaryDlg()
{
}

void CSummaryDlg::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_Ctrl_SampleArea,m_ctrlSampleArea);
	DDX_Control(pDX,IDC_Ctrl_ReagentArea,m_ctrlReagentArea);
	DDX_Control(pDX,IDC_Ctrl_IncuArea,m_ctrlIncuArea);
}

BEGIN_MESSAGE_MAP(CSummaryDlg, CBaseDialog)
	ON_WM_PAINT()
	ON_MESSAGE(UM_SampleArea_StateChange, &CSummaryDlg::OnSampleAreaDataUpdate)
END_MESSAGE_MAP()


// CSummaryDlg message handlers

BOOL CSummaryDlg::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	CSampleAreaData *pSampelAreaData = CSampleAreaData::GetInstance();
	pSampelAreaData->SetColumeState(1,TRUE);
	//pSampelAreaData->SetColumeState(2,TRUE);
	//pSampelAreaData->SetColumeState(3,TRUE);
	pSampelAreaData->SetColumeState(4,TRUE);
	pSampelAreaData->SetColumeState(5,TRUE);
	//pSampelAreaData->SetColumeState(6,TRUE);
	pSampelAreaData->SetColumeState(7,TRUE);
	pSampelAreaData->SetColumeState(8,TRUE);
	pSampelAreaData->SetColumeState(9,TRUE);
	//pSampelAreaData->SetColumeState(10,TRUE);
	pSampelAreaData->SetColumeState(11,TRUE);
	pSampelAreaData->SetColumeState(12,TRUE);
	pSampelAreaData->AttchDialog(this);
	CRect rect;

	// ���á��������ؼ����ġ����������ԡ�
	ControlsResizeProp itemSampleArea;
	m_ctrlSampleArea.GetWindowRect(rect);
	ScreenToClient(rect);
	itemSampleArea.ID = IDC_Ctrl_SampleArea;
	itemSampleArea.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������϶���
	itemSampleArea.ctmXofPosBps = tmProportion;				// ����x���갴�����Ŵ�
	itemSampleArea.ctmYofPosBps = tmProportion;				// ����y�ᰴ�����Ŵ�
	itemSampleArea.ctmWigth = tmProportion;					// �ؼ���Ȱ���������
	itemSampleArea.ctmHight = tmProportion;					// �ؼ����Ȱ���������
	SetResize(itemSampleArea);

	// ���á��������ؼ����ġ����������ԡ�
	ControlsResizeProp itemIncuArea;
	m_ctrlIncuArea.GetWindowRect(rect);
	ScreenToClient(rect);
	itemIncuArea.ID = IDC_Ctrl_IncuArea;
	itemIncuArea.ptOrigPosBps = rect.TopLeft();				// �ض�λ���������϶���
	itemIncuArea.ctmXofPosBps = tmProportion;				// ����x���갴�����Ŵ�
	itemIncuArea.ctmYofPosBps = tmProportion;				// ����y�ᰴ�����Ŵ�
	itemIncuArea.ctmWigth = tmProportion;					// �ؼ���Ȱ���������
	itemIncuArea.ctmHight = tmProportion;					// �ؼ����Ȱ���������
	SetResize(itemIncuArea);

	// ���á��Լ����ؼ����ġ����������ԡ�
	ControlsResizeProp itemReagentArea;
	m_ctrlReagentArea.GetWindowRect(rect);
	ScreenToClient(rect);
	itemReagentArea.ID = IDC_Ctrl_ReagentArea;
	itemReagentArea.ptOrigPosBps = rect.TopLeft();			// �ض�λ���������϶���
	itemReagentArea.ctmXofPosBps = tmProportion;			// ����x���갴�����Ŵ�
	itemReagentArea.ctmYofPosBps = tmProportion;			// ����y�ᰴ�����Ŵ�
	itemReagentArea.ctmWigth = tmProportion;				// �ؼ���Ȱ���������
	itemReagentArea.ctmHight = tmProportion;				// �ؼ����Ȱ���������
	SetResize(itemReagentArea);
	return TRUE;
}

void CSummaryDlg::OnPaint()
{
	CPaintDC dc(this);	
	CBrush brWhiteBrush(RGB(255,255,255));
	dc.FillRgn(&m_rgnBackGround,&brWhiteBrush);
}

LRESULT CSummaryDlg::OnSampleAreaDataUpdate(WPARAM wParam, LPARAM lParam)
{
	Sample_Pos pos = static_cast<Sample_Pos>(wParam);
	m_ctrlSampleArea.InvalidNode(pos);
	return 0;
}
