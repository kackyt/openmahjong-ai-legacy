// Pai.cpp: CPai クラスのインプリメンテーション
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
// 構築/消滅
//////////////////////////////////////////////////////////////////////

CPai::CPai()
{
	m_iCategory = -1;
	m_iNo = -1;
	m_iId = -1;
	m_bTsumogiri = FALSE;
	m_bRiichi = FALSE;
	m_bNaki = FALSE;
	m_bLast = FALSE;

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
	const TCHAR *table[] = {_T("一"),_T("二"),_T("三"),_T("四"),_T("五"),_T("六"),_T("七"),_T("八"),_T("九")};
	const TCHAR *table2[] = {_T("東"),_T("南"),_T("西"),_T("北"),_T("白"),_T("発"),_T("中")};
	text = _T("");

	switch(m_iCategory){
	case PAI_MANZU:
		text += table[m_iNo - 1];
		text += _T("萬");
		break;
	case PAI_SOUZU:
		text += table[m_iNo - 1];
		text += _T("索");
		break;
	case PAI_PINZU:
		text += table[m_iNo - 1];
		text += _T("筒");
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

BOOL CPai::isAka(CRule& rule)
{
	if(rule.m_iAka == 1){
		if(m_iNo == 5 && m_iId == 4 && m_iCategory != PAI_ZIHAI){
			return TRUE;
		}
	}else if(rule.m_iAka == 2){
		if(m_iNo == 5 && m_iId == 4 && m_iCategory != PAI_ZIHAI){
			return TRUE;
		}

		if(m_iCategory == PAI_PINZU && m_iNo == 5 && m_iId == 3){
			return TRUE;
		}
	}

	return FALSE;

}

int CPai::operator >=(CPai& pai)
{
	if((UINT)pai == PAI_NIL) return FALSE;
	if((UINT)*this == PAI_NIL) return TRUE;
	if(m_iCategory > pai.m_iCategory){
		return TRUE;
	}else if(m_iCategory == pai.m_iCategory){
		if(m_iNo > pai.m_iNo){
			return TRUE;
		}else if(m_iNo == pai.m_iNo){
			if(m_iId >= pai.m_iId){
				return TRUE;
			}
		}
	}

	return FALSE;
}
