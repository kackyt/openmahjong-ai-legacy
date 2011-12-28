#ifndef UICOMMANDER_H
#define UICOMMANDER_H

#include "OMXMLCommon.h"
#include "Member.h"
#include "Pai.h"

class OMUICommander
{
private:
    OMArray<OMPai> m_aSelPai;
    OMMember m_member;
    OMCommand m_selectedCommand;
    bool searchCommand(int type,int count = -1,OMCommand *pCom = NULL);
    virtual void __unused_function();
    enum {
        OM_COM_STATE_DAHAI,
        OM_COM_STATE_TII,
        OM_COM_STATE_PON,
        OM_COM_STATE_KAN,
        OM_COM_STATE_RIICHI,
        OM_COM_STATE_CONFIRM
    } m_state;
public:
    OMUICommander();
    virtual ~OMUICommander();
    void initialize(OMMember &member);
    bool addPai(OMPai &pai);
    void removePai(OMPai &pai);
    void setTii(bool enable);
    void setPon(bool enable);
    void setKan(bool enable);
    bool setRon();
    bool setTsumo();
    void setRiichi(bool enable);
    bool setPass();
    bool setTouhai();
    bool isTiiAvailable();
    bool isPonAvailable();
    bool isKanAvailable();
    bool isRonAvailable();
    bool isRiichiAvailable();
    bool isTsumoAvailable();
    bool isTouhaiAvailable();
    bool isPassAvailable();
    bool setConfirm();
    bool setStart();
    bool getCommand(OMCommand &com);
};

#endif // UICOMMANDER_H
