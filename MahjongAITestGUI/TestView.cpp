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
	m_haigaList.loadImages(_T("../res/haiga/man"),RGB(0,255,255),_T("gif"),1);
	m_haigaList.loadImages(_T("../res/haiga/pin"),RGB(0,255,255),_T("gif"),1);
	m_haigaList.loadImages(_T("../res/haiga/sou"),RGB(0,255,255),_T("gif"),1);
	m_haigaList.loadImages(_T("../res/haiga/ton"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("../res/haiga/nan"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("../res/haiga/sha"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("../res/haiga/pei"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("../res/haiga/haku"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("../res/haiga/hatu"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("../res/haiga/tyun"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("../res/haiga/ura"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("../res/haiga/manred"),RGB(0,255,255),_T("gif"),1);
	m_haigaList.loadImages(_T("../res/haiga/pinred"),RGB(0,255,255),_T("gif"),1);
	m_haigaList.loadImages(_T("../res/haiga/soured"),RGB(0,255,255),_T("gif"),1);
	m_haigaList.loadImages(_T("../res/haiga/tonred"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("../res/haiga/nanred"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("../res/haiga/shared"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("../res/haiga/peired"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("../res/haiga/hakured"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("../res/haiga/hatured"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("../res/haiga/tyun"),RGB(0,255,255),_T("gif"),-1);

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
	int count = 0;
	
	GetClientRect(&rect);
	
	dc.FillSolidRect(&rect,RGB(0,127,0));
	dc.SetBkMode(TRANSPARENT);

	for (i = 0; i < m_tehai.tehai_max; i++){
		m_haigaList.setState(m_tehai.tehai[i]);
		m_haigaList.drawCurrentImage(dc, i * 25 + 20, 50);
	}
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
#if 0
	for(i=0;i<m_iTehaiSize;i++){
		m_haigaList.setState(m_aTehai[i]);
		m_haigaList.drawCurrentImage(dc,i*25+20,50);
	}
#endif
	
	for(i=0;i<m_iSutehaiSize;i++){
		m_haigaList.setState(m_aSutehai[i]);
		m_haigaList.drawCurrentImage(dc,i*25+20,10);
	}

	for (i = 0; i < m_tehai.ankan_max; i++){
		m_haigaList.setState(34);
		m_haigaList.drawCurrentImage(dc, 375 - count * 75 - 50, 50);
		m_haigaList.setState(m_tehai.ankan[i]);
		m_haigaList.drawCurrentImage(dc, 375 - count * 75 - 25, 50);
		m_haigaList.setState(34);
		m_haigaList.drawCurrentImage(dc, 375 - count * 75, 50);
		count++;
	}

	for (i = 0; i < m_tehai.minkan_max; i++){
		m_haigaList.setState(34);
		m_haigaList.drawCurrentImage(dc, 375 - count * 75 - 50, 50);
		m_haigaList.setState(m_tehai.minkan[i]);
		m_haigaList.drawCurrentImage(dc, 375 - count * 75 - 25, 50);
		m_haigaList.setState(m_tehai.minkan[i]);
		m_haigaList.drawCurrentImage(dc, 375 - count * 75, 50);
		count++;
	}

	for (i = 0; i < m_tehai.minkou_max; i++){
		m_haigaList.setState(m_tehai.minkou[i]);
		m_haigaList.drawCurrentImage(dc, 375 - count * 75 - 50, 50);
		m_haigaList.setState(m_tehai.minkou[i]);
		m_haigaList.drawCurrentImage(dc, 375 - count * 75 - 25, 50);
		m_haigaList.setState(m_tehai.minkou[i]);
		m_haigaList.drawCurrentImage(dc, 375 - count * 75, 50);
		count++;
	}

	for (i = 0; i < m_tehai.minshun_max; i++){
		m_haigaList.setState(m_tehai.minshun[i]);
		m_haigaList.drawCurrentImage(dc, 375 - count * 75 - 50, 50);
		m_haigaList.setState(m_tehai.minshun[i]+1);
		m_haigaList.drawCurrentImage(dc, 375 - count * 75 - 25, 50);
		m_haigaList.setState(m_tehai.minshun[i]+2);
		m_haigaList.drawCurrentImage(dc, 375 - count * 75, 50);
		count++;
	}

	if (m_iTsumohai >= 0){
		m_haigaList.setState(m_iTsumohai);
		m_haigaList.drawCurrentImage(dc, m_tehai.tehai_max * 25 + 30, 50);
	}
	// 描画用メッセージとして CButton::OnPaint() を呼び出してはいけません
}

void CTestView::reset()
{
	memset(&m_tehai, 0, sizeof(m_tehai));
	m_iTsumohai = -1;
	m_iSutehaiSize = 0;
}
