// MyButton.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "MyButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyButton

CMyButton::CMyButton()
{
}

CMyButton::~CMyButton()
{
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
	//{{AFX_MSG_MAP(CMyButton)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyButton メッセージ ハンドラ

HBRUSH CMyButton::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CButton::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: この位置で DC のアトリビュートを変更してください
	if((GetState() & 0x0003) == 1){
		SetFont(&m_pushedFont);
		return m_pushedBrush;
	}else{
		SetFont(&m_defFont);
	}
	
	// TODO: デフォルトのブラシが望みのものでない場合には、違うブラシを返してください
	return hbr;
}

void CMyButton::PreSubclassWindow() 
{
	CFont *pFont;
	LOGFONT lFont;
	//SetButtonStyle(BS_AUTOCHECKBOX | BS_PUSHLIKE);
	m_pushedBrush.CreateSolidBrush(RGB(255,0,0));
	pFont = GetFont();
	pFont->GetLogFont(&lFont);

	m_defFont.CreateFontIndirect(&lFont);
	lFont.lfHeight = 11 * lFont.lfHeight / 10;
	lFont.lfWidth = 0;

	m_pushedFont.CreateFontIndirect(&lFont);

	
	CButton::PreSubclassWindow();
}
