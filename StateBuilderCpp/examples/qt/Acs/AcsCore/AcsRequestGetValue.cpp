#include <QtCore/QDebug>

#include "AcsClient.h"
//#include "Observer.h"
#include "AcsRequestGetValue.h"
#include "Acs.h"
#include "AcsConnection.h"

/**
  @class AcsRequestGetValue
*/
AcsRequestGetValue::AcsRequestGetValue(AcsClient &client) : AcsRequest(client)
{
}

void AcsRequestGetValue::start(AcsConnection &connection)
{
  connection.sendRequestGetValue(getParam());
};


