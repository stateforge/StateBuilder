//------------------------------------------------------------------------------
// <copyright file="IObserverInt.h" company="StateForge">
//      Copyright (c) 2010-2015 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#ifndef _FSM_OBSERVERINT_H_
#define _FSM_OBSERVERINT_H_

#include <cstddef>

#include <fsm/Definitions.h>

namespace fsm {


/** @class IObserverInt
 *  @brief IObserver is a abstract class use to observe the fsm with state as integer instead of string.
*/
class FSM_EXPORT IObserverInt {
  public:
    IObserverInt() {};
    virtual ~IObserverInt() {};
    virtual void OnTransitionBegin(int /*pContextName*/,
                                   int /*pStatePrevious*/,
                                   int /*pStateNext*/,
                                   int /*pTransitionName*/){}

    virtual void OnTransitionEnd(int /*pContextName*/,
                                   int /*pStatePrevious*/,
                                   int /*pStateNext*/,
                                   int /*pTransitionName*/){}

    virtual void OnTimerStart(int /*pContextName*/, int /*pTimerName*/, long /*duration*/){}
    virtual void OnTimerStop(int /*pContextName*/, int /*pTimerName*/){}
    virtual void OnEntry(int /*pContextName*/, int /*pStateName*/){}
    virtual void OnExit(int /*pContextName*/, int /*pStateName*/){}
    virtual void OnProcessEventStart(int /*pContextName*/, size_t /*remainingEvents*/){}
    virtual void OnProcessEventStop(int /*pContextName*/, size_t /*remainingEvents*/){}
};

}

#endif
