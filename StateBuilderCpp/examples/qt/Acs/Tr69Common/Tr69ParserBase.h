#ifndef TR69PARSERBASE_H
#define TR69PARSERBASE_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QSharedPointer>

#include <QtSoapMessage>

/**
  * @class Tr69ParserBase
  * @brief This is a the base parser for both the client side parser Tr69Parser and the server side parser AcsParser.
*/
class Tr69ParserBase : public QObject
{
  Q_OBJECT
  
  public:
    
   typedef enum Error {
       ERROR_NO,
       ERROR_CONTENT_INVALID,
       ERROR_HEADER_INVALID,
       ERROR_ID_INVALID,
       ERROR_BODY_INVALID,
       ERROR_UNKNOWN_REQUEST
    } ErrorCode;
    
    Tr69ParserBase(QObject *pParent);
    virtual ~Tr69ParserBase();
    
    ErrorCode errorCode(){return m_eErrorCode;};
    
  public slots:
    virtual void parse() = 0;
    void reset();
    void accumulateData(const QByteArray &payload);
    
   Q_SIGNALS:
    void error();
    
  protected:
    
    void setError(ErrorCode errorCode);
    void parseSoapHeader();
    
    ErrorCode m_eErrorCode;
    QByteArray m_buffer;
    //QSharedPointer<QtSoapMessage> m_pSoapMessage;
    QtSoapMessage* m_pSoapMessage;
    
};

#endif // TR69PARSERBASE_H
