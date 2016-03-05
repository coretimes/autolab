//*****************************************************************************
// �ļ��� : SplineAlgorithm.cpp
// 
// ��Ҫ   : ������ֵ�㷨�� (�����ļ�)
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-4       ��־��       �½�
// ------------------------------------------
//*****************************************************************************

#include "StdAfx.h"
#include "SplineCurve.h"
#include <algorithm>
#include <functional>
#include "float.h"
CSplineCurve::CSplineCurve()
{
}

CSplineCurve::CSplineCurve(const vector<CCoordPoint> &InterNodeArr)
{
	Reset(InterNodeArr);
}

CSplineCurve::~CSplineCurve(void)
{
	m_InterNodeArr.clear();
	m_M.clear();
}

bool CSplineCurve::Reset(const vector<CCoordPoint> &InterNodeArr)
{
	m_InterNodeArr.clear();
	m_NodeCount = (int)InterNodeArr.size();
	if( m_NodeCount < 3)
		return false;
	for(int i= 0; i < m_NodeCount; i++)
	{
		m_InterNodeArr.push_back(InterNodeArr[i]);
	}
	sort(m_InterNodeArr.begin(), m_InterNodeArr.end(), less<CCoordPoint>());

	m_M.resize(m_NodeCount);
	_Calculate();

	for(int i=0; i < m_NodeCount; i++)
	{
		if(_isnan(m_M[i]))
			return false;
	}
	return true;
}

double CSplineCurve::GetYByX(const double &x) const
{
	if( m_NodeCount < 3)
		return 0;
	return	S(x);																	// ����S(x)��������ֵ
}

//  **************************************************
//  ��ţ����ɽ����������xֵ
//  **************************************************
double CSplineCurve::GetXByY(const double &y) const
{
	if( m_NodeCount < 3)
		return 0;

	const double e = 1.0/100000.0;													// Ĭ�Ͼ���ֵΪʮ���֮һ
	double x,p;
	int j = _FindIntervalByY(y);													// ���ҳ�x�����ĸ���
	x = m_InterNodeArr[j].x;														// ��ʼ������ֵ														
	do
	{
		p = x;
		double c = 1;																// ��ɽ����,Ϊ��֤����
		do
		{
			x = p - c * (S(p) - y)/dS(p);											// ע��������S(x) - y = 0,����f(x)��S(x) - y ������S(x);
			c = c / 2;
		} while(abs(S(x) - y) > abs(S(p) - y));
	}while(abs(x - p) > e);
	
	if(x >= m_InterNodeArr[j].x && x<= m_InterNodeArr[j+1].x)						// ����������xֵ������֮�䣬˵��������Ч
		return x;

	x = m_InterNodeArr[j+1].x;														// �����������֮�䣬˵��ѡȡ��ֵ���ԡ��������������ǵ���ʱ����ѡȡ��ֵ�й�
	do
	{
		p = x;
		double c = 1;																// ��ɽ����,Ϊ��֤����
		do
		{
			x = p - c * (S(p) - y)/dS(p);											// ע��������S(x) - y = 0,����f(x)��S(x) - y ������S(x);
			c = c / 2;
		} while(abs(S(x) - y) > abs(S(p) - y));
	}while(abs(x - p) > e);
	return x;	
}

void CSplineCurve::_Calculate()
{
	vector<double> Beta,Y;
	Beta.push_back(0);																// ��0 = 0

	for (int i =1 ; i < m_NodeCount; i++)											// ��i ����
	{
		double Betai = Lambda(i)/( 2 - Mu(i) * Beta[i-1]);
		Beta.push_back(Betai);
	}

	Y.push_back(0);																	// Y0 = 0
	for (int i =1 ; i < m_NodeCount; i++)											// Yi ����
	{
		double Yi = (d(i) - Mu(i) * Y[i-1])/( 2 - Mu(i) * Beta[i-1]);
		Y.push_back(Yi);
	}

	M(m_NodeCount - 1) = 0;															// M0 = 0
	for (int i = m_NodeCount - 2 ; i >= 0; i--)										// Mi ����
	{
		M(i) = Y[i] - Beta[i] * M(i+1);
	}
}

int CSplineCurve::_FindIntervalByX(const double& x) const		
{
	if (x < x(0))																	// ��ֵ�ڵ��Ǵ�С�������е�
		return 0;
	for(int i = 1;i < m_NodeCount - 1; i++)
	{
		if( x <= x(i))
			return i-1;
	}
	return m_NodeCount - 2;
}

int CSplineCurve::_FindIntervalByY(const double& y) const		
{
	if (y < y(0))																	// ��ֵ�ڵ��Ǵ�С�������е�
		return 0;
	for(int i = 1;i < m_NodeCount - 1; i++)
	{
		if( y >= y(i))
			return i-1;
	}
	return m_NodeCount - 2;
}

int CSplineCurve::GetInflectionPointCount()	const		
{
	//---------------------------------------------------
	//	�յ㴦�Ķ��׵���ֵΪ0
	//	��ֵ������ÿ�κ����ϵĶ��׵�������һ��ֱ��
	//  ����˶������˵�Ķ��׵������Ż��죬����йյ�
	//	���������������ߣ���Ϊ���˵㶼�Ƕ��׵���Ϊ��ģ�
	//	���Ե�һ�κ�����������һ�κ������䲻�����йյ�
	//---------------------------------------------------
	int num = 0;
	for(int i=1; i<m_NodeCount-2;i++)
	{																																	 
		if(M(i) == 0)																// ���M(i) == 0 ˵���˵��ǹյ�
			num++;
		if(M(i)*M(i+1) < 0)															 
			num++;																	
	}
	return num;
}