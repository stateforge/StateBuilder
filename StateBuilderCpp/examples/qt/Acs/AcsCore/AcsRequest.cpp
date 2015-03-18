#include <QtCore/QDebug>
#include <QtCore/QSocketNotifier>
#include <QtNetwork/QHostAddress>
#include <QtSoapMessage>

#include <QsLog.h>
#include <SmQtObserver.h>

#include "AcsClient.h"
#include "AcsRequest.h"
#include "Acs.h"
#include "AcsConnection.h"

/**
  @class AcsRequest
*/

AcsRequest::AcsRequest(AcsClient &client) :
  m_client(client),
  m_pRxMessage(NULL)
{
  QLOG_DEBUG();
  m_client.requestAdd(this);
  connect(this, SIGNAL(end()),
          &client, SLOT(onRequestEnd()));
}

AcsRequest::~AcsRequest()
{
  QLOG_DEBUG();
}

void AcsRequest::setRxMessage(QtSoapMessage *pRxMessage) 
{
  m_pRxMessage = pRxMessage;
};

void AcsRequest::onEnd()
{
    emit end();
}

/**
  @class AcsRequestSet
*/
AcsRequestSet::AcsRequestSet(AcsClient &client) : AcsRequest(client)
{
}

/**
  @class ParamGetNames
*/
ParamGetNames::ParamGetNames() :
  parameterPath("InternetGatewayDevice.")
, nextLevel(1)
{
};

/**
  @class AcsRequestGetName
*/
void AcsRequestGetNames::start(AcsConnection &connection)
{
  connection.sendRequestGetName();
};
