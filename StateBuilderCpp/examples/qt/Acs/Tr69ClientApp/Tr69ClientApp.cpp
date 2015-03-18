#include <iostream>
#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <QtNetwork/QHostAddress>

#include "Tr69ClientApp.h"
#include "Tr69Client.h"

using namespace std;

Tr69ClientApp::Tr69ClientApp()
{
    m_pTr69Client = new Tr69Client();
}


void Tr69ClientApp::start()
{

}

void usage()
{
    cout << "Usage: " << endl;
    cout << "  Tr69ClientApp" << endl;
    cout << "  example: Tr69ClientApp" << endl;
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
    
    Tr69ClientApp tr69ClientApp;
    tr69ClientApp.start();
    return app.exec();
}
