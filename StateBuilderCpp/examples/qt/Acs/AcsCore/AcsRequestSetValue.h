#ifndef ACSREQUESTSETVALUE_H
#define ACSREQUESTSETVALUE_H

#include "AcsRequest.h"

class Acs;
class AcsConnection;
class AcsClient;

/**
  @class AcsRequestSetValue
*/
class AcsRequestSetValue : public AcsRequest
{
    Q_OBJECT

  public:
    AcsRequestSetValue(AcsClient &client);

    virtual ~AcsRequestSetValue(){};

    AcsParamList& getParam() { return m_paramList;}

    void start(AcsConnection &connection);

  private:
    AcsParamList  m_paramList;
};


#endif
