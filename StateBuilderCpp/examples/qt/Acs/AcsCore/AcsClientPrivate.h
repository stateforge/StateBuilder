#ifndef ACSCLIENTPRIVATE_H
#define ACSCLIENTPRIVATE_H

#include <QtCore/QObject>
#include <QtCore/QQueue>
#include <QtCore/QDebug>
#include <QtCore/QLinkedList>
#include <QtCore/QTimerEvent>
#include <QtCore/QUrl>


#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QHttp>
//#include <QtNetwork/QAbstractSocket>
//#include <QtNetwork/QNetworkReply>
#include <QtSoapMessage>
#include <SmQtObserver.h>

#include "AcsClient.h"
#include "AcsClientFsm.h"
#include "AcsRequest.h"
#include "AcsConnection.h"

class AcsClientPrivate : public QObject {

  Q_OBJECT

  public:
    AcsClientPrivate(AcsClient *pParent, const QUrl &clientUrl, AcsClientToken &clientToken);
    ~AcsClientPrivate();

    void setConnection(AcsConnection *pConnection);
    void setRxMessage(QtSoapMessage *pRxMessage);

    // call from outside
    void start();

    // call from AcsConnection
    void onHttpPost();
    void onInformRequest(QtSoapMessage *pRxMessage);
    void onGetNameResponse(QtSoapMessage *pRxMessage);
    void onSetValueResponse(QtSoapMessage *pRxMessage);
    void onGetValueResponse(QtSoapMessage *pRxMessage);
    void onRebootResponse(QtSoapMessage *pRxMessage);
    void onTransferComplete(QtSoapMessage *pRxMessage);
    void onDownloadResponse(QtSoapMessage *pRxMessage);
    void onFault(QtSoapMessage *pRxMessage);

    void onSocketError(QAbstractSocket::SocketError error);
    void onRequestEnd();
    void onUnknownRequest();
    // Call from AcsRequestxxx
    
    bool requestAdd(AcsRequest* pRequest);
    void requestEnd();
   // Call by fsm
    void connectionRequestSend();
    void connectionRequestDestroy();

    void sendInformResponse();
    void sendTransferCompleteResponse();
    void sendRequest();
    void sendHttpResponse(int code = 200, const QString& status = "OK");

    bool hasPendingRequest(){return !m_requestQueue.isEmpty();}

    
    //Emit signals
    void emitError();
    void emitEnd();

    //AcsClient *pClient;
    AcsClientContext *m_pContext;
    AcsClientConf m_conf;
    AcsClientToken m_clientToken;
    AcsConnection *m_pConnection;

    QHttp *m_pHttpClient;
    void httpClientInit();
    //QNetworkAccessManager *m_pNetworkManager;
    //QNetworkReply *m_pNetworkReply;
    QUrl m_url;

    QQueue<AcsRequest*> m_requestQueue;

    AcsRequest* m_pRequestCurrent;
    QString m_errorMessage;
    int m_iErrorCode;

  private slots:
 // QHttp
    void httpDone(bool error);
    void httpRequestFinished(int id, bool error);
    void httpAuthenticationRequired(const QString &hostname, quint16 port, QAuthenticator *pAuthenticator);
    void httpStateChanged(int state);
    //QNetworkReply
    //void connectionRequestReplyFinished(QNetworkReply *pReply);
    //void connectionRequestReadyRead();
    //AcsClientContext
    void contextEnd();

  private:
    void contextInit();
    //void networkManagerInit();
    void requestRemoveCurrent();

    AcsClient *m_pParent;
};

#endif
