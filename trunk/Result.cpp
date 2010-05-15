// Result.cpp: CResult クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "Result.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CResult::CResult()
{
	m_bActive = FALSE;
}

CResult::~CResult()
{

}

void CResult::parseXML(IXMLDOMNodePtr pElem)
{
	IXMLDOMNodePtr pNode;
	IXMLDOMNodeListPtr pNodeList;
	CString text;
	BSTR pStr;
	int i;

	m_bActive = TRUE;

	/* スカラーデータの格納 */

	pNode = pElem->selectSingleNode(_T(TAG_DORAKAZU));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iDorakazu = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_HAN));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iHan = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_FU));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iFu = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_MACHI));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iMachi = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_MANGAN));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iMangan = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_SCORE));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iScore = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_OYASCORE));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iOyaScore = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_KOSCORE));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iKoScore = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_TSUMO));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_bTsumo = text == _T("true") ? TRUE : FALSE;
	}

	pNode = pElem->selectSingleNode(_T(TAG_MACHIHAI "/" TAG_PAI));

	if(pNode != NULL){
		m_machihai.parseXML(pNode);
	}

	/* 役名リストを格納 */
	m_aYaku.RemoveAll();
	pNodeList = pElem->selectNodes(_T(TAG_YAKU));

	if(pNodeList != NULL){
		for(i=0;i<pNodeList->Getlength();i++){
			pNode = pNodeList->Getitem(i);
			pNode->get_text(&pStr);
			text = pStr;
			m_aYaku.Add(text);
		}
	}


}

CResult& CResult::operator=(CResult& value)
{
	m_iDorakazu = value.m_iDorakazu;
	m_iFu = value.m_iFu;
	m_bActive = value.m_bActive;
	m_iHan = value.m_iHan;
	m_iMachi = value.m_iMachi;
	m_iMangan = value.m_iMangan;
	m_iScore = value.m_iScore;
	m_iOyaScore = value.m_iOyaScore;
	m_iKoScore = value.m_iKoScore;
	m_bTsumo = value.m_bTsumo;
	m_machihai = value.m_machihai;
	m_aYaku.Copy(value.m_aYaku);

	return *this;
}
