#include <QtSoapMessage>
#include <QsLog.h>

#include "Tr69Parser.h"
#include <Tr69Tag.h>

/**
@class Tr69Parser
*/
Tr69Parser::Tr69Parser(QObject *pParent) : Tr69ParserBase(pParent)

{
    QLOG_DEBUG();
}

Tr69Parser::~Tr69Parser()
{
    QLOG_DEBUG();
}

void Tr69Parser::parse()
{
    QLOG_DEBUG();

    parseSoapHeader();
    
    QtSoapStruct& body = m_pSoapMessage->body();
    
    if(body[Tr69Tag::INFORMRESPONSE].isValid()){
        parseInformResponse(*m_pSoapMessage);
    } else {
        setError(ERROR_UNKNOWN_REQUEST);
    } 
}

void Tr69Parser::parseInformResponse(QtSoapMessage &informResponse)
{
    QLOG_DEBUG();
    // TODO HEEFRE
    //QtSoapStruct& body = informResponse.body();

    emit informResponseReceived(&informResponse);
}



