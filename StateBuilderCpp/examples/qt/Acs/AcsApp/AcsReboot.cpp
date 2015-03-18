#include <iostream>
#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <QtNetwork/QHostAddress>

#include "Acs.h"
#include "AcsRequestReboot.h"
#include "AcsClient.h"

#include "AcsReboot.h"

using namespace std;

AcsRebootApp::AcsRebootApp(const QString &host) : AcsAppBase(host)
{
}


void AcsRebootApp::addRequest()
{
  m_acs.addRequestReboot(m_clientToken);
}

void usage()
{
    cout << "Usage: " << endl;
    cout << "  AcsReboot <target>" << endl;
    cout << "  example: AcsReboot 7.7.7.8" << endl;
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QString target;
    QStringList argumentList = QCoreApplication::arguments();

    if(argumentList.size () != 2){
      usage();
      return -1;
    }
    
    target = argumentList.at(1);
    AcsRebootApp rebootApp(target);
    rebootApp.addRequest();
    rebootApp.start();
    return app.exec();
}
