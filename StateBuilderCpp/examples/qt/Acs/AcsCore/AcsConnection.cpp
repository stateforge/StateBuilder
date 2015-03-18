#include <QsLog.h>
#include "AcsConnection.h"
#include "AcsConnectionPrivate.h"
#include "AcsConnectionFsm.h"
#include "AcsClient.h"
#include "AcsMessageParam.h"

//// Hope 20 MB is enough
const int CONTENTLENGTH_MAX = 20 * 1024 * 1025; 
const int INDENT_LEVEL = 2; 

//
/**
@class AcsConnection
*/
AcsConnection::AcsConnection(QHttpConnection *pHttpConnection,
                             QObject *pParent) : QObject(pParent)
{
    QLOG_DEBUG();
    m_pPrivate = new AcsConnectionPrivate(this, pHttpConnection);
}

AcsConnection::~AcsConnection()
{
    QLOG_DEBUG();
}

void AcsConnection::sendInformResponse()
{
    m_pPrivate->sendInformResponse();
}

void AcsConnection::sendRequestGetName()
{
    m_pPrivate->sendRequestGetName();
}

void AcsConnection::sendRequestSetValue(AcsParamList &paramList)
{
    m_pPrivate->sendRequestSetValue(paramList);
}

void AcsConnection::sendRequestGetValue(AcsParamList &paramList)
{
    m_pPrivate->sendRequestGetValue(paramList);
}

void AcsConnection::sendRequestReboot()
{
    m_pPrivate->sendRequestReboot();
}

void AcsConnection::sendRequestDownload(AcsParamDownload &paramDownload)
{
    m_pPrivate->sendRequestDownload(paramDownload);
}

void AcsConnection::sendTransferCompleteResponse()
{
    m_pPrivate->sendTransferCompleteResponse();
}

void AcsConnection::sendHttpResponse(int code, const QString& status)
{
    m_pPrivate->sendHttpResponse(code, status);
}

/**
@class AcsConnectionPrivate
*/
AcsConnectionPrivate::AcsConnectionPrivate(
                             AcsConnection *pParent,
                             QHttpConnection *pHttpConnection) : Tr69ConnectionBase(pParent),
m_pContext(NULL),
m_pHttpConnection(pHttpConnection),
m_pRequest(NULL),
m_pResponse(NULL),
m_pParser(new AcsParser(this)),
m_pMessageBuilder(new AcsMessageBuilder(this)),
m_pParent(pParent)
{
    m_pContext = new AcsConnectionContext(*m_pParent, *this, this);
    httpConnectionInit();
    parserInit();
}



AcsConnectionPrivate::~AcsConnectionPrivate()
{
    QLOG_DEBUG();
}

void AcsConnectionPrivate::sendInformResponse()
{
    QLOG_DEBUG()<<  "TODO id ";
    //Response
    QtSoapMessage informResponse;
    m_pMessageBuilder->buildInformResponse("TODO", informResponse);
    sendSoapMessage(informResponse);
}

void AcsConnectionPrivate::sendRequestGetName()
{
    QLOG_DEBUG();
    QtSoapMessage request;
    m_pMessageBuilder->buildGetParameterNamesRequest(request);
    sendSoapMessage(request);
}

void AcsConnectionPrivate::sendRequestSetValue(AcsParamList &paramList)
{
    QLOG_DEBUG();
    QtSoapMessage request;
    m_pMessageBuilder->buildSetValueRequest(request, paramList);
    sendSoapMessage(request);
}

void AcsConnectionPrivate::sendRequestGetValue(AcsParamList &paramList)
{
    QLOG_DEBUG();
    QtSoapMessage request;
    m_pMessageBuilder->buildGetValueRequest(request, paramList);
    sendSoapMessage(request);
}

void AcsConnectionPrivate::sendRequestReboot()
{
    QLOG_DEBUG();
    QtSoapMessage request;
    m_pMessageBuilder->buildRebootRequest(request);
    sendSoapMessage(request);
}

void AcsConnectionPrivate::sendRequestDownload(AcsParamDownload &paramDownload)
{
    QLOG_DEBUG();
    QtSoapMessage request;
    m_pMessageBuilder->buildDownloadRequest(request, paramDownload);
    sendSoapMessage(request);
}

void AcsConnectionPrivate::sendTransferCompleteResponse()
{
    QLOG_DEBUG();
    QtSoapMessage response;
    m_pMessageBuilder->buildTransferCompleteResponse(response);
    sendSoapMessage(response);
}

void AcsConnectionPrivate::requestResponseInit()
{
    QObject::connect(m_pRequest, SIGNAL(data(const QByteArray&)), 
                     this, SLOT(readyRead(const QByteArray&)));
    QObject::connect(m_pRequest, SIGNAL(end()),
                     this, SLOT(endRequest()));
    QObject::connect(m_pResponse, SIGNAL(done()), 
                     this, SLOT(doneResponse()));
}

void AcsConnectionPrivate::parserInit()
{
    QObject::connect(m_pParser, SIGNAL(error()), 
                     this, SLOT(parserError()));
    QObject::connect(m_pParser, SIGNAL(informRequestReceived(QtSoapMessage *)),
                     this, SLOT(onInformRequest(QtSoapMessage *)));
                     
}

void AcsConnectionPrivate::httpConnectionInit()
{
   QObject::connect(m_pHttpConnection, SIGNAL(newRequest(QHttpRequest*,QHttpResponse*)), 
                    this, SLOT(httpConnectionNewRequest(QHttpRequest*,QHttpResponse*))); 
}
            
// Slots private
void AcsConnectionPrivate::readyRead(const QByteArray &buffer)
{
    QLOG_DEBUG() << "size " << buffer.size();
    m_pParser->accumulateData(buffer);
}

void AcsConnectionPrivate::endRequest()
{
    QLOG_DEBUG();
    m_pContext->evEndRequest();
    m_pParser->parse();
}

void AcsConnectionPrivate::doneResponse()
{
    QLOG_DEBUG();
    m_pContext->evDoneResponse();
}

void AcsConnectionPrivate::onHttpPost()
{
    QLOG_DEBUG();
    //m_client.onHttpPost();
}

//Private
void AcsConnectionPrivate::onInformRequest(QtSoapMessage *pRxMessage)
{
    QLOG_DEBUG();
    emit m_pParent->informRequestReceived(pRxMessage);
}

void AcsConnectionPrivate::onGetNameResponse(QtSoapMessage *pRxMessage)
{
    QLOG_DEBUG();
    //m_client.onGetNameResponse(pRxMessage);
}

void AcsConnectionPrivate::onSetValueResponse(QtSoapMessage *pRxMessage)
{
    QLOG_DEBUG();
    //m_client.onSetValueResponse(pRxMessage);
}

void AcsConnectionPrivate::onGetValueResponse(QtSoapMessage *pRxMessage)
{
    QLOG_DEBUG();
    //m_client.onGetValueResponse(pRxMessage);
}

void AcsConnectionPrivate::onTransferComplete(QtSoapMessage *pRxMessage)
{
    QLOG_DEBUG();
    //m_client.onTransferComplete(pRxMessage);
}

void AcsConnectionPrivate::onDownloadResponse(QtSoapMessage *pRxMessage)
{
    QLOG_DEBUG();
    //m_client.onDownloadResponse(pRxMessage);
}

void AcsConnectionPrivate::onRebootResponse(QtSoapMessage *pRxMessage)
{
    QLOG_DEBUG();
    //m_client.onRebootResponse(pRxMessage);
}

void AcsConnectionPrivate::onFault(QtSoapMessage *pRxMessage)
{
    QLOG_DEBUG();
    //m_client.onFault(pRxMessage);
}

void AcsConnectionPrivate::sendSoapMessage(QtSoapMessage &message)
{
    QLOG_DEBUG();
    QByteArray payload = message.toXmlString(INDENT_LEVEL).toAscii();
    m_pResponse->setHeader("Content-Length",QString::number(payload.size()));
    
    m_pResponse->writeHead(200);
    m_pResponse->write(payload);
}

void AcsConnectionPrivate::sendHttpResponse(int code, const QString& status)
{
    QLOG_DEBUG() << "code " << code << ", status " << status;
    m_pResponse->writeHead(code);
    m_pResponse->end();
}

// Slots private

void AcsConnectionPrivate::httpConnectionNewRequest(QHttpRequest *pRequest, QHttpResponse *pResponse)
{
    QLOG_DEBUG() << "From" << pRequest->remoteAddress() << ":" << pRequest->remotePort();
    QLOG_DEBUG() << pRequest->method() << " " << pRequest->url();
    
    m_pRequest = pRequest;
    m_pResponse = pResponse;
    requestResponseInit();
    m_pParser->reset();
    //TODO HEEFRE send event to fsm
}

void AcsConnectionPrivate::parserError()
{
    QLOG_DEBUG() << "error " << m_pParser->errorCode();
    //TODO HEEFRE
    //m_pContext->evParserError();
}

