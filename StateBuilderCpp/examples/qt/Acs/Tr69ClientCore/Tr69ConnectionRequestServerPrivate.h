#ifndef TR69CONNECTIONREQUESTSERVERPRIVATE_H
#define TR69CONNECTIONREQUESTSERVERPRIVATE_H

#include <QtCore/QObject>

#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHttp>

#include <qhttpserver.h>
#include <qhttprequest.h>
#include <qhttpresponse.h>

#include "Tr69ConnectionRequestServer.h"
#include "Tr69ConnectionRequestServerFsm.h"

class Tr69ConnectionRequestServerPrivate : public QObject {

    Q_OBJECT

public:
    Tr69ConnectionRequestServerPrivate(Tr69ConnectionRequestServer *pParent);
    ~Tr69ConnectionRequestServerPrivate();

    void emitReadyToSendInformRequest();

    Tr69ConnectionRequestServerContext *m_pContext;
    Tr69ConnectionRequestServerConf m_conf;

    QHttpServer *m_pHttpServer;
    quint16 m_wPortListen; // Listen to connection request from Acs

    QString m_errorMessage;
    int m_iErrorCode;

public slots:
    void start();
    void stop();

private slots:    
    // QHttpServer
    void httpServerNewRequest(QHttpRequest*, QHttpResponse*);
    //Tr69ConnectionRequestResponder
    void responderEnd();

private:
    bool listen();
    void httpServerInit();
    void contextInit();

    Tr69ConnectionRequestServer *m_pParent;
};

#endif
