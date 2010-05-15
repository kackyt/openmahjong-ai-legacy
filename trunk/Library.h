// Library.h: CLibrary クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIBRARY_H__823E5C0D_EA91_4E97_964F_6B612FCDB742__INCLUDED_)
#define AFX_LIBRARY_H__823E5C0D_EA91_4E97_964F_6B612FCDB742__INCLUDED_

#include "Member.h"	// ClassView によって追加されました。
#include "Pai.h"	// ClassView によって追加されました。
#include "Result.h"
#include "GameState.h"	// ClassView によって追加されました。
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLibrary  
{
public:
	CGameState m_gamestate;
	void setMember(int id,CMember& member,MJITehai* pTehai,int agarihai);
	CArray<CResult,CResult&> m_aResultList;
	CArray<CPai,CPai&> m_aTehai;
	CPai m_agarihai;
	int m_iId;
	CPlayer m_player;
	CLibrary();
	virtual ~CLibrary();

};

#endif // !defined(AFX_LIBRARY_H__823E5C0D_EA91_4E97_964F_6B612FCDB742__INCLUDED_)
