#include <QtCore/QDebug>
#include <QtCore/QSocketNotifier>
#include <QtNetwork/QHostAddress>
#include <QtSoapMessage>

#include "Acs.h"
#include "AcsClient.h"
#include "AcsConnection.h"
#include "AcsRequestReboot.h"
#include "AcsRequestRebootPrivate.h"

/**
  @class AcsRequestReboot
*/

AcsRequestReboot::AcsRequestReboot(AcsClient &client) : AcsRequest(client)
{
  m_pPrivate = new AcsRequestRebootPrivate(this, client);
};

AcsRequestReboot::~AcsRequestReboot()
{
  delete m_pPrivate;
};

void AcsRequestReboot::start(AcsConnection &connection)
{
  m_pPrivate->start(connection);
};

void AcsRequestReboot::evSocketError()
{
  m_pPrivate->evSocketError();
}

void AcsRequestReboot::evHttpPost()
{
  m_pPrivate->evHttpPost();
}

void AcsRequestReboot::evInformRequest()
{
  m_pPrivate->evInformRequest();
}

void AcsRequestReboot::evRebootResponse()
{
  m_pPrivate->evRebootResponse();
}

