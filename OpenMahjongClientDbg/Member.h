// Member.h: CMember クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMBER_H__AEABC3C8_3C44_48CF_92D2_AC795B8A7D0F__INCLUDED_)
#define AFX_MEMBER_H__AEABC3C8_3C44_48CF_92D2_AC795B8A7D0F__INCLUDED_

#include "Player.h"	// ClassView によって追加されました。
#include "Pai.h"
#include "Command.h"
#include "GameState.h"	// ClassView によって追加されました。
#include "Result.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMember  
{
public:
	CPai m_tsumohai;
	CMember& operator = (CMember&);
	BOOL isExecutableCommand(CCommand&);
	CGameState m_gamestate;
	void parseXML(IXMLDOMNodePtr);
	CArray<CResult,CResult&> m_aResultList;
	CArray<CCommand,CCommand&> m_aCommandList;
	CArray<CPai,CPai&> m_aTehai;
	CArray<CPai,CPai&> m_aDahai;
	int m_iState;
	int m_iPoint;
	CPlayer m_player;
	CMember();
	virtual ~CMember();

};

#endif // !defined(AFX_MEMBER_H__AEABC3C8_3C44_48CF_92D2_AC795B8A7D0F__INCLUDED_)
