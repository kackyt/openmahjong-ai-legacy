// MahjongStatic.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "MahjongStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMahjongStatic

CMahjongStatic::CMahjongStatic()
{
	m_iType = TYPE_KOUHAI;
}

CMahjongStatic::~CMahjongStatic()
{
}


BEGIN_MESSAGE_MAP(CMahjongStatic, CStatic)
	//{{AFX_MSG_MAP(CMahjongStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMahjongStatic メッセージ ハンドラ

void CMahjongStatic::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	CFont agariFont,*pOldFont;
	int i;
	CString str;
	dc.SetBkMode(TRANSPARENT);
	agariFont.CreateFont(20,0,0,0,FW_BOLD,TRUE,FALSE,FALSE,
		SHIFTJIS_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DRAFT_QUALITY,DEFAULT_PITCH,_T("ＭＳ ゴシック"));

	switch(m_iType){
	case TYPE_START:

		pOldFont = dc.SelectObject(&agariFont);				

		for(i=0;i<m_result.m_aYaku.GetSize();i++){
			dc.TextOut(0,20 + 25 * i,m_result.m_aYaku[i]);
		}

		str.Format(_T("%d翻 %d符 %d点"),m_result.m_iHan,m_result.m_iFu,m_result.m_iScore);
		dc.TextOut(0,270,str);

		dc.SelectObject(pOldFont);
		break;
	case TYPE_KOUHAI:
		pOldFont = dc.SelectObject(&agariFont);

		dc.TextOut(100,150,_T("荒牌平局"));

		dc.SelectObject(pOldFont);
		break;
	}
	
	// 描画用メッセージとして CStatic::OnPaint() を呼び出してはいけません
}
