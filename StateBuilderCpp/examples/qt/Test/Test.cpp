#include <QtCore/QDebug>
#include <QtCore/QDir>

#include <QtTest/QtTest>
#include <QsLog.h>
#include <QsLogDest.h>

#include <SmQtObserver.h>

#include "Action.h"
#include "Test.h"
#include "ParallelRootFsm.h"

using namespace std;

// The purpose of Management is to know when state machine enters the final state
// Management is OPTIONAL
class Management : public fsm::IManagement 
{
public:
  Management(): bEnd(false) {}
  //Implements fsm::IManagement::OnEnd()
  void OnEnd(){bEnd = true;}
  bool IsEnd() {return bEnd;}
  
private:
  bool bEnd;
};

void UniTest::testParallelRoot()
{
	Action action;
	ParallelRootContext context(action);
	context.SetObserver(SmQtObserver::GetInstance());

	//Find out when the state machine ends. Optional
    Management management;
    context.SetManagement(&management);

	QLOG_DEBUG() << "context.EnterInitialState()";
    context.EnterInitialState();

	QLOG_DEBUG() << "context.evStart()";
	context.evStart();

	QVERIFY(!management.IsEnd());

	QLOG_DEBUG() << "context.evStop();";
	context.evStop();

    QVERIFY(management.IsEnd());
}

UniTest::UniTest()
{
    logInit();
}

void UniTest::logInit()
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



QTEST_MAIN(UniTest)
