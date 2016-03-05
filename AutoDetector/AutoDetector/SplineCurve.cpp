//*****************************************************************************
// 文件名 : SplineAlgorithm.cpp
// 
// 概要   : 样条差值算法类 (定义文件)
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-4       崔志雷       新建
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
	return	S(x);																	// 利用S(x)函数计算值
}

//  **************************************************
//  用牛顿下山迭代法逆推x值
//  **************************************************
double CSplineCurve::GetXByY(const double &y) const
{
	if( m_NodeCount < 3)
		return 0;

	const double e = 1.0/100000.0;													// 默认精度值为十万分之一
	double x,p;
	int j = _FindIntervalByY(y);													// 查找出x属于哪个段
	x = m_InterNodeArr[j].x;														// 初始化坐标值														
	do
	{
		p = x;
		double c = 1;																// 下山因子,为保证收敛
		do
		{
			x = p - c * (S(p) - y)/dS(p);											// 注：方程是S(x) - y = 0,所以f(x)是S(x) - y 而不是S(x);
			c = c / 2;
		} while(abs(S(x) - y) > abs(S(p) - y));
	}while(abs(x - p) > e);
	
	if(x >= m_InterNodeArr[j].x && x<= m_InterNodeArr[j+1].x)						// 如果计算出的x值在区间之间，说明计算有效
		return x;

	x = m_InterNodeArr[j+1].x;														// 如果不在区间之间，说明选取初值不对。当三次样条不是单调时，与选取初值有关
	do
	{
		p = x;
		double c = 1;																// 下山因子,为保证收敛
		do
		{
			x = p - c * (S(p) - y)/dS(p);											// 注：方程是S(x) - y = 0,所以f(x)是S(x) - y 而不是S(x);
			c = c / 2;
		} while(abs(S(x) - y) > abs(S(p) - y));
	}while(abs(x - p) > e);
	return x;	
}

void CSplineCurve::_Calculate()
{
	vector<double> Beta,Y;
	Beta.push_back(0);																// β0 = 0

	for (int i =1 ; i < m_NodeCount; i++)											// βi 递推
	{
		double Betai = Lambda(i)/( 2 - Mu(i) * Beta[i-1]);
		Beta.push_back(Betai);
	}

	Y.push_back(0);																	// Y0 = 0
	for (int i =1 ; i < m_NodeCount; i++)											// Yi 递推
	{
		double Yi = (d(i) - Mu(i) * Y[i-1])/( 2 - Mu(i) * Beta[i-1]);
		Y.push_back(Yi);
	}

	M(m_NodeCount - 1) = 0;															// M0 = 0
	for (int i = m_NodeCount - 2 ; i >= 0; i--)										// Mi 递推
	{
		M(i) = Y[i] - Beta[i] * M(i+1);
	}
}

int CSplineCurve::_FindIntervalByX(const double& x) const		
{
	if (x < x(0))																	// 插值节点是从小到大排列的
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
	if (y < y(0))																	// 插值节点是从小到大排列的
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
	//	拐点处的二阶导数值为0
	//	插值函数在每段函数上的二阶导函数是一条直线
	//  如果此段两个端点的二阶导数符号互异，则必有拐点
	//	对于自由样条曲线，因为两端点都是二阶导数为零的，
	//	所以第一段函数区间和最后一段函数区间不可能有拐点
	//---------------------------------------------------
	int num = 0;
	for(int i=1; i<m_NodeCount-2;i++)
	{																																	 
		if(M(i) == 0)																// 如果M(i) == 0 说明此点是拐点
			num++;
		if(M(i)*M(i+1) < 0)															 
			num++;																	
	}
	return num;
}