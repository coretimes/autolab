// MaintainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoDetector.h"
#include "MaintainDlg.h"


// CMaintainDlg 对话框

IMPLEMENT_DYNAMIC(CMaintainDlg, CDialog)

CMaintainDlg::CMaintainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMaintainDlg::IDD, pParent)
{

}

CMaintainDlg::~CMaintainDlg()
{
}

void CMaintainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMaintainDlg, CDialog)
END_MESSAGE_MAP()


// CMaintainDlg 消息处理程序
