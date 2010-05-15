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
