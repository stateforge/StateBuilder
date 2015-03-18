#include <cstdlib>
#include <iostream>
#include <ObserverStd.h>
#include "Persistence.h"


using namespace std;


Action::Action()
{
    cout << "Action ctor" << endl;
}

Action::~Action()
{
     cout << "Action dtor" << endl;
}

void Action::print(const std::string &message)
{
    cout << message << endl;
}