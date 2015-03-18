#include <cstdlib>
#include <iostream>
#include <new>
#include <cstdlib>
#include <cassert>
#include "TestUtils.h"
#include "Action.h"
#include "ObserverInt.h"
#include "LeanObserverIntFsm.h"

using namespace std;
using namespace carefusion;

void testWithObserverInt()
{
    Action action;
    LeanObserverIntContext context(action);
    context.SetId(1);
    ObserverInt observer;
    context.SetObserverInt(observer);
    context.EnterInitialState();
    processMachine(context);
}

