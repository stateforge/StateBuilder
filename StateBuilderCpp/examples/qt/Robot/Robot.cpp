
#include <iostream>

#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <QtCore/QDir>


#include <QsLog.h>
#include <QsLogDest.h>

#include <SmQtObserver.h>

#include "Robot.h"
#include "Arm.h"
#include "Camera.h"
#include <RobotFsm.h> // Generated header

Robot::Robot() 
{
    m_pArm = new Arm();
    m_pCamera = new Camera();

    logInit();
    // Generated RobotContext
    m_pRobotContext = new RobotContext(*m_pCamera, *m_pArm);
    m_pRobotContext->SetObserver(SmQtObserver::GetInstance());
    QObject::connect(m_pRobotContext,
                     SIGNAL(end()),
                     this,
                     SLOT(contextEnd()));

    QObject::connect(m_pArm,
                     SIGNAL(calibrationDone()),
                     m_pRobotContext,
                     SLOT(evArmCalibrationDone()));

    QObject::connect(m_pCamera,
                     SIGNAL(calibrationDone()),
                     m_pRobotContext,
                     SLOT(evCameraCalibrationDone()));

}

Robot::~Robot()
{
    QLOG_DEBUG();
    delete m_pRobotContext;
    delete m_pArm;
    delete m_pCamera;
}

void Robot::logInit()
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

void Robot::contextEnd()
{
    QCoreApplication::exit(0);
}

void Robot::start()
{
    QLOG_INFO();
    m_pRobotContext->evCalibrateRequest();
}

void Robot::stop()
{
    QLOG_INFO();
    m_pRobotContext->evStop();
}


#include "Robot.moc"
