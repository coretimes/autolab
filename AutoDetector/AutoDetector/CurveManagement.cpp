#include "StdAfx.h"
#include "CurveManagement.h"
#include "CurveTable.h"

CCurveManagement * CCurveManagement::m_pInstance = NULL;
CCurveManagement * CCurveManagement::GetInstance(void)
{
	if(m_pInstance == NULL)
		m_pInstance = new CCurveManagement();
	return m_pInstance;
}
CCurveManagement::CCurveManagement(void)
{
}

CCurveManagement::~CCurveManagement(void)
{
	map<UINT, CCaliCurve*>::iterator iter = m_CurveList.begin();
	while(iter != m_CurveList.end())
	{
		delete iter->second;
		iter++;
	}
}

CCaliCurve* CCurveManagement::GetCurve(uint32 CurveID)
{
	map<UINT, CCaliCurve*>::iterator iter = m_CurveList.find(CurveID);

	// �������list�л�û�д����ߣ����ѯ���ݿ�����ݼ�¼���ݴ�������
	if(iter == m_CurveList.end())
	{
		CCurveTable * pCurveTable = CCurveTable::GetInstance();
		CurveRecord curveinfo = pCurveTable->GetRecordInfo(CurveID);
		if(curveinfo.ID)
		{
			CCaliCurve * pNewCurve = new CCaliCurve(curveinfo);	
			m_CurveList.insert(map<UINT, CCaliCurve*>::value_type(CurveID,pNewCurve));
			return pNewCurve;
		}
		else
			return NULL;
	}
	// �������list���Ѿ��д����ߣ��򷵻ش�����ָ��
	else
		return iter->second;
}
