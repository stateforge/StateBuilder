
#include <iostream>

#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtNetwork/QHttp>
#include <QtNetwork/QFtp>

#include <QsLog.h>
#include <QsLogDest.h>

#include <SmQtObserver.h>

#include "UrlGet.h"
#include <UrlGetFsm.h> // Generated header

UrlGet::UrlGet() :
        m_pHttp(NULL),
        m_httpHost("qt.nokia.com"),
        m_httpFile("/"),
        m_pFtp(NULL),
        m_ftpHost("ftp.qt.nokia.com"),
        m_ftpFile("INSTALL")
{
    logInit();
    // Generated UrlGetContext
    m_pUrlGetContext = new UrlGetContext(*this, this);
    m_pUrlGetContext->SetObserver(SmQtObserver::GetInstance());
    QObject::connect(m_pUrlGetContext,
                     SIGNAL(end()),
                     this,
                     SLOT(contextEnd()));

}

UrlGet::~UrlGet()
{
    QLOG_DEBUG();
}

void UrlGet::logInit()
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

void UrlGet::contextEnd()
{
    QCoreApplication::exit(0);
}

void UrlGet::start()
{
    QLOG_INFO();
    m_pUrlGetContext->evStart();
}

QHttp* UrlGet::getHttp()
{
  if(m_pHttp == NULL){
    httpInit();
  }
  return m_pHttp;
};

void UrlGet::httpInit()
{
    if (m_pHttp != NULL) {
        delete m_pHttp;
    }
    m_pHttp = new QHttp(this);
    connect(m_pHttp, SIGNAL(done(bool)), this, SLOT(httpDone(bool)));
    connect(m_pHttp, SIGNAL(stateChanged(int)), this, SLOT(httpStateChange(int)));
}

void UrlGet::httpStart()
{
    QLOG_INFO() << "host: " << m_httpHost << ", file: " << m_httpFile;
    getHttp()->setHost(m_httpHost);
    getHttp()->get(m_httpFile);
}

void UrlGet::httpStateChange(int state)
{
    QLOG_INFO() << "state " << state;
}

void UrlGet::httpDone(bool error)
{
    if (error == true) {
        QLOG_INFO() << "error: " << getHttp()->errorString();
        m_pUrlGetContext->evHttpDoneError();
    } else {
        QLOG_INFO() << "ok";
        m_pUrlGetContext->evHttpDone();
    }
}

QFtp* UrlGet::getFtp()
{
  if(m_pFtp == NULL){
    ftpInit();
  }
  return m_pFtp;
};

void UrlGet::ftpInit()
{
    if (m_pFtp != NULL) {
        delete m_pFtp;
    }
    m_pFtp = new QFtp(this);
    connect(m_pFtp, SIGNAL(done(bool)), this, SLOT(ftpDone(bool)));
    connect(m_pFtp, SIGNAL(stateChanged(int)), this, SLOT(ftpStateChange(int)));
}

void UrlGet::ftpDone(bool error)
{
    if (error == true) {
        QLOG_INFO() << error << getFtp()->errorString();
        m_pUrlGetContext->evFtpDoneError();
    } else {
        QLOG_INFO() << "ok";
        m_pUrlGetContext->evFtpDone();
    }
}

void UrlGet::ftpStart()
{
    QLOG_INFO() << "host: " << m_ftpHost << ", file: " << m_ftpFile;
    getFtp()->connectToHost(m_ftpHost);
    getFtp()->login();
    getFtp()->cd("qt");
    getFtp()->get(m_ftpFile);
    getFtp()->close();
}

void UrlGet::ftpStateChange(int state)
{
    QLOG_INFO() << "state " << state; 
}

#include "UrlGet.moc"
