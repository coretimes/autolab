//*****************************************************************************
// �ļ��� : TestManagement.h
// 
// ��Ҫ   : ���Թ����� (ͷ�ļ�)
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-11       ��־��       �½�
// ------------------------------------------
//*****************************************************************************
#pragma once
#include "TestUnit.h"
#include <map>
#include "SampleTest.h"
#include "StandardTest.h"

// ���Ի��������ݸ�����Ϣ����
// wparam: ���Ե�ԪID
// lparam: NULL

#define UM_TestBuffer_Update		WM_USER + 1002

using namespace std;
typedef pair<Sample_Key,Reagent_Key> Sample_Reagent_Key;

class CTestManagement
{
public:
	CTestManagement(void);
	~CTestManagement(void);

public:
	static CTestManagement * GetInstance()											// ȡ��ʵ��ָ��
	{
		if(m_pInstance == NULL)
			m_pInstance = new CTestManagement();
		return m_pInstance;
	}

	// --------------------------------------------------
	// ժҪ��
	//	   ɾ��ʵ��
	// --------------------------------------------------
	static void DestroyInstance()
	{
		if (m_pInstance != NULL)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}
	// ---------------------------------------------------
	// ժҪ��
	//		����һ���µ��������Խڵ㣬�Զ�������Ե�Ԫ
	// ������
	//		node - �µ��������Խڵ�
	// ---------------------------------------------------
	TestUnit_ID AddNewNode(const SampleTestNode & node);

	// ---------------------------------------------------
	// ժҪ��
	//		���ݲ��Ե�ԪID��ȡ�ò��Ե�Ԫָ��
	// ������
	//		ID - ���Ե�ԪID
	// ---------------------------------------------------
	CTestUnit * GetTestUnit(TestUnit_ID ID)
	{
		map<TestUnit_ID, CTestUnit *>::iterator iter = m_TestBuffer.find(ID);
		if(iter != m_TestBuffer.end())
			return iter->second;
		else
			return NULL;
	}

	// ---------------------------------------------------
	// ժҪ��
	//		ȡ�ò��Ի��������ݣ�Ϊ�˱�����
	// ---------------------------------------------------
	const map<TestUnit_ID, CTestUnit *> & GetTestBuffer() const
	{
		return m_TestBuffer;
	}

	// --------------------------------------------------
	// ժҪ��
	//	   ����¹����Ի���
	// --------------------------------------------------
	void AttchDialog(CDialog * dlg)
	{
		m_AssociateDlgList.push_back(dlg);
	};

	// --------------------------------------------------
	// ժҪ��
	//	   ������Ի���ɢ�����ݸ��µ���Ϣ
	// ���أ�
	//     1.unitID.���ݵĲ��Ե�ԪID
	// --------------------------------------------------
	void SpreadUpdateMessage(TestUnit_ID unitID);

private:
	// ---------------------------------------------------
	// ժҪ��
	//		�½�һ�����Ե�Ԫ�����ص�ԪID
	// ������
	//		node - �µ��������Խڵ�
	// ---------------------------------------------------
	TestUnit_ID _AddNewUnit(const SampleTestNode & node);

private:
	// ---------------------------------------------------
	// ��Ա����
	//		���Ե�Ԫ�洢ӳ���
	// ---------------------------------------------------
	map<TestUnit_ID, CTestUnit *> m_TestBuffer;

	// ---------------------------------------------------
	// ��Ա����
	//		���ұ�1,���ҷ�ʽ������ID -> ���Ե�ԪID
	//      һ���������ж�����Ե�Ԫ
	// ---------------------------------------------------
	multimap<Sample_Key,TestUnit_ID> m_SearchSampleKey;

	// ---------------------------------------------------
	// ��Ա����
	//		���ұ�2,���ҷ�ʽ���Լ�ID -> ���Ե�ԪID
	//		һ���Լ�Ҳ���ж�����Ե�Ԫ
	// ---------------------------------------------------
	multimap<Reagent_Key,TestUnit_ID> m_SearchReagentKey;

	// ---------------------------------------------------
	// ��Ա����
	//		���ұ�3,���ҷ�ʽ��<����ID���Լ�ID> -> ���Ե�ԪID
	//		ͬһ��������ͬһ���Լ�Ҳ�ܲ��Զ�Σ��γɶ�����Ե�Ԫ
	// ---------------------------------------------------
	multimap<Sample_Reagent_Key,TestUnit_ID> m_Search_SampleReagentKey;

	// ---------------------------------------------------
	// ��Ա����
	//		��ǰ���Ĳ��Ե�ԪID
	// ---------------------------------------------------
	TestUnit_ID m_MaxUnitID;

private:
	static CTestManagement * m_pInstance;
	vector<CDialog *> m_AssociateDlgList ;							// �����Ի���ָ��
};
