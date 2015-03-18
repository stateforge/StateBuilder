#ifndef ACSPARSER_H
#define ACSPARSER_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>

#include <QtSoapMessage>

#include "Tr69ParserBase.h"

/**
  @class AcsParser
*/
class AcsParser : public Tr69ParserBase
{
    Q_OBJECT

  public:
    AcsParser(QObject *pParent);
    ~AcsParser();

  public slots:
    void parse();
    
  Q_SIGNALS:
    void informRequestReceived(QtSoapMessage *pInformRequest);

  private:
      
    void parseInformRequest(QtSoapMessage *pRequest);

};

#endif
