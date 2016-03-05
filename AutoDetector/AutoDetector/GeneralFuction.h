//*****************************************************************************
// 文件名 : GeneralFuction.h
// 
// 概要   : 通用函数，对可能会反复调用的与成员无关的函数，统一定义在此类下
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-9       崔志雷       新建
// ------------------------------------------
//*****************************************************************************
#pragma once
#include <vector>
#include <atlimage.h>

using namespace std; 
class CGeneralFuction
{
private:
	CGeneralFuction(void){};

public:
	static CGeneralFuction * GetInstance()
	{
		if(m_pInstance == NULL)
			m_pInstance = new CGeneralFuction();
		return m_pInstance;
	}
	// --------------------------------------------------
	// 摘要：
	//	   删除实体
	// --------------------------------------------------
	static void DestroyInstance()
	{
		if (m_pInstance != NULL)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	~CGeneralFuction(void){};

	// --------------------------------------------------
	// 摘要：
	//     求和函数
	// 参数：
	//     1.datalist - 给定数据数组
	// 返回：
	//     数组各元素和
	// --------------------------------------------------
	double Sum(const vector<double> & datalist) const;

	// --------------------------------------------------
	// 摘要：
	//     求平均值函数
	// 参数：
	//     1.datalist - 给定数据数组
	// 返回：
	//     数组各元素平均值
	// --------------------------------------------------
	double Average(const vector<double> & datalist) const;

	// --------------------------------------------------
	// 摘要：
	//     求变异系数函数
	// 参数：
	//     1.datalist - 给定数据数组
	// 返回：
	//     数组的变异系数
	// --------------------------------------------------
	double GetCV(const vector<double> & datalist) const;

	// --------------------------------------------------
	// 摘要：
	//     求方差
	// 参数：
	//     1.datalist - 给定数据数组
	// 返回：
	//     数组的方差
	// --------------------------------------------------
	double Variance(const vector<double> & datalist) const;
	
	// --------------------------------------------------
	// 摘要：
	//     求标准差
	// 参数：
	//     1.datalist - 给定数据数组
	// 返回：
	//     数组的标准差差
	// --------------------------------------------------
	double StdDev(const vector<double> & datalist) const;

	// --------------------------------------------------
	// 摘要：
	//     解析发光值字符串，把各发光值保存在给定数组中
	// 参数：
	//     1.strRluList - 字符串格式形如：123131/119259/130034
	//     2.arrRluList - 指定的保存发光值的数组
	// --------------------------------------------------
	void ParseRluString(const CString &strRluList,vector<double> & arrRluList) const;

	// --------------------------------------------------
	// 摘要：
	//	   绘制阴影字体
	// --------------------------------------------------
	void DrawShadowCation(CDC * pDc,const CRect &rcTextRect,const CString &strContext);

	// --------------------------------------------------
	// 摘要：
	//	 绘制垂直文本  
	// --------------------------------------------------
	int DrawVerticalText(CDC* pDC, const CString &strText,const CRect & rcTextRect, const UINT &format);

	// --------------------------------------------------
	// 摘要：
	//	 加载自定义资源函数  
	// --------------------------------------------------
	bool LoadImageFromResource(CImage& image,const UINT &nResID,const LPCTSTR &lpTyp);

private:
	static CGeneralFuction * m_pInstance;
};
