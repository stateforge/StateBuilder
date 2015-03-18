
#include <iostream>
#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <QtNetwork/QHostAddress>

#include "Acs.h"
#include "AcsRequestDownload.h"
#include "AcsDownload.h"

AcsDownloadApp::AcsDownloadApp(const QString &host) : AcsAppBase(host)
{
}

void AcsDownloadApp::addRequest(AcsParamDownload &paramDownload)
{
  m_acs.addRequestDownload(m_clientToken, paramDownload);
}

using namespace std;

void usage()
{
    cout << "Usage: " << endl;
    cout << "  AcsDownload <target> <filename> <url>" << endl;
    cout << "From Wan: " << endl;
    cout << "  AcsDownload 7.7.7.8 firmware  http://7.7.7.3:8080" << endl;
    cout << "From Lan: " << endl;
    cout << "  AcsDownload 192.168.1.254 firmware  http://192.168.1.100:80" << endl;
}

int main(int argc, char** argv)
{
    QString target, firmwareFilename, downloadUrl;


    QCoreApplication app(argc, argv);
    QStringList argumentList = QCoreApplication::arguments();

    if(argumentList.size () != 4){
      usage();
      return -1;
    }

    target = argumentList.at(1);
    firmwareFilename = argumentList.at(2);
    downloadUrl = argumentList.at(3);

    AcsDownloadApp downloadApp(target);
    
    AcsParamDownload paramDownload;
    paramDownload.firmwareFilename = firmwareFilename;
    paramDownload.url = downloadUrl + '/' + paramDownload.firmwareFilename;
    
    downloadApp.addRequest(paramDownload);
    downloadApp.start();

    return app.exec();
}
