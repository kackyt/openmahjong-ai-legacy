// TestView.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MahjongAITestGUI.h"
#include "TestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestView

CTestView::CTestView()
{
	m_haigaList.loadImages(_T("haiga/man"),RGB(0,255,255),_T("gif"),1);
	m_haigaList.loadImages(_T("haiga/pin"),RGB(0,255,255),_T("gif"),1);
	m_haigaList.loadImages(_T("haiga/sou"),RGB(0,255,255),_T("gif"),1);
	m_haigaList.loadImages(_T("haiga/ton"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("haiga/nan"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("haiga/sha"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("haiga/pei"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("haiga/haku"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("haiga/hatu"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("haiga/tyun"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("haiga/ura"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("haiga/manred"),RGB(0,255,255),_T("gif"),1);
	m_haigaList.loadImages(_T("haiga/pinred"),RGB(0,255,255),_T("gif"),1);
	m_haigaList.loadImages(_T("haiga/soured"),RGB(0,255,255),_T("gif"),1);
	m_haigaList.loadImages(_T("haiga/tonred"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("haiga/nanred"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("haiga/shared"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("haiga/peired"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("haiga/hakured"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("haiga/hatured"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("haiga/tyun"),RGB(0,255,255),_T("gif"),-1);

	reset();
}

CTestView::~CTestView()
{
}


BEGIN_MESSAGE_MAP(CTestView, CButton)
	//{{AFX_MSG_MAP(CTestView)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestView メッセージ ハンドラ

void CTestView::OnPaint() 
{
	int i;
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	CRect rect;
	
	GetClientRect(&rect);
	
	dc.FillSolidRect(&rect,RGB(0,127,0));
	dc.SetBkMode(TRANSPARENT);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	for(i=0;i<m_iTehaiSize;i++){
		m_haigaList.setState(m_aTehai[i]);
		m_haigaList.drawCurrentImage(dc,i*25+20,50);
	}
	
	for(i=0;i<m_iSutehaiSize;i++){
		m_haigaList.setState(m_aSutehai[i]);
		m_haigaList.drawCurrentImage(dc,i*25+20,10);
	}

	if(m_iTsumohai >= 0){
		m_haigaList.setState(m_iTsumohai);
		m_haigaList.drawCurrentImage(dc,375,50);
	}
	// 描画用メッセージとして CButton::OnPaint() を呼び出してはいけません
}

void CTestView::reset()
{
	m_iTehaiSize = 0;
	m_iTsumohai = -1;
	m_iSutehaiSize = 0;
}
