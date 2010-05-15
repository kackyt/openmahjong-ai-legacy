// Player.cpp: CPlayer クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "Player.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CPlayer::CPlayer()
{
	m_iId = 0;
	m_bIsComp = FALSE;
	m_pFunc = NULL;
	m_pInst = NULL;
	m_strName = _T("");

}

CPlayer::~CPlayer()
{

}

void CPlayer::parseXML(IXMLDOMNodePtr pElem)
{
	IXMLDOMNodePtr pNode;
	CString text;
	BSTR pStr;

	pNode = pElem->selectSingleNode(_T(TAG_ID));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iId = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_NAME));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		m_strName = pStr;
	}

}

void CPlayer::toXML(IXMLDOMDocumentPtr pDoc,IXMLDOMElementPtr pParent)
{
	IXMLDOMElementPtr pElemPlayer,pElemID,pElemName;
	CString str;

	pElemPlayer = pDoc->createElement(_T(TAG_PLAYER));

	pElemID = pDoc->createElement(_T(TAG_ID));
	str.Format(_T("%d"),m_iId);
	pElemID->appendChild(pDoc->createTextNode((LPCTSTR)str));
	pElemPlayer->appendChild(pElemID);

	pElemName = pDoc->createElement(_T(TAG_NAME));
	pElemName->appendChild(pDoc->createTextNode((LPCTSTR)m_strName));
	pElemPlayer->appendChild(pElemName);

	pParent->appendChild(pElemPlayer);

}
