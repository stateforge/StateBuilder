#ifndef ACSCONNECTIONPRIVATE_H
#define ACSCONNECTIONPRIVATE_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>

#include <QtCore/QUrl>

#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QHostAddress>

#include <qhttpserver.h>
#include <qhttprequest.h>
#include <qhttpresponse.h>
#include <qhttpconnection.h>

#include <Tr69ConnectionBase.h>
#include <AcsMessageBuilder.h>
#include <AcsMessageParam.h>

#include "AcsConnection.h"
#include "AcsClient.h"
#include "AcsParser.h"

class AcsConnectionContext;

/**
  @class AcsConnection
*/
class AcsConnectionPrivate : public Tr69ConnectionBase
{
    Q_OBJECT

  public:
    AcsConnectionPrivate(AcsConnection *pAcsConnection, 
                         QHttpConnection *pQHttpConnection);
    ~AcsConnectionPrivate();

    void sendInformResponse();
    void sendRequestGetName();
    void sendRequestSetValue(AcsParamList &paramList);
    void sendRequestGetValue(AcsParamList &paramList);
    void sendRequestReboot();
    void sendRequestDownload(AcsParamDownload &paramDownload);
    void sendTransferCompleteResponse();

    void sendHttpResponse(int code = 200, const QString& status = "OK");

    AcsConnectionContext *m_pContext;
    QHttpConnection *m_pHttpConnection;
    //TODO HEEFRE do we need these anymore ?
    QHttpRequest *m_pRequest;
    QHttpResponse *m_pResponse;
    AcsParser *m_pParser;

  private:

    void requestResponseInit();
    void parserInit();
    void httpConnectionInit();
    
    void sendSoapMessage(QtSoapMessage &message);

  private slots:
    //QHttpRequest
    void readyRead(const QByteArray &buffer);
    void endRequest();
    //QHttpResponse
    void doneResponse();
    //QHttpConnection
    void httpConnectionNewRequest(QHttpRequest *pRequest, QHttpResponse *pResponse);
    //AcsParser
    void parserError();
    void onInformRequest(QtSoapMessage *pRxMessage);

    //TODO HEEFRE implement these
    void onHttpPost();
    
    void onGetNameResponse(QtSoapMessage *pRxMessage);
    void onSetValueResponse(QtSoapMessage *pRxMessage);
    void onGetValueResponse(QtSoapMessage *pRxMessage);
    void onTransferComplete(QtSoapMessage *pRxMessage);
    void onDownloadResponse(QtSoapMessage *pRxMessage);
    void onRebootResponse(QtSoapMessage *pRxMessage);
    void onFault(QtSoapMessage *pRxMessage);

private:
    AcsMessageBuilder *m_pMessageBuilder;
    AcsConnection *m_pParent;
};


#endif
