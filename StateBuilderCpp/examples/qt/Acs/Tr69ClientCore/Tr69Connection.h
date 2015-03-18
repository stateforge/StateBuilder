#ifndef TR69CONNECTION_H
#define TR69CONNECTION_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkReply>

#include <Tr69ConnectionBase.h>

class QtSoapMessage;
class Tr69ConnectionPrivate;

/**
@class Tr69ConnectionConf
*/
class Tr69ConnectionConf
{
public:
    Tr69ConnectionConf() :
      requestTimeout(15000){}
    long requestTimeout;
};

/**
@class Tr69Connection
*/
class Tr69Connection : public Tr69ConnectionBase
{
    Q_OBJECT
    
public:
    Tr69Connection(QUrl &acsUrl, QObject *pParent = NULL);
    ~Tr69Connection();

    //void doSendInformRequest();

    QUrl &acsUrl() const;
    const QString& getErrorMessage() const ;

    Tr69ConnectionConf &conf() const;

public slots:
    void sendInformRequest();

signals:
    void finished();

private:
    friend class Tr69ConnectionPrivate;
    Tr69ConnectionPrivate *m_pPrivate;
};

#endif
