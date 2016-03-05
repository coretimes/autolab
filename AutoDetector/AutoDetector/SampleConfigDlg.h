//*****************************************************************************
// �ļ��� : SampleConfigDlg.h
// 
// ����   : �����������öԻ�����
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-11       ��־��       �½�
// ------------------------------------------
// ��ע��
// ------------------------------------------
// 
//*****************************************************************************
#include "CustomListCtrl.h"
#include "SampleConfigListItem.h"
#include "SampleAreaCtrl.h"
#include "ReagentAreaCtrl.h"
#include "Types.h"

#pragma once
class CSampleConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CSampleConfigDlg)

public:
	// ---------------------------------------------------------
	// �������ܣ�
	//     ���캯��
	// ������
	//     1.IsAddSamples - �Ƿ�������������Ա�־��
	//                      ture���������,false���޸���������
	//     2.SampleConfigNodes - ����������ڵ�ָ������
	//     3.ReagentConfigNode - ������Լ��ڵ�ָ������
	// ---------------------------------------------------------
	CSampleConfigDlg(const bool &IsAddSamples,
					 map<Sample_Pos,SampleConfigNode *> &SampleConfigNodes,
					 map<Reagent_Pos,const ReagentConfigNode *> &RegentConfigNodes,
					 CWnd* pParent = NULL);

	virtual ~CSampleConfigDlg();

	enum { IDD = IDD_SampleConfig_Dlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	// ---------------------------------------------------------
	// �������ܣ�
	//     ��ȷ�ϡ���ť������Ӧ
	// ---------------------------------------------------------
	afx_msg void OnBnClickedOk();

	// ---------------------------------------------------------
	// �������ܣ�
	//     ��ȡ������ť������Ӧ
	// ---------------------------------------------------------
	afx_msg void OnBnClickedCancle();

	// ---------------------------------------------------------
	// �������ܣ�
	//     �����ǰ׺���༭�����ݷ����仯��Ӧ
	// ---------------------------------------------------------
	afx_msg void OnEnChangeValueNumprefix();

	// ---------------------------------------------------------
	// �������ܣ�
	//     ����š���Ͽ�ѡ����仯��Ӧ
	// ---------------------------------------------------------
	afx_msg void OnCbnSelchangeComboxSerials();

	// ---------------------------------------------------------
	// �������ܣ�
	//     �����͡���Ͽ�ѡ������ݷ����仯��Ӧ
	// ---------------------------------------------------------
	afx_msg void OnCbnSelchangeComboxType();
	afx_msg void OnCbnEditchangeComboxType();

	// ---------------------------------------------------------
	// �������ܣ�
	//     ��ϡ�ͱ������༭�����ݷ����仯��Ӧ
	// ---------------------------------------------------------
	afx_msg void OnEnChangeValueDilution();

	// ---------------------------------------------------------
	// �������ܣ�
	//     ��ϡ�ͱ�����΢����ť������Ӧ����
	// ---------------------------------------------------------
	afx_msg void OnDeltaposSpinDilution(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// �������ܣ�
	//     ���ͼ쵥λ���༭�����ݷ����仯��Ӧ
	// ---------------------------------------------------------
	afx_msg void OnEnChangeValueDept();

	// ---------------------------------------------------------
	// �������ܣ�
	//     ���ͼ��ˡ��༭�����ݷ����仯��Ӧ
	// ---------------------------------------------------------
	afx_msg void OnEnChangeValueApplicant();

	// ---------------------------------------------------------
	// �������ܣ�
	//     ����Ŀ�б�������Ӧ����
	// ---------------------------------------------------------
	afx_msg void OnLbnSelchangeListboxProject();

	// ---------------------------------------------------------
	// �������ܣ�
	//     ����Ŀ�б�check�仯��Ӧ����
	// ---------------------------------------------------------
	afx_msg void OnProjectChkChange();

	// ---------------------------------------------------------
	// �������ܣ�
	//     ���ͼ����ڡ��仯��Ӧ����
	// ---------------------------------------------------------
	afx_msg void OnDtnDatetimechangeDateApplydate(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// �������ܣ�
	//     ����������Ũ�ȡ���ѡ����Ӧ����
	// ---------------------------------------------------------
	afx_msg void OnBnClickedCheckboxSettheoryconc();

	// ---------------------------------------------------------
	// �������ܣ�
	//     ���ظ��������༭����Ӧ����
	// ---------------------------------------------------------
	afx_msg void OnEnChangeEditRepeation();

	// ---------------------------------------------------------
	// �������ܣ�
	//     ���ظ�������΢����ť��Ӧ����
	// ---------------------------------------------------------
	afx_msg void OnDeltaposSpinRepeation(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// �������ܣ�
	//     ����ʼŨ�ȡ�΢����ť��Ӧ����
	// ---------------------------------------------------------
	afx_msg void OnDeltaposSpinStartconc(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// �������ܣ�
	//     ��Ũ��������΢����ť��Ӧ����
	// ---------------------------------------------------------
	afx_msg void OnDeltaposSpinIncreament(NMHDR *pNMHDR, LRESULT *pResult);

	// ---------------------------------------------------------
	// �������ܣ�
	//     ����ʼŨ�ȡ��༭����Ӧ����
	// ---------------------------------------------------------
	afx_msg void OnEnChangeEditStartconc();

	// ---------------------------------------------------------
	// �������ܣ�
	//     ����Ũ���������༭����Ӧ����
	// ---------------------------------------------------------
	afx_msg void OnEnChangeEditIncreament();

private:
	// ---------------------------------------------------------
	// �������ܣ�
	//     ��ʼ��������ź���
	// ---------------------------------------------------------
	void _InitSampleNo();

	// ---------------------------------------------------------
	// �������ܣ�
	//     �������Ŀ
	// ---------------------------------------------------------
	void _AddNewProject(const int &newProjecIndex);

	// ---------------------------------------------------------
	// �������ܣ�
	//     ɾ����Ŀ
	// ---------------------------------------------------------
	void _DeleteProject(const int &newProjecIndex);

	// ---------------------------------------------------------
	// �������ܣ�
	//     ˢ�²�ѯ�б�
	// ---------------------------------------------------------
	void _RefreshSearchList();

	// ---------------------------------------------------------
	// �������ܣ�
	//     �Զ�����ɾ������Ũ��
	// ������
	//     IsAdd-�����true�����Զ���䣻�����false��ɾ������Ũ��
	// ---------------------------------------------------------
	void _AutoFillThoeryConc(bool IsAdd = true);

	// ---------------------------------------------------------
	// �������ܣ�
	//     ȡ��ָ����������Ŀ��Ũ�ȵ�λ
	// ---------------------------------------------------------
	CString _GetUnitConc(int idxProject);

	// ---------------------------------------------------------
	// �������ܣ�
	//     �������б�ListItem�������������ýڵ�ConfigNode
	// ---------------------------------------------------------
	void _ListToConfig(const CSampleConfigListItem *pItem,SampleConfigNode *pNode);

	// ---------------------------------------------------------
	// �������ܣ�
	//     ���������������ýڵ�ConfigNode�����б�ListItem
	// ---------------------------------------------------------
	void _ConfigToList(const SampleConfigNode *pNode,vector <CSampleConfigListItem*> & ItemList);
private:
	CCustomListCtrl m_ctrlConfigList;								// ���������á��б�ؼ�
	CEdit m_ctrlEditNumPrefix;										// �����ǰ׺���༭��
	CComboBox m_ctrlCombSerials;									// ����š���Ͽ�
	CComboBox m_ctrlCombTypes;										// ���������͡���Ͽ�
	CEdit m_ctrlEditDilution;										// ��ϡ�Ͷȡ��༭��
	CSpinButtonCtrl m_ctrlSpinBtnDilution;							// ��ϡ�Ͷȡ�΢����ť
	CEdit m_ctrlEditDept;											// ���ͼ쵥λ���༭��
	CEdit m_ctrlEditApplicant;										// ���ͼ��ˡ��༭��
	CDateTimeCtrl m_ctrlDateApply;									// ���ͼ����ڡ�����ѡ��ؼ�
	CButton m_CheckSetDefault;										// ������ΪĬ��ֵ����ѡ��ť
	CCheckListBox m_ctrlListProject;								// �������Ŀ����Ͽ�
	CEdit m_ctrlEditRepeations;										// ���ظ��������༭��
	CSpinButtonCtrl m_ctrlSpinBtnRepeations;						// ���ظ�������΢����ť
	CButton m_CheckSetThoeryConc;									// ����������Ũ�ȡ���ѡ��ť
	CEdit m_ctrlEditStartConc;										// ����ʼŨ�ȡ��༭��
	CSpinButtonCtrl m_ctrlSpinBtnStartConc;							// ����ʼŨ�ȡ�΢����ť
	CEdit m_ctrlEditIncreament;										// ���������༭��
	CSpinButtonCtrl m_ctrlSpinBtnIncreament;						// ��������΢����ť

	map<Reagent_Pos,const ReagentConfigNode *> &m_RegentConfigNodes;// �Լ����Լ����ýڵ���Ϣ��Reagent_Pos���Լ���λ�ñ��ReagentConfigNode*ָ���Լ�������
	map<Sample_Pos,SampleConfigNode *> &m_SampleConfigNodes;		// �������������ýڵ���Ϣ��Sample_Pos��������λ�ñ��
	bool m_IsAddSamples;											// �Ƿ���������������Ա�־

	multimap<Sample_Pos,CSampleConfigListItem*> m_mapListItemData;	// ���������б����������飬uint16��������λ�ñ��
	multimap<int/*idxProjec*/,int/*idxListItem*/> m_mapSearchList;	// ���������б������������ѯ��ǰһ��int����Ŀ��Ͽ���������һ��int�������б�����
	int m_nCountOfConfigNode;										// �������ýڵ�ĸ���
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
