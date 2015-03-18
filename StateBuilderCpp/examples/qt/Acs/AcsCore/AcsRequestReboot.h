#ifndef ACSREQUESTREBOOT_H
#define ACSREQUESTREBOOT_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtNetwork/QAbstractSocket>

#include "AcsRequest.h"

class QHttp;
class QtSoapMessage;
class Acs;
class AcsConnection;
class AcsClient;
class AcsRequestRebootPrivate;


/**
  @class AcsRequestReboot
*/

class AcsRequestReboot : public AcsRequest
{
  Q_OBJECT

  friend class AcsRequestRebootPrivate;

  public:
    AcsRequestReboot(AcsClient &client);
    virtual ~AcsRequestReboot();

    void start(AcsConnection &connection);

    virtual void evHttpPost();
    virtual void evInformRequest();
    virtual void evRebootResponse();
    virtual void evSocketError();

  protected:
    AcsRequestRebootPrivate *m_pPrivate;
};


#endif
