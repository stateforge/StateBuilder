
#ifndef ACSCLIENT_H
#define ACSCLIENT_H

#include <QtCore/QObject>

#include <QtNetwork/QAbstractSocket>
#include <QtSoapMessage>

class AcsConnection;
class AcsRequest;
class AcsClientPrivate;

typedef QString AcsClientToken;
typedef QString RequestToken;

/**
  * @class AcsClientConf
  */

class AcsClientConf
{
public:
    AcsClientConf() :
       connectionRequestTimeout(30000),
        informRequestTimeout(10000),
        httpPostTimeout(5000){}
    long connectionRequestTimeout;
    long informRequestTimeout;
    long httpPostTimeout;
};

/**
  * @class AcsClient
  */
class AcsClient : public QObject
{
  Q_OBJECT

  public:
    AcsClient(const QUrl &url, AcsClientToken &clientToken);
    ~AcsClient();

    AcsClientConf& conf();

    const QUrl& getUrl() const;
    const QString& getErrorMessage() const;
    int getErrorCode() const;

    void setConnection(AcsConnection *pConnection);
    void setRxMessage(QtSoapMessage *pRxMessage);

    // call from outside
    void start();

   // Call from AcsRequestxxx
    void requestEnd();    
    bool requestAdd(AcsRequest* pRequest);
    
    void sendInformResponse();
    void sendTransferCompleteResponse();
    void sendRequest();
    void sendHttpResponse(int code = 200, const QString& status = "OK");

  public slots:
    void onSocketError(QAbstractSocket::SocketError error);
    void onRequestEnd();
    void onUnknownRequest();

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

  Q_SIGNALS:
    void end(AcsClientToken &clientToken, bool error);

  private:
    friend class AcsClientPrivate;
    AcsClientPrivate *m_pPrivate;
};

#endif
