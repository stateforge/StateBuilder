#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include "Ping.h"
#include "UdpEchoServer.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    stateforge::network::Ping ping;
    QTimer::singleShot(0,&ping, SLOT(start()));

    stateforge::network::UdpEchoServer udpEchoServer;
    QTimer::singleShot(0,&udpEchoServer, SLOT(start()));

    return app.exec();
}
