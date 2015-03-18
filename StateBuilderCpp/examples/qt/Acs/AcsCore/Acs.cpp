#include <QtCore/QLinkedList>
#include <QtCore/QSocketNotifier>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QHttpResponseHeader>
#include <QtSoapMessage>

#include <QsLog.h>
#include "Acs.h"
#include "AcsPrivate.h"
#include "AcsClient.h"
#include "AcsConnection.h"
#include "AcsRequestReboot.h"
#include "AcsRequestDownload.h"


/** Acs Implementation
*/
Acs::Acs()
{
  m_pPrivate = new AcsPrivate(this);
}

Acs::~Acs()
{
    delete m_pPrivate;
}

bool Acs::clientCreate(AcsClientToken &clientToken, const QUrl &target)
{
    return m_pPrivate->clientCreate(clientToken, target);
}

bool Acs::clientDestroy(AcsClientToken &clientToken)
{
    return m_pPrivate->clientDestroy(clientToken);
}

bool Acs::clientStart(const AcsClientToken &clientToken)
{
    return m_pPrivate->clientStart(clientToken);
}

const QString& Acs::clientGetError(const AcsClientToken &clientToken)
{
    return m_pPrivate->clientGetError(clientToken);
}

void Acs::addRequestReboot(const AcsClientToken &clientToken)
{
    m_pPrivate->addRequestReboot(clientToken);
}

void Acs::addRequestDownload(const AcsClientToken &clientToken, AcsParamDownload &paramDownload)
{
    m_pPrivate->addRequestDownload(clientToken, paramDownload);
}

bool Acs::listen()
{
    return m_pPrivate->listen();
}

// AcsPrivate Implementation

AcsPrivate::AcsPrivate(Acs *pAcs) :
  m_pHttpServer(NULL),
  m_wPortListen(7547),
  m_pParent(pAcs)
{
  QLOG_DEBUG();
  httpServerInit();
}

AcsPrivate::~AcsPrivate()
{
    QLOG_DEBUG() << m_clientMap.size() << " client(s) left";
}

bool AcsPrivate::clientCreate(AcsClientToken &clientToken, const QUrl &target)
{
  clientToken = target.host();
  if(m_clientMap.contains(clientToken) == true){
    QLOG_WARN() << clientToken << " already exists";
  } else {
    AcsClient *pClient = new AcsClient(target, clientToken);
    QObject::connect(pClient, SIGNAL(end(AcsClientToken&, bool)),
                     this, SLOT(onClientEnd(AcsClientToken&, bool)));

    m_clientMap.insert(clientToken, pClient);  
    QLOG_DEBUG() << m_clientMap.size() << " total client(s)";
  }
  return true;
}

bool AcsPrivate::clientDestroy(AcsClientToken &clientToken)
{
  QLOG_DEBUG() << clientToken;
  if(m_clientMap.contains(clientToken) == true){
    AcsClient* pClient = m_clientMap[clientToken];
    m_clientMap.remove(clientToken);
    //This was tricky, this function can be called from pClient, so do not delete pClient now.
    pClient->deleteLater();
    QLOG_DEBUG() << m_clientMap.size() << " client(s) left";
    return true;
  } else {
    QLOG_DEBUG() << "no client for token " << clientToken;
    emit m_pParent->clientTokenInvalid(clientToken);
    return false;
  }  
}

bool AcsPrivate::clientStart(const AcsClientToken &clientToken)
{
  QLOG_DEBUG() << clientToken;
  AcsClient* pClient = getClient(clientToken);
  if(pClient){
    pClient->start();
    return true;
  } else {
    emit m_pParent->clientTokenInvalid(clientToken);
    return false;
  }
}

const QString& AcsPrivate::clientGetError(const AcsClientToken &clientToken)
{
  AcsClient* pClient = getClient(clientToken);
  if(pClient){
    return pClient->getErrorMessage();
  } else {
    emit m_pParent->clientTokenInvalid(clientToken);
    static QString empty;
    return empty;
  }
}

void AcsPrivate::onClientEnd(AcsClientToken &clientToken, bool bError)
{
  QLOG_DEBUG() << clientToken << ", error " << bError;
  emit m_pParent->clientEnd(clientToken, bError);
}

void AcsPrivate::onConnectionDestroyed()
{
  QLOG_DEBUG();
  connectionRemove((AcsConnection*)sender());
}

AcsClient* AcsPrivate::getClient(const AcsClientToken &clientToken)
{
  if(m_clientMap.contains(clientToken) == true){
    return m_clientMap[clientToken];
  } else {
    return NULL;
  }  
}

void AcsPrivate::addRequestReboot(const AcsClientToken &clientToken)
{
  QLOG_DEBUG() << clientToken;
  AcsClient* pClient = getClient(clientToken);
  if(pClient){
    new AcsRequestReboot(*pClient);
  } else {
    emit m_pParent->clientTokenInvalid(clientToken);
  }  
}

void AcsPrivate::addRequestDownload(const AcsClientToken &clientToken, AcsParamDownload &paramDownload)
{
  QLOG_DEBUG() << clientToken;
  AcsClient* pClient = getClient(clientToken);
  if(pClient){
    new AcsRequestDownload(*pClient, paramDownload);
  } else {
    emit m_pParent->clientTokenInvalid(clientToken);
  }  
}

bool AcsPrivate::connectionAdd(AcsConnection* pConnection)
{
  m_connectionList.append(pConnection);
  QLOG_DEBUG() << "connection number " << m_connectionList.size();
  return true;
}

bool AcsPrivate::connectionRemove(AcsConnection* pConnection)
{
  QLOG_DEBUG();
  QLOG_DEBUG() << "current connection number " << m_connectionList.size();
  return m_connectionList.removeOne(pConnection);
}

void AcsPrivate::connectionCreate(AcsClient* pClient, QHttpConnection *pQHttpConnection)
{
  QLOG_DEBUG();

  AcsConnection *pConnection = new AcsConnection(pQHttpConnection, this);
  connectionAdd(pConnection);
  connect(pConnection, SIGNAL(unknownRequest()), 
          pClient, SLOT(onUnknownRequest()));
  connect(pConnection, SIGNAL(informRequestReceived(QtSoapMessage *)),
          pClient, SLOT(onInformRequest(QtSoapMessage *)));

  connect(pConnection, SIGNAL(destroyed()), 
          this, SLOT(onConnectionDestroyed()));
  
  pClient->setConnection(pConnection);
}

bool AcsPrivate::listen()
{
    QLOG_DEBUG() << "port " << m_wPortListen;
    if (m_pHttpServer->listen(QHostAddress::Any, m_wPortListen) == false) {
        QLOG_ERROR() << "cannot listen";
        return false;
    } else {
        return true;
    }
}

//Private
void AcsPrivate::httpServerInit()
{
    if(m_pHttpServer != NULL){
        delete m_pHttpServer;
    }

    m_pHttpServer = new QHttpServer(this);
    QObject::connect(m_pHttpServer, SIGNAL(newIncomingConnection(QHttpConnection*)), 
                     this, SLOT(httpServerNewIncomingConnection(QHttpConnection*)));            
}

// Slot Private


void AcsPrivate::httpServerNewIncomingConnection(QHttpConnection *pQHttpConnection)
{
    QLOG_DEBUG();
    const QHostAddress peerAddress = pQHttpConnection->peerAddress();
    QLOG_DEBUG() << "peerAddress" << peerAddress.toString();
    QUrl url = peerAddress.toString();
    AcsClientToken clientToken = peerAddress.toString();
    AcsClient* pClient = getClient(clientToken);
    if(!pClient){
        QLOG_DEBUG() << "no exiting client for " << url;
        clientCreate(clientToken, url);
        pClient = getClient(clientToken);
        emit m_pParent->clientNew(clientToken);
    }

    connectionCreate(pClient, pQHttpConnection);    
}

