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
// Result.cpp: CResult クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Result.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

OMResult::OMResult()
{
	m_bActive = FALSE;
}

OMResult::~OMResult()
{

}

void OMResult::parseXML(QDomNode pElem)
{
        QDomNode pNode;
        QDomNodeList pNodeList;
        QString text;
	BSTR pStr;
	int i;

	m_bActive = TRUE;

	/* スカラーデータの格納 */

        pNode = OM_GETELEMENT(pElem,_T(TAG_DORAKAZU));

        OM_TOLONG(pNode,m_iDorakazu);

        pNode = OM_GETELEMENT(pElem,_T(TAG_HAN));

        OM_TOLONG(pNode,m_iHan);

        pNode = OM_GETELEMENT(pElem,_T(TAG_FU));

        OM_TOLONG(pNode,m_iFu);

        pNode = OM_GETELEMENT(pElem,_T(TAG_MACHI));

        OM_TOLONG(pNode,m_iMachi);

        pNode = OM_GETELEMENT(pElem,_T(TAG_MANGAN));

        OM_TOLONG(pNode,m_iMangan);

        pNode = OM_GETELEMENT(pElem,_T(TAG_SCORE));

        OM_TOLONG(pNode,m_iScore);

        pNode = OM_GETELEMENT(pElem,_T(TAG_OYASCORE));

        OM_TOLONG(pNode,m_iOyaScore);

        pNode = OM_GETELEMENT(pElem,_T(TAG_KOSCORE));

        OM_TOLONG(pNode,m_iKoScore);

        pNode = OM_GETELEMENT(pElem,_T(TAG_TSUMO));

        OM_TOBOOL(pNode,m_bTsumo);

        pNode = OM_GETELEMENT(pElem,_T(TAG_MACHIHAI "/" TAG_PAI));

        if(!OM_ISNULL(pNode)){
		m_machihai.parseXML(pNode);
	}

	/* 役名リストを格納 */
        m_aYaku.clear();
        pNodeList = OM_GETELEMENTLIST(pElem,_T(TAG_YAKU));

        if(!OM_ISEMPTY(pNodeList)){
            for(i=0;i<OM_LISTLENGTH(pNodeList);i++){
                pNode = OM_LISTITEM(pNodeList,i);
                OM_GETTEXT(pNode,pStr);
                text = QString(pStr);
                m_aYaku.append(text);
            }
	}


}

#ifdef _MSC_VER
OMResult& OMResult::operator=(OMResult& value)
#else
OMResult& OMResult::operator=(const OMResult& value)
#endif
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
        OM_COPYARRAY(m_aYaku,((OMResult&)value).m_aYaku);

	return *this;
}
