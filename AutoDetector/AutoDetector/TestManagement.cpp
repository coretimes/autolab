#include "StdAfx.h"
#include "TestManagement.h"

CTestManagement * CTestManagement::m_pInstance = NULL;
CTestManagement::CTestManagement(void)
{
	m_MaxUnitID = 0;
}

CTestManagement::~CTestManagement(void)
{
	map<TestUnit_ID, CTestUnit *>::iterator iter = m_TestBuffer.begin();
	while(iter != m_TestBuffer.end())
	{
		delete iter->second;
		iter++;
	}
}

TestUnit_ID CTestManagement::AddNewNode(const SampleTestNode & node)
{
	TestUnit_ID UintID = 0;

	// ���ݲ��ұ��жϴ˽ڵ��Ƿ��Ѿ������Ѿ����ڵĲ��Ե�Ԫ
	multimap<Sample_Reagent_Key,TestUnit_ID>::iterator iter = m_Search_SampleReagentKey.find(Sample_Reagent_Key(node.SampleID,node.ReagentID));
	// �����û�д˲��Ե�Ԫ
	if(iter == m_Search_SampleReagentKey.end())
	{
		// �½����Ե�Ԫ
		UintID = _AddNewUnit(node);
	}
	else
	{
		UintID = iter->second;
		map<TestUnit_ID, CTestUnit *>::iterator iterUnit = m_TestBuffer.find(UintID);
		ASSERT(iterUnit != m_TestBuffer.end());
		CTestUnit * poldTest = iterUnit->second;
		poldTest->AddSampleNode(node);
	}

	SpreadUpdateMessage(UintID);
	return UintID;
}

TestUnit_ID CTestManagement::_AddNewUnit(const SampleTestNode & node)
{
	// ȡ��� ID + 1 Ϊ��ID
	m_MaxUnitID++;

	// ���ݱ�׼Ʒ�ͷǱ�׼Ʒ������ͬ���Ե�Ԫ
	CTestUnit * pNewTest = NULL;
	if(node.IsStandard == true)
		pNewTest = new CStandardTest(m_MaxUnitID);
	else
		pNewTest = new CSampleTest(m_MaxUnitID);

	// ���²��Ե�Ԫ�м����һ���������Խڵ�
	pNewTest->AddSampleNode(node);

	// ָ�뱣����ӳ�����
	m_TestBuffer.insert(pair<TestUnit_ID,CTestUnit*>(m_MaxUnitID,pNewTest));

	// �ڸ��ֲ��ұ��й����˵�Ԫ
	m_SearchReagentKey.insert(pair<Reagent_Key,TestUnit_ID>(node.ReagentID,m_MaxUnitID));
	m_SearchSampleKey.insert(pair<Sample_Key,TestUnit_ID>(node.SampleID,m_MaxUnitID));
	m_Search_SampleReagentKey.insert(pair<Sample_Reagent_Key,TestUnit_ID>(make_pair(node.SampleID,node.ReagentID),m_MaxUnitID));

	return m_MaxUnitID;
}

void CTestManagement::SpreadUpdateMessage(TestUnit_ID unitID)
{
	for(int i=0; i < (int)(m_AssociateDlgList.size()); i++)
	{
		m_AssociateDlgList[i]->SendMessage(UM_TestBuffer_Update,unitID,NULL);
	}
};
