// Pai.cpp: CPai NXΜCve[V
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OpenMahjongClientDbg.h"
#include "Pai.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// \z/ΑΕ
//////////////////////////////////////////////////////////////////////

CPai::CPai()
{
	m_iCategory = -1;
	m_iNo = 0;
	m_iId = 0;
	m_bTsumogiri = FALSE;
	m_bRiichi = FALSE;

}

CPai::~CPai()
{

}

void CPai::parseXML(IXMLDOMNodePtr pElem)
{
	IXMLDOMNodePtr pNode;
	CString text;
	BSTR pStr;

	pNode = pElem->selectSingleNode(_T(TAG_CATEGORY));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iCategory = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_NO));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iNo = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_ID));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		m_iId = _tcstol((const TCHAR*)text,NULL,0);
	}

	pNode = pElem->selectSingleNode(_T(TAG_TSUMOGIRI));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		if(text == _T("true")){
			m_bTsumogiri = TRUE;
		}else{
			m_bTsumogiri = FALSE;
		}
	}

	pNode = pElem->selectSingleNode(_T(TAG_RIICHI));

	if(pNode != NULL){
		pNode->get_text(&pStr);
		text = pStr;
		if(text == _T("true")){
			m_bRiichi = TRUE;
		}else{
			m_bRiichi = FALSE;
		}
	}

}

CPai::operator UINT()
{
	if(m_iCategory < 0) return PAI_NOTINIT;
	else if(m_iCategory == 0 && m_iNo == 0) return PAI_NIL;
	else return (m_iCategory - 1) * 9 + (m_iNo - 1);
}

void CPai::getName(CString& text)
{
	const TCHAR *table[] = {_T("κ"),_T("ρ"),_T("O"),_T("l"),_T("ά"),_T("Z"),_T("΅"),_T("ͺ"),_T("γ")};
	const TCHAR *table2[] = {_T(""),_T("μ"),_T("Ό"),_T("k"),_T(""),_T("­"),_T("")};
	text = _T("");

	switch(m_iCategory){
	case PAI_MANZU:
		text += table[m_iNo - 1];
		text += _T("δέ");
		break;
	case PAI_SOUZU:
		text += table[m_iNo - 1];
		text += _T("υ");
		break;
	case PAI_PINZU:
		text += table[m_iNo - 1];
		text += _T("");
		break;
	case PAI_ZIHAI:
		text += table2[m_iNo - 1];
		break;
	default:
		break;
	}

	if(m_bTsumogiri){
		text += _T("*");
	}
	if(m_bRiichi){
		text += _T("#");
	}

}

void CPai::getDora(CPai& dora)
{
	dora.m_iCategory = m_iCategory;

	if(m_iCategory == PAI_ZIHAI){
		if(m_iNo == 4){
			dora.m_iNo = 1;
		}else if(m_iNo == 7){
			dora.m_iNo = 5;
		}else{
			dora.m_iNo = m_iNo + 1;
		}
	}else{
		if(m_iNo == 9){
			dora.m_iNo = 1;
		}else{
			dora.m_iNo = m_iNo + 1;
		}
	}

}

void CPai::set(UINT id)
{
	m_iCategory = ((id & 0x3F)/ 9) + 1;
	m_iNo = ((id & 0x3F) % 9) + 1;
}

BOOL CPai::match(CPai& pai)
{
	if(m_iCategory == pai.m_iCategory && m_iNo == pai.m_iNo && m_iId == pai.m_iId){
		return TRUE;
	}else{
		return FALSE;
	}
}
