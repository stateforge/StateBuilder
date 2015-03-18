#ifndef UDPECHOCLIENT_H
#define UDPECHOCLIENT_H

#include <QtCore/QObject>
#include <QtNetwork/QHostAddress>

class QUdpSocket;

namespace stateforge { namespace network {

class UdpEchoClient : public QObject
{
    Q_OBJECT
public:
    UdpEchoClient(QObject *pParent = NULL); 
    virtual ~UdpEchoClient();
    void send();

signals:
    void reply();

private slots:
    void onReply();

private:
    QUdpSocket *m_pUdpSocket;
    QHostAddress m_address;
    int m_port;
    QByteArray m_datagram;
};

}} // namespace stateforge { namespace network { 

#endif // UDPECHOCLIENT_H
