// TakuEvent.cpp: CTakuEvent クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "TakuEvent.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CTakuEvent::CTakuEvent()
{
	m_bActive = FALSE;
	m_iSeq = rand();

}

CTakuEvent::~CTakuEvent()
{

}

void CTakuEvent::parseXML(IXMLDOMNodePtr pElem)
{
	IXMLDOMNodePtr pNode;
	CString text;
	BSTR pStr;

	m_bActive = TRUE;

	/* スカラーデータの格納 */

	pNode = pElem->selectSingleNode(_T(TAG_COMMAND));

	if(pNode != NULL){
		m_command.parseXML(pNode);
	}

	pNode = pElem->selectSingleNode(_T(TAG_RESULT));

	if(pNode != NULL){
		m_result.parseXML(pNode);
	}else{
		m_result.m_bActive = FALSE;
	}

	pNode = pElem->selectSingleNode(_T(TAG_SEQ));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iSeq = _tcstol((const TCHAR*)text,NULL,0);
	}
}
