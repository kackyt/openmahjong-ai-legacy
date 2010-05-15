#pragma once
#define KOMA_MAX (256)

class CKomae
{
public:
public:
	CKomae(void);
public:
	~CKomae(void);
private:
#if 0
	CBitmap m_aBitmap[KOMA_MAX];
#endif
	CImageList m_imageList;

public:
	int m_iImageNum;
public:
	int loadImages(LPCTSTR strBase,COLORREF tColor,LPCTSTR strExt =_T("jpg"),int orig = 0);
	int loadImages(LPCTSTR strBase,LPCTSTR strExt =_T("jpg"),int orig = 0);
public:
	int m_iLoopStart;
public:
	int m_iLoopEnd;
public:
	int releaseImages(void);
public:
	void setLoopPointer(int loopStart, int loopEnd);
#if 0
public:
	CBitmap *getCurrentImage(void);
#endif
public:
	int m_iState;
public:
	int nextState(void);
public:
	void drawCurrentImage(CPaintDC & dc, int posx, int posy);
	void drawCurrentImage(CPaintDC & dc, int posx, int posy,int alpha);
public:
	void setState(int newState);
public:
	BOOL m_bVisible;
public:
	void setVisible(bool isVisible);
public:
	int getState(void);
public:
	int m_iHeight;
	int m_iWidth;
	void setAlpha(int alpha);
};
