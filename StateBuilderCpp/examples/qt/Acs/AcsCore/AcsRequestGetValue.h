#ifndef ACSREQUESTGETVALUE_H
#define ACSREQUESTGETVALUE_H

#include "AcsRequest.h"

class Acs;
class AcsConnection;
class AcsClient;


/**
  @class AcsRequestGetValue
*/
class AcsRequestGetValue : public AcsRequest
{
    Q_OBJECT

  public:
    AcsRequestGetValue(AcsClient &client);

    virtual ~AcsRequestGetValue(){};

    AcsParamList& getParam() { return m_paramList;}

    void start(AcsConnection &connection);

  private:
    AcsParamList  m_paramList;
};


#endif
