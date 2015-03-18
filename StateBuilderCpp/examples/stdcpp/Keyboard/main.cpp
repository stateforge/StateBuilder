#include <cstdlib>
#include <iostream>
#include <new>
#include <cassert>
#include <ObserverStd.h>
#include "Action.h"
#include "KeyboardFsm.h"

using namespace std;

int main(int /*argc*/, char ** /*argv[]*/)
{
  Action action;
  // The generated state machine context
  KeyboardContext context(action);
  // Set the standard observer which output on the standard output
  context.SetObserver(fsm::ObserverStd::GetInstance());
  //Call All OnEntry from the root state to the initial state, e.g the first leaf
  context.EnterInitialState();
  // Send the event
  context.evAnyKey();
  context.evCapslock();
  context.evAnyKey();
  context.evCapslock();
  
  context.evNumKey();
  context.evNumLock();
  context.evNumKey();
  context.evNumLock();
  return 0;
}
