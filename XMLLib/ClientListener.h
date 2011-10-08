#ifndef CLIENTLISTENER_H
#define CLIENTLISTENER_H
#include "OMXMLCommon.h"
#include "Command.h"
#include "Taku.h"

class OMClientListener
{
public:
    OMClientListener();
    virtual ~OMClientListener(){}
    virtual void appendMessageText(OMString *) = 0;
    virtual void onTii();
    virtual void onPon();
    virtual void onKan();
    virtual void onTsumo();
    virtual void onRon();
    virtual void onRiichi();
    virtual void onRivalRiichi();
    virtual void onDahai();
    virtual void onProgressed(int index,OMTaku *);
    virtual void onStarted(int index,OMTaku *);
    virtual void onStatusCode(int code);
};

#endif // CLIENTLISTENER_H
