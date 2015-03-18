#include <cstdlib>
#include <iostream>
#include <new>
#include <cstdlib>
#include <cassert>
#include "TestUtils.h"
#include "Action.h"
#include "LeanObserverNoFsm.h"

using namespace std;
using namespace carefusion;

void testWithObserverNo()
{
    Action action;
    LeanObserverNoContext context(action);
    context.EnterInitialState();
    processMachine(context);
}

