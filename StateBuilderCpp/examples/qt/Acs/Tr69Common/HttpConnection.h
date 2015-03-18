#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include <memory>
#include <QtCore/QObject>

#include <QtCore/QUrl>
#include <QtNetwork/QAbstractSocket>

class QTcpServer;
class QTcpSocket;
class QHttp;
class QAuthenticator;
class QHttpResponseHeader;

class HttpParser;

/**
  @class HttpConnection
*/
class HttpConnection : public QObject
{
    Q_OBJECT

  public:
    HttpConnection(QTcpSocket *pTcpSocket, QObject *pParent = NULL);
    virtual ~HttpConnection();

    void sendHttpResponse(int code = 200, const QString& status = "OK");
    void close();

Q_SIGNALS:
    void packetReceived();

  private slots:
    void readyRead();

  private:
    HttpParser *m_pHttpParser;
    QTcpSocket *m_pTcpSocket;
    quint64    m_bytesRead;

    HttpParser* getHttpParser();
    void httpParserReset();

    void initSignalSlot();

    void buildHttpHeaderResponse(QHttpResponseHeader &responseHeader, 
                                 int iContentLength, 
                                 int code = 200, 
                                 const QString& status = "OK");
};

#endif
