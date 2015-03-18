#ifndef UDPEHOSERVER_H
#define UDPEHOSERVER_H

#include <QtCore/QObject>

class QUdpSocket;

namespace stateforge { namespace network { 

class UdpEchoServer : public QObject
{
    Q_OBJECT
    
public:
    UdpEchoServer();
    virtual ~UdpEchoServer();
    
public slots:
    bool start();

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket *m_pUdpSocket;
    int m_port;
};

}} // namespace stateforge { namespace network { 

#endif // UDPEHOSERVER_H
