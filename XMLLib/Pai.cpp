/****************************************************************************************
 * Copyright (c) 2010, Takaya Kakizaki(kacky)
 * All rights reserved.

  ソースコード形式かバイナリ形式か、変更するかしないかを問わず、以下の条件を満たす場合に限り、再頒布および使用が許可されます。 

  ・ソースコードを再頒布する場合、上記の著作権表示、本条件一覧、および下記免責条項を含めること。 

  ・バイナリ形式で再頒布する場合、頒布物に付属のドキュメント等の資料に、上記の著作権表示、本条件一覧、および下記免責条項を含めること。 

  ・書面による特別の許可なしに、本ソフトウェアから派生した製品の宣伝または販売促進に、オープン麻雀の名前またはコントリビューターの名前を使用してはならない。


  本ソフトウェアは、著作権者およびコントリビューターによって「現状のまま」提供されており、明示黙示を問わず、
  商業的な使用可能性、および特定の目的に対する適合性に関する暗黙の保証も含め、またそれに限定されない、いかなる保証もありません。
  著作権者もコントリビューターも、事由のいかんを問わず、 損害発生の原因いかんを問わず、かつ責任の根拠が契約であるか厳格責任であるか
  （過失その他の）不法行為であるかを問わず、仮にそのような損害が発生する可能性を知らされていたとしても、本ソフトウェアの使用によって発生した
  （代替品または代用サービスの調達、使用の喪失、データの喪失、利益の喪失、業務の中断も含め、またそれに限定されない）
  直接損害、間接損害、偶発的な損害、特別損害、懲罰的損害、または結果損害について、一切責任を負わないものとします。 

****************************************************************************************/
// Pai.cpp: CPai クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Pai.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

OMPai::OMPai()
{
	m_iCategory = -1;
	m_iNo = -1;
	m_iId = -1;
	m_bTsumogiri = FALSE;
	m_bRiichi = FALSE;
	m_bNaki = FALSE;
	m_bLast = FALSE;

}

OMPai::~OMPai()
{

}

void OMPai::parseXML(QDomNode pElem)
{
        QDomNode pNode;

        pNode = OM_GETELEMENT(pElem,_T(TAG_CATEGORY));

        OM_TOLONG(pNode,m_iCategory);

        pNode = OM_GETELEMENT(pElem,_T(TAG_NO));

        OM_TOLONG(pNode,m_iNo);

        pNode = OM_GETELEMENT(pElem,_T(TAG_ID));

        OM_TOLONG(pNode,m_iId);

        pNode = OM_GETELEMENT(pElem,_T(TAG_TSUMOGIRI));

        OM_TOBOOL(pNode,m_bTsumogiri);

        pNode = OM_GETELEMENT(pElem,_T(TAG_RIICHI));

        OM_TOBOOL(pNode,m_bRiichi);

}

OMPai::operator UINT()
{
	if(m_iCategory < 0) return PAI_NOTINIT;
	else if(m_iCategory == 0 && m_iNo == 0) return PAI_NIL;
	else return (m_iCategory - 1) * 9 + (m_iNo - 1);
}

void OMPai::getName(QString& text)
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

void OMPai::getDora(OMPai& dora)
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

void OMPai::set(UINT id)
{
	m_iCategory = ((id & 0x3F)/ 9) + 1;
	m_iNo = ((id & 0x3F) % 9) + 1;
}

BOOL OMPai::match(OMPai& pai)
{
	if(m_iCategory == pai.m_iCategory && m_iNo == pai.m_iNo && m_iId == pai.m_iId){
		return TRUE;
	}else{
		return FALSE;
	}
}

BOOL OMPai::isAka(OMRule& rule)
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

int OMPai::operator >=(OMPai& pai)
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
