#include <iostream>
#include <cstring>
#include <cstdlib>
#include <assert.h>
#include "Action.h"
#include "SillyFsm.h"

using namespace std;

int main(int /*argc*/, char ** /*argv[]*/)
{
	Action action;
	SillyContext context(action);
	context.evStart();
    context.evBringDown();
    context.evBringDown();
    context.evLiftUp();
    context.evLiftUp();
    context.evTurnRight();
    context.evPaint(BLUE);
    context.evTurnLeft();
    context.evPaint(RED);
    context.evPaint(BLUE);
 
}
