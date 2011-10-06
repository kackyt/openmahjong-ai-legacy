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
// Player.cpp: CPlayer クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Player.h"
#include "Taku.h"
#include "Command.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable:4786)

const UINT OMPlayer::ieTable[][4] = {
    { 0, 1, 2, 3},
    { 3, 0, 1, 2},
    { 2, 3, 0, 1},
    { 1, 2, 3, 0},
};


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

OMPlayer::OMPlayer()
{
    m_iId = 0;
    m_bIsComp = FALSE;
    m_pFunc = NULL;
    m_pInst = NULL;
    m_strName = _T("");
    m_iPrivateId = 0;

}

OMPlayer::~OMPlayer()
{

}

void OMPlayer::parseXML(OMDomNode pElem)
{
    OMDomNode pNode;
    OMString pStr;

    pNode = OMGetElement(pElem,_T(TAG_ID));

    OMToNum(pNode,m_iId);

    pNode = OMGetElement(pElem,_T(TAG_NAME));

    if(!OMIsNull(pNode)){
        OMGetText(pNode,pStr);
        m_strName = OMString(pStr);
    }

    pNode = OMGetElement(pElem,_T(TAG_PRIVATEID));
    OMToNum(pNode,m_iPrivateId);
}

void OMPlayer::toXML(OMDomDocument pDoc,OMDomElement pParent)
{
    OMDomElement pElemPlayer,pElemID,pElemName;
    OMString str;
    OMDomNode pTxtNode;

    pElemPlayer = OMCreateElement(pDoc,_T(TAG_PLAYER));

    pElemID = OMCreateElement(pDoc,_T(TAG_ID));
    str.Format(_T("%d"),m_iId);
    pTxtNode = OMCreateTextNode(pDoc,str);
    OMAppendChild(pElemID,pTxtNode);
    OMAppendChild(pElemPlayer,pElemID);

    pElemName = OMCreateElement(pDoc,_T(TAG_NAME));
    pTxtNode = OMCreateTextNode(pDoc,m_strName);
    OMAppendChild(pElemName,pTxtNode);
    OMAppendChild(pElemPlayer,pElemName);

    pElemID = OMCreateElement(pDoc,_T(TAG_PRIVATEID));
    str.Format(_T("%d"),m_iPrivateId);
    pTxtNode = OMCreateTextNode(pDoc,str);
    OMAppendChild(pElemID,pTxtNode);
    OMAppendChild(pElemPlayer,pElemID);

    OMAppendChild(pParent,pElemPlayer);

}

/* コンピュータのアクションを計算 */
UINT OMPlayer::onPlayerAction(OMTaku *pTaku, OMPlayerActionParam &param)
{
    UINT ret = 0;
    int i;
    int ind = pTaku->getMemberIndex(this);
    bool bAction = false;
    int kaze3;
    LONG pointdiff[4] = { 0 };
    int ieparam;

    switch(param.m_iType){
    case TYPE_TII:
    case TYPE_PON:
    case TYPE_RON:
    case TYPE_DAIMINKAN:
        /* 相手のあるアクション */
        kaze3 = pTaku->m_members[pTaku->m_event.m_command.m_mentsu.m_iAite].m_gamestate.m_iZikaze - 1;
        ieparam = (ieTable[param.m_iKaze1][kaze3] << 16) | ieTable[param.m_iKaze1][param.m_iKaze2];
        break;
    default:
        /* 相手のないアクション */
        ieparam = ieTable[param.m_iKaze1][param.m_iKaze2];
        break;
    }


    switch(param.m_iType){
    case TYPE_KOUHAI:
        m_pFunc(m_pInst,MJPI_ENDKYOKU,MJEK_RYUKYOKU,(UINT)pointdiff);
        break;
    case TYPE_START:
        m_pFunc(m_pInst,MJPI_STARTKYOKU,pTaku->m_iKyokuCount,pTaku->m_members[ind].m_gamestate.m_iZikaze - 1);
        break;
    case TYPE_DAHAI:
        bAction = true;
        ret = m_pFunc(m_pInst,MJPI_ONACTION,ieparam,MJPIR_SUTEHAI | (UINT)pTaku->m_event.m_command.m_pai);
        break;
    case TYPE_TII:
        switch(pTaku->m_event.m_command.m_mentsu.getNakiPos()){
        case 0:
            ret = m_pFunc(m_pInst,MJPI_ONACTION,ieparam,MJPIR_CHII1 | (UINT)pTaku->m_event.m_command.m_pai);
            break;
        case 1:
            ret = m_pFunc(m_pInst,MJPI_ONACTION,ieparam,MJPIR_CHII2 | (UINT)pTaku->m_event.m_command.m_pai);
            break;
        case 2:
            ret = m_pFunc(m_pInst,MJPI_ONACTION,ieparam,MJPIR_CHII3 | (UINT)pTaku->m_event.m_command.m_pai);
        default:
            break;
        }

        break;
    case TYPE_PON:
        ret = m_pFunc(m_pInst,MJPI_ONACTION,ieparam,MJPIR_PON | (UINT)pTaku->m_event.m_command.m_pai);
        break;
    case TYPE_RON:
        ret = m_pFunc(m_pInst,MJPI_ONACTION,ieparam,MJPIR_RON | (UINT)pTaku->m_event.m_command.m_pai);
        pointdiff[ieTable[param.m_iKaze1][kaze3]] = -(pTaku->m_event.m_result.m_iScore + pTaku->m_iTsumibou * 300);
        pointdiff[ieTable[param.m_iKaze1][param.m_iKaze2]] = (pTaku->m_event.m_result.m_iScore + pTaku->m_iTsumibou * 300 + pTaku->m_iRiichibou * 1000);
        m_pFunc(m_pInst,MJPI_ENDKYOKU,MJEK_AGARI,(UINT)pointdiff);
        break;
    case TYPE_TSUMO:
        ret = m_pFunc(m_pInst,MJPI_ONACTION,ieparam,MJPIR_TSUMO | (UINT)pTaku->m_event.m_command.m_pai);
        if(param.m_iKaze2 == 0){
            /* 親のツモアガリ */
            for(i=0;i<4;i++){
                if(i==(int)ieparam){
                    pointdiff[i] = (pTaku->m_event.m_result.m_iKoScore + pTaku->m_iTsumibou * 100) * 3 + pTaku->m_iRiichibou * 1000;
                }else{
                    pointdiff[i] = -(pTaku->m_event.m_result.m_iKoScore + pTaku->m_iTsumibou * 100);
                }
            }
        }else{
            /* 子のツモアガリ */
            for(i=0;i<4;i++){
                if(i==(int)ieparam){
                    pointdiff[i] = (pTaku->m_event.m_result.m_iKoScore + pTaku->m_iTsumibou * 100) * 2 + (pTaku->m_event.m_result.m_iOyaScore + pTaku->m_iTsumibou * 100) + pTaku->m_iRiichibou * 1000;
                }else if((param.m_iKaze1 + i) % 4 == 0){
                    pointdiff[i] = -(pTaku->m_event.m_result.m_iOyaScore + pTaku->m_iTsumibou * 100);
                }else{
                    pointdiff[i] = -(pTaku->m_event.m_result.m_iKoScore + pTaku->m_iTsumibou * 100);
                }
            }
        }
        m_pFunc(m_pInst,MJPI_ENDKYOKU,MJEK_AGARI,(UINT)pointdiff);
        break;
    case TYPE_RIICHI:
        bAction = true;
        ret = m_pFunc(m_pInst,MJPI_ONACTION,ieparam,MJPIR_REACH | (UINT)pTaku->m_event.m_command.m_pai);
        break;
    case TYPE_ANKAN:
        ret = m_pFunc(m_pInst,MJPI_ONACTION,ieparam,MJPIR_ANKAN | (UINT)pTaku->m_event.m_command.m_pai);
        break;
    case TYPE_KUWAEKAN:
        ret = m_pFunc(m_pInst,MJPI_ONACTION,ieparam,MJPIR_MINKAN | (UINT)pTaku->m_event.m_command.m_pai);
        break;
    case TYPE_DAIMINKAN:
        ret = m_pFunc(m_pInst,MJPI_ONACTION,ieparam,MJPIR_MINKAN | (UINT)pTaku->m_event.m_command.m_pai);
        break;
    default:
        break;
    }


    return ret;

}

void OMPlayer::setCommand(UINT id,OMTaku *pTaku, OMCommand &command)
{
    int i;
    int ind = pTaku->getMemberIndex(this);
    switch(id & 0xFFFFFF00){
    case MJPIR_CHII1:
        for(i=0;i<pTaku->m_members[ind].m_aCommandList.size();i++){
            if(pTaku->m_members[ind].m_aCommandList[i].m_mentsu.getNakiPos() == 0){
                command.m_iId = pTaku->m_members[ind].m_aCommandList[i].m_iId;
                break;
            }
        }
        break;
    case MJPIR_CHII2:
        for(i=0;i<pTaku->m_members[ind].m_aCommandList.size();i++){
            if(pTaku->m_members[ind].m_aCommandList[i].m_mentsu.getNakiPos() == 2){
                command.m_iId = pTaku->m_members[ind].m_aCommandList[i].m_iId;
                break;
            }
        }
        break;
    case MJPIR_CHII3:
        for(i=0;i<pTaku->m_members[ind].m_aCommandList.size();i++){
            if(pTaku->m_members[ind].m_aCommandList[i].m_mentsu.getNakiPos() == 1){
                command.m_iId = pTaku->m_members[ind].m_aCommandList[i].m_iId;
                break;
            }
        }
        break;
    case MJPIR_PON:
        command.m_iId = ID_PON;
        break;
    case MJPIR_KAN:
        command.m_iId = ID_DAIMINKAN;
        break;
    case MJPIR_RON:
        command.m_iId = ID_RON;
        break;
    default:
        command.m_iId = ID_PASS;
        break;
    }
}

UINT OMPlayer::onSutehai(OMTaku *pTaku, OMCommand &command)
{
    UINT ret = 0;
    int ind = pTaku->getMemberIndex(this);
    UINT paiID;
    int i;
    if(pTaku->m_members[ind].m_gamestate.m_bTsumo){
        paiID = pTaku->m_members[ind].m_aTehai[pTaku->m_members[ind].m_aTehai.size()-1];
    }else{
        paiID = 63;
    }

    ret = m_pFunc(m_pInst,MJPI_SUTEHAI,(UINT)paiID,0);
    switch(ret & 0xFFFFFF00){
    case MJPIR_SUTEHAI:
            if((ret & 0xFF) == 13){
                    command.m_iId = ID_DAHAI + pTaku->m_members[pTaku->m_iTurn].m_aTehai.size() -1;
            }else{
                    command.m_iId = ID_DAHAI + (ret & 0xFF);
            }
            break;
    case MJPIR_REACH:
            command.m_iId = ID_RIICHI + (ret & 0xFF);
            if(!pTaku->m_members[ind].isExecutableCommand(command)){
                    command.m_iId = ID_DAHAI + (ret & 0xFF);
            }
            break;
    case MJPIR_TSUMO:
            command.m_iId = ID_TSUMO;
            if(!pTaku->m_members[ind].isExecutableCommand(command)){
                    AfxDebugBreak();
                    command.m_iId = ID_DAHAI + pTaku->m_members[pTaku->m_iTurn].m_aTehai.size() -1;
            }
            break;
    case MJPIR_NAGASHI:
            command.m_iId = ID_TOUHAI;
            if(!pTaku->m_members[ind].isExecutableCommand(command)){
                    AfxDebugBreak();
                    command.m_iId = ID_DAHAI + pTaku->m_members[pTaku->m_iTurn].m_aTehai.size() -1;
            }
            break;
    case MJPIR_KAN:
            command.m_iId = ID_DAHAI + pTaku->m_members[pTaku->m_iTurn].m_aTehai.size() -1;
            if((ret & 0xFF) < pTaku->m_members[ind].m_aTehai.size()){
                    OMPai pai = pTaku->m_members[ind].m_aTehai[ret & 0xFF];
                    for(i=0;i<pTaku->m_members[ind].m_aCommandList.size();i++){
                            if(((UINT)pTaku->m_members[ind].m_aCommandList[i].m_pai & 63) == ((UINT)pai & 63) && (pTaku->m_members[ind].m_aCommandList[i].m_iType == TYPE_ANKAN || pTaku->m_members[ind].m_aCommandList[i].m_iType == TYPE_KUWAEKAN)){
                                    command.m_iId = pTaku->m_members[ind].m_aCommandList[i].m_iId;
                            }
                    }
            }

            break;
    default:
            command.m_iId = ID_DAHAI + pTaku->m_members[pTaku->m_iTurn].m_aTehai.size() -1;
            break;
    }

    return ret;
}
