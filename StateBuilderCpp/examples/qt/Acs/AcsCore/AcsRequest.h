#ifndef ACSREQUEST_H
#define ACSREQUEST_H

#include <QtCore/QObject>
#include <QtCore/QLinkedList>
#include <QtCore/QUrl>
#include <QtNetwork/QAbstractSocket>

#include <AcsClient.h>
#include <AcsMessageBuilder.h>

class QtSoapMessage;
class Acs;
class AcsConnection;



/**
  @class AcsRequest
*/
class AcsRequest : public QObject
{
    Q_OBJECT

  public:
    AcsRequest(AcsClient &client);
    virtual ~AcsRequest();

    virtual void start(AcsConnection &connection) = 0;

    virtual void evSocketError(){};
    virtual void evHttpPost(){};
    virtual void evInformRequest(){};
    virtual void evRebootResponse(){};
    virtual void evDownloadResponse(){};
    virtual void evTransferComplete(){};
    virtual void evSetValueResponse(){};

    AcsClient& client(){return m_client;} 
    const QString& getErrorMessage() const {return m_errorMessage;} 
    
    void setRxMessage(QtSoapMessage *pRxMessage);

  public slots:
    void onEnd();

  signals:

    void end();
    
  protected:
    AcsClient &m_client;
    QtSoapMessage *m_pRxMessage;
    QString m_errorMessage;
};

/**
  @class AcsRequestSet
*/
class AcsRequestSet : public AcsRequest
{
    Q_OBJECT

  public:
    AcsRequestSet(AcsClient &client);

    virtual ~AcsRequestSet(){};

    AcsParamList& getParam() { return m_paramList;}

  private:
    AcsParamList  m_paramList;
};

/**
  @class  ParamGetNames
*/

class ParamGetNames
{
public:
  ParamGetNames();
  QString parameterPath;
  int nextLevel;
};

/**
  @class AcsRequestGetName
*/
class AcsRequestGetNames : public AcsRequest
{
    Q_OBJECT

  public:
    AcsRequestGetNames(AcsClient &client) : AcsRequest(client) {};
    virtual ~AcsRequestGetNames(){};

    ParamGetNames& getParam() { return m_param;}

    void start(AcsConnection &connection);

  private:
    ParamGetNames m_param;
};


#endif
