
#include <QsLog.h>
#include <SmQtObserver.h>
#include "Tr69ConnectionRequestResponder.h"
#include "Tr69ConnectionRequestResponderPrivate.h"
#include "Tr69ConnectionRequestServer.h"

// Tr69ConnectionRequestResponder implementation

Tr69ConnectionRequestResponder::Tr69ConnectionRequestResponder(QHttpRequest *pRequest, QHttpResponse *pResponse, QObject *pParent) : QObject(pParent)
{
    QLOG_DEBUG();
    m_pPrivate = new Tr69ConnectionRequestResponderPrivate(pRequest, pResponse,this);
}

Tr69ConnectionRequestResponder::~Tr69ConnectionRequestResponder()
{
    QLOG_DEBUG();
}

void Tr69ConnectionRequestResponder::start()
{
    m_pPrivate->start();
}

void Tr69ConnectionRequestResponder::stop()
{
    m_pPrivate->stop();
}

const QString& Tr69ConnectionRequestResponder::errorMessage() const
{
    return m_pPrivate->m_errorMessage;
}

Tr69ConnectionRequestResponderConf& Tr69ConnectionRequestResponder::conf()
{
    return m_pPrivate->m_conf;
}

// Tr69ConnectionRequestResponderPrivate implementation

Tr69ConnectionRequestResponderPrivate::Tr69ConnectionRequestResponderPrivate(QHttpRequest *pRequest, 
                                                                             QHttpResponse *pResponse,
                                                                             Tr69ConnectionRequestResponder *pParent) : QObject(pParent),
m_pContext(NULL),
m_pRequest(pRequest),
m_pResponse(pResponse),
m_pParent(pParent)
{
    contextInit();
    connect(pRequest, SIGNAL(data(const QByteArray&)), this, SLOT(accumulateRequest(const QByteArray&)));
    connect(pRequest, SIGNAL(end()), this, SLOT(endRequest()));
    connect(pResponse, SIGNAL(done()), this, SLOT(doneResponse()));
}

Tr69ConnectionRequestResponderPrivate::~Tr69ConnectionRequestResponderPrivate()
{
    QLOG_DEBUG();
}

//Public
void Tr69ConnectionRequestResponderPrivate::start()
{
    //Do not call EnterInitialState in the constructor because the object is not fully created at that stage
    m_pContext->EnterInitialState();
}

void Tr69ConnectionRequestResponderPrivate::stop()
{
    m_pContext->evStop();
}

//Private:
void Tr69ConnectionRequestResponderPrivate::contextInit()
{
    m_pContext = new Tr69ConnectionRequestResponderContext(*m_pParent, *this, *m_pRequest, *m_pResponse, this);
    m_pContext->SetObserver(SmQtObserver::GetInstance());

    QObject::connect(m_pContext,
        SIGNAL(end()),
        this,
        SLOT(contextEnd()));
}

//Slots private
void Tr69ConnectionRequestResponderPrivate::contextEnd()
{
    QLOG_DEBUG();
    emit m_pParent->end();
    m_pParent->deleteLater();
}

void Tr69ConnectionRequestResponderPrivate::accumulateRequest(const QByteArray &data)
{
    QLOG_DEBUG() << data;
}

void Tr69ConnectionRequestResponderPrivate::endRequest()
{
    QLOG_DEBUG();
    m_pContext->evEndRequest();
}
void Tr69ConnectionRequestResponderPrivate::doneResponse()
{
    QLOG_DEBUG();
    m_pContext->evDoneResponse();
}
