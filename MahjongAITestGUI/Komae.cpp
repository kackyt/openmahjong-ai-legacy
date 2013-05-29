/****************************************************************************************
 * Copyright (c) 2010, Takaya Kakizaki(kacky)
 * All rights reserved.

  ソースコード形式かバイナリ形式か、変更するかしないかを問わず、以下の条件を満たす場合に限り、再頒布および使用が許可されます。 

  ・ソースコードを再頒布する場合、上記の著作権表示、本条件一覧、および下記免責条項を含めること。 

  ・バイナリ形式で再頒布する場合、頒布物に付属のドキュメント等の資料に、上記の著作権表示、本条件一覧、および下記免責条項を含めること。 

  ・書面による特別の許可なしに、本ソフトウェアから派生した製品の宣伝または販売促進に、オープン麻雀の名前またはコントリビューターの名前を使用してはならない。


  本ソフトウェアは、著作権者およびコントリビューターによって「現状のまま」提供されており、明示黙示を問わず、
  商業的な使用可能性、および特定の目的に対する適合性に関する暗黙の保証も含め、またそれに限定されない、いかなる保証もありません。
  著作権者もコントリビューターも、事由のいかんを問わず、 損害発生の原因いかんを問わず、かつ責任の根拠が契約であるか厳格責任であるか
  （過失その他の）不法行為であるかを問わず、仮にそのような損害が発生する可能性を知らされていたとしても、本ソフトウェアの使用によって発生した
  （代替品または代用サービスの調達、使用の喪失、データの喪失、利益の喪失、業務の中断も含め、またそれに限定されない）
  直接損害、間接損害、偶発的な損害、特別損害、懲罰的損害、または結果損害について、一切責任を負わないものとします。 

****************************************************************************************/
#include "StdAfx.h"
#include "Komae.h"
#pragma comment(lib, "msimg32.lib")

CKomae::CKomae(void)
: m_iImageNum(0)
, m_iLoopStart(0)
, m_iLoopEnd(0)
, m_iState(0)
, m_bVisible(TRUE)
{
}

CKomae::~CKomae(void)
{
	releaseImages();
}

int CKomae::loadImages(LPCTSTR strBase,COLORREF tColor,LPCTSTR strExt,int orig)
{
	CFile f;
	CString str;
	int count = 0;
	BOOL ret;
	DWORD dwSize;
	HGLOBAL h;
	void *pBuf;
	LPSTREAM pstm = NULL;
	HRESULT hr;
	LPPICTURE picture;
	HBITMAP hbmp1,hbmp2;
	BITMAP bmp;


	while(1){
		if(orig >= 0){
			str.Format(_T("%s%d.%s"),strBase,orig,strExt);
		}else{
			str.Format(_T("%s.%s"),strBase,strExt);
		}
		ret = f.Open(str,CFile::modeRead);
		if(ret == FALSE) break;
		dwSize = (DWORD)f.GetLength();
		h = ::GlobalAlloc(GMEM_MOVEABLE,dwSize);
		if(h == NULL){
			f.Close();
			break;
		}

		pBuf = ::GlobalLock(h);
		if(pBuf == NULL){
			f.Close();
			::GlobalFree(h);
			break;
		}

		if(f.Read(pBuf,dwSize) != dwSize){
			f.Close();
			::GlobalUnlock(h);
			::GlobalFree(h);
			break;
		}

		f.Close();
		::GlobalUnlock(h);

		hr = ::CreateStreamOnHGlobal(h,TRUE,&pstm);
		if(SUCCEEDED(hr) == FALSE){
			break;
		}
		hr = ::OleLoadPicture(pstm,dwSize,FALSE,IID_IPicture,(LPVOID *)&picture);
		if(SUCCEEDED(hr) == FALSE || picture == NULL){
			pstm->Release();
			break;
		}
		pstm->Release();


		// IPicture から Bitmapへ変換(ImageListを使って透過色とかを実現したいから)

		picture->get_Handle((OLE_HANDLE*)&hbmp1);

		hbmp2 = (HBITMAP)CopyImage(hbmp1,IMAGE_BITMAP,0,0,LR_COPYRETURNORG);

		picture->Release();

		if(count == 0 && m_iImageNum == 0){
			GetObject(hbmp2,sizeof(BITMAP),&bmp);
			m_iWidth = bmp.bmWidth;
			m_iHeight = bmp.bmHeight;
			m_imageList.Create(m_iWidth,m_iHeight,ILC_COLOR32|ILC_MASK,0,KOMA_MAX);
		}

		m_imageList.Add(CBitmap::FromHandle(hbmp2),tColor);		

		count++;
		if(orig < 0) break;
		orig++;
	}

	m_iImageNum += count;
	m_iState = 0;
	setLoopPointer(0,m_iImageNum - 1);
	return count;
}

int CKomae::loadImages(LPCTSTR strBase,LPCTSTR strExt,int orig)
{
	CFile f;
	CString str;
	int count = 0;
	BOOL ret;
	DWORD dwSize;
	HGLOBAL h;
	void *pBuf;
	LPSTREAM pstm = NULL;
	HRESULT hr;
	LPPICTURE picture;
	HBITMAP hbmp1,hbmp2;
	BITMAP bmp;


	while(1){
		if(orig >= 0){
			str.Format(_T("%s%d.%s"),strBase,orig,strExt);
		}else{
			str.Format(_T("%s.%s"),strBase,strExt);
		}
		ret = f.Open(str,CFile::modeRead);
		if(ret == FALSE) break;
		dwSize = (DWORD)f.GetLength();
		h = ::GlobalAlloc(GMEM_MOVEABLE,dwSize);
		if(h == NULL){
			f.Close();
			break;
		}

		pBuf = ::GlobalLock(h);
		if(pBuf == NULL){
			f.Close();
			::GlobalFree(h);
			break;
		}

		if(f.Read(pBuf,dwSize) != dwSize){
			f.Close();
			::GlobalUnlock(h);
			::GlobalFree(h);
			break;
		}

		f.Close();
		::GlobalUnlock(h);

		hr = ::CreateStreamOnHGlobal(h,TRUE,&pstm);
		if(SUCCEEDED(hr) == FALSE){
			break;
		}
		hr = ::OleLoadPicture(pstm,dwSize,FALSE,IID_IPicture,(LPVOID *)&picture);
		if(SUCCEEDED(hr) == FALSE || picture == NULL){
			pstm->Release();
			break;
		}
		pstm->Release();


		// IPicture から Bitmapへ変換(ImageListを使って透過色とかを実現したいから)

		picture->get_Handle((OLE_HANDLE*)&hbmp1);

		hbmp2 = (HBITMAP)CopyImage(hbmp1,IMAGE_BITMAP,0,0,LR_COPYRETURNORG);

		picture->Release();

		if(count == 0 && m_iImageNum == 0){
			GetObject(hbmp2,sizeof(bmp),&bmp);
			m_iWidth = bmp.bmWidth;
			m_iHeight = bmp.bmHeight;
			m_imageList.Create(m_iWidth,m_iHeight,ILC_COLOR32,0,KOMA_MAX);
		}

		m_imageList.Add(CBitmap::FromHandle(hbmp2),RGB(0,0,0));		

		count++;
		if(orig < 0) break;
		orig++;
	}

	m_iImageNum += count;
	m_iState = 0;
	setLoopPointer(0,m_iImageNum - 1);
	return count;
}

int CKomae::releaseImages(void)
{

	m_imageList.DeleteImageList();
	m_iImageNum = 0;

	return 0;
}

void CKomae::setLoopPointer(int loopStart, int loopEnd)
{
	m_iLoopStart = loopStart;
	m_iLoopEnd = loopEnd;
}

#if 0
CBitmap *CKomae::getCurrentImage(void)
{
	return &m_aBitmap[m_iState];
}
#endif

int CKomae::nextState(void)
{
	if(m_iState >= m_iLoopStart && m_iState <= m_iLoopEnd){
		if(++m_iState > m_iLoopEnd){
			m_iState = m_iLoopStart;
		}
	}

	return m_iState;
}

void CKomae::drawCurrentImage(CPaintDC &dc, int posx, int posy)
{
	m_imageList.Draw(&dc,m_iState,CPoint(posx,posy),ILD_TRANSPARENT);

}

void CKomae::setState(int newState)
{
	m_iState = newState;
}

void CKomae::setVisible(bool isVisible)
{
	m_bVisible = isVisible;
}

int CKomae::getState(void)
{
	return m_iState;
}

// αブレンディングして描画
void CKomae::drawCurrentImage(CPaintDC &dc, int posx, int posy,int alpha)
{
	CBitmap *pOldBitmap;
	CBitmap bitmap;
	CDC memDC;
	BLENDFUNCTION blendfunc;
	blendfunc.BlendOp = AC_SRC_OVER;
	blendfunc.BlendFlags = 0;
	blendfunc.SourceConstantAlpha = alpha;
	blendfunc.AlphaFormat = 0;

	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc,m_iWidth,m_iHeight);

	pOldBitmap = memDC.SelectObject(&bitmap);

	memDC.BitBlt(0,0,m_iWidth,m_iHeight,&dc,posx,posy,SRCCOPY);

	m_imageList.Draw(&memDC,m_iState,CPoint(0,0),ILD_TRANSPARENT);

	AlphaBlend(dc.GetSafeHdc(),posx,posy,m_iWidth,m_iHeight,memDC.GetSafeHdc(),0,0,m_iWidth,m_iHeight,blendfunc);

	memDC.SelectObject(pOldBitmap);

	memDC.DeleteDC();

}
