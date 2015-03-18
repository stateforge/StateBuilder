
#include <QsLog.h>
#include <SmQtObserver.h>

#include <qhttpserver.h>
#include <qhttprequest.h>
#include <qhttpresponse.h>

#include "Tr69ConnectionRequestResponder.h"
#include "Tr69ConnectionRequestServer.h"
#include "Tr69ConnectionRequestServerPrivate.h"

// Tr69ConnectionRequestServer implementation

Tr69ConnectionRequestServer::Tr69ConnectionRequestServer(QObject* pParent) : QObject(pParent)
{
    QLOG_DEBUG();
    m_pPrivate = new Tr69ConnectionRequestServerPrivate(this);
}

Tr69ConnectionRequestServer::~Tr69ConnectionRequestServer()
{
    QLOG_DEBUG();
    delete m_pPrivate;
}

void Tr69ConnectionRequestServer::start()
{
    QLOG_DEBUG();
    m_pPrivate->start();
}

void Tr69ConnectionRequestServer::stop()
{
    QLOG_DEBUG();
    m_pPrivate->stop();
}

Tr69ConnectionRequestServerConf& Tr69ConnectionRequestServer::conf()
{
    return m_pPrivate->m_conf;
}

// Tr69ConnectionRequestServerPrivate implementation

Tr69ConnectionRequestServerPrivate::Tr69ConnectionRequestServerPrivate(Tr69ConnectionRequestServer *pParent) : QObject(pParent),
m_pContext(NULL),
m_pHttpServer(NULL),
m_wPortListen(30005),
m_pParent(pParent)
{
    httpServerInit();
    contextInit();
}

Tr69ConnectionRequestServerPrivate::~Tr69ConnectionRequestServerPrivate()
{
    QLOG_DEBUG();
}

//Public
void Tr69ConnectionRequestServerPrivate::start()
{
    listen();
}

void Tr69ConnectionRequestServerPrivate::stop()
{
    //TODO
}

void Tr69ConnectionRequestServerPrivate::emitReadyToSendInformRequest()
{
    QLOG_DEBUG();
    emit m_pParent->readyToSendInformRequest();
}

//Private
void Tr69ConnectionRequestServerPrivate::contextInit()
{
    m_pContext = new Tr69ConnectionRequestServerContext(*m_pParent, *this, this);
    m_pContext->SetObserver(SmQtObserver::GetInstance());
}

void Tr69ConnectionRequestServerPrivate::httpServerInit()
{
    if(m_pHttpServer != NULL){
        delete m_pHttpServer;
    }

    m_pHttpServer = new QHttpServer(this);
    QObject::connect(m_pHttpServer, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)), 
        this, SLOT(httpServerNewRequest(QHttpRequest*, QHttpResponse*)));
}

bool Tr69ConnectionRequestServerPrivate::listen()
{
    QLOG_DEBUG() << "port " << m_wPortListen;
    if (m_pHttpServer->listen(QHostAddress::Any, m_wPortListen) == false) {
        QLOG_ERROR() << "cannot listen";
        return false;
    } else {
        return true;
    }
}

//Slots private

void Tr69ConnectionRequestServerPrivate::httpServerNewRequest(QHttpRequest *pRequest, QHttpResponse *pResponse)
{
    QLOG_DEBUG() << "From" << pRequest->remoteAddress() << ":" << pRequest->remotePort();
    QLOG_DEBUG() << pRequest->method() << " " << pRequest->url();

    Tr69ConnectionRequestResponder *pResponder = new Tr69ConnectionRequestResponder(pRequest, pResponse, this);
    QObject::connect(pResponder, SIGNAL(end()), 
        this, SLOT(responderEnd()));

    pResponder->start();
}

void Tr69ConnectionRequestServerPrivate::responderEnd()
{
    QLOG_DEBUG();
    Tr69ConnectionRequestResponder *pResponder = (Tr69ConnectionRequestResponder*)sender();
    if(pResponder->errorMessage().isEmpty() == true){
        emit m_pParent->readyToSendInformRequest();
    } else {
        QLOG_WARN() << pResponder->errorMessage();
    }
}
