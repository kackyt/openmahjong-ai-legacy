#include "GenericClient.h"
#include "Player.h"
#include "Message.h"
#include "ConnectionException.h"
#include "IllegalParamException.h"
#include "IllegalStateException.h"

static const UINT ieTable[][4] = {
    { 0, 1, 2, 3},
    { 3, 0, 1, 2},
    { 2, 3, 0, 1},
    { 1, 2, 3, 0},
};

static const TCHAR *ieStrTable[] = {_T("東"),_T("南"),_T("西"),_T("北")};
static const TCHAR *posStrTable[] = {_T("自分"),_T("下家"),_T("対面"),_T("上家")};

OMGenericClient::OMGenericClient()
    : m_gamestate(OM_GAME_STATE_STOPED),
      m_pListener(NULL)
{

}

void OMGenericClient::connect(OM_CONNECTION_TYPE contype,int session)
{
    int i,rescom;
    OMString text,errmessage;
    OMCommand com;
    OMDomDocument doc;
    OMDomNode node;

    switch(contype){
    case OM_CONNECTION_TYPE_DEBUG:
        com.m_iId = ID_DEBUG;
        m_iSession = session;
        do {
            rescom = sendCommand(com,text);
            if(rescom >= 0 && rescom != RESPONCE_OK) {
                /* 接続失敗 */
                errmessage.arg("error connection responce code = %1",rescom);
                throw OMConnectionException(errmessage);
            }
        }while(rescom < 0);

        OMCreateDocument(doc);
        OMLoadXML(doc,text);
        node = OMGetElement(doc,_T(TAG_OPENMAHJONGSERVER "/" TAG_TAKU));
        if(!OMIsNull(node)){
            m_aTakuAll[0].parseXML(node);
        }

        m_pCurTaku = &m_aTakuAll[0];

        /* コンピュータの自動サーチ(TODO) */
        break;
    case OM_CONNECTION_TYPE_CREATE:
    case OM_CONNECTION_TYPE_CONNECT:
        if(contype == OM_CONNECTION_TYPE_CONNECT) {
            m_iSession = session;
        }

        for(i=0;i<m_iPlayerNum+m_iCompNum;i++){
            if((i == 0) && (contype == OM_CONNECTION_TYPE_CREATE)) {
                com.m_iId = ID_CREATE;
                m_iSession = -1;
                com.m_rule = m_rule;
            }else{
                com.m_iId = ID_CONNECT;
            }

            m_players[i].m_iPrivateId = (rand() << 16) | (rand() & 0xFFFF);
            com.m_player = m_players[i];

            do {
                rescom = sendCommand(com,text);
                if(rescom >= 0 && rescom != RESPONCE_OK) {
                    /* 接続失敗 */
                    errmessage.arg("error connection responce code = %1",rescom);
                    throw OMConnectionException(errmessage);
                }
            }while(rescom < 0);

            OMCreateDocument(doc);
            OMLoadXML(doc,text);

            if(i==0 && contype == OM_CONNECTION_TYPE_CREATE){
                OMDomElement elem = OMGetElement(doc,_T(TAG_OPENMAHJONGSERVER));
                OMString sesStr;
                OMGetAttribute(elem,_T("session"),sesStr);
                m_iSession = OMStrtol(sesStr);

            }

            if(i==0 && contype == OM_CONNECTION_TYPE_CONNECT){
                OMDomElement elem = OMGetElement(doc,_T(TAG_OPENMAHJONGSERVER "/" TAG_RESPONCE "/" TAG_COMMAND "/" TAG_RULE));
                m_rule.parseXML(elem);
            }

            node = OMGetElement(doc,_T(TAG_OPENMAHJONGSERVER "/" TAG_RESPONCE "/" TAG_COMMAND "/" TAG_PLAYER "/" TAG_ID));
            OMToNum(node,m_players[i].m_iId);

            if(m_players[i].m_bIsComp){
                m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_STARTGAME,0,0);
            }

        }


        break;

    }

    m_gamestate = OM_GAME_STATE_CONNECTED;
}

void OMGenericClient::debugPrint()
{
    OMString text;
    m_pCurTaku->printState(m_iPlayerIndex,text);
    //m_strCUIMessage += text;

#if 0
    if(m_bDebugPrint){
        MJITehai tehai;
        int machihai[34];
        TENPAI_LIST tenpai_list;
        CString mestext,tmptext,tmptext2;

        m_pCurTaku->getMJITehai(index,&tehai);

        if(m_pCurTaku->m_members[index].m_gamestate.m_bTsumo == TRUE){
            search_tenpai((int *)tehai.tehai,tehai.tehai_max,machihai,&tenpai_list,1,99);
            tmptext = _T("");
            if(tenpai_list.shanten == 0){
                for(j=0;j<34;j++){
                    if(machihai[j]){
                        tmptext2.Format(_T(" %d"),j);
                        tmptext += tmptext2;
                    }
                }
            }
            mestext.Format(_T("Debug：向聴数 %d %s\r\n"),tenpai_list.shanten,tmptext);

            appendMessageText(mestext);

            MJITehai1 mjtehai[4];
            MJ0PARAM mjparam[4];
            UINT dora[8];
            MJIKawahai kawahai[4][20];
            int doranum;
            double nokori[34];
            double kikenhai[34];
            double dmentsu[27+34];
            double dmentsu2[27+34];
            double dmentsu3[27+34];
            double dbest[3];
            int ibest[3];
            double dworst[3];
            int iworst[3];

            dbest[0] = 0;
            dbest[1] = 0;
            dbest[2] = 0;
            dworst[0] = 0;
            dworst[1] = 0;
            dworst[2] = 0;

            m_pCurTaku->getMJITehai(index,&mjtehai[0],m_rule);
            m_pCurTaku->getMJITehai((index+1) & 3,&mjtehai[1],m_rule);
            m_pCurTaku->getMJITehai((index+2) & 3,&mjtehai[2],m_rule);
            m_pCurTaku->getMJITehai((index+3) & 3,&mjtehai[3],m_rule);
            mjparam[0].pTehai = &mjtehai[0];
            mjparam[1].pTehai = &mjtehai[1];
            mjparam[2].pTehai = &mjtehai[2];
            mjparam[3].pTehai = &mjtehai[3];
            mjparam[0].kawalength = m_pCurTaku->getKawahaiEx((index) & 3,&kawahai[0][0]);
            mjparam[1].kawalength = m_pCurTaku->getKawahaiEx((index+1) & 3,&kawahai[1][0]);
            mjparam[2].kawalength = m_pCurTaku->getKawahaiEx((index+2) & 3,&kawahai[2][0]);
            mjparam[3].kawalength = m_pCurTaku->getKawahaiEx((index+3) & 3,&kawahai[3][0]);
            mjparam[0].pKawahai = &kawahai[0][0];
            mjparam[1].pKawahai = &kawahai[1][0];
            mjparam[2].pKawahai = &kawahai[2][0];
            mjparam[3].pKawahai = &kawahai[3][0];
            for(j=0;j<m_pCurTaku->m_aDora.GetSize();j++){
                dora[j] = m_pCurTaku->m_aDora[i];
            }

            doranum = m_pCurTaku->m_aDora.GetSize();

            MJ0(&mjparam[0],(int*)dora,doranum,nokori,kikenhai,dmentsu,dmentsu2,dmentsu3);

            for(j=0;j<34;j++){
                if(dbest[0] < nokori[j]){
                    dbest[2] = dbest[1];
                    ibest[2] = ibest[1];
                    dbest[1] = dbest[0];
                    ibest[1] = ibest[0];
                    dbest[0] = nokori[j];
                    ibest[0] = j;
                }else if(dbest[1] < nokori[j]){
                    dbest[2] = dbest[1];
                    ibest[2] = ibest[1];
                    dbest[1] = nokori[j];
                    ibest[1] = j;
                }else if(dbest[2] < nokori[j]){
                    dbest[2] = nokori[j];
                    ibest[2] = j;
                }

                if(dworst[0] < kikenhai[j]){
                    dworst[2] = dworst[1];
                    iworst[2] = iworst[1];
                    dworst[1] = dworst[0];
                    iworst[1] = iworst[0];
                    dworst[0] = kikenhai[j];
                    iworst[0] = j;
                }else if(dworst[1] < kikenhai[j]){
                    dworst[2] = dworst[1];
                    iworst[2] = iworst[1];
                    dworst[1] = kikenhai[j];
                    iworst[1] = j;
                }else if(dworst[2] < kikenhai[j]){
                    dworst[2] = kikenhai[j];
                    iworst[2] = j;
                }
            }


            tmptext = _T("");
            for(j=0;j<3;j++){
                CPai pai;
                CString str;
                pai.set(ibest[j]);
                pai.getName(str);
                tmptext2.Format(_T("%d位[%s](%.1f枚) "),j+1,str,dbest[j]);
                tmptext += tmptext2;
            }

            mestext.Format(_T("Debug：ツモ予想 %s\r\n"),tmptext);

            appendMessageText(mestext);

            tmptext = _T("");
            for(j=0;j<3;j++){
                CPai pai;
                CString str;
                pai.set(iworst[j]);
                pai.getName(str);
                tmptext2.Format(_T("%d位[%s](%.1f%%) "),j+1,str,dworst[j] * 100);
                tmptext += tmptext2;
            }

            mestext.Format(_T("Debug：危険牌予想 %s\r\n"),tmptext);

            appendMessageText(mestext);
        }
    }
#endif

}

OM_SYNC_STATE OMGenericClient::gameSync()
{
    OMDomDocument doc;
    OMDomNode node,nodeTaku;
    OMDomNodeList nodeList;
    OMDomNodeList nodeListTaku;
    OMDomNodeList nodeListCode;
    OMString recvMessage;
    OMCommand command;
    OMArray<OMCommand> aCommand;
    OMString text;
    UINT ret;
    int i,j,code,kaze3,rescom;
    int aCode[4];
    int iPlayerDlgIndex;
    OM_SYNC_STATE retVal = OM_SYNC_STATE_OK;


    m_bBusy = TRUE;

    if(m_gamestate == OM_GAME_STATE_STARTED || m_gamestate == OM_GAME_STATE_PROGRESSING || m_gamestate == OM_GAME_STATE_WAITCOMMAND) {
        aCommand.clear();
        for(i=0;i<m_iPlayerNum+m_iCompNum;i++) {
            command.m_iId = m_gamestate == OM_GAME_STATE_STARTED ? ID_STATUS : ID_UPDATE;
            command.m_player = m_players[i];
            aCommand.append(command);
        }

        sendCommand(aCommand,recvMessage);

        OMCreateDocument(doc);
        OMLoadXML(doc,recvMessage);

        node = OMGetElement(doc,_T(TAG_OPENMAHJONGSERVER "/" TAG_ERROR));

        /* エラーを確認したら出力する [TODO] */
        if(!OMIsNull(node)){
            return OM_SYNC_STATE_ERROR;
        }

        node = OMGetElement(doc,_T(TAG_OPENMAHJONGSERVER "/" TAG_RESPONCE "/" TAG_CODE));

        code = 0;

        OMToNum(node,code);

        if(code == RESPONCE_SESSION) {
            return OM_SYNC_STATE_ERROR;
        }

        if(code == RESPONCE_PLAYERID) {
            return OM_SYNC_STATE_ERROR;
        }

        node = OMGetElement(doc,_T(TAG_OPENMAHJONGSERVER "/" TAG_STATE "/" TAG_CODE));
        code = 0;

        OMToNum(node,code);

        if(code == CODE_BUSY) {
            return OM_SYNC_STATE_ERROR;
        }

        nodeListTaku = OMGetElementList(doc,_T(TAG_OPENMAHJONGSERVER "/" TAG_TAKU));

        if(OMIsEmpty(nodeListTaku) || OMListLength(nodeListTaku) < m_iPlayerNum + m_iCompNum) {
            /* もし見つからなかったらタイムアウトもしくはサーバーエラーの可能性があるので、ステータスの取得をやり直す */
            return OM_SYNC_STATE_ERROR;
        }

        nodeListCode = OMGetElementList(doc,_T(TAG_OPENMAHJONGSERVER "/" TAG_STATE "/" TAG_CODE));

        for(i=0;i<OMListLength(nodeListCode);i++){
            node = OMListItem(nodeListCode,i);
            OMToNum(node,aCode[i]);
        }

        for(i=0;i<OMListLength(nodeListTaku);i++){
            nodeTaku = OMListItem(nodeListTaku,i);

            if(!OMIsNull(nodeTaku)){
                if(m_gamestate == OM_GAME_STATE_STARTED){
                    m_aTakuAll[i].parseXML(nodeTaku);
                }else{
                    OMTaku value;
                    value.parseXML(nodeTaku);

                    m_aTakuAll[i].update(value);
                }
            }
        }


        i = 0;
        while(i<m_iPlayerNum+m_iCompNum){
            m_pCurPlayer = &m_players[i];
            m_pCurTaku = &m_aTakuAll[i];
            m_iPlayerIndex = m_pCurTaku->getMemberIndex(&m_players[i]);

            if(m_gamestate == OM_GAME_STATE_STARTED && m_pListener != NULL){
                m_pListener->onStarted(i,m_pCurTaku);
            }

            node = OMListItem(nodeListCode,i);
            if(!OMIsNull(node)){
                code = aCode[i];
                if(m_pListener != NULL){
                    m_pListener->onStatusCode(code);
                }

                if(m_players[i].m_bIsComp){
                    /* コンピュータのターン */
                    OMPlayerActionParam param;
                    param.m_iIndex1 = m_pCurTaku->getMemberIndex(&m_players[i]);
                    param.m_iKaze1 = m_pCurTaku->m_members[param.m_iIndex1].m_gamestate.m_iZikaze - 1;
                    param.m_iIndex2 = m_pCurTaku->getMemberIndex(&m_pCurTaku->m_event.m_command.m_player);
                    param.m_iKaze2 = m_pCurTaku->m_members[param.m_iIndex2].m_gamestate.m_iZikaze - 1;
                    param.m_iType = m_pCurTaku->m_event.m_command.m_iType;

                    if(code != CODE_WAITSYNC){
                        ret = m_players[i].onPlayerAction(m_pCurTaku,param);
                    }
                    if(code == CODE_WAITCOMMAND){
                        /* コンピュータの行動を取得 */
                        int ind = m_pCurTaku->getMemberIndex(&m_players[i]);
                        command.m_player = m_players[i];
                        if(m_pCurTaku->m_members[ind].m_aCommandList[0].m_iId == ID_START){
                            command.m_iId = ID_START;
                            while(sendCommand(command,recvMessage) < 0);
                        }else{
                            if(ind == m_pCurTaku->m_iTurn){
                                m_players[i].onSutehai(m_pCurTaku,command);
                                while(sendCommand(command,recvMessage) < 0);
                            }else{
                                m_players[i].setCommand(ret,m_pCurTaku,command);
                                do{
                                    rescom = sendCommand(command,recvMessage);
                                    if(rescom >= 0 && rescom != RESPONCE_OK){
                                        /* 異常発生！ */
                                        // AfxDebugBreak();
                                    }
                                }while(rescom < 0);
                            }
                        }
                    }
                    /* コンピュータのターン(終わり) */
                }else{
                    /* プレーヤーのターン(終わり) */
                    // 自分宛のメッセージを引っ張り出してくる
                    nodeTaku = OMListItem(nodeListTaku,i);
#if 0
                    nodeList = OMGetElementList(nodeTaku,_T(TAG_MESSAGE));
                    for(j=0;j<OMListLength(nodeList);j++){
                        OMMessage mes;
                        int index;
                        OMString mestext;
                        node = OMListItem(nodeList,j);
                        mes.parseXML(node);

                        index = m_pCurTaku->getMemberIndex(&mes.m_playerFrom);

                        mestext.Format(_T("%s[%s]：%s\r\n"),mes.m_playerFrom.m_strName,
                                       ieStrTable[m_pCurTaku->m_members[index].m_gamestate.m_iZikaze - 1],mes.m_strText);

                        if(m_pListener != NULL){
                            m_pListener->appendMessageText(&mestext);
                        }
                    }
#endif

#if 0
                    for(j=0;j<4;j++){
                        kaze3 = ieTable[m_pCurTaku->m_members[m_iPlayerIndex].m_gamestate.m_iZikaze - 1][m_pCurTaku->m_members[j].m_gamestate.m_iZikaze - 1];

                        text.Format(_T("%s(%s)"),m_pCurTaku->m_members[j].m_player.m_strName,posStrTable[kaze3]);
                        if(kaze3 == 1){
                            m_btnSndTo1.SetWindowText((LPCTSTR)text);
                        }else if(kaze3 == 2){
                            m_btnSndTo2.SetWindowText((LPCTSTR)text);
                        }else if(kaze3 == 3){
                            m_btnSndTo3.SetWindowText((LPCTSTR)text);
                        }
                    }
#endif

                    if(m_pListener != NULL){
                        if(code != CODE_WAITSYNC && code != CODE_BUSY && m_pCurTaku->m_event.m_command.m_iType == TYPE_RIICHI
                                && m_pCurTaku->m_event.m_command.m_player.m_iId != m_players[i].m_iId
                                && !m_pCurTaku->m_members[m_iPlayerIndex].m_gamestate.m_bRiichi){
                            // ライバルがリーチ
                            m_pListener->onRivalRiichi();
                        }

                        if(code != CODE_WAITSYNC){
                            switch(m_pCurTaku->m_event.m_command.m_iType){
                            case TYPE_DAHAI:
                                m_pListener->onDahai();
                                break;
                            case TYPE_TII:
                                m_pListener->onTii();
                                break;
                            case TYPE_PON:
                                m_pListener->onPon();
                                break;
                            case TYPE_DAIMINKAN:
                            case TYPE_ANKAN:
                            case TYPE_KUWAEKAN:
                                m_pListener->onKan();
                                break;
                            case TYPE_RIICHI:
                                m_pListener->onRiichi();
                                break;
                            default:
                                break;
                            }
                        }
                    }

                    if(code == CODE_PROGRESSED){
                        if(m_pListener != NULL){
                            m_pListener->onProgressed(m_iPlayerIndex,m_pCurTaku);
                        }
                    }else if(code == CODE_WAITCOMMAND && m_gamestate != OM_GAME_STATE_WAITCOMMAND){
                        int index = m_pCurTaku->getMemberIndex(&m_players[i]);
                        iPlayerDlgIndex = i;
                        debugPrint();
                        retVal = OM_SYNC_STATE_USERCOMMAND;
                        m_gamestate = OM_GAME_STATE_WAITCOMMAND;
                        for(j=0;j<m_pCurTaku->m_members[index].m_aCommandList.size();j++){
                            switch(m_pCurTaku->m_members[index].m_aCommandList[j].m_iType){
                            case TYPE_START:
                            case TYPE_KOUHAI:
                                retVal = OM_SYNC_STATE_NEXTKYOKU;
                                break;
                            default:
                                break;
                            }
                        }
                    }

                }

            }else{
                break;
            }

            i++;
        }

        if(aCode[0] != CODE_WAITSYNC && aCode[0] != CODE_BUSY && m_gamestate == OM_GAME_STATE_STARTED){
            m_gamestate = OM_GAME_STATE_PROGRESSING;
        }

    }

    m_bBusy = FALSE;

    if(retVal == OM_SYNC_STATE_USERCOMMAND){
        m_pCurPlayer = &m_players[iPlayerDlgIndex];
        m_pCurTaku = &m_aTakuAll[iPlayerDlgIndex];
        m_iPlayerIndex = m_pCurTaku->getMemberIndex(m_pCurPlayer);
    }

    return retVal;

}

void OMGenericClient::sendCommand(OMArray<OMCommand>& aCommand, OMString& recvMessage)
{
    OMDomDocument doc;
    OMDomElement elemRoot,elemCom,elemID,elemPlayer;
    OMDomNode node;
    OMString str;
    OMString sendMessage;
    int i;

    recvMessage = _T("");

    OMCreateDocument(doc);

    elemRoot = OMCreateElement(doc,_T(TAG_OPENMAHJONGCLIENT));

    OMAppendChild(doc,elemRoot);


    str.Format(_T("%d"),m_iSession);

    OMSetAttribute(elemRoot,_T("session"),str);
    OMSetAttribute(elemRoot,_T("version"),_T("0.1"));

    for(i=0;i<aCommand.size();i++)
    {
        aCommand[i].toXML(doc,elemRoot);
    }

    /* キューに入ったメッセージを送信 */

    OMToXML(doc,sendMessage);

    sendString(sendMessage,recvMessage);

}

int OMGenericClient::sendCommand(OMCommand &command, OMString &recvMessage)
{
    OMDomDocument doc;
    OMDomElement elemRoot;
    OMDomNode node;
    OMString str;
    OMString sendMessage;
    int code = -1;

    recvMessage = _T("");

    OMCreateDocument(doc);

    elemRoot = OMCreateElement(doc,_T(TAG_OPENMAHJONGCLIENT));

    OMAppendChild(doc,elemRoot);


    str.Format(_T("%d"),m_iSession);

    OMSetAttribute(elemRoot,_T("session"),str);
    OMSetAttribute(elemRoot,_T("version"),_T("0.1"));

    if(command.m_iId != 0){
        command.toXML(doc,elemRoot);
    }

    /* キューに入ったメッセージを送信(TODO) */

    OMToXML(doc,sendMessage);

    try{
        sendString(sendMessage,recvMessage);
        /* 受信メッセージをパース */
        OMLoadXML(doc,recvMessage);

        node = OMGetElement(doc,_T(TAG_OPENMAHJONGSERVER "/" TAG_RESPONCE "/" TAG_CODE));

        OMToNum(node,code);

        if(code == RESPONCE_OK && m_gamestate == OM_GAME_STATE_WAITCOMMAND){
            m_gamestate = OM_GAME_STATE_PROGRESSING;
        }

    }catch(...){

    }


    return code;
}

void OMGenericClient::setPlayerName(OMArray<OMString> &playernames, OMArray<OMString>&compnames)
{
    int i;

    if(m_gamestate != OM_GAME_STATE_STOPED){
        OMString mes = OMString("setPlayerName called when state is not stopped.");
        throw OMIllegalStateException(mes);
    }
    m_iPlayerNum = playernames.size();
    m_iCompNum = compnames.size();

    if(m_iPlayerNum + m_iCompNum > 4){
        /* パラメータが不正 */
        OMString mes("playernum + compnum > 4");
        throw OMIllegalParamException(mes);
    }

    for(i=0;i<m_iPlayerNum;i++){
        m_players[i].m_strName = playernames[i];
        m_players[i].m_bIsComp = false;
        m_players[i].m_pFunc = NULL;
        m_players[i].m_pInst = NULL;
    }

    for(i=0;i<m_iCompNum;i++){
        m_players[i + m_iPlayerNum].m_strName = compnames[i];
        m_players[i + m_iPlayerNum].m_bIsComp = true;
        m_players[i + m_iPlayerNum].m_pFunc = NULL;
        m_players[i + m_iPlayerNum].m_pInst = NULL;
    }

    m_gamestate = OM_GAME_STATE_PLAYERSETNAME;
}

int OMGenericClient::getPlayerIndex() const
{
    return m_iPlayerIndex;
}

void OMGenericClient::gameStart()
{
    if(m_gamestate == OM_GAME_STATE_CONNECTED || m_gamestate == OM_GAME_STATE_STOPED || m_gamestate == OM_GAME_STATE_WAITCOMMAND){
        m_gamestate = OM_GAME_STATE_STARTED;
    }
}

void OMGenericClient::gameStop()
{
    m_gamestate = OM_GAME_STATE_STOPED;
}

int OMGenericClient::getSessionNum() const
{
    return m_iSession;
}

void OMGenericClient::setClientListener(OMClientListener *pListener)
{
    m_pListener = pListener;
}
