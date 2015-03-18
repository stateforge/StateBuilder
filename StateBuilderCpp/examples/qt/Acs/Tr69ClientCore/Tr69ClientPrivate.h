
#ifndef TR69CLIENTPRIVATE_H
#define TR69CLIENTPRIVATE_H

#include <memory.h>

#include <QtCore/QObject>
#include <QtCore/QQueue>
#include <QtCore/QDebug>
#include <QtCore/QLinkedList>
#include <QtCore/QTimerEvent>

#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHttp>

#include <QtSoapMessage>


#include "Tr69Client.h"
#include "Tr69Connection.h"
#include "Tr69ClientFsm.h"
#include "Tr69ConnectionRequestServer.h"

class Tr69ClientPrivate : public QObject {

    Q_OBJECT

public:
    Tr69ClientPrivate(Tr69Client *pParent);
    ~Tr69ClientPrivate();

    void listen();
    void sendHttpResponse(int code, const QString& status);

    Tr69ClientContext *m_pContext;
    Tr69ClientConf m_conf;

    QUrl m_acsUrl;
    Tr69Connection *m_pTr69Connection;
    Tr69ConnectionRequestServer *m_pConnectionRequestServer;
    
    QString m_errorMessage;
    int m_iErrorCode;

public slots:
    void start();
    void stop();
    void sendInformRequest();

private slots:
    // Tr69ClientContext slots
    void contextEnd();

private:
    void setDefaultAcsUrl();
    void tr69ConnectionInit();
    void connectionRequestServerInit();
    void contextInit();

    Tr69Client *m_pParent;
};

#endif
