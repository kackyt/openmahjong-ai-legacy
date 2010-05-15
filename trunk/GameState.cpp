// GameState.cpp: CGameState クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "GameState.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CGameState::CGameState()
{
	m_iZikaze = 1;
	m_iCount = 1;
	m_bTsumo = FALSE;
	m_bRiichi = FALSE;
	m_bOya = FALSE;
	m_bIppatsu = FALSE;
	m_bNaki = FALSE;
}

CGameState::~CGameState()
{

}

void CGameState::parseXML(IXMLDOMNodePtr pElem)
{
	IXMLDOMNodePtr pNode;
	IXMLDOMNodeListPtr pNodeList;
	CString text;
	BSTR pStr;
	int i;

	/* スカラーデータの格納 */

	pNode = pElem->selectSingleNode(_T(TAG_ZIKAZE));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iZikaze = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_COUNT));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iCount = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_OYA));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_bOya = text == _T("true") ? TRUE : FALSE;
	}

	pNode = pElem->selectSingleNode(_T(TAG_NAKI));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_bNaki = text == _T("true") ? TRUE : FALSE;
	}

	pNode = pElem->selectSingleNode(_T(TAG_RIICHI));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_bRiichi = text == _T("true") ? TRUE : FALSE;
	}

	pNode = pElem->selectSingleNode(_T(TAG_IPPATSU));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_bIppatsu = text == _T("true") ? TRUE : FALSE;
	}

	pNode = pElem->selectSingleNode(_T(TAG_TSUMO));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_bTsumo = text == _T("true") ? TRUE : FALSE;
	}

	/* 鳴き面子の格納 */
	m_aNakiList.RemoveAll();
	pNodeList = pElem->selectNodes(_T(TAG_NAKILIST "/" TAG_NAKIMENTSU));

	if(pNodeList != NULL){
		for(i=0;i<pNodeList->Getlength();i++){
			CNakiMentsu mentsu;
			pNode = pNodeList->Getitem(i);
			mentsu.parseXML(pNode);
			m_aNakiList.Add(mentsu);
		}
	}

}

CGameState& CGameState::operator =(CGameState& value)
{
	int i;
	m_aNakiList.RemoveAll();
	for(i=0;i<value.m_aNakiList.GetSize();i++){
		m_aNakiList.Add(value.m_aNakiList[i]);
	}
	m_bIppatsu = value.m_bIppatsu;
	m_bOya = value.m_bOya;
	m_bRiichi = value.m_bRiichi;
	m_bTsumo = value.m_bTsumo;
	m_iZikaze = value.m_iZikaze;

	return *this;
}
