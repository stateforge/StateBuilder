#include <QtCore/QDebug>
#include <QtCore/QSocketNotifier>
#include <QtNetwork/QHostAddress>
#include <QtSoapMessage>

#include "Acs.h"
#include "AcsClient.h"
#include "AcsConnection.h"
#include "AcsRequestRebootPrivate.h"
#include "AcsRequestRebootFsm.h"
#include <SmQtObserver.h>

/**
  @class AcsRequestReboot
*/

AcsRequestRebootPrivate::AcsRequestRebootPrivate(AcsRequestReboot *pAcsRequestReboot, AcsClient &client) : 
 m_pParent(pAcsRequestReboot)
{
  m_pContext = new AcsRequestRebootContext(*this, this);
  m_pContext->SetObserver(SmQtObserver::GetInstance());
  QObject::connect(m_pContext,
                   SIGNAL(end()),
                   this->m_pParent,
                   SLOT(onEnd()));
};

AcsRequestRebootPrivate::~AcsRequestRebootPrivate()
{
};

void AcsRequestRebootPrivate::start(AcsConnection &connection)
{
  qDebug() << "AcsRequestRebootPrivate\tstart";
  m_pContext->evStart(connection);
};

void AcsRequestRebootPrivate::evSocketError()
{
  m_pContext->evSocketError();
}

void AcsRequestRebootPrivate::evHttpPost()
{
  m_pContext->evHttpPost();
}

void AcsRequestRebootPrivate::evInformRequest()
{
  m_pContext->evInformRequest();
}

void AcsRequestRebootPrivate::evRebootResponse()
{
  m_pContext->evRebootResponse();
}

