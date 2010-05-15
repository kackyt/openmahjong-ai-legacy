// GameState.h: CGameState クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMESTATE_H__1B5C03DF_3BEA_45C5_B046_EB254C489968__INCLUDED_)
#define AFX_GAMESTATE_H__1B5C03DF_3BEA_45C5_B046_EB254C489968__INCLUDED_

#include "NakiMentsu.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGameState  
{
public:
	BOOL m_bNaki;
	int m_iCount;
	CGameState& operator =(CGameState&);
	void parseXML(IXMLDOMNodePtr);
	CArray<CNakiMentsu,CNakiMentsu&> m_aNakiList;
	BOOL m_bRiichi;
	BOOL m_bTsumo;
	BOOL m_bIppatsu;
	BOOL m_bOya;
	int m_iZikaze;
	CGameState();
	virtual ~CGameState();

};

#endif // !defined(AFX_GAMESTATE_H__1B5C03DF_3BEA_45C5_B046_EB254C489968__INCLUDED_)
