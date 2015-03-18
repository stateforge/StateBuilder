#include <QtSoapMessage>
#include <QsLog.h>

#include "Tr69MessageBuilder.h"
#include <Tr69Tag.h>
/**
@class Tr69MessageBuilder
*/
Tr69MessageBuilder::Tr69MessageBuilder(QObject *pParent) : QObject(pParent) ,
m_nsPrefix(QString(Tr69Tag::NS_PREFIX) + ':')
{
    QLOG_DEBUG();
}

Tr69MessageBuilder::~Tr69MessageBuilder()
{
    QLOG_DEBUG();
}

void Tr69MessageBuilder::buildEnvelope(QtSoapMessage &message)
{
    message.namespaceAdd(Tr69Tag::NS_PREFIX, Tr69Tag::NS);
}

void Tr69MessageBuilder::buildInformRequest(QtSoapMessage &request)
{
    //Envelope
    buildEnvelope(request);

    //Header  
    QString id = "987654321";
    request.header().insert(new QtSoapSimpleType(QtSoapQName(nsPrefix() + Tr69Tag::TAG_ID), id));

    //Body
    QtSoapStruct *pInformRequestStruct = new QtSoapStruct(QtSoapQName(nsPrefix() + Tr69Tag::TAG_INFORM));
    //pInformRequestStruct->insert(new QtSoapSimpleType(QtSoapQName(TAG_COMMAND_KEY), "Reboot Method (ACS)"));

    QtSoapStruct& body = request.body();
    body.insert(pInformRequestStruct);


}

