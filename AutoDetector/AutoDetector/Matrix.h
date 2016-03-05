//*****************************************************************************
// 文件名 : Matrix.h
// 
// 概要   : 矩阵类
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-6       崔志雷       新建
// ------------------------------------------
//*****************************************************************************
// double类型矩阵类，用于科学计算  
// 继承自MATRIX类  
// 实现常用操作符重载，并实现计算矩阵的行列式、逆以及LU分解  
//*****************************************************************************
//注：行列索引值均从0开始  
#pragma once
#include "MatrixTemplate.h"

class CMatrix:public MATRIX<double>  
{  
public:  
	CMatrix():MATRIX<double>(){}  
	CMatrix( int c, int r ):MATRIX<double>(c,r){}  
	CMatrix( int c, int r,double initval);  
	CMatrix( const CMatrix& m){ *this  = m; }  

	CMatrix& operator =( const CMatrix& m );  
	CMatrix& operator+=( const CMatrix& m );  
	CMatrix& operator-=( const CMatrix& m );  
	CMatrix& operator*=( const CMatrix& m );  
	CMatrix& operator/=( const CMatrix& m );  

	bool operator==( const CMatrix& rhs ) const;							// 重载操作符==  
	bool operator!=( const CMatrix& rhs ) const;							// 重载操作符!=  
	CMatrix operator+( const CMatrix& rhs ) const;							// 重载操作符+  
	CMatrix operator-( const CMatrix& rhs ) const;							// 重载操作符-  
	CMatrix operator*( const CMatrix& rhs ) const;							// 重载操作符*，两个矩阵相乘  
	CMatrix operator*( double aa ) const;									// 重载操作符*，矩阵乘上一个数值
	CMatrix operator/( const CMatrix& rhs ) const;							// 重载操作符/  
	CMatrix operator/( double aa ) const;									// 重载操作符/，矩阵除上一个数值
	double Det() const;														// 计算行列式  
	double Det(int start, int end ) const;									// 计算子矩阵行列式,
	double Algebraic(int indexRow, int indexCol ) const;					// 计算某元素的代数余子式式
	CMatrix AdjointMatr() const;											// 返回伴随矩阵
	CMatrix Abs() const;													// 计算所有元素的绝对值  
	double Max() const;														// 所有元素的最大值  
	double Max(int& row, int& col) const;									// 所有元素中的最大值及其下标  
	double Min() const;														// 所有元素的最小值  
	double Min(int& row, int& col) const;									// 所有元素的最小值及其下标  
	CMatrix Trans() const;													// 返回转置矩阵  
	CMatrix SubMatrix(int rb,int re,int cb,int ce) const;					// 返回子矩阵  
	CMatrix Inverse() const;												// 计算逆矩阵  
	CMatrix LU() const;														// 计算方阵的LU分解  

private:
	inline double LxAbs( double d )	const									// 求绝对值
	{  
		return (d>=0)?(d):(-d);  
	}  

	inline bool isSignRev( const vector<double>& v )	const				// 求行列式值时，判断是否变号
	{  
		int p = 0;  
		int sum = 0;  
		int n = (int)v.size();  

		for ( int i = 0; i < n; ++i )  
		{  
			p = (int)v[i];  
			if ( p >= 0 )  
			{  
				sum += p;  
			}  
		}  

		// 如果是偶数，说明不变号  
		if ( sum % 2 == 0 ) 
		{  
			return false;  
		}  
		return true;  
	}  
	inline int max_idx(int k, int n )  const
	{  
		// 返回k列数据中，k行以下最大元素的行号
		int p = k;  
		for ( int i = k+1; i < n; ++i )  
		{  
			if ( LxAbs(array[p][k]) < LxAbs(array[i][k]) )  
			{  
				p = i;  
			}  
		}  
		return p;  
	}  
};  
