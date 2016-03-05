//*****************************************************************************
// �ļ��� : FontsLib.h
// 
// ��Ҫ   : ����������
// 
// ��������     �޶���       �޸�����         
// ------------------------------------------
// 2013-12      ��־��       ������ԭ���ļ�
// ------------------------------------------
//*****************************************************************************

#pragma once

// ����ö��
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

// �������ݽṹ
struct FontData 
{
	TCHAR facename[LF_FACESIZE];				// ������
	UINT height;								// ����߶�
	BOOL bold;									// �Ƿ�Ӵ�
	BOOL italic;								// �Ƿ���б
	BOOL underline;								// �Ƿ����»���
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
	// ժҪ��
	//		����ָ�����ͷ�������
	// ��ע��
	//     �����߲��ܸı�ԭ������
	// --------------------------------------------------
	CFont *GetFont(FontLib type)
	{
		ASSERT(type >= FontBegin && type <FontNum);
		if (type < FontBegin || type > FontNum)
			return NULL;

		return m_fontList[type];
	}

	// --------------------------------------------------
	// ժҪ��
	//		����ָ�����ʹ����Զ����С������
	// ������
	//     1.type - ��������
	//     2.times - ���ű���
	//     3.pFont - ��������ָ��
	// ��ע��
	//     �����߸��𴴽������ɾ������
	// --------------------------------------------------
	BOOL CreateFont(FontLib type, double times, CFont* pFont);

	// --------------------------------------------------
	// ժҪ��
	//		����ָ�����ͷ�������
	// ������
	//     1.type - ��������
	//     2.new_size - ���ű���
	//     3.pFont - ��������ָ��
	// ��ע��
	//     �����߸��𴴽������ɾ������
	// --------------------------------------------------
	BOOL CreateFont(FontLib type, int new_size, CFont* pFont);
private:
	static CFontsLib * m_pInstance;
};
