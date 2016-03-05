//*****************************************************************************
// 文件名 : FontsLib.h
// 
// 概要   : 字体库管理类
// 
// 更新日期     修订人       修改内容         
// ------------------------------------------
// 2013-12      崔志雷       复制与原有文件
// ------------------------------------------
//*****************************************************************************

#pragma once

// 字体枚举
enum FontLib
{
	FontBegin = 0,
	FontDefault = FontBegin,
	FontTabs,
	FontTabsUnderline,
	FontTitle,
	FontTitleUnderline,
	FontSerialNo,
	FontSerialNoUnderline,
	FontText,
	FontTextBond,
	//FontField,
	//FontFieldValue,
	//FontLink,
	//FontLinkHover,
	//FontConfigSubTitle,
	//FontConfigSelSubTitle,
	//FontListHeader,
	//FontListItem,
	//FontButton,
	//FontBigButton,
	//FontStatusText,
	FontNum
};

// 字体数据结构
struct FontData 
{
	TCHAR facename[LF_FACESIZE];				// 字体名
	UINT height;								// 字体高度
	BOOL bold;									// 是否加粗
	BOOL italic;								// 是否倾斜
	BOOL underline;								// 是否有下划线
};

class CFontsLib
{
private:
	CFontsLib(void);

public:
	static CFontsLib * GetInstance()
	{
		if(m_pInstance == NULL)
			m_pInstance = new CFontsLib();
		return m_pInstance;
	}

	static void DestroyInstance()
	{
		if (m_pInstance != NULL)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	~CFontsLib(void);

protected:
	CFont *m_fontList[FontNum];

public:
	// --------------------------------------------------
	// 摘要：
	//		根据指定类型返回字体
	// 备注：
	//     调用者不能改变原有字体
	// --------------------------------------------------
	CFont *GetFont(FontLib type)
	{
		ASSERT(type >= FontBegin && type <FontNum);
		if (type < FontBegin || type > FontNum)
			return NULL;

		return m_fontList[type];
	}

	// --------------------------------------------------
	// 摘要：
	//		根据指定类型创建自定义大小的字体
	// 参数：
	//     1.type - 字体类型
	//     2.times - 缩放倍数
	//     3.pFont - 传入字体指针
	// 备注：
	//     调用者负责创建字体的删除工作
	// --------------------------------------------------
	BOOL CreateFont(FontLib type, double times, CFont* pFont);

	// --------------------------------------------------
	// 摘要：
	//		根据指定类型返回字体
	// 参数：
	//     1.type - 字体类型
	//     2.new_size - 缩放倍数
	//     3.pFont - 传入字体指针
	// 备注：
	//     调用者负责创建字体的删除工作
	// --------------------------------------------------
	BOOL CreateFont(FontLib type, int new_size, CFont* pFont);
private:
	static CFontsLib * m_pInstance;
};
