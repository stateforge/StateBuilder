
#include <iostream>

#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <QtCore/QDir>

#include <QsLog.h>
#include <QsLogDest.h>

#include <SmQtObserver.h>

#include "TrafficLight.h"
#include "Light.h"
#include <TrafficLightFsm.h> // Generated header

TrafficLight::TrafficLight()
{
    logInit();
    m_pLight = new Light();
    // Generated TrafficLightContext
    m_pTrafficLightContext = new TrafficLightContext(*m_pLight, this);
    m_pTrafficLightContext->SetObserver(SmQtObserver::GetInstance());
    QObject::connect(m_pTrafficLightContext,
                     SIGNAL(end()),
                     this,
                     SLOT(contextEnd()));
}

TrafficLight::~TrafficLight()
{
    delete m_pLight;
}

void TrafficLight::logInit()
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

void TrafficLight::contextEnd()
{
    QCoreApplication::exit(0);
}

void TrafficLight::start()
{
    QLOG_INFO();
    m_pTrafficLightContext->EnterInitialState();
}

#include "TrafficLight.moc"
