#ifndef ILLEGALSTATEEXCEPTION_H
#define ILLEGALSTATEEXCEPTION_H
#include "OMXMLCommon.h"
#include "Exception.h"

class OMIllegalStateException : public OMException
{
public:
    OMIllegalStateException(QString &);
};

#endif // ILLEGALSTATEEXCEPTION_H
