//*****************************************************************************
// 文件名 : CCoordPoint.h
// 
// 概要   : 二维坐标点类型定义
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-6       崔志雷       新建
// ------------------------------------------
//*****************************************************************************
 
#pragma once

class CCoordPoint
{
public:
	CCoordPoint(void)
	{
		x = 0.f;
		y = 0.f;
	}

	CCoordPoint(double xx,double yy)
	{
		x = xx;
		y = yy;
	}

public:
	~CCoordPoint(void){};

public:
	double x;																	// x坐标
	double y;																	// y坐标

public:
	bool operator < (const CCoordPoint& node) const
	{
		return x < node.x;
	}

	bool operator > (const CCoordPoint& node) const
	{
		return x > node.x;
	}

	bool operator == (const CCoordPoint& node) const
	{
		return x == node.x;
	}
};
