#include <QtCore/QObject>
#include <QtTest/QtTest>

#include "Acs.h"
#include "Tr69Client.h"
#include "Tr69Connection.h"
#include "Tr69ConnectionRequestServer.h"

class AcsTest : public QObject {
  
  Q_OBJECT 
  
  public:
    AcsTest();

  public slots:

    //Acs
    void onClientEnd(AcsClientToken&, bool);
    void onClientNew(const AcsClientToken &clientToken);
  
    //Tr69Connection
    void tr69ConnectionFinished();

  private slots:
    void testConnectionRequest();
    void testInformRequest();
    
    //void testConnectionRequestTimeout();
    //void testConnectionRequestPortClosed();

  private:
    void logInit();
    void acsInit();
    void createClient();
    void tr69ClientInit();
    void tr69ConnectionInit();
    void connectionRequestServerInit();
    void testCleanUp();
    bool m_bErrorExpected;
    QString m_messageExpected;

    Tr69Client *m_pTr69Client;
    Tr69Connection *m_pTr69Connection;
    Tr69ConnectionRequestServer *m_pConnectionRequestServer;
    QUrl m_acsUrl;
    Acs *m_pAcs;
    AcsClientToken m_clientToken;
};

