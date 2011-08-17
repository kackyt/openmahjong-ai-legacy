#ifndef ILLEGALPARAMEXCEPTION_H
#define ILLEGALPARAMEXCEPTION_H
#include "OMXMLCommon.h"
#include "Exception.h"

class OMIllegalParamException : public OMException
{
public:
    OMIllegalParamException(QString&);
};

#endif // ILLEGALPARAMEXCEPTION_H
