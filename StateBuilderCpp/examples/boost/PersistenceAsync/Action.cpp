#include <iostream>
#include <logging.hpp>
#include <SmBoostObserver.h>

#include "Action.h"

using namespace std;

Action::Action()
{
}

Action::~Action()
{
}

void Action::Print(const std::string &message)
{
    cout << message << endl;
}
