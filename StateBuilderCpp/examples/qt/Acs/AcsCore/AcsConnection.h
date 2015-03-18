#ifndef ACSCONNECTION_H
#define ACSCONNECTION_H

#include <QtCore/QObject>
#include <QtCore/QUrl>

#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>

#include <QtSoapMessage>
#include <AcsMessageParam.h>

class QHttpConnection;

class AcsConnectionPrivate;

/**
  @class AcsConnection
*/
class AcsConnection : public QObject
{
    Q_OBJECT

  public:
    AcsConnection(QHttpConnection *pQHttpConnection,
                  QObject *pParent = NULL);

    ~AcsConnection();

    void sendInformResponse();
    void sendRequestGetName();
    void sendRequestSetValue(AcsParamList &paramList);
    void sendRequestGetValue(AcsParamList &paramList);
    void sendRequestReboot();
    void sendRequestDownload(AcsParamDownload &paramDownload);
    void sendTransferCompleteResponse();

    void sendHttpResponse(int code = 200, const QString& status = "OK");

signals:
    void unknownRequest();
    //void socketError(QAbstractSocket::SocketError);
    void finished();

    void informRequestReceived(QtSoapMessage *pRxMessage);

  private:
      friend class AcsConnectionPrivate;
      AcsConnectionPrivate *m_pPrivate;
};

#endif
