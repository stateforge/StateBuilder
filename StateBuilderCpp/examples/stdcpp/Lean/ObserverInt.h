#ifndef OBSERVERINT_H
#define OBSERVERINT_H

#include <fsm/State.h>
#include <fsm/IObserverInt.h>

class ObserverInt : public fsm::IObserverInt
{
public:
    ObserverInt();

    void OnEntry(int contextName, int stateId);

    void OnExit(int contextName, int stateId);

    void OnTransitionBegin(int contextName,
        int statePrevious,
        int stateNext,
        int transitionName);

    void OnTransitionEnd(int contextName,
        int statePrevious,
        int stateNext,
        int transitionId);

private:
    int _onEntryCount;
    int _onExitCount;
    int _transitionBeginCount;
    int _transitionEndCount;
};


#endif