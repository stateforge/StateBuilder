#include <iostream>
#include <ObserverStd.h>
#include "LightPrivate.h"

using namespace std;

LightPrivate::LightPrivate()
{
}

LightPrivate::~LightPrivate()
{
}

void LightPrivate::doOn()
{
    cout << "doOn" << endl;
}

void LightPrivate::doOff()
{
    cout << "doOff" << endl;
}
