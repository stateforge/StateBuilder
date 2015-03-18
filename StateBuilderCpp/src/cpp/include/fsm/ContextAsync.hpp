//------------------------------------------------------------------------------
// <copyright file="ContextAsync.hpp" company="StateForge">
//      Copyright (c) 2010-2013 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#ifndef FSM_CONTEXTASYNC_HPP
#define FSM_CONTEXTASYNC_HPP


#include <cassert> // assert
#include <list>   // std::list
#include <algorithm> //std::remove
#include <memory> // std::auto_ptr
#include <limits>
#include <fsm/ContextAsync.h>
#include <fsm/Definitions.h>
#include <fsm/Event.h>
#include <fsm/Timer.h>
#include <fsm/IObserver.h>

namespace fsm
{  
    
template<class TState, class TContext, class TContextParent>
ContextAsync<TState, TContext, TContextParent>::ContextAsync(ISchedule &schedule,
                                                             TContextParent *pContextParent)
  : Context<TState, TContextParent>(pContextParent)
  , m_schedule(schedule)
{
}

template<class TState, class TContext, class TContextParent>
ContextAsync<TState, TContext, TContextParent>::~ContextAsync()
{
  while(m_eventList.size() > 0) {
      std::auto_ptr<IEvent<TContext> > pEvent(m_eventList.front());
      m_eventList.pop_front();
  };
}

#ifdef FSM_TIMERS_INTERNAL
template<class TState, class TContext, class TContextParent>
bool ContextAsync<TState, TContext, TContextParent>::TimerStart(const string &name, 
                                                                         long lDuration)
{
  ContextBase::GetObserver().OnTimerStart(name, lDuration);
  return m_timerList.Start(name, lDuration);
}

template<class TState, class TContext, class TContextParent>
bool ContextAsync<TState, TContext, TContextParent>::TimerStop(const string &name)
{
  ContextBase::GetObserver().OnTimerStop(name);
  return m_timerList.Stop(name);
}
#endif //#ifdef FSM_TIMERS_INTERNAL

template<class TState, class TContext, class TContextParent>
ProcessReturn ContextAsync<TState, TContext, TContextParent>::ProcessEvent(long &nextTick){
  ContextAsync<TState, TContext, TContextParent>::Lock();
  
  while(m_eventList.size() > 0) {
    // since dispatch can throw an exception, the event is wrapped inside an auto_ptr 
    // which takes care of deleting the event when it goes out of scope 
    if(ContextAsync<TState, TContext, TContextParent>::m_pObserver){
      ContextAsync<TState, TContext, TContextParent>::m_pObserver->OnProcessEventStart(Context<TState, TContextParent>::GetName(), m_eventList.size());
    }
    std::auto_ptr<IEvent<TContext> > pEvent(m_eventList.front());
    m_eventList.pop_front();
    ContextAsync<TState, TContext, TContextParent>::UnLock();
    //dispatch can add an event to the list
    pEvent->dispatch();
    ContextAsync<TState, TContext, TContextParent>::Lock();
    if(ContextAsync<TState, TContext, TContextParent>::m_pObserver){
      ContextAsync<TState, TContext, TContextParent>::m_pObserver->OnProcessEventStop(Context<TState, TContextParent>::GetName(), m_eventList.size());
    }
  };

  ContextAsync<TState, TContext, TContextParent>::UnLock();

  bool bError = false;
  // Process all orthognal states
  Parallel *pParallel = Context<TState, TContextParent>::GetParallel();
  if(pParallel != NULL){
    long nextTickParallel = nextTick;
    if(pParallel->ProcessEvent(nextTickParallel) == ProcessError){
      bError = true;
    }
    nextTick = (std::min)(nextTickParallel, nextTick);
  }

  //Process timers
  #ifdef FSM_TIMERS_INTERNAL
  m_timerList.Process(nextTick);
  #endif //#ifdef FSM_TIMERS_INTERNAL

  //Return depending on the new state
  if((Context<TState, TContextParent>::GetState().getKind() == STATE_ERROR) || 
      (bError == true)){
    return ProcessError;
  } else if(Context<TState, TContextParent>::GetState().getKind() == STATE_FINAL){
    return ProcessEnd;
  } else {
    return ProcessRunning;
  }
}


} // namespace fsm

#endif 
