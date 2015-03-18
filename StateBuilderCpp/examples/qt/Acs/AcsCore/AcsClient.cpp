#include <QsLog.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include "AcsClient.h"
#include "AcsClientPrivate.h"
#include "AcsConnection.h"
#include "Tr69Tag.h"

const int CONNECTION_REQUEST_DEFAULT_PORT = 30005;

AcsClient::AcsClient(const QUrl &clientUrl, AcsClientToken &clientToken)
{
  m_pPrivate = new AcsClientPrivate(this, clientUrl, clientToken);
}

AcsClient::~AcsClient()
{
  delete m_pPrivate;
}

AcsClientConf& AcsClient::conf()
{
    return m_pPrivate->m_conf;
}

const QUrl& AcsClient::getUrl() const 
{
  return m_pPrivate->m_url;
}

const QString& AcsClient::getErrorMessage() const 
{
  return m_pPrivate->m_errorMessage;
}

int AcsClient::getErrorCode() const 
{
  return m_pPrivate->m_iErrorCode;
}

bool AcsClient::requestAdd(AcsRequest* pRequest)
{
  return m_pPrivate->requestAdd(pRequest);
}

void AcsClient::setConnection(AcsConnection *pConnection)
{
  m_pPrivate->setConnection(pConnection);
}

void AcsClient::setRxMessage(QtSoapMessage *pRxMessage)
{
  m_pPrivate->setRxMessage(pRxMessage);
}

void AcsClient::sendInformResponse()
{
  m_pPrivate->sendInformResponse();
}

void AcsClient::sendTransferCompleteResponse()
{
  m_pPrivate->sendTransferCompleteResponse();
}

void AcsClient::start()
{
  m_pPrivate->start();
}

void AcsClient::sendHttpResponse(int code, const QString& status)
{
  m_pPrivate->sendHttpResponse(code, status);
}

void AcsClient::onSocketError(QAbstractSocket::SocketError error)
{
  m_pPrivate->onSocketError(error);
}

void AcsClient::onRequestEnd()
{
  m_pPrivate->onRequestEnd();
}

void AcsClient::onUnknownRequest()
{
  m_pPrivate->onUnknownRequest();
}

void AcsClient::onHttpPost()
{
  m_pPrivate->onHttpPost();
}

void AcsClient::onInformRequest(QtSoapMessage *pRxMessage)
{
  m_pPrivate->onInformRequest(pRxMessage);
}

void AcsClient::onGetNameResponse(QtSoapMessage *pRxMessage)
{
  m_pPrivate->onGetNameResponse(pRxMessage);
}

void AcsClient::onSetValueResponse(QtSoapMessage *pRxMessage)
{
  m_pPrivate->onSetValueResponse(pRxMessage);
}

void AcsClient::onGetValueResponse(QtSoapMessage *pRxMessage)
{
  m_pPrivate->onGetValueResponse(pRxMessage);
}

void AcsClient::onRebootResponse(QtSoapMessage *pRxMessage)
{
  m_pPrivate->onRebootResponse(pRxMessage);
}

void AcsClient::onDownloadResponse(QtSoapMessage *pRxMessage)
{
  m_pPrivate->onDownloadResponse(pRxMessage);
}

void AcsClient::onTransferComplete(QtSoapMessage *pRxMessage)
{
  m_pPrivate->onTransferComplete(pRxMessage);
}

void AcsClient::onFault(QtSoapMessage *pRxMessage)
{
  //qDebug() << "AcsClient::onFault() " << m_host;
  //TODO HEEFRE
}

void AcsClient::requestEnd()
{
  m_pPrivate->requestEnd();
}

AcsClientPrivate::AcsClientPrivate(AcsClient *pParent, const QUrl &clientUrl, AcsClientToken &clientToken) :
m_clientToken(clientToken),
m_pConnection(NULL),
m_pHttpClient(NULL),
m_url(clientUrl),
//m_pNetworkManager(new QNetworkAccessManager(this)),
//m_pNetworkReply(NULL),
m_pRequestCurrent(NULL),
m_iErrorCode(0),
m_pParent(pParent)
{
    QLOG_DEBUG() << m_clientToken;
    QLOG_DEBUG() << "port " << clientUrl.port();
    if(m_url.port() < 0){
        m_url.setPort(CONNECTION_REQUEST_DEFAULT_PORT);
    }
    if(m_url.path().isEmpty() == true){
        m_url.setPath("/");
    }

    contextInit();

    //networkManageInit();
}

AcsClientPrivate::~AcsClientPrivate()
{
    QLOG_DEBUG() << m_clientToken;
    delete m_pConnection;
    delete m_pRequestCurrent;
}

void AcsClientPrivate::contextInit()
{
    m_pContext = new AcsClientContext(*m_pParent, *this, this);
    m_pContext->SetObserver(SmQtObserver::GetInstance());

    QObject::connect(m_pContext,
        SIGNAL(end()),
        this,
        SLOT(contextEnd()));
}


void AcsClientPrivate::contextEnd()
{
    QLOG_DEBUG() <<  m_clientToken << ", error " << m_errorMessage;
    bool bError;
    m_errorMessage.isEmpty() == true ? bError = false: bError = true;
    emit m_pParent->end(m_clientToken, bError);
}

bool AcsClientPrivate::requestAdd(AcsRequest* pRequest)
{
    QLOG_DEBUG() <<  m_clientToken;
    m_requestQueue.enqueue(pRequest);
    return true;
}

void AcsClientPrivate::requestRemoveCurrent()
{
    QLOG_DEBUG();
    m_pRequestCurrent->deleteLater();
    m_pRequestCurrent = NULL;
}

void AcsClientPrivate::setConnection(AcsConnection *pConnection)
{
    if (m_pConnection) {
        QLOG_WARN() << "connection was already set for " << m_url;
        delete m_pConnection;
    }

    m_pConnection = pConnection;
}

void AcsClientPrivate::setRxMessage(QtSoapMessage *pRxMessage)
{
    if(m_pRequestCurrent) {
        m_pRequestCurrent->setRxMessage(pRxMessage);
    } else {
        QLOG_DEBUG () << "no current request";
    }
}

void AcsClientPrivate::httpClientInit()
{
    m_pHttpClient = new QHttp(m_url.host(), m_url.port(), this);
    connect(m_pHttpClient, SIGNAL(requestFinished(int, bool)),
        this, SLOT(httpRequestFinished(int, bool)));
    connect(m_pHttpClient, SIGNAL(done(bool)),
        this, SLOT(httpDone(bool)));
    connect(m_pHttpClient, SIGNAL(stateChanged(int)),
        this, SLOT(httpStateChanged(int)));
    connect(m_pHttpClient, SIGNAL(authenticationRequired(const QString &, quint16, QAuthenticator *)),
        this, SLOT(httpAuthenticationRequired(const QString &, quint16, QAuthenticator *)));
}

void AcsClientPrivate::httpDone(bool error)
{
    if (error == true) {
        QLOG_DEBUG() << "error " << m_pHttpClient->errorString();
        m_errorMessage = m_pHttpClient->errorString();
        m_pContext->evConnectionRequestError();
    } else {
        QLOG_DEBUG() << "done ";
        m_pContext->evConnectionRequestDone();
    }
}

void AcsClientPrivate::httpRequestFinished(int id , bool error)
{
    QLOG_DEBUG() << "id " << id << " error " << error;
}

void AcsClientPrivate::httpAuthenticationRequired(const QString &hostname, quint16 port, QAuthenticator *pAuthenticator)
{
    QLOG_DEBUG() << "hostname " << hostname
        << ", port " << port
        << ", set user " << m_url.userName()
        << ", password " << m_url.password();

    pAuthenticator->setUser(m_url.userName());
    pAuthenticator->setPassword(m_url.password());
}

void AcsClientPrivate::httpStateChanged(int state)
{
    switch(state){
    case QHttp::Unconnected:
        QLOG_DEBUG() << "Unconnected";
        //m_pContext->evConnectionRequestUnconnected();
        break;
    case QHttp::HostLookup:
        QLOG_DEBUG() << "HostLookup";
        //m_pContext->evConnectionRequestHostLookup();
        break;
    case QHttp::Connecting:
        QLOG_DEBUG()<< "Connecting";
        //m_pContext->evConnectionRequestConnecting();
        break;
    case QHttp::Sending:
        QLOG_DEBUG()<< "Sending";
        //m_pContext->evConnectionRequestSending();
        break;
    case QHttp::Reading:
        QLOG_DEBUG()<< "Reading";
        //m_pContext->evConnectionRequestReading();
        break;
    case QHttp::Connected:
        QLOG_DEBUG()<< "Connected";
        //m_pContext->evConnectionRequestConnected();
        break;
    case QHttp::Closing:
        QLOG_DEBUG()<< "Closing";
        //m_pContext->evConnectionRequestClosing();
        break;
    default:
        QLOG_ERROR();
        Q_ASSERT(0);
    }
}

void AcsClientPrivate::connectionRequestSend()
{
    QLOG_DEBUG() << "url " << m_url;
    httpClientInit();
    m_pHttpClient->get(m_url.path());
#if 0
    m_pNetworkReply = m_pNetworkManager->get(QNetworkRequest(m_url));
    QObject::connect(m_pNetworkReply, SIGNAL(readyRead()),
                     this, SLOT(connectionRequestReadyRead()));
#endif
}


void AcsClientPrivate::connectionRequestDestroy()
{
    QLOG_DEBUG() << "url " << m_url;
#if 0
    if(m_pNetworkReply != NULL){
      m_pNetworkReply->abort();
      delete m_pNetworkReply;
      m_pNetworkReply = NULL;
    }
#endif
}

void AcsClientPrivate::sendInformResponse()
{
    QLOG_DEBUG() << m_url;
    if (m_pConnection) {
        m_pConnection->sendInformResponse();
    } else {
        Q_ASSERT(0);
    }
}

void AcsClientPrivate::sendTransferCompleteResponse()
{
    QLOG_DEBUG() << m_url;
    if (m_pConnection) {
        m_pConnection->sendTransferCompleteResponse();
    } else {
        Q_ASSERT(0);
    }
}

void AcsClientPrivate::sendRequest()
{
    QLOG_DEBUG() << m_url << ", " << m_requestQueue.size() << " request(s)";

    if (m_pConnection) {
        if (!m_requestQueue.isEmpty()) {
            m_pRequestCurrent = m_requestQueue.dequeue();
            m_pRequestCurrent->start(*m_pConnection);
        } else {
            QLOG_DEBUG() << m_url << " no (more) request";
            m_pRequestCurrent = NULL;
        }
    } else {
        Q_ASSERT(0);
    }
}

void AcsClientPrivate::start()
{
    QLOG_DEBUG() << m_url;
    m_pContext->evStart();
}

void AcsClientPrivate::sendHttpResponse(int code, const QString& status)
{
    QLOG_DEBUG() << m_url;
    if (m_pConnection) {
        m_pConnection->sendHttpResponse(code, status);
    } else {
        Q_ASSERT(0);
    }
}

void AcsClientPrivate::onSocketError(QAbstractSocket::SocketError error)
{
    QLOG_DEBUG() << "error " << error;
    m_pContext->evSocketError();
    if(m_pRequestCurrent) m_pRequestCurrent->evSocketError();
}

void AcsClientPrivate::onRequestEnd()
{
    QLOG_DEBUG();
    if(m_iErrorCode != 0){
        if(m_pRequestCurrent != NULL){
            m_errorMessage = m_pRequestCurrent->getErrorMessage();
        } else {
            Q_ASSERT(0);
        }
    }
    m_pContext->evRequestEnd();
    requestRemoveCurrent();
}

void AcsClientPrivate::onUnknownRequest()
{
    QLOG_DEBUG();
//    m_errorMessage = m_pConnection->getErrorMessage();
    m_errorMessage = "Unknown Request";
    m_pContext->evUnknownRequest();
}

void AcsClientPrivate::onHttpPost()
{
    QLOG_DEBUG() << m_url;
    m_pContext->evHttpPost();
    if(m_pRequestCurrent) m_pRequestCurrent->evHttpPost();
}

void AcsClientPrivate::onInformRequest(QtSoapMessage *pRxMessage)
{
    QLOG_DEBUG() << m_url;
    setRxMessage(pRxMessage);
    m_pContext->evInformRequest();
    if(m_pRequestCurrent) {
        m_pRequestCurrent->evInformRequest();
    } else {
        QLOG_DEBUG() << "no current request " << m_url;
    }
}

void AcsClientPrivate::onGetNameResponse(QtSoapMessage *pRxMessage)
{
    QLOG_DEBUG() << m_url;
    m_pContext->evRequestEnd();
}

void AcsClientPrivate::onSetValueResponse(QtSoapMessage *pRxMessage)
{
    QLOG_DEBUG() << m_url;
    m_pContext->evRequestEnd();
}

void AcsClientPrivate::onGetValueResponse(QtSoapMessage *pRxMessage)
{
    QLOG_DEBUG() << m_url;
    m_pContext->evRequestEnd();
}

void AcsClientPrivate::onRebootResponse(QtSoapMessage *pRxMessage)
{
    QLOG_DEBUG() << m_url;
    if(m_pRequestCurrent) m_pRequestCurrent->evRebootResponse();
}

void AcsClientPrivate::onDownloadResponse(QtSoapMessage *pRxMessage)
{
    QLOG_DEBUG() << m_url;
    if(m_pRequestCurrent) m_pRequestCurrent->evDownloadResponse();
}

/**
  * AcsClientPrivate
  */
void AcsClientPrivate::onTransferComplete(QtSoapMessage *pRxMessage)
{
    QLOG_DEBUG() << m_url;
    if(m_pRequestCurrent == NULL) {
        return;
    }

    QtSoapStruct& body = pRxMessage->body();
    QtSoapType& tranfertComplete = body[Tr69Tag::TAG_TRANSFER_COMPLETE];
    QtSoapType& faultStruct = tranfertComplete[Tr69Tag::TAG_FAULTSTRUCT];

    if(faultStruct.isValid()){
        m_errorMessage = faultStruct[Tr69Tag::TAG_FAULTSTRING].toString();
        m_iErrorCode = faultStruct[Tr69Tag::TAG_FAULTCODE].toString().toInt();
        QLOG_DEBUG() << "Error Code " << m_iErrorCode 
            << ", Fault " << m_errorMessage;
    }

    m_pRequestCurrent->evTransferComplete();
}

void AcsClientPrivate::onFault(QtSoapMessage *pRxMessage)
{
    QLOG_DEBUG() << m_url;
    //TODO HEEFRE
}

void AcsClientPrivate::requestEnd()
{
    QLOG_DEBUG() << m_url;
    m_pContext->evRequestEnd();
}

void AcsClientPrivate::emitError()
{
    QLOG_DEBUG() << m_clientToken;
    emit m_pParent->end(m_clientToken, true);
}

void AcsClientPrivate::emitEnd()
{
    QLOG_DEBUG() << m_clientToken;
    emit m_pParent->end(m_clientToken, m_iErrorCode == 0 ? false:true);
}

#if 0
void AcsClientPrivate::networkManagerInit()
{
    QObject::connect(m_pNetworkManager, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(connectionRequestReplyFinished(QNetworkReply*)));
    // authenticationRequired ( QNetworkReply * reply, QAuthenticator * authenticator )
}

void AcsClientPrivate::connectionRequestReplyFinished(QNetworkReply *pReply)
{
    QLOG_DEBUG() << "bytesAvailable " << pReply->bytesAvailable();
    QLOG_DEBUG() << "is finished " << pReply->isFinished();

    Q_ASSERT(m_pNetworkReply == pReply);

    if(pReply->error() != QNetworkReply::NoError){
        m_errorMessage = pReply->errorString();
        QLOG_DEBUG() << "error " << m_errorMessage;
        m_pContext->evConnectionRequestError();
    } else {
        m_pContext->evConnectionRequestDone();
    }
    
    QLOG_DEBUG() << "HttpStatusCodeAttribute" << pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    QByteArray replyBuffer = pReply->readAll();
    pReply->close();
    pReply->deleteLater();
    m_pNetworkReply = NULL;
}

void AcsClientPrivate::connectionRequestReadyRead()
{
    QLOG_DEBUG();
    QByteArray replyBuffer = m_pNetworkReply->readAll();
    QLOG_DEBUG() << replyBuffer;
}
#endif
