#pragma once


// CMaintainDlg �Ի���

class CMaintainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMaintainDlg)

public:
	CMaintainDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMaintainDlg();

// �Ի�������
	enum { IDD = IDD_Maintain_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
