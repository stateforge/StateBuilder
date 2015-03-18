#include <QUdpSocket>

#include <QsLog.h>
#include <QsLogDest.h>

#include "UdpEchoServer.h"

namespace stateforge { namespace network { 

UdpEchoServer::UdpEchoServer() :
m_pUdpSocket(new QUdpSocket(this)),
m_port(2011)
{
    
    connect(m_pUdpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

UdpEchoServer::~UdpEchoServer()
{
}

bool UdpEchoServer::start()
{
    QLOG_INFO();
    if(m_pUdpSocket->bind(m_port, QUdpSocket::ShareAddress) == false){
        QLOG_ERROR() << "Cannot listen on port " << m_port << ", error " << m_pUdpSocket->errorString();
        return false;
    } else {
        return true;
    }
}

void UdpEchoServer::processPendingDatagrams(){
    while (m_pUdpSocket->hasPendingDatagrams()){
        QByteArray datagram;
        QHostAddress address;
        quint16 port;
        datagram.resize(m_pUdpSocket->pendingDatagramSize());
        m_pUdpSocket->readDatagram(datagram.data(), datagram.size(), &address, &port);
        quint64 written = m_pUdpSocket->writeDatagram(datagram.data(), datagram.size(), address, port);
        QLOG_DEBUG() << "written " << written << " bytes to " << address.toString() << ":" << port;
    }
}

#include "UdpEchoServer.moc"

}} // namespace stateforge { namespace network { 
