#ifndef ACSREQUESTREBOOTPRIVATE_H
#define ACSREQUESTREBOOTPRIVATE_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtNetwork/QAbstractSocket>

#include "AcsClient.h"
#include "AcsRequest.h"
#include "AcsRequestReboot.h"

class QHttp;
class QtSoapMessage;
class Acs;
class AcsConnection;
class AcsRequestRebootContext;

/**
  @class AcsRequestReboot
*/

class AcsRequestRebootPrivate : public QObject
{
    Q_OBJECT

  public:
    AcsRequestRebootPrivate(AcsRequestReboot *pAcsRequestReboot, AcsClient &client);
    virtual ~AcsRequestRebootPrivate();

    void start(AcsConnection &connection);

    virtual void evHttpPost();
    virtual void evInformRequest();
    virtual void evRebootResponse();
    virtual void evSocketError();

    AcsClient& client(){return m_pParent->m_client;} 

  protected slots:

  protected:
    AcsRequestReboot *m_pParent;
    AcsRequestRebootContext *m_pContext;
};


#endif
