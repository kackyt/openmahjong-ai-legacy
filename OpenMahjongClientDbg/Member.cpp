// Member.cpp: CMember ƒNƒ‰ƒX‚ÌƒCƒ“ƒvƒŠƒƒ“ƒe[ƒVƒ‡ƒ“
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "Member.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// \’z/Á–Å
//////////////////////////////////////////////////////////////////////

CMember::CMember()
{

}

CMember::~CMember()
{

}

void CMember::parseXML(IXMLDOMNodePtr pElem)
{
	IXMLDOMNodePtr pNode;
	IXMLDOMNodeListPtr pNodeList;
	CString text;
	BSTR pStr;
	int i;

	/* ƒXƒJƒ‰[ƒf[ƒ^‚ÌŠi”[ */

	pNode = pElem->selectSingleNode(_T(TAG_MSTATE));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iState = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_POINT));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iPoint = _tcstol((const TCHAR*)text,NULL,0);
	}

	/* ‘Å”v‚ÌŠi”[ */
	m_aDahai.RemoveAll();
	pNodeList = pElem->selectNodes(_T(TAG_DAHAI "/ "  TAG_PAI));

	if(pNodeList != NULL){
		for(i=0;i<pNodeList->Getlength();i++){
			CPai pai;
			pNode = pNodeList->Getitem(i);
			pai.parseXML(pNode);
			m_aDahai.Add(pai);
		}
	}

	/* Žè”v‚ÌŠi”[ */
	m_aTehai.RemoveAll();
	pNodeList = pElem->selectNodes(_T(TAG_TEHAI "/" TAG_PAI));

	if(pNodeList != NULL){
		for(i=0;i<pNodeList->Getlength();i++){
			CPai pai;
			pNode = pNodeList->Getitem(i);
			pai.parseXML(pNode);
			m_aTehai.Add(pai);
		}
	}

	/* ƒRƒ}ƒ“ƒhƒŠƒXƒg‚ÌŠi”[ */
	m_aCommandList.RemoveAll();
	pNodeList = pElem->selectNodes(_T(TAG_COMMANDLIST "/" TAG_COMMAND));

	if(pNodeList != NULL){
		for(i=0;i<pNodeList->Getlength();i++){
			CCommand com;
			pNode = pNodeList->Getitem(i);
			com.parseXML(pNode);
			m_aCommandList.Add(com);
		}
	}

	/* ƒcƒ‚”v‚ÌŠi”[ */
	pNode = pElem->selectSingleNode(_T(TAG_TSUMOHAI));

	if(pNode != NULL){
		m_gamestate.m_bTsumo = TRUE;
		pNode = pElem->selectSingleNode(_T(TAG_TSUMOHAI "/" TAG_PAI));
		if(pNode != NULL){
			m_tsumohai.parseXML(pNode);
		}
	}else{
		m_gamestate.m_bTsumo = FALSE;
	}


	/* ƒvƒŒ[ƒ„[‚ÌŠi”[ */

	pNode = pElem->selectSingleNode(_T(TAG_PLAYER));

	if(pNode != NULL){
		m_player.parseXML(pNode);
	}

	/* ƒQ[ƒ€ó‘Ô‚ÌŠi”[ */
	pNode = pElem->selectSingleNode(_T(TAG_GAMESTATE));

	if(pNode != NULL){
		m_gamestate.parseXML(pNode);
	}

	/* ‚ ‚ª‚è‚ÌŠi”[ */
	m_aResultList.RemoveAll();
	pNodeList = pElem->selectNodes(_T(TAG_RESULTLIST "/" TAG_RESULT));

	if(pNodeList != NULL){
		for(i=0;i<pNodeList->Getlength();i++){
			CResult res;
			pNode = pNodeList->Getitem(i);
			res.parseXML(pNode);
			m_aResultList.Add(res);
		}
	}

}

BOOL CMember::isExecutableCommand(CCommand& command)
{
	int i;

	for(i=0;i<m_aCommandList.GetSize();i++){
		if(m_aCommandList[i].m_iId == command.m_iId){
			return TRUE;
		}
	}

	return FALSE;
}

CMember& CMember::operator =(CMember& value)
{
	int i;
	m_aCommandList.RemoveAll();
	for(i=0;i<value.m_aCommandList.GetSize();i++){
		m_aCommandList.Add(value.m_aCommandList[i]);
	}
	m_aDahai.Copy(value.m_aDahai);
	m_aResultList.RemoveAll();
	for(i=0;i<value.m_aResultList.GetSize();i++){
		m_aResultList.Add(value.m_aResultList[i]);
	}
	m_aTehai.Copy(value.m_aTehai);
	m_gamestate = value.m_gamestate;
	m_iPoint = value.m_iPoint;
	m_iState = value.m_iState;
	m_player = value.m_player;

	return *this;
}
