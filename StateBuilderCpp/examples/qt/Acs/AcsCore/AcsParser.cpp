#include <QtSoapMessage>
#include <QsLog.h>

#include "AcsParser.h"
#include <Tr69Tag.h>

/**
  * @class AcsParser
  * @brief AcsParser decodes server side tr069 messages: InformRequest etc ...
*/
AcsParser::AcsParser(QObject *pParent) : Tr69ParserBase(pParent)
{
    QLOG_DEBUG();
}

AcsParser::~AcsParser()
{
    QLOG_DEBUG();
}

void AcsParser::parse()
{
    QLOG_DEBUG();

    parseSoapHeader();
    
    QtSoapStruct& body = m_pSoapMessage->body();
    
    if(body[Tr69Tag::TAG_INFORM].isValid()){
        parseInformRequest(m_pSoapMessage);
    } else if(body[Tr69Tag::TAG_GETPARAMETERNAMESRESPONSE].isValid()){
        //onGetNameResponse(pRxMessage);
    } else if(body[Tr69Tag::TAG_SETPARAMETERVALUESRESPONSE].isValid()){
        //onSetValueResponse(pRxMessage);
    } else if(body[Tr69Tag::TAG_GETPARAMETERVALUESRESPONSE].isValid()){
        //onGetValueResponse(pRxMessage);
    } else if(body[Tr69Tag::TAG_TRANSFER_COMPLETE].isValid()){
        //onTransferComplete(pRxMessage);
    } else if(body[Tr69Tag::TAG_DOWNLOAD_RESPONSE].isValid()){
        //onDownloadResponse(pRxMessage);
    } else if(body[Tr69Tag::TAG_REBOOT_RESPONSE].isValid()){
        //onRebootResponse(pRxMessage);
    } else if(m_pSoapMessage->isFault()) {
        QLOG_WARN() << "Error: " << m_pSoapMessage->faultString().toString();
        //onFault(pRxMessage);
    } else {
        setError(ERROR_UNKNOWN_REQUEST);
        
    }    
}

void AcsParser::parseInformRequest(QtSoapMessage *pInformRequest)
{
    QLOG_DEBUG();
    // TODO HEEFRE
    //QtSoapStruct& body = pInformRequest->body();

    emit informRequestReceived(pInformRequest);
}



