// Command.cpp: CCommand クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "Command.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CCommand::CCommand()
{
	m_iId = 0;
	m_iType = 0;

}

CCommand::~CCommand()
{

}

void CCommand::parseXML(IXMLDOMNodePtr pElem)
{
	IXMLDOMNodePtr pNode;
	CString text;
	BSTR pStr;

	/* スカラーデータの格納 */

	pNode = pElem->selectSingleNode(_T(TAG_ID));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iId = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_TYPE));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iType = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_PAI));

	if(pNode != NULL){
		m_pai.parseXML(pNode);
	}

	pNode = pElem->selectSingleNode(_T(TAG_PLAYER));

	if(pNode != NULL){
		m_player.parseXML(pNode);
	}

	pNode = pElem->selectSingleNode(_T(TAG_NAKIMENTSU));

	if(pNode != NULL){
		m_mentsu.parseXML(pNode);
	}else{
		pNode= pElem->selectSingleNode(_T(TAG_MENTSU));

		if(pNode != NULL){
			m_mentsu.parseXML(pNode);
		}
	}

}

CCommand& CCommand::operator=(CCommand& value)
{
	m_iId = value.m_iId;
	m_iType = value.m_iType;
	m_mentsu = value.m_mentsu;
	m_pai = value.m_pai;
	m_player = value.m_player;

	return *this;
}

void CCommand::toXML(IXMLDOMDocumentPtr pDoc,IXMLDOMElementPtr pParent)
{
	IXMLDOMElementPtr pElemCom,pElemID;
	CString str;

	pElemCom = pDoc->createElement(_T(TAG_COMMAND));
	pParent->appendChild(pElemCom);
	pElemID = pDoc->createElement(_T(TAG_ID));
	pElemCom->appendChild(pElemID);

	str.Format(_T("%d"),m_iId);

	pElemID->appendChild(pDoc->createTextNode((LPCTSTR)str));

	m_player.toXML(pDoc,pElemCom);
}
