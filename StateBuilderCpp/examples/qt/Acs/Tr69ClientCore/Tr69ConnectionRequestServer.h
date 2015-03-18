#ifndef TR69CONNECTIONREQUESTSERVER_H
#define TR69CONNECTIONREQUESTSERVER_H

#include <QtCore/QObject>

class Tr69ConnectionRequestServerPrivate;

/**
  * @class AcsClientConf
  */

class Tr69ConnectionRequestServerConf
{
public:
    Tr69ConnectionRequestServerConf() :
       processingMaxDuration(10000),
       httpResponseDelay(100),
       informRequestDelay(100){}
    long processingMaxDuration;
    long httpResponseDelay;
    long informRequestDelay;
};

/**
  * @class Tr69ConnectionRequestServer
  */
class Tr69ConnectionRequestServer : public QObject
{
  Q_OBJECT

  public:
    Tr69ConnectionRequestServer(QObject *pParent = NULL);
    ~Tr69ConnectionRequestServer();
    
    Tr69ConnectionRequestServerConf& conf();

  public slots:
    void start();
    void stop();

  Q_SIGNALS:
    void readyToSendInformRequest();

  private:
    friend class Tr69ConnectionRequestServerPrivate;
    Tr69ConnectionRequestServerPrivate *m_pPrivate;
};

#endif
