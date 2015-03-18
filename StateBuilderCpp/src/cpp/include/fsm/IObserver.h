//------------------------------------------------------------------------------
// <copyright file="IObserver.h" company="StateForge">
//      Copyright (c) 2010-2013 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#ifndef _FSM_OBSERVER_H_
#define _FSM_OBSERVER_H_

#include <cstddef>

#include <fsm/Definitions.h>

namespace fsm {


/** @class IObserver
 *  @brief IObserver is a abstract class use to observe the fsm.
 *         The  main purpose is to log changes inside the fsm
*/
class FSM_EXPORT IObserver {
  public:
    IObserver() {};
    virtual ~IObserver() {};
    virtual void OnTransitionBegin(const char * /*pContextName*/,
                                 const char * /*pStatePrevious*/,
                                 const char * /*pStateNext*/,
                                 const char * /*pTransitionName*/){}
    
    virtual void OnTransitionEnd(const char * /*pContextName*/,
                                  const char * /*pStatePrevious*/,
                                  const char * /*pStateNext*/,
                                  const char * /*pTransitionName*/){}

    virtual void OnTimerStart(const char * /*pContextName*/, const char * /*pTimerName*/, long /*duration*/){}
    virtual void OnTimerStop(const char * /*pContextName*/, const char * /*pTimerName*/){}
    virtual void OnEntry(const char * /*pContextName*/, const char * /*pStateName*/){}
    virtual void OnExit(const char * /*pContextName*/, const char * /*pStateName*/){}
    virtual void OnProcessEventStart(const char * /*pContextName*/, size_t /*remainingEvents*/){}
    virtual void OnProcessEventStop(const char * /*pContextName*/, size_t /*remainingEvents*/){}
};

}

#endif
