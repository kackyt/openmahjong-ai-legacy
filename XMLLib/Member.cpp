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
// Member.cpp: CMember クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Member.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

OMMember::OMMember()
{

}

OMMember::~OMMember()
{

}

void OMMember::parseXML(QDomNode pElem)
{
        QDomNode pNode;
        QDomNodeList pNodeList;
	int i;

	/* スカラーデータの格納 */

        pNode = OM_GETELEMENT(pElem,_T(TAG_MSTATE));

        OM_TOLONG(pNode,m_iState);

        pNode = OM_GETELEMENT(pElem,_T(TAG_POINT));

        OM_TOLONG(pNode,m_iPoint);

	/* 打牌の格納 */
        m_aDahai.clear();
        pNodeList = OM_GETELEMENTLIST(pElem,_T(TAG_DAHAI "/"  TAG_PAI));

        if(!OM_ISEMPTY(pNodeList)){
            for(i=0;i<OM_LISTLENGTH(pNodeList);i++){
                        OMPai pai;
                        pNode = OM_LISTITEM(pNodeList,i);
                        pai.parseXML(pNode);
                        m_aDahai.append(pai);
		}
	}

	/* 手牌の格納 */
        m_aTehai.clear();
        pNodeList = OM_GETELEMENTLIST(pElem,_T(TAG_TEHAI "/" TAG_PAI));

        if(!OM_ISEMPTY(pNodeList)){
            for(i=0;i<OM_LISTLENGTH(pNodeList);i++){
                        OMPai pai;
                        pNode = OM_LISTITEM(pNodeList,i);
                        pai.parseXML(pNode);
                        m_aTehai.append(pai);
		}
	}

	/* コマンドリストの格納 */
        m_aCommandList.clear();
        pNodeList = OM_GETELEMENTLIST(pElem,_T(TAG_COMMANDLIST "/" TAG_COMMAND));

        if(!OM_ISEMPTY(pNodeList)){
            for(i=0;i<OM_LISTLENGTH(pNodeList);i++){
                        OMCommand com;
                        pNode = OM_LISTITEM(pNodeList,i);
                        com.parseXML(pNode);
                        m_aCommandList.append(com);
		}
	}

	/* ツモ牌の格納 */
        pNode = OM_GETELEMENT(pElem,_T(TAG_TSUMOHAI));

        if(!OM_ISNULL(pNode)){
		m_gamestate.m_bTsumo = TRUE;
                pNode = OM_GETELEMENT(pElem,_T(TAG_TSUMOHAI "/" TAG_PAI));
                if(!OM_ISNULL(pNode)){
			m_tsumohai.parseXML(pNode);
		}
	}else{
		m_gamestate.m_bTsumo = FALSE;
	}


	/* プレーヤーの格納 */

        pNode = OM_GETELEMENT(pElem,_T(TAG_PLAYER));

        if(!OM_ISNULL(pNode)){
		m_player.parseXML(pNode);
	}

	/* ゲーム状態の格納 */
        pNode = OM_GETELEMENT(pElem,_T(TAG_GAMESTATE));

        if(!OM_ISNULL(pNode)){
		m_gamestate.parseXML(pNode);
	}

	/* あがりの格納 */
        m_aResultList.clear();
        pNodeList = OM_GETELEMENTLIST(pElem,_T(TAG_RESULTLIST "/" TAG_RESULT));

        if(!OM_ISEMPTY(pNodeList)){
            for(i=0;i<OM_LISTLENGTH(pNodeList);i++){
                        OMResult res;
                        pNode = OM_LISTITEM(pNodeList,i);
                        res.parseXML(pNode);
                        m_aResultList.append(res);
		}
	}

}

BOOL OMMember::isExecutableCommand(OMCommand& command)
{
	int i;

        for(i=0;i<m_aCommandList.size();i++){
		if(m_aCommandList[i].m_iId == command.m_iId){
			return TRUE;
		}
	}

	return FALSE;
}

OMMember& OMMember::operator =(OMMember& value)
{
	int i;
        m_aCommandList.clear();
        for(i=0;i<value.m_aCommandList.size();i++){
                m_aCommandList.append(value.m_aCommandList[i]);
	}
        OM_COPYARRAY(m_aDahai,value.m_aDahai);
        m_aResultList.clear();
        for(i=0;i<value.m_aResultList.size();i++){
                m_aResultList.append(value.m_aResultList[i]);
	}
        OM_COPYARRAY(m_aTehai,value.m_aTehai);
	m_gamestate = value.m_gamestate;
	m_iPoint = value.m_iPoint;
	m_iState = value.m_iState;
	m_player = value.m_player;

	return *this;
}
