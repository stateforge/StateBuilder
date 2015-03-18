#ifndef OBSERVERSTRING_H
#define OBSERVERSTRING_H

#include <fsm/State.h>
#include <fsm/IObserver.h>

class ObserverString : public fsm::IObserver
{
public:
    ObserverString():
      _onEntryCount(0), _onExitCount(0),
          _transitionBeginCount(0), _transitionEndCount(0){}

      void OnEntry(const char */*pContextName*/, const char */*pStateName*/){_onEntryCount++;};
      void OnExit(const char */*pContextName*/, const char */*pStateName*/){_onExitCount++;};

      void OnTransitionBegin(const char */*pContextName*/,
          const char */*pStatePrevious*/,
          const char */*pStateNext*/,
          const char */*pTransitionName*/) {_transitionBeginCount++;}

      void OnTransitionEnd(const char */*pContextName*/,
          const char */*pStatePrevious*/,
          const char */*pStateNext*/,
          const char */*pTransitionName*/) {_transitionEndCount++;}

      int getOnEntryCount() const {return _onEntryCount;}
      int getOnExitCount() const {return _onExitCount;}

      int getTransitionBeginCount() const {return _transitionBeginCount;}
      int getTransitionEndCount() const {return _transitionEndCount;}

private:
    int _onEntryCount;
    int _onExitCount;
    int _transitionBeginCount;
    int _transitionEndCount;
};

#endif