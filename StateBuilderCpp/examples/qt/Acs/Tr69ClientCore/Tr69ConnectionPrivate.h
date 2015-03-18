#ifndef TR69CONNECTIONPRIVATE_H
#define TR69CONNECTIONPRIVATE_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QPointer>
#include <QtNetwork/QTcpSocket>

#include <qhttprequest.h>
#include <qhttpresponse.h>
#include <qhttpconnection.h>

#include "Tr69Connection.h"
#include "Tr69MessageBuilder.h"
#include "Tr69Parser.h"

class QtSoapMessage;
class QNetworkAccessManager;
class Tr69ConnectionContext;

/**
@class Tr69Connection
*/
class Tr69ConnectionPrivate : public QObject
{
    Q_OBJECT

public:
    Tr69ConnectionPrivate(Tr69Connection *pParent, QUrl &acsUrl);
    ~Tr69ConnectionPrivate();

    void doConnect();
    void sendInformRequest();
    void doSendInformRequest();
    void doSendPost();

    void emitFinished();

    Tr69ConnectionContext *m_pContext;
    QUrl &m_acsUrl;
    QTcpSocket *m_pSocket;
    QHttpConnection* m_pHttpConnection;
    QString m_id;
    QString m_errorMessage;
    Tr69ConnectionConf m_conf;
    Tr69MessageBuilder *m_pMessageBuilder;
    Tr69Parser *m_pParser;

private slots:
    // QTcpSocket
    void socketError(QAbstractSocket::SocketError error);
    void socketDisconnected();
    void socketStateChanged(QAbstractSocket::SocketState socketState);
    //QHttpConnection
    void httpNewRequest(QHttpRequest *pRequest, QHttpResponse *pReponse);
    //QHttpRequest
    void readyRead(const QByteArray &buffer);
    void endRequest();
    //QHttpResponse
    void doneResponse();
    // Tr69ConnectionContext
    void contextEnd();
    //Tr69Parser
    void parserError();
    void onInformResponse(QtSoapMessage *pRxMessage);

private:
    void contextInit();
    void socketInit();
    void parserInit();

private:
    Tr69Connection *m_pParent;
};

#endif
