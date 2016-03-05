#include "StdAfx.h"
#include "GeneralFuction.h"
#include <math.h>
#include "FontLib.h"

CGeneralFuction * CGeneralFuction::m_pInstance = NULL;

double CGeneralFuction::Sum(const vector<double> & datalist) const
{
	double sum = 0;													

	for(size_t i = 0; i < datalist.size(); i++)
	{
		sum += datalist[i];
	}
	return sum;
}

double CGeneralFuction::Average(const vector<double> & datalist) const
{
	double num = double(datalist.size());
	if(num > 0)
		return Sum(datalist) / num;
	else
		return 0;
}

double CGeneralFuction::GetCV(const vector<double> & datalist) const
{
	return (StdDev(datalist) / Average(datalist))*100;
}

double CGeneralFuction::Variance(const vector<double> & datalist) const
{
	size_t num = datalist.size();
	double variance = 0;
	double aver = Average(datalist);
	for(size_t i = 0; i < num; i++)
	{
		double data = pow((datalist[i] - aver),2);
		variance += data;
	}

	variance /= double(num - 1);
	return variance;
}

double CGeneralFuction::StdDev(const vector<double> & datalist) const
{
	return sqrt(Variance(datalist));
}

void CGeneralFuction::ParseRluString(const CString &strRluList,vector<double> & arrRluList) const
{
	int nGroupStart = 0;
	int nTotalLength = strRluList.GetLength();
	CString rlu;
	while(nGroupStart < nTotalLength)
	{
		int pos = strRluList.Find(_T("/"),nGroupStart);
		if(pos < 0)
		{													
			rlu = strRluList.Right(nTotalLength - nGroupStart);
			nGroupStart = nTotalLength;											
		}																	
		else												
		{																	
			rlu = strRluList.Mid(nGroupStart,pos - nGroupStart);			
			nGroupStart = pos + 1;												
		}																	
		arrRluList.push_back(_tstof(rlu.GetBuffer(0)));
	}
}
void CGeneralFuction::DrawShadowCation(CDC * pDc,const CRect &rcTextRect,const CString &strContext)
{
	int nSaveDc = pDc->SaveDC();
	CFontsLib * pFontLib = CFontsLib::GetInstance();
	pDc->SelectObject(pFontLib->GetFont(FontTitle));

	CRect rect = rcTextRect;
	rect.left += 4;
	rect.top += 2;
	pDc->SetBkMode(TRANSPARENT);
	pDc->SetTextColor(RGB(128,128,128));
	pDc->DrawText(strContext,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	rect.left -= 4;
	rect.top -= 2;
	pDc->SetTextColor(RGB(255,255,255));
	pDc->DrawText(strContext,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	pDc->RestoreDC(nSaveDc);
}

int CGeneralFuction::DrawVerticalText(CDC* pDC, const CString &strText,const CRect & rcTextRect, const UINT &format)
{
	if(pDC == NULL)
		return 0;

	CString strTemp;
	int i = 0;

	// 循环遍历整个字符串，在每个字符后均加一组回车换行符
	while (i < strText.GetLength())
	{
		strTemp += strText.GetAt(i);
		strTemp += _T("\r\n");
		i ++;
	}

	RECT rect = rcTextRect;
	if (format & DT_VCENTER)
	{
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);
		rect.top += (rect.bottom - rect.top - tm.tmHeight * strText.GetLength()) / 2 - 2;
	}

	return pDC->DrawText(strTemp, &rect, format);
}

bool CGeneralFuction::LoadImageFromResource(CImage& image,const UINT &nResID,const LPCTSTR &lpTyp)
{
	image.Destroy();

	// 查找资源
	HRSRC hRsrc = ::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(nResID), lpTyp);
	if (hRsrc == NULL) return false;

	// 加载资源
	HGLOBAL hImgData = ::LoadResource(AfxGetResourceHandle(), hRsrc);
	if (hImgData == NULL)
	{
		::FreeResource(hImgData);
		return false;
	}

	// 锁定内存中的指定资源
	LPVOID lpVoid    = ::LockResource(hImgData);

	LPSTREAM pStream = NULL;
	DWORD dwSize    = ::SizeofResource(AfxGetResourceHandle(), hRsrc);
	HGLOBAL hNew    = ::GlobalAlloc(GHND, dwSize);
	LPBYTE lpByte    = (LPBYTE)::GlobalLock(hNew);
	::memcpy(lpByte, lpVoid, dwSize);

	// 解除内存中的指定资源
	::GlobalUnlock(hNew);

	// 从指定内存创建流对象
	HRESULT ht = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
	if ( ht != S_OK )
	{
		GlobalFree(hNew);
	}
	else
	{
		// 加载图片
		image.Load(pStream);

		GlobalFree(hNew);
	}

	// 释放资源
	::FreeResource(hImgData);

	return true;
}