#include <QtCore/QDebug>

#include "AcsClient.h"
//#include "Observer.h"
#include "AcsRequestSetValue.h"
#include "Acs.h"
#include "AcsConnection.h"

/**
  @class AcsRequestSetValue
*/
AcsRequestSetValue::AcsRequestSetValue(AcsClient &client) : AcsRequest(client)
{
}

void AcsRequestSetValue::start(AcsConnection &connection)
{
  connection.sendRequestSetValue(getParam());
};


