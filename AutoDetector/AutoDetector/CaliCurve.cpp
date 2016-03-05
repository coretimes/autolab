#include "StdAfx.h"
#include "CaliCurve.h"
#include "StandardTable.h"
#include "GeneralFuction.h"

CCaliCurve::CCaliCurve(void)
{
	m_bIsCurveErr = true;
	m_pSpline = NULL;
	m_pFourParams = NULL;
}

CCaliCurve::~CCaliCurve(void)
{
	if(m_pSpline)
		delete m_pSpline;
	if(m_pFourParams)
		delete m_pFourParams;
}

CCaliCurve::CCaliCurve(const CurveRecord &CurveData)
{
	UpdateCurve(CurveData);
}

void CCaliCurve::UpdateCurve(const CurveRecord &CurveData)
{
	m_CurveRecord = CurveData;
	_ParseCurveRecord(CurveData);
}

void CCaliCurve::SaveData()
{
}

void CCaliCurve::UpdateCurve(const vector<CaliNode> &CaliNodeArr)
{
	m_CaliNodes.clear();
	for(size_t i=0; i < CaliNodeArr.size(); i++)
	{
		m_CaliNodes.push_back(CaliNodeArr[i]);
	}

	// 如果已经生成过曲线，则需要重新生成；如果还未生成过曲线，则不需要在此生成
	if(m_pSpline)
		_CreateSpline();
	if(m_pFourParams)
		_CreateFourParams();
}

void CCaliCurve::AddCaliNode(const CaliNode & node)
{
	m_CaliNodes.push_back(node);
	
	// 如果已经生成过曲线，则需要重新生成；如果还未生成过曲线，则不需要在此生成
	if(m_pSpline)
		_CreateSpline();
	if(m_pFourParams)
		_CreateFourParams();
}

void CCaliCurve::DeleteCaliNode(const double &conc)
{
	vector<CaliNode>::iterator iter = m_CaliNodes.begin( );
	while(iter != m_CaliNodes.end())
	{
		if(iter->conc == conc)
			m_CaliNodes.erase(iter);
		iter++;
	}

	// 如果已经生成过曲线，则需要重新生成；如果还未生成过曲线，则不需要在此生成
	if(m_pSpline)
		_CreateSpline();
	if(m_pFourParams)
		_CreateFourParams();
}

double CCaliCurve::GetConcByRul (const double& rlu,const CurveType &curvetype)
{
	CurveType type;
	if(curvetype == Default)
		type = (CurveType)m_CurveRecord.DefaultFitType;
	else
		type = curvetype;

	switch(type)
	{
	case Spline:
		// 如果还未生成过曲线，则需要在此生成；如果已经生成过曲线，则不需要在此重新生成
		if(m_pSpline == NULL)
			_CreateSpline();
		if(m_bIsCurveErr)
			return -1;
		return m_pSpline->GetXByY(rlu);
		break;

	case FourParams:
		// 如果还未生成过曲线，则需要在此生成；如果已经生成过曲线，则不需要在此重新生成
		if(m_pFourParams == NULL)
			_CreateFourParams();
		if(m_bIsCurveErr)
			return -1;
		return m_pFourParams->GetXByY(rlu);
		break;
	default:
		return -1;
	}
}

double CCaliCurve::GetRluByConc (const double& conc,const CurveType &curvetype)
{
	CurveType type;
	if(curvetype == Default)
		type = (CurveType)m_CurveRecord.DefaultFitType;
	else
		type = curvetype;

	switch(type)
	{
	case Spline:
		// 如果还未生成过曲线，则需要在此生成；如果已经生成过曲线，则不需要在此重新生成
		if(m_pSpline == NULL)
			_CreateSpline();
		if(m_bIsCurveErr)
			return -1;
		return m_pSpline->GetYByX(conc);
		break;

	case FourParams:
		// 如果还未生成过曲线，则需要在此生成；如果已经生成过曲线，则不需要在此重新生成
		if(m_pFourParams == NULL)
			_CreateFourParams();
		if(m_bIsCurveErr)
			return -1;
		return m_pFourParams->GetYByX(conc);
		break;
	default:
		return -1;
	}
}

void CCaliCurve::_ParseCurveRecord(const CurveRecord & record)
{
	// 1.根据标准品ID取得标准品信息
	const StandardRecord & recStandard = CStandardTable::GetInstance()->GetRecordInfo(record.StandardID);
	CGeneralFuction *pFuntion = CGeneralFuction::GetInstance();

	CaliNode node;
	// 2.标准点
	for(int i=0; i< recStandard.CountOfStdPoints; i++)
	{
		node.conc = recStandard.StdConc[i];
		pFuntion->ParseRluString(record.RluString[i],node.RluList);
		node.rlu = pFuntion->Average(node.RluList);
		m_CaliNodes.push_back(node);
	}
	// 3.如果已经生成过曲线，则需要重新生成；如果还未生成过曲线，则不需要在此生成
	if(m_pSpline)
		_CreateSpline();
	if(m_pFourParams)
		_CreateFourParams();
}

void CCaliCurve::_CreateSpline()
{
	if(m_pSpline == NULL)
		m_pSpline = new CSplineCurve;
	vector<CCoordPoint> InterNodeArr;
	for(size_t i=0; i<m_CaliNodes.size();i++)
	{
		CCoordPoint node;

		if(m_CaliNodes[i].conc > 0)
		{
			//三次函数的x值取浓度的对数
			node.x = log(m_CaliNodes[i].conc);
			node.y = m_CaliNodes[i].rlu;
			InterNodeArr.push_back(node);
		}
	}
	m_pSpline->Reset(InterNodeArr);
}

void CCaliCurve::_CreateFourParams()
{
	if(m_pFourParams == NULL)
		m_pFourParams = new CFourParamsCurve;
	vector<CCoordPoint> InterNodeArr;
	for(size_t i=0; i<m_CaliNodes.size();i++)
	{
		CCoordPoint node;
		if(m_CaliNodes[i].conc > 0)
		{
			node.x = m_CaliNodes[i].conc;
			node.y = m_CaliNodes[i].rlu / m_CaliNodes[0].rlu;
			InterNodeArr.push_back(node);
		}
	}
	m_pFourParams->Reset(InterNodeArr);
}