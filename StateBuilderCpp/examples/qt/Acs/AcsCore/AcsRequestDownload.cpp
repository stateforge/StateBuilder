#include <QtCore/QDebug>
#include <QtCore/QSocketNotifier>
#include <QtNetwork/QHostAddress>
#include <QtSoapMessage>

#include "Acs.h"
#include "AcsClient.h"
#include "AcsConnection.h"
#include "AcsRequestDownload.h"
#include "AcsRequestDownloadFsm.h"
#include <SmQtObserver.h>


/**
  @class AcsRequestDownload
*/

AcsRequestDownload::AcsRequestDownload(AcsClient &client, AcsParamDownload& paramDownload) : 
  AcsRequest(client) , 
  m_paramDownload(paramDownload)
{
  m_pContext = new AcsRequestDownloadContext(*this, this);
  m_pContext->SetObserver(SmQtObserver::GetInstance());
  //TODO HEEFRE
  //QObject::connect(m_pContext,
  //                 SIGNAL(end()),
  //                 this,
  //                 SLOT(contextEnd()));
};

AcsRequestDownload::~AcsRequestDownload()
{
};

void AcsRequestDownload::start(AcsConnection &connection)
{
  qDebug() << "AcsRequestDownload\tstart";
  m_pContext->evStart(connection);
};

void AcsRequestDownload::evSocketError()
{
  m_pContext->evSocketError();
}

void AcsRequestDownload::evHttpPost()
{
  m_pContext->evHttpPost();
}

void AcsRequestDownload::evInformRequest()
{
  m_pContext->evInformRequest();
}

void AcsRequestDownload::evDownloadResponse()
{
  m_pContext->evDownloadResponse();
}

void AcsRequestDownload::evTransferComplete()
{
  m_pContext->evTransferComplete();
}
