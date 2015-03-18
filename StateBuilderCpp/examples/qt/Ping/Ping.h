#ifndef PING_H
#define PING_H

#include <QtCore/QObject>

namespace stateforge { namespace network { 

class PingContext; // Generated class
class UdpEchoClient;

class Ping : public QObject
{
    Q_OBJECT
    
public:
    Ping();
    virtual ~Ping();

    int getTx() const {return m_iTx;}
    int getCount() const {return m_iCount;}
    long getTimeout() const {return m_lTimeout;}
    void send();
    
public slots:
    void start();
    void onReply();
  
private slots:
    void contextEnd();

private:
    PingContext *m_pPingContext; // Generated class
    UdpEchoClient *m_pUdpEchoClient;
    
    int m_iRx; // Number of packet received
    int m_iTx; // Number of packet already sent
    int m_iCount; //Total number of packet to send
    long m_lTimeout;// Timeout in msec between 2 packets
    void logInit();
    void printStatistics();
};

}} // namespace stateforge { namespace network { 

#endif // PING_H
