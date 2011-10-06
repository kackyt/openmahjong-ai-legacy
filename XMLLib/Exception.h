#ifndef EXCEPTION_H
#define EXCEPTION_H
#include "OMXMLCommon.h"

class OMException
{
public:
    OMException(OMString &);
    virtual ~OMException() {}
    OMString& getMessage();
protected:
    OMString m_message;
};

#endif // EXCEPTION_H
