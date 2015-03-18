
#include <QsLog.h>
#include <SmQtObserver.h>
#include "Tr69Client.h"
#include "Tr69ClientPrivate.h"
#include "Tr69ConnectionRequestServer.h"

// Tr69Client implementation

Tr69Client::Tr69Client(QObject *pParent) : QObject(pParent)
{
    QLOG_INFO();
    m_pPrivate = new Tr69ClientPrivate(this);
}

Tr69Client::~Tr69Client()
{
}

void Tr69Client::start()
{
    m_pPrivate->start();
}

void Tr69Client::stop()
{
    m_pPrivate->stop();
}

const QUrl& Tr69Client::acsUrl()
{
    return m_pPrivate->m_acsUrl;
}

Tr69ClientConf& Tr69Client::conf()
{
    return m_pPrivate->m_conf;
}

// Tr69ClientPrivate implementation

Tr69ClientPrivate::Tr69ClientPrivate(Tr69Client *pParent) : QObject(pParent),
m_pContext(NULL),
m_pParent(pParent)
{
    setDefaultAcsUrl();
    contextInit();
    tr69ConnectionInit();
    connectionRequestServerInit();
}

Tr69ClientPrivate::~Tr69ClientPrivate()
{
}

void Tr69ClientPrivate::tr69ConnectionInit()
{
    m_pTr69Connection = new Tr69Connection(m_acsUrl, this);
}

void Tr69ClientPrivate::connectionRequestServerInit()
{
    m_pConnectionRequestServer = new Tr69ConnectionRequestServer(this);
    QObject::connect(m_pConnectionRequestServer, SIGNAL(readyToSendInformRequest()),
                     m_pTr69Connection, SLOT(sendInformRequest()));
}

void Tr69ClientPrivate::setDefaultAcsUrl()
{
    //Set default Acs url
    m_acsUrl.setScheme("http");
    m_acsUrl.setHost("127.0.0.1");
    m_acsUrl.setPort(7547);
    m_acsUrl.setPath("/");
}

void Tr69ClientPrivate::contextInit()
{
    m_pContext = new Tr69ClientContext(*m_pParent, *this, this);
    m_pContext->SetObserver(SmQtObserver::GetInstance());
    QObject::connect(m_pContext,
        SIGNAL(end()),
        this,
        SLOT(contextEnd()));
}

void Tr69ClientPrivate::contextEnd()
{
    QLOG_DEBUG();
}

void Tr69ClientPrivate::start()
{
    QLOG_INFO();
    m_pConnectionRequestServer->start();
}

void Tr69ClientPrivate::stop()
{
    QLOG_INFO();
    m_pConnectionRequestServer->stop();
}

void Tr69ClientPrivate::sendInformRequest()
{
    m_pTr69Connection->sendInformRequest();
}
