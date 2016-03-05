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

	// 根据查找表，判断此节点是否已经属于已经存在的测试单元
	multimap<Sample_Reagent_Key,TestUnit_ID>::iterator iter = m_Search_SampleReagentKey.find(Sample_Reagent_Key(node.SampleID,node.ReagentID));
	// 如果还没有此测试单元
	if(iter == m_Search_SampleReagentKey.end())
	{
		// 新建测试单元
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
	// 取最大 ID + 1 为新ID
	m_MaxUnitID++;

	// 根据标准品和非标准品创建不同测试单元
	CTestUnit * pNewTest = NULL;
	if(node.IsStandard == true)
		pNewTest = new CStandardTest(m_MaxUnitID);
	else
		pNewTest = new CSampleTest(m_MaxUnitID);

	// 在新测试单元中加入第一个样本测试节点
	pNewTest->AddSampleNode(node);

	// 指针保存在映射表中
	m_TestBuffer.insert(pair<TestUnit_ID,CTestUnit*>(m_MaxUnitID,pNewTest));

	// 在各种查找表中关联此单元
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
