//*****************************************************************************
// 文件名 : FourParamsAlgorithm.h
// 
// 概要   : 四参数拟合算法类 (头文件)
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-7       崔志雷       新建
// ------------------------------------------
//*****************************************************************************
//
//		       四参数拟合算法原理
// ------------------------------------------
//  四参数方程形式:
//  y =		(Am - A0) / ( 1 + (X/X0) ^ P) + A0
//  
//  拟合算法：Levenberg-Marquardt算法
//*****************************************************************************

#pragma once
#include "CoordPoint.h"
#include <assert.h>
#include <vector>
#include "math.h"
#include "CoordPoint.h"
#include "Matrix.h"

using namespace std;

class CFourParamsCurve
{
public:
	CFourParamsCurve (const vector<CCoordPoint> & SamplePointArr);	// 构造对象时,需要输入拟合样本点
	bool Reset(const vector<CCoordPoint> &SamplePointArr);					// 重新设置曲线样本点
	double GetCorrelationCoe() const;										// 取得相关系数
	double GetYByX(const double &x) const;									// 根据给定的x坐标值,取得该曲线上的y坐标值
	double GetXByY(const double &y) const;									// 根据给定的y坐标值,逆推x值,即求方程S(x) - y = 0根
	double GetAm()const { return m_Am; }
	double GetA0()const { return m_A0; }
	double GetX0()const { return m_X0; }
	double GetP()const { return m_P; }
public:
	CFourParamsCurve(void);
	~CFourParamsCurve(void);

private:
	double m_A0;														// 参数A0
	double m_Am;														// 参数Am
	double m_X0;														// 参数X0
	double m_P;															// 参数P

private:
	vector<CCoordPoint> m_SamplePointArr;								// 曲线的样本节点数组
	int m_PointCount;													// 样本节点数量

private:
	void _Calculate();													// 计算函数
	void _Logit_Linear();												// 做Logit-Ln线性回归
	void _LM_Nonelinear();												// Levenberg-Marquardt 非线性回归

	inline double dAm(double x)	const									// 方程对Am参数的偏导数
	{
		return 1.0 / ( 1.0 + pow( x / m_X0, m_P));
	}

	inline double dA0(double x)	const									// 方程对A0参数的偏导数
	{
		return 1.0 - dAm(x);
	}

	inline double dX0(double x)	const									// 方程对X0参数的偏导数
	{
		if(abs(x) < 0.00001 )
			x = 0.00001;
		return  ( m_P / m_X0  ) * 
				( m_Am - m_A0 ) * 
				(pow(x/m_X0, m_P)) /
				pow((1 + pow(x / m_X0, m_P)), 2);
	}

	inline double dP(double x)	const									// 方程对P参数的偏导数
	{
		if(abs(x) < 0.00001 )
			x = 0.00001;
		return  -log(x / m_X0) * dX0(x) * m_X0 / m_P; 
	}

	inline void JacobiMatr(CMatrix & mJ)	const						// 当前参数状态下的雅克比矩阵
	{
		mJ.resize(m_PointCount,4);
		for(int row = 0; row < m_PointCount; row ++)
		{
			mJ[row][0] = dAm(m_SamplePointArr[row].x); 
			mJ[row][1] = dA0(m_SamplePointArr[row].x); 
			mJ[row][2] = dX0(m_SamplePointArr[row].x); 
			mJ[row][3] = dP(m_SamplePointArr[row].x); 
		}
	}

	inline void ParamsMatr(CMatrix & mP)	const						// 当前参数状态下的参数矩阵
	{
		mP.resize(4,1);
		mP[0][0] = m_Am; 
		mP[1][0] = m_A0; 
		mP[2][0] = m_X0; 
		mP[3][0] = m_P; 
	}

	inline double YResiduals()	const									// 计算测量y值和计算y值的残差
	{
		double dRes = 0;
		for(int i = 0; i < m_PointCount; i ++)
		{
			dRes += pow((m_SamplePointArr[i].y - GetYByX(m_SamplePointArr[i].x)),2);
																		// 即 var_y = sum((Y[i] - fitY[i])^2),测量y值与计算y值差的平方
		}
		return dRes;
	}
	inline void YDevMatr(CMatrix & mY)	const							// 计算实际y值和计算y值的误差
	{
		mY.resize(m_PointCount,1);
		for(int i = 0; i < m_PointCount; i ++)
		{
			mY[i][0] = m_SamplePointArr[i].y - GetYByX(m_SamplePointArr[i].x);
		}
	}
};
