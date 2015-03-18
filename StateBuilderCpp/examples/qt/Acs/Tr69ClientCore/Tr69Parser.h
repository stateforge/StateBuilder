#ifndef TR69PARSER_H
#define TR69PARSER_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>

#include <QtSoapMessage>

#include "Tr69ParserBase.h"

/**
  @class Tr69Parser
*/
class Tr69Parser : public Tr69ParserBase
{
    Q_OBJECT

  public:
    Tr69Parser(QObject *pParent);
    ~Tr69Parser();

  public slots:
    void parse();
  
  Q_SIGNALS:

    void informResponseReceived(QtSoapMessage *pInformResponse);

  private:

    
    void parseInformResponse(QtSoapMessage &request);

};

#endif
