
#include <QtNetwork/QUdpSocket>
#include <QsLog.h>
#include <QsLogDest.h>

#include <UdpEchoClient.h>

namespace stateforge { namespace network { 

UdpEchoClient::UdpEchoClient(QObject *pParent) : QObject(pParent),
  m_address(QString("127.0.0.1")),
  m_port(2012)
{
    QLOG_DEBUG();
    m_pUdpSocket = new QUdpSocket(this);

    if(m_pUdpSocket->bind(m_port, QUdpSocket::ShareAddress) == false){
        QLOG_ERROR() << "Cannot listen on port " << m_port << ", error " << m_pUdpSocket->errorString();
    }
    
    connect(m_pUdpSocket, SIGNAL(readyRead()), this, SLOT(onReply()));
    m_datagram.append("Ciao Biloute");
}

UdpEchoClient::~UdpEchoClient()
{
    QLOG_DEBUG();
}

void UdpEchoClient::onReply()
{
    QLOG_INFO();
    QByteArray datagram;
    QHostAddress address;
    quint16 port;
    
    datagram.resize(m_pUdpSocket->pendingDatagramSize());
    m_pUdpSocket->readDatagram(datagram.data(),datagram.size(), &address, &port);
    emit reply();
}

void UdpEchoClient::send()
{
    QLOG_INFO();
    m_pUdpSocket->writeDatagram(m_datagram.data(), m_datagram.size(), m_address, m_port);
}

#include "UdpEchoClient.moc"

}} // namespace stateforge { namespace network { 
