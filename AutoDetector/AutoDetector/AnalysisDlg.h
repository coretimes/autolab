#pragma once


// CAnalysisDlg �Ի���

class CAnalysisDlg : public CDialog
{
	DECLARE_DYNAMIC(CAnalysisDlg)

public:
	CAnalysisDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAnalysisDlg();

// �Ի�������
	enum { IDD = IDD_Analysis_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
