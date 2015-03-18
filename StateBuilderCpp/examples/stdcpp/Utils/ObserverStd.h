//------------------------------------------------------------------------------
// <copyright file="ObserverStd.h" company="StateForge">
//      Copyright (c) 2010-2013 StateForge.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------
#ifndef OBSERVERSTD_H
#define OBSERVERSTD_H

#include <string>
#include <fsm/Definitions.h>
#include <fsm/IObserver.h>

namespace fsm {
  
/**
ObserverStd implements fsm:IObserver, it logs all messages from the fsm to std::cout and std::cerr
@author frederic heem
 */
class FSM_EXPORT ObserverStd : public IObserver
{
  public:
    
    static ObserverStd& GetInstance();
    
    ~ObserverStd();

    void OnTransitionBegin(const char *pContextName,
                         const char *pStatePrevious,
                         const char *pStateNext,
                         const char *pTransitionName);
    
    void OnTransitionEnd(const char *pContextName,
                          const char *pStatePrevious,
                          const char *pStateNext,
                          const char *pTransitionName);
                        
    void OnTimerStart(const char *pContextName, const char *pTimerName, long duration);
    void OnTimerStop(const char *pContextName, const char *pTimerName);
    
    void OnEntry(const char *pContextName, const char *pStateName);
    void OnExit(const char *pContextName, const char *pStateName);

    void OnProcessEventStart(const char *pContextName, size_t remainingEvent);
    void OnProcessEventStop(const char *pContextName, size_t remainingEvent);
    
  protected:
    ObserverStd();
    // static class 
    static ObserverStd *m_pObserverStd;
};

} //namespace fsm {

#endif
