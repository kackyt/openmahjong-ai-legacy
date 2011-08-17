#include "GenericClient.h"
#include "Player.h"
#include "Message.h"
#include "ConnectionException.h"
#include "IllegalParamException.h"
#include "IllegalStateException.h"

static const TCHAR *ieStrTable[] = {_T("東"),_T("南"),_T("西"),_T("北")};
static const TCHAR *posStrTable[] = {_T("自分"),_T("下家"),_T("対面"),_T("上家")};

OMGenericClient::OMGenericClient()
    : m_gamestate(OM_GAME_STATE_STOP)
{

}

void OMGenericClient::connect(OM_CONNECTION_TYPE contype,int session)
{
    int i,rescom;
    QString text,errmessage;
    OMCommand com;
    QDomDocument doc;
    QDomNode node;

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

        OM_CREATEDOCUMENT(doc);
        OM_LOADXML(doc,text);
        node = OM_GETELEMENT(doc,_T(TAG_OPENMAHJONGSERVER "/" TAG_TAKU));
        if(!OM_ISNULL(node)){
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

            OM_CREATEDOCUMENT(doc);
            OM_LOADXML(doc,text);

            if(i==0 && contype == OM_CONNECTION_TYPE_CREATE){
                QDomElement elem = OM_GETELEMENT(doc,_T(TAG_OPENMAHJONGSERVER));
                QString sesStr;
                OM_GETATTRIBUTE(elem,_T("session"),sesStr);
                m_iSession = OM_STRTOL(sesStr);

            }

            if(i==0 && contype == OM_CONNECTION_TYPE_CONNECT){
                QDomElement elem = OM_GETELEMENT(doc,_T(TAG_OPENMAHJONGSERVER "/" TAG_RESPONCE "/" TAG_COMMAND "/" TAG_RULE));
                m_rule.parseXML(elem);
            }

            node = OM_GETELEMENT(doc,_T(TAG_OPENMAHJONGSERVER "/" TAG_RESPONCE "/" TAG_COMMAND "/" TAG_PLAYER "/" TAG_ID));
            OM_TOLONG(node,m_players[i].m_iId);

            if(m_players[i].m_bIsComp){
                m_players[i].m_pFunc(m_players[i].m_pInst,MJPI_STARTGAME,0,0);
            }

        }


        break;

    }
}

OM_SYNC_STATE OMGenericClient::gameSync()
{
    QDomDocument doc;
    QDomNode node,nodeTaku;
    QDomNodeList nodeList;
    QDomNodeList nodeListTaku;
    QDomNodeList nodeListCode;
    QString recvMessage;
    OMCommand command;
    OM_DEFARRAY(OMCommand) aCommand;
    QString text;
    UINT ret;
    int i,j,code,kaze3,rescom;
    int aCode[4];
    int iPlayerDlgIndex;
    BOOL bPlayerCommand = FALSE;
    OM_SYNC_STATE retVal = OM_SYNC_STATE_USERCOMMAND;


    m_bBusy = TRUE;

    while(!bPlayerCommand && m_gamestate == OM_GAME_STATE_START) {
        for(i=0;i<m_iPlayerNum+m_iCompNum;i++) {
            command.m_iId = m_bFirst ? ID_STATUS : ID_UPDATE;
            command.m_player = m_players[i];
            aCommand.append(command);
        }

        sendCommand(aCommand,recvMessage);

        OM_CREATEDOCUMENT(doc);
        OM_LOADXML(doc,recvMessage);

        node = OM_GETELEMENT(doc,_T(TAG_OPENMAHJONGSERVER "/" TAG_ERROR));

        /* エラーを確認したら出力する */
        if(!OM_ISNULL(node)){

        }

        node = OM_GETELEMENT(doc,_T(TAG_OPENMAHJONGSERVER "/" TAG_RESPONCE "/" TAG_CODE));

        code = 0;

        OM_TOLONG(node,code);

        if(code == RESPONCE_SESSION) {
            return OM_SYNC_STATE_ERROR;
        }

        if(code == RESPONCE_PLAYERID) {
            return OM_SYNC_STATE_ERROR;
        }

        node = OM_GETELEMENT(doc,_T(TAG_OPENMAHJONGSERVER "/" TAG_STATE "/" TAG_CODE));
        code = 0;

        OM_TOLONG(node,code);

        if(code == CODE_BUSY) {
            OM_SLEEP(m_iSyncTick);
            continue;
        }

        nodeListTaku = OM_GETELEMENTLIST(doc,_T(TAG_OPENMAHJONGSERVER "/" TAG_TAKU));

        if(OM_ISEMPTY(nodeListTaku) || OM_LISTLENGTH(nodeListTaku) < m_iPlayerNum + m_iCompNum) {
            /* もし見つからなかったらタイムアウトもしくはサーバーエラーの可能性があるので、ステータスの取得をやり直す */
            m_bFirst = TRUE;
            continue;
        }

        nodeListCode = OM_GETELEMENTLIST(doc,_T(TAG_OPENMAHJONGSERVER "/" TAG_STATE "/" TAG_CODE));

        for(i=0;i<OM_LISTLENGTH(nodeListCode);i++){
            node = OM_LISTITEM(nodeListCode,i);
            OM_TOLONG(node,aCode[i]);
        }

        for(i=0;i<OM_LISTLENGTH(nodeListTaku);i++){
            nodeTaku = OM_LISTITEM(nodeListTaku,i);

            if(!OM_ISNULL(nodeTaku)){
                if(m_bFirst){
                    m_aTakuAll[i].parseXML(nodeTaku);
                }else{
                    OMTaku value;
                    value.parseXML(nodeTaku);
                    m_aTakuAll[i].update(value);
                }
            }
        }

        if(aCode[0] != CODE_WAITSYNC && aCode[0] != CODE_BUSY){
            m_bFirst = FALSE;
        }


        i = 0;
        while(i<m_iPlayerNum+m_iCompNum){


            m_pCurPlayer = &m_players[i];
            m_pCurTaku = &m_aTakuAll[i];

            node = OM_LISTITEM(nodeListCode,i);
            if(!OM_ISNULL(node)){
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
                    nodeTaku = OM_LISTITEM(nodeListTaku,i);
                    nodeList = OM_GETELEMENTLIST(nodeTaku,_T(TAG_MESSAGE));
                    for(j=0;j<OM_LISTLENGTH(nodeList);j++){
                        OMMessage mes;
                        int index;
                        QString mestext;
                        node = OM_LISTITEM(nodeList,j);
                        mes.parseXML(node);

                        index = m_pCurTaku->getMemberIndex(&mes.m_playerFrom);

                        mestext.Format(_T("%s[%s]：%s\r\n"),mes.m_playerFrom.m_strName,
                                       ieStrTable[m_pCurTaku->m_members[index].m_gamestate.m_iZikaze - 1],mes.m_strText);

                        if(m_pListener != NULL){
                            m_pListener->appendMessageText(mestext);
                        }
                    }

                    m_iPlayerIndex = m_pCurTaku->getMemberIndex(&m_players[i]);
                    for(j=0;j<4;j++){
                        kaze3 = OMPlayer::ieTable[m_pCurTaku->m_members[m_iPlayerIndex].m_gamestate.m_iZikaze - 1][m_pCurTaku->m_members[j].m_gamestate.m_iZikaze - 1];

                        text.Format(_T("%s(%s)"),m_pCurTaku->m_members[j].m_player.m_strName,posStrTable[kaze3]);
#if 0
                        if(kaze3 == 1){
                            m_btnSndTo1.SetWindowText((LPCTSTR)text);
                        }else if(kaze3 == 2){
                            m_btnSndTo2.SetWindowText((LPCTSTR)text);
                        }else if(kaze3 == 3){
                            m_btnSndTo3.SetWindowText((LPCTSTR)text);
                        }
#endif
                    }

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
                            m_pListener->onProgressed(m_iPlayerIndex,*m_pCurTaku);
                        }
                    }else if(code == CODE_WAITCOMMAND){
                        iPlayerDlgIndex = i;
                        m_pCurTaku->printState(m_iPlayerIndex,text);
                        //m_strCUIMessage += text;
                        m_bBusy = FALSE;
                        /* 実行可能なコマンドをサーチする */
                        for(j=0;j<m_pCurTaku->m_members[m_iPlayerIndex].m_aCommandList.size();j++){
                            if(m_pListener != NULL){
                                m_pListener->enableCommand(m_pCurTaku->m_members[m_iPlayerIndex].m_aCommandList[j]);
                            }
                        }

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

                        bPlayerCommand = TRUE;
                    }

                }

            }else{
                break;
            }

            i++;
        }

        OM_SLEEP(m_iSyncTick);

    }

    if(m_gamestate == OM_GAME_STATE_START){
        m_pCurPlayer = &m_players[iPlayerDlgIndex];
        m_pCurTaku = &m_aTakuAll[iPlayerDlgIndex];
    }

    return retVal;

}

void OMGenericClient::sendCommand(OM_DEFARRAY(OMCommand)& aCommand, QString& recvMessage)
{
    QDomDocument doc;
    QDomElement elemRoot,elemCom,elemID,elemPlayer;
    QDomNode node;
    QString str;
    QString sendMessage;
    int i;

    recvMessage = _T("");

    OM_CREATEDOCUMENT(doc);

    elemRoot = OM_EVAL(doc,createElement(_T(TAG_OPENMAHJONGCLIENT)));

    OM_EVAL(doc,appendChild(elemRoot));


    str.Format(_T("%d"),m_iSession);

    OM_EVAL(elemRoot,setAttribute(_T("session"),str));
    OM_EVAL(elemRoot,setAttribute(_T("version"),_T("0.1")));

    for(i=0;i<aCommand.size();i++)
    {
        aCommand[i].toXML(doc,elemRoot);
    }

    /* キューに入ったメッセージを送信 */

    OM_TOXML(doc,str);

    sendString(sendMessage,recvMessage);

}

int OMGenericClient::sendCommand(OMCommand &command, QString &recvMessage)
{
    QDomDocument doc;
    QDomElement elemRoot;
    QDomNode node;
    QString str;
    QString sendMessage;
    int code;

    recvMessage = _T("");

    OM_CREATEDOCUMENT(doc);

    elemRoot = OM_EVAL(doc,createElement(_T(TAG_OPENMAHJONGCLIENT)));

    OM_EVAL(doc,appendChild(elemRoot));


    str.Format(_T("%d"),m_iSession);

    OM_EVAL(elemRoot,setAttribute(_T("session"),str));
    OM_EVAL(elemRoot,setAttribute(_T("version"),_T("0.1")));

    if(command.m_iId != 0){
        command.toXML(doc,elemRoot);
    }

    /* キューに入ったメッセージを送信(TODO) */



    OM_TOXML(doc,sendMessage);

    try{
        sendString(sendMessage,recvMessage);
    }catch(...){

    }

    /* 受信メッセージをパース */
    OM_LOADXML(doc,recvMessage);

    node = OM_GETELEMENT(doc,_T(TAG_OPENMAHJONGSERVER "/" TAG_RESPONCE "/" TAG_CODE));

    OM_TOLONG(node,code);

    return code;
}

void OMGenericClient::setPlayerName(OM_DEFARRAY(QString) &playernames, OM_DEFARRAY(QString)&compnames)
{
    int i;

    if(m_gamestate != OM_GAME_STATE_STOP){
        QString mes = QString("setPlayerName called when state is not stopped.");
        throw OMIllegalStateException(mes);
    }
    m_iPlayerNum = playernames.size();
    m_iCompNum = compnames.size();

    if(m_iPlayerNum + m_iCompNum > 4){
        /* パラメータが不正 */
        QString mes("playernum + compnum > 4");
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
