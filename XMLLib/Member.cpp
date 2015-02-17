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

void OMMember::parseXML(OMDomNode pElem)
{
        OMDomNode pNode;
        OMDomNodeList pNodeList;
	int i;

	/* スカラーデータの格納 */

        pNode = OMGetElement(pElem,_T(TAG_MSTATE));

        OMToNum(pNode,m_iState);

        pNode = OMGetElement(pElem,_T(TAG_POINT));

        OMToNum(pNode,m_iPoint);

	/* 打牌の格納 */
        m_aDahai.clear();
        pNodeList = OMGetElementList(pElem,_T(TAG_DAHAI "/"  TAG_PAI));

        if(!OMIsEmpty(pNodeList)){
            for(i=0;i<OMListLength(pNodeList);i++){
                        OMPai pai;
                        pNode = OMListItem(pNodeList,i);
                        pai.parseXML(pNode);
                        m_aDahai.append(pai);
		}
	}

	/* 手牌の格納 */
        m_aTehai.clear();
        pNodeList = OMGetElementList(pElem,_T(TAG_TEHAI "/" TAG_PAI));

        if(!OMIsEmpty(pNodeList)){
            for(i=0;i<OMListLength(pNodeList);i++){
                        OMPai pai;
                        pNode = OMListItem(pNodeList,i);
                        pai.parseXML(pNode);
                        m_aTehai.append(pai);
		}
	}

	/* コマンドリストの格納 */
        m_aCommandList.clear();
        pNodeList = OMGetElementList(pElem,_T(TAG_COMMANDLIST "/" TAG_COMMAND));

        if(!OMIsEmpty(pNodeList)){
            for(i=0;i<OMListLength(pNodeList);i++){
                        OMCommand com;
                        pNode = OMListItem(pNodeList,i);
                        com.parseXML(pNode);
                        m_aCommandList.append(com);
		}
	}

	/* ツモ牌の格納 */
        pNode = OMGetElement(pElem,_T(TAG_TSUMOHAI));

        if(!OMIsNull(pNode)){
            m_gamestate.m_bTsumo = TRUE;
            pNode = OMGetElement(pElem,_T(TAG_TSUMOHAI "/" TAG_PAI));
            if(!OMIsNull(pNode)){
                m_tsumohai.parseXML(pNode);
            }
        }else{
            m_gamestate.m_bTsumo = FALSE;
        }


	/* プレーヤーの格納 */

        pNode = OMGetElement(pElem,_T(TAG_PLAYER));

        if(!OMIsNull(pNode)){
		m_player.parseXML(pNode);
	}

	/* ゲーム状態の格納 */
        pNode = OMGetElement(pElem,_T(TAG_GAMESTATE));

        if(!OMIsNull(pNode)){
		m_gamestate.parseXML(pNode);
	}

	/* あがりの格納 */
        m_aResultList.clear();
        pNodeList = OMGetElementList(pElem,_T(TAG_RESULTLIST "/" TAG_RESULT));

        if(!OMIsEmpty(pNodeList)){
            for(i=0;i<OMListLength(pNodeList);i++){
                        OMResult res;
                        pNode = OMListItem(pNodeList,i);
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
        m_aDahai.copy(value.m_aDahai);
        m_aResultList.clear();
        for(i=0;i<value.m_aResultList.size();i++){
                m_aResultList.append(value.m_aResultList[i]);
	}
        m_aTehai.copy(value.m_aTehai);
	m_gamestate = value.m_gamestate;
	m_iPoint = value.m_iPoint;
	m_iState = value.m_iState;
	m_player = value.m_player;

	return *this;
}

/* それぞれの家の距離を返す
  0 - 自身
  1 - 下家
  2 - 対面
  3 - 上家
  */
int OMMember::getChaDistance(int x, int y)
{
    return (y - x + 4) % 4;
}

int OMMember::getDahaiRealPos(OMPai &pai)
{
    int pos = 0;
    int i;
    bool matched = false;

    for(i=0;i<m_aDahai.size();i++){
        if(m_aDahai[i].match(pai)){
            matched = true;
            break;
        }else if(!m_aDahai[i].m_bNaki){
            pos++;
        }
    }

    if(matched){
        return pos;
    }else{
        return -1;
    }
}
