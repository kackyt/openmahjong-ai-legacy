#ifndef EXCEPTION_H
#define EXCEPTION_H
#include "OMXMLCommon.h"

class OMException
{
public:
    OMException(QString &);
    virtual ~OMException() {}
    QString& getMessage();
protected:
    QString m_message;
};

#endif // EXCEPTION_H
