#include "IllegalParamException.h"

OMIllegalParamException::OMIllegalParamException(OMString &message)
    : OMException(message)
{
}
