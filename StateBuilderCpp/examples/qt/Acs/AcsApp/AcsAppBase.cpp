#include <iostream>
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#include "AcsAppBase.h"

using namespace std;

AcsAppBase::AcsAppBase(const QString &host)
{
  QObject::connect(&m_acs, SIGNAL(clientEnd(AcsClientToken&, bool)),
                   this, SLOT(onClientEnd(AcsClientToken&, bool)));
                                    
  m_acs.clientCreate(m_clientToken, host);
}

AcsAppBase::~AcsAppBase()
{
  m_acs.clientDestroy(m_clientToken);
}

void AcsAppBase::start()
{
  m_acs.clientStart(m_clientToken);
}

void AcsAppBase::onClientEnd(AcsClientToken& clientToken, bool bError)
{
  qDebug() << "onClientEnd "  << clientToken << ", error ";
  if(clientToken != m_clientToken){
    qWarning() << "onClientEnd " << clientToken << " is not our client " << m_clientToken;
    return;
  }

  if(bError){
    cout << "***********************************************************************" << endl;  
    cout << "  FAILURE " << m_acs.clientGetError(clientToken).toStdString() << endl;  
    cout << "***********************************************************************" << endl;  
  } else {
    cout << "***********************************************************************" << endl;  
    cout << "                            SUCCESS                                    " << endl;  
    cout << "***********************************************************************" << endl;
  }
  QCoreApplication::instance()->exit(0);
}
