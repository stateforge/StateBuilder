#ifndef LOGINMVC_H
#define LOGINMVC_H

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtGui/QMainWindow>

#include <QtGui/QApplication>

#include <QtGui/QLabel>
#include <QtGui/QMessageBox>
#include "ui_frame.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslSocket>

#include <QsLog.h>
#include "LoginMvcFsm.h"

class AbstractModel : public QObject
{
    Q_OBJECT
public:
    AbstractModel(QObject* o = NULL) : QObject(o)
    {
    }
public slots:
    virtual void login(const QString & u, const QString & p) = 0;
signals:
    void loginComplete(bool);
};


class DummyModel : public AbstractModel
{
    Q_OBJECT
public:
    DummyModel(QObject* o = NULL) : AbstractModel(o)
    {
    }
public slots:
    void test()
    {
        QLOG_INFO();
        emit loginComplete(user == "user" && password == "password");
    }
    virtual void login(const QString & u, const QString & p)
    {
        QLOG_INFO() << "username " << u;
        user = u; password = p;
        QTimer::singleShot(5000,this,SLOT(test()));
    }
signals:
    void loginComplete(bool);
private:
    QString user,password;
    QNetworkAccessManager netAccess;
};
class GMailModel : public AbstractModel
{
    Q_OBJECT
public:
    GMailModel(QObject* o = NULL) : AbstractModel(o)
    {
    }
public slots:
    void loginFinished()
    {
        QNetworkReply* rep = qobject_cast<QNetworkReply*>(sender());
        rep->deleteLater();
        QLOG_INFO() << rep->error() << " " << rep->errorString();
        emit loginComplete(rep->error() == QNetworkReply::NoError);
    }
    virtual void login(const QString & u, const QString & p)
    {
        QNetworkRequest req;
        QUrl url("https://mail.google.com/mail/feed/atom");
        url.setUserName(u);
        url.setPassword(p);
        req.setUrl(url);
        QNetworkReply* reply = netAccess.get(req);
        reply->ignoreSslErrors();
        connect(reply,SIGNAL(finished()),this,SLOT(loginFinished()));
    }
private:
    QNetworkAccessManager netAccess;
};


class MyView : public QFrame, public virtual Ui::Frame
{
    Q_OBJECT
public:
    QString welcomeText;
    MyView(QWidget* o = NULL) : QFrame(o)
    {
        setupUi(this);
        connect(loginButton,SIGNAL(clicked()),this,SIGNAL(loginIntent()));
        connect(logoutButton,SIGNAL(clicked()),this,SIGNAL(logoutIntent()));
        connect(cancelButton,SIGNAL(clicked()),this,SIGNAL(cancelIntent()));
    }

public slots:

    void notifyLogin()
    {
        QLOG_INFO();
        QMessageBox::information(this,"Logged In","You are now logged in!");
        emit contIntent();
    }
    void notifyTimeout()
    {
        QLOG_INFO();
        QMessageBox::information(this,"Timeout...","Sorry, login has timed out");
        emit contIntent();
    }
    void notifyWelcome()
    {
        QLOG_INFO();
        QMessageBox::information(this,"Welcome!",welcomeText);
    }
    void notifyError()
    {
        QLOG_INFO();
        QMessageBox::warning(this,"Not Logged in","Login has failed...");
        emit contIntent();
        QLOG_INFO() << "done";
    }

    void notifyCancel()
    {
        QLOG_INFO();
        QMessageBox::information(this,"Cancel","You have cancelled");
        emit contIntent();
    }
    void notifyHello(const QString & name)
    {
        QLOG_INFO() << name;
        QMessageBox::information(this,"Welcome",QString("Hello ") + name);
    }

signals:
    void loginIntent();
    void logoutIntent();
    void cancelIntent();
    void contIntent();
};

class LoginMvc : public QMainWindow
{
    Q_OBJECT

public:
    LoginMvc(bool use_gmail);
    ~LoginMvc();
    void start();

    MyView* getView() const {return m_pView;};
    AbstractModel* getModel() const { return m_pModel;};

    const QString& getUsername() const { return m_username;};
    const QString& getPassword() const { return m_password;};
    int getLoginTimeout() const {return m_loginTimeout;};

    void login();

private slots:

    void setUsername(const QString& username) {m_username =  username;}
    void setPassword(const QString& password) {m_password =  password;}
    void setLoginTimeout(int loginTimeout) { m_loginTimeout = loginTimeout;}

    // Events from the View to the Fsm
    void evIntentLogin(){m_pContext->evIntentLogin();}
    void evCancelLogin(){m_pContext->evCancelLogin();}
    void evIntentContinue();
    void evIntentLogout(){m_pContext->evIntentLogout();}

    // Events from the Model to the Fsm
    void evLoginComplete(bool success){m_pContext->evLoginComplete(success);}

    // Called when the context ends
    void contextEnd();

private:

    void initSignalSlot();

    void logInit();

    // Generated class
    LoginMvcContext *m_pContext;

    AbstractModel* m_pModel;
    MyView* m_pView;

    int m_loginTimeout;
    bool firstTime;
    QString m_username;
    QString m_password;
};

#endif // LOGINMVC_H
