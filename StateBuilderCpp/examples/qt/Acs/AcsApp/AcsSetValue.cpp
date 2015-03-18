#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtNetwork/QHostAddress>

#include "Acs.h"
#include "AcsRequestSetValue.h"
#include "AcsClient.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    Acs acs;
    QString host("192.168.1.254");

    AcsClient *pClient = new AcsClient(acs, host);

    AcsRequestSetValue *pRequestSetValue = new AcsRequestSetValue(*pClient);

    AcsParamList& paramList = pRequestSetValue->getParam();
    paramList.append(AcsParam("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.1.Line.1.CallingFeatures.CallWaitingEnable", "TRUE"));
    paramList.append(AcsParam("InternetGatewayDevice.Services.VoiceService.1.VoiceProfile.2.Line.1.CallingFeatures.CallWaitingEnable", "TRUE"));

    pClient->start();

    return app.exec();
}
