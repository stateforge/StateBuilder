#include <QtCore/QObject>
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include <QsLog.h>
#include <QsLogDest.h>
#include <SmQtObserver.h>

#include "LoginMvc.h"

LoginMvc::LoginMvc(bool use_gmail):
        m_loginTimeout(10000),
        firstTime(true)
{
    logInit();

    //Model
    m_pModel = use_gmail ? (AbstractModel*)new GMailModel() : (AbstractModel*)new DummyModel();

    //View
    m_pView = new MyView();
    m_pView->welcomeText = (use_gmail  ?"Please enter your GMail user/password":"Username=user, Password=password");

    //State Machine
    m_pContext = new LoginMvcContext(*this, *m_pView, this);
    m_pContext->SetObserver(SmQtObserver::GetInstance());

    initSignalSlot();
    setCentralWidget(m_pView);
}

LoginMvc::~LoginMvc()
{
    QLOG_INFO() << "dtor";
}

void LoginMvc::logInit()
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

void LoginMvc::contextEnd()
{
    QLOG_INFO();
    QCoreApplication::exit(0);
}

void LoginMvc::initSignalSlot()
{
    //Signals from the view
    QObject::connect(m_pView->usernameEdit,SIGNAL(textChanged(QString)),this, SLOT(setUsername(QString)));
    QObject::connect(m_pView->passwordEdit,SIGNAL(textChanged(QString)),this ,SLOT(setPassword(QString)));
    QObject::connect(m_pView->timeoutSlider,SIGNAL(valueChanged(int))  ,this ,SLOT(setLoginTimeout(int)));

    QObject::connect(m_pView,SIGNAL(loginIntent()),this,SLOT(evIntentLogin()));
    QObject::connect(m_pView,SIGNAL(cancelIntent()),this,SLOT(evCancelLogin()));
    QObject::connect(m_pView,SIGNAL(contIntent()),this,SLOT(evIntentContinue()));
    QObject::connect(m_pView,SIGNAL(logoutIntent()),this,SLOT(evIntentLogout()));

    //Signals from the model
    QObject::connect(m_pModel,SIGNAL(loginComplete(bool)),
                     this, SLOT(evLoginComplete(bool)));

    //Signals from the state machine
    QObject::connect(m_pContext, SIGNAL(end()), this, SLOT(contextEnd()));
}

void LoginMvc::evIntentContinue()
{
    QLOG_INFO();
    m_pContext->evIntentContinue();
}

void LoginMvc::login()
{
    QLOG_INFO() << "username " << m_username << ", password " << m_password;
    m_pModel->login(m_username, m_password);
}
void LoginMvc::start()
{
    QLOG_INFO();
    m_pContext->evStart();
}

#include "LoginMvc.moc"
