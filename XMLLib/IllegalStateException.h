#ifndef ILLEGALSTATEEXCEPTION_H
#define ILLEGALSTATEEXCEPTION_H
#include "OMXMLCommon.h"
#include "Exception.h"

class OMIllegalStateException : public OMException
{
public:
    OMIllegalStateException(OMString &);
};

#endif // ILLEGALSTATEEXCEPTION_H
