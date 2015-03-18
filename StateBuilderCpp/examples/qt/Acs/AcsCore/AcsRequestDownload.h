#ifndef ACSREQUESTDOWNLOAD_H
#define ACSREQUESTDOWNLOAD_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtNetwork/QAbstractSocket>

#include "AcsRequest.h"

class QHttp;
class QtSoapMessage;
class Acs;
class AcsConnection;
class AcsClient;
class AcsRequestDownloadContext;



/**
@class AcsRequestDownload
*/

class AcsRequestDownload : public AcsRequest
{
    Q_OBJECT

  public:
    AcsRequestDownload(AcsClient &client, AcsParamDownload& paramDownload);
    virtual ~AcsRequestDownload();

    void start(AcsConnection &connection);

    virtual void evHttpPost();
    virtual void evInformRequest();
    virtual void evDownloadResponse();
    virtual void evSocketError();
    virtual void evTransferComplete();

    AcsParamDownload& getParam() {return m_paramDownload;}

  protected slots:

  protected:
    AcsRequestDownloadContext *m_pContext;
    AcsParamDownload& m_paramDownload;
};

#endif
