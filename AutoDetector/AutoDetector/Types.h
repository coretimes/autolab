#pragma once

typedef unsigned char		uchar;
typedef unsigned char		uint8;
typedef	  signed char		sint8;
typedef unsigned short		uint16;
typedef	  signed short		sint16;
typedef unsigned int		uint32;
typedef	  signed int		sint32;
typedef unsigned __int64	uint64;
typedef   signed __int64	sint64;

typedef uint32				TestUnit_ID;		// 测试单元ID
typedef uint32				Metering_ID;		// 测光节点ID
typedef uint16				Tick_ID;			// 仪器计时周期
typedef uint32				Table_Key;			// 数据库表key值
typedef uint32				Sample_Key;			// 样本表key值
typedef uint32				Standard_Key;		// 标准品表key值
typedef uint32				Curve_Key;			// 标准曲线key值
typedef uint32				Result_Key;			// 结果表key值
typedef uint32				Reagent_Key;		// 试剂表key值
typedef uint16				Sample_Pos;			// 样本区位置坐标
typedef uint16				Reagent_Pos;		// 试剂区位置坐标
typedef uint16				Incu_Pos;			// 孵育区位置坐标
typedef int					ConcPoint_Index;	// 同一测试单元内，浓度点索引
typedef int					Metering_Index;		// 同一样本测试节点内，测光节点索引

// 项目名称和简称组成的“双名称”类型
typedef struct
{
	CString ProjectName;		// 项目名称
	CString ShortName;			// 项目简称
} ProjectFullName;

//*****************************************************
// 定标曲线类型
//*****************************************************
enum CurveType
{
	Default = 0,
	Spline,	
	FourParams
};
