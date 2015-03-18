#include <cstring>
#include <fsm/IObserverInt.h>
#include "ObserverInt.h"


ObserverInt::ObserverInt() 
    :_onEntryCount(0), 
    _onExitCount(0),
    _transitionBeginCount(0),
    _transitionEndCount(0)
{
}

void ObserverInt::OnEntry(int contextId, int stateId)
{
    _onEntryCount++;
};

void ObserverInt::OnExit(int contextId, int stateId)
{
    _onExitCount++;
};

void ObserverInt::OnTransitionBegin(int contextId,
    int statePrevious,
    int stateNext,
    int transitionName) 
{
    _transitionBeginCount++;
}

void ObserverInt::OnTransitionEnd(int contextId,
    int statePrevious,
    int stateNext,
    int transitionName) 
{
    _transitionEndCount++;
}