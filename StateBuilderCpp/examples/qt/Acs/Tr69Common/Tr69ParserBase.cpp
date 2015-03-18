

#include <QsLog.h>

#include "Tr69ParserBase.h"
#include <Tr69Tag.h>

Tr69ParserBase::Tr69ParserBase(QObject *pParent) : QObject(pParent),
m_eErrorCode(ERROR_NO),
m_pSoapMessage(new QtSoapMessage())
{
    QLOG_DEBUG() << m_pSoapMessage;
    //reset();
}

Tr69ParserBase::~Tr69ParserBase()
{
    QLOG_DEBUG();
    QLOG_DEBUG() << m_pSoapMessage;
    delete m_pSoapMessage;
}

void Tr69ParserBase::reset()
{
    QLOG_DEBUG();
    
    QLOG_DEBUG() << m_pSoapMessage;
    QLOG_DEBUG() << m_pSoapMessage->errorString();
    
    //m_pSoapMessage = QSharedPointer<QtSoapMessage>(new QtSoapMessage());
    QLOG_DEBUG() << m_pSoapMessage->toXmlString();
    m_pSoapMessage->clear();
    m_buffer.clear();
}

void Tr69ParserBase::parseSoapHeader()
{
    QLOG_DEBUG() << m_pSoapMessage;
    
    QLOG_DEBUG() << m_buffer;
    
    if(m_pSoapMessage->setContent(m_buffer) == false){
        setError(ERROR_CONTENT_INVALID);
        return;
    }

    //Header
    QtSoapStruct& header = m_pSoapMessage->header();
    if (!header.isValid()) {
        setError(ERROR_HEADER_INVALID);
        return;
    }

    //look for cwmp:ID, we need it for the reply
    QtSoapType& id = header[Tr69Tag::TAG_ID];
    QLOG_DEBUG() << "Id " << id.toString();
    //TODO HEEFRE
    //m_id = id.toString();
    if(id.toString().isEmpty()){
        QLOG_WARN() << "Id is empty";
        setError(ERROR_ID_INVALID);
        return;
    }

    // Body
    QtSoapStruct& body = m_pSoapMessage->body();
    if (!body.isValid()) {
        setError(ERROR_BODY_INVALID);
        return;
    }
    QLOG_DEBUG() << "end of parsing";
    QLOG_DEBUG() << m_pSoapMessage;
    QLOG_DEBUG() << m_pSoapMessage->faultCode();
    QLOG_DEBUG() << m_pSoapMessage->toXmlString();
}

void Tr69ParserBase::setError(ErrorCode errorCode)
{
    QLOG_DEBUG() << errorCode;
    m_eErrorCode = errorCode;
    emit error();
}

void Tr69ParserBase::accumulateData(const QByteArray &payload)
{
    QLOG_DEBUG() << "current size " << m_buffer.size() << ", arrived " << payload.size();
    m_buffer.append(payload);
}