#pragma once


// CAnalysisDlg 对话框

class CAnalysisDlg : public CDialog
{
	DECLARE_DYNAMIC(CAnalysisDlg)

public:
	CAnalysisDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAnalysisDlg();

// 对话框数据
	enum { IDD = IDD_Analysis_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
