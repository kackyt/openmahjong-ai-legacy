// Rule.cpp: CRule クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "Rule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable:4786)

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CRule::CRule()
{
	m_bActive = FALSE;
	m_iGlasshai = 0;
	m_iKuitan = 1;
	m_iAka = 0;
	m_iAotenjo = 0;
}

CRule::~CRule()
{

}

void CRule::parseXML(IXMLDOMNodePtr pElem)
{
	IXMLDOMNodePtr pNode;
	CString text;
	BSTR pStr;

	m_bActive = TRUE;

	pNode = pElem->selectSingleNode(_T(TAG_GLASSHAI));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iGlasshai = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_KUITAN));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iKuitan = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_AKA));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iAka = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_AOTENJO));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iAotenjo = _tcstol((const TCHAR*)text,NULL,0);
	}
}

void CRule::toXML(IXMLDOMDocumentPtr pDoc,IXMLDOMElementPtr pParent)
{
	IXMLDOMElementPtr pElemKey,pElemRule;
	CString str;

	pElemRule = pDoc->createElement(_T(TAG_RULE));

	pElemKey = pDoc->createElement(_T(TAG_GLASSHAI));
	str.Format(_T("%d"),m_iGlasshai);
	pElemKey->appendChild(pDoc->createTextNode((LPCTSTR)str));
	pElemRule->appendChild(pElemKey);

	pElemKey = pDoc->createElement(_T(TAG_KUITAN));
	str.Format(_T("%d"),m_iKuitan);
	pElemKey->appendChild(pDoc->createTextNode((LPCTSTR)str));
	pElemRule->appendChild(pElemKey);

	pElemKey = pDoc->createElement(_T(TAG_AKA));
	str.Format(_T("%d"),m_iAka);
	pElemKey->appendChild(pDoc->createTextNode((LPCTSTR)str));
	pElemRule->appendChild(pElemKey);

	pElemKey = pDoc->createElement(_T(TAG_AOTENJO));
	str.Format(_T("%d"),m_iAotenjo);
	pElemKey->appendChild(pDoc->createTextNode((LPCTSTR)str));
	pElemRule->appendChild(pElemKey);

	pParent->appendChild(pElemRule);

}
