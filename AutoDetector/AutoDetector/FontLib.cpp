#include "StdAfx.h"
#include "FontLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

FontData m_fontStyleList[] = {
	//{字体名, 字体大小, 加粗标志, 倾斜标志, 下划线标志}
	{_T("Tahoma"), 14, 0, 0, 0},		// FontDefault
	{_T("Tahoma"), 18, 1, 0, 0},		// FontTabs
	{_T("Tahoma"), 18, 1, 0, 1},		// FontTabsUnderline
	{_T("Tahoma"), 18, 0, 0, 0},		// FontTitle
	{_T("Tahoma"), 18, 0, 0, 1},		// FontTitleUnderline
	{_T("Tahoma"), 14, 1, 0, 0},		// FontSerialNo
	{_T("Tahoma"), 14, 1, 0, 1},		// FontSerialNoUnderline
	{_T("Tahoma"), 12, 0, 0, 0},		// FontText
	{_T("Tahoma"), 12, 1, 0, 0}			// FontTextBond
	//{_T("Tahoma"), 14, 0, 0, 0},		// FontField
	//{_T("Tahoma"), 18, 0, 0, 0},		// FontFieldValue
	//{_T("Tahoma"), 12, 0, 0, 0},		// FontLink
	//{_T("Tahoma"), 12, 0, 0, 1},		// FontLinkHover
	//{_T("Tahoma"), 14, 0, 0, 0},		// FontConfigSubTitle
	//{_T("Tahoma"), 14, 1, 0, 0},		// FontConfigSelSubTitle
	//{_T("Tahoma"), 14, 0, 0, 0},		// FontListHeader
	//{_T("Tahoma"), 13, 0, 0, 0},		// FontListItem
	//{_T("Tahoma"), 14, 0, 0, 0},		// FontButton
	//{_T("Tahoma"), 22, 0, 0, 0},		// FontBigButton
	//{_T("Tahoma"), 28, 1, 0, 0}			// FontStatusText
};

CFontsLib * CFontsLib::m_pInstance = NULL;
CFontsLib::CFontsLib(void)
{
	LOGFONT lf;
	CFont *pFont;

	for (int i = FontBegin; i < FontNum; i ++)
	{
		ZeroMemory(&lf, sizeof(lf));
		pFont = NULL;

		_tcscpy_s(lf.lfFaceName, LF_FACESIZE, m_fontStyleList[i].facename);
		lf.lfHeight = m_fontStyleList[i].height;
		lf.lfWeight = m_fontStyleList[i].bold ? FW_HEAVY : FW_NORMAL;
		lf.lfItalic = m_fontStyleList[i].italic;
		lf.lfUnderline = m_fontStyleList[i].underline;

		pFont = new CFont();
		pFont->CreateFontIndirect(&lf);
		m_fontList[i] = pFont;
	}
}

CFontsLib::~CFontsLib(void)
{
	for (int i = FontBegin; i < FontNum; i ++)
	{
		delete m_fontList[i];
		m_fontList[i] = NULL;
	}
}

BOOL CFontsLib::CreateFont(FontLib type, double times, CFont* pFont)
{
	if (type < FontBegin || type > FontNum || times <= 0 || pFont == NULL)
	{
		ASSERT(0);
		return FALSE;
	}

	LOGFONT lf;

	ZeroMemory(&lf, sizeof(lf));

	_tcscpy_s(lf.lfFaceName, LF_FACESIZE, m_fontStyleList[type].facename);
	lf.lfHeight = (long)(m_fontStyleList[type].height * times);
	lf.lfWeight = m_fontStyleList[type].bold ? FW_HEAVY : FW_NORMAL;
	lf.lfItalic = m_fontStyleList[type].italic;
	lf.lfUnderline = m_fontStyleList[type].underline;

	if (pFont->m_hObject != NULL)
	{
		ASSERT(0);		// only attach once, detach on destroy
		if (pFont->DeleteObject() == FALSE)
			return FALSE;
	}

	return pFont->CreateFontIndirect(&lf);
}

BOOL CFontsLib::CreateFont(FontLib type, int new_size, CFont* pFont)
{
	if (type < FontBegin || type > FontNum || new_size <= 0 || pFont == NULL)
	{
		ASSERT(0);
		return FALSE;
	}

	LOGFONT lf;

	ZeroMemory(&lf, sizeof(lf));

	_tcscpy_s(lf.lfFaceName, LF_FACESIZE, m_fontStyleList[type].facename);
	lf.lfHeight = new_size;
	lf.lfWeight = m_fontStyleList[type].bold ? FW_HEAVY : FW_NORMAL;
	lf.lfItalic = m_fontStyleList[type].italic;
	lf.lfUnderline = m_fontStyleList[type].underline;

	if (pFont->m_hObject != NULL)
	{
		ASSERT(0);		// only attach once, detach on destroy
		if (pFont->DeleteObject() == FALSE)
			return FALSE;
	}

	return pFont->CreateFontIndirect(&lf);
}
