//*****************************************************************************
// 文件名 : Matrix.h
// 
// 概要   : 矩阵类 (定义文件)
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

#include "stdafx.h"
#include "Matrix.h"  

CMatrix::CMatrix(int c, int r,double initval):MATRIX<double>(c,r)
{  
	for ( int i = 0; i < r; ++i )  
	{  
		for ( int j = 0; j < c; ++j )  
		{  
			array[i][j] = initval;  
		}  
	}  
}  

CMatrix& CMatrix::operator =( const CMatrix& m )  
{  
	int r = m.rows();  
	int c = m.cols();  

	resize(r,c);

	for ( int i = 0; i < r; ++i )  
	{  
		for ( int j = 0; j < c; ++j )  
		{  
			array[i][j] = m[i][j];  
		}  
	}  

	return *this;  
}  

CMatrix& CMatrix::operator+=( const CMatrix& m )  
{  
	// 如果矩阵大小不同，则返回本矩阵，不进行运算
	if ( rows() != m.rows() || rows() != m.cols() )  
	{  
		ASSERT(0);
		return *this;  
	}  

	int r = rows();  
	int c = cols();  

	for ( int i = 0; i < r; ++i )  
	{  
		for ( int j = 0; j < c; ++j )  
		{  
			array[i][j] += m[i][j];  
		}  
	}  

	return *this;  
}  


CMatrix& CMatrix::operator-=( const CMatrix& m )  
{  
	// 如果矩阵大小不同，则返回本矩阵，不进行运算
	if ( rows() != m.rows() || cols() != m.cols() )  
	{  
		ASSERT(0);
		return *this;  
	}  

	int r = rows();  
	int c = cols();  

	for ( int i = 0; i < r; ++i )  
	{  
		for ( int j = 0; j < c; ++j )  
		{  
			array[i][j] -= m[i][j];  
		}  
	}  

	return *this;  
}  

CMatrix& CMatrix::operator*=( const CMatrix& m )  
{  
	// 如果矩阵大小不同，则返回本矩阵，不进行运算
	if ( cols() != m.rows() || !m.square() )  
	{  
		ASSERT(0);
		return *this;  
	}  

	CMatrix ret( rows(), cols() );  

	int r = rows();  
	int c = cols();  

	for ( int i = 0; i < r; ++i )  
	{  
		for ( int j = 0; j < c; ++j )  
		{  
			double sum = 0.0;  
			for ( int k = 0; k < c; ++k )  
			{  
				sum += array[i][k] * m[k][j];  
			}  
			ret[i][j] = sum;  
		}  
	}  

	*this = ret;  
	return *this;  
}  

CMatrix& CMatrix::operator/=( const CMatrix& m )  
{  
	CMatrix tmp = m.Inverse();  
	return operator*=( tmp );  
}  


bool CMatrix::operator==( const CMatrix& rhs ) const
{  
	if ( this->rows() != rhs.rows() || this->cols() != rhs.cols() )  
	{  
		return false;  
	}  

	for ( int i = 0; i < this->rows(); ++i )  
	{  
		for ( int j = 0; j < this->cols(); ++j )  
		{  
			if ( rhs[i][j] != rhs[i][j] )  
			{  
				return false;  
			}  
		}  
	}  

	return true;  
}  

bool CMatrix::operator!=( const CMatrix& rhs ) const
{  
	return !( (*this) == rhs );  
}  

CMatrix CMatrix::operator+( const CMatrix& rhs) const  
{  
	CMatrix m;  

	// 如果矩阵大小不同，则返回本矩阵，不进行运算
	if ( this->rows() != rhs.rows() || this->cols() != rhs.cols() )  
	{  
		ASSERT(0);
		return m;  
	}  

	m = (*this);  
	m += rhs;  

	return m;  
}  

CMatrix CMatrix::operator-( const CMatrix& rhs ) const
{  
	CMatrix m;  
	// 如果矩阵大小不同，则返回本矩阵，不进行运算
	if ( this->rows() != rhs.rows() || this->cols() != rhs.cols() )  
	{  
		ASSERT(0);
		return m;  
	}  

	m = (*this);  
	m -= rhs;  

	return m;  
}  

CMatrix CMatrix::operator*( const CMatrix& rhs ) const
{  
	CMatrix m;  
	// 如果左矩阵列数不等于右矩阵行数，则不运算
	if ( this->cols() != rhs.rows() )  
	{  
		ASSERT(0);
		return m;  
	}  

	m.resize( this->rows(), rhs.cols() );  

	int r = m.rows();  
	int c = m.cols();  
	int K = this->cols();  

	for ( int i = 0; i < r; ++i )  
	{  
		for ( int j = 0; j < c; ++j )  
		{  
			double sum = 0.0;  
			for ( int k = 0; k < K; ++k )  
			{  
				sum += (*this)[i][k] * rhs[k][j];  
			}  
			m[i][j] = sum;  
		}  
	}  

	return m;  
}  


CMatrix CMatrix::operator*( double aa ) const
{  
	int r = rows();  
	int c = cols();  

	CMatrix m;
	m.resize(r,c);
	for ( int i = 0; i < r; ++i )  
	{  
		for ( int j = 0; j < c; ++j )  
		{  
			m[i][j] = array[i][j] * aa;  
		}  
	}  
	return m;  
}  

CMatrix CMatrix::operator/( double aa ) const
{  
	ASSERT(aa != 0);
	return (*this) * (1.0 / aa);
}  

CMatrix CMatrix::operator/( const CMatrix& rhs ) const
{  
	CMatrix tmp = rhs.Inverse();  
	CMatrix m;  

	if ( tmp.empty() )  
	{  
		ASSERT(0);
		return m;  
	}  

	return m = (*this) * tmp;  
}  

// 计算方阵行列式  
double CMatrix::Det() const
{  
	// *******************************
	// 利用展开方法计算行列式
	// *******************************
	double ret = 0.0;  

	if ( empty() || !square() ) return ret;

	int ncols = cols();

	// 此处判断3*3方阵和2*2方阵仅仅是为了减少递归次数，提高运算效率
	switch(ncols)
	{
	case 3:
		ret =  array[0][0] * array[1][1] * array[2][2]
		     + array[0][1] * array[1][2] * array[2][0]
			 + array[0][2] * array[1][0] * array[2][1]
			 - array[0][2] * array[1][1] * array[2][0]
			 - array[0][1] * array[1][0] * array[2][2]
			 - array[0][0] * array[1][2] * array[2][1];
		break;

	case 2:
		ret = array[0][0] * array[1][1] - array[0][1] * array[1][0];
		break;

	case 1:
		ret = array[0][0];
		break;

	default:
		for(int i = 0; i < ncols; i++)
		{
			ret += (array[0][i] * Algebraic(0,i));
		}

	}
	return ret;  

	//// *******************************
	//// 利用变换方法计算行列式
	//// *******************************
	//double ret = 0.0;  

	//if ( empty() || !square() ) return ret;  

	//CMatrix N = LU();  

	//if ( N.empty() ) return ret;  

	//ret = 1.0;  
	//for ( int i = 0; i < N.cols(); ++ i )  
	//{  
	//	ret *= N[i][i];  
	//}  

	//if ( isSignRev( N[N.rows()-1] ))  
	//{  
	//	return -ret;  
	//}  

	//return ret;  
}  

// 计算矩阵指定子方阵的行列式   
double CMatrix::Det(int start, int end ) const
{  
	return SubMatrix(start, end, start, end).Det();  
}  

// 计算某元素的代数余子式式   
double CMatrix::Algebraic(int indexRow, int indexCol )  const
{  
	CMatrix m;
	// 代数余子式的行列数均小1
	int nrows = rows() - 1;
	int ncols = cols() - 1; 

	m.resize(nrows,ncols);
	for(int r = 0; r < nrows; r++)
	{
		int sr;

		if(r < indexRow)
			sr = r;
		else 
			sr = r + 1;

		for(int c = 0; c < ncols; c++)
		{
			int sc;
			if(c < indexCol)
				sc = c;
			else 
				sc = c + 1;
			m[r][c] = array[sr][sc];
		}
	}
	double t = m.Det();
	if( (indexRow + indexCol) % 2 > 0 )
		return - m.Det();
	else 
		return m.Det();  
}  

// 返回伴随矩阵   
CMatrix CMatrix::AdjointMatr() const
{  
	if(!square())
		return *this;

	int nrows = cols();  
	int ncols = rows();  

	CMatrix m(nrows,ncols);
	for(int r = 0; r < nrows; r ++)
	{
		for (int c = 0; c < ncols ; c++)
		{
			m[r][c] = Algebraic(c,r);
		}
	}
	return m;
}  



// 计算矩阵转置  
CMatrix CMatrix::Trans() const
{  
	CMatrix ret;  
	if (empty() ) return ret;  

	int c = cols();  
	int r = rows();  

	ret.resize(c, r);  
	for ( int i = 0; i < c; ++i )  
	{  
		for ( int j = 0; j < r; ++j )  
		{  
			ret[i][j] = array[j][i];  
		}  
	}  

	return ret;  
}  

// 计算逆矩阵  
// 等于伴随矩阵乘以行列式值的倒数  
CMatrix CMatrix::Inverse() const 
{  
	CMatrix ret;  

	if ( empty() || !square() )  
	{  
		return ret;  
	}  

	if(Det() == 0)
	{
		ASSERT(0);
		return *this;
	}
	return AdjointMatr() * (1.0 / Det());
}  

// 计算绝对值  
CMatrix CMatrix::Abs() const
{  
	CMatrix ret;  

	if( empty() )  
	{  
		return ret;  
	}  

	int r = rows();  
	int c = cols();  
	ret.resize( r, c );  

	for ( int i = 0; i < r; ++i )  
	{  
		for ( int j = 0; j < c; ++j )  
		{  
			double t = array[i][j];  
			if ( t < 0 ) ret[i][j] = -t;  
			else ret[i][j] = t;  
		}  
	}  

	return ret;  
}  

// 返回矩阵所有元素的最大值  
double CMatrix::Max() const
{  
	if ( empty() ) return 0.;  

	double ret = array[0][0];  
	int r = rows();  
	int c = cols();  

	for ( int i = 0; i < r; ++i )  
	{  
		for ( int j = 0; j < c; ++j )  
		{  
			if ( array[i][j] > ret ) ret = array[i][j];  
		}  
	}  
	return ret;  
}  

// 计算矩阵最大值，并返回该元素的引用  
double CMatrix::Max(int& row, int& col ) const
{  
	if ( empty() ) return 0.;  

	double ret = array[0][0];  
	row = 0;  
	col = 0;  

	int r = rows();  
	int c = cols();  

	for ( int i = 0; i < r; ++i )  
	{  
		for ( int j = 0; j < c; ++j )  
		{  
			if ( array[i][j] > ret )  
			{  
				ret = array[i][j];  
				row = i;  
				col = j;  
			}  
		}  
	}  
	return ret;  
}  

// 计算矩阵所有元素最小值  
double CMatrix::Min() const
{  
	if ( empty() ) return 0.;  

	double ret = array[0][0];  
	int r = rows();  
	int c = cols();  

	for ( int i = 0; i < r; ++i )  
	{  
		for ( int j = 0; j < c; ++j )  
		{  
			if ( array[i][j] > ret ) ret = array[i][j];  
		}  
	}  

	return ret;  
}  

// 计算矩阵最小值，并返回该元素的引用  
double CMatrix::Min( int& row, int& col) const
{  
	if ( empty() ) return 0.;  

	double ret = array[0][0];  
	row = 0;  
	col = 0;  
	int r = rows();  
	int c = cols();  

	for ( int i = 0; i < r; ++i )  
	{  
		for ( int j = 0; j < c; ++j )  
		{  
			if ( array[i][j] > ret )  
			{  
				ret = array[i][j];  
				row = i;  
				col = j;  
			}  
		}  
	}  

	return ret;  
}  

// 取矩阵中指定位置的子矩阵   
CMatrix CMatrix::SubMatrix(int rb,int re,int cb,int ce) const
{  
	CMatrix ret;  
	if ( empty() ) return ret;  

	if ( rb < 0 || re >= rows() || rb > re ) return ret;  
	if ( cb < 0 || ce >= cols() || cb > ce ) return ret;  

	ret.resize( re-rb+1, ce-cb+1 );  

	for ( int i = rb; i <= re; ++i )  
	{  
		for ( int j = cb; j <= ce; ++j )  
		{  
			ret[i-rb][j-cb] = array[i][j];  
		}  
	}  

	return ret;  
}  

// 计算方阵 M 的 LU 分解  
// 其中L为对角线元素全为1的下三角阵，U为对角元素依赖M的上三角阵  
// 使得 M = LU  
// 返回矩阵下三角部分存储L(对角元素除外)，上三角部分存储U(包括对角线元素)
// 返回矩阵最后一行保存行交换信息
CMatrix CMatrix::LU() const
{  
	CMatrix ret;  

	if ( empty() || !square() ) return ret;  

	int n = rows();  
	ret.resize( n+1, n );  

	for ( int i = 0; i < n; ++i )  
	{  
		ret[n][i] = -1.0;								// 初始化为小于0的数 
	}  

	for ( int i = 0; i < n; ++i )  
	{  
		for ( int j = 0; j < n; ++j )  
		{  
			ret[i][j] = array[i][j];  
		}  
	}  

	for ( int k = 0; k < n-1; ++k )  
	{  
		int p = ret.max_idx( k, n );  
		if ( p != k )									// 进行行交换，根据行列式性质，交换两行符号改变  
		{  
			ret.swap_row( k, p );  
			ret[n][k] = 1.0;							// 记录将换信息  
		}  

		if ( ret[k][k] == 0.0 )  
		{  
			ASSERT(0);
			ret.resize(0,0);  
			return ret;  
		}  

		for ( int i = k+1; i < n; ++i )  
		{  
			ret[i][k] /= ret[k][k];  
			for ( int j = k+1; j < n; ++j )  
			{  
				ret[i][j] -= ret[i][k] * ret[k][j];  
			}  
		}  
	}  

	return ret;  
}  
