// AnalysisDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "AnalysisDlg.h"


// CAnalysisDlg 对话框

IMPLEMENT_DYNAMIC(CAnalysisDlg, CDialog)

CAnalysisDlg::CAnalysisDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnalysisDlg::IDD, pParent)
{

}

CAnalysisDlg::~CAnalysisDlg()
{
}

void CAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAnalysisDlg, CDialog)
END_MESSAGE_MAP()


// CAnalysisDlg 消息处理程序
