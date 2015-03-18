

#include <QtNetwork/QHttpResponseHeader>
#include <QtSoapMessage>

#include <QsLog.h>

#include "AcsMessageBuilder.h"
#include "AcsRequestDownload.h"

/**
@class AcsMessageBuilder
*/
AcsMessageBuilder::AcsMessageBuilder(QObject *pParent) : QObject(pParent) ,
m_nsPrefix(QString(NS_PREFIX) + ':')
{
    QLOG_DEBUG();

}

AcsMessageBuilder::~AcsMessageBuilder()
{
    QLOG_DEBUG();
}

void AcsMessageBuilder::buildHttpHeaderResponse(QHttpResponseHeader &responseHeader,
                                            int iContentLength,
                                            int code, 
                                            const QString& status)
{
    responseHeader.setStatusLine(code, status);
    responseHeader.setContentLength(iContentLength);
    if(iContentLength == 0){
        responseHeader.setContentType("text/plain");
    } else {
        responseHeader.setContentType("text/xml; charset=\"utf-8\"");
    }
    return;
}

void AcsMessageBuilder::buildEnvelope(QtSoapMessage &message)
{
    message.namespaceAdd(NS_PREFIX, NS);
}

void AcsMessageBuilder::buildRebootRequest(QtSoapMessage &request)
{
    //Envelope
    buildEnvelope(request);

    //Header  
    QString id = "987654321";
    request.header().insert(new QtSoapSimpleType(QtSoapQName(nsPrefix() + TAG_ID), id));

    //Body
    QtSoapStruct *pInformResponseStruct = new QtSoapStruct(QtSoapQName(nsPrefix() + TAG_REBOOT));
    pInformResponseStruct->insert(new QtSoapSimpleType(QtSoapQName(TAG_COMMAND_KEY), "Reboot Method (ACS)"));

    QtSoapStruct& body = request.body();
    body.insert(pInformResponseStruct);
}

void AcsMessageBuilder::buildDownloadRequest(QtSoapMessage &request, AcsParamDownload &paramDownload)
{
    //Envelope
    buildEnvelope(request);

    //Header  
    QString id = "987654321";
    request.header().insert(new QtSoapSimpleType(QtSoapQName(nsPrefix() + TAG_ID), id));

    //Body
    QtSoapStruct *pInformResponseStruct = new QtSoapStruct(QtSoapQName(nsPrefix() + TAG_DOWNLOAD));
    pInformResponseStruct->insert(new QtSoapSimpleType(QtSoapQName(TAG_COMMAND_KEY), TAG_DOWNLOAD_ACS));
    pInformResponseStruct->insert(new QtSoapSimpleType(QtSoapQName(TAG_FILE_TYPE), TAG_FIRMWARE_UPGRADE_IMAGE));
    pInformResponseStruct->insert(new QtSoapSimpleType(QtSoapQName(TAG_URL), paramDownload.url));
    pInformResponseStruct->insert(new QtSoapSimpleType(QtSoapQName(TAG_USERNAME), paramDownload.username));
    pInformResponseStruct->insert(new QtSoapSimpleType(QtSoapQName(TAG_PASSWORD), paramDownload.password));
    pInformResponseStruct->insert(new QtSoapSimpleType(QtSoapQName(TAG_FILE_SIZE), 0));
    pInformResponseStruct->insert(new QtSoapSimpleType(QtSoapQName(TAG_TARGET_FILE_NAME), paramDownload.targetFilename));

    QtSoapStruct& body = request.body();
    body.insert(pInformResponseStruct);
}

void AcsMessageBuilder::buildInformResponse(const QString &id, QtSoapMessage &informResponse)
{
    //Envelope
    buildEnvelope(informResponse);

    //Header
    informResponse.header().insert(new QtSoapSimpleType(QtSoapQName(nsPrefix() + TAG_ID), id));

    //Body
    QtSoapStruct *pInformResponseStruct = new QtSoapStruct(QtSoapQName(nsPrefix() + INFORMRESPONSE));
    pInformResponseStruct->insert(new QtSoapSimpleType(QtSoapQName(TAG_MAXENVELOPES), "1"));

    QtSoapStruct& body = informResponse.body();
    body.insert(pInformResponseStruct);
}

void AcsMessageBuilder::buildGetParameterNamesRequest(QtSoapMessage &request)
{
    //Envelope
    buildEnvelope(request);

    //Header  
    QString id = "123456";
    request.header().insert(new QtSoapSimpleType(QtSoapQName(nsPrefix() + TAG_ID), id));

    //Body
    QString path = "InternetGatewayDevice.";
    QtSoapStruct *pGetParameterNamesStruct = new QtSoapStruct(QtSoapQName(nsPrefix() + TAG_GETPARAMETERNAMES));
    pGetParameterNamesStruct->insert(new QtSoapSimpleType(QtSoapQName("ParameterPath"), path));
    pGetParameterNamesStruct->insert(new QtSoapSimpleType(QtSoapQName("NextLevel"), 1));

    QtSoapStruct& body = request.body();
    body.insert(pGetParameterNamesStruct);
}


void AcsMessageBuilder::buildSetValueRequest(QtSoapMessage &request, AcsParamList &paramList)
{
    //Envelope
    buildEnvelope(request);

    //Header  
    QString id = "123458";
    request.header().insert(new QtSoapSimpleType(QtSoapQName(nsPrefix() + TAG_ID), id));

    //Body
    QtSoapStruct *pSetParameterValuesStruct = new QtSoapStruct(QtSoapQName(nsPrefix() + TAG_SETPARAMETERVALUES));

    QtSoapStruct *pParameterListStruct = new QtSoapStruct(QtSoapQName(TAG_PARAMETERLIST));
    pSetParameterValuesStruct->insert(pParameterListStruct);

    foreach(AcsParam param, paramList){
        QtSoapStruct *pParameterValueStruct = new QtSoapStruct(QtSoapQName(TAG_PARAMETERVALUESTRUCT));

        pParameterValueStruct->insert(new QtSoapSimpleType(QtSoapQName(TAG_NAME), param.name));
        pParameterValueStruct->insert(new QtSoapSimpleType(QtSoapQName(TAG_VALUE), param.value));

        pParameterListStruct->insert(pParameterValueStruct);
    }

    QtSoapStruct& body = request.body();
    body.insert(pSetParameterValuesStruct);
}

void AcsMessageBuilder::buildGetValueRequest(QtSoapMessage &request, AcsParamList &paramList)
{
    //Envelope
    buildEnvelope(request);

    //Header  
    QString id = "1234583";
    request.header().insert(new QtSoapSimpleType(QtSoapQName(nsPrefix() + TAG_ID), id));

    //Body
    QtSoapStruct *pGetParameterValuesStruct = new QtSoapStruct(QtSoapQName(nsPrefix() + TAG_GETPARAMETERVALUES));

    QtSoapStruct *pParameterListStruct = new QtSoapStruct(QtSoapQName(nsPrefix() + TAG_PARAMETERNAMES));
    pGetParameterValuesStruct->insert(pParameterListStruct);

    foreach(AcsParam param, paramList){
        pParameterListStruct->insert(new QtSoapSimpleType(QtSoapQName(TAG_STRING), param.name));
    }

    QtSoapStruct& body = request.body();
    body.insert(pGetParameterValuesStruct);
}

void AcsMessageBuilder::buildTransferCompleteResponse(QtSoapMessage &response)
{
    //Envelope
    buildEnvelope(response);

    //Body
    QtSoapStruct *pTranferCompleteResponseStruct = new QtSoapStruct(QtSoapQName(nsPrefix() + TAG_TRANSFER_COMPLETE_RESPONSE));

    QtSoapStruct& body = response.body();
    body.insert(pTranferCompleteResponseStruct);
}


