// MahjongBtn.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "MahjongBtn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMahjongBtn

CMahjongBtn::CMahjongBtn()
{
	m_bActive = FALSE;
	m_iSelMode = 0;
	
	/* 0 ～ 37 */
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
	m_haigaList.loadImages(_T("haiga/man5red"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("haiga/pin5red"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList.loadImages(_T("haiga/sou5red"),RGB(0,255,255),_T("gif"),-1);
	
	m_haigaList180.loadImages(_T("haiga/tman"),RGB(0,255,255),_T("gif"),1);
	m_haigaList180.loadImages(_T("haiga/tpin"),RGB(0,255,255),_T("gif"),1);
	m_haigaList180.loadImages(_T("haiga/tsou"),RGB(0,255,255),_T("gif"),1);
	m_haigaList180.loadImages(_T("haiga/tton"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList180.loadImages(_T("haiga/tnan"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList180.loadImages(_T("haiga/tsha"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList180.loadImages(_T("haiga/tpei"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList180.loadImages(_T("haiga/thaku"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList180.loadImages(_T("haiga/thatu"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList180.loadImages(_T("haiga/ttyun"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList180.loadImages(_T("haiga/tura"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList180.loadImages(_T("haiga/tman5red"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList180.loadImages(_T("haiga/tpin5red"),RGB(0,255,255),_T("gif"),-1);
	m_haigaList180.loadImages(_T("haiga/tsou5red"),RGB(0,255,255),_T("gif"),-1);
	/* 38 ～ */
	m_haigaListYoko.loadImages(_T("haiga/yman"),RGB(0,255,255),_T("gif"),1);
	m_haigaListYoko.loadImages(_T("haiga/ypin"),RGB(0,255,255),_T("gif"),1);
	m_haigaListYoko.loadImages(_T("haiga/ysou"),RGB(0,255,255),_T("gif"),1);
	m_haigaListYoko.loadImages(_T("haiga/yton"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko.loadImages(_T("haiga/ynan"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko.loadImages(_T("haiga/ysha"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko.loadImages(_T("haiga/ypei"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko.loadImages(_T("haiga/yhaku"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko.loadImages(_T("haiga/yhatu"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko.loadImages(_T("haiga/ytyun"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko.loadImages(_T("haiga/yura"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko.loadImages(_T("haiga/yman5red"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko.loadImages(_T("haiga/ypin5red"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko.loadImages(_T("haiga/ysou5red"),RGB(0,255,255),_T("gif"),-1);
	
	m_haigaListYoko180.loadImages(_T("haiga/tyman"),RGB(0,255,255),_T("gif"),1);
	m_haigaListYoko180.loadImages(_T("haiga/typin"),RGB(0,255,255),_T("gif"),1);
	m_haigaListYoko180.loadImages(_T("haiga/tysou"),RGB(0,255,255),_T("gif"),1);
	m_haigaListYoko180.loadImages(_T("haiga/tyton"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko180.loadImages(_T("haiga/tynan"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko180.loadImages(_T("haiga/tysha"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko180.loadImages(_T("haiga/typei"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko180.loadImages(_T("haiga/tyhaku"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko180.loadImages(_T("haiga/tyhatu"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko180.loadImages(_T("haiga/tytyun"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko180.loadImages(_T("haiga/tyura"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko180.loadImages(_T("haiga/tyman5red"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko180.loadImages(_T("haiga/typin5red"),RGB(0,255,255),_T("gif"),-1);
	m_haigaListYoko180.loadImages(_T("haiga/tysou5red"),RGB(0,255,255),_T("gif"),-1);
}

CMahjongBtn::~CMahjongBtn()
{
}


BEGIN_MESSAGE_MAP(CMahjongBtn, CButton)
//{{AFX_MSG_MAP(CMahjongBtn)
ON_WM_PAINT()
ON_WM_RBUTTONUP()
ON_WM_LBUTTONUP()
ON_WM_RBUTTONDOWN()
ON_WM_LBUTTONDOWN()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMahjongBtn メッセージ ハンドラ
#define PLAYER_OFFSETX  (90)
#define PLAYER_OFFSETY  (450)
const UINT ieTable2[][4] = {
	{ 0, 1, 2, 3},
	{ 3, 0, 1, 2},
	{ 2, 3, 0, 1},
	{ 1, 2, 3, 0},
};

void CMahjongBtn::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	CRect rect;
	CRect paiRect;
	CMember *member;
	CString str;
	CPen selPen,*pOldPen;
	CFont agariFont,*pOldFont;
	CBrush nullbrush;
	CBrush* pOldBrush;
	int i,j,count;
	const TCHAR *kyokuTable[] = { _T("東一局"),_T("東二局"),_T("東三局"),_T("東四局"),_T("南一局"),_T("南二局"),_T("南三局"),_T("南四局") };
	const TCHAR *ieTable[] = {_T("東家"),_T("南家"),_T("西家"),_T("北家")};
	
	GetClientRect(&rect);
	
	selPen.CreatePen(PS_SOLID,4,RGB(255,0,0));
	nullbrush.CreateStockObject(NULL_BRUSH);        // 透明ブラシを用意
	
	dc.FillSolidRect(&rect,RGB(0,127,0));
	dc.SetBkMode(TRANSPARENT);
	
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
	if(m_bActive){
		dc.TextOut(200,180,kyokuTable[m_taku.m_iKyokuCount]);
		str.Format(_T("残り : %d"),m_taku.m_iYama);
		dc.TextOut(270,180,str);
		str.Format(_T("%d本場"),m_taku.m_iTsumibou);
		dc.TextOut(210,200,str);
		str.Format(_T("リーチ棒×%d"),m_taku.m_iRiichibou);
		dc.TextOut(270,200,str);
		
		//dc.TextOut(220,200,_T("ドラ"));
		for(i=0;i<4;i++){
			if(i<m_taku.m_aDora.GetSize()){
				m_haigaList.setState((UINT)m_taku.m_aDora[i]);
			}else{
				m_haigaList.setState(34);
			}
			
			m_haigaList.drawCurrentImage(dc,220 + 25 * i,230);
			
			if(i<m_taku.m_aUradora.GetSize()){
				m_haigaList.setState((UINT)m_taku.m_aUradora[i]);
			}else{
				m_haigaList.setState(34);
			}
			m_haigaList.drawCurrentImage(dc,220 + 25 * i,265);
		}
		
		count = 0;
		
		for(i=0;i<4;i++){
			if(i!=m_iPlayerIndex){
				member = &m_taku.m_members[i];
				str.Format(_T("%s  %s %d点"),member->m_player.m_strName,ieTable[member->m_gamestate.m_iZikaze - 1],member->m_iPoint);
				if(m_iPlayerIndex != -1){
					count = ieTable2[m_taku.m_members[m_iPlayerIndex].m_gamestate.m_iZikaze - 1][member->m_gamestate.m_iZikaze - 1] - 1;
					paintPai(dc,rect,str,member,count);
				}
			}
		}
		
		if(m_iPlayerIndex != -1){
			member = &m_taku.m_members[m_iPlayerIndex];
			str.Format(_T("%s  %s %d点"),member->m_player.m_strName,ieTable[member->m_gamestate.m_iZikaze - 1],member->m_iPoint);

			paintPai(dc,rect,str,member,3);
			
			pOldPen = dc.SelectObject(&selPen);
			pOldBrush = dc.SelectObject(&nullbrush);
			for(j=0;j<getSelMode();j++){
				if(m_selIndex[j] != -1){
					if(member->m_gamestate.m_bTsumo && m_selIndex[j]==member->m_aTehai.GetSize() - 1){
						rect.SetRect(PLAYER_OFFSETX + 25 * m_selIndex[j] + 5,PLAYER_OFFSETY,PLAYER_OFFSETX + 25 * m_selIndex[j] + 24 + 5,PLAYER_OFFSETY+35);
					}else{
						rect.SetRect(PLAYER_OFFSETX + 25 * m_selIndex[j],PLAYER_OFFSETY,PLAYER_OFFSETX + 25 * m_selIndex[j] + 24,PLAYER_OFFSETY+35);
					}
					dc.Rectangle(&rect);
				}
			}
			dc.SelectObject(pOldPen);
			dc.SelectObject(pOldBrush);
		}
		
	}
	
}

void CMahjongBtn::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	CButton::OnRButtonUp(nFlags, point);
}

void CMahjongBtn::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	CButton::OnLButtonUp(nFlags, point);
}

void CMahjongBtn::refresh(int index,CTaku& value)
{
	m_taku = value;
	m_bActive = TRUE;
	m_iPlayerIndex = index;
	Invalidate(FALSE);
}

void CMahjongBtn::PreSubclassWindow() 
{
	// TODO: この位置に固有の処理を追加するか、または基本クラスを呼び出してください
	SetButtonStyle(BS_OWNERDRAW,FALSE);
	
	CButton::PreSubclassWindow();
}

void CMahjongBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: 指定されたアイテムを描画するためのコードを追加してください
	
}

void CMahjongBtn::setSelMode(int mode)
{
	m_iSelMode = mode;
	m_selIndex[0] = -1;
	m_selIndex[1] = -1;
	Invalidate(FALSE);
}

int CMahjongBtn::getSelMode()
{
	return m_iSelMode;
}

void CMahjongBtn::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bActive && (m_taku.m_iTurn != m_iPlayerIndex)) GetParent()->PostMessage(WM_SNDCOMMAND,120,0);
	
	CButton::OnRButtonDown(nFlags, point);
}

void CMahjongBtn::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rect;
	int i;
	CMember *member = &m_taku.m_members[m_iPlayerIndex];
	
	if(m_bActive){
		for(i=0;i<member->m_aTehai.GetSize();i++){
			if(member->m_gamestate.m_bTsumo && i==member->m_aTehai.GetSize() - 1){
				rect.SetRect(PLAYER_OFFSETX + 25 * i + 5,PLAYER_OFFSETY,PLAYER_OFFSETX + 25 * i + 24 + 5,PLAYER_OFFSETY+35);
			}else{
				rect.SetRect(PLAYER_OFFSETX + 25 * i,PLAYER_OFFSETY,PLAYER_OFFSETX + 25 * i + 24,PLAYER_OFFSETY+35);
			}
			if(rect.PtInRect(point)){
				if(getSelMode() == 0 && (m_taku.m_iTurn == m_iPlayerIndex)){
					GetParent()->PostMessage(WM_SNDCOMMAND,100 + i,0);
				}else{
					if(i==m_selIndex[0]){
						m_selIndex[0] = m_selIndex[1];
						m_selIndex[1] = -1;
					}else if(i==m_selIndex[1]){
						m_selIndex[1] = -1;
					}else{
						if(m_selIndex[0] == -1){
							m_selIndex[0] = i;
						}else{
							m_selIndex[m_iSelMode-1] = i;
						}
					}
					
				}
				break;
			}
		}
	}
	Invalidate(FALSE);
	
	CButton::OnLButtonDown(nFlags, point);
}

void CMahjongBtn::paintPai(CPaintDC &dc,CRect &rect,CString &str,CMember *member,int count)
{
	CKomae *pPaiga,*pPaigaYoko;
	CFont memFont,*pOldFont;
	int i,j,k,offset,ind,num;
	int posx,posy;
	int tmpx,tmpy;
	CBrush nullbrush;
	CBrush* pOldBrush;
	CPen selPen,*pOldPen;

	selPen.CreatePen(PS_SOLID,4,RGB(0,0,255));
	nullbrush.CreateStockObject(NULL_BRUSH);        // 透明ブラシを用意

	memFont.CreateFont(12,0,0,0,
		FW_NORMAL,
		FALSE,FALSE,FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,
		FIXED_PITCH | FF_MODERN,
        _T("HG丸ｺﾞｼｯｸM-PRO"));

	pOldFont = dc.SelectObject(&memFont);
	
	if(count == 0){
		dc.TextOut(370,370,str);
		posx = 380;
		posy = 300;
		pPaiga = &m_haigaListYoko;
		pPaigaYoko = &m_haigaList180;
	}else if(count == 1){
		dc.TextOut(370,70,str);
		posx = 340;
		posy = 130;
		pPaiga = &m_haigaList180;
		pPaigaYoko = &m_haigaListYoko180;
	}else if(count == 2){
		dc.TextOut(40,70,str);
		posx = 150;
		posy = 150;
		pPaiga = &m_haigaListYoko180;
		pPaigaYoko = &m_haigaList;
	}else if(count == 3){
		dc.TextOut(40,370,str);
		posx = 190;
		posy = 330;
		pPaiga = &m_haigaList;
		pPaigaYoko = &m_haigaListYoko;
	}

	dc.SelectObject(pOldFont);

	tmpx = posx;
	tmpy = posy;

	pOldPen = dc.SelectObject(&selPen);
	pOldBrush = dc.SelectObject(&nullbrush);
	num = 0;
	for(j=0;j<member->m_aDahai.GetSize();j++){
		if(member->m_aDahai[j].m_bNaki) continue;
		if(member->m_aDahai[j].m_bRiichi){
			pPaigaYoko->setState((UINT)member->m_aDahai[j]);
			if(count == 0){
				if(m_rule.m_iGlasshai == 1 && member->m_aDahai[j].m_iId != 4){
					pPaigaYoko->drawCurrentImage(dc,tmpx+8,tmpy-10,120);
				}else{
					pPaigaYoko->drawCurrentImage(dc,tmpx+8,tmpy-10);
				}
				if(j==member->m_aDahai.GetSize() - 1 && member->m_aDahai[j].m_bTsumogiri){
					dc.Rectangle(tmpx+8,tmpy-10,tmpx+8+24,tmpy-10+34);
				}
				tmpy -= 35;
			}else if(count == 1){
				if(m_rule.m_iGlasshai == 1 && member->m_aDahai[j].m_iId != 4){
					pPaigaYoko->drawCurrentImage(dc,tmpx-10,tmpy,120);
				}else{
					pPaigaYoko->drawCurrentImage(dc,tmpx-10,tmpy);
				}
				if(j==member->m_aDahai.GetSize() - 1 && member->m_aDahai[j].m_bTsumogiri){
					dc.Rectangle(tmpx-10,tmpy,tmpx-10+32,tmpy+24);
				}
				tmpx -= 35;
			}else if(count == 2){
				if(m_rule.m_iGlasshai == 1 && member->m_aDahai[j].m_iId != 4){
					pPaigaYoko->drawCurrentImage(dc,tmpx,tmpy,120);
				}else{
					pPaigaYoko->drawCurrentImage(dc,tmpx,tmpy);
				}
				if(j==member->m_aDahai.GetSize() - 1 && member->m_aDahai[j].m_bTsumogiri){
					dc.Rectangle(tmpx,tmpy,tmpx+24,tmpy+34);
				}
				tmpy += 35;
			}else if(count == 3){
				if(m_rule.m_iGlasshai == 1 && member->m_aDahai[j].m_iId != 4){
					pPaigaYoko->drawCurrentImage(dc,tmpx,tmpy+10,120);
				}else{
					pPaigaYoko->drawCurrentImage(dc,tmpx,tmpy+10);
				}
				if(j==member->m_aDahai.GetSize() - 1 && member->m_aDahai[j].m_bTsumogiri){
					dc.Rectangle(tmpx,tmpy+10,tmpx-10+32,tmpy+24);
				}
				tmpx += 35;
			}
		}else{
			pPaiga->setState((UINT)member->m_aDahai[j]);
			if(m_rule.m_iGlasshai == 1 && member->m_aDahai[j].m_iId != 4){
				pPaiga->drawCurrentImage(dc,tmpx,tmpy,120);
			}else{
				pPaiga->drawCurrentImage(dc,tmpx,tmpy);
			}
			if(count == 0){
				if(j==member->m_aDahai.GetSize() - 1 && member->m_aDahai[j].m_bTsumogiri){
					dc.Rectangle(tmpx,tmpy,tmpx+32,tmpy+24);
				}
				tmpy -= 25;
			}else if(count == 1){
				if(j==member->m_aDahai.GetSize() - 1 && member->m_aDahai[j].m_bTsumogiri){
					dc.Rectangle(tmpx,tmpy,tmpx+24,tmpy+34);
				}
				tmpx -= 25;
			}else if(count == 2){
				if(j==member->m_aDahai.GetSize() - 1 && member->m_aDahai[j].m_bTsumogiri){
					dc.Rectangle(tmpx,tmpy,tmpx+32,tmpy+24);
				}
				tmpy += 25;
			}else if(count == 3){
				if(j==member->m_aDahai.GetSize() - 1 && member->m_aDahai[j].m_bTsumogiri){
					dc.Rectangle(tmpx,tmpy,tmpx+24,tmpy+34);
				}
				tmpx += 25;
			}
		}
		if((num + 1) % 6 == 0 && num < 12){
			if(count == 0){
				tmpx += 35;
				tmpy = posy;
			}else if(count == 1){
				tmpx = posx;
				tmpy -= 35;
			}else if(count == 2){
				tmpx -= 35;
				tmpy = posy;
			}else if(count == 3){
				tmpx = posx;
				tmpy += 35;
			}
		}
		num++;
	}

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);


	if(member->m_aTehai.GetSize()>0){
		for(j=0;j<(member->m_gamestate.m_bTsumo ? member->m_aTehai.GetSize() - 1 : (member->m_aTehai.GetSize()));j++){
			pPaiga->setState((UINT)member->m_aTehai[j]);
			if(count == 0){
				tmpx = 520;
				tmpy = 400-25*j;
			}else if(count == 1){
				tmpx = 480-25*j;
				tmpy = 5;
			}else if(count == 2){
				tmpx = 5;
				tmpy = 60+25*j;
			}else if(count == 3){
				tmpx = PLAYER_OFFSETX+25*j;
				tmpy = PLAYER_OFFSETY;
			}
			if(m_rule.m_iGlasshai == 1 && member->m_aTehai[j] != PAI_NIL && member->m_aTehai[j].m_iId != 4){
				pPaiga->drawCurrentImage(dc,tmpx,tmpy,120);
			}else{
				pPaiga->drawCurrentImage(dc,tmpx,tmpy);
			}
		}
		if(count == 0){
			tmpy -= 30;
		}else if(count == 1){
			tmpx -= 30;
		}else if(count == 2){
			tmpy += 30;
		}else if(count == 3){
			tmpx += 30;
		}
		if(member->m_gamestate.m_bTsumo){
			pPaiga->setState((UINT)member->m_aTehai[j]);
			if(m_rule.m_iGlasshai == 1 && member->m_aTehai[j].m_iId != 4){
				pPaiga->drawCurrentImage(dc,tmpx,tmpy,120);
			}else{
				pPaiga->drawCurrentImage(dc,tmpx,tmpy);
			}
		}
	}else{
		for(j=0;j<13-member->m_gamestate.m_aNakiList.GetSize()*3;j++){
			if(count == 0){
				tmpx = 520;
				tmpy = 400-25*j;
			}else if(count == 1){
				tmpx = 480-25*j;
				tmpy = 5;
			}else if(count == 2){
				tmpx = 5;
				tmpy = 60+25*j;
			}else if(count == 3){
				tmpx = PLAYER_OFFSETX+25*j;
				tmpy = PLAYER_OFFSETY;
			}
			pPaiga->setState(34);
			pPaiga->drawCurrentImage(dc,tmpx,tmpy);
		}

		if(count == 0){
			tmpy -= 30;
		}else if(count == 1){
			tmpx -= 30;
		}else if(count == 2){
			tmpy += 30;
		}else if(count == 3){
			tmpx += 30;
		}
		if(member->m_gamestate.m_bTsumo){
			pPaiga->drawCurrentImage(dc,tmpx,tmpy);
		}
	}
				
	if(count == 0){
		tmpx = rect.right - 35;
		tmpy = rect.top + 15;
	}else if(count == 1){
		tmpx = rect.left + 5;
		tmpy = rect.top + 5;
	}else if(count == 2){
		tmpx = rect.left + 5;
		tmpy = rect.bottom - 35;
	}else if(count == 3){
		tmpx = rect.right - 35;
		tmpy = PLAYER_OFFSETY;
	}
	for(j=0;j<member->m_gamestate.m_aNakiList.GetSize();j++){
		if(member->m_gamestate.m_aNakiList[j].m_iCategory == MENTSU_ANKAN){
			for(k=0;k<4;k++){
				if((k==0 || k==3) && m_rule.m_iGlasshai == 0){
					pPaiga->setState(34);
				}else{
					pPaiga->setState((UINT)member->m_gamestate.m_aNakiList[j].m_aPaiList[0]);
				}
				
				if(m_rule.m_iGlasshai == 1 && member->m_gamestate.m_aNakiList[j].m_aPaiList[k].m_iId != 4){
					pPaiga->drawCurrentImage(dc,tmpx,tmpy,120);
				}else{
					pPaiga->drawCurrentImage(dc,tmpx,tmpy);
				}
				if(count == 0){
					tmpy += 26;
				}else if(count == 1){
					tmpx += 25;
				}else if(count == 2){
					tmpy -= 26;
				}else if(count == 3){
					tmpx -= 25;
				}
			}
		}else{
			ind = ieTable2[m_taku.getMemberIndex(&member->m_player)][member->m_gamestate.m_aNakiList[j].m_iAite];
			if(member->m_gamestate.m_aNakiList[j].m_iCategory == MENTSU_MINKAN && ind == 3){
				ind = 4;
			}
			
			ind--;
			
			num = 0;
			if(count == 0){
				offset = tmpy;
			}else if(count == 1){
				offset = tmpx;
			}else if(count == 2){
				offset = tmpy - 10;
			}else if(count == 3){
				offset = tmpx - 10;
			}
			
			/* 鳴き牌以外を描画 */
			for(k=0;k<member->m_gamestate.m_aNakiList[j].m_aPaiList.GetSize();k++){
				if(num==ind){
					if(count == 0){
						tmpy += 36;
					}else if(count == 1){
						tmpx += 35;
					}else if(count == 2){
						tmpy -= 36;
					}else if(count == 3){
						tmpx -= 35;
					}
					num++;
				}
				if(k!=(int)member->m_gamestate.m_aNakiList[j].getNakiPos()){
					pPaiga->setState((UINT)member->m_gamestate.m_aNakiList[j].m_aPaiList[k]);
					if(m_rule.m_iGlasshai == 1 && member->m_gamestate.m_aNakiList[j].m_aPaiList[k].m_iId != 4){
						pPaiga->drawCurrentImage(dc,tmpx,tmpy,120);
					}else{
						pPaiga->drawCurrentImage(dc,tmpx,tmpy);
					}
					if(count == 0){
						tmpy += 26;
					}else if(count == 1){
						tmpx += 25;
					}else if(count == 2){
						tmpy -= 26;
					}else if(count == 3){
						tmpx -= 25;
					}
					num++;
				}
			}
			
			/* 鳴き牌を描画(横) */
			pPaigaYoko->setState((UINT)member->m_gamestate.m_aNakiList[j].m_nakihai);
			if(count == 0){
				if(m_rule.m_iGlasshai == 1 && member->m_gamestate.m_aNakiList[j].m_nakihai.m_iId != 4){
					pPaigaYoko->drawCurrentImage(dc,tmpx + 8,offset + 26*ind,120);
				}else{
					pPaigaYoko->drawCurrentImage(dc,tmpx + 8,offset + 26*ind);
				}
			}else if(count == 1){
				if(m_rule.m_iGlasshai == 1 && member->m_gamestate.m_aNakiList[j].m_nakihai.m_iId != 4){
					pPaigaYoko->drawCurrentImage(dc,offset + 25*ind,tmpy,120);
				}else{
					pPaigaYoko->drawCurrentImage(dc,offset + 25*ind,tmpy);
				}
			}else if(count == 2){
				if(m_rule.m_iGlasshai == 1 && member->m_gamestate.m_aNakiList[j].m_nakihai.m_iId != 4){
					pPaigaYoko->drawCurrentImage(dc,tmpx,offset - 26*ind,120);
				}else{
					pPaigaYoko->drawCurrentImage(dc,tmpx,offset - 26*ind);
				}
			}else if(count == 3){
				if(m_rule.m_iGlasshai == 1 && member->m_gamestate.m_aNakiList[j].m_nakihai.m_iId != 4){
					pPaigaYoko->drawCurrentImage(dc,offset - 25*ind,tmpy+10,120);
				}else{
					pPaigaYoko->drawCurrentImage(dc,offset - 25*ind,tmpy+10);
				}
			}
			
			if(num < member->m_gamestate.m_aNakiList[j].m_aPaiList.GetSize()){
				if(count == 0){
					tmpy += 36;
				}else if(count == 1){
					tmpx += 35;
				}else if(count == 2){
					tmpy -= 36;
				}else if(count == 3){
					tmpx -= 35;
				}
			}
		}
	}
}
