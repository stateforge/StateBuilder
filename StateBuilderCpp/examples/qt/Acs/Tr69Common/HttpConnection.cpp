
#include <QtCore/QDebug>
#include <QtCore/QSocketNotifier>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QHttp>
#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QHttpResponseHeader>

#include <QsLog.h>

#include "HttpParser.h"
#include "HttpConnection.h"

// Hope 20 MB is enough
const int CONTENTLENGTH_MAX = 20 * 1024 * 1025; 

/**
  @class HttpConnection
*/
HttpConnection::HttpConnection(QTcpSocket* pTcpSocket, QObject *pParent) : QObject(pParent),
  m_pHttpParser(NULL),
  m_pTcpSocket(pTcpSocket),
  m_bytesRead(0)
{
  QLOG_DEBUG();
  initSignalSlot();
  m_pTcpSocket->setParent(this);
}

HttpConnection::~HttpConnection()
{
  QLOG_DEBUG() << "socket state " << m_pTcpSocket->state();
  delete m_pHttpParser;
}

void HttpConnection::initSignalSlot()
{
  connect(m_pTcpSocket, SIGNAL(readyRead()),
             this, SLOT(readyRead()));
}

HttpParser* HttpConnection::getHttpParser()
{
  if(!m_pHttpParser){
    m_pHttpParser = new HttpParser();
  }
  return m_pHttpParser;
}

void HttpConnection::httpParserReset()
{
  delete m_pHttpParser;
  m_pHttpParser = NULL;
}

void HttpConnection::readyRead()
{
  qint64 bytesAvailable = m_pTcpSocket->bytesAvailable();
  QByteArray payload(bytesAvailable, 0);
  qint64 bytesRead = m_pTcpSocket->read(payload.data(), bytesAvailable);
  if(bytesRead == -1){
      QLOG_WARN() << "Error while reading socket: " << m_pTcpSocket->errorString();
    this->deleteLater();
    return;
  } else if(bytesRead == 0){
    QLOG_WARN() << "no more byte to read";
    return;
  } 

  m_bytesRead += bytesRead;

  //qDebug() << "HttpConnection::readyRead() bytesAvailable " << bytesAvailable << ", total " << m_bytesRead;
  HttpParser* pHttpParser = getHttpParser();

  HttpParser::status_t eStatus = pHttpParser->addBytes(payload);
  switch(eStatus){
    case HttpParser::Incomplete:
      return;
      break;
    case HttpParser::Error:
      QLOG_WARN() << "Http parsing error";
      //TODO HEEFRE 
      //this->deleteLater();
      return;
      break;
    case HttpParser::Done:
      //qWarning() << "Http parsing done";
      break;
    default:
      Q_ASSERT(0);
  }

  int iContentLength = pHttpParser->getContentLength();

  //qDebug() << "URI: " << pHttpParser->getUri();
  QLOG_DEBUG() << "User-Agent: " << pHttpParser->getValue("User-Agent");
  QLOG_DEBUG() << "Content-Type: " << pHttpParser->getValue("Content-Type");
  QLOG_DEBUG() << "Content-Length: " << iContentLength;

  if((iContentLength < 0) || (iContentLength > CONTENTLENGTH_MAX)){
      QLOG_WARN() << "Invalid Content-Length: " << iContentLength;
    //TODO send error
    this->deleteLater();
    return;
  }

  QLOG_DEBUG() << "Uri " << pHttpParser->getUri();
  
  emit packetReceived();

}
void HttpConnection::close()
{
    m_pTcpSocket->close();
}

void HttpConnection::sendHttpResponse(int code, const QString& status)
{
  QLOG_DEBUG() << "code " << code << ", status " << status;
  httpParserReset();

  QHttpResponseHeader responseHeader;
  buildHttpHeaderResponse(responseHeader, 0, code, status);
  QLOG_DEBUG() << responseHeader.toString();
  m_pTcpSocket->write(responseHeader.toString().toUtf8().constData());
}

void HttpConnection::buildHttpHeaderResponse(QHttpResponseHeader &responseHeader,
                                            int iContentLength,
                                            int code, 
                                            const QString& status)
{
  responseHeader.setStatusLine(code, status);
  responseHeader.setContentLength(iContentLength);
  if(iContentLength == 0){
    responseHeader.setContentType("text/plain");
  } else {
    responseHeader.setContentType("text/xml; charset=\"utf-8\"");
  }
  return;
}
