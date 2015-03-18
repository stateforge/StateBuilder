#ifndef ACSPRIVATE_H
#define ACSPRIVATE_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QLinkedList>
#include <QtCore/QUrl>

#include <qhttpserver.h>
#include <qhttprequest.h>
#include <qhttpresponse.h>
#include <qhttpconnection.h>
#include "Acs.h"

class AcsConnection;
class AcsRequest;
class AcsClient;
class AcsParamDownload;

/**
  @class Acs
*/
class AcsPrivate : public QObject
{
    Q_OBJECT

  public:
    AcsPrivate(Acs *pAcs);
    ~AcsPrivate();

    bool clientCreate(AcsClientToken &clientToken, const QUrl &url);
    bool clientDestroy(AcsClientToken &clientToken);
    bool clientStart(const AcsClientToken &clientToken);
    const QString& clientGetError(const AcsClientToken &clientToken);
    
    void addRequestReboot(const AcsClientToken &clientToken);
    void addRequestDownload(const AcsClientToken &clientToken, AcsParamDownload &paramDownload);
    
    bool listen();

    // Connection 
    QLinkedList<AcsConnection*> m_connectionList;
    friend class AcsConnection;
    bool connectionAdd(AcsConnection* pConnection);
    bool connectionRemove(AcsConnection* pConnection);

    //Client
    AcsClient* getClient(const AcsClientToken &clientToken);
    QMap<AcsClientToken, AcsClient*> m_clientMap;
    friend class AcsClient;

    //QHttpServer
    QHttpServer *m_pHttpServer;
    quint16 m_wPortListen;

  private slots:
    // QHttpServer
    void httpServerNewIncomingConnection(QHttpConnection *pQHttpConnection);
    //AcsClient
    void onClientEnd(AcsClientToken &clientToken, bool bError);
    //AcsConnection
    void onConnectionDestroyed();

  private:
    Acs  *m_pParent;

    void httpServerInit();
    void connectionCreate(AcsClient* pClient, QHttpConnection *pQHttpConnection);
};

#endif
