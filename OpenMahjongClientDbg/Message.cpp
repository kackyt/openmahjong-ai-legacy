// Message.cpp: CMessage クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "Message.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CMessage::CMessage()
{

}

CMessage::~CMessage()
{

}

void CMessage::parseXML(IXMLDOMNodePtr pElem)
{
	IXMLDOMNodePtr pNode;
	IXMLDOMNodeListPtr pNodeList;
	CString text;
	BSTR pStr;
	int i;

	pNode = pElem->selectSingleNode(_T(TAG_TEXT));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		m_strText = pStr;
	}

	pNode = pElem->selectSingleNode(_T(TAG_FROM "/" TAG_PLAYER));

	if(pNode != NULL){
		m_playerFrom.parseXML(pNode);
	}

	pNodeList = pElem->selectNodes(_T(TAG_TO "/" TAG_PLAYER));

	if(pNodeList != NULL){
		for(i=0;i<pNodeList->Getlength();i++){
			CPlayer player;
			pNode = pNodeList->Getitem(i);
			player.parseXML(pNode);
			m_aPlayerTo.Add(player);
		}
	}
}

void CMessage::toXML(IXMLDOMDocumentPtr pDoc,IXMLDOMElementPtr pParent)
{
	IXMLDOMElementPtr pElemMes,pElemTo,pElemFrom,pElemText;
	int i;

	pElemMes = pDoc->createElement(_T(TAG_MESSAGE));

	pElemTo = pDoc->createElement(_T(TAG_TO));

	for(i=0;i<m_aPlayerTo.GetSize();i++){
		m_aPlayerTo[i].toXML(pDoc,pElemTo);
	}

	pElemMes->appendChild(pElemTo);


	pElemFrom = pDoc->createElement(_T(TAG_FROM));
	m_playerFrom.toXML(pDoc,pElemFrom);
	pElemMes->appendChild(pElemFrom);

	pElemText = pDoc->createElement(_T(TAG_TEXT));
	pElemText->appendChild(pDoc->createTextNode((LPCTSTR)m_strText));
	pElemMes->appendChild(pElemText);

	pParent->appendChild(pElemMes);

}

CMessage& CMessage::operator =(CMessage& value)
{
	m_aPlayerTo.Copy(value.m_aPlayerTo);

	m_playerFrom = value.m_playerFrom;

	m_strText = value.m_strText;

	return *this;
}


