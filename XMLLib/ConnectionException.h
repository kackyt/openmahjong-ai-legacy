#ifndef CONNECTIONEXCEPTION_H
#define CONNECTIONEXCEPTION_H
#include "OMXMLCommon.h"
#include "Exception.h"

class OMConnectionException : public OMException
{
public:
    OMConnectionException(OMString&);
};

#endif // CONNECTIONEXCEPTION_H
