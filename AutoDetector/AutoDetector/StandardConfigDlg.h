//*****************************************************************************
// �ļ��� : StandardConfigDlg.h
// 
// ����   : ��׼Ʒ�������öԻ�����
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-10       ��־��       �½�
// ------------------------------------------
// ��ע��
// ------------------------------------------
// 
//*****************************************************************************
#pragma once
#include "CustomListCtrl.h"
#include "SampleAreaCtrl.h"
#include "ReagentAreaCtrl.h"
#include "StandardTable.h"
#include "StandardConfigListItem.h"

using namespace std;

class CStandardConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CStandardConfigDlg)

public:
	// ---------------------------------------------------------
	// �������ܣ�
	//     ����ָ���ġ��������ýڵ㡱���鹹�캯��
	// ������
	//     1.IsAddStanderds - �Ƿ�����±�׼Ʒ���Ա�־��
	//                      ture����±�׼Ʒ,false���޸ı�׼Ʒ����
	//     2.SampleConfigNodes - ����������ڵ�ָ������
	//     3.ReagentConfigNode - ������Լ��ڵ�ָ������
	// ---------------------------------------------------------
	CStandardConfigDlg(	const bool &IsAddStanderds,
						map<Sample_Pos,SampleConfigNode *> &SampleConfigNodes,
						map<Reagent_Pos,const ReagentConfigNode *> &RegentConfigNodes,
						CWnd* pParent = NULL);

	virtual ~CStandardConfigDlg();
	enum { IDD = IDD_StandardConfig_Dlg };
	virtual BOOL OnInitDialog();

private:
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()

	// ---------------------------------------------------------
	// �������ܣ�
	//     ��ȷ�ϡ���ť������Ӧ����
	// ---------------------------------------------------------
	afx_msg void OnBnClickedOk();

	// ---------------------------------------------------------
	// �������ܣ�
	//    �� ȡ������ť������Ӧ����
	// ---------------------------------------------------------
	afx_msg void OnBnClickedCancel();

	// ---------------------------------------------------------
	// �������ܣ�
	//    �������Ŀ����Ͽ����ݱ仯��Ӧ����
	// ---------------------------------------------------------
	afx_msg void OnCbnSelchangeComboxTestproject();

	// ---------------------------------------------------------
	// �������ܣ�
	//    �����š���Ͽ����ݱ仯��Ӧ����
	// ---------------------------------------------------------
	afx_msg void OnCbnSelchangeComboxBatchnum();

	// ---------------------------------------------------------
	// �������ܣ�
	//    ��ת��ťλ�ñ仯��Ϣ��Ӧ����
	// ---------------------------------------------------------
	afx_msg void OnDeltaposSpinRepeation(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// �������ܣ�
	//    �ظ������༭��ֵ�����仯ʱ����Ӧ����
	// ---------------------------------------------------------
	afx_msg void OnEnChangeEditRepeation();

	// ---------------------------------------------------------
	// �������ܣ�
	//    ��ʼ����׼Ʒ��Ϣ�ؼ�
	// ---------------------------------------------------------
	void _InitSampleInfoControls();

	// ---------------------------------------------------------
	// �������ܣ�
	//    ���õ�ǰ��׼Ʒ��Ϣ��ʾ
	// ---------------------------------------------------------
	void _DisplayCurrentStanderd(const StandardRecord &record);

	// ---------------------------------------------------------
	// �������ܣ�
	//     �������б�ListItem�������������ýڵ�ConfigNode
	// ---------------------------------------------------------
	void _ListToConfig(const CStandardConfigListItem *pItem,SampleConfigNode *pNode);

	// ---------------------------------------------------------
	// �������ܣ�
	//     ���������������ýڵ�ConfigNode�����б�ListItem
	// ---------------------------------------------------------
	void _ConfigToList(const SampleConfigNode *pNode,CStandardConfigListItem * pItem);

private:
	CCustomListCtrl m_ctrlConfigList;								// ���������б�ؼ�
	CComboBox m_ComboxProjectName;									// �������Ŀ����Ͽ�ؼ�
	CComboBox m_ComboxBatchNum;										// �����š���Ͽ�ؼ�
	CEdit m_EditRepeation;											// ���ظ��������༭��ؼ�
	CSpinButtonCtrl m_SpinRepeation;								// ���ظ�������΢����ť�ؼ�
	CStatic m_ctrlStdPointTitle[10];								// ����׼�㡱��������
	CStatic m_ctrlStdPointValue[10];								// ����׼�㡱Ũ��ֵ

	map<Reagent_Pos,const ReagentConfigNode *> m_mapRegentConfigNodes;	// �Լ����Լ����ýڵ���Ϣ��uint16���Լ���λ�ñ��
	map<Sample_Pos,SampleConfigNode *> &m_mapSampleConfigNodes;			// �������������ýڵ���Ϣ��uint16��������λ�ñ��
	bool m_IsAddStanderds;												// �Ƿ�����±�׼Ʒ���Ա�־��ture��

	vector<CStandardConfigListItem*> m_vctListItemData;					// ���������б�����������,��̬����
	int m_nCountOfConfigNode;											// �������ýڵ�ĸ���
	int m_nNumOfValidConfigNode;										// ��Ч�������ýڵ�ĸ���������׼��������롰ѡ������ýڵ������֮���С����ֵ

	vector<StandardRecord> m_vctStandardRecords;						// ��ǰ�����Ŀ�����£����б�׼Ʒ��¼��Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
