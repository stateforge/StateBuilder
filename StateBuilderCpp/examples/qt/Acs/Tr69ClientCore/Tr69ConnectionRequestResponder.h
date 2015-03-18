#ifndef TR69CONNECTIONREQUESTRESPONDER_H
#define TR69CONNECTIONREQUESTRESPONDER_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtNetwork/QAbstractSocket>

#include <qhttprequest.h>
#include <qhttpresponse.h>

class Tr69ConnectionRequestResponderPrivate;

/**
  * @class Tr69ConnectionRequestResponderConf
  */
class Tr69ConnectionRequestResponderConf
{
public:
    Tr69ConnectionRequestResponderConf() :
        maxProcessingDuration(10000),
        httpResponseDelay(100){}
    long maxProcessingDuration;
    long httpResponseDelay;
};

/**
  * @class Tr69ConnectionRequestResponder
  */
class Tr69ConnectionRequestResponder : public QObject
{
  Q_OBJECT

  public:
    Tr69ConnectionRequestResponder(QHttpRequest *pRequest, QHttpResponse *pResponse, QObject *pParent = NULL);
    ~Tr69ConnectionRequestResponder();
    
    Tr69ConnectionRequestResponderConf& conf();

    const QString& errorMessage() const;

  public slots:
    void start();
    void stop();

  Q_SIGNALS:
    void end();

  private:
    friend class Tr69ConnectionRequestResponderPrivate;
    Tr69ConnectionRequestResponderPrivate *m_pPrivate;
};

#endif
