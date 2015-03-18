#include <cstdlib>
#include <iostream>
#include <new>
#include <cstdlib> // for malloc() and free()
#include <cassert>

#include "TestUtils.h"
#include "TestObserverInt.h"
#include "TestObserverNo.h"
#include "Action.h"
#include "IdObserver.h"
#include "ObserverString.h"
#include "LeanFsm.h"

using namespace std;
using namespace carefusion;

static unsigned gAllocCount = 0;

// overide new and delete to set breakpoint. Make sure no dynamic allocation is done
void* operator new (size_t size)
{
    gAllocCount++;
    void *p = malloc(size); 
    return p;
}

void operator delete (void *p)
{
    free(p); 
}

void testWithObserverToInt()
{
    Action action;
    LeanContext context(action);
    context.EnterInitialState();
    IdObserver observer(LeanContext::GetStateNameToId());
    context.SetObserver(observer);
    processMachine(context);
}

void testWithObserverString()
{
    Action action;
    LeanContext context(action);
    context.EnterInitialState();
    ObserverString observer;
    context.SetObserver(observer);
    processMachine(context);
}

void testFunctional()
{
    int iActionMax = 1000000;
    Action action(iActionMax);
    LeanContext context(action);
    ObserverString observer;
    context.SetObserver(observer);
    //Call All OnEntry from the root state to the initial state, e.g the first leaf state S1
    context.EnterInitialState();
    // Root state OnEntry + S1 OnEntry
    assert(observer.getOnEntryCount() == 2);
    assert(observer.getOnExitCount() == 0);
    //Action::Off has been called in S1::OnEntry
    assert(action.GetActionCount() == 1);
    assert(observer.getTransitionBeginCount() == 0);

    processMachine(context);

    assert(observer.getOnEntryCount() == (action.GetActionCount() + 1 /*Root::OnEntry*/+ 1 /*End::OnEntry*/));
    assert(observer.getOnExitCount() == action.GetActionCount());
    assert(observer.getTransitionBeginCount() == action.GetActionCount());
    assert(observer.getTransitionEndCount() == action.GetActionCount());
    assert(action.GetActionCount() >= iActionMax);
}

int main(int /*argc*/, char ** /*argv[]*/)
{
    int iReturnValue = 0;
    // Action is given the state machine context to execute the action. Mandatory
    testFunctional();
    testWithObserverToInt();
    testWithObserverString();
    testWithObserverInt();
    testWithObserverNo();

    //No dymanic allocation. Microsoft c++ lib allocates memory even for an empty std::list 
#ifndef _MSC_VER
    assert(gAllocCount == 0);
#endif

    return iReturnValue;
}
