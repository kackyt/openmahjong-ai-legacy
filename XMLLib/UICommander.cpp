#include "UICommander.h"

OMUICommander::OMUICommander()
    : m_state(OM_COM_STATE_DAHAI)
{
}

OMUICommander::~OMUICommander()
{

}

void OMUICommander::initialize(OMMember &member)
{
    m_state = OM_COM_STATE_DAHAI;
    m_member = member;
    m_aSelPai.clear();
}

bool OMUICommander::searchCommand(int type,int count,OMCommand *pCom)
{
    int i;
    bool matched = false;

    if(count < 0 || m_aSelPai.size() == count){
        for(i=0;i<m_member.m_aCommandList.size();i++){
            OMCommand com = m_member.m_aCommandList[i];

            if(com.m_iType == type){
                switch(count){
                case 1:
                    if(com.m_pai == m_aSelPai[0] || com.m_mentsu.indexOf(m_aSelPai[0]) >= 0){
                        matched = true;
                    }
                    break;
                case 2:
                    if(com.m_mentsu.indexOf(m_aSelPai[0]) >= 0 && com.m_mentsu.indexOf(m_aSelPai[1]) >= 0){
                        matched = true;
                    }
                    break;
                default:
                    matched = true;
                    break;
                }
            }

            if(matched){
                if(pCom != NULL){
                    *pCom = com;
                }
                break;
            }
        }
    }

    return matched;
}

bool OMUICommander::searchCommandById(int id,OMCommand *pCom)
{
    int i;
    bool matched = false;

    for(i=0;i<m_member.m_aCommandList.size();i++){
        OMCommand com = m_member.m_aCommandList[i];

        if(com.m_iId == id){
            matched = true;
            if(pCom != NULL){
                *pCom = com;
            }
            break;
        }

    }

    return matched;
}


bool OMUICommander::addPai(OMPai &pai)
{
    bool ret = false;
    m_aSelPai.append(pai);

    switch(m_state){
    case OM_COM_STATE_DAHAI:
        ret = searchCommand(TYPE_DAHAI,1,&m_selectedCommand);
        if(ret){
            m_state = OM_COM_STATE_CONFIRM;
        }
        m_aSelPai.clear();
        break;
    case OM_COM_STATE_TII:
    case OM_COM_STATE_PON:
        if(m_aSelPai.size() > 2){
            m_aSelPai.remove(0);
        }
        break;
    case OM_COM_STATE_KAN:
    case OM_COM_STATE_RIICHI:
        if(m_aSelPai.size() > 1){
            m_aSelPai.remove(0);
        }
        break;
    default:
        break;
    }

    return ret;
}

void OMUICommander::removePai(OMPai &pai)
{
    int i;
    for(i=0;i<m_aSelPai.size();i++){
        if(m_aSelPai[i] == pai){
            m_aSelPai.remove(i);
            break;
        }
    }

}

void OMUICommander::setTii(bool enable)
{
    if(m_state != OM_COM_STATE_CONFIRM){
        if(enable){
            m_state = OM_COM_STATE_TII;
        }else{
            m_state = OM_COM_STATE_DAHAI;
        }
    }
}

void OMUICommander::setPon(bool enable)
{
    if(m_state != OM_COM_STATE_CONFIRM){
        if(enable){
            m_state = OM_COM_STATE_PON;
        }else{
            m_state = OM_COM_STATE_DAHAI;
        }
    }
}

void OMUICommander::setKan(bool enable)
{
    if(m_state != OM_COM_STATE_CONFIRM){
        if(enable){
            m_state = OM_COM_STATE_KAN;
        }else{
            m_state = OM_COM_STATE_DAHAI;
        }
    }
}

void OMUICommander::setRiichi(bool enable)
{
    if(m_state != OM_COM_STATE_CONFIRM){
        if(enable){
            m_state = OM_COM_STATE_RIICHI;
        }else{
            m_state = OM_COM_STATE_DAHAI;
        }
    }
}

bool OMUICommander::setRon()
{
    bool ret = false;
    if(m_state != OM_COM_STATE_CONFIRM){
        if(searchCommand(TYPE_RON,-1,&m_selectedCommand)){
            m_state = OM_COM_STATE_CONFIRM;
            ret = true;
        }
    }
    return ret;
}

bool OMUICommander::setTsumo()
{
    bool ret = false;
    if(m_state != OM_COM_STATE_CONFIRM){
        if(searchCommand(TYPE_TSUMO,-1,&m_selectedCommand)){
            m_state = OM_COM_STATE_CONFIRM;
            ret = true;
        }
    }
    return ret;
}

bool OMUICommander::setTouhai()
{
    bool ret = false;
    if(m_state != OM_COM_STATE_CONFIRM){
        if(searchCommand(TYPE_TOUHAI,-1,&m_selectedCommand)){
            m_state = OM_COM_STATE_CONFIRM;
            ret = true;
        }
    }
    return ret;
}

bool OMUICommander::setPass()
{
    bool ret = false;
    if(m_state != OM_COM_STATE_CONFIRM){
        if(searchCommand(TYPE_PASS,-1,&m_selectedCommand)){
            m_state = OM_COM_STATE_CONFIRM;
            ret = true;
        }
    }
    return ret;
}

bool OMUICommander::setConfirm()
{
    bool matched = false;
    switch(m_state){
    case OM_COM_STATE_TII:
        matched = searchCommand(TYPE_TII,2,&m_selectedCommand);
        break;
    case OM_COM_STATE_PON:
        matched = searchCommand(TYPE_PON,2,&m_selectedCommand);
        break;
    case OM_COM_STATE_KAN:
        matched = searchCommand(TYPE_ANKAN,1,&m_selectedCommand)
                || searchCommand(TYPE_DAIMINKAN,1,&m_selectedCommand)
                || searchCommand(TYPE_KUWAEKAN,1,&m_selectedCommand);
        break;
    case OM_COM_STATE_RIICHI:
        matched = searchCommand(TYPE_RIICHI,1,&m_selectedCommand);
        break;
    default:
        break;
    }

    if(matched){
        m_state = OM_COM_STATE_CONFIRM;
    }

    return matched;
}

bool OMUICommander::setStart()
{
    bool ret = false;
    if(searchCommandById(ID_START,&m_selectedCommand)){
        m_state = OM_COM_STATE_CONFIRM;
        ret = true;
    }

    return ret;
}

bool OMUICommander::getCommand(OMCommand &com)
{
    bool ret = false;

    if(m_state == OM_COM_STATE_CONFIRM){
        com = m_selectedCommand;
        ret = true;
    }

    return ret;
}

bool OMUICommander::isTiiAvailable()
{
    return searchCommand(TYPE_TII);
}

bool OMUICommander::isPonAvailable()
{
    return searchCommand(TYPE_PON);
}

bool OMUICommander::isKanAvailable()
{
    return searchCommand(TYPE_ANKAN) || searchCommand(TYPE_DAIMINKAN) || searchCommand(TYPE_KUWAEKAN);
}

bool OMUICommander::isRonAvailable()
{
    return searchCommand(TYPE_RON);
}

bool OMUICommander::isRiichiAvailable()
{
    return searchCommand(TYPE_RIICHI);
}

bool OMUICommander::isTsumoAvailable()
{
    return searchCommand(TYPE_TSUMO);
}

bool OMUICommander::isTouhaiAvailable()
{
    return searchCommand(TYPE_TOUHAI);
}

bool OMUICommander::isPassAvailable()
{
    return searchCommand(TYPE_PASS);
}

void OMUICommander::__unused_function()
{

}
