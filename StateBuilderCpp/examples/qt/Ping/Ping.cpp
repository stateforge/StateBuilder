
#include <iostream>

#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <QtCore/QDir>

#include <QsLog.h>
#include <QsLogDest.h>

#include <SmQtObserver.h>

#include "Ping.h"
#include "UdpEchoClient.h"
#include <PingFsm.h> // Generated header

namespace stateforge { namespace network { 

Ping::Ping() :
  m_iRx(0),
  m_iTx(0),
  m_iCount(5),
  m_lTimeout(1000)
{
    logInit();
    m_pUdpEchoClient = new UdpEchoClient(this);
    QObject::connect(m_pUdpEchoClient,
                     SIGNAL(reply()),
                     this,
                     SLOT(onReply()));

    // Generated PingContext
    m_pPingContext = new PingContext(*this, this);
	m_pPingContext->SetObserver(SmQtObserver::GetInstance());
    m_pPingContext->EnterInitialState();
    
    QObject::connect(m_pPingContext,
                     SIGNAL(end()),
                     this,
                     SLOT(contextEnd()));
}

Ping::~Ping()
{
    QLOG_INFO();
}

void Ping::send()
{
    m_iTx++;
    m_pUdpEchoClient->send();
}

void Ping::logInit()
{
   QCoreApplication *pApp = QCoreApplication::instance();
   QsLogging::Logger& logger = QsLogging::Logger::instance();
   logger.setLoggingLevel(QsLogging::TraceLevel);
   //Log to file
   const QString logPath(QDir(pApp->applicationDirPath()).filePath("Log.txt"));
   QsLogging::DestinationPtr fileDestination(
      QsLogging::DestinationFactory::MakeFileDestination(logPath) );
   logger.addDestination(fileDestination);
   //Log to standard output
   QsLogging::DestinationPtr debugDestination = QsLogging::DestinationFactory::MakeDebugOutputDestination();
   logger.addDestination(debugDestination);
}

void Ping::onReply()
{
   m_iRx++;
   m_pPingContext->EvPingReply();
}

void Ping::contextEnd()
{
    QLOG_INFO();
    printStatistics();
    QCoreApplication::exit(0);
}

void Ping::start()
{
    QLOG_INFO();
    m_pPingContext->EvStart();
}

void Ping::printStatistics()
{
    QLOG_INFO() << ": " << m_iRx << "/" <<  m_iTx;

}

#include "Ping.moc"

}} // namespace stateforge { namespace network { 
