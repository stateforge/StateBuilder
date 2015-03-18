#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QHttpRequestHeader>

#include <QtSoapMessage>

#include <QsLog.h>
#include <SmQtObserver.h>

#include "Tr69Connection.h"
#include "Tr69ConnectionPrivate.h"
#include "Tr69ConnectionFsm.h"

/**
* Tr69Connection Implementation
*/
Tr69Connection::Tr69Connection(QUrl &acsUrl, QObject *pParent) :  Tr69ConnectionBase(pParent)
{
    m_pPrivate = new Tr69ConnectionPrivate(this, acsUrl);
}

Tr69Connection::~Tr69Connection()
{
    QLOG_DEBUG();
}

const QString& Tr69Connection::getErrorMessage() const 
{
    return m_pPrivate->m_errorMessage;
}

QUrl& Tr69Connection::acsUrl() const
{
    return m_pPrivate->m_acsUrl;
}

Tr69ConnectionConf& Tr69Connection::conf() const
{
    return m_pPrivate->m_conf;
}

void Tr69Connection::sendInformRequest()
{
    m_pPrivate->sendInformRequest();
}

/**
@class Tr69ConnectionPrivate Implementation
*/
Tr69ConnectionPrivate::Tr69ConnectionPrivate(Tr69Connection *pParent, QUrl &acsUrl) : QObject(pParent),
m_pContext(NULL),
m_acsUrl(acsUrl),
m_pSocket(new QTcpSocket()),
m_pHttpConnection(new QHttpConnection(m_pSocket, this)),
m_pMessageBuilder(new Tr69MessageBuilder(this)),
m_pParser(new Tr69Parser(this)),
m_pParent(pParent)
{
    QLOG_DEBUG();
    contextInit();
    socketInit();
    parserInit();
}

Tr69ConnectionPrivate::~Tr69ConnectionPrivate()
{
    QLOG_DEBUG();
}

void Tr69ConnectionPrivate::contextEnd()
{
    QLOG_DEBUG();
}

void Tr69ConnectionPrivate::sendInformRequest()
{
    m_pContext->evSendInformRequest();
}

void Tr69ConnectionPrivate::doSendInformRequest()
{
    QLOG_DEBUG();
    QtSoapMessage informRequest;
    m_pMessageBuilder->buildInformRequest(informRequest);
    QHttpRequestHeader request("POST", "/");
    
    QByteArray payload = informRequest.toXmlString().toAscii();
    request.setContentLength(payload.size());
    request.setContentType("xml");
    QByteArray packet(request.toString().toAscii());
    packet.append(payload);
    QLOG_DEBUG() << "writting " << packet;
    m_pSocket->write(packet);
}

void Tr69ConnectionPrivate::doConnect()
{
    QLOG_DEBUG();
    m_pSocket->connectToHost(m_acsUrl.host(), m_acsUrl.port());
}

void Tr69ConnectionPrivate::doSendPost()
{
    QLOG_DEBUG();
    QHttpRequestHeader request("POST", "/");
    QByteArray packet(request.toString().toAscii());
    QLOG_DEBUG() << packet;
    m_pSocket->write(packet);
}

void Tr69ConnectionPrivate::onInformResponse(QtSoapMessage *pRxMessage)
{
    QLOG_DEBUG();
    m_pContext->evInformResponse();
}

//Slots private
void Tr69ConnectionPrivate::parserError()
{
    QLOG_DEBUG() << "error " << m_pParser->errorCode();
    m_pContext->evParserError();
}

void Tr69ConnectionPrivate::socketError(QAbstractSocket::SocketError error)
{
    QLOG_WARN() << error;
    m_pContext->evSocketError();
}

void Tr69ConnectionPrivate::socketDisconnected()
{
    QLOG_DEBUG();
    m_pContext->evSocketDisconnected();
}

void Tr69ConnectionPrivate::socketStateChanged(QAbstractSocket::SocketState socketState)
{
    switch(socketState){
        case QAbstractSocket::ConnectedState:
             QLOG_DEBUG() << "Connected";
            m_pContext->evSocketConnected();
            break;
        case QAbstractSocket::UnconnectedState:
             QLOG_DEBUG() << "Unconnected";
            break;
        default:
            QLOG_WARN() << socketState;
            break;
    }
}

// Slots private

void Tr69ConnectionPrivate::httpNewRequest(QHttpRequest *pRequest, QHttpResponse *pResponse)
{
    QLOG_DEBUG();
    QLOG_DEBUG() << "From" << pRequest->remoteAddress() << ":" << pRequest->remotePort();
    QLOG_DEBUG() << pRequest->url();
    connect(pRequest, SIGNAL(data(const QByteArray&)), this, SLOT(readyRead(const QByteArray&)));
    connect(pRequest, SIGNAL(end()), this, SLOT(endRequest()));
    connect(pResponse, SIGNAL(done()), this, SLOT(doneResponse()));
}

void Tr69ConnectionPrivate::readyRead(const QByteArray &buffer)
{
    QLOG_DEBUG() << "size " << buffer.size();
    m_pParser->accumulateData(buffer);
}

void Tr69ConnectionPrivate::endRequest()
{
    QLOG_DEBUG();
    QLOG_DEBUG() << "TODO";
    m_pParser->parse();
    //Delete QHttpRequest
    //sender()->deleteLater();
}

void Tr69ConnectionPrivate::doneResponse()
{
    QLOG_DEBUG() << "TODO";
    //m_pContext->evDoneResponse();
}


//Private
void Tr69ConnectionPrivate::contextInit()
{
    m_pContext = new Tr69ConnectionContext(*m_pParent, *this, this);
    m_pContext->SetObserver(SmQtObserver::GetInstance());
    QObject::connect(m_pContext, SIGNAL(end()),
                     this, SLOT(contextEnd()));
}

void Tr69ConnectionPrivate::socketInit()
{
    QObject::connect(m_pSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(socketError(QAbstractSocket::SocketError)));
    QObject::connect(m_pSocket, SIGNAL(disconnected()),
                     this, SLOT(socketDisconnected()));
    QObject::connect(m_pSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                     this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));

    QObject::connect(m_pHttpConnection, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
                     this, SLOT(httpNewRequest(QHttpRequest*, QHttpResponse*)));
}

void Tr69ConnectionPrivate::parserInit()
{
    QObject::connect(m_pParser, SIGNAL(error()),
                     this, SLOT(parserError()));

    QObject::connect(m_pParser, SIGNAL(informResponseReceived(QtSoapMessage *)),
                     this, SLOT(onInformResponse(QtSoapMessage *)));
}

void Tr69ConnectionPrivate::emitFinished()
{
    emit m_pParent->finished();
}

