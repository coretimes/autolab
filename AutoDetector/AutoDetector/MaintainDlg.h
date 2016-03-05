#pragma once


// CMaintainDlg 对话框

class CMaintainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMaintainDlg)

public:
	CMaintainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMaintainDlg();

// 对话框数据
	enum { IDD = IDD_Maintain_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
