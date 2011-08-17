#include "IllegalParamException.h"

OMIllegalParamException::OMIllegalParamException(QString &message)
    : OMException(message)
{
}
