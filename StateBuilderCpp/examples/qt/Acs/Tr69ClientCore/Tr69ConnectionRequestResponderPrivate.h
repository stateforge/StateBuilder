#ifndef TR69CONNECTIONREQUESTRESPONDERPRIVATE_H
#define TR69CONNECTIONREQUESTRESPONDERPRIVATE_H

#include <QtCore/QObject>
#include <QtCore/QQueue>
#include <QtCore/QDebug>
#include <QtCore/QLinkedList>
#include <QtCore/QTimerEvent>

#include <qhttpconnection.h>
#include "Tr69ConnectionRequestResponder.h"
#include "Tr69ConnectionRequestResponderFsm.h"

class Tr69ConnectionRequestResponderPrivate : public QObject {

    Q_OBJECT

public:
    Tr69ConnectionRequestResponderPrivate(QHttpRequest *pRequest, 
                                          QHttpResponse *pResponse,
                                          Tr69ConnectionRequestResponder *pParent);
    ~Tr69ConnectionRequestResponderPrivate();

    Tr69ConnectionRequestResponderContext *m_pContext;
    Tr69ConnectionRequestResponderConf m_conf;

    QHttpRequest *m_pRequest;
    QHttpResponse *m_pResponse;

    QString m_errorMessage;
    int m_iErrorCode;

public slots:
    void start();
    void stop();

private slots:
    // Tr69ConnectionRequestResponderContext slots
    void contextEnd();
    //QHttpRequest
    void accumulateRequest(const QByteArray &data);
    void endRequest();
    //QHttpResponse
    void doneResponse();

private:
    void contextInit();

    Tr69ConnectionRequestResponder *m_pParent;
};

#endif
