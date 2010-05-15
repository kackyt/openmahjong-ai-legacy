// Library.cpp: CLibrary クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "Library.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CLibrary::CLibrary()
{

}

CLibrary::~CLibrary()
{

}

void CLibrary::setMember(int id,CMember& member,MJITehai* pTehai,int agarihai)
{
	UINT i,j;
	m_iId = id;
	m_player = member.m_player;
	m_gamestate = member.m_gamestate;
	m_gamestate.m_aNakiList.RemoveAll();

	if(pTehai != NULL){
		for(i=0;i<pTehai->tehai_max;i++){
			CPai pai;
			pai.set(pTehai->tehai[i]);
			m_aTehai.Add(pai);
		}

		for(i=0;i<pTehai->minshun_max;i++){
			CNakiMentsu nakimentsu;
			nakimentsu.m_iAite = 3;
			nakimentsu.m_iCategory = MENTSU_SYUNTSU;
			for(j=0;j<3;j++){
				CPai pai;
				pai.set(pTehai->minshun[i] + j);
				nakimentsu.m_aPaiList.Add(pai);
			}

			m_gamestate.m_aNakiList.Add(nakimentsu);
		}

		for(i=0;i<pTehai->minkou_max;i++){
			CNakiMentsu nakimentsu;
			nakimentsu.m_iAite = 3;
			nakimentsu.m_iCategory = MENTSU_KOUTSU;
			for(j=0;j<3;j++){
				CPai pai;
				pai.set(pTehai->minkou[i]);
				nakimentsu.m_aPaiList.Add(pai);
			}
			m_gamestate.m_aNakiList.Add(nakimentsu);
		}

		for(i=0;i<pTehai->minkan_max;i++){
			CNakiMentsu nakimentsu;
			nakimentsu.m_iAite = 3;
			nakimentsu.m_iCategory = MENTSU_MINKAN;
			for(j=0;j<4;j++){
				CPai pai;
				pai.set(pTehai->minkan[i]);
				nakimentsu.m_aPaiList.Add(pai);
			}
			m_gamestate.m_aNakiList.Add(nakimentsu);
		}

		for(i=0;i<pTehai->ankan_max;i++){
			CNakiMentsu nakimentsu;
			nakimentsu.m_iAite = 3;
			nakimentsu.m_iCategory = MENTSU_ANKAN;
			for(j=0;j<4;j++){
				CPai pai;
				pai.set(pTehai->ankan[i]);
				nakimentsu.m_aPaiList.Add(pai);
			}
			m_gamestate.m_aNakiList.Add(nakimentsu);
		}
	}

	if(agarihai >= 0){
		m_agarihai.set((UINT)agarihai);
	}

}

