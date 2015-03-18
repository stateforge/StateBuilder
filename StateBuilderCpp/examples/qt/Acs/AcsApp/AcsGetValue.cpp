#include <iostream>
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#include "AcsGetValue.h"

using namespace std;

GetValueApp::GetValueApp(const QString &host, const QString &paramName) :
m_client(m_acs, host),
m_requestGetValue(m_client)
{
    QObject::connect(&m_requestGetValue, SIGNAL(end(bool)), 
                     this, SLOT(requestEnd(bool)));

    AcsParamList& paramList = m_requestGetValue.getParam();
    paramList.append(AcsParam(paramName, ""));

    m_client.start();
}

void GetValueApp::requestEnd(bool error)
{
   cout << "requestEnd" << endl;
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    
    QString host("192.168.1.254");
    QString paramName("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.1.CallingFeatures.");

    GetValueApp getValueApp(host, paramName);

    return app.exec();
}
