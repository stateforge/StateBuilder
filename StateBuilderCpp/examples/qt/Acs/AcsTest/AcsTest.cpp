#include <QtCore/QDebug>
#include <QtCore/QDir>

#include <QtTest/QtTest>
#include <QsLog.h>
#include <QsLogDest.h>

#include "AcsTest.h"

using namespace std;

AcsTest::AcsTest() : 
m_pTr69Client(NULL),
m_pTr69Connection(NULL),
m_pConnectionRequestServer(NULL),
m_acsUrl("http://127.0.0.1:7547/"),
m_pAcs(NULL)
{
    logInit();
}

void AcsTest::acsInit()
{
    if(m_pAcs != NULL){
        delete m_pAcs;
    }
    m_pAcs = new Acs();
    QObject::connect(m_pAcs, SIGNAL(clientEnd(AcsClientToken&, bool)),
        this, SLOT(onClientEnd(AcsClientToken&, bool)));
    QObject::connect(m_pAcs, SIGNAL(clientNew(const AcsClientToken &)),
        this, SLOT(onClientNew(const AcsClientToken &)));
}

void AcsTest::createClient()
{
    QUrl url("http://127.0.0.1");
    m_pAcs->clientCreate(m_clientToken, url);
}

void AcsTest::tr69ClientInit()
{
    if(m_pTr69Client != NULL){
        delete m_pTr69Client;
    }
    m_pTr69Client = new Tr69Client();
}

void AcsTest::tr69ConnectionInit()
{
    if(m_pTr69Connection != NULL){
        delete m_pTr69Connection;
    }

    m_pTr69Connection = new Tr69Connection(m_acsUrl);

    QObject::connect(m_pTr69Connection, SIGNAL(finished()),
        this, SLOT(tr69ConnectionFinished()));
}

void AcsTest::connectionRequestServerInit()
{
    if(m_pConnectionRequestServer != NULL){
        delete m_pConnectionRequestServer;
    }

    m_pConnectionRequestServer = new Tr69ConnectionRequestServer();

   // QObject::connect(m_pConnectionRequestServer, SIGNAL(finished()),
   //     this, SLOT(tr69ConnectionFinished()));
}

void AcsTest::testCleanUp()
{
    delete m_pTr69Connection; m_pTr69Connection = NULL;
    delete m_pTr69Client; m_pTr69Client = NULL;
    delete m_pAcs; m_pAcs = NULL;
    delete m_pConnectionRequestServer; m_pConnectionRequestServer = NULL;
}

void AcsTest::tr69ConnectionFinished()
{
    QLOG_INFO() << m_pTr69Connection->getErrorMessage();
    //QCoreApplication::instance()->exit(0);
}

void AcsTest::logInit()
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

void AcsTest::onClientNew(const AcsClientToken &clientToken)
{
    QLOG_INFO() << clientToken;
}

void AcsTest::onClientEnd(AcsClientToken& clientToken, bool bError)
{
    QLOG_INFO() << clientToken << ", error " << bError;

    //TODO check clientToken
    if(bError){
        QLOG_INFO() << "***********************************************************************";  
        QLOG_INFO() << "  FAILURE " << m_pAcs->clientGetError(clientToken);  
        QLOG_INFO() << "***********************************************************************";  
    } else {
        QLOG_INFO() << "***********************************************************************";  
        QLOG_INFO() << "                            SUCCESS                                    ";  
        QLOG_INFO() << "***********************************************************************";
    }

    //QVERIFY(bError == m_bErrorExpected);
    if(bError == true){
        //QVERIFY(m_pAcs->clientGetError(clientToken) == m_messageExpected);
    }

    m_pAcs->clientDestroy(clientToken);

    QCoreApplication::instance()->exit(0);
}

void AcsTest::testInformRequest()
{
    QLOG_INFO() << "***********************************************************************";  
    QLOG_INFO() << "                            testInformRequest                          ";  
    QLOG_INFO() << "***********************************************************************";
    m_bErrorExpected = false;
    acsInit();
    bool bSuccess = m_pAcs->listen();
    QVERIFY (bSuccess == true);
    tr69ConnectionInit();
    m_pTr69Connection->sendInformRequest();
    QCoreApplication::instance()->exec();
    testCleanUp();
}

#if 0
void AcsTest::testConnectionRequestTimeout()
{
    QLOG_INFO() << "***********************************************************************";  
    QLOG_INFO() << "                            testConnectionRequestTimeout               ";  
    QLOG_INFO() << "***********************************************************************";

    m_bErrorExpected = true;
    m_messageExpected = "Connection Request Timeout";
    acsInit();
    createClient();
    //tr69ClientInit();
    connectionRequestServerInit();

    m_pConnectionRequestServer->conf().httpResponseDelay = 200000;
    m_pConnectionRequestServer->conf().processingMaxDuration = m_pConnectionRequestServer->conf().httpResponseDelay + 2000;
    m_pConnectionRequestServer->start();

    m_pAcs->addRequestReboot(m_clientToken);
    m_pAcs->clientStart(m_clientToken);

    QCoreApplication::instance()->exec();
    testCleanUp();
}

void AcsTest::testConnectionRequestPortClosed()
{
    QLOG_INFO() << "***********************************************************************";  
    QLOG_INFO() << "                            testConnectionRequestPortClosed            ";  
    QLOG_INFO() << "***********************************************************************";
    m_bErrorExpected = true;
    m_messageExpected = "Connection refused (or timed out)";
    acsInit();
    tr69ClientInit();
    m_pTr69Client->stop();
    createClient();
    m_pAcs->addRequestReboot(m_clientToken);
    m_pAcs->clientStart(m_clientToken);
    QCoreApplication::instance()->exec();
    testCleanUp();
}
#endif

void AcsTest::testConnectionRequest()
{
    QLOG_INFO() << "***********************************************************************";  
    QLOG_INFO() << "                            testConnectionRequest                      ";  
    QLOG_INFO() << "***********************************************************************";

    QLOG_INFO();
    m_bErrorExpected = false;
    acsInit();
    tr69ClientInit();
    m_pTr69Client->start();

    createClient();
    m_pAcs->addRequestReboot(m_clientToken);
    m_pAcs->clientStart(m_clientToken);
    QCoreApplication::instance()->exec();
    testCleanUp();
}



QTEST_MAIN(AcsTest)
